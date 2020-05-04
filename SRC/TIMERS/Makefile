CXX = g++
CXXFLAGS = -fpermissive -I.. -I../GENERAL -I../DATABASE -I../DRIVERS -D_BORLAND -DLEGENDAIR_S2 -I../DISPLAY -I../DRIVERS_DISPLAY -I../SYSTEM -I../DATABASE_SETTINGS -I../DRIVERS_HMI

CC = gcc
CFLAGS =  -I.. -I../GENERAL -I../DATABASE -I../DRIVERS -D_BORLAND -DLEGENDAIR_S2 -I../DISPLAY -I../DRIVERS_DISPLAY -I../SYSTEM -I../DATABASE_SETTINGS -I../DRIVERS_HMI -I../SECURITY -I../MAIN -I../VENTILATION -I../EEPROM

OBJS = TIM_Launch.o \
TIM_ReadCounterValue.o \
TIM_ReadDecounterValue.o \
TIM_ReadMinuteCounterValue.o \
TIM_ReadMinuteDecounterValue.o \
TIM_StartCounter.o \
TIM_StartDecounter.o \
TIM_StartMinuteCounter.o \
TIM_StartMinuteDecounter.o \
TIM_StopDecounter.o \
TIM_StopMinuteDecounter.o \
TIM_Temporize.o \
TIM_TestMinuteDecounterActivateOk.o \
TIM_TestMinuteOverflowOk.o \
TIM_TestOverflowOk.o



all: 	$(OBJS)
	${CC} -o test $(OBJS)

%.o:	%.c
	${CC} ${CFLAGS} -c $^

clean:
		rm -f $(OBJS)

