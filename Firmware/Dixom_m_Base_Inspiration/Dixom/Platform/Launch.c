/*
 * Launch.c
 *
 *  Created on: 10 янв. 2022 г.
 *      Author: 79825
 */
#include <Information.h>
#include <Memory_Eeprom.h>
#include <Memory_Spi.h>
#include <Memory_Stm.h>
#include <UpdateFirmware.h>
//#include <UpdateBootloader.h>
#include "Launch.h"
#include "GPIO.h"
#include "licens.h"
#include "display_Menu.h"
#include "Bluetooth.h"
#include "FmRadio.h"
#include "UsbAudio.h"
#include "Sigma.h"
#include "DisplayMain.h"
#include "Exchange.h"
#include "Functions.h"
#include "main.h"

extern sDixom Dixom;

void Launch_Dixom(){

	//BootloaderUpdate(); //обновлене загрузчика со старой версии на новую

	Dixom.DevInfo.InMode = Initialization;
	Dixom.Module.Bluetooth.InMode = Initialization;
    Dixom.Module.UsbAudio.InMode = Initialization;
    Dixom.Exchange.Interface.Current_interface = usbHid;
    Dixom.Exchange.Interface.Current_encoding = ASCII;

    //1 Loading all structures and initializing chips
	Launch_Sequence();

	//2 Firmware health check
	Launch_Checking_The_Firmware();

	//3 Firmware error protection
    Launch_Emergency_Protection();

    //4 Initialization Bluetooth
    Initialization_Bluetooth();

    //5 Getting information about firmware and modules
	Launch_Update_Information();

	//6 Completing the firmware download
	Launch_Successful();

	Dixom.DevInfo.InMode = ReadyForWork;
}

void Launch_Sequence(){

	//1 Gpio Initialization
	Launch_Initialization_Gpio();

	//3 Turn on the power line 12v
	Software_CRC32_Initialization();

	Switch_Dac_Reset(ON);

	Delay(500,FreeRTOS);

	//5 Initialization of communication periphery
	Exchange_Initialization();

	//4 Display initialization, for external displays
	Display_Initialization();

	//6 Initialization of structures from STM32 flash memory
	Structures_Initialization_From_Flash ();

	//7 Initialization of structures from external EEPROM memory
	Structures_Initialization_From_EEPROM();

	//8 Final adjustment of structures for successful work
	Structures_Initialization_Final();

	//9 Trigger state initialization
	Configured_Initialization_Gpio();

	//10 Automatic detection of Audio DAC connection and its initialization
	Automatic_Detection_Audio_DAC_And_Initialization();

	//11 Turn on the mute mode for the audio DAC to eliminate audible clicks.
	DAC_Mute(ON);

	//11 Turn on the mute mode for the audio DAC to eliminate audible clicks.
	Up_Modules_Reset(OFF);

	//11 Turn on the mute mode for the audio DAC to eliminate audible clicks.
	Delay(1000,FreeRTOS);

	//16 Automatic detection of USB Audio Card connection and its initialization
	Initialization_DSP_Audio_Processor();

	//19 Setting the volume level depending on the selected volume control mode
	DSP_StartVolControlSettings();

	//15 Automatic detection of USB Audio Card connection and its initialization
	Automatic_Detection_USB_Audio_Card_And_Initialization();

	//10 Automatic detection of FM Radio connection and its initialization
	Automatic_Detection_FM_Radio_And_Initialization();
}

void Launch_Successful(){
	//1 Disable Mute mode in the DAC
	DAC_Mute(OFF);

	//2 We send information to the external display that the platform loading is completed
	Display_Loading_Complete();

	//3 Final adjustment of various parameters before going into normal operation.
	Dixom.Service.ACC.State = ON;
	Dixom.Module.DSP.MuteATT.TouchedVolControl = NO;

	//11 Turn on the mute mode for the audio DAC to eliminate audible clicks.
	Power_Remout(ON);
}


void Launch_Update_Information(){

	Initialization_Device_Information();

	Activation_Key_Initialization();

	Initialization_Firmware_Information();
}



/*
 * The function checks the firmware for errors after the update. Those. after updating the firmware,
 * if the firmware does not load before this function, then the flag in Dixom.DevInfo.UpdateInfo.State
 * will not be set to Ok_Frimware, and at the next boot, the loader will understand that the update was not successful,
 *  or there are problems in the firmware and will boot in mode bootloader.
 *
 */
void Launch_Checking_The_Firmware(){

	static uint8_t ReadingAttempts;

	Spi_Flash_Read_CheckSumm(&Dixom.Service.CheckSum.RX, SpiFlashAddrUpdateInfo, (uint32_t*)&Dixom.DevInfo.Firmware_UpdateInfo, sizeof(Dixom.DevInfo.Firmware_UpdateInfo)/4);

	if(Dixom.Service.CheckSum.RX != Dixom.DevInfo.Firmware_UpdateInfo.CheckSum){
		if(ReadingAttempts<5){
			ReadingAttempts++;
			Delay(50, FreeRTOS);
			Launch_Checking_The_Firmware();
		}
	}else{
		if(Dixom.DevInfo.Firmware_UpdateInfo.State!= Ok_Frimware){

			if(Dixom.DevInfo.Firmware_UpdateInfo.Boot_VerFrimware > 400 && Dixom.DevInfo.Firmware_UpdateInfo.Boot_VerFrimware < getCurrentBootVersion() && Dixom.DevInfo.Firmware_UpdateInfo.StateIWDG == 0){
			//	Only_BootUpdate();
			}

			FlashMemoryWriteEnable();
			FlashSectorErase(255);
			FlashMemoryWriteEnable();
			Dixom.DevInfo.Firmware_UpdateInfo.State = Ok_Frimware;
			Dixom.DevInfo.Firmware_UpdateInfo.StateIWDG = 0;
			Spi_Flash_Write_CheckSumm(&Dixom.DevInfo.Firmware_UpdateInfo.CheckSum, SpiFlashAddrUpdateInfo, (uint32_t*)&Dixom.DevInfo.Firmware_UpdateInfo, sizeof(Dixom.DevInfo.Firmware_UpdateInfo)/4);


			FlashMemoryWriteDisable();
		}
	}
}



void Launch_Emergency_Protection(){
	if(Dixom.DevInfo.Firmware_UpdateInfo.StateIWDG == 1){
		IWDG_Init();
	}
}


void BootloaderUpdate(){


	Dixom.DevInfo.PlatormLife.Num_Eeprom_Write = 0;
	Dixom.DevInfo.PlatormLife.Num_Flash_Write = 0;
	Dixom.DevInfo.PlatormLife.Working_Hours = 0;
	Dixom.DevInfo.PlatormLife.UseExperimentalFunctions = 0;
	Transmit_EEPROM( EEPROM_ADDR_PLATFORM_LIFE, (uint8_t *)&Dixom.DevInfo.PlatormLife, sizeof(Dixom.DevInfo.PlatormLife), EEPROM_TIME_OUT );

	FlashMemoryWriteEnable();
	FlashSectorErase(255);
	FlashMemoryWriteEnable();
	Dixom.DevInfo.Firmware_UpdateInfo.State = Bad_Frimware;
	Dixom.DevInfo.Firmware_UpdateInfo.StateIWDG = 1;
	Spi_Flash_Write_CheckSumm(&Dixom.DevInfo.Firmware_UpdateInfo.CheckSum, SpiFlashAddrUpdateInfo, (uint32_t*)&Dixom.DevInfo.Firmware_UpdateInfo, sizeof(Dixom.DevInfo.Firmware_UpdateInfo)/4);
	FlashMemoryWriteDisable();

	Only_BootUpdate();

	Delay(500, FreeRTOS);
	NVIC_SystemReset();
}















