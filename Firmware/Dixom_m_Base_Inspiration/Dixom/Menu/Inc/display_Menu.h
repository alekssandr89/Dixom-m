#ifndef __DISPLAY_MENU_H__
#define __DISPLAY_MENU_H__

#include <Constants.h>

#define MenuBreakTime          5.0     //Second
#define MenuAutoUpdatePeriod   1.0     //Second display auto update period
#define MenuEventDuration      2.0     //Second display duration event
#define MenuErrorDuration      4.0     //Second display duration error
#define MaxParamD              5
#define MaxParamS_BufSize      30

#define TimerFactor            1000     //Second
#define EventBlinkPeriod       0.4      //Second

typedef struct
{
	uint8_t      Count[MaxParamS_BufSize];
	uint8_t      Num[MaxParamD];
} sMenuType;


typedef enum
{
	Action_Left,
	Action_Up,
	Action_Down,
	Action_Right,
	Action_Set,
	Action_None,
	Action_Back,
	Action_OpenMenu
}tMenuAction;

typedef enum
{
	DispError,
	DispErrorCritical,
}tMenuError;

typedef enum
{
	Status_Default,
	Status_Setup,
}tMenuStatus;

typedef enum
{
	Level0,
	Level1,
	Level2,
	Level3,
	Level4,
	Level5,
	Level6,
	MaxMenuLevel
}tMenuLevel;

typedef enum
{
	ChangeSource,
	ChangeVolume,
}tEventWhence;

typedef enum
{
	EventBlink,
	EventStatik,
	EventParam,
}tMenuEvent;

typedef enum
{
	mLevel,
	mPunkt,
}tMenuCurent;

typedef struct
{
	uint8_t      Buf[MaxParamS_BufSize];
	uint16_t     Num[MaxParamD];
	float        Float[MaxParamD];
} sMenuParam;

typedef struct
{
	uint8_t      type;
	uint8_t      blinkState;
	uint8_t      whence;
	uint16_t     timer;
} sMenuEvent;

typedef struct
{
	int8_t       RefreshDisplay;
	int8_t       MenuBusy;
	int8_t       MenuEvent;
	int8_t       MenuError;
	int8_t       MenuStatus;
	uint8_t      MenuLevel_1;
	uint8_t      MenuLevel_2;
	uint8_t      MenuLevel_3;
	int8_t       MenuOpen;
	int8_t       MenuAction;
	uint16_t     TimerBreak;
	sMenuEvent   Event;
	sMenuParam   Param;
} sDisplayMenu;

void ActionDisplay(sDisplayMenu *DisplayMenu);
void DisplayEvent(sDisplayMenu *DisplayMenu);

typedef enum
{
	MainMenuSound,
	MainMenuDataTime,
	MainMenuSettings,
	MainMenuCount
}tDisplayMenuLevel0;


typedef enum
{
	DisplayActionNone            = 0,
	DisplayActionSet             = 1,
	DisplayActionBack            = 2,
	DisplayActionUp              = 3,
	DisplayActionDown            = 4,
	DisplayActionLeft            = 5,
	DisplayActionRight1          = 6,
}tActionDisplay;


typedef struct
{
	int8_t    RefreshDisplay;
	int8_t    SetActionCurrent;
	int8_t    SetMenuCurrent;
	int8_t    SetMenuCurrentLvl1;
	int8_t    SetMenuCurrentLvl2;
	int8_t    SetMenuCurrentLvl3;
	int8_t    SetMenuCurrentLvl4;
	int8_t    SetBusy;
	int8_t    Setup;
	uint16_t  MenuBteakTimer;
	int8_t    MenuLevel;
	uint16_t  Param1;
	uint16_t  Param2;
	float     Param3;
	uint32_t  Param4;
} tSetDisplay;


typedef enum
{
	DisplayMenuFast             = 100,
	DisplayEventVolume          = 101,
	DisplayMenuLevel0           = 0,
	DisplayMenuLevel1           = 1,
	DisplayMenuLevel2           = 2,
	DisplayMenuLevel3           = 3,
}tDisplayMenuLevel;

void             DisplayRefresh(void);
void             DisplayMenuOpen(short MenuNum, short channel);
void             ActionMenu(short Action, short channel);
void             DisplayAction(sDisplayMenu *DisplayMenu);
void             Display_Initialization(void);
void             RefreshDisplay(void);
void             SetDisplayMenu(sDisplayMenu *DisplayMenu);
void             MenuBreakTimerUpdate();
void             DisplayOpenMenu();
void             DisplayCloseMenu();
void             RefresheEventDisplay(tDisplayMenuLevel EventDisplay, uint16_t param1, uint16_t param2, float param3, uint32_t param4);

void             TIM6_mSecondOledDisplay();
void             DisplayAction(sDisplayMenu *DisplayMenu);
void             InitDisplayMenu(void);
void             RefreshDisplay(void);
void             SetDisplayMenu(sDisplayMenu *DisplayMenu);


typedef enum
{
	DisplayMenuMain             = -100,
	DisplayMenuSource           = 0,
	DisplayMenuVolGroup1        = 1,
	DisplayMenuVolGroup2        = 2,
	DisplayMenuVolGroup3        = 3,
	DisplayMenuVolGroup4        = 4,
	DisplayMenuVolGroup5        = 5,
	DisplayMenuVolGroup6        = 6,
	DisplayMenuVolGroupSPDIF    = 7,
	DisplayMenuPresetEQ         = 8,
	DisplayMenuPresetCROSS      = 9,
	DisplayMenuBrightness       = 10,
	DisplayMenuVolControl       = 11,
	DisplayMenuTimeset          = 12,
	DisplayMenuEQ               = 13,
	DisplayMenuCount
}tPMenuDisplay;

typedef enum
{
	PMenuTimesetMin             = 0,
	PMenuTimesetHour            = 1,
	PMenuTimesetCount
}tPMenuTimeset;

typedef enum
{
	DisplayEQcentr           = 0,
	DisplayEQch1             = 1,
	DisplayEQch2             = 2,
	DisplayEQch3             = 3,
	DisplayEQch4             = 4,
	DisplayEQch5             = 5,
	DisplayEQch6             = 6,
	DisplayEQch7             = 7,
	DisplayEQch8             = 8,
	DisplayEQch9             = 9,
	DisplayEQch10            = 10,
	DisplayEQch11            = 11,
	DisplayEQch12            = 12,
	DisplayEQch13            = 13,
	DisplayEQch14            = 14,
	PMenuEQCount
}tPMenuEQ;

typedef enum
{
	DisplayFreq              = 0,
	DisplayBoost             = 1,
	DisplayqFactor           = 2,
	DisplayFiltrType         = 3,
	DisplayGain              = 4,
	PMenuEQpointCount
}tPMenuEQpoint;







#endif

