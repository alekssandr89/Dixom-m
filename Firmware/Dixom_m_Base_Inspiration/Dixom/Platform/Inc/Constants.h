#ifndef _MAIN_CONST_H_
#define _MAIN_CONST_H_

#include "stdint.h"

#define LENGTH_CIRCULAR_BUFF    1024
#define LENGTH_DIXOM_BUFF       64
#define LENGTH_SCAN_BUFF        3

//#define MAIN_APLICATION_ADDR 0x08008000
#define MAIN_APLICATION_ADDR 0x08020000

typedef struct{
	uint16_t      Size;                //| structure size
	uint8_t       Version;             //| structure version
	uint8_t       Status:4;            //| Displaying the state of the structure, the presence of errors, etc.
	uint8_t       NeedToSave:1;        //| If 1(true) then we save the structure into stm32 memory. if 0(false) then no.
	uint8_t       reserved0:3;         //|
	uint32_t      reserved1;           //|
} sStructInfo;

typedef enum
{
	NotInitialized              = 0,  // Initialization of parameters.
	Connected                   = 1,  // There are connections
	Connecting                  = 2, // Waiting for connection
	NotConnected                = 3, // No connection
	Disconnecting               = 4, // Disconnecting
	NotFound                    = 5, // Nothing found
	Available                   = 6, // Nothing found
	NotAvailable                = 7, // No connection
	Ok                          = 8, // Nothing found
	Error                       = 9, // No connection
	Enable                      = 10,
	Disable                     = 11,
    changed                     = 12,
    noPermission                = 13,
    notSupported                = 14,
}WorkStateTDE;


typedef enum
{
	Initialization              = 0,  // Initialization of parameters.
	ReadyForWork                = 1,  // Ready for normal operation
	SafeMode                    = 2,  // Errors occurred during initialization, some structures are in standard configuration
	DataSave                    = 3,  // Saving data to energy-independent memory
	WaitingForShutdown          = 4,  // Waiting for platform shutdown
	FirmwareUpdate              = 5,  // Platform firmware update
	DefaultStruct               = 6,  // Structure in standard mode
	PowerDown                   = 7,  // The platform is preparing for a power outage
	BootLoader                  = 8,  // Bootloader mode
	OutgoingCall                = 9,  // Outgoing bluetooth call
	IncomingCall                = 10,  // Incoming bluetooth call
	ActiveCall                  = 11,  // Bluetooth active call
	Stopped                     = 12, // In stop mode
	Playing                     = 13, // In playback mode
	Paused                      = 14, // Pause mode
	FastForwarding              = 15, // Fast forward playback
	FastRewinding               = 16, // Fast backward playback
	DebugMode                   = 17, // The device is in developer mode
}DeviceStateTDE;

//Result of work
typedef enum
{
	WR_OK                       = 0x00U,
	WR_ERROR                    = 0x01U,
	WR_BUSY                     = 0x02U,
	WR_TIMEOUT                  = 0x03U,
	WR_QUEUE_TIMEOUT            = 0x04U,
	WR_NO_CONNECTION            = 0x05U,
	WR_CRC_ERROR                = 0x06U,
	WR_OTHER                    = 0x07U
}WorkRes;


#define NUMBER_StatusStructTDE    12

typedef enum
{
	All_enum                    = 0,
	Settings_enum               = 1,
	Control_enum                = 2,
	Module_DSP_enum             = 3,
	Module_AudioDAC_enum        = 4,
	Module_FmRadio_enum         = 5,
	Module_MiniDSP_enum         = 6,
	EEPROM_DSP_enum             = 7,
	EEPROM_RADIO_enum           = 8,
	EEPROM_SETTINGS_enum        = 9,
	CAN_enum                    = 10,
	BLUETOOTH_enum              = 11,
}StatusStructTDE;
/*
typedef enum
{
	CMD_DEFAULT             = 0,
	CMD_FLASH_SAVE          = 1,
	CMD_EEPROM_SAVE         = 2,
	CMD_SHOW_ALL_MOD        = 3,
	CMD_I2C1_SCAN           = 4,
	CMD_I2C2_SCAN           = 5,
	CMD_BOOTUPDATE          = 6,

}CodeCMD_TDE;
*/
typedef enum
{
	Not_Save                    = 0,
	Save                        = 1
}tDataSaveSate;;

typedef enum
{
	ACC  =0,
	Light  =1
}ADC_Acc_Light;


#define CCMRAM __attribute__((section(".ccmram")))

#define TYPEPROGRAM_8bit     0x00000000U  /*!< Program byte (8-bit) at a specified address           */
#define TYPEPROGRAM_16bit    0x00000001U  /*!< Program a half-word (16-bit) at a specified address   */
#define TYPEPROGRAM_32bit    0x00000002U  /*!< Program a word (32-bit) at a specified address        */
#define TYPEPROGRAM_64bit    0x00000003U  /*!< Program a double word (64-bit) at a specified address */

#define NumberFunction            10  /*!< Program a double word (64-bit) at a specified address */


//#define    DixomC12              0
//#define    DixomM                1
#define    FConv                 0.01



/************DAC select***********/
#define DAC_NotConnected          0
#define DAC_ADAU1962              1
#define DAC_AK4458VN              2
#define DAC_PCM5102A              3
#define DAC_PCM1681               4
#define DAC_AK4440                5
#define DAC_TAS3251               6
#define DAC_PCM1690               7
#define AMPLIFIER_3xTAS3251       8


#define ByteControl_Dev_AK4440     0
#define ByteControl_Dev_ADAU1962   1
#define ByteControl_Dev_AK4458     2
#define ByteControl_Dev_RDA5807    3
#define ByteControl_Dev_DS3231     4
#define ByteControl_Dev_TAS3251    5
#define ByteControl_Dev_SI4735     6
#define ByteControl_Dev_ADAU1452   7
#define ByteControl_Dev_24LC256    8
#define ByteControl_Dev_25Q80DV    9
#define ByteControl_Dev_TDF8551J   10
#define ByteControl_Dev_PCM1690    11
#define ByteControl_Dev_STM32F405  12



typedef enum
{
	Loading_Start                  = 0,
	Loading_Init_StmFlash          = 1,
	Loading_Settings_Struct_Init   = 2,
	Loading_Settings               = 3,
	Loading_DSP                    = 4,
	Loading_Control                = 5
}FunctionTDE;

/*********BLUETOOTH Decoder********/
#define BTDecoder_NO             '0'
#define BTDecoder_CBC            '1'
#define BTDecoder_MP3            '2'
#define BTDecoder_AAC            '3'
#define BTDecoder_Fastream       '4'
#define BTDecoder_AptX           '5'
#define BTDecoder_AptX_St        '6'
#define BTDecoder_AptX_HD        '7'
#define BTDecoder_AptX_LL        '8'

/**********DeviceMODEL*************/
#define Model_DIXOM_C12           1
#define Model_DIXOM_m             2

/**********DeviceType*************/
#define Device_Type_Platform      1

/*********FirmwareName************/
#define FirmwareName_BootFriend   1
#define FirmwareName_AsIwant      2



/****Sours volum addr select******/

#define MinVolUsb                 5
#define MinVolBluetoooth          6
#define MinVolAux                 7
#define MinVolsRadio              8
#define MinVolSpdif               9
#define MinVolMic                 4

#define VolSoursUsb               11
#define VolSoursBluetoooth        14
#define VolSoursAux               17
#define VolSoursRadio             20
#define VolSoursSpdif             23
#define VolSoursMic               26

#define VolGroup1                 29
#define VolGroup2                 32
#define VolGroup3                 35
#define VolGroup4                 38
#define VolGroup5                 41
#define VolGroup6                 44
#define VolGroup7                 47


#define VERY_IMPORTANT            175
#define ONLY_USB_HID              185
//#define YES                       1
//#define NO                        0
//#define ON                        1
//#define OFF                       0
#define NOT_USED                  0
#define BLOCKED                   1
#define UNCLOCKED                 0
#define AUTO_SELECT               1
#define MANUAL_SELECT             1

typedef enum
{
	ON                     = 0x01U,
	OFF                    = 0x00U,
	YES                    = 0x01U,
	NO                     = 0x00U,
}differentStateTDE;

#define AutoLenght                0

#define Interrupt                 0
#define FreeRTOS                  1
#define Hal_lib                   2

#define All                       0
#define Modified                  1

//#define ErrorComman              "COMMAND_ERROR"
//#define RadioChipNot             "RADIO_MODULE NOT FOUND"

//#define CommanError              "<COMMAND_ERROR"
//#define NotSupportedr            "NOT_SUPPORTED"
#define StrMAX                   "MAX"
#define StrMIN                   "MIN"
#define LenCommanError            14
#define LenNotSupportedr          14

/************Time Out**************/
#define EEPROM_TIME_OUT           1000
#define I2C2_TIME_OUT             1000

/*Direction of exchange USB Audio*/
#define DirectionTypeWrite        48
#define DirectionTypeRead         49
#define DirectionTypeAuto         50


/***********I2C Device address************/
#define DEVICE_ADDR_DIXOM_UsbAudioBoot  0x31    //49
#define DEVICE_ADDR_DIXOM_UsbAudio      0x30    //49
#define DEVICE_ADDR_DSP_ADAU1452        0x70    //112
#define DEVICE_ADDR_DAC_AK4440          0x12    //18
#define DEVICE_ADDR_DAC_ADAU1962        0x8     //8
#define DEVICE_ADDR_DAC_AK4458VN        0x10    //16
#define DEVICE_ADDR_EEPROM_24LC256      0x50    //80
#define DEVICE_ADDR_RADIO_RDA5807       0x11    //17
#define DEVICE_ADDR_RTC_DS3231          0xD0    //208
#define DEVICE_ADDR_LCD_OLED1306        0x3C    //120
#define DEVICE_ADDR_LCD_HD44780         0x4E    //78
#define DEVICE_ADDR_DAC_TAS3251         0x4A    //74
#define DEVICE_ADDR_DSP_ADAU1701        0x68    //104
#define DEVICE_ADDR_RADIO_SI4735        0x63    //99
#define DEVICE_ADDR_RADIO_ADAU1701      0x34    //52
#define DEVICE_ADDR_AMP_TDF8551J        0x6C    //216
#define DEVICE_ADDR_DAC_PCM1690         0x4C    //216
#define DEVICE_ADDR_AMPLIFIER_3xTAS3251 0x70    //112


/*************EEPROM_MEM_ADDR***************/
#define EEPROM_ADDR_LICENS_KEY           0
#define EEPROM_ADDR_PCB_REV             12
#define EEPROM_ADDR_SETTINGS            250
#define EEPROM_ADDR_BLUETOOTH           950
#define EEPROM_ADDR_PLATFORM_LIFE       1024
#define EEPROM_ADDR_RADIO               1536
#define EEPROM_ADDR_DSP                 2048


/*******STM FLASH SECTOR ERASE NUM*********/
#define STM_FLASH_ERASE_STRUCT          4  //quantity 1 size 65536bit Sector 4 0x08010000 - 0x0801FFFF 64 Kbytes  dixom structure
#define STM_FLASH_ERASE_PRESET          3  //quantity 1 size 16384bit


/***********STM FLASH_MEM_ADDR*************/
#define FLASH_ADDR_SETTINGS       0x08010000  //134283264    0x08010000 + 0              2048            944   bit
#define FLASH_ADDR_CONTROL        0x08010800  //134285312    0x08010000 + 2048 byte      10240           5936  bit
#define FLASH_ADDR_CAN            0x08013000  //134295552    0x08010000 + 12288 byte     -               -     bit
#define FLASH_ADDR_DSP            0x08019C00  //134323200    0x08019C00 + 39936 byte     25600 byte      17064 byte
//      Name                      ADDR HEX     ADDR DEC      OFFSET FROM SECTOR START    LENGHT          USED
#define STM_FLASH_ADDR_PRESET_1    0x0800C000  //134266880    +0                          5440           4910 bit      Sector 3 0x0800C000 - 0x0800FFFF 16 Kbytes  user flash



/***********INTERFACE CODE************/
#define  NumInterfaceCode         42

#define EMPTY                     0
#define DAC_WRITE                 1
#define DAC_READ                  2
#define DSP_WRITE                 3
#define DSP_READ                  4
#define EEPROM_WRITE              5
#define EEPROM_READ               6
#define FLASH_WRITE               7
#define FLASH_READ                8
#define I2C2_WRITE                9
#define I2C2_READ                 10
#define SPI2_WRITE                11
#define SPI2_READ                 12
#define RADIO_WRITE               13
#define RADIO_READ                14
#define I2C1_WRITE                15
#define I2C1_READ                 16
#define USB_SOUND_CARD_WRITE      17
#define USB_SOUND_CARD_READ       18
#define DISPLAY_TEXT_LCD_WRITE    19
#define DISPLAY_OLED_WRITE        20
#define RTC_WRITE                 21
#define RTC_READ                  22
#define USB_HID_WRITE             23
#define USB_CDC_WRITE             24
#define UART1_BLUETOOTH_READ      25
#define UART1_BLUETOOTH_WRITE     26
#define UART2_WIRELESS_RF_READ    27
#define UART2_WIRELESS_RF_WRITE   28
#define UART3_ARDUINO_READ        29
#define UART3_ARDUINO_WRITE       30
#define UART5_NEXTION_READ        31
#define UART5_NEXTION_WRITE       32
#define MiniDSP_WRITE             33
#define MiniDSP_READ              34
#define STM_FLASH_LOCK            35
#define STM_FLASH_UNCLOCK         36
#define STM_FLASH_ERASE           37
#define STM_FLASH_WRITE           38
#define STM_FLASH_READ            39
#define CAN_SEND                  40
#define CAN_READ                  41







#endif

