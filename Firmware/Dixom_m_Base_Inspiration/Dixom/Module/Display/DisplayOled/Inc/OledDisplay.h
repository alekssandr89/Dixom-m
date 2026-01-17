
#ifndef __OLEDDISPLAY_H__
#define __OLEDDISPLAY_H__


#define Oled_Menu_Type1 0
#include "display_Menu.h"


void             OledMenuLevel1(sDisplayMenu *DisplayMenu);
void             OledMenuLevel2_1(sDisplayMenu *DisplayMenu);
void             OledSourceSetup(sDisplayMenu *DisplayMenu);
void             OledBrightness(sDisplayMenu *DisplayMenu);
void             OledGroupVol(sDisplayMenu *DisplayMenu);

void             OledMenuMain(sDisplayMenu *DisplayMenu);
void             OledMenuPowerDown(sDisplayMenu *DisplayMenu);
void             OledDisplayEvent(sDisplayMenu *DisplayMenu);
void             OledMenuBluetoothPhone(sDisplayMenu *DisplayMenu);

void             OledMenuSource(tSetDisplay *SetDisplay);
void             OledMenuVolGroup(tSetDisplay *SetDisplay, short VolNum);
void             OledMenuPresetEq(tSetDisplay *SetDisplay);
void             OledMenuPresetCross(tSetDisplay *SetDisplay);
void             OledMenuBrightness(tSetDisplay *SetDisplay);
void             OledMenuLoudness(tSetDisplay *SetDisplay);
void             OledTimerUpdate(tSetDisplay *SetDisplay);
void             OledMenuTimeset(tSetDisplay *SetDisplay);

void             Automatic_Detection_Oled_Display_And_Initialization(void);
void             OledDisplayDoneInit(void);
void             mSecondOledTimer(void);

void             LoadingOled(short staus);

void             OledLCD_GoodBay();


#endif
