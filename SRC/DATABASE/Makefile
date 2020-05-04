include ../../common.mk

OUTPUT=libDATABASE.a

OBJS = DB_AlarmInfoRead.o \
DB_AlarmInfoWrite.o \
DB_AlarmIsActive.o \
DB_AlarmStatusRead.o \
DB_AlarmStatusWrite.o \
DB_ComputeBreathTimingRatios.o \
DB_ComputeRead.o \
DB_ComputeWrite.o \
DB_ConfigRead.o \
DB_ConfigWrite.o \
DB_ControlRead.o \
DB_ControlWrite.o \
DB_CopyMngt.o \
DB_CurrentRead.o \
DB_CurrentWrite.o \
DB_EventMngt.o \
DB_IHM_Setting_Rtc_Day.o \
DB_InitializeTables.o \
DB_KeyboardEventRead.o \
DB_KeyboardEventWrite.o \
DB_LoadAndCheckEepromIntoRam.o \
DB_MeasurementWrite.o \
DB_Revision.o \
DB_RiseTimeCompute.o \
DB_RTCRead.o \
DB_RTCWrite.o \
DB_SoftwareVersionTest.o \
DB_StartInitDb.o \
DB_TrendRead.o \
DB_TrendWrite.o \
DB_UsbRead.o \
DB_VersionRead.o \
DB_VersionWrite.o \
DB_WriteDataInEepAndRam.o


all: 	$(OBJS)
	rm -f $(OUTPUT)
	${AR} rcs $(OUTPUT) $(OBJS)

%.o:	%.c
	${CC} ${CFLAGS} -c $^

clean:
		rm -f $(OBJS) $(OUTPUT)

