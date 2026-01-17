#include <Functions.h>
#include <Main.h>
#include <Structures.h>
#include <UpdateFirmware.h>
#include "GPIO.h"
#include "Encoder.h"
#include "Bluetooth.h"
#include "FmRadio.h"
#include "Exchange.h"
#include "Launch.h"
#include "stm32f4xx_hal.h"
#include "cmsis_os.h"
#include "display_Menu.h"
#include "main_Menu.h"
#include "ADC.h"
#include "StructDeviceHID.h"
#include "Arduino.h"
#include "Nextion_LCD.h"
#include "Description.h"
#include "Control.h"
#include "RTC.h"
#include "Streams.h"
#include <CrystalSix140DThermometer.h>

extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim6;
extern TIM_HandleTypeDef htim7;
extern TIM_HandleTypeDef htim8;
extern IWDG_HandleTypeDef hiwdg;
extern sDixom Dixom;



void Task_Main_Setup(){
	Launch_Dixom();

	Dixom.Module.UsbAudio.StateSize = sizeof(Dixom.Module.UsbAudio.State);

	HAL_TIM_Encoder_Start(&htim3,TIM_CHANNEL_1);  //ENCODER CHANNEL 1
	HAL_TIM_Encoder_Start(&htim8,TIM_CHANNEL_1);  //ENCODER CHANNEL 2
	HAL_TIM_Base_Start_IT(&htim6);
	HAL_TIM_Base_Start_IT(&htim7);
	ADC_Start_DMA();

}


#define  RTC_polling_rate  500         //опросс каждые 0.5 секунды
#define  SoundCard_polling_rate  1000  //опросс каждую секунду
#define  FmRadio_polling_rate    2000  //опросс каждые 2 секунды

#define dUsbStringRx Dixom.Exchange.CircularBuff.UsbStringRx

void Task_Main_Infinite_Loop(){

	if(Dixom.DevInfo.InMode != Initialization){

		///	---Executing commands from the ring buffer--- ///

		if(dUsbStringRx.QueueFilling!=0){

			main_Menu(&dUsbStringRx.Buff[dUsbStringRx.RxQueuePointer][0]);
			dUsbStringRx.RxQueuePointer++;

			if(dUsbStringRx.RxQueuePointer==USB_RX_QUEUE_PACKETS_NUM){
				dUsbStringRx.RxQueuePointer=0;
			}
			dUsbStringRx.QueueFilling--;

		}else{

			///	---Performing various actions in the absence of commands--- ///
			Bluetooth_Loop();
			canRxLoop();
			DSP_Loop();
			Main_Function_Loop();
			Control_Loop();
			FM_Radio_Loop();
			GPIO_Loop();
			Regiser_Read_Loop();
			AppCameraAutomatic();

			if(Dixom.DevInfo.InMode == ReadyForWork){
				static uint32_t FmRadio_Time;
				if(FmRadio_Time+FmRadio_polling_rate < HAL_GetTick()){
					FM_Radio_Execution_With_Time_Interval_2000ms();
					FmRadio_Time = HAL_GetTick();
				}
			}
		}
	}
}

void AppCameraAutomatic(){
	if(Dixom.Basis.Settings.ParamNoSave[SettingsNoSave_VolUpUsbConnect_State].element == 1
			|| Dixom.Basis.Settings.ParamNoSave[SettingsNoSave_VolUpCamera_State].element == 1){
		/*Автоматическое сворачивание приложения, нажатие кнопки PLAY, и увеличение громкости планшета если есть USB подключение*/
		if(Dixom.Exchange.State.USB.ConnectionHid == Connected || Dixom.Exchange.State.USB.ConnectionCdc == Connected){

			if(Dixom.Basis.Settings.ParamNoSave[SettingsNoSave_VolUpCamera_State].element == 1){
				if(Dixom.Basis.Settings.Param[SettingsBackRearCameraOff].element == ON){
					Delay(100, FreeRTOS);
					HID_ALT_TAB();
					Delay(1000, FreeRTOS);
				}else{
					Delay(1000, FreeRTOS);
				}

				if(Dixom.Basis.Settings.Param[SettingsPlayCameraOff].element == ON){
					HID_Play(); /*Нажатие кнопки плей*/
					Delay(50, FreeRTOS);
				}
			}else{
				Delay(1000, FreeRTOS);
			}

			if(Dixom.Basis.Settings.Param[SettingsVolUpUsbConnect].element == ON){

				for(uint8_t i = 0; i <20; i++){
					Delay(50, FreeRTOS);
					HID_VolUP(); /*Увеличение громкости планшета*/
				}
			}
		}
		Dixom.Basis.Settings.ParamNoSave[SettingsNoSave_VolUpUsbConnect_State].element = 0;
		Dixom.Basis.Settings.ParamNoSave[SettingsNoSave_VolUpCamera_State].element = 0;

	}
}

uint32_t   testCount = 0;
boolean testResponse = true;

void setTestCount(uint32_t cnt, boolean response){
	testCount = cnt;
	testResponse = response;
}

void Task_Perifer_Service_Infinite_Loop(){

	if(Dixom.DevInfo.InMode == ReadyForWork){

		if(testCount!=0){
			testCount--;
			sprintf( (char*)Dixom.Buff, MaskReceiveDataLossTest , testCount);
	 	 	TransmitDataOutputs(Dixom.Buff, AutoLenght,  false,  testResponse, allAvailable);
		}

		ScanBtDeviceDisconnecting();
		Nextion_Loop();
		Arduino_Loop();
	}
}
uint16_t TimeutTimerUsbHidSend = 500;
uint8_t oldInterface = 0;
//Accuracy 0.1 sec




void Task_Execution_With_Time_Interval(){



	if(oldInterface != Dixom.Exchange.Interface.Current_interface){
		oldInterface = Dixom.Exchange.Interface.Current_interface;
		GetCurrentExchangeInterface();
	}


	if(Dixom.Exchange.State.USB.ConnectionHid == Connected || Dixom.Module.Bluetooth.Connection == Connected){
		if(Dixom.Exchange.Interface.TimeutTimerUsbHidSend > TimeutTimerUsbHidSend){
 	    	TransmitDataOutputs((uint8_t *)EmptySystemMessage, 5,  false,  false, allAvailableForced);

			Dixom.Exchange.Interface.TimeutTimerUsbHidSend = 0;
		}else{
			Dixom.Exchange.Interface.TimeutTimerUsbHidSend++;
		}
	}

	if(Dixom.DevInfo.InMode == ReadyForWork){

		if(Dixom.Basis.Settings.ParamNoSave[SettingsNoSave_Task_Execution_State].element == ON){
			static uint32_t RTC_Time;
			static uint32_t SoundCard_Time;


			if(RTC_Time+RTC_polling_rate < HAL_GetTick()){
				RTC_GettingTime();
				RTC_Time = HAL_GetTick();
			}

			if(SoundCard_Time+SoundCard_polling_rate < HAL_GetTick()){
				if(Dixom.Module.UsbAudio.InMode == ReadyForWork && Dixom.Basis.Settings.Param[SettingsAutocSoundCardPolling].element == ON){
					ExchangeStatus(	Receiver_I2C2( DEVICE_ADDR_DIXOM_UsbAudio<<1, ADDR_REG_USB_AUDIO_STATE, 1, (uint8_t*)&Dixom.Module.UsbAudio.State,
							Dixom.Module.UsbAudio.StateSize, 300, ExchangeMemMainSteamDMA), USB_SOUND_CARD_READ);
				}
				SoundCard_Time = HAL_GetTick();
			}

			TEST_Stream();
			CrystalSix140DTemperatureReadStream();
		}
	}
}


void Task_Display_Infinite_Loop(){

	if(Dixom.DevInfo.InMode != Initialization){
		DisplayRefresh();
	}
}

void Task_Interaction_Infinite_Loop(){

	if(Dixom.DevInfo.InMode != Initialization){

		static uint16_t SecontTik;
		if(SecontTik>1000){
			SecontTik = 0;
			Dixom.DevInfo.PlatormLife.Working_Hours = Dixom.DevInfo.PlatormLife.Working_Hours+1;
		}
		SecontTik++;

		ENCODER1();
		ENCODER2();
		Can_Interaction_Loop();
	}
}

void Task_Flashing_LED_Indicator_Infinite_Loop(){


	if(Dixom.DevInfo.Firmware_UpdateInfo.StateIWDG == 1){
		HAL_IWDG_Refresh(&hiwdg);
	}

	if(Dixom.DevInfo.InMode == ReadyForWork){
		HAL_GPIO_TogglePin(GPIOB, LED_Pin);
		osDelay(100);
		HAL_GPIO_TogglePin(GPIOB, LED_Pin);
		osDelay(100);
		HAL_GPIO_TogglePin(GPIOB, LED_Pin);
		osDelay(100);
		HAL_GPIO_TogglePin(GPIOB, LED_Pin);
		osDelay(1200);
		Dixom.Service.CrashedTimeout = 0;
	}else if (Dixom.DevInfo.InMode==Initialization){
		HAL_GPIO_TogglePin(GPIOB, LED_Pin);
		osDelay(100);
		HAL_GPIO_TogglePin(GPIOB, LED_Pin);
		osDelay(400);
	}else if(Dixom.DevInfo.InMode==FirmwareUpdate){
		HAL_GPIO_TogglePin(GPIOB, LED_Pin);
		osDelay(100);
		HAL_GPIO_TogglePin(GPIOB, LED_Pin);
		osDelay(100);
		Dixom.Service.CrashedTimeout = 0;
	}else if(Dixom.DevInfo.InMode==PowerDown){
		HAL_GPIO_TogglePin(GPIOB, LED_Pin);
		osDelay(400);
		HAL_GPIO_TogglePin(GPIOB, LED_Pin);
		osDelay(400);
		Dixom.Service.CrashedTimeout = 0;
	}
}






