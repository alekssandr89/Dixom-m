/*
 * Settings_Constant.h
 *
 *  Created on: 8 ����. 2021 �.
 *      Author: 79825
 */

#ifndef _SETTINGS_CONSTANT_H_
#define _SETTINGS_CONSTANT_H_

#include <Constants.h>

#define UNIQ_ELEM_DAC_SELECT      5
#define UNIQ_ELEM_SETTINGS        64
#define UNIQ_ELEM_TIMER           32
#define UNIQ_ELEM_ERROR           6

#define UNIQ_ELEM_SETTINGS_NoSave 16
#define UNIQ_ELEM_TIMER_NoSave    8



/*******Mode on power usb*********/
#define UsbOff                    0
#define UsbOn                     1
#define UsbOnTimer                2
#define UsbOnRear                 3




/*******Don't Sleep Command*********/
#define NoSleepCommandF7          0
#define NoSleepCommandMouse       1
#define NoSleepCommandWakeUp      2
#define NoSleepCommandF8          3

/*********USB Sound Cadr***********/
#define  Dixom_HiResAudio         0    // DIXOM Hi Res Audio
#define  NoSoundDevice            255  // ��� USB �������� �����



/***********radio band************/
#define BandEurope                0     // US/Europe  default
#define BandJapan                 1     // Japan
#define BandWorld                 2     // world wide
#define BandEastEur               3     // East Europe

#define TriggerOnHolding       0
#define TriggerOnOffNoSave     1
#define TriggerOnOffSave       2
/*
#define VolumControlOnDAC      0
#define VolumControlOnDSP      1
#define VolumControlOnLoudness 2
*/
#define DisplayNoShow          0
#define DisplayShow            1

#define HID_Modified           0
#define HID_Keyboard           1
#define HID_Mouse              2
#define HID_System_Control     3
#define HID_Consumer           4

#define PlatformAccess         0
#define DirectAccrs            1
#define AnyAccess              2



#define DataOut_All            0
#define DataOut_Some           0
#define DataOut_USB            1
#define DataOut_Bluetooth      2
#define DataOut_Nextion        3
#define DataOut_Arduino        4

#define PriorityLow            0
#define PriorityMidle          1
#define PriorityHigh           2




#define MaxBrightness             16    //������������ �������

/************LANGUAGE*************/
#define English                   0
#define Russian                   1

/********Nextion LCD Type**********/
#define NX4832T035_011_320x480    0   //Nextion LCD 320x480  ��� ���� 1


/*********BLUETOOTH select********/
#define BLUETOOTH_WT32_iWRAP4     0    // ������ ������ WT32
#define BLUETOOTH_BC127_MELODY6   1    // ������ ������ BC127
#define BLUETOOTH_FSC_BT806       2    // ������ ������ Feasycom FSC-BT806
#define BLUETOOTH_FSC_BT1006C     3    // ������ ������ Feasycom FSC_BT1006C
#define BLUETOOTH_FSC_BT1006A     4    // ������ ������ Feasycom FSC_BT1006A
#define BLUETOOTH_FSC_BT802       5    // ������ ������ Feasycom FSC_BT802
#define NoBluetoothDevice         255  // ��� ������ ������


/********define Settings**********/
#define SettingsPlay                               0   // 0   Status Play on Start dixom
#define SettingsPlay2                              1   // 1   Status Play x2 on Start dixom
#define SettingsAccOffStatPause                    2   // 30  ���������� �� �������� ������� ����� ����� ��� ���������� 1 �������� 0 ���
#define SettingsAccOffComPause                     3   // 31  ����� ������� ���������� ��� ��������� 0 Stop 1 Pause
#define SettingsAccOffStatSleep                    4   // 32  ���������� �� �������� ������� ����� ��� ���������� 1 �������� 0 ���
#define SettingsAccOffComSleep                     5   // 33  ����� ������� ���������� ��� ��������� 0 PowerDown 1 Sleep
#define SettingsStatusNoSleep                      6   // 3   Status No Sleep
#define SettingsNoSleepComand                      7   // 4   Command No Sleep
#define SettingsVbusTablet                         8   // 5   Status Vbus on input usb
#define SettingsModeOnUSB1                         9   // 6   Mode on power usb3
#define SettingsModeOnUSB2                         10  // 7   Mode on power usb4
#define SettingsRearSignal                         11  // 8   RearSignal 0 Analog 1 Can
#define SettingsBackRearCameraOff                  12  // 9   Alt tab camera off
#define SettingsPlayCameraOff                      13  // 10  Play music camera OFF
#define SettingsRearCamOnVolDivider                14  // 11  Camera on volume divider
#define SettingsCamOnMinVol                        15  // 12  Camera on minimum volume
#define SettingsStateSoundCard                     16  // 13  Usb sound card state
#define SettingsAutocSoundCardPolling              17  // 14  Status Automatic sound card polling
#define SettingsLanguage                           18  // 15  Platform language
#define Settings1Trigger                           19  // 16  ����� ������ �������� 1: 0 ��� ���� ����������, 1 ���/��� �� �����, 2 ���/���� �� ����� � ����������� � EEPROM
#define SettingsState1Trigger                      20  // 17  ��������� �������� 1: 1 ������� 0 ��������
#define Settings2Trigger                           21  // 18  ����� ������ �������� 2: 0 ��� ���� ����������, 1 ���/��� �� �����, 2 ���/���� �� ����� � ����������� � EEPROM
#define SettingsState2Trigger                      22  // 19  ��������� �������� 2: 1 ������� 0 ��������
#define Settings3Trigger                           23  // 20  ����� ������ �������� 3: 0 ��� ���� ����������, 1 ���/��� �� �����, 2 ���/���� �� ����� � ����������� � EEPROM
#define SettingsState3Trigger                      24  // 21  ��������� �������� 3: 1 ������� 0 ��������
#define SettingsTypeTextLCD                        25  // 22  Type Text LCD
#define SettingsTypeOledLCD                        26  // 23  Type Oled LCD
#define SettingsTypeNextionLCD                     27  // 25  Type Nextion LCD
#define SettingsNextionLCD                         28  // 24  Status Nextion LCD
#define SettingsDisplayBrightness                  29  // 26  ������� �������� ��������� �������� �� 0 �� 16 ��������
#define SettingsDisplayBrightMode                  30  // 27  ����� ����������� ������ ������� (������ �������������� � �.�.)
#define SettingsRemLowVoltageVal                   31  // 31  remot voltage level
#define SettingsVolUpUsbConnect                    32  // 32  volume up usb connect
#define Setting_current_selection_mode             33  // 33  Current select interface mode
#define Setting_manual_tx_usb_hid                  34  // 34  Setting_manual_tx_usb_hid
#define Setting_manual_tx_usb_cdc                  35  // 35  Setting_manual_tx_usb_cdc
#define Setting_manual_tx_usb_bluetooth            36  // 36  Setting_manual_tx_usb_cdc


#define SettingsNoSave_Task_Execution_State         0 // 0 ���������� ����� ��������� ��������
#define SettingsNoSave_VolUpUsbConnect_State        1 // 0 ��������� �������� ���������� ��������� ��� ����������� � USB
#define SettingsNoSave_VolUpCamera_State            2 // 0 ��������� �������� ���������� ��������� ��� ����������� � USB






/**********define Timer***********/
#define TimerTabletVdd            0
#define TimerPlay                 1
#define TimerNoSleep              2
#define TimerUsb1                 3
#define TimerUsb2                 4
#define TimerOtgOn                5
#define TimerUsbOn                6
#define TimerRearCam              7


#define ResBtnIntervalDef         150



#endif /* SETTINGS_SETTINGS_CONSTANT_H_ */
