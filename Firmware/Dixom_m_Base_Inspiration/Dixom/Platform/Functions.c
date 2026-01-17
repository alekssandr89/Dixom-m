/*
 * Main_Functions.c
 *
 *  Created on: Feb 8, 2022
 *      Author: 79825
 */
#include <Description.h>
#include <Functions.h>
#include <UpdateFirmware.h>
#include "Structures.h"
#include "stdint.h"
#include "string.h"
#include "cmsis_os.h"
#include "main.h"
#include "StructDeviceHID.h"
#include "ADC.h"
#include "Bluetooth.h"
#include "GPIO_Constants.h"
#include "DIX_usb.h"

extern sDixom Dixom;

#define dUsbStringRx Dixom.Exchange.CircularBuff.UsbStringRx

void Add_Packet_To_CircularBuffer_UsbStringRx(uint8_t* data, uint8_t len, uint8_t interface){

	if(dUsbStringRx.QueueFilling<USB_RX_QUEUE_PACKETS_NUM){
		for(uint8_t i = 0; i<USB_RX_BUFF_DATA_LEN; i++){
			dUsbStringRx.Buff[dUsbStringRx.TxQueuePointer][i] = data[i];
		}

		dUsbStringRx.TxQueuePointer++;

		if(dUsbStringRx.TxQueuePointer==USB_RX_QUEUE_PACKETS_NUM){
			dUsbStringRx.TxQueuePointer=0;
		}

		if(interface == usbHid){
			USB_Connected_UsbHid();

		}else if(interface == USB_CDC){
			USB_Connected_UsbCdc();
		}

		dUsbStringRx.QueueFilling++;
	}
}



void RefreshIWDG(void)
{
//	HAL_IWDG_Refresh(&hiwdg);
}

void Main_Function_Loop(){
	if ( Dixom.Service.PeripheryState[Usb_Otg] == OFF ){
		static uint32_t VbusTimer;
		if(VbusTimer == Dixom.Basis.Settings.Timer[TimerTabletVdd].element){

			/*Power_VBUS(ON);
			Delay(1000, FreeRTOS);
			Switch_USB_Otg(ON);
			Delay(1500, FreeRTOS);
			HID_NoSleepCommand(Dixom.Basis.Settings.Param[SettingsNoSleepComand].element);*/
		}
		VbusTimer++;
	}
	Power_Control_Loop();
}


void Power_Control_Loop(){

	static uint8_t  OldPlatformState;
	static uint32_t NoSleepTimer;
	static uint8_t  Ready_PowerDown;
	static uint32_t StartTimer;
	static uint16_t TimeeACCLowVoltage;
	static uint16_t StateACCLowVoltage;
	static uint32_t AccTimerOff;
	static uint32_t PlayTimer;
	static uint8_t  StatusLcdSleep;
	static uint8_t  SoundMuteState;

	if(StartTimer<25000){
		StartTimer++;
	}

	if( Dixom.Service.Acc_Light_ADC_Val[ACC] > 950 ){

		Dixom.Service.ACC.State = ON;

		if(Dixom.DevInfo.InMode != Initialization){
			if(SoundMuteState == 1){
				DAC_Mute(OFF);
				SoundMuteState = 0;
				Power_Remout(ON);
			}
		}

		if(Dixom.Service.ACC.Voltage < GetSettings(SettingsRemLowVoltageVal)){
			if(StateACCLowVoltage == 0){
				TimeeACCLowVoltage++;
				if(TimeeACCLowVoltage > 15000){
					Power_Remout(OFF);
					StateACCLowVoltage = 1;
				}
			}
		}else{
			TimeeACCLowVoltage = 0;
			if(StateACCLowVoltage == 1){
				Power_Remout(ON);
				StateACCLowVoltage = 0;
			}
		}

		AccTimerOff=0;
		if(Dixom.Basis.Settings.Param[SettingsStatusNoSleep].element==ON){
			if(NoSleepTimer>=Dixom.Basis.Settings.Timer[TimerNoSleep].element){
				NoSleepTimer=0;
				if(GetSettings(SettingsStatusNoSleep)==ON){
					HID_NoSleepCommand(Dixom.Basis.Settings.Param[SettingsNoSleepComand].element);
				}
			}
			NoSleepTimer++;
		}

		if(Dixom.DevInfo.InMode != Initialization && Dixom.Basis.Settings.Param[SettingsPlay].element==ON && PlayTimer <= Dixom.Basis.Settings.Timer[TimerPlay].element ){
			if(PlayTimer==Dixom.Basis.Settings.Timer[TimerPlay].element){
				HID_Play();

				if(GetSettings(SettingsPlay2) == ON){
					osDelay(3000);
					HID_Play();
				}
			}
			PlayTimer++;
		}

		if(Ready_PowerDown==YES){
			Launch_Dixom();
		}

		if(StatusLcdSleep == ON){
			StatusLcdSleep = OFF;
			Dixom.DevInfo.InMode = OldPlatformState;
		}

	}else{

		Dixom.Service.ACC.State = OFF;

		if(StartTimer>15000){
			if(SoundMuteState == 0){
				DAC_Mute(ON);
				SoundMuteState = 1;
				Power_Remout(OFF);
			}

			NoSleepTimer = Dixom.Basis.Settings.Timer[TimerNoSleep].element;;

			if(AccTimerOff == 2000){
				StatusLcdSleep = ON;
				OldPlatformState = Dixom.DevInfo.InMode;
				Dixom.DevInfo.InMode = WaitingForShutdown;
			}

			if(AccTimerOff==3500){

				Dixom.DevInfo.InMode = PowerDown;
				RefreshDisplay();
				//СДЕЛАТЬ СОХРАНЕНИЕ ПРИ ОТКЛЮЧЕНИИ!!!
				Structures_Write_To_Flash();
				Structures_Write_To_EEPROM();

				if( Dixom.Service.Acc_Light_ADC_Val[ACC] < 950 ){

					if(GetSettings(SettingsAccOffStatPause) == ON){
						AccOffMediaCommand();
					}
					Ready_PowerDown = YES;
					DAC_Mute(ON);
					Delay(1500, FreeRTOS);

					if(GetSettings(SettingsAccOffStatSleep) == ON){
						AccOffSystemCommand();
					}

					Delay(1000, FreeRTOS);

					if(Dixom.Service.Acc_Light_ADC_Val[ACC] < 950){
						SleepSTM32();
					}else{
						Ready_PowerDown = NO;

						Dixom.DevInfo.InMode = ReadyForWork;

						if(GetSettings(SettingsStatusNoSleep)==ON){
							HID_NoSleepCommand(Dixom.Basis.Settings.Param[SettingsNoSleepComand].element);
						}
					}

				}else{
					Dixom.DevInfo.InMode = ReadyForWork;
				}
			}
			AccTimerOff++;
		}
	}
}

void CURRENT_EXCHANGE_INTERFACE(uint8_t ControlByte, uint8_t *Received_String){

	uint8_t interface = 0, usb_hid = 0, usb_cdc = 0, bluetooth = 0;
	sscanf((char *)Received_String, "%hhu %hhu %hhu %hhu", &interface, &usb_hid , &usb_cdc , &bluetooth );

	if(ControlByte == DATA_SET){ //SET
		 if(interface < 3){

			 Dixom.Exchange.Interface.Current_interface = interface;
			 Dixom.Exchange.Interface.Manual_tx_usb_hid = usb_hid;
			 Dixom.Exchange.Interface.Manual_tx_usb_cdc = usb_cdc;
			 Dixom.Exchange.Interface.Manual_tx_bluetooth = bluetooth;
			 SetSettings(Setting_manual_tx_usb_hid, usb_hid);
			 SetSettings(Setting_manual_tx_usb_cdc, usb_cdc);
			 SetSettings(Setting_manual_tx_usb_bluetooth, bluetooth);

			 /* can take state 1 or 0
	          * if 0 = the current interface is changed to the interface from which the set or get commands were received
		      * if 1 = the current interface is set manually and does not change when receiving set or get commands from the interface*/
			 GetCurrentExchangeInterface();
		 }else{
			 TransmitDataOutputs((uint8_t *)GeneralNotifi_CommandError, AutoLenght,  false,  true, allAvailable);
		 }
	}else if(ControlByte == DATA_DEFAULT){
 		 TransmitDataOutputs((uint8_t *)GeneralNotifi_NoDefaulOption, AutoLenght,  false,  true, allAvailable);

	}else if(ControlByte == DATA_GET){ //GET
		GetCurrentExchangeInterface();
    }else{
 		 TransmitDataOutputs((uint8_t *)GeneralNotifi_CtlBitNotSupported, AutoLenght,  false,  true, allAvailable);

    }
}





void GetCurrentExchangeInterface(){

	sprintf((char *)Dixom.Buff, General_Connection,
			Dixom.Exchange.Interface.Current_interface,
			Dixom.Exchange.State.USB.ConnectionHid,
			Dixom.Exchange.Interface.Manual_tx_usb_hid,
			Dixom.Exchange.State.USB.ConnectionCdc,
			Dixom.Exchange.Interface.Manual_tx_usb_cdc,
			Dixom.Module.Bluetooth.Connection,
			Dixom.Exchange.Interface.Manual_tx_bluetooth);

 	 TransmitDataOutputs(Dixom.Buff, AutoLenght,  false,  true, allAvailableForced);


}

void SleepSTM32(void){
	SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk; //для M3 разрешаем sleepdeep
	PWR->CR  |= PWR_CR_PDDS;//выбираем режим Power Down Deepsleep
	PWR->CR  |= PWR_CR_CWUF ; //очищаем wakeup flag
	PWR->CSR |= PWR_CSR_EWUP; //разрешаем вэйкап, то есть пробуждение по переднему фронту на А0
	__WFI();  //уснули
}


void CMD_IWDG_SET(uint8_t ControlByte, uint8_t *Received_String){

	if(ControlByte == DATA_SET){  //SET
		uint8_t   StateIWDG  = ON;
		sscanf((char *)Received_String, "%hhu ", &StateIWDG);
		if(StateIWDG<2){
			FlashMemoryWriteEnable();
			FlashSectorErase(255);
			FlashMemoryWriteEnable();
			Dixom.DevInfo.Firmware_UpdateInfo.StateIWDG = StateIWDG;
			Spi_Flash_Write_CheckSumm(&Dixom.DevInfo.Firmware_UpdateInfo.CheckSum, SpiFlashAddrUpdateInfo, (uint32_t*)&Dixom.DevInfo.Firmware_UpdateInfo, sizeof(Dixom.DevInfo.Firmware_UpdateInfo)/4);
			FlashMemoryWriteDisable();
			sprintf( (char *) Dixom.Buff, General_State_IWDG , Dixom.DevInfo.Firmware_UpdateInfo.StateIWDG);
		 	 TransmitDataOutputs(Dixom.Buff, AutoLenght,  false,  true, allAvailable);
		}else{
 		 	 TransmitDataOutputs((uint8_t *)GeneralNotifi_CommandError, AutoLenght,  false,  true, allAvailable);

		}
	}else{ //GET
		sprintf( (char *) Dixom.Buff, General_State_IWDG , Dixom.DevInfo.Firmware_UpdateInfo.StateIWDG);
	 	 TransmitDataOutputs(Dixom.Buff, AutoLenght,  false,  true, allAvailable);
	}
}


void PlatformHrdRestart(){

	NVIC_SystemReset();
}

void PlatformRestart(uint8_t from){
	DAC_Mute(ON);
	Power_Remout(OFF);
	Delay(1000, from);
	NVIC_SystemReset();
}

void Delay(uint32_t timeDelay, uint8_t from){
	if(timeDelay>0){
		if(from == FreeRTOS)      {	osDelay(timeDelay);	                                          }
		else if(from == Interrupt){	for(uint32_t timer = 0; timer < timeDelay*(1000); timer++ ){ }}
		else if(from == Hal_lib)  {	HAL_Delay(timeDelay);	                                      }
	}
}
