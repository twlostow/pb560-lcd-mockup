#!/usr/bin/python3

import sys
import glob
import re
from PIL import Image

class Glyph:
    def __init__(self, ch, w, h,  ix, iy, xoffset, yoffset, xadvance, image ):
        self.ch = ch
        self.w = w
        self.h = h
        self.ix = ix
        self.iy = iy
        self.xoffset = xoffset
        self.yoffset = yoffset
        self.xadvance = xadvance
        self.data=[0] * (w * (h + 10))
        self.image = image

    def convert(self):
        for y in range(0,self.h):
            for x in range(0,self.w):
                p = self.image[(x+self.ix,y+self.iy)][3]
                if p:
                    self.data[self.w*y+x] = 1
        pass


class Font:
    def __init__(self, name, size):
        self.name = name
        self.size = size
        self.glyphs=[]
        self.lineHeight = 0
        self.base = 0

    def setLineHeight(self, x):
        self.lineHeight = x
    
    def setBase(self,x ):
        self.base = x
    
    def addGlyph(self, glyph):
        self.glyphs += [ glyph ]

    def write( self, filename ):
        f = open("font_%s_%d.c" % (self.name, self.size), "w")
        f.write("#include <stdint.h>\n")
        f.write("const uint8_t font_data_%s_%d[] = {" % (self.name, self.size) )
        f.write("0x%02x, 0x%02x, " % ( self.base, self.lineHeight ));
        offset = 2
        glyph_offsets={}
        for g in self.glyphs:
            glyph_offsets[g.ch] = offset
            f.write("0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x, \n" % (g.w, g.h, (g.xoffset) & 0xff, (g.yoffset) & 0xff, (g.xadvance) & 0xff) )
            offset += 5
            data_len = (g.w*g.h+7)//8
            for i in range(0, data_len):
                mask = 0
                for k in range(0, 8):
                    if g.data[i*8+k]:
                        mask |= (1<<k)
                f.write("0x%02x, " % mask)
            offset += data_len
            f.write("\n")
        f.write("0};\n")
        f.write("const uint16_t font_offsets_%s_%d[] = {\n" % (self.name, self.size))
        for c in range(0, 256):
            if c in glyph_offsets:
                f.write("%d, // char '%c'\n" % (glyph_offsets[c], chr(c)))
            else:
                f.write("%d,\n" % 0xffff)
        f.write("0};\n")
        f.close()
                



image = None
for file in glob.glob("fonts/*.fnt"):
    for l in open(file,"rb").readlines():
        toks = l.split()
        cmd = toks[0]
        fields = {}

        for t in toks[1:]:
            s = t.split(b'=')
            fieldname = s[0].decode('utf-8')
            value = s[1].lstrip(b'"').rstrip(b'"').decode('utf-8')
            print(value, type(value))
            #print(s)
            fields[fieldname] = value

        #for f in fields:
            #print(f)
        #print(cmd)
        if cmd == b"info":
            font = Font( str( fields['face'].lstrip('"').rstrip('"')), int(fields['size']))
        if cmd == b"common":
            font.setBase( int( str(fields['base'] ) ) )
            font.setLineHeight (int( str(fields['lineHeight'] ) ) )
        if cmd == b"page":
            image = Image.open("fonts/"+fields["file"])
            #print(image.mode)
            pixels = image.load()
        if cmd == b"char":
            g = Glyph( int(fields['id']),
                       int(fields['width']),
                       int(fields['height']),
                       int(fields['x']),
                       int(fields['y']),
                       int(fields['xoffset']),
                       int(fields['yoffset']),
                       int(fields['xadvance']),
                       pixels );
            g.convert()
            font.addGlyph(g)
        if cmd == b"kerning":
            font.write('x')
            break

    #print('Dupa')
