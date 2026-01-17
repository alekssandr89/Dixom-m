#ifndef _MAIN_STRUCT_H_
#define _MAIN_STRUCT_H_

#include "Control.h"
#include "DSP.h"
#include "FmRadio.h"
#include "Settings.h"
#include "AudioDAC.h"
#include "UsbAudio.h"
#include "Bluetooth.h"
#include "Exchange.h"
#include "DisplayMain.h"
#include "GPIO.h"
#include "RTC.h"
#include "Can_Bus.h"
#include "GPIO_Constants.h"

WorkRes       Structures_Initialization_Final();
WorkRes       Structures_Initialization_From_Flash ();
void          Save_Structure_When_Power_Off();
WorkRes       Structures_Write_To_Flash ();
WorkRes       Structures_Initialization_From_EEPROM();
WorkRes       Structures_Write_To_EEPROM();

void          SET_DEFAULT_STRUCTURES(uint8_t ControlByte, uint8_t *Received_String);
WorkRes       Structures_Default(uint8_t Structures_enum);


void          Loading(uint8_t func, uint8_t status );

typedef struct
{
	uint32_t    *PTR;                    // 1 word
	uint32_t     RX;                     // 1 word
}sCheckSum;

typedef struct
{
    uint32_t     Working_Hours;         // Total device time in seconds
    uint32_t     Num_Eeprom_Write:31;      // Number of writes per eeprom memory
    uint32_t     UseExperimentalFunctions:1;      // Number of writes per eeprom memory
    uint16_t     Num_Flash_Write;       // Number of writes per stm32 flash memory
}sPlatormLife;

typedef struct
{
	uint32_t     CheckSum;              // Is the checksum of the sDevInfo structure.
	uint32_t     ApplicationAddr;       // Location address of the main firmware in STM32 memory
	uint8_t      State :7;              // Update system status
	uint8_t      StateIWDG :1;          // IWDG On Off state
	uint8_t      Erase_Sector :4;       // Initial sector of stm32 flash must be erased to load new firmware
	uint8_t      Erase_Count  :4;       // The number of erasable sectors relative to the initial sector
	uint16_t     Boot_VerFrimware;      // Bootloader firmware version
	uint32_t     Firmware_Size;         // New firmware size
	uint32_t     Frimware_CRC;          // �hecksum of the new firmware
}sUpdateInfo;

typedef struct
{
	uint32_t     CheckSum;              // Is the checksum of the sDevInfo structure.
	uint8_t 	 InMode;                // The mode the platforms are in
	uint16_t     dRevPCB;               // PCB revision
	uint8_t      dType;                 // Device type
    uint8_t      dModel;                // Device model
    uint8_t      dFrimwareName;         // Device model
	uint8_t      ActivationStatus;      // Device activation status
	uint16_t     dVerFrimware;          // Platform firmware version
	sPlatormLife PlatormLife;           // Operating time, number of flash memory rewriting cycles STM32
	uint8_t      RevPCB[4];             // PCB version in string form
	uint8_t      Type[16];              // Device type in string form
	uint8_t      Model[16];             // Device model in string form
	uint8_t      VerBootloader[6];      // Bootloader firmware version in string form
	uint8_t      VerFrimware[6];        // Firmware version in string form
	uint8_t      FirmwareName[12];      // Firmware name in string form
	uint8_t      SerialNumber[21];      // Serial number in string form
	uint8_t      LicenseKey[14];        // License key in string form
	uint8_t   	 MiniSetting[8];        // Various mini settings
	sDateTime    DateTime;              // Date and time
	sUpdateInfo  Firmware_UpdateInfo;   // Structure for servicing the update system and managing the bootloader
} sDevInfo;  //size 32

typedef struct
{
	uint8_t         Firmaware;
	uint8_t         Struct[NUMBER_StatusStructTDE];
} sStatus;

typedef struct
{
	uint8_t         State;
	uint8_t         Voltage;
} sACC;

typedef struct
{
	sCheckSum          CheckSum;
	uint8_t            NeedToSave;                //If 1(true) then we save the structure into stm32 memory. if 0(false) then no.
	uint8_t            DixomLoading;
	uint8_t            Loading[NumberFunction];
	sStatus            Status;
	uint16_t           CrashedTimeout;
	uint8_t            PeripheryState[32];
	uint32_t           crc_table[256];
    sACC               ACC;
    volatile uint16_t  Btn_ADC_Val [4];
    volatile uint16_t  Acc_Light_ADC_Val [2];
} sService;

typedef struct
{
	uint8_t StateTextDisplay;
	uint8_t StateOledDisplay;
	uint8_t Brightness;
} sI2CDisplays;


typedef struct
{
	sUsbAudio      UsbAudio;
	sBluetooth     Bluetooth;
	sFmRadio       FmRadio;
	sDSP           DSP;
	sMiniDSP       MiniDSP;
	sAudioDAC      AudioDAC;
	sI2CDisplays   Display;
} sModule;

typedef struct
{
	sSettings       Settings;
} sBasis;

typedef struct
{
	uint8_t         Buff[256];

	uint32_t        CalculateCRC;
	sDevInfo        DevInfo;
	sModule         Module;
	sBasis          Basis;
	sTimer          Timer;
	sControl        Control;
	sService        Service;
	sExchange       Exchange;
	sCan            Can;
} sDixom;






#endif /* MAIN_STRUCT_H_ */
