/*
 * Settings_Default.h
 *
 *  Created on: 8 ����. 2021 �.
 *      Author: 79825
 */

#ifndef SETTINGS_SETTINGS_DEFAULT_H_
#define SETTINGS_SETTINGS_DEFAULT_H_



const uint8_t  DEFAULT_SETTINGS[UNIQ_ELEM_SETTINGS] = {
ON,                               // 0   Status Play on Start dixom
OFF,                              // 1   Status Play x2 on Start dixom
ON,                               // 2   Status stop music power down: 1 on 0 off
0,                                // 3   Command stop: 0 Stop 1 Pause
ON,                               // 4   Status sleep on power down platform: 1 on 0 off
1,                                // 5   Command sleep: 0 PowerDown 1 Sleep
ON,                               // 6   Status Don't Sleep
NoSleepCommandF8,                 // 7   Command Don't Sleep
OFF,                              // 8   Status Vbus on input usb
UsbOn,                            // 9   Mode on power usb1
UsbOnRear,                        // 10  Mode on power usb2
0,                                // 11  RearSignal 0 Analog 1 Can
OFF,                              // 12  Alt tab camera off
OFF,                              // 13  Play music camera OFF
2,                                // 14  Rear Camera On Volume Default
20,                               // 15  Rear Camera minimum volume
ON,                               // 16  Usb sound card state
OFF,                              // 17  Status Automatic sound card polling
English,                          // 18  Platform language
TriggerOnHolding,                 // 19  ����� ������ �������� 1: 0 ��� ���� ����������, 1 ���/��� �� �����, 2 ���/���� �� ����� � ����������� � EEPROM
OFF,                              // 20  ��������� �������� 1: 1 ������� 0 ��������
TriggerOnHolding,                 // 21  ����� ������ �������� 2: 0 ��� ���� ����������, 1 ���/��� �� �����, 2 ���/���� �� ����� � ����������� � EEPROM
OFF,                              // 22  ��������� �������� 2: 1 ������� 0 ��������
TriggerOnOffNoSave,               // 23  ����� ������ �������� 3: 0 ��� ���� ����������, 1 ���/��� �� �����, 2 ���/���� �� ����� � ����������� � EEPROM
OFF,                              // 24  ��������� �������� 3: 1 ������� 0 ��������
0,                                // 25  Type Text LCD
0,                                // 26  Type Oled LCD
0,                                // 27  Type Nextion LCD
OFF,                              // 28  Status Nextion LCD
MaxBrightness,                    // 29  ������� �������� ��������� ��������: �� 0 �� 16 ��������
0,                                // 30  ����� ����������� ������ ������� (������ �������������� � �.�.)
110,                              // 31  ���������� ��� ��������� �������� ���������� ����� 110 ��� 11.0 ������ �.�. *0.1
OFF,                              // 32  �������� ���������� ��������� ��� ����������� � USB
AUTO_SELECT,                      // 33  Current select interface mode
OFF,                              // 34  Setting_manual_tx_usb_hid
OFF,                              // 35  Setting_manual_tx_usb_cdc
OFF                 ,             // 36  Setting_manual_tx_usb_cdc
0,                                // 37
0,                                // 38
0,                                // 39
0,                                // 40
0,                                // 41
0,                                // 42
0,                                // 43
0,                                // 44
0,                                // 45
0,                                // 46
0,                                // 47
0,                                // 48
0,                                // 49
0,                                // 50
0,                                // 51
0,                                // 52
0,                                // 53
0,                                // 54
0,                                // 55
0,                                // 56
0,                                // 57
0,                                // 58
0,                                // 59
0,                                // 60
0,                                // 61
0,                                // 62
0,                                // 63
};


const uint32_t  DEFAULT_TIMER[UNIQ_ELEM_TIMER] = {
0,                                //0   ������ ������� ������� ��������
7000,                             //1   ������ �������� ������� Play
120000,                           //2   ������ �������� ������� �� �����
400,                              //3   ������ USB 1
400,                              //4   ������ USB 2
2000,                             //5   ������ ��������� OTG
2000,                             //6   ������ ��������� VbusSTM
1000,                             //7   ������ �������� ��������� ������ ������� ���� TimerRearCam
2000,                             //8
2000,                             //9
2000,                             //10
2000,                             //11
2000,                             //12
2000,                             //13
2000,                             //14
2000,                             //15
2000,                             //16
2000,                             //17
2000,                             //18
2000,                             //19
};


#endif /* SETTINGS_SETTINGS_DEFAULT_H_ */
