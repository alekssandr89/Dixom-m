
#include <Description.h>
#include "stdio.h"
#include "main_Action.h"
#include <Structures.h>
#include <Functions.h>
#include <Memory_Stm.h>
//#include <DIX_Usb.h>

extern sDixom Dixom;

#define  dAnalog        Dixom.Control.Analog
#define  ChannelButton 	dAnalog.CH[channel].Button[dAnalog.CH[channel].Point]


void SET_EXTERNAL_CONTROL(uint8_t ControlByte, uint8_t Function, uint8_t *Received_String){

	if(Function == EC_BUTTON   )         {  ANALOG_SET_BUTTON(ControlByte, Received_String);   }  // OK Save FLASH
	else if(Function == EC_BUTTON_NAME)  {  ANALOG_SET_BUTTON_NAME(ControlByte, Received_String);        }  // OK Save FLASH
	else if(Function == EC_BTN_SETTINGS) {  CONTROL_Settings(ControlByte, Received_String);        }  // OK Save FLASH
    else{
  		TransmitDataOutputs((uint8_t *)GeneralNotifi_NoPackageFunction, AutoLenght,  false,  true, allAvailable);
	}
}

void CONTROL_Settings(uint8_t ControlByte, uint8_t *Received_String){

	uint8_t data = 0;
	uint16_t param = 0;
	sscanf((char *)Received_String, "%hu %hhu", &param, &data);

	if( param<0 || param>ANLOG_SETTINGS_COUNT ){
 		TransmitDataOutputs((uint8_t *)GeneralNotifi_CommandError, AutoLenght,  false,  true, allAvailable);

	}else{
		if(ControlByte == DATA_SET){ //SET
			if( data<0 || data>255 ){
				Transmit_Data_Out((uint8_t *)GeneralNotifi_CommandError, AutoLenght, NO, FreeRTOS, YES);
			}else{
				Dixom.Control.Settings[param].param = data;
		    	Save_Structure_When_Power_Off();
			}
		}else if(ControlByte == DATA_GET_MODIFIED){
			CONTROL_Show_Settings(Modified);
		}else if(ControlByte == DATA_GET_ALL){
			CONTROL_Show_Settings(All);
		}else if(ControlByte == DATA_DEFAULT){
			 setDefaultControlSettings();
		}else if(ControlByte == DATA_GET){
			 sprintf((char*)Dixom.Buff, MaskGetControlSettings, param, Dixom.Control.Settings[param].param);
			 TransmitDataOutputs(Dixom.Buff, AutoLenght,  false,  true, allAvailable);
		}else{
	 		TransmitDataOutputs((uint8_t *)GeneralNotifi_CtlBitNotSupported, AutoLenght,  false,  true, allAvailable);

		}
	}
}

void CONTROL_Show_Settings(uint8_t ShowType){
    for (uint8_t point = 0; point<ANLOG_SETTINGS_COUNT; point++){
    	if(Dixom.Control.Settings[point].param !=DEFAULT_CONTROL_SETTINGS[point] || ShowType == All){
			sprintf((char*)Dixom.Buff, MaskGetControlSettings, point, Dixom.Control.Settings[point].param);
	    	TransmitDataOutputs(Dixom.Buff, AutoLenght,  false,  true, allAvailable);
    	}
    }
}

 WorkRes AnalogControl_Initialization_Final(){

	 dAnalog.ADC_DataCH       = DefaultBtnValue;
	 dAnalog.ZamerCount       = DefaultZamerCount;
	 return  WR_OK;
}

 void setDefaultControlSettings(){
	  for(uint8_t i = 0; i < ANLOG_SETTINGS_COUNT; i++){
		 Dixom.Control.Settings[i].param = DEFAULT_CONTROL_SETTINGS[i];
	 }
 }

 WorkRes AnalogControl_Struct_Default(uint8_t OnlyEditingReset){

	 if(OnlyEditingReset == NO){ Save_Structure_When_Power_Off();}

	 for (short channel=0;channel<NumberChannels;channel++){


		 dAnalog.CH[channel].ActionTimer   = DefaultActionTimer;
		 dAnalog.CH[channel].ChTimer       = DefaultChTimer;
		 dAnalog.CH[channel].ActionStatus  = DefaultActionStatus;
		 dAnalog.CH[channel].ActionRelease = DefaultActionStatus;
		 dAnalog.CH[channel].Point         = DefaultPoint;
		 dAnalog.CH[channel].doublStatus   = DefaultDoublStatus;
		 dAnalog.CH[channel].HoldStatus    = DefaultHoldStatus;
		 dAnalog.CH[channel].ChClick       = 1;

		 for (short point = 0; point<NumberButtons; point++){

			 if(OnlyEditingReset == NO){

				 dAnalog.CH[channel].Button[point].adcValue = DefaultChTimer;
		 		 dAnalog.CH[channel].Button[point].action = DefaultAction;
		 		 dAnalog.CH[channel].Button[point].delayAction = DefaultDelayAction;
		 	 	 dAnalog.CH[channel].Button[point].doublAction = DefaultDoublAction;
		 		 if (channel==0){ dAnalog.CH[channel].Button[point].interval = 10; }else{ dAnalog.CH[channel].Button[point].interval = Dixom.Control.Settings[SettingsAdcInterval].param; }
		 		 if (channel!=0){ dAnalog.CH[channel].Button[point].delayTiming = DefaultDelayTiming; }else{ dAnalog.CH[channel].Button[point].delayTiming  = 0; }
		 		 if (channel!=0){ dAnalog.CH[channel].Button[point].doublTiming = DefaultDoublTiming; }else{ dAnalog.CH[channel].Button[point].doublTiming  = 0; }
		 		 dAnalog.CH[channel].Button[point].action2      = DefaultAction2;
		 		 dAnalog.CH[channel].Button[point].delayAction2 = DefaultDelayAction2;
		 		 dAnalog.CH[channel].Button[point].doublAction2 = DefaultDoublAction2;
		 		 dAnalog.CH[channel].Button[point].param1       = Defaultparam1;
		 		 dAnalog.CH[channel].Button[point].param2       = Defaultparam2;
		 		 dAnalog.CH[channel].Button[point].param3       = Defaultparam3;
		 	  	 dAnalog.CH[channel].Button[point].param4       = Defaultparam4;

		 		 for(uint8_t i = 0 ; i<BtnNameLenght; i++){
		 			 dAnalog.CH[channel].Button[point].BtnName[i] = 0;
		 		 }


		 		 if((channel==3) & (point ==0)){
		 			 dAnalog.CH[channel].Button[point].adcValue = 0;
		 			 dAnalog.CH[channel].Button[point].action = 104;
		 			 dAnalog.CH[channel].Button[point].delayAction = 107;
		 		 }
		 		 if((channel==4) & (point ==0)){
		 			 dAnalog.CH[channel].Button[point].adcValue = 0;
		 			 dAnalog.CH[channel].Button[point].action = 104;
		 			 dAnalog.CH[channel].Button[point].delayAction = 107;
		 		 }

		 		 if((channel==0) & (point ==0)){ dAnalog.CH[channel].Button[point].adcValue = 500;  dAnalog.CH[channel].Button[point].action = 101; }
		 		 if((channel==0) & (point ==1)){ dAnalog.CH[channel].Button[point].adcValue = 1000; dAnalog.CH[channel].Button[point].action = 102; }
		 		 if((channel==0) & (point ==2)){ dAnalog.CH[channel].Button[point].adcValue = 1500; dAnalog.CH[channel].Button[point].action = 101; }
		 		 if((channel==0) & (point ==3)){ dAnalog.CH[channel].Button[point].adcValue = 2000; dAnalog.CH[channel].Button[point].action = 102; }
			 }
			 dAnalog.CH[channel].Button[point].editing = YES;
		 }
	 }
	 return  WR_OK;
 }

 void ANALOG_SET_BUTTON(uint8_t ControlByte, uint8_t*  Recive_USB){


	 	uint8_t Bias = 3;
	 	uint16_t channel = 0;
	 	uint16_t point = 0;
	 	sscanf((char *)Recive_USB, "%hu %hu", &channel, &point);
	 	if(point>9) Bias++;
	 	if(point>99) Bias++;
	 	if(point>999) Bias++;
	 	Bias++;

	    if(ControlByte == DATA_SET){ //SET

	    	Save_Structure_When_Power_Off();


	    	if( channel > NUM_ANALOG_CONTROL_CHANNEL-1 ){
 	      		TransmitDataOutputs((uint8_t *)GeneralNotifi_CommandError, AutoLenght,  false,  true, allAvailable);

	    	}else{


 		 	short adcValue     = point,
 		 	      action       = DefaultAction,
 		 	      delayAction  = DefaultDelayAction,
 		 	      doublAction  = DefaultDoublAction,
 		 	      delayTiming  = DefaultDelayTiming,
 		 	      doublTiming  = DefaultDoublTiming,
 		 	      action2      = DefaultAction2,
 		 	      delayAction2 = DefaultDelayAction2,
 		 	      doublAction2 = DefaultDoublAction2,
 		 	      param1       = Defaultparam1,
 		 	      param2       = Defaultparam2,
 		 	      param3       = Defaultparam3,
 		 	      param4       = Defaultparam4;

 		 	uint8_t  interval  = Dixom.Control.Settings[SettingsAdcInterval].param;

 		 	sscanf((char *)Recive_USB+Bias, "%hu %hu %hu %hu %hu %hu %hu %hu %hu %hu %hu %hu %hu",
 		 		  &interval,      //1  �����������
 		 		  &action,        //2  �������� 1
 		 		  &delayAction,   //3  �������� 1 ��� ���������
 		 		  &doublAction,   //4  �������� 1 ��� ������� �����
 		 		  &delayTiming,   //5  ����� ��������� ���������� �������� ��� ���������
 		 		  &doublTiming,   //6  ����� ��������� ��� �������� �����
 		 		  &action2,       //7  �������� 2
 		 		  &delayAction2,  //8  �������� 2 ��� ���������
 		 		  &doublAction2,  //9  �������� 2 ��� ������� �����
 		 	      &param1,        //10 �������� 1
 		 	      &param2,        //11 �������� 2
 		 	      &param3,        //12 �������� 3
 		 	      &param4);       //13 �������� 4


 		 	if((channel==0) & (delayTiming==DefaultDelayTiming)) delayTiming=0;
 		 	if((channel==0) & (doublTiming==DefaultDoublTiming)) doublTiming=0;

 			if(channel == 5) interval = 2;

 		 	if(adcValue > 3899 || adcValue < 0){
  	      		TransmitDataOutputs((uint8_t *)GeneralNotifi_CommandError, AutoLenght,  false,  true, allAvailable);

 		 	}else{
 		 		Analog_SetButton( channel, adcValue, interval, action, delayAction, doublAction, delayTiming, doublTiming, action2, delayAction2, doublAction2, param1, param2, param3, param4);
 		 		Save_Structure_When_Power_Off();

 		 	}
	      }
	    }else if(ControlByte == DATA_GET_ALL){
	    	ANALOG_BUTTON_SHOW_PARAMETERS(All);
	    	TransmitDataOutputs((uint8_t *)MaskGetAllBtnPointDone, AutoLenght,  false,  true, allAvailable);
	    }else if(ControlByte == DATA_GET_MODIFIED){
	    	ANALOG_BUTTON_SHOW_PARAMETERS(Modified);
	      	TransmitDataOutputs((uint8_t *)MaskGetAnaloControlLoadCplt, AutoLenght,  false,  true, allAvailable);
	    }else if(ControlByte == DATA_DELETE){

	    }else if(ControlByte == DATA_DEFAULT){
	    	AnalogControl_Struct_Default(NO);
	    	TransmitDataOutputs((uint8_t *)AnaloControlBtnDefault, AutoLenght,  false,  true, allAvailable);
	    }else if(ControlByte == DATA_GET || ControlByte == DATA_GET_ALL ){
		 	if( point > NUM_BUTTON_ONE_CHANNEL-1){
  	      		TransmitDataOutputs((uint8_t *)GeneralNotifi_CommandError, AutoLenght,  false,  true, allAvailable);
		 	}else{
		 		Analog_GetButton(channel, point, All);
		 	}
	    }else{
	    	TransmitDataOutputs((uint8_t *)GeneralNotifi_CtlBitNotSupported, AutoLenght,  false,  true, allAvailable);
	    }
 }


 void ANALOG_GET_BUTTON(uint8_t*  Recive_USB){

 	short channel = 0,
 	      point   = 0;

     sscanf((char *)Recive_USB, "%hu %hu", &channel, &point);

 	if( channel<0 || channel>NumberChannels-1 ||  point<0 || point>ButtonElem-1 || (channel==0) & (point > EncoderElem-1) ){
     		TransmitDataOutputs((uint8_t *)GeneralNotifi_CommandError, AutoLenght,  false,  true, allAvailable);

 	}else{
 		Analog_GetButton(channel, point, All);
 	}
 }

 void ANALOG_GET_ALL_BUTTON(uint8_t *Recive_USB){

 	short   channel;
 	short   CountElem;

 	sscanf((char *)Recive_USB, "%hu", &channel);

 	if( channel<0 || channel>NumberChannels-1){
     		TransmitDataOutputs((uint8_t *)GeneralNotifi_CommandError, AutoLenght,  false,  true, allAvailable);

 	}else{

 		if(channel==0) {
 			CountElem = EncoderElem;
 		}else{
 			CountElem = ButtonElem;
 		}
 		for(short point = 0; point<CountElem; point++ ){

 			Analog_GetButton(channel, point, All);
 			Delay(10, FreeRTOS);
 		}
 	}
 }

void SHOW_CONTROL_PARAMETERS(uint8_t AllOrModified){
	 CONTROL_Show_Settings(Modified);
	 ANALOG_BUTTON_SHOW_PARAMETERS(Modified);
}

 void ANALOG_BUTTON_SHOW_PARAMETERS(uint8_t AllOrModified){

 	uint8_t   CountElem;
 	for(uint8_t ch = 0 ; ch < NumberChannels; ch ++){
 		if(ch==0) {
 			CountElem = EncoderElem;
 		}else{
 			CountElem = ButtonElem;
 		}
 		for(short point = 0; point<CountElem; point++ ){
 			Analog_GetButton(ch, point, AllOrModified);
 		}
 	}
 }

 void Analog_LogicOfActions(){  //TIM6 run every millisecond

	 if(Dixom.DevInfo.InMode == ReadyForWork){

		 for ( short channel=0; channel<NumberChannels; channel++ ){
			 if(dAnalog.CH[channel].HoldStatus==1)  {
				 dAnalog.CH[channel].ActionTimer++;
			 }
		 }
		 for ( short channel=0; channel<NumberChannels; channel++ ){

			 if((dAnalog.CH[channel].ChTimer==0)&(ChannelButton.delayAction == 0)&(ChannelButton.delayAction2 ==0)&(ChannelButton.doublAction  ==0)&(ChannelButton.doublAction2 ==0)){

				 dAnalog.CH[channel].pressed = YES;
				 dAnalog.CH[channel].ActionStatus=Click;
				 dAnalog.CH[channel].ChTimer = DefaultChTimer;

			 }else if((dAnalog.CH[channel].ActionTimer==ChannelButton.delayTiming)&(ChannelButton.delayAction!=0||ChannelButton.delayAction2!=0)){

				 if(dAnalog.CH[channel-1].ResButBlockTimer > 500){
					 dAnalog.CH[channel].ActionComplite = NO;
					 dAnalog.CH[channel].pressed = YES;
					 dAnalog.CH[channel].ActionStatus=DelayClick;

					 dAnalog.CH[channel].ActionTimer = DefaultActionTimer;
				 }else{
					 dAnalog.CH[channel].ActionTimer = DefaultActionTimer;
				 }
			 }else if (((dAnalog.CH[channel].ChTimer<ChannelButton.doublTiming)&(dAnalog.CH[channel].doublStatus>=2)&(dAnalog.CH[channel].HoldStatus==0))
					 &(ChannelButton.doublAction!=0||ChannelButton.doublAction2!=0)){

				 dAnalog.CH[channel].pressed = YES;
				 dAnalog.CH[channel].ActionStatus=DoublClick;
				 dAnalog.CH[channel].ChTimer = ChannelButton.doublTiming;

			 }else if (((dAnalog.CH[channel].ChTimer > 2)&(dAnalog.CH[channel].ChTimer!= DefaultChTimer)&(ChannelButton.doublAction==0)&(ChannelButton.doublAction2==0)
					  & (dAnalog.CH[channel].doublStatus<=1)&(dAnalog.CH[channel].HoldStatus==0 ))&(ChannelButton.action!=0||ChannelButton.action2!=0)){

				 dAnalog.CH[channel].pressed = YES;
				 dAnalog.CH[channel].ActionStatus=Click;

			 }else if (((dAnalog.CH[channel].ChTimer == ChannelButton.doublTiming)&(dAnalog.CH[channel].doublStatus<=1)&(dAnalog.CH[channel].HoldStatus==0 ))
					 & (ChannelButton.action!=0||ChannelButton.action2!=0)){

				 dAnalog.CH[channel].pressed = YES;
				 dAnalog.CH[channel].ActionStatus=Click;

			 }else if ( dAnalog.CH[channel].ChTimer >  ChannelButton.doublTiming ){

				 dAnalog.CH[channel].doublStatus=0;
			 }
		 }
		 for ( short channel=0; channel<NumberChannels; channel++ ){

			 if(channel<NumberChannels-1){

				 if( (dAnalog.CH[channel].ChTimer<DefaultChTimer) & (dAnalog.CH[channel].HoldStatus==0) ){
					 dAnalog.CH[channel].ChTimer++;
				 }
			 }else{

				 if(dAnalog.CH[channel].ChTimer<DefaultChTimer){
					 dAnalog.CH[channel].ChTimer++;
				 }
			 }
		 }
	 }
 }

 void Analog_ActionBtn(void){

 	 for ( short channel=0; channel<NumberChannels; channel++ ){

 	     if(dAnalog.CH[channel].ActionStatus==DelayClick){
 	    	 if(dAnalog.CH[channel].ActionComplite == NO){
 		    	 if ( ChannelButton.delayAction !=0) {  Action( ChannelButton.delayAction,  ChannelButton.param1, ChannelButton.param2, ChannelButton.param3, ChannelButton.param4, channel, YES ); Delay(ActionDelay, FreeRTOS);  }
 		    	 if ( ChannelButton.delayAction2!=0) {  Action( ChannelButton.delayAction2, ChannelButton.param1, ChannelButton.param2, ChannelButton.param3, ChannelButton.param4, channel, YES ); }
 		    	dAnalog.CH[channel].ActionComplite = YES;
 	    	 }else if (dAnalog.CH[channel].ActionPreset == NO){
 		    	 if ( ChannelButton.delayAction !=0) {  Action( ChannelButton.delayAction,  ChannelButton.param1, ChannelButton.param2, ChannelButton.param3, ChannelButton.param4, channel, NO ); Delay(ActionDelay, FreeRTOS);}
 		    	 if ( ChannelButton.delayAction2!=0) {  Action( ChannelButton.delayAction2, ChannelButton.param1, ChannelButton.param2, ChannelButton.param3, ChannelButton.param4, channel, NO ); }
 		    	Analog_ResetCH(channel);
 	    	 }
 	     } else if(dAnalog.CH[channel].ActionStatus==DoublClick){
 	    	 if(dAnalog.CH[channel].ActionComplite == NO){
 		    	 if ( ChannelButton.doublAction !=0) {  Action( ChannelButton.doublAction,  ChannelButton.param1, ChannelButton.param2, ChannelButton.param3, ChannelButton.param4, channel, YES ); Delay(ActionDelay, FreeRTOS);}
 		    	 if ( ChannelButton.doublAction2!=0) {  Action( ChannelButton.doublAction2, ChannelButton.param1, ChannelButton.param2, ChannelButton.param3, ChannelButton.param4, channel, YES ); }
 		      	 dAnalog.CH[channel].ActionComplite = YES;
 	    	 }else if (dAnalog.CH[channel].ActionPreset == NO){
 		    	 if ( ChannelButton.doublAction !=0) {  Action( ChannelButton.doublAction,  ChannelButton.param1, ChannelButton.param2, ChannelButton.param3, ChannelButton.param4, channel, NO ); Delay(ActionDelay, FreeRTOS);}
 		    	 if ( ChannelButton.doublAction2!=0) {  Action( ChannelButton.doublAction2, ChannelButton.param1, ChannelButton.param2, ChannelButton.param3, ChannelButton.param4, channel, NO ); }
 		    	Analog_ResetCH(channel);
 	    	 }
 	     } else if(dAnalog.CH[channel].ActionStatus==Click){
 	    	 if(dAnalog.CH[channel].ActionComplite == NO){
 		    	 if ( ChannelButton.action      !=0) {  Action( ChannelButton.action,       ChannelButton.param1, ChannelButton.param2, ChannelButton.param3, ChannelButton.param4, channel, YES ); Delay(ActionDelay, FreeRTOS);}
 		    	 if ( ChannelButton.action2     !=0) {  Action( ChannelButton.action2,      ChannelButton.param1, ChannelButton.param2, ChannelButton.param3, ChannelButton.param4, channel, YES ); }
 			     dAnalog.CH[channel].ActionComplite = YES;
 	    	 }else if (dAnalog.CH[channel].ActionPreset == NO){
 		    	 if ( ChannelButton.action      !=0) {  Action( ChannelButton.action,       ChannelButton.param1, ChannelButton.param2, ChannelButton.param3, ChannelButton.param4, channel, NO ); Delay(ActionDelay, FreeRTOS);}
 		    	 if ( ChannelButton.action2     !=0) {  Action( ChannelButton.action2,      ChannelButton.param1, ChannelButton.param2, ChannelButton.param3, ChannelButton.param4, channel, NO ); }
 		    	Analog_ResetCH(channel);
 	    	 }
 	     } else if(dAnalog.CH[channel].ActionStatus==ClickReset ){
 	    	Analog_ResetCH(channel);
 	     }
 	 }
  }


 void Analog_ResetCH(uint8_t channel){
	 dAnalog.CH[channel].ActionTimer   = DefaultActionTimer;
	 dAnalog.CH[channel].doublStatus   = DefaultDoublStatus;
	 dAnalog.CH[channel].ChTimer       = DefaultChTimer;
	 dAnalog.CH[channel].ActionRelease = dAnalog.CH[channel].ActionStatus;
	 dAnalog.CH[channel].ActionStatus  = 0;
 }

 void Analog_ChannelBlock(short channel){

	 dAnalog.CH[channel].HoldStatus=1;
 }

 void Analog_ActionCH(short channel, short value, short pressed){

 	if( value != DefaultChTimer ){
 		for ( short point = 0; point< ButtonElem; point++ ){

 			if (((( dAnalog.CH[channel].Button[point] .adcValue-dAnalog.CH[channel].Button[point].interval)<value)
 					&& ((dAnalog.CH[channel].Button[point].adcValue+dAnalog.CH[channel].Button[point].interval)>value))
 					|| (dAnalog.CH[channel].Button[point].action==0) ){

 				if (dAnalog.CH[channel].Button[point].action==0){
 					if(dAnalog.CH[channel].ChClick==1 && pressed == YES){
 						char str[10];
 						sprintf(str, MaskGetAnaloControlBtnInit, channel, value);
  		  	      		TransmitDataOutputs((uint8_t *)str, AutoLenght,  false,  true, allAvailable);

 						dAnalog.CH[channel].ChClick=0;
 					}
 					return;
 				}else {
 					if(dAnalog.CH[channel].ChClick==1 && pressed == YES){
 						dAnalog.CH[channel].ActionComplite = NO;
 						dAnalog.CH[channel].Point=point;
 						dAnalog.CH[channel].ChTimer=0;
 						dAnalog.CH[channel].doublStatus++;
 						dAnalog.CH[channel].ChClick=0;
 					}
 					dAnalog.CH[channel].ActionPreset = pressed;
 					return;
 				}
 			}
 		}
 	}else{
 		dAnalog.CH[channel].HoldStatus=0;
 		dAnalog.CH[channel].ChClick=1;
 	}
 }


 void Analog_SetButton(short channel, short adcValue, uint8_t interval, short action, short delayAction, short doublAction,
		 short delayTiming, short doublTiming, short action2, short delayAction2, short doublAction2, short param1, short param2, short param3, short param4){

 	short CountElem;

 	if (delayTiming < 1)
 		delayTiming = 1;

 	if (channel == 0) {
 		CountElem = EncoderElem;
 		delayAction = DefaultDelayAction;
 		doublAction = DefaultDoublAction;
 		delayTiming = 0;
 		doublTiming = 0;
 		delayAction2 = DefaultDelayAction2;
 		doublAction2 = DefaultDoublAction2;
 	} else {
 		CountElem = ButtonElem;
 	}

 	for (short point = 0; point < CountElem; point++) {
 		if (action != 0 && dAnalog.CH[channel].Button[point].action == 0) {
 			dAnalog.CH[channel].Button[point].adcValue = adcValue;
 			dAnalog.CH[channel].Button[point].action = action;
 			dAnalog.CH[channel].Button[point].delayAction = delayAction;
 			dAnalog.CH[channel].Button[point].doublAction = doublAction;
 			dAnalog.CH[channel].Button[point].action2 = action2;
 			dAnalog.CH[channel].Button[point].delayAction2 = delayAction2;
 			dAnalog.CH[channel].Button[point].doublAction2 = doublAction2;
 			if (channel == 0 || channel == 5) {
 				dAnalog.CH[channel].Button[point].interval = 10;
 			}
 			dAnalog.CH[channel].Button[point].delayTiming = delayTiming;
 			dAnalog.CH[channel].Button[point].doublTiming = doublTiming;
 			dAnalog.CH[channel].Button[point].param1 = param1;
 			dAnalog.CH[channel].Button[point].param2 = param2;
 			dAnalog.CH[channel].Button[point].param3 = param3;
 			dAnalog.CH[channel].Button[point].param4 = param4;
 			dAnalog.CH[channel].Button[point].editing = 1;
 			point = CountElem;
 			//return;
 		} else if (dAnalog.CH[channel].Button[point].adcValue == adcValue) {
 			if (action == 0) {
 				dAnalog.CH[channel].Button[point].adcValue = DefaultChTimer;
 			}
 			dAnalog.CH[channel].Button[point].action = action;
 			dAnalog.CH[channel].Button[point].delayAction = delayAction;
 			dAnalog.CH[channel].Button[point].doublAction = doublAction;
 			dAnalog.CH[channel].Button[point].action2 = action2;
 			dAnalog.CH[channel].Button[point].delayAction2 = delayAction2;
 			dAnalog.CH[channel].Button[point].doublAction2 = doublAction2;
 			if (channel == 0 || channel == 5) {
 				dAnalog.CH[channel].Button[point].interval = 10;
 			}else{
 				dAnalog.CH[channel].Button[point].interval = Dixom.Control.Settings[SettingsAdcInterval].param;
 			}
 			dAnalog.CH[channel].Button[point].delayTiming = delayTiming;
 			dAnalog.CH[channel].Button[point].doublTiming = doublTiming;
 			dAnalog.CH[channel].Button[point].param1 = param1;
 			dAnalog.CH[channel].Button[point].param2 = param2;
 			dAnalog.CH[channel].Button[point].param3 = param3;
 			dAnalog.CH[channel].Button[point].param4 = param4;
 			 for(uint8_t i = 0 ; i<BtnNameLenght; i++){
 				dAnalog.CH[channel].Button[point].BtnName[i] = 0;
 			 }
 			dAnalog.CH[channel].Button[point].editing = 1;
 			//return;
 			point = CountElem;
 		}

 	}
 }


 void ANALOG_SET_BUTTON_NAME(uint8_t ControlByte, uint8_t *Recive_USB) {

	uint8_t CountElem;
	uint8_t name[BTN_NAME_LEN];
	uint8_t biasC = 2;
	uint8_t biasV = 2;

	uint16_t channel = 0;
	uint16_t adcValue = 0;
	sscanf((char*) Recive_USB, "%hu %hu", &channel, &adcValue);
	if (adcValue > 9)
		biasV++;
	if (adcValue > 99)
		biasV++;
	if (adcValue > 999)
		biasV++;

	if (channel > NUM_ANALOG_CONTROL_CHANNEL - 1 || adcValue > 4900) {

		TransmitDataOutputs((uint8_t *) GeneralNotifi_CommandError, AutoLenght,  false,  true, allAvailable);

	} else {
		if (ControlByte == DATA_SET) { //SET
			for (uint8_t i = 0; i < BtnNameLenght; i++) {
				name[i] = Recive_USB[i + biasC + biasV];
			}

			if (channel == 0) {
				CountElem = EncoderElem;
			} else {
				CountElem = ButtonElem;
			}

			for (short point = 0; point < CountElem; point++) {
				if (dAnalog.CH[channel].Button[point].adcValue == adcValue) {
					dAnalog.CH[channel].Button[point].editing = 1;
					for (uint8_t i = 0; i < BtnNameLenght; i++) {
						dAnalog.CH[channel].Button[point].BtnName[i] = name[i];
					}
				}
			}
			Save_Structure_When_Power_Off();
		} else if (ControlByte == DATA_GET_MODIFIED) {

		} else if (ControlByte == DATA_DEFAULT) {

		} else if (ControlByte == DATA_GET || ControlByte == DATA_GET_ALL) {
			if (adcValue > NUM_BUTTON_ONE_CHANNEL - 1) {

				TransmitDataOutputs((uint8_t *) GeneralNotifi_CommandError, AutoLenght,  false,  true, allAvailable);

			} else {
				if (dAnalog.CH[channel].Button[adcValue].BtnName[0] != 0) {

					for (uint8_t i = 0; i < 65; i++) {
						Dixom.Buff[i] = 0;
					}

					sprintf((char*) Dixom.Buff, MaskGetAnaloControlBtnName,
							channel,
							dAnalog.CH[channel].Button[adcValue].adcValue);

					uint8_t bias = 8;

					if (dAnalog.CH[channel].Button[adcValue].adcValue > 9) {
						bias = 9;
					}
					if (dAnalog.CH[channel].Button[adcValue].adcValue > 99) {
						bias = 10;
					}
					if (dAnalog.CH[channel].Button[adcValue].adcValue > 999) {
						bias = 11;
					}

					for (uint8_t count = 0; count < BtnNameLenght; count++) {
						Dixom.Buff[count + bias] =
						dAnalog.CH[channel].Button[adcValue].BtnName[count];
					}
					TransmitDataOutputs(Dixom.Buff, AutoLenght,  false,  true, allAvailable);

				} else {
					TransmitDataOutputs((uint8_t*) AnaloControlBtnNotName, AutoLenght,  false,  true, allAvailable);

				}
			}
		}else{

			TransmitDataOutputs((uint8_t*) GeneralNotifi_CtlBitNotSupported, AutoLenght,  false,  true, allAvailable);

		}

	}

}


 void Analog_GetButton(short channel, short point, uint8_t AllOrModified){

 	if(AllOrModified == All || dAnalog.CH[channel].Button[point].action != 0){

 		if(dAnalog.CH[channel].Button[point].action !=0){
 			sprintf((char*)Dixom.Buff, MaskGetAnaloControlBtn,
 	    		 channel,
				 dAnalog.CH[channel].Button[point].adcValue,
				 dAnalog.CH[channel].Button[point].interval,
				 dAnalog.CH[channel].Button[point].action,
				 dAnalog.CH[channel].Button[point].delayAction,
				 dAnalog.CH[channel].Button[point].doublAction,
				 dAnalog.CH[channel].Button[point].delayTiming,
				 dAnalog.CH[channel].Button[point].doublTiming,
				 dAnalog.CH[channel].Button[point].action2,
				 dAnalog.CH[channel].Button[point].delayAction2,
				 dAnalog.CH[channel].Button[point].doublAction2,
				 dAnalog.CH[channel].Button[point].param1,
				 dAnalog.CH[channel].Button[point].param2,
				 dAnalog.CH[channel].Button[point].param3,
				 dAnalog.CH[channel].Button[point].param4
 			);

 			TransmitDataOutputs(Dixom.Buff, AutoLenght,  false,  true, allAvailable);


 			if(dAnalog.CH[channel].Button[point].BtnName[0] != 0){

 				for(uint8_t i = 0; i< 65; i++){
 					Dixom.Buff[i] = 0;
 				}

 				sprintf((char*)Dixom.Buff, MaskGetAnaloControlBtnName, channel, dAnalog.CH[channel].Button[point].adcValue);

 				uint8_t bias = 8;

 				if(dAnalog.CH[channel].Button[point].adcValue > 9){ bias = 9;}
 				if(dAnalog.CH[channel].Button[point].adcValue > 99){ bias = 10;}
 				if(dAnalog.CH[channel].Button[point].adcValue > 999){ bias = 11;}

 				for(uint8_t count = 0; count < BtnNameLenght; count ++){
 					Dixom.Buff[count+bias] = dAnalog.CH[channel].Button[point].BtnName[count];
 				}

 				TransmitDataOutputs(Dixom.Buff, AutoLenght,  false,  true, allAvailable);

 			}else{
 			//	Transmit_Data_Out((uint8_t *)AnaloControlBtnNotName, AutoLenght, NO, FreeRTOS, YES);
 			}
 		}else{

 			TransmitDataOutputs((uint8_t *) GeneralNotifi_NotFound, AutoLenght,  false,  true, allAvailable);

 		}
 	}else{
 	//	Transmit_Data_Out((uint8_t *) AnaloControlBtnNotConfig, AutoLenght, 7, FreeRTOS, YES);
 	}
 }

 void Analog_AppInitBTN(void){

 	ANALOG_GET_ALL_BUTTON((uint8_t *)"0");
 	ANALOG_GET_ALL_BUTTON((uint8_t *)"1");
 	ANALOG_GET_ALL_BUTTON((uint8_t *)"2");
 	ANALOG_GET_ALL_BUTTON((uint8_t *)"3");
 	ANALOG_GET_ALL_BUTTON((uint8_t *)"4");
 	ANALOG_GET_ALL_BUTTON((uint8_t *)"5");
 	ANALOG_GET_ALL_BUTTON((uint8_t *)"6");

 	Transmit_Data_Out((uint8_t *)MaskGetAnaloControlLoadCplt , AutoLenght, NO, FreeRTOS, YES);

 }

