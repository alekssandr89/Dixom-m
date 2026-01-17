#ifndef __TEXTDISPLAY_H__
#define __TEXTDISPLAY__
#include "display_Menu.h"
#define LCD2x20             0
#define LCD2x40             1

void             TextMenuLevel1(sDisplayMenu *DisplayMenu);
void             TextMenuLevel2_1(sDisplayMenu *DisplayMenu);
void             TextSourceSetup(sDisplayMenu *DisplayMenu);
void             TextGroupVol(sDisplayMenu *DisplayMenu);
void             TextMenuError(sDisplayMenu *DisplayMenu);
void             TextMenuViewError(sDisplayMenu *DisplayMenu);

void             TextBrightness(sDisplayMenu *DisplayMenu);

//void setTexLcdRES(short channel, short value, short pressed);

void             TextDisplayEvent(sDisplayMenu *DisplayMenu);
void             TextDisplayPowerDown(sDisplayMenu *DisplayMenu);

void             TextMenuMain(sDisplayMenu *DisplayMenu);
void             TextMenuSource(tSetDisplay *SetDisplay);
void             TextMenuVolGroup(tSetDisplay *SetDisplay, short VolNum);
void             TextMenuPresetEq(tSetDisplay *SetDisplay);
void             TextMenuPresetCross(tSetDisplay *SetDisplay);
void             TextMenuBrightness(tSetDisplay *SetDisplay);
void             TextMenuLoudness(tSetDisplay *SetDisplay);
void             TextEventVolume(tSetDisplay *SetDisplay);
void             TextMenuEqSetup(tSetDisplay *SetDisplay);
void             TextTimerUpdate(tSetDisplay *SetDisplay);
void             TextMenuTimeset(tSetDisplay *SetDisplay);

void             Display_Text_Character_Creator(uint8_t addr, uint8_t *pData);
void             Display_Text_ProgressBarr(uint8_t start_pos, uint8_t row, uint8_t bar_length, uint8_t fill_max, uint8_t fill_percent);

void             Automatic_Detection_Text_Display_And_Initialization(void);
void             TextDisplayDoneInit(void);
void             TextDisplayMenu();

void             LcdGeneralWhile_Text(void);

void             Menu1Function(void);
void             Menu1FunctionAction(void);

void             Menu1down(void);
void             Menu1up(void);
void             Menu1set(void);

#endif
