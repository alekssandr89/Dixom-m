#include <Main.h>
#include <RDA_5807.h>
#include "main_Action.h"
#include "stdint.h"
#include "GPIO.h"
#include "Exchange.h"
#include "StructDeviceHID.h"
#include "TextDisplay.h"
#include "display_Menu.h"
#include "DSP_InternalCalls.h"
#include "Description.h"
#include <Structures.h>

extern sDixom Dixom;

void ActionSend(uint8_t ControlByte, uint8_t *Recive_USB){

	if(ControlByte == DATA_SET){ //SET
		short action   =0,
		      data1    =0,
		      data2    =0,
		      data3    =0,
		      data4    =0,
		      channel  =0,
			  pressed  =0;

		sscanf(Recive_USB, "%hu %hd %hd %hd %hd %hu %hu", &action, &data1, &data2, &data3, &data4, channel, pressed);

		Action(action,data1,data2,data3,data4,channel, 1);
	    Delay(200, FreeRTOS);
		Action(action,data1,data2,data3,data4,channel, 0);
	}else{
  		TransmitDataOutputs((uint8_t *)GeneralNotifi_CtlBitNotSupported, AutoLenght,  false,  true, allAvailable);

	}
}
/*
#define MultiWheel_Set 0
#define MultiWheel_Next 1
#define MultiWheel_Previus 2
#define MultiWheel_HoldNext 3
#define MultiWheel_HoldPrevius 4*/
void Action(short action, short data1, short data2, short data3,  short data4, short channel, short pressed){

	switch (action){
        case 0     :                                                                                      break; // ������ �� ������
        case 1     : if(pressed == YES) PlatformRestart(FreeRTOS);                                                break; // ������������ ���������
        case 2     :                                                                                      break; // Reserved

        case 10    : if(pressed == YES) OnOffRearCam();                                                   break; // ���/���� ������ ������� ����
        case 11    : SetTrigger(SettingsState1Trigger, pressed);                                          break; // �������� �������
        case 12    : SetTrigger(SettingsState2Trigger, pressed);                                          break; // ��������� ��������
        case 13    : SetTrigger(SettingsState3Trigger, pressed);                                          break; // Reserved

	    case 18    : Action_MultiWheel(0, NOT_USED, NOT_USED, NOT_USED, channel, pressed);                break; // Play pause
	    case 19    : Action_MultiWheel(1, NOT_USED, NOT_USED, NOT_USED, channel, pressed);                break; // ���������� ���������
	    case 20    : Action_MultiWheel(2, NOT_USED, NOT_USED, NOT_USED, channel, pressed);                break; // ���������� ����������
	    case 21    : Action_MultiWheel(3, NOT_USED, NOT_USED, NOT_USED, channel, pressed);                break; // ���������� ��������� ���������
	    case 22    : Action_MultiWheel(4, NOT_USED, NOT_USED, NOT_USED, channel, pressed);                break; // ���������� ���������� ���������
	    case 23    : if(pressed == YES) DSP_VolumUP(0);                                                  DSP_EEPROM_NeedToSave(); break; // ���������� ��������� +
	    case 24    : if(pressed == YES) DSP_VolumDown(0);                                                DSP_EEPROM_NeedToSave(); break; // ���������� ��������� -
	    case 25    : if(pressed == YES) DSP_Mute();                                                       break; // ���������� ����
	    case 26    : if(pressed == YES) DSP_ATT();                                                        break; // ���������� ���
        case 27    :                                                                                      break; // Reserved

	    case 42    : ActionHID (HID_Consumer, 205, NOT_USED, NOT_USED, NOT_USED, channel, pressed);       break; // HID Play/Pause
	    case 43    : ActionHID (HID_Consumer, 176, NOT_USED, NOT_USED, NOT_USED, channel, pressed);       break; // HID Play
	    case 44    : ActionHID (HID_Consumer, 177, NOT_USED, NOT_USED, NOT_USED, channel, pressed);       break; // HID Pause
	    case 45    : ActionHID (HID_Consumer, 183, NOT_USED, NOT_USED, NOT_USED, channel, pressed);       break; // HID Stop
	    case 46    : ActionHID (HID_Consumer, 226, NOT_USED, NOT_USED, NOT_USED, channel, pressed);       break; // HID Mute
	    case 47    : ActionHID (HID_Consumer, 181, NOT_USED, NOT_USED, NOT_USED, channel, pressed);       break; // HID next
	    case 48    : ActionHID (HID_Consumer, 182, NOT_USED, NOT_USED, NOT_USED, channel, pressed);       break; // HID previus
	    case 49    : ActionHID (HID_Consumer, 179, NOT_USED, NOT_USED, NOT_USED, channel, pressed);       break; // HID forward
	    case 50    : ActionHID (HID_Consumer, 180, NOT_USED, NOT_USED, NOT_USED, channel, pressed);       break; // HID rewind
	    case 51    : if(pressed == YES) HID_ALT_TAB();                                                    break; // HID Alt Tab
	    case 52    : ActionHID (HID_Keyboard, 300, NOT_USED, NOT_USED, NOT_USED, channel, pressed);       break; // HID Sleep
	    case 53    : ActionHID (HID_Keyboard, 301, NOT_USED, NOT_USED, NOT_USED, channel, pressed);       break; // HID PowerDown
	    case 54    : ActionHID (HID_Keyboard, 302, NOT_USED, NOT_USED, NOT_USED, channel, pressed);       break; // HID WakeUP
	    case 55    :                                                                                      break; // Reserved

	    case 60    : DSP_Action_ControlSource (0, 1, NOT_USED, NOT_USED, channel, pressed);                break; // Source routing ++
	    case 61    : DSP_Action_ControlSource (0, 0, NOT_USED, NOT_USED, channel, pressed);                break; // Source routing --
	    case 62    : DSP_Action_ControlSource (1, SoursUsb, NOT_USED, NOT_USED, channel, pressed);         break; // Source select Usb audio
	    case 63    : DSP_Action_ControlSource (1, SoursBluetoooth, NOT_USED, NOT_USED, channel, pressed);  break; // Source select Bluetooth
	    case 64    : DSP_Action_ControlSource (1, SoursAux, NOT_USED, NOT_USED, channel, pressed);         break; // Source select Aux
	    case 65    : DSP_Action_ControlSource (1, SoursRadio, NOT_USED, NOT_USED, channel, pressed);       break; // Source select Radio
	    case 66    : DSP_Action_ControlSource (1, SoursSpdif, NOT_USED, NOT_USED, channel, pressed);       break; // Source select Spdif
	    case 67    : DSP_Action_ControlSource (1, SoursMic, NOT_USED, NOT_USED, channel, pressed);         break; // Source Microphone
	    case 68    :                                                                                      break; // Reserved

	    case 75    : DSP_Action_ControlSource (2, SoursUsb, NOT_USED, NOT_USED, channel, pressed);         break; // Source On/Off Usb audio
	    case 76    : DSP_Action_ControlSource (2, SoursBluetoooth, NOT_USED, NOT_USED, channel, pressed);  break; // Source On/Off Bluetooth
	    case 77    : DSP_Action_ControlSource (2, SoursAux, NOT_USED, NOT_USED, channel, pressed);         break; // Source On/Off Aux
	    case 78    : DSP_Action_ControlSource (2, SoursRadio, NOT_USED, NOT_USED, channel, pressed);       break; // Source On/Off Radio
	    case 79    : DSP_Action_ControlSource (2, SoursSpdif, NOT_USED, NOT_USED, channel, pressed);       break; // Source On/Off Spdif
	    case 80    : DSP_Action_ControlSource (2, SoursMic, NOT_USED, NOT_USED, channel, pressed);         break; // Source Microphone
	    case 81    :                                                                                      break; // Reserved

	    case 100   : if(pressed == YES) ActionMenu (0, channel);                                          break; // Display Menu Left
	    case 101   : if(pressed == YES) ActionMenu (1, channel);                                          break; // Display Menu Up
	    case 102   : if(pressed == YES) ActionMenu (2, channel);                                          break; // Display Menu Down
	    case 103   : if(pressed == YES) ActionMenu (3, channel);                                          break; // Display Menu Right
	    case 104   : if(pressed == YES) ActionMenu (4, channel);                                          break; // Display Menu Set
	    case 105   : if(pressed == YES) ActionMenu (5, channel);                                          break; // Display Menu None
	    case 106   : if(pressed == YES) ActionMenu (6, channel);                                          break; // Display Menu Back
	    case 107   : if(pressed == YES) DisplayOpenMenu();                                                break; // Display Menu OpenMenu
	    case 108   :                                                                                      break; // Display Menu CloseMenu
	    case 109   :                                                                                      break; // Reserved

	    case 130   : DSP_Action_ControlVolum (1, 11, NOT_USED, NOT_USED, channel, pressed);                break; // Volume Up   Usb Audio
	    case 131   : DSP_Action_ControlVolum (0, 11, NOT_USED, NOT_USED, channel, pressed);                break; // Volume Down Usb Audio
	    case 132   : DSP_Action_ControlVolum (1, 14, NOT_USED, NOT_USED, channel, pressed);                break; // Volume Up   Bluetooth
	    case 133   : DSP_Action_ControlVolum (0, 14, NOT_USED, NOT_USED, channel, pressed);                break; // Volume Down Bluetooth
	    case 134   : DSP_Action_ControlVolum (1, 17, NOT_USED, NOT_USED, channel, pressed);                break; // Volume Up   Aux
	    case 135   : DSP_Action_ControlVolum (0, 17, NOT_USED, NOT_USED, channel, pressed);                break; // Volume Down Aux
	    case 136   : DSP_Action_ControlVolum (1, 20, NOT_USED, NOT_USED, channel, pressed);                break; // Volume Up   Radio
	    case 137   : DSP_Action_ControlVolum (0, 20, NOT_USED, NOT_USED, channel, pressed);                break; // Volume Down Radio
	    case 138   : DSP_Action_ControlVolum (1, 23, NOT_USED, NOT_USED, channel, pressed);                break; // Volume Up   Spdif
	    case 139   : DSP_Action_ControlVolum (0, 23, NOT_USED, NOT_USED, channel, pressed);                break; // Volume Down Spdif
	    case 140   : DSP_Action_ControlVolum (1, 26, NOT_USED, NOT_USED, channel, pressed);                break; // Volume Up   Microphone
	    case 141   : DSP_Action_ControlVolum (0, 26, NOT_USED, NOT_USED, channel, pressed);                break; // Volume Down Microphone
	    case 142   : DSP_Action_ControlVolum (1, 232, NOT_USED, NOT_USED, channel, pressed);               break; // Volume Up   Beep
	    case 143   : DSP_Action_ControlVolum (0, 232, NOT_USED, NOT_USED, channel, pressed);               break; // Volume Down Beep
	    case 144   :                                                                                       break; // Reserved

	    case 150   : DSP_Action_ControlVolum (1, 29, NOT_USED, NOT_USED, channel, pressed);                break; // Volume Up   Group CH1-2
	    case 151   : DSP_Action_ControlVolum (0, 29, NOT_USED, NOT_USED, channel, pressed);                break; // Volume Down Group CH1-2
	    case 152   : DSP_Action_ControlVolum (1, 32, NOT_USED, NOT_USED, channel, pressed);                break; // Volume Up   Group CH3-4
	    case 153   : DSP_Action_ControlVolum (0, 32, NOT_USED, NOT_USED, channel, pressed);                break; // Volume Down Group CH3-4
	    case 154   : DSP_Action_ControlVolum (1, 35, NOT_USED, NOT_USED, channel, pressed);                break; // Volume Up   Group CH5-6
	    case 155   : DSP_Action_ControlVolum (0, 35, NOT_USED, NOT_USED, channel, pressed);                break; // Volume Down Group CH5-6
	    case 156   : DSP_Action_ControlVolum (1, 38, NOT_USED, NOT_USED, channel, pressed);                break; // Volume Up   Group CH7-8
	    case 157   : DSP_Action_ControlVolum (0, 38, NOT_USED, NOT_USED, channel, pressed);                break; // Volume Down Group CH7-8
	    case 158   : DSP_Action_ControlVolum (1, 41, NOT_USED, NOT_USED, channel, pressed);                break; // Volume Up   Group CH9-10
	    case 159   : DSP_Action_ControlVolum (0, 41, NOT_USED, NOT_USED, channel, pressed);                break; // Volume Down Group CH9-10
	    case 160   : DSP_Action_ControlVolum (1, 44, NOT_USED, NOT_USED, channel, pressed);                break; // Volume Up   Group CH11-12
	    case 161   : DSP_Action_ControlVolum (0, 44, NOT_USED, NOT_USED, channel, pressed);                break; // Volume Down Group CH11-12
	    case 162   : DSP_Action_ControlVolum (1, 47, NOT_USED, NOT_USED, channel, pressed);                break; // Volume Up   Group Spdif L+R
	    case 163   : DSP_Action_ControlVolum (0, 47, NOT_USED, NOT_USED, channel, pressed);                break; // Volume Down Group Spdif L+R
	    case 164   :                                                                                       break; // Reserved

	    case 170   : DSP_Action_ControlVolum (1, 30, NOT_USED, NOT_USED, channel, pressed);                break; // Volume Up   CH1
	    case 171   : DSP_Action_ControlVolum (0, 30, NOT_USED, NOT_USED, channel, pressed);                break; // Volume Down CH1
	    case 172   : DSP_Action_ControlVolum (1, 31, NOT_USED, NOT_USED, channel, pressed);                break; // Volume Up   CH2
	    case 173   : DSP_Action_ControlVolum (0, 31, NOT_USED, NOT_USED, channel, pressed);                break; // Volume Down CH2
	    case 174   : DSP_Action_ControlVolum (1, 33, NOT_USED, NOT_USED, channel, pressed);                break; // Volume Up   CH3
	    case 175   : DSP_Action_ControlVolum (0, 33, NOT_USED, NOT_USED, channel, pressed);                break; // Volume Down CH3
	    case 176   : DSP_Action_ControlVolum (1, 34, NOT_USED, NOT_USED, channel, pressed);                break; // Volume Up   CH4
	    case 177   : DSP_Action_ControlVolum (0, 34, NOT_USED, NOT_USED, channel, pressed);                break; // Volume Down CH4
	    case 178   : DSP_Action_ControlVolum (1, 36, NOT_USED, NOT_USED, channel, pressed);                break; // Volume Up   CH5
	    case 179   : DSP_Action_ControlVolum (0, 36, NOT_USED, NOT_USED, channel, pressed);                break; // Volume Down CH5
	    case 180   : DSP_Action_ControlVolum (1, 37, NOT_USED, NOT_USED, channel, pressed);                break; // Volume Up   CH6
	    case 181   : DSP_Action_ControlVolum (0, 37, NOT_USED, NOT_USED, channel, pressed);                break; // Volume Down CH6
	    case 182   : DSP_Action_ControlVolum (1, 39, NOT_USED, NOT_USED, channel, pressed);                break; // Volume Up   CH7
	    case 183   : DSP_Action_ControlVolum (0, 39, NOT_USED, NOT_USED, channel, pressed);                break; // Volume Down CH7
	    case 184   : DSP_Action_ControlVolum (1, 40, NOT_USED, NOT_USED, channel, pressed);                break; // Volume Up   CH8
	    case 185   : DSP_Action_ControlVolum (0, 40, NOT_USED, NOT_USED, channel, pressed);                break; // Volume Down CH8
	    case 186   : DSP_Action_ControlVolum (1, 42, NOT_USED, NOT_USED, channel, pressed);                break; // Volume Up   CH9
	    case 187   : DSP_Action_ControlVolum (0, 42, NOT_USED, NOT_USED, channel, pressed);                break; // Volume Down CH9
	    case 188   : DSP_Action_ControlVolum (1, 43, NOT_USED, NOT_USED, channel, pressed);                break; // Volume Up   CH10
	    case 189   : DSP_Action_ControlVolum (0, 43, NOT_USED, NOT_USED, channel, pressed);                break; // Volume Down CH10
	    case 190   : DSP_Action_ControlVolum (1, 45, NOT_USED, NOT_USED, channel, pressed);                break; // Volume Up   CH11
	    case 191   : DSP_Action_ControlVolum (0, 45, NOT_USED, NOT_USED, channel, pressed);                break; // Volume Down CH11
	    case 192   : DSP_Action_ControlVolum (1, 46, NOT_USED, NOT_USED, channel, pressed);                break; // Volume Up   CH12
	    case 193   : DSP_Action_ControlVolum (0, 46, NOT_USED, NOT_USED, channel, pressed);                break; // Volume Down CH12
	    case 194   : DSP_Action_ControlVolum (1, 48, NOT_USED, NOT_USED, channel, pressed);                break; // Volume Up   CH SPDIF L
	    case 195   : DSP_Action_ControlVolum (0, 48, NOT_USED, NOT_USED, channel, pressed);                break; // Volume Down CH SPDIF L
	    case 196   : DSP_Action_ControlVolum (1, 49, NOT_USED, NOT_USED, channel, pressed);                break; // Volume Up   CH SPDIF R
	    case 197   : DSP_Action_ControlVolum (0, 49, NOT_USED, NOT_USED, channel, pressed);                break; // Volume Down CH SPDIF R
	    case 198   :                                                                                       break; // Reserved

	    case 230   :DSP_PresetRouting(0, 1, pressed);                                                                break; // Preset EQ Up
	    case 231   :DSP_PresetRouting(0, 0, pressed);                                                                break; // Preset EQ Down
	    case 232   :DSP_PresetRouting(1, 1, pressed);                                                                break; // Set preset EQ user 1
	    case 233   :DSP_PresetRouting(1, 2, pressed);                                                                break; // Set preset EQ user 2
	    case 234   :DSP_PresetRouting(1, 3, pressed);                                                                break; // Set preset EQ user 3
	    case 235   :DSP_PresetRouting(1, 4, pressed);                                                                break; // Set preset EQ user 4
	    case 236   :DSP_PresetRouting(1, 5, pressed);                                                                break; // Set preset EQ user 5
	    case 237   :DSP_PresetRouting(1, 6, pressed);                                                                break; // Set preset EQ FLAT
	    case 238   :DSP_PresetRouting(1, 7, pressed);                                                                break; // Set preset EQ Bass
	    case 239   :DSP_PresetRouting(1, 8, pressed);                                                                break; // Set preset EQ BassExtrem
	    case 240   :DSP_PresetRouting(1, 9, pressed);                                                                break; // Set preset EQ BassAndHigh
	    case 241   :DSP_PresetRouting(1, 10, pressed);                                                               break; // Set preset EQ High
	    case 242   :DSP_PresetRouting(1, 11, pressed);                                                               break; // Set preset EQ Classic
	    case 243   :DSP_PresetRouting(1, 12, pressed);                                                               break; // Set preset EQ Dancin
	    case 244   :DSP_PresetRouting(1, 13, pressed);                                                               break; // Set preset EQ Rock
	    case 245   :DSP_PresetRouting(1, 14, pressed);                                                               break; // Set preset EQ Tehno
	    case 246   :DSP_PresetRouting(1, 15, pressed);                                                               break; // Set preset EQ Speaker
	    case 247   :DSP_PresetRouting(1, 16, pressed);                                                               break; // Set preset EQ Live
	    case 248   :DSP_PresetRouting(1, 17, pressed);                                                               break; // Set preset EQ Medium
	    case 249   :DSP_PresetRouting(1, 18, pressed);                                                               break; // Set preset EQ Soft
	    case 250   :DSP_PresetRouting(1, 19, pressed);                                                               break; // Set preset EQ SoftBass
	    case 251   :DSP_PresetRouting(1, 20, pressed);                                                               break; // Set preset EQ SoftHigh
	    case 252   :                                                                                                 break; // Reserved

/*	    case 258   :DSP_PresetRouting(2, 1, pressed); PresetCrossRouting(0, 1, pressed);                                      break; // Preset EQ + Cross Up
	    case 259   :DSP_PresetRouting(2, 0, pressed); PresetCrossRouting(0, 0, pressed);                                      break; // Preset EQ + Cross Down
	    case 260   :DSP_PresetRouting(1, 1, pressed); PresetCrossRouting(1, 1, pressed);                                      break; // Set preset EQ + Cross user 1
	    case 261   :DSP_PresetRouting(1, 2, pressed); PresetCrossRouting(1, 2, pressed);                                      break; // Set preset EQ + Cross user 2
	    case 262   :DSP_PresetRouting(1, 3, pressed); PresetCrossRouting(1, 3, pressed);                                      break; // Set preset EQ + Cross user 3
	    case 263   :DSP_PresetRouting(1, 4, pressed); PresetCrossRouting(1, 4, pressed);                                      break; // Set preset EQ + Cross user 4
	    case 264   :DSP_PresetRouting(1, 5, pressed); PresetCrossRouting(1, 5, pressed);                                      break; // Set preset EQ + Cross user 5


	    case 270   :PresetCrossRouting(0, 1, pressed);                                                             break; // Preset Crossover Up
	    case 271   :PresetCrossRouting(0, 0, pressed);                                                             break; // Preset Crossover Down
	    case 272   :PresetCrossRouting(1, 1, pressed);                                                             break; // Set preset crossover user 1
	    case 273   :PresetCrossRouting(1, 2, pressed);                                                             break; // Set preset crossover user 2
	    case 274   :PresetCrossRouting(1, 3, pressed);                                                             break; // Set preset crossover user 3
	    case 275   :PresetCrossRouting(1, 4, pressed);                                                             break; // Set preset crossover user 4
	    case 276   :PresetCrossRouting(1, 5, pressed);                                                             break; // Set preset crossover user 5
	    case 277   :                                                                                      break; // Reserved*/

	    case 290   :RadioPresetRouting(0, 1, pressed);                                                             break; // Preset radio UP
	    case 291   :RadioPresetRouting(0, 0, pressed);                                                             break; // Preset radio Down
	    case 292   :RadioPresetRouting(1, 1, pressed);                                                             break; // Set preset radio 1
	    case 293   :RadioPresetRouting(1, 2, pressed);                                                             break; // Set preset radio 2
	    case 294   :RadioPresetRouting(1, 3, pressed);                                                             break; // Set preset radio 3
	    case 295   :RadioPresetRouting(1, 4, pressed);                                                             break; // Set preset radio 4
	    case 296   :RadioPresetRouting(1, 5, pressed);                                                             break; // Set preset radio 5
	    case 297   :RadioPresetRouting(1, 6, pressed);                                                             break; // Set preset radio 6
	    case 298   :RadioPresetRouting(1, 7, pressed);                                                             break; // Set preset radio 7
	    case 299   :RadioPresetRouting(1, 8, pressed);                                                             break; // Set preset radio 8
	    case 300   :RadioPresetRouting(1, 9, pressed);                                                             break; // Set preset radio 9
	    case 301   :RadioPresetRouting(1, 10, pressed);                                                            break; // Set preset radio 10
	    case 302   :RadioPresetRouting(1, 11, pressed);                                                            break; // Set preset radio 11
	    case 303   :RadioPresetRouting(1, 12, pressed);                                                            break; // Set preset radio 12
	    case 304   :RadioPresetRouting(1, 13, pressed);                                                            break; // Set preset radio 13
	    case 305   :RadioPresetRouting(1, 14, pressed);                                                            break; // Set preset radio 14
	    case 306   :RadioPresetRouting(1, 15, pressed);                                                            break; // Set preset radio 15
	    case 307   :RadioPresetRouting(1, 16, pressed);                                                            break; // Set preset radio 16
	    case 308   :RadioPresetRouting(1, 17, pressed);                                                            break; // Set preset radio 17
	    case 309   :RadioPresetRouting(1, 18, pressed);                                                            break; // Set preset radio 18
	    case 310   :RadioPresetRouting(1, 19, pressed);                                                            break; // Set preset radio 19
	    case 311   :RadioPresetRouting(1, 20, pressed);                                                            break; // Set preset radio 20
	    case 312   :                                                                                      break; // Reserved

		case 331   : Action_MultiWheel(data1, data2, data3, data4, channel, pressed);  	                  break; // Func MultiWheel
	    case 332   : ActionHID (HID_Keyboard, data1, data2, data3, data4, channel, pressed);              break; // Func Action Hid Keyboard 100
	    case 333   : ActionHID (HID_Consumer, data1, data2, data3, data4, channel, pressed);              break; // Func Action Hid Media 110
	    case 334   : ActionHID (HID_Mouse, data1, data2, data3, data4, channel, pressed);                 break; // Func Action Hid Mouse 120
	    case 335   : ActionHID (HID_Modified,data1, data2, data3, data4, channel, pressed);               break; // Func Action Hid Mod  130
	    case 336   : DSP_Action_ControlVolum (data1, data2, data3, data4, channel, pressed);  	          break; // Func Action DSP Vol Control 140
	    case 337   : DSP_Action_ControlSource (data1, data2, data3, data4, channel, pressed);  	          break; // Func Action DSP Source Control 150
	    case 338   : if(pressed == YES) ActionMenu (data1, channel);                                                         break; // Func Action Display Menu
	   // case 339   : if(pressed == YES) AppStart (data1);                                                                    break; // Func Action App start
	    case 340   :                                                                                      break; // Reserved

	    case 390   :  ActionHID (HID_Mouse, 1, data2, data3, data4, channel, pressed);                    break; // Mouse Left Btn
	    case 391   :  ActionHID (HID_Mouse, 2, data2, data3, data4, channel, pressed);                    break; // Mouse Right Btn
		case 392   :  ActionHID (HID_Mouse, 3, data2, data3, data4, channel, pressed);                    break; // Mouse Centr Btn
		case 393   :  ActionHID (HID_Mouse, 0, data2, data3, data4, channel, pressed);                    break; // Mouse Centr Btn

	    case 400   : if(pressed == YES) HID_ALT_TAB();                                                                       break; // HID Alt+Tab
	    case 401   : ActionHID (HID_Keyboard, 41, 0, 0, 0, channel, pressed);                         break; // Esc
	    case 402   : ActionHID (HID_Keyboard, 300, 0, 0, 0, channel, pressed);                        break; // Sleep
	    case 403   : ActionHID (HID_Keyboard, 301, 0, 0, 0, channel, pressed);                        break; // Power Down
	    case 404   : ActionHID (HID_Keyboard, 302, 0, 0, 0, channel, pressed);                        break; // WakeUp
	    case 405   : ActionHID (HID_Keyboard, 82, 0, 0, 0, channel, pressed);                         break; // Up
	    case 406   : ActionHID (HID_Keyboard, 81, 0, 0, 0, channel, pressed);                         break; // Down
	    case 407   : ActionHID (HID_Keyboard, 80, 0, 0, 0, channel, pressed);                         break; // Left
	    case 408   : ActionHID (HID_Keyboard, 79, 0, 0, 0, channel, pressed);                         break; // Right
	    case 409   : ActionHID (HID_Keyboard, 42, 0, 0, 0, channel, pressed);                         break; // Backspace
	    case 410   : ActionHID (HID_Keyboard, 40, 0, 0, 0, channel, pressed);                         break; // Enter
	    case 411   : ActionHID (HID_Keyboard, 74, 0, 0, 0, channel, pressed);                         break; // Home
	    case 412   : ActionHID (HID_Keyboard, 75, 0, 0, 0, channel, pressed);                         break; // PageUp
	    case 413   : ActionHID (HID_Keyboard, 78, 0, 0, 0, channel, pressed);                         break; // PageDown
	    case 414   : ActionHID (HID_Keyboard, 43, 0, 0, 0, channel, pressed);                         break; // Tab
	    case 415   : ActionHID (HID_Keyboard, 43, 0, 0, 0, channel, pressed);                         break; // Gui
	    case 416   : ActionHID (HID_Keyboard, 58, 0, 0, 0, channel, pressed);                         break; // Sspace
	    case 417   : ActionHID (HID_Keyboard, 58, 0, 0, 0, channel, pressed);                         break; // F1
	    case 418   : ActionHID (HID_Keyboard, 59, 0, 0, 0, channel, pressed);                         break; // F2
	    case 419   : ActionHID (HID_Keyboard, 60, 0, 0, 0, channel, pressed);                         break; // F3
	    case 420   : ActionHID (HID_Keyboard, 61, 0, 0, 0, channel, pressed);                         break; // F4
	    case 421   : ActionHID (HID_Keyboard, 62, 0, 0, 0, channel, pressed);                         break; // F5
	    case 422   : ActionHID (HID_Keyboard, 63, 0, 0, 0, channel, pressed);                         break; // F6
	    case 423   : ActionHID (HID_Keyboard, 64, 0, 0, 0, channel, pressed);                         break; // F7
	    case 424   : ActionHID (HID_Keyboard, 65, 0, 0, 0, channel, pressed);                         break; // F8
	    case 425   : ActionHID (HID_Keyboard, 66, 0, 0, 0, channel, pressed);                         break; // F9
	    case 426   : ActionHID (HID_Keyboard, 67, 0, 0, 0, channel, pressed);                         break; // F10
	    case 427   : ActionHID (HID_Keyboard, 68, 0, 0, 0, channel, pressed);                         break; // F11
	    case 428   : ActionHID (HID_Keyboard, 69, 0, 0, 0, channel, pressed);                         break; // F12

	    case 450   : Bluetooth_Play();                                                                    break; // Bluetooth Play
	    case 451   : Bluetooth_Pause();                                                                   break; // Bluetooth Pause
	    case 452   : Bluetooth_Play_Pause();                                                              break; // Bluetooth PlayPause
	    case 453   : Bluetooth_Stop();                                                                    break; // Bluetooth Stop
	    case 454   : Bluetooth_Track_Next();                                                              break; // Bluetooth Next
	    case 455   : Bluetooth_Track_Prev();                                                              break; // Bluetooth Previus
	    case 456   : Bluetooth_Forward();                                                                 break; // Bluetooth Next
	    case 457   : Bluetooth_Rewind();                                                                  break; // Bluetooth Previus
	    case 458   : Bluetooth_PhoneReply();                                                              break; // Bluetooth Reply
	    case 459   : Bluetooth_PhoneCancel();                                                             break; // Bluetooth Cancell
	    case 460   : Bluetooth_PhoneRedial();                                                             break; // Bluetooth CallBack



	}
}






#define MultiWheel_Set 0
#define MultiWheel_Next 1
#define MultiWheel_Previus 2
#define MultiWheel_HoldNext 3
#define MultiWheel_HoldPrevius 4

void Action_MultiWheel(short data1, short data2, short data3, short data4, short channel, short pressed){

	if(pressed == YES){
		uint8_t sourse = Dixom.Module.DSP.Settings[SoundSource].Data;

		if(data1 == MultiWheel_Set){
			if(sourse==SoursUsb)             { HID_Play_Pause();           }
			else if(sourse==SoursBluetoooth) { Bluetooth_Play_Pause();     }
			else if(sourse==SoursAux)        {                             }
			else if(sourse==SoursRadio)      {                             }
			else if(sourse==SoursSpdif)      {                             }
		}else if(data1 == MultiWheel_Next){
			if(sourse==SoursUsb)             { HID_Track_Next();           }
			else if(sourse==SoursBluetoooth) { Bluetooth_Track_Next();     }
			else if(sourse==SoursAux)        {                             }
			else if(sourse==SoursRadio)      { FmRadio_Preset_Routing(0, 1, YES);   }
			else if(sourse==SoursSpdif)      {                             }
		}else if(data1 == MultiWheel_Previus){
			if(sourse==SoursUsb)             { HID_Track_Prev();           }
			else if(sourse==SoursBluetoooth) { Bluetooth_Track_Prev();     }
			else if(sourse==SoursAux)        {                             }
			else if(sourse==SoursRadio)      { FmRadio_Preset_Routing(0, 0, YES);   }
			else if(sourse==SoursSpdif)      {                             }
		}else if(data1 == MultiWheel_HoldNext){
			if(sourse==SoursUsb)             { HID_Forward();              }
			else if(sourse==SoursBluetoooth) { Bluetooth_Forward();        }
			else if(sourse==SoursAux)        {                             }
			else if(sourse==SoursRadio)      { RadioAutoSearch(1);         }
			else if(sourse==SoursSpdif)      {                             }
		}else if(data1 == MultiWheel_HoldPrevius){
			if(sourse==SoursUsb)             { HID_Rewind();               }
			else if(sourse==SoursBluetoooth) { Bluetooth_Rewind();         }
			else if(sourse==SoursAux)        {                             }
			else if(sourse==SoursRadio)      { RadioAutoSearch(0);         }
			else if(sourse==SoursSpdif)      {                             }
		}
	}
}



void Set(void){
	uint8_t sourse = Dixom.Module.DSP.Settings[SoundSource].Data;

	if(sourse==SoursUsb)             { HID_Play_Pause(); }
	else if(sourse==SoursBluetoooth) { Bluetooth_Track_Next(); }
	else if(sourse==SoursAux)        {}
	else if(sourse==SoursRadio)      { FmRadio_Preset_Routing(0, 1, YES); }
	else if(sourse==SoursSpdif)      {}
}

void Next(void){
	uint8_t sourse = Dixom.Module.DSP.Settings[SoundSource].Data;

	if(sourse==SoursUsb)             { HID_Track_Next(); }
	else if(sourse==SoursBluetoooth) { Bluetooth_Track_Next(); }
	else if(sourse==SoursAux)        {}
	else if(sourse==SoursRadio)      { FmRadio_Preset_Routing(0, 1, YES); }
	else if(sourse==SoursSpdif)      {}
}

void Previous(void){
	uint8_t sourse = Dixom.Module.DSP.Settings[SoundSource].Data;

	if(sourse==SoursUsb)             { HID_Track_Prev(); }
	else if(sourse==SoursBluetoooth) { Bluetooth_Track_Prev(); }
	else if(sourse==SoursAux)        {}
	else if(sourse==SoursRadio)      { FmRadio_Preset_Routing(0, 0, YES); }
	else if(sourse==SoursSpdif)      {}
}

void HoldNext(void){
	uint8_t sourse = Dixom.Module.DSP.Settings[SoundSource].Data;

	if(sourse==SoursUsb)             { HID_Forward(); }
	else if(sourse==SoursBluetoooth) { Bluetooth_Forward(); }
	else if(sourse==SoursAux)        {}
	else if(sourse==SoursRadio)      { RadioAutoSearch(1); }
	else if(sourse==SoursSpdif)      {}
}

void HoldPrevious(void){

	if(Dixom.Module.DSP.Settings[SoundSource].Data==SoursUsb)             { HID_Rewind(); }
	else if(Dixom.Module.DSP.Settings[SoundSource].Data==SoursBluetoooth) { Bluetooth_Rewind(); }
	else if(Dixom.Module.DSP.Settings[SoundSource].Data==SoursAux)        {}
	else if(Dixom.Module.DSP.Settings[SoundSource].Data==SoursRadio)      { RadioAutoSearch(0); }
	else if(Dixom.Module.DSP.Settings[SoundSource].Data==SoursSpdif)      {}
}














