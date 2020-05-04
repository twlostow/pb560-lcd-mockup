include ../../common.mk

OUTPUT=libSYSTEM.a

OBJS = INT_AbsPress_Interpol.o \
INT_BlowerTemperatureInterpol.o \
INT_ExhFlow_Interpol.o \
INT_Fio2_Interpol.o \
INT_Flow_Conversion_Equation.o \
INT_InspFlow_Interpol.o \
INT_O2Flow_Interpol.o \
INT_O2Press_Interpol.o \
INT_PatPress_Interpol.o \
INT_ProxiPress_Interpol.o \
INT_SpeedBlow_Interpol.o \
INT_ValvePress_Interpol.o \
INT_Voltages_Interpol.o \
KEY_Event.o \
SYS_Backlight_Mngt.o \
SYS_CalibrationManagement.o \
SYS_Counters.o \
SYS_CurveFlowCalcul.o \
SYS_ExhFlowCalibration.o \
SYS_Fio2Calibration.o \
SYS_Fio2Detection.o \
SYS_Interpolation.o \
SYS_Keyboard.o \
SYS_PressionChoice.o \
SYS_RTC_Update.o \
SYS_SetupLaunch.o \
SYS_VentilationLaunch.o


all: 	$(OBJS)
	rm -f $(OUTPUT)
	${AR} rcs $(OUTPUT) $(OBJS)

%.o:	%.c
	${CC} ${CFLAGS} -c $^

clean:
		rm -f $(OBJS) $(OUTPUT)
