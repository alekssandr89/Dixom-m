#ifndef __STRUCTDEVICEHID_H__
#define __STRUCTDEVICEHID_H__
#include "stdio.h"

//HID Keyboard

// USB keyboard codes
#define USB_HID_MODIFIER_LEFT_CTRL   0x01
#define USB_HID_MODIFIER_LEFT_SHIFT  0x02
#define USB_HID_MODIFIER_LEFT_ALT    0x04
#define USB_HID_MODIFIER_LEFT_GUI    0x08 // (Win/Apple/Meta)
#define USB_HID_MODIFIER_RIGHT_CTRL  0x10
#define USB_HID_MODIFIER_RIGHT_SHIFT 0x20
#define USB_HID_MODIFIER_RIGHT_ALT   0x40
#define USB_HID_MODIFIER_RIGHT_GUI   0x80
#define USB_HID_KEY_L                0x0F
#define USB_HID_TAB                  0x2B
#define USB_HID_F9                   0x42
#define USB_HID_F10                  0x43
#define USB_HID_F11                  0x44
#define USB_HID_F12                  0x45

// USB System Control
#define USB_HID_SYSTEM_POWER_DOWN    0x01
#define USB_HID_SYSTEM_SLEEP         0x02
#define USB_HID_SYSTEM_WAKE_UP       0x04



// USB media codes
#define USB_HID_NEXT        0xB5
#define USB_HID_PREV        0xB6
#define USB_HID_PAUSE       0xB1
#define USB_HID_PLAY        176
#define USB_HID_STOP        183
#define USB_HID_PLAY_PAUSE  205
#define USB_HID_MUTE        0xE2
#define USB_HID_REWUND      0xB4
#define USB_HID_FORWARD     0xB3
#define USB_HID_VOLUP       0xE9
#define USB_HID_VOLDOWN     0xEA




void Multimedia(short data1, short data2, short data3, short data4, short channel, short pressed);
void ActionHID( uint8_t id, short data1, short data2, short data3, short data4, short channel, short pressed);

void             StructDeviceHIDInit(void);

void             HID_Track_Next(void);
void             HID_VolUP(void);
void             HID_VolDown(void);
void             HID_Action(void);
void             HID_HOME(void);


void             HID_Rewind(void);
void             HID_Forward(void);
void             HID_Track_Next(void);
void             HID_Track_Prev(void);
void             HID_Play_Pause(void);
void             HID_Mute(void);
void             HID_Play(void);
void             HID_Pause(void);
void             HID_Stop(void);
void             HID_ALT_TAB(void);
void             HID_NoSleepCommand(uint8_t command);
void             HID_SleepCommand(void);
void             HID_System_Wake_Up(void);

void             TIM7_mSecond();

void             HID_Mouse_Set(short SetBtn, short SetX, short SetY, short SetWheel);
void             HID_Arbitrary_Keyboard(short  key1, short  key2, short  key3, short  modifiers);
void             HID_Arbitrary_Command(short  key, short additionaKey);

void             AccOffSystemCommand(void);
void             AccOffMediaCommand(void);

void             SendMediaKey(void);
void             SendSystemControl(void);
void             HID_System_PowerDown(void);




























#endif
