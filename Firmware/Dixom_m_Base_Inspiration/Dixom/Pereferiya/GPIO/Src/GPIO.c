#include <Description.h>
#include "GPIO.h"
#include "stdint.h"
#include "main.h"
#include "Exchange.h"
#include <Structures.h>
#include "GPIO_Constants.h"

extern sDixom Dixom;

//#include "StructSettings.h"

uint8_t StateUSB1 = OFF;
uint8_t StateUSB2 = OFF;
uint8_t RearOnOffBtn = OFF;
uint8_t TriggerStateCh1 = OFF;
uint8_t TriggerStateCh2 = OFF;
uint8_t TriggerStateCh3 = OFF;

void Launch_Initialization_Gpio(){
	//1 turn on DC DC converters for 3.3v and 5v. Platform power.
	Power_DCDC_33V_5V(ON);

	//2 Turn on the platform status LE
	Power_LED(ON);

	//3 Power off USB port �1
	Power_USB1(OFF);

	//4 Power off USB port �2
	Power_USB2(OFF);

	//5 Power off VBUS
	Power_VBUS(OFF);

	//6 Switch trigger 1 to the off position
	Switch_Trigger1(ON);

	//7 Switch trigger 2 to the off position
	Switch_Trigger2(OFF);

	//8 Switch trigger 3 to the off position
	Switch_Trigger3(OFF);

	//9	We put the DAC in reboot mode, or in power pown mode
	Switch_Dac_Reset(OFF);

	//10 Switch USB OTG to the off position
	Switch_USB_Otg(OFF);

	//11 Power off power remout
	Power_Remout(OFF);

	//12 Power off power camera
	Power_Camera(OFF);

	//13 SPI DSP chip selection switch in off position
	Switch_CS_DSP(OFF);

	//13 SPI Flash memory chip selection switch in off position
	Switch_CS_FLASH(OFF);

	//14 turn on DC DC converters for 3.3v and 5v. Platform power.
	Power_CINV_12v(ON);

	//15 Reloading modules on the top board
	Up_Modules_Reset(OFF);
}

void Configured_Initialization_Gpio(){


    /*if(Dixom.Basis.Settings.Param[Settings1Trigger].element == TriggerOnOffSave){
		Switch_Trigger1(Dixom.Basis.Settings.Param[SettingsState1Trigger].element);
	}else{
		Switch_Trigger1(OFF);
	}*/

	if(Dixom.Basis.Settings.Param[Settings2Trigger].element == TriggerOnOffSave){
		Switch_Trigger2(Dixom.Basis.Settings.Param[SettingsState2Trigger].element);
	}else{
		Switch_Trigger2(OFF);
	}

	if(Dixom.Basis.Settings.Param[Settings3Trigger].element == TriggerOnOffSave){
		Switch_Trigger3(Dixom.Basis.Settings.Param[SettingsState3Trigger].element);
	}else{
		Switch_Trigger3(OFF);
	}


	//28 Initialize USB Port Power State and VBUS Power State
	Power_VBUS(Dixom.Basis.Settings.Param[SettingsVbusTablet].element);
	Power_USB1(Dixom.Basis.Settings.Param[SettingsModeOnUSB1].element);
	Power_USB2(Dixom.Basis.Settings.Param[SettingsModeOnUSB2].element);

}


void Power_DCDC_33V_5V(uint8_t state){

	if(state == ON){
		HAL_GPIO_WritePin(GPIOB, DC_DC_Power_Pin, GPIO_PIN_RESET);
	}else{
		HAL_GPIO_WritePin(GPIOB, DC_DC_Power_Pin, GPIO_PIN_SET);
	}
	Dixom.Service.PeripheryState[PowerDCDC] = state;
}

void Up_Modules_Reset(uint8_t state){

	if(state == ON){
		HAL_GPIO_WritePin(GPIOA, Modules_Reset_Pin, GPIO_PIN_RESET);
	}else{
		HAL_GPIO_WritePin(GPIOA, Modules_Reset_Pin, GPIO_PIN_SET);
	}
	Dixom.Service.PeripheryState[Modules_Reset] = state;
}

void Power_LED(uint8_t state){

	if(state == ON){
		HAL_GPIO_WritePin(GPIOB, LED_Pin, GPIO_PIN_SET);
	}else{
		HAL_GPIO_WritePin(GPIOB, LED_Pin, GPIO_PIN_RESET);
	}
	Dixom.Service.PeripheryState[Led] = state;
}

void Power_USB1(uint8_t state){

	if(state == ON){
		HAL_GPIO_WritePin(GPIOA, VDD_5V_USB1_Pin, GPIO_PIN_SET);
	}else{
		HAL_GPIO_WritePin(GPIOA, VDD_5V_USB1_Pin, GPIO_PIN_RESET);
	}
	sprintf( (char *) Dixom.Buff, General_UsbPowerControl , 1, state);
	TransmitDataOutputs(Dixom.Buff, AutoLenght,  false,  true, allAvailable);
	Dixom.Service.PeripheryState[Usb1] = state;
}

void Power_USB2(uint8_t state){

	if(state == ON){
		HAL_GPIO_WritePin(GPIOC, VDD_5V_USB2_Pin, GPIO_PIN_SET);
	}else{
		HAL_GPIO_WritePin(GPIOC, VDD_5V_USB2_Pin, GPIO_PIN_RESET);
	}
	sprintf( (char *) Dixom.Buff, General_UsbPowerControl , 2, state);
	TransmitDataOutputs(Dixom.Buff, AutoLenght,  false,  true, allAvailable);
	Dixom.Service.PeripheryState[Usb2] = state;
}

void Power_VBUS(uint8_t state){
	if(state == ON){
		HAL_GPIO_WritePin(GPIOB, VDD_5V_Vbus_Pin, GPIO_PIN_SET);
	}else{
		HAL_GPIO_WritePin(GPIOB, VDD_5V_Vbus_Pin, GPIO_PIN_RESET);
	}
	Dixom.Service.PeripheryState[Vbus] = state;
}

void Switch_Trigger1(uint8_t state){
	if(state == ON){
		HAL_GPIO_WritePin(GPIOC, GND_Triger_CH1_Pin, GPIO_PIN_SET);
	}else{
		HAL_GPIO_WritePin(GPIOC, GND_Triger_CH1_Pin, GPIO_PIN_RESET);
	}
	Dixom.Service.PeripheryState[Trigger1] = state;
	Trigger_State_Show(SettingsState1Trigger, state);
}

void Switch_Trigger2(uint8_t state){
	if(state == ON){
		HAL_GPIO_WritePin(GPIOC, GND_Triger_CH2_Pin, GPIO_PIN_SET);
	}else{
		HAL_GPIO_WritePin(GPIOC, GND_Triger_CH2_Pin, GPIO_PIN_RESET);
	}
	Dixom.Service.PeripheryState[Trigger2] = state;
	Trigger_State_Show(SettingsState2Trigger, state);
}

void Switch_Trigger3(uint8_t state){
	if(state == ON){
		HAL_GPIO_WritePin(GPIOC, GND_Triger_CH3_Pin, GPIO_PIN_SET);
	}else{
		HAL_GPIO_WritePin(GPIOC, GND_Triger_CH3_Pin, GPIO_PIN_RESET);
	}
	Dixom.Service.PeripheryState[Trigger3] = state;
	Trigger_State_Show(SettingsState3Trigger, state);
}

void Switch_Dac_Reset(uint8_t state){
	if(state == ON){
		HAL_GPIO_WritePin(GPIOB, DAC_RESET_Pin, GPIO_PIN_SET);
	}else{
		HAL_GPIO_WritePin(GPIOB, DAC_RESET_Pin, GPIO_PIN_RESET);
	}
	Dixom.Service.PeripheryState[DacReset] = state;
}
void Switch_USB_Otg(uint8_t state){
	if(state == ON){
		HAL_GPIO_WritePin(GPIOA, OTG_Pin, GPIO_PIN_SET);
	}else{
		HAL_GPIO_WritePin(GPIOA, OTG_Pin, GPIO_PIN_RESET);
	}
	Dixom.Service.PeripheryState[Usb_Otg] = state;
}


void Power_CINV_12v(uint8_t state){
	if(state == ON){
		HAL_GPIO_WritePin(GPIOA, VDD_CINV_Pin, GPIO_PIN_SET);
	}else{
		HAL_GPIO_WritePin(GPIOA, VDD_CINV_Pin, GPIO_PIN_RESET);
	}
	Dixom.Service.PeripheryState[Cinv12v] = state;
}

void Power_Remout(uint8_t state){
	if(state == ON){
		HAL_GPIO_WritePin(GPIOC, VDD_Remot_Pin, GPIO_PIN_SET);
	}else{
		HAL_GPIO_WritePin(GPIOC, VDD_Remot_Pin, GPIO_PIN_RESET);
	}
	Dixom.Service.PeripheryState[Remout] = state;
}

void Power_Camera(uint8_t state){
	if(state == ON){
		HAL_GPIO_WritePin(GPIOC, VDD_Camera_Pin, GPIO_PIN_SET);
	}else{
		HAL_GPIO_WritePin(GPIOC, VDD_Camera_Pin, GPIO_PIN_RESET);
	}
	Dixom.Service.PeripheryState[Camera] = state;
}
void Switch_CS_DSP(uint8_t state){
	if(state == ON){
		HAL_GPIO_WritePin(GPIOC, SPI_CS_DSP_Pin, GPIO_PIN_SET);
	}else{
		HAL_GPIO_WritePin(GPIOC, SPI_CS_DSP_Pin, GPIO_PIN_RESET);
	}
	Dixom.Service.PeripheryState[CS_DSP] = state;
}
void Switch_CS_FLASH(uint8_t state){
	if(state == ON){
		HAL_GPIO_WritePin(GPIOB, SPI_CS_EEPROM_Pin, GPIO_PIN_SET);
	}else{
		HAL_GPIO_WritePin(GPIOB, SPI_CS_EEPROM_Pin, GPIO_PIN_RESET);
	}
	Dixom.Service.PeripheryState[CS_FLASH] = state;
}


void SetTrigger(uint8_t triggerNum, uint8_t pressed) {


	if (triggerNum == SettingsState1Trigger) {

		if (GetSettings(Settings1Trigger) == TriggerOnHolding) { //������� ���� ����������

			if (pressed == ON) {
				Switch_Trigger1(ON);
			} else {
				Switch_Trigger1(OFF);
			}
		} else if (GetSettings(Settings1Trigger) == TriggerOnOffNoSave && pressed == YES) { //���/���� ��� ����������

			if (GetSettings(SettingsState1Trigger) == ON) {
				Switch_Trigger1(OFF);
			} else {
				Switch_Trigger1(ON);
			}
		} else if (GetSettings(Settings1Trigger) == TriggerOnOffSave && pressed == YES){ //���/���� �� ����� � ����������� ��������� � EEPROM
			if (GetSettings(SettingsState1Trigger) == ON && pressed == YES) {
				Switch_Trigger1(OFF);
			} else if(pressed == YES){
				Switch_Trigger1(ON);
			}
		}
	}else if (triggerNum == SettingsState2Trigger) {

		if (GetSettings(Settings2Trigger) == TriggerOnHolding) { //������� ���� ����������

			if (pressed == ON) {
				Switch_Trigger2(ON);
			} else {
				Switch_Trigger2(OFF);
			}
		} else if (GetSettings(Settings2Trigger) == TriggerOnOffNoSave && pressed == YES) { //���/���� ��� ����������

			if (GetSettings(SettingsState2Trigger) == ON) {
				Switch_Trigger2(OFF);
			} else {
				Switch_Trigger2(ON);
			}
		} else if (GetSettings(Settings2Trigger) == TriggerOnOffSave && pressed == YES){ //���/���� �� ����� � ����������� ��������� � EEPROM
			if (GetSettings(SettingsState2Trigger) == ON && pressed == YES) {
				Switch_Trigger2(OFF);
			} else if(pressed == YES){
				Switch_Trigger2(ON);
			}
		}
	}else if (triggerNum == SettingsState3Trigger) {

		if (GetSettings(Settings3Trigger) == TriggerOnHolding) { //������� ���� ����������

			if (pressed == ON) {
				Switch_Trigger3(ON);
			} else {
				Switch_Trigger3(OFF);
			}
		} else if (GetSettings(Settings3Trigger) == TriggerOnOffNoSave && pressed == YES) { //���/���� ��� ����������

			if (GetSettings(SettingsState3Trigger) == ON) {
				Switch_Trigger3(OFF);
			} else {
				Switch_Trigger3(ON);
			}
		} else if (GetSettings(Settings3Trigger) == TriggerOnOffSave && pressed == YES){ //���/���� �� ����� � ����������� ��������� � EEPROM
			if (GetSettings(SettingsState3Trigger) == ON && pressed == YES) {
				Switch_Trigger3(OFF);
			} else if(pressed == YES){
				Switch_Trigger3(ON);
			}
		}
	}
}


void Trigger_State_Show(int8_t triggerNum, uint8_t state){

	//SetSettings(triggerNum, state);
	sprintf((char *)Dixom.Buff,"SETTINGS %hu %hu",triggerNum, state);
	TransmitDataOutputs(Dixom.Buff, AutoLenght,  false,  true, allAvailable);
}



void SET_STATE_USB(uint8_t ControlByte, uint8_t *Recive_USB){

	short usb    = 0,
	      state  = 0;
	sscanf((char *)Recive_USB,"%hu %hu", &usb, &state);
	if( usb<1 || usb>2){
 	    TransmitDataOutputs((uint8_t *)GeneralNotifi_CommandError, AutoLenght,  false,  true, allAvailable);

	}else{
		if(ControlByte == DATA_SET){ //SET
			if(state<0 || state>1){
 	  			TransmitDataOutputs((uint8_t *)GeneralNotifi_CommandError, AutoLenght,  false,  true, allAvailable);

			}else{
				if(usb == 1){
					Power_USB1(state);
				}else{
					Power_USB2(state);
				}
			}
		}else if (ControlByte == DATA_GET_ALL){
			sprintf( (char *) Dixom.Buff, General_UsbPowerControl , 1, Dixom.Service.PeripheryState[Usb1]);
			TransmitDataOutputs(Dixom.Buff, AutoLenght,  false,  true, allAvailable);
			sprintf( (char *) Dixom.Buff, General_UsbPowerControl , 2, Dixom.Service.PeripheryState[Usb2]);
			TransmitDataOutputs(Dixom.Buff, AutoLenght,  false,  true, allAvailable);
		}else if (ControlByte == DATA_GET){ //GET
			if(usb == 1){
				sprintf( (char *) Dixom.Buff, General_UsbPowerControl , 1, Dixom.Service.PeripheryState[Usb1]);
			}else{
				sprintf( (char *) Dixom.Buff, General_UsbPowerControl , 2, Dixom.Service.PeripheryState[Usb2]);
			}
			TransmitDataOutputs(Dixom.Buff, AutoLenght,  false,  true, allAvailable);
		}else{
  			TransmitDataOutputs((uint8_t *)GeneralNotifi_CtlBitNotSupported, AutoLenght,  false,  true, allAvailable);

		}
	}
}


void GET_STATE_USB(uint8_t *Recive_USB){


	    short usb    = 0,
	          stUsb  = 0;

		char data[20];

		sscanf((char *)Recive_USB,"%hu", &usb);

		if( usb<1 || usb>2 ){
 			TransmitDataOutputs((uint8_t *)GeneralNotifi_CommandError, AutoLenght,  false,  true, allAvailable);


		}else{

			 if(usb == 1){
				 stUsb = StateUSB1;
			 }else{
				 stUsb = StateUSB2;
			 }
			 sprintf( data,"USB_STATE %hu %hu",usb, stUsb);
 			 TransmitDataOutputs((uint8_t *)data, AutoLenght,  false,  true, allAvailable);

		}
}

void RearPinState(void){

	if(GetSettings(SettingsRearSignal) == 0){ //AnalogRead
		if(HAL_GPIO_ReadPin(GPIOC, Rear_Detected_Pin)==1){
			SetRearState(ON);
		}else{
			SetRearState(OFF);
		}
	}
}


void SetRearState(uint8_t state){
	if(state == ON){
		Dixom.Control.Analog.RearState = ON;
		Dixom.Service.PeripheryState[Camera] = ON;
	}else{
		Dixom.Control.Analog.RearState = OFF;
		Dixom.Service.PeripheryState[Camera] = OFF;
	}
}

//uint32_t Usb1Timer              = 0;
//uint32_t Usb2Timer              = 0;
uint32_t RearOffTimer           = 0;
uint8_t  RearUsb1OnState        = OFF;
uint8_t  RearUsb2OnState        = OFF;

uint32_t Usb1TimerIncrement     = 0;
uint32_t Usb2TimerIncrement     = 0;
uint16_t RearOnTimerCount       = 0;
uint8_t  StrtUsb1OnStatus       = 0;
uint8_t  StrtUsb2OnStatus       = 0;

#define UsbOnStartPlatform        0
#define UsbOffStartPlatform       1
#define UsbOnTimerStartPlatform   2
#define UsbOnRearDetected         3


uint8_t camOn = 0;

void OnOffRearCam(){
	if(RearOnOffBtn == OFF){
		RearOnOffBtn = ON;
		if(GetSettings (SettingsModeOnUSB1) == UsbOnRearDetected){
			Power_USB1(ON);
		}
		if(GetSettings (SettingsModeOnUSB2) == UsbOnRearDetected){
			Power_USB2(ON);
		}
		RearCamVolDwon();
		camOn = YES;
		Power_Camera(ON); //�������� �������� ������ ������
	}else{
		RearOnOffBtn = OFF;
		if(GetSettings (SettingsModeOnUSB1) == UsbOnRearDetected){
			Power_USB1(OFF);
		}
		if(GetSettings (SettingsModeOnUSB2) == UsbOnRearDetected){
			Power_USB2(OFF);
		}
		ReadCamVolUp();
		camOn = OFF;
		Power_Camera(OFF);

	}
}

void ActionRearState(void){

	if(RearOnOffBtn == OFF){

		if(Dixom.Control.Analog.RearState == 1){

		//	osDelay(GetTimer(TimerRearCam));


			if(GetTimer(TimerRearCam) == RearOnTimerCount){
				RearPinState();
				if(Dixom.Control.Analog.RearState ==1){

					RearOffTimer = 0;

					if(GetSettings (SettingsModeOnUSB1) == UsbOnRearDetected){

						if(StateUSB1 ==0){
							Power_USB1(ON);
							RearCamVolDwon();
							camOn = YES;
						}
						RearUsb1OnState = ON;
					}

					if(GetSettings (SettingsModeOnUSB2) == UsbOnRearDetected){

						if(StateUSB2 ==0){
							Power_USB2(ON);
							RearCamVolDwon();
							camOn=YES;
						}
						RearUsb2OnState = ON;
					}
					Power_Camera(ON); //�������� �������� ������ ������
				}
			}else{
				RearOnTimerCount++;
			}


		}else{
			//��������� ������ �������� ������������ ��� ������ ������
			RearOnTimerCount = 0;
			if(RearUsb1OnState == ON || RearUsb2OnState == ON){

				if(GetSettings (SettingsModeOnUSB1) == UsbOnRearDetected){
					if(GetTimer(TimerUsb1) == RearOffTimer){
						Power_USB1(OFF);
						RearUsb1OnState = OFF;
						ReadCamVolUp();
						camOn=OFF;
						if(RearUsb2OnState == OFF){
							Power_Camera(OFF);
						}
					}
				}

				if(GetSettings (SettingsModeOnUSB2) == UsbOnRearDetected){
					if(GetTimer(TimerUsb2) == RearOffTimer){
						Power_USB2(OFF);
						RearUsb2OnState = OFF;
						ReadCamVolUp();
						camOn=OFF;
						if(RearUsb1OnState == OFF){
							Power_Camera(OFF);
						}
					}
				}
				RearOffTimer++;
			}
		}

		if(StrtUsb1OnStatus == 0){
			if(GetSettings (SettingsModeOnUSB1) == UsbOnTimerStartPlatform){
				if(Dixom.Basis.Settings.Timer[TimerUsb1].element == Usb1TimerIncrement){
					Power_USB1(ON);
					StrtUsb1OnStatus = 1;
				}
				Usb1TimerIncrement++;
			}else{
				StrtUsb1OnStatus = 1;
			}
		}

		if(StrtUsb2OnStatus == 0){
			if(GetSettings (SettingsModeOnUSB2) == UsbOnTimerStartPlatform){
				if(Dixom.Basis.Settings.Timer[TimerUsb2].element == Usb2TimerIncrement){
					Power_USB2(ON);
					StrtUsb2OnStatus = 1;
				}
				Usb2TimerIncrement++;
			}else{
				StrtUsb2OnStatus = 1;
			}
		}
	}
}

uint8_t OldVolumeSettingsRead = 0;

void RearCamVolDwon(void){

	if(GetSettings(SettingsRearCamOnVolDivider) != 0 && GetSettings(SettingsCamOnMinVol) < DSP_GetVolume (0) && camOn == NO){
		OldVolumeSettingsRead = DSP_GetVolume (0);
		DSP_DirectVolumControl ( 0, OldVolumeSettingsRead/GetSettings(SettingsRearCamOnVolDivider));
		Dixom.Module.DSP.MuteATT.TouchedVolControl = 0;
	}else if (camOn != YES){
		OldVolumeSettingsRead = 0;
	}
}

void ReadCamVolUp(void){
	if(Dixom.Module.DSP.MuteATT.TouchedVolControl == 0 && GetSettings(SettingsRearCamOnVolDivider) != 0 && OldVolumeSettingsRead !=0 && camOn == YES){
		DSP_DirectVolumControl ( 0, OldVolumeSettingsRead);
	}
	Dixom.Basis.Settings.ParamNoSave[SettingsNoSave_VolUpCamera_State].element = 1;
}

void GPIO_Loop(){
	RearPinState();
	ActionRearState();
}






