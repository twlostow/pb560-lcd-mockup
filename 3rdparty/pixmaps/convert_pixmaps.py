#!/usr/bin/python3

import sys
import glob
import re
from PIL import Image

class Pixmap:
    def __init__(self, w, h, image, name ):
        self.w = w
        self.h = h
        self.data=[0] * (w * (h + 10))
        self.image = image
        self.name = name

    def convert(self):
        for y in range(0,self.h):
            for x in range(0,self.w):
                p = self.image[(x,y)]
                if type(p) == int:
                    v = p
                else:
                    v = p[0]
                if v:
                    self.data[self.w*y+x] = 1
        pass


class PixmapSet:
    def __init__(self):
        self.pixmaps=[]
    
    def addPixmap(self, pixmap):
        self.pixmaps += [ pixmap ]

    def write( self, filename ):
        f = open(filename, "w")
        f.write("#include <stdint.h>\n")
        f.write("#include \"pixmap.h\"\n")

        for p in self.pixmaps:
            f.write("const uint8_t pixmap_data_%s[] = {" % p.name )
            f.write("0x%02x, 0x%02x, \n" % (p.w, p.h) )
            data_len = (p.w*p.h+7)//8
            for i in range(0, data_len):
                mask = 0
                for k in range(0, 8):
                    if p.data[i*8+k]:
                        mask |= (1<<k)
                f.write("0x%02x, " % mask)
            f.write("0};\n")
        
        f.write("\n\nconst struct gfx_pixmap builtin_pixmaps[] = {")
        for p in self.pixmaps:
            f.write("  {%s, pixmap_data_%s},\n" % ( p.name, p.name) )
        f.write("  {0, 0}};\n" )
        f.close();


pixmap_names = {}

pset = PixmapSet()


for l in open("pixmaps.list","r").readlines():
    pixmap_names[ l.rstrip('\n').upper() ] = l.rstrip('\n')

print(pixmap_names)

image = None
for filename in glob.glob("*"):
    toks = filename.split(".")
    name = toks[0].upper()
    if toks[1].upper() in ["PNG","BMP"] and name in pixmap_names.keys():
        print("Process %s" % filename)

        image = Image.open(filename)
        #print(image.size[])
        pixels = image.load()
        pixmap = Pixmap( image.size[0], image.size[1], pixels, pixmap_names[name] )
        pixmap.convert()
        pset.addPixmap( pixmap )
        
pset.write("pixmaps.c")


    #print('Dupa')
