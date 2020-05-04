include ../../common.mk

OBJS = \
HMI_AlarmManager.o \
HMI_AlarmTable.o \
HMI_AutoCalibValue.o \
HMI_BarGraph.o \
HMI_BatteryObject.o \
HMI_BlinkTable.o \
HMI_Config.o \
HMI_Controller.o \
HMI_Controller_Data.o \
HMI_ControllerInit.o \
HMI_CurveData.o \
HMI_DataBaseAccess.o \
HMI_DataBaseLink.o \
HMI_DisplayObject.o \
HMI_EditableObject.o \
HMI_EventNode.o \
HMI_FieldObject.o \
HMI_FrameAlarme.o \
HMI_FrameBatteryMenu.o \
HMI_FrameCircuitCheck.o \
HMI_FrameConfiguration.o \
HMI_FrameGraphConfig.o \
HMI_FrameGraph.o \
HMI_FrameHistoAlarme.o \
HMI_FrameHistoAno.o \
HMI_FrameMaintenance.o \
HMI_FramePreference.o \
HMI_FrameTendance.o \
HMI_FrameUSB.o \
HMI_FrameVentilation.o \
HMI_FrameVoltControl.o \
HMI_FrameWelcome.o \
HMI_GlobalEventNode.o \
HMI_GraphControl.o \
HMI_Graph.o \
HMI_GraphicBox.o \
HMI_GraphicTable.o \
HMI_GraphTitle.o \
HMI_InterfaceCppC.o \
HMI_KeyboardEventCppC.o \
HMI_LabelAlarme.o \
HMI_LabelAlarmeTwoLine.o \
HMI_Label.o \
HMI_LabelLanguage.o \
HMI_LabelMode.o \
HMI_LabelOneValue.o \
HMI_LabelOneValueDbRT.o \
HMI_LabelOneValueDefault.o \
HMI_LabelOneValueFiO2Flag.o \
HMI_LabelOneValueFlag.o \
HMI_LabelOneValueFourDefault.o \
HMI_LabelOneValueLine.o \
HMI_LabelOneValueSign.o \
HMI_LabelOneValueSignDefault.o \
HMI_LabelOneValueSignFlag.o \
HMI_LabelOneValueState.o \
HMI_LabelOneValueThreeDefault.o \
HMI_LabelOneValueTrig.o \
HMI_LabelProgressBar.o \
HMI_LabelRatio.o \
HMI_LabelRatioTrunk.o \
HMI_LabelSerial.o \
HMI_LabelStringNItems.o \
HMI_LabelStringTwoItems.o \
HMI_LabelSymbol.o \
HMI_LabelTabOneValue.o \
HMI_LabelTabOneValueDefault.o \
HMI_LabelTabOneValueFlag.o \
HMI_LabelTabTwoValue.o \
HMI_LabelTabTwoValueDefault.o \
HMI_LabelTabTwoValueFlag.o \
HMI_LabelTabTwoValueTwoLabels.o \
HMI_LabelTwoStringsTwoItems.o \
HMI_LabelTwoValueCentered.o \
HMI_LabelTwoValue.o \
HMI_LabelTwoValueLock.o \
HMI_LabelTwoValueTrig.o \
HMI_LabelValueString.o \
HMI_LabelVersion.o \
HMI_LoupeAlarm.o \
HMI_Loupe.o \
HMI_ManualCalibTwoValue.o \
HMI_ManualCalibValue.o \
HMI_MenuControlNode.o \
HMI_MenuObject.o \
HMI_Message2.o \
HMI_Message.o \
HMI_MessageDelayed.o \
HMI_MessageDialog.o \
HMI_MessageList.o \
HMI_MonitoringObject.o \
HMI_MonitoringObjectTwoValue.o \
HMI_MonitoringObjectWithRecall.o \
HMI_ParameterSpy.o \
HMI_ParamNode.o \
HMI_Pen.o \
HMI_PerformanceGraphControl.o \
HMI_Pixel.o \
HMI_Rampe.o \
HMI_RefreshTable.o \
HMI_RiseTime.o \
HMI_Scale.o \
HMI_Setup1.o \
HMI_ShiftingDots.o \
HMI_SymbolField.o \
HMI_TendanceTable.o \
HMI_TextField.o \
HMI_TextFieldLine.o \
HMI_TimerEventCppC.o \
HMI_Tools.o \
HMI_USBInterfaceCppC.o \
HMI_ValueField.o \
HMI_ValueTextField.o \
HMI_VDate.o \
HMI_VFrame.o \
HMI_VPoint.o \
HMI_VTime.o \
HMI_VTimer.o \
HMI_WobGraphControl.o \
HMI_IhmType.o

all: 	$(OBJS)
	rm -f libHMI.a
	${AR} rcs libHMI.a $(OBJS)

%.o:	%.c
	${CC} ${CFLAGS} -c $^ -o $@

%.o:	%.cpp
	${CXX} ${CXXFLAGS} -c $^ -o $@

clean:
		rm -f $(OBJS)
