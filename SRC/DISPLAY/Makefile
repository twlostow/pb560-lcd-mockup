include ../../common.mk

AR = ar

OUTPUT= libDISPLAY.a

OBJS = \
DIS_Backlight.o \
DIS_Box.o \
DIS_DisplayPage.o \
DIS_ErasePage.o \
DIS_EraseZone.o \
DIS_FifoRead.o \
DIS_FifoWrite.o \
DIS_Line.o \
DIS_LoadTable.o \
DIS_NegativeCar.o \
DIS_Number.o \
DIS_Pixel.o \
DIS_String.o \
DIS_Symbol.o


all: 	$(OBJS)
	rm -f $(OUTPUT)
	${AR} rcs $(OUTPUT) $(OBJS)

%.o:	%.o \
	${CC} ${CFLAGS} -c $^

clean:
		rm -f $(OBJS) $(OUTPUT)

