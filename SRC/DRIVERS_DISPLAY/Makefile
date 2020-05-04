include ../../common.mk

OUTPUT= libDRIVERS_DISPLAY.a

OBJS = DRV_DIS_Delay_ms.o \
DRV_DIS_Delay_us.o \
DRV_DIS_grAbout.o \
DRV_DIS_grBackLight.o \
DRV_DIS_grCar.o \
DRV_DIS_grCls.o \
DRV_DIS_grControlPage.o \
DRV_DIS_grFullRect.o \
DRV_DIS_grLine.o \
DRV_DIS_grNegative.o \
DRV_DIS_grPutStamp.o \
DRV_DIS_grRect.o \
DRV_DIS_grReset.o \
DRV_DIS_grSetColor.o \
DRV_DIS_grSetContrast.o \
DRV_DIS_grSetPage.o \
DRV_DIS_grSetPixel.o \
DRV_DIS_grSetShutdown.o \
DRV_DIS_grString.o \
DRV_DIS_grTestFont.o \
DRV_DIS_grTestStamp.o \
DRV_DIS_grWriteDisplay.o \
DRV_DIS_HostRequest.o

all: 	$(OBJS)
	rm -f $(OUTPUT)
	${AR} rcs $(OUTPUT) $(OBJS)

%.o:	%.c
	${CC} ${CFLAGS} -c $^

clean:
		rm -f $(OBJS) $(OUTPUT)

