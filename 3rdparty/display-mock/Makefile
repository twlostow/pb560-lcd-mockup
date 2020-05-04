include ../../common.mk

PB560_SRCS = ../../SRC
INCLUDE_PATH=-I$(PB560_SRCS)/GENERAL \
	     -I$(PB560_SRCS)/HMI \
		 -I$(PB560_SRCS)/DATABASE \
		 -I$(PB560_SRCS)/DISPLAY \
		 -I$(PB560_SRCS)/DRIVERS_DISPLAY \
		 -I$(PB560_SRCS)/DRIVERS \
		 -I$(PB560_SRCS)/SYSTEM

CXX = g++

CXXFLAGS += -I. -g `pkg-config sdl --cflags` $(INCLUDE_PATH)

LDFLAGS = -g -m32 -L$(PB560_SRCS)/DATABASE -lDATABASE \
		  -L$(PB560_SRCS)/DISPLAY -lDISPLAY \
          -L$(PB560_SRCS)/HMI -lHMI \
		  -L$(PB560_SRCS)/DATABASE -L$(PB560_SRCS)/DRIVERS_DISPLAY -lDATABASE -lDRIVERS_DISPLAY \
		  -L$(PB560_SRCS)/DATABASE -lDATABASE \
		  -L$(PB560_SRCS)/DISPLAY -lDISPLAY \
          -L$(PB560_SRCS)/HMI -lHMI \
		  -L$(PB560_SRCS)/SYSTEM -lSYSTEM \
		  -L$(PB560_SRCS)/DATABASE -L$(PB560_SRCS)/DRIVERS_DISPLAY -lDATABASE -lDRIVERS_DISPLAY \
		/usr/lib/i386-linux-gnu/libSDL_gfx.so.5 \
		/usr/lib/i386-linux-gnu/libSDL-1.2.so.0

 # -lSDL -lX11

OBJS=sdl_view.o display-mock.o

CC = gcc
CFLAGS = += -I. 

all:	$(OBJS) 
	${CXX} -m32 -o display-mock $(OBJS) $(LDFLAGS)

%.o:	%.c
	${CC} ${CFLAGS} -c $^ -o $@

%.o:	%.cpp
	${CXX} ${CXXFLAGS} -c $^ -o $@

clean:
		rm -f $(OBJS)
