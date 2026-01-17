/*
 * String_Constants.h
 *
 *  Created on: 16 ����. 2022 �.
 *      Author: 79825
 */

#ifndef PLATFORM_INC_DESCRIPTION_H_
#define PLATFORM_INC_DESCRIPTION_H_

////////////////////////////// --MENU-- /////////////////////////////

typedef enum
{
	DATA_SET               = '=', //������������� ����� ��������
	DATA_GET               = ' ', //���������� ��������
	DATA_GET_ALL           = '*', //���������� ��� ��������
	DATA_GET_MODIFIED      = '#', //���������� ��������� ��������
	DATA_DEFAULT           = '~', //������������� ����������� ���������
	DATA_DELETE            = '-', //������� ��������
}eMenu_Action;

/////////////////////////// --COMMAND-- ///////////////////////////////

typedef enum
{
	CMD_SHOW_MODIFIED       = 100,
	CMD_DIXOM_KEY           = 101,
	CMD_DIXOM_PCB           = 102,
	CMD_I2C1_SCAN           = 106,
	CMD_I2C2_SCAN           = 107,
	CMD_IWDG                = 140,
	CMD_SPI                 = 141,
	CMD_DAC_RESTART         = 142,
	CMD_RESTART             = 143,
	CMD_DEFAULT             = 145,
	CMD_SAVE                = 155,
	CMD_SPI_FLASH_ERASE     = 160,
	CMD_ACTION              = 170,
	CMD_USB_PWR_CONTROL     = 171,
}eCMD_PARAMETR;

////////////////////////////// --PACKED-- /////////////////////////////

typedef enum
{
	PACKED_SYSTEM            = 0,  //����� ���������� ��������
	PACKED_GENERAL           = 1,  //����� � �������� �����������
	PACKED_GATEWAY           = 2,  //����� � ������
	PACKED_CAN               = 3,  //����� � CAN �����
	PACKED_SOUND             = 4,  //����� � ����������� DSP, ���������� ���������, �����������, �������� � �.�. (��������� � PARAM.DSP)
	PACKED_BLUETOOTH         = 5,  //����� � ����������� Bluetooth
	PACKED_EXTERNAL_CONTROL  = 6,  //����� � ����������� ������� ������
	PACKED_FM_RADIO          = 7,  //����� � ����������� FM �����
	PACKED_EXPERIMENT        = 20, //����� � ��������������

}ePACKED;


/////////////////////////// --PACKED GENRAL-- /////////////////////////////

typedef enum
{
	GENERAL_MESSAGE                = 0,
	GENERAL_NOTIFICATION           = 1,
	GENERAL_SETTINGS               = 2,
	GENERAL_NO_SAVE_SETTINGS       = 3,
	GENERAL_SHOW_MODIFIED          = 4,
	GENERAL_SAVE                   = 5,
	GENERAL_DEFAULT                = 6,
	GENERAL_DAC_RESTART            = 7,
	GENERAL_LOG                    = 8,
	GENERAL_BIT_CHIP_CONTROL       = 9,
	GENERAL_TIMER                  = 10,
	GENERAL_DATA_TIME              = 11,
	GENERAL_I2C1_SCAN              = 12,
	GENERAL_I2C2_SCAN              = 13,
	GENERAL_IWDG                   = 14,
	GENERAL_SPI                    = 15,
	GENERAL_ACTION                 = 16,
	GENERAL_USB_PWR_CONTROL        = 17,
	GENERAL_SPI_STATUS             = 18,
	GENERAL_RECIVE_DATA_LOSS_TEST  = 19,
	GENERAL_TEMPERATURE            = 20,


}ePACKED_BASIS;

//                                         +----------------------------------------- Packed (Basis)
//                                         | +--------------------------------------- Parameter (Notification)
//                                         | | +------------------------------------- Detailed parameter
//                                         | | |
#define GeneralNotifi_CommandOk           "1 1 0 'Выполнено'" //Ok
#define GeneralNotifi_CommandError        "1 1 1 'Ошибка команды'" //Error
#define GeneralNotifi_NotSupported        "1 1 2 'Не поддерживается'" //Not supported
#define GeneralNotifi_NotFound            "1 1 3 'Не найдено'" // Not found
#define GeneralNotifi_NoReturnData        "1 1 4 'Нет возвратных данных'" //No return data
#define GeneralNotifi_NotReady            "1 1 5 'Не готов'"//Not ready
#define GeneralNotifi_NoDefaulOption      "1 1 6 'Нет стандартныз настроек'"//No default option
#define GeneralNotifi_NoDeleteOption      "1 1 7 'Нельзя удалить'"//No delete option
#define GeneralNotifi_NoSuchPackage       "1 1 9 'Не существующий паке'"//No such package
#define GeneralNotifi_NoPackageFunction   "1 1 10 'В пакете нет этой функции'"//The package does not have this feature
#define GeneralNotifi_CtlBitNotSupported  "1 1 11 'команда не поддерживается'"//Control bit is not supported
#define GeneralNotifi_RebootToApply       "1 1 12 'Рестарт для применения'"

#define Basis_Settings_1_2_0              "1 2 %hu %hhu"
#define Basis_Settings_1_2_1              "1 20 %hu %hhu 'Not saved'"


#define Basis_Notification_1_1_5_0        "1 4 0 'Отобразил все изменения'"//Ok all modified parameters are shown
#define Basis_Notification_1_1_5_1        "1 4 1 'Ошибка отображения изменений'"//Error all modified parameters are shown

#define Basis_Notification_1_1_11_0_0     "1 5 0 0 'Сохранено в EEPROM'"//Ok write to eeprom memory
#define Basis_Notification_1_1_11_0_1     "1 5 0 1 'Ошибка сохранения в EEPROM'"//Error write to eeprom memory
#define Basis_Notification_1_1_11_1_0     "1 5 1 0 'Сохранено в Flash'"//Ok write to flash memory
#define Basis_Notification_1_1_11_1_1     "1 5 1 1 'шибка сохранения в Flash'"//Error write to flash memory

#define Basis_Notification_1_8_0          "1 6 0 'Сброшено на стандартные'"//Ok set default settings
#define Basis_Notification_1_8_1          "1 6 1 'Ошибка сброса на стандартные'"//Error set default settings

#define General_DacReboot_Ok              "1 7 0 'Ok DAC reboot'"
#define General_DacReboot_Error           "1 7 1 'Error DAC reboot'"


#define General_1_Log_0                   "1 8 0 %hhu %hhu %lu %lu %lu %lu %lu %lu"  //General information
#define General_1_Log_1                   "1 8 1 %hhu %lu %lu %lu %lu %lu %lu"       //Detailed information
#define General_1_Log_2                   "1 8 2 'Done'"       //Detailed information

#define Basis_bitControl_1_6_0            "1 9 0 %lu %lu 'Register Control'"
#define Basis_bitControl_1_6_1            "1 9 1 'Register set successfully'"
#define Basis_bitControl_1_6_2            "1 9 2 'Сhip not supported'"
#define Basis_bitControl_1_6_3            "1 9 3 'Chip not found on data bus'"


#define Basis_Timer_1_12_0                "1 10 0 %hu %lu"
#define Basis_Timer_1_12_1                "1 10 1 %hu %lu 'Not saved'"

#define macros_rtc_data_time              "1 11 0 %hhu %hhu %hhu %hhu %hhu %hhu %hu 'Data Time'"
#define macros_rtc_temperature            "1 11 1 %.2f 'Temperature'"

#define Basis_Notification_1_14_0         "1 12 0 Start scanning bus I2C %hhu..." // I2C SCAN
#define Basis_Notification_1_14_1         "1 12 1 Device %hhu addr 0x%02X"        // FRIMWARE UPDATE
#define Basis_Notification_1_14_2         "1 12 I2C %hhu bus scan completed"    // FRIMWARE UPDATE

#define General_State_IWDG                "1 14 %hhu 'State IWDG'"
#define General_State_SPI                 "1 15 %hhu 'State SPI'"

#define General_UsbPowerControl           "1 17 %hhu %hhu 'USB port power status'"

//                                         | | |   |    |    |
//                                         | | |   +----+----+---------------------- Data
//                                         | | +------------------------------------ Detailed parameter
//                                         | +-------------------------------------- Parameter
//                                         +---------------------------------------- Packed (Basis)

//////////////////////// --PAKED SYSTEM & FIRMWARE UPDATE-- ///////////////////////

typedef enum
{
	SYSTEM_FU_USB_SOUND_CARD      = 0,
	SYSTEM_NOTIFICATION           = 1,
	SYSTEM_SETTINGS               = 2,
	SYSTEM_INFORMATION            = 3,
	SYSTEM_ACCESS_CONTROL         = 4,
	SYSTEM_EXCHANGE_INTERFACE     = 5,
	SYSTEM_RESTART                = 6,
	SYSTEM_FU_FIRMWARE_DOWNLOAD   = 7,
	SYSTEM_FU_PREPARING_UPDATE    = 8,
	SYSTEM_FU_COMPLETE_UPDATE     = 9,
	SYSTEM_FU_CHECK_FIRMWARE      = 10,
	SYSTEM_FU_CANCEL              = 11,
	SYSTEM_FU_BOOTLOADER          = 12,
	SYSTEM_SPI_FLASH_STATE        = 13,
	SYSTEM_SPI_FLASH_ERASE        = 14,
	SYSTEM_LICENSE_KEY            = 15,
	SYSTEM_PCB_REVISION           = 16,
	SYSTEM_CONNECTION             = 17,
}ePACKED_SYSTEM;


#define EmptySystemMessage                 "0 0 0"

#define MessageUpdate_2_1_0                "0 1 0 'Update preparation done'"
#define MessageUpdate_2_1_1                "0 1 1 'Firmware successfully verified'"
#define MessageUpdate_2_1_2                "0 1 2 'Firmware is corrupted'"
#define MessageUpdate_2_1_3                "0 1 3 'Firmware uploaded error'"
#define MessageUpdate_2_1_4                "0 1 4 'Firmware uploaded successfully'"
#define MessageUpdate_2_1_5                "0 1 5 'Flash memory chip ready'"
#define MessageUpdate_2_1_6                "0 1 6 'Flash chip not found'"
#define MessageUpdate_2_1_7                "0 1 7 'Reboot'"
#define MessageUpdate_2_1_8                "0 1 8 'Cancel'"
#define MessageUpdate_2_1_9                "0 1 9 'Update system unlocked'"
#define MessageUpdate_2_1_10               "0 1 10 'Update system blocked'"
#define MessageUpdate_2_1_11               "0 1 11 'No bootloader update required'"
#define MessageUpdate_2_1_12               "0 1 12 'Bootloader updated successfully'"
#define MessageUpdate_2_1_13               "0 1 13 'Bootloader update fail!'"
#define MessageUpdate_2_1_14               "0 1 14 'Not ready for firmware update'"
#define SystemNotifi_SysCommandBlocked     "0 1 15 'System commands are blocked'"
#define MessageUpdate_2_1_16               "0 1 16 'Successful delivery'"
#define Basis_Notification_1_1_22_0        "0 1 17 'Ok cleared spi flash memory'"
#define Basis_Notification_1_1_11_1        "0 1 18 'Error cleared spi flash memory'"
#define MessageUpdate_0_1_19               "0 1 19 'Error while preparing update'"
#define MessageUpdate_0_1_20               "0 1 20 'Error delivery'"

#define System_Info_1_3_0                  "0 3 0 %hhu %hhu %hhu %s %s %s %s %s %s"   //Palfthorma version information
#define System_Info_1_3_1                  "0 3 1 %s %s"                              //Serial number and activation information
#define System_Info_1_3_2                  "0 3 2 04.06.2023 %hhu %hu %lu %lu"        //Information about the life of the platform
#define System_Info_1_3_3                  "0 3 3 %s %s %hhu %hhu"                    //Information about the USB sound card
#define System_Info_1_3_4                  "0 3 4 %hhu %hhu %hhu"                     //About DAC
#define MaskNewBootloader                  "0 3 5 %.2f"
//                                          | | |      |        |       |
//                                          | | |      +--------+-------+-------------- Data
//                                          | | +-------------------------------------- Detailed parameter (optional)
//                                          | +---------------------------------------- Parameter (Notification)
//                                          +------------------------------------------ Packed (Basis)

#define General_Connection                "0 5 %hhu %hhu %hhu %hhu %hhu %hhu %hhu"           //Connection interface information



/////////////////////// --CAN-- /////////////////////

typedef enum
{
	CAN_MESSAGE        = 0,
	CAN_NOTIFICATION   = 1,
	CAN_SETTINGS       = 2,
	CAN_STATUS         = 3,
	CAN_SPEED          = 4,
	CAN_FILTER         = 5,
	CAN_CAR            = 6,
}eCAN;

#define CanMessage_3_0               "3 0 0 %hu %d"

#define CanNotifcation_3_1_0         "3 1 0 Connection error"
#define CanNotifcation_3_1_1         "3 1 1 Disconnect error"
#define CanNotifcation_3_1_2         "3 1 2 Speed change selection error"
#define CanNotifcation_3_1_3         "3 1 3 Structure default"
#define CanNotifcation_3_1_4         "3 1 4 Structure check sum error"
#define CanNotifcation_3_1_5         "3 1 5 Connecting"

#define CanSettings_3_2              "3 2 %hhu %hhu"

#define CanStatus_3_3                "3 3 %hhu %hhu %hhu %hhu %hhu %hhu %hhu %hhu %hhu" //Status, CanSpeed, ListerenMode, AutoConnect, CanMsgCount, DataRedirection

#define CanSpeed_3_4                 "3 4 %hhu"

#define CanCar_3_6_0                 "3 6 %hhu %hhu %hhu %hhu %hhu %hhu %hhu" //	Manufacturer, Model, Generation, Year, Transmission, DriveUnit, HandlebarSide

#define CanCar_3_6_1                 "3 7 1 %hhu" // Climat

#define CanCar_3_5_0                 "3 5 %hhu %hhu %hhu %hhu %hhu %lo %lo %lo %lo" //	FilterNum, Activation, Mode, Scale, IdType, IdHigh, IdLow, MaskIdHigh, MaskIdLow

/////////////////////// --PACKED BLUETOOTH-- /////////////////////

typedef enum
{
	BT_MESSAGE        = 0,
	BT_NOTIFICATION   = 1,
	BT_SETTINGS       = 2,
	BT_DIRECT_ACCESS  = 3,
	BT_TITLE          = 4,
	BT_ARTIST         = 5,
	BT_ALBUM          = 6,
	BT_PLAY_STATE     = 7,
	BT_CALL_INFO      = 8,
	BT_PHONE_STATE    = 9,
	BT_PHONEBOOK_DATA = 10,
	BT_BOOK_STATE     = 11,
	BT_AUDIO_CODEC    = 12,
	BT_MODULE_MODEL   = 13,
	BT_MODULE_ADDR    = 14,
	BT_MODULE_NAME    = 15,
	BT_MODULE_PASS    = 16,
	BT_DIVICE_CONN    = 17,
	BT_DIVICE_ADDR    = 18,
	BT_DIVICE_NAME    = 19,
	BT_NETWORK_NAME   = 20,
	BT_NETWORK_SIGNAL = 21,
	BT_CONTROL_MODE   = 22,
	BT_MEDIA          = 23,
}eBLUETOOTH;

typedef enum
{
	BT_MESSAGE_MEDIA_TIME           = 0,
}eBLUETOOTH_MSG;

#define BtNotificationDefault          "5 1 0 %hu 'Стандартные парамтеры'"
#define BtNotificationThroughput       "5 1 1 %hu 'Throughput Mode'"

                                       /*BLUETOOTH*/
#define BluetoothSettings              "5 2 %hhu %hhu"
#define BluetoothDirectAccess          "5 3 %s"
#define MaskGetBtMediaTime             "5 0 0 %lu %lu"
#define MaskGetBtMediaTit              "5 4 "
#define MaskGetBtMediaArt              "5 5 "
#define MaskGetBtMediaAlb              "5 6 "
#define MaskGetBtMediaPst              "5 7 ? Play_State"
#define MaskGetBtPhonePhn              "5 8 "
#define MaskGetBtPhonePhs              "5 9 ? Phone_State"
#define MaskGetBtBookPbd               "5 10 "
#define MaskGetBtBookState             "5 11 "
#define MaskGetBtMediaCodec            "5 12 "
#define MaskGetInfoBtModule            "5 13 %hhu %s"
#define MaskGetInfoBtmAddr             "5 14 %s"
#define MaskGetInfoBtName              "5 15 %s"
#define MaskGetInfoBtPass              "5 16 %s"
#define MaskGetInfoBtDevConn           "5 17 %hhu"
#define MaskGetInfoBtDevAddr           "5 18 %s"
#define MaskGetInfoBtDevName           "5 19 %s"
#define MaskGetInfoBtDeNetName         "5 20 %s"
#define MaskGetInfoBtDeNetSignal       "5 21 %hhu"
#define MaskGetBtDevConn               "5 22 1"


#define MaskGetBtDevConn0              "5 16 0"
#define MaskGetBtDevAddrDis            "5 17 DISCONNECTED"
#define MaskGetBtDevNameDis            "5 18 DISCONNECTED"
#define MaskGetBtDevNetDis             "5 20 0"



///////////////////////////// --PACKED SOUND-- ///////////////////////////

typedef enum
{
	SOUND_MESAGE                 = 0,
	SOUND_NOTIFICATION           = 1,
	SOUND_SETTINGS               = 2,

	SOUND_VOLUME                 = 3,
	SOUND_LOUDNESS               = 4,
	SOUND_VOL_CONTROL            = 5,

	SOUND_SOURCE                 = 6,
	SOUND_SC_SOURCE              = 7,
	SOUND_SOURCE_EQUALIZER       = 8,

	SOUND_PRESET                 = 9,
	SOUND_SC_PRESET              = 10,
	SOUND_SAVE_PRESET            = 11,
	SOUND_PRESET_NAME            = 12,

	SOUND_GAIN                   = 13,
	SOUND_GROUP_CH               = 14,
	SOUND_COPY_CHANNEL           = 15,
	SOUND_GROUP_NAME             = 16,
	SOUND_CHANNEL_NAME           = 17,

	SOUND_EQUALIZER              = 18,
	SOUND_BYPASS_CH_EQUALIZER    = 19,
	SOUND_CROSSOVER              = 20,

	SOUND_DELAY                  = 21,
	SOUND_DELAY_GROUP_NAME       = 22,
	SOUND_DELAY_GROUP            = 23,

	SOUND_INVERT                 = 24,
	SOUND_NOISE                  = 25,
	SOUND_SIN_GEN                = 26,
	SOUND_P_STEREO               = 27,
	SOUND_W_STEREO               = 28,
	SOUND_S_BASS                 = 29,

	SOUND_DEFAULT                = 34,
	SOUND_GET_ALL_MODIFIED_PARAM = 35,
	SOUND_GET_ALL_EQUALIZER      = 36,
	SOUND_GET_ALL_CROSSOVER      = 37,
	SOUND_GET_ALL_SOUND_DELAY    = 38,
	SOUND_GET_ALL_PHASE          = 39,
	SOUND_GET_ALL_VOLUME         = 40,
	SOUND_GET_ALL_PHATSTEREO     = 41,
	SOUND_GET_ALL_LOUDNESS       = 42,
	SOUND_GET_ALL_GROUP_CH       = 43,

	SOUND_TEMPERATURE_CONFIG     = 50,
	SOUND_TEMPERATURE_SHOW       = 51,


}eSOUND_PARAMETR_COMMAND;
                                       /*DSP*/
#define MaskGetDspMesage               "4 0 %hu %d"

#define DspNotificationDefault         "4 1 0 %hu %hu 'Default'"
#define DspNotificationAmplifier       "4 1 1 %.1f %hhu 'Нагрев усилителя'"

#define MaskDspGetSettings             "4 2 %hhu %hhu"

#define MaskGetDspVol                  "4 3 %hu %d"
#define MaskGetDspLoudness             "4 4 %hu %hu %hu %hu %hu"
#define MaskGetDspVolControl           "4 5 %hhu"

#define MaskGetDspSource               "4 6 %hhu"
#define MaskGetDspSCSource             "4 7 %hu %hu %hu %hu %hu %hu"
#define MaskGetDspSourceEq             "4 8 %hu %hu %hu %hu %.2f %hu %.2f"

#define MaskGetDspPreset               "4 9 %hhu"
#define MaskGetDspSCPreset             "4 10 %hhu %hhu %hhu"
#define MaskGetDspPresetLable          "4 12 %hhu %s"

#define MaskGetDspGain                 "4 13 %hu %f"
#define MaskGetDspGroupCH              "4 14 %hu %hu"
#define MaskGetDspCopyChannel          "4 15 %hu %hu"
#define MaskSspGetGroupName            "4 16 %hhu %s"
#define MaskSspGetChannelName          "4 17 %hhu %s"

#define MaskGetDspEq                   "4 18 %hu %hu %hu %hu %.2f %hu %.2f"
#define MaskGetDspStateEQ              "4 19 %hu %hu"

#define MaskGetDspCross                "4 20 %hu %hu %hu %hhu %hu %hu %.2f"

#define MaskGetDspDelay                "4 21 %hu %.2f"
#define MaskGetDspDelayGroupLable      "4 22 %hhu %s"
#define MaskGetDelayGroup              "4 23 %hhu %hhu"



#define MaskGetDspInvert               "4 24 %hu %hu"
#define MaskGetDspNoise                "4 25 %hhu"
#define MaskGetDspSinGen               "4 26 %hhu %hu"
#define MaskGetDspPhatStereo           "4 27 %hu %hu %hu"
#define MaskGetDspWideStereo           "4 28 %hu %hu"
#define MaskGetDspSupperBass           "4 29 %hhu %hhu %hhu %hhu %hu"

#define DspTemperatureControl          "4 50 %hhd %hhd"

#define MaskSetDspMesage               "4 0 %hu %d"
#define MaskSetDspNotification2        "4 1 %hu %d"
#define MaskSspGetSettings             "4 2 %hhu %hhu"
#define MaskSetDspVol                  "4 3 %hu %d"
#define MaskSetDspLoudness             "4 4 %hu %hu %hu %hu %hu"
#define MaskSetDspVolControl           "4 5 %hhu"
#define MaskSetDspSource               "4 6 %hhu"
#define MaskSetDspSCSource             "4 7 %hu %hu %hu %hu %hu %hu"
#define MaskSetDspPreset               "4 8 %hhu"
#define MaskSetDspSCPreset             "4 9 %hhu %hhu %hhu"
#define MaskSetDspGroupCH              "4 10 %hu %hu"
#define MaskSetDspGain                 "4 11 %hu %d"
#define MaskSetDspEq                   "4 12 %hu %hu %hu %hu %.2f %hu %.2f"
#define MaskSetDspStateEQ              "4 13 %hu %hu"
#define MaskSetDspCopyEQState          "4 14 %hu %hu"
#define MaskSetDspCross                "4 15 %hu %hu %hu %hhu %hu %hu %.2f"
#define MaskSetDspCopyCrossState       "4 16 %hu %hu"
#define MaskSetDspInvert               "4 17 %hu %hu"
#define MaskSetDspDelay                "4 18 %hu %.2f"
#define MaskSetDspNoise                "4 19 %hhu"
#define MaskSetDspSinGen               "4 20 %hhu %hu"
#define MaskSetDspPhatStereo           "4 21 %hu %hu %hu"
#define MaskSetDspWideStereo           "4 22 %hu %hu"
#define MaskSGetDspSupperBass          "4 23 %hhu %hhu %hhu %hhu %hu"



/////////////////////// --PACKED FM RADIO-- /////////////////////

typedef enum
{
	RADIO_MESSAGE         = 0,
	RADIO_NOTIFICATION    = 1,
	RADIO_SETTINGS        = 2,
	RADIO_CURRENT_PRESET  = 3,
	RADIO_TABLE_PRESET    = 4,
	RADIO_FREQ            = 5,
	RADIO_CHIP            = 6,
	RADIO_AUTO_SEARCH     = 7,       // ��������� �����
	RADIO_MANUAL_SEARCH    = 8,       // ��������� �����
	RADIO_SAVE_PRESET      = 9,       // ��������� �����
	RADIO_AUTO_SAVE_PRESET = 10,       // ��������� �����
	RADIO_DELETE_PRESET    = 11,           // �������� � � ������
	RADIO_GET_ALL_MODIFIED = 12,           // �������� � � ������
}eRADIO;

                                       /*RADIO*/

#define MaskGetRadioNotifiPointOccup   "7 1 0 'All modified fm_radio loaded'"
#define MaskGetRadioNotifiPointOccup   "7 1 1 %hhu 'This preset is already saved'"
#define MaskGetRadioServiceInfo        "7 0 0 %hhu %hhu %hhu %hhu %hhu %hhu"
#define MaskGetRadioSettings           "7 2 %hhu %hhu %hhu %hhu %hhu %hhu %hhu"
#define MaskGetRadioCurentPreset       "7 3 %hhu %.2f"
#define MaskGetRadioPreset             "7 4 %hhu %.2f"
#define MaskGetRadioFreq               "7 5 %.2f"
#define MaskGetRadioModule             "7 6 %hhu %hhu"
#define MaskGetRadioDelPreset          "7 11 %hhu"


/////////////////////// --PACKED EXTERNAL CONTROL-- /////////////////////

typedef enum
{
	EC_BTN_MESSAGE             = 0,
	EC_BTN_NOTIFICATION        = 1,
	EC_BTN_SETTINGS            = 2,
	EC_BUTTON                  = 3,
	EC_NEW_BUTTON              = 4,
	EC_BUTTON_NAME             = 5,
}eExternalControl;

                                       /*External Control*/

#define MaskGetAnaloControlLoadCplt    "6 1 0 'All modified buttons loaded done'"
#define MaskGetAllBtnPointDone         "6 1 1 'All point buttons loaded done'"
#define AnaloControlBtnNotConfig       "6 1 2 'Button not configured'"
#define AnaloControlBtnNotName         "6 1 3 'Button not name'"
#define AnaloControlBtnDefault         "6 1 4 'All button default'"
#define MaskGetControlSettings         "6 2 %hu %hu "
#define MaskGetAnaloControlBtn         "6 3 %hu %hu %hhu %hu %hu %hu %hu %hu %hu %hu %hu %hu %hu %hu %hu"
#define MaskGetAnaloControlBtnInit     "6 4 %hu %hu"
#define MaskGetAnaloControlBtnName     "6 5 %hu %hu "









//#define MaskReceiveDataLossTest        "20 10 %hu"
#define MaskReceiveDataLossTest        "20 10 %hu 'Проверка потери кадров'"










#endif /* PLATFORM_INC_DESCRIPTION_H_ */
