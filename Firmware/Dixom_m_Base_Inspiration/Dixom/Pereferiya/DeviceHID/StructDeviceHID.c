#include <Description.h>
#include "Exchange.h"
#include "DIX_usb.h"
#include "cmsis_os.h"
#include "StructDeviceHID.h"
#include <Structures.h>

extern sDixom Dixom;

extern TIM_HandleTypeDef htim7;

CustomHID_t CustomHID;
SystemControlHID_t SystemControlHID;
joystickHID_t joystickHID;
mouseHID_t mouseHID;
mediaHID_t mediaHID;
keyboardHID_t keyboardHID;

void StructDeviceHIDInit(){
}

void ActionHID( uint8_t id, short data1, short data2, short data3, short data4, short channel, short pressed){

	if(pressed == YES){
		uint8_t delay = 20;

		switch (id){
		   case HID_Keyboard:

			   if(data1 == 300 || data1 == 301 || data1 == 302 ){
				   SystemControlHID.id = HID_System_Control;
				   if(data1 == 300) SystemControlHID.keys = USB_HID_SYSTEM_SLEEP;
				   if(data1 == 301) SystemControlHID.keys = USB_HID_SYSTEM_POWER_DOWN;
				   if(data1 == 302) SystemControlHID.keys = USB_HID_SYSTEM_WAKE_UP;

				   Transmit_Hid_Device((uint8_t *)&SystemControlHID, 2);
				   osDelay(100);
				   SystemControlHID.keys = 0;
				   Transmit_Hid_Device((uint8_t *)&SystemControlHID, 2);

			   }else{
				   keyboardHID.id = HID_Keyboard;
				   keyboardHID.key1 = data1;
				   keyboardHID.key2 = data2;
				   keyboardHID.key3 = data3;
				   keyboardHID.modifiers = data4;

				   Transmit_Hid_Device((uint8_t *)&keyboardHID, 5);
				   osDelay(delay);

				   keyboardHID.key1 = 0;
				   keyboardHID.key2 = 0;
				   keyboardHID.key3 = 0;
				   keyboardHID.modifiers = 0;

				   Transmit_Hid_Device((uint8_t *)&keyboardHID, 5);
			   }
			 break;
		   case HID_Consumer:
			   mediaHID.id = HID_Consumer;
				mediaHID.keys = data1;
				ExchangeStatus(Transmit_Hid_Device((uint8_t *)&mediaHID, 3)+10, USB_HID_WRITE);
				osDelay(delay);
				mediaHID.keys = 0;
				ExchangeStatus(Transmit_Hid_Device((uint8_t *)&mediaHID, 3)+10, USB_HID_WRITE);
				;
			   break;
		   case HID_Mouse:


			   	if(data1 > 3 || data1 < 0 || data2 > 127 || data2 < -127 || data3 > 127 || data3 < -127 || data4 > 127 || data4 < -127){
 		 	    	TransmitDataOutputs((uint8_t *)GeneralNotifi_CommandError, AutoLenght,  false,  true, allAvailable);

			   	}else{

			   		if(data1 == 3 ) data1 = 4;
			   		data3 = data3 * (-1);

			   		mouseHID.id = HID_Mouse;
			   		mouseHID.buttons = data1;
			   		mouseHID.x = data2;
			   		mouseHID.y = data3;
			   		mouseHID.wheel = data4;

			   		Transmit_Hid_Device((uint8_t *)&mouseHID, 5);

			   		osDelay(delay);

			   		mouseHID.buttons = 0;
			   		mouseHID.x = 0;
			   		mouseHID.y = 0;
			   		mouseHID.wheel = 0;

			   		Transmit_Hid_Device((uint8_t *)&mouseHID, 5);
			   	}
			   break;
		   case HID_System_Control:
			   SystemControlHID.id = HID_System_Control;
			   SystemControlHID.keys = data1;
			   Transmit_Hid_Device((uint8_t *)&SystemControlHID, 2);
			   osDelay(delay);
			   SystemControlHID.keys = 0;
			   Transmit_Hid_Device((uint8_t *)&SystemControlHID, 2);
			   break;
		   case HID_Modified:
				keyboardHID.modifiers = USB_HID_MODIFIER_LEFT_ALT;
				keyboardHID.key1 = USB_HID_TAB;

			   break;

		}
	}
}


void HID_System_PowerDown(void)  {    ActionHID(HID_Keyboard, 301, 0, 0, 0, 0, YES);                   }
void HID_System_Sleep(void)      {	  ActionHID(HID_Keyboard, 300, 0, 0, 0, 0, YES);                   }
void HID_System_Wake_Up(void)    {	  ActionHID(HID_Keyboard, 302, 0, 0, 0, 0, YES);                   }
void HID_VolUP(void)             {    ActionHID(HID_Consumer, USB_HID_VOLUP, 0, 0, 0, 0, YES);         }
void HID_VolDown(void)           {    ActionHID(HID_Consumer, USB_HID_VOLDOWN, 0, 0, 0, 0, YES);       }
void HID_HOME(void)              {                                                             }
void HID_Rewind(void)            {    ActionHID(HID_Consumer, USB_HID_REWUND, 0, 0, 0, 0, YES);        }
void HID_Forward(void)           {    ActionHID(HID_Consumer, USB_HID_FORWARD, 0, 0, 0, 0, YES);       }
void HID_Track_Next(void)        {    ActionHID(HID_Consumer, USB_HID_NEXT, 0, 0, 0, 0, YES);          }
void HID_Track_Prev(void)        {    ActionHID(HID_Consumer, USB_HID_PREV, 0, 0, 0, 0, YES);          }
void HID_Play_Pause(void)        {    ActionHID(HID_Consumer, USB_HID_PLAY_PAUSE, 0, 0, 0, 0, YES);    }
void HID_Mute(void)              {    ActionHID(HID_Consumer, USB_HID_MUTE, 0, 0, 0, 0, YES);          }
void HID_Play(void)              {	  ActionHID(HID_Consumer, USB_HID_PLAY, 0, 0, 0, 0, YES);          }
void HID_Pause(void)             {    ActionHID(HID_Consumer, USB_HID_PAUSE, 0, 0, 0, 0, YES);         }
void HID_Stop(void)              {    ActionHID(HID_Consumer, USB_HID_STOP, 0, 0, 0, 0, YES);          }

void AccOffMediaCommand(void){

	Delay(100, FreeRTOS);
	if(GetSettings(SettingsAccOffComPause) == 0){
		HID_Stop();
	}else{
		HID_Pause();
	}
}

void AccOffSystemCommand(void){

	Delay(100, FreeRTOS);
	if(GetSettings(SettingsAccOffComSleep) == 0){
		HID_System_PowerDown();
	}else{
		HID_System_Sleep();
	}
}


int AltTabTimer=0;
int AltTabStatus=0;

void HID_ALT_TAB(void){
	keyboardHID.id = HID_Keyboard;

	if(AltTabStatus==0){
		AltTabStatus=1;
		AltTabTimer=1;
		keyboardHID.modifiers = USB_HID_MODIFIER_LEFT_ALT;
		keyboardHID.key1 = USB_HID_TAB;
		Transmit_Hid_Device((uint8_t *)&keyboardHID, sizeof(keyboardHID_t));
		osDelay(20);
		keyboardHID.key1 = 0;
		Transmit_Hid_Device((uint8_t *)&keyboardHID, sizeof(keyboardHID_t));
	}else{
		AltTabTimer=1;
		keyboardHID.key1 = USB_HID_TAB;
		Transmit_Hid_Device((uint8_t *)&keyboardHID, sizeof(keyboardHID_t));
		osDelay(20);
		keyboardHID.key1 = 0;
		Transmit_Hid_Device((uint8_t *)&keyboardHID, sizeof(keyboardHID_t));
	}
}

void TIM7_mSecond(){

	if(Dixom.DevInfo.InMode != Initialization){
	    if (AltTabTimer==800&&AltTabStatus==1){
		keyboardHID.modifiers = 0;
		keyboardHID.key1 = 0;
		Transmit_Hid_Device((uint8_t *)&keyboardHID, sizeof(keyboardHID_t));
	    AltTabStatus=0;
	    }else if(AltTabTimer<1000){
	    	 AltTabTimer++;
	    }
	}
}

void HID_NoSleepCommand(uint8_t command){

	switch(command){
	case NoSleepCommandF7      :  ActionHID(HID_Keyboard, 0x40, 0, 0, 0, 0, YES);     break;
	case NoSleepCommandMouse   :  ActionHID(HID_Mouse, 0, 127, -127, 0, 0, YES);      break;
	case NoSleepCommandWakeUp  :  ActionHID(HID_Keyboard, 302, 0, 0, 0, 0, YES);      break;
	case NoSleepCommandF8      :  ActionHID(HID_Keyboard, 0x41, 0, 0, 0, 0, YES);     break;
	}
}


void HID_Joystick(){

	 uint32_t c_tick = HAL_GetTick();
	 joystickHID.buttons = (c_tick / 1000) & 0xff;
	 joystickHID.d_pad = ((c_tick / 1000) >> 8) & 0xff;;
	// HID_SendReport(&hUsbDeviceFS, (uint8_t *)&joystickHID, sizeof(joystickHID));

}

void HID_CustomON() {

	CustomHID.id = 6;
	CustomHID.data = 1;
	CustomHID.data1 = 1;
	CustomHID.data2 = 1;
	CustomHID.data3 = 1;
	//HID_SendReport(&hUsbDeviceFS, (uint8_t *)&CustomHID, sizeof( CustomHID));
}

void HID_CustomOFF(){

	CustomHID.id = 6;
    CustomHID.data = 0;
    CustomHID.data1 = 0;
    CustomHID.data2 = 0;
    CustomHID.data3 = 0;
    //HID_SendReport(&hUsbDeviceFS, (uint8_t *)&CustomHID, sizeof( CustomHID));
}


































