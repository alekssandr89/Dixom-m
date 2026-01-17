#include <Sigma.h>
#include "display_Menu.h"
#include "RTC.h"
#include "math.h"
#include "Exchange.h"
#include "OledDisplay.h"
#include "TextDisplay.h"
#include "Nextion_LCD.h"
#include "MicroMenu.h"
#include "Action_displayMenu.h"
#include <Structures.h>

extern sDixom Dixom;

sDisplayMenu DisplayMenu;
uint8_t ResetMenu = 0;

#define dDisplay Dixom.Module.Display

extern char PointPick[1];

//uint8_t StateTextDisplay = 0;
//uint8_t StateOledDisplay = 0;

//Menu_Item_t DixomMenu;
float  MenuAutoUpdateTimer = MenuAutoUpdatePeriod;
static void Generic_Write(const char* Text);
static void SetMenuSetup(void);
static void OpenMenu_0_0_0(void);
static void OpenMenu_1_0_0(void);
static void OpenMenu_2_0_0(void);
static void OpenMenu_2_1_0(void);
static void OpenMenu_2_2_0(void);
static void OpenMenu_2_3_0(void);
static void OpenMenu_2_2_1(void);
static void OpenMenu_2_3_1(void);
static void OpenMenu_5_0_0(void);
static void OpenMenu_5_1_0(void);
static void OpenMenu_6_0_0(void);
static void OpenMenu_6_1_0(void);

#define Menu000  "0 0 0"  //Main screen
#define Menu100  "1 0 0"  //Fast settings
#define Menu200  "2 0 0"  //Sound settings
#define Menu300  "3 0 0"  //Platform settings

#define Menu110  "1 1 0" //Source
#define Menu120  "1 2 0" //Brightness

#define Menu210  "2 1 0" //SOURCE
#define Menu220  "2 2 0" //VOLUME CONTROL
#define Menu221  "2 2 1" //VOLUME CONTROL

static void MenuLevel_1(void);
static void Menu2_Level2(void);
static void Menu_Source(void);
static void Menu_Brightness(void);
static void Menu_GroupVol(void);
static void Menu_Error(void);
static void Menu_View_Error(void);

static void Menu_reinit_dac(void);
static void Menu_reinit_dsp(void);
static void Menu_reinit_sound(void);


int dMenuAutoUpdatePeriod, dMenuBreakTime, dMenuEventDuration, dMenuErrorDuration, dEventBlinkPeriod, commonTimer = 0;


//         Name          | Down  ↓        | Up ↑           | Left  ←        | Right  →       | SelectFunction   | EnterFunction      | Text
MENU_ITEM( Menu_0        , NULL_MENU      , NULL_MENU      , NULL_MENU      , NULL_MENU      , NULL             , NULL               , "0 0 0"); //Main screen

MENU_ITEM( Menu_1        , Menu_2         , Menu_6         , Menu_0         , Menu_0         , MenuLevel_1      , OpenMenu_0_0_0     , "1 0 0"); //Back
MENU_ITEM( Menu_2        , Menu_3         , Menu_1         , Menu_0         , Menu_2_1       , MenuLevel_1      , OpenMenu_2_1_0     , "2 0 0"); //Fast settings
MENU_ITEM( Menu_3        , Menu_4         , Menu_2         , Menu_0         , NULL_MENU      , MenuLevel_1      , NULL               , "3 0 0"); //Sound settings
MENU_ITEM( Menu_4        , Menu_5         , Menu_3         , Menu_0         , NULL_MENU      , MenuLevel_1      , NULL               , "4 0 0"); //Platform settings
MENU_ITEM( Menu_5        , Menu_6         , Menu_4         , Menu_0         , NULL_MENU/*Menu_5_1*/       , MenuLevel_1      , NULL/*OpenMenu_5_1_0*/     , "5 0 0"); //SystemSettings
MENU_ITEM( Menu_6        , Menu_1         , Menu_5         , Menu_0         , NULL_MENU      , MenuLevel_1      , NULL/*OpenMenu_6_1_0*/     , "6 0 0"); //Error statistika

MENU_ITEM( Menu_2_1      , Menu_2_2       , Menu_2_4       , Menu_2         , Menu_2         , Menu2_Level2     , OpenMenu_2_0_0     , "2 1 0"); //Back
MENU_ITEM( Menu_2_2      , Menu_2_3       , Menu_2_1       , Menu_2         , NULL_MENU      , Menu_Source      , SetMenuSetup       , "2 2 0"); //Source
MENU_ITEM( Menu_2_3      , Menu_2_4       , Menu_2_2       , Menu_2         , Menu_2_3_1     , Menu2_Level2     , OpenMenu_2_3_1     , "2 3 0"); //Group vol
MENU_ITEM( Menu_2_4      , Menu_2_1       , Menu_2_3       , Menu_2         , NULL_MENU      , Menu_Brightness  , SetMenuSetup       , "2 4 0"); //Brightness

MENU_ITEM( Menu_2_2_1    , Menu_2_2_2     , Menu_2_2_2     , Menu_2_2       , NULL_MENU      , Menu_Source      , OpenMenu_2_2_0     , "2 2 1"); //Back
MENU_ITEM( Menu_2_2_2    , Menu_2_2_1     , Menu_2_2_1     , Menu_2_2       , NULL_MENU      , Menu_Source      , SetMenuSetup       , "2 2 2"); //Source setup

MENU_ITEM( Menu_2_3_1    , Menu_2_3_2     , Menu_2_3_8     , Menu_2_3       , NULL_MENU      , Menu_GroupVol    , OpenMenu_2_3_0     , "2 3 1"); //Back
MENU_ITEM( Menu_2_3_2    , Menu_2_3_3     , Menu_2_3_1     , Menu_2_3       , NULL_MENU      , Menu_GroupVol    , SetMenuSetup       , "2 3 2"); //GROUP1
MENU_ITEM( Menu_2_3_3    , Menu_2_3_4     , Menu_2_3_2     , Menu_2_3       , NULL_MENU      , Menu_GroupVol    , SetMenuSetup       , "2 3 3"); //GROUP2
MENU_ITEM( Menu_2_3_4    , Menu_2_3_5     , Menu_2_3_3     , Menu_2_3       , NULL_MENU      , Menu_GroupVol    , SetMenuSetup       , "2 3 4"); //GROUP3
MENU_ITEM( Menu_2_3_5    , Menu_2_3_6     , Menu_2_3_4     , Menu_2_3       , NULL_MENU      , Menu_GroupVol    , SetMenuSetup       , "2 3 5"); //GROUP4
MENU_ITEM( Menu_2_3_6    , Menu_2_3_7     , Menu_2_3_5     , Menu_2_3       , NULL_MENU      , Menu_GroupVol    , SetMenuSetup       , "2 3 6"); //GROUP5
MENU_ITEM( Menu_2_3_7    , Menu_2_3_8     , Menu_2_3_6     , Menu_2_3       , NULL_MENU      , Menu_GroupVol    , SetMenuSetup       , "2 3 7"); //GROUP6
MENU_ITEM( Menu_2_3_8    , Menu_2_3_1     , Menu_2_3_7     , Menu_2_3       , NULL_MENU      , Menu_GroupVol    , SetMenuSetup       , "2 3 8"); //GROUP7

MENU_ITEM( Menu_5_1      , Menu_5_2       , Menu_5_4       , Menu_5         , NULL_MENU      , Menu_Error       , OpenMenu_5_0_0     , "5 1 0"); //Back
MENU_ITEM( Menu_5_2      , Menu_5_3       , Menu_5_1       , Menu_5         , NULL_MENU      , Menu_Error       , Menu_reinit_dac    , "5 2 0"); //Dac reinit
MENU_ITEM( Menu_5_3      , Menu_5_4       , Menu_5_2       , Menu_5         , NULL_MENU      , Menu_Error       , Menu_reinit_dsp    , "5 3 0"); //dsp reinit
MENU_ITEM( Menu_5_4      , Menu_5_1       , Menu_5_3       , Menu_5         , NULL_MENU      , Menu_Error       , Menu_reinit_sound  , "5 4 0"); //sound reinit

MENU_ITEM( Menu_6_1      , Menu_6_2       , Menu_6_8       , Menu_6         , NULL_MENU      , Menu_View_Error  , OpenMenu_6_0_0     , "6 1 0"); //Back
MENU_ITEM( Menu_6_2      , Menu_6_3       , Menu_6_1       , Menu_6         , NULL_MENU      , Menu_View_Error  , NULL               , "6 2 0"); //Dac reinit
MENU_ITEM( Menu_6_3      , Menu_6_4       , Menu_6_2       , Menu_6         , NULL_MENU      , Menu_View_Error  , NULL               , "6 3 0"); //dsp reinit
MENU_ITEM( Menu_6_4      , Menu_6_5       , Menu_6_3       , Menu_6         , NULL_MENU      , Menu_View_Error  , NULL               , "6 4 0"); //sound reinit
MENU_ITEM( Menu_6_5      , Menu_6_6       , Menu_6_4       , Menu_6         , NULL_MENU      , Menu_View_Error  , NULL               , "6 5 0"); //Back
MENU_ITEM( Menu_6_6      , Menu_6_7       , Menu_6_5       , Menu_6         , NULL_MENU      , Menu_View_Error  , NULL               , "6 6 0"); //Back
MENU_ITEM( Menu_6_7      , Menu_6_8       , Menu_6_6       , Menu_6         , NULL_MENU      , Menu_View_Error  , NULL               , "6 7 0"); //Back
MENU_ITEM( Menu_6_8      , Menu_6_1       , Menu_6_7       , Menu_6         , NULL_MENU      , Menu_View_Error  , NULL               , "6 8 0"); //Back


static void SetMenuSetup(void){
	if(DisplayMenu.MenuStatus == Status_Default){
		DisplayMenu.MenuStatus = Status_Setup;
	}else{
		DisplayMenu.MenuStatus = Status_Default;
	}
}

static void MenuLevel_1(void)            {  if(dDisplay.StateTextDisplay==ON) TextMenuLevel1(&DisplayMenu);
	                                        if(dDisplay.StateOledDisplay==ON) OledMenuLevel1(&DisplayMenu);}

static void Menu2_Level2(void)           {  if(dDisplay.StateTextDisplay==ON) TextMenuLevel2_1(&DisplayMenu);
	                                        if(dDisplay.StateOledDisplay==ON) OledMenuLevel2_1(&DisplayMenu);}

static void Menu_Source(void)            {  DisplayAction_SourceSetup(&DisplayMenu);
	                                        if(dDisplay.StateTextDisplay==ON) TextSourceSetup(&DisplayMenu);
	                                        if(dDisplay.StateOledDisplay==ON) OledSourceSetup(&DisplayMenu);}

static void Menu_Brightness(void)        {  DisplayAction_LcdBrightness(&DisplayMenu);
                                            if(dDisplay.StateTextDisplay==ON) TextBrightness(&DisplayMenu);
                                            if(dDisplay.StateOledDisplay==ON) OledBrightness(&DisplayMenu);}

static void Menu_GroupVol(void)          {  DisplayAction_GroupVol(&DisplayMenu);
                                            if(dDisplay.StateTextDisplay==ON) TextGroupVol(&DisplayMenu);
                                            if(dDisplay.StateOledDisplay==ON) OledGroupVol(&DisplayMenu);}

static void Menu_Error(void)             {/* 	TextMenuError(&DisplayMenu);  */      }
static void Menu_View_Error(void)        {	/*TextMenuViewError(&DisplayMenu); */   }

static void Menu_reinit_dac(void)        { DAC_REINIT();                        }
static void Menu_reinit_dsp(void)        { DSP_REINIT();                        }
static void Menu_reinit_sound(void)      { SOUND_REINIT();	                    }


static void OpenMenu_0_0_0(){        	Menu_Navigate(&Menu_0);	 DisplayMenu.RefreshDisplay = YES;         }
static void OpenMenu_1_0_0(){        	Menu_Navigate(&Menu_1);	          }
static void OpenMenu_2_0_0(){        	Menu_Navigate(&Menu_2);	          }
static void OpenMenu_5_0_0(){        	Menu_Navigate(&Menu_5);	          }
static void OpenMenu_2_1_0(){        	Menu_Navigate(&Menu_2_1);	      }
static void OpenMenu_2_2_0(){        	Menu_Navigate(&Menu_2_2);	      }
static void OpenMenu_2_3_0(){        	Menu_Navigate(&Menu_2_3);	      }
static void OpenMenu_2_2_1(){        	Menu_Navigate(&Menu_2_2_1);	      }
static void OpenMenu_2_3_1(){        	Menu_Navigate(&Menu_2_3_1);	      }
static void OpenMenu_5_1_0(){        	Menu_Navigate(&Menu_5_1);	      }
static void OpenMenu_6_0_0(){        	Menu_Navigate(&Menu_6);	          }
static void OpenMenu_6_1_0(){        	Menu_Navigate(&Menu_6_1);	      }

void DisplayOpenMenu(){
	if(Dixom.Module.Bluetooth.InMode == IncomingCall){
		Bluetooth_PhoneCancel();
	}else if(dDisplay.StateTextDisplay == ON || dDisplay.StateOledDisplay == ON){
		Menu_Navigate(&Menu_1);
		DisplayMenu.MenuLevel_1 = 1;
		DisplayMenu.RefreshDisplay = YES;
	}
}

void DisplayCloseMenu(){
	Menu_Navigate(&Menu_0);
	DisplayMenu.MenuLevel_1 = 0;
	DisplayMenu.RefreshDisplay = YES;

}


static void Generic_Write(const char* Text)
{
	sscanf(Text, "%hhu %hhu %hhu", &DisplayMenu.MenuLevel_1, &DisplayMenu.MenuLevel_2, &DisplayMenu.MenuLevel_3);

	if(DisplayMenu.MenuLevel_1 > 0){
		DisplayMenu.MenuOpen = YES;
	}else{
		DisplayMenu.MenuOpen = NO;
	}

	TransmitDataOutputs((uint8_t *)Text, AutoLenght,  false,  true, allAvailable);

}


void Display_Initialization(void){

	Menu_SetGenericWriteCallback(Generic_Write);
	Menu_Navigate(&Menu_0);

	dMenuAutoUpdatePeriod = MenuAutoUpdatePeriod * TimerFactor;
	dMenuBreakTime = MenuBreakTime * TimerFactor;
	dMenuEventDuration = MenuEventDuration * TimerFactor;
	dEventBlinkPeriod = EventBlinkPeriod * TimerFactor;
	dMenuErrorDuration = MenuErrorDuration * TimerFactor;


	DisplayMenu.RefreshDisplay      = NO;
	DisplayMenu.MenuBusy            = NO;
	DisplayMenu.MenuEvent           = NO;
	DisplayMenu.MenuError           = NO;
	DisplayMenu.MenuLevel_1         = NO;
	DisplayMenu.MenuLevel_2         = NO;
	DisplayMenu.MenuLevel_3         = NO;
	DisplayMenu.MenuOpen            = NO;
	DisplayMenu.MenuAction          = Action_None;
	DisplayMenu.MenuStatus          = Status_Default;
	DisplayMenu.TimerBreak          = dMenuBreakTime;
	DisplayMenu.Event.type          = EventBlink;
	DisplayMenu.Event.timer         = dMenuEventDuration;
	DisplayMenu.Event.whence        = 0;
	DisplayMenu.Event.blinkState    = OFF;

	for(uint8_t count = 0; count<MaxParamD; count++){
		DisplayMenu.Param.Num[count]            = NO;
		DisplayMenu.Param.Float[count]            = NO;
	}

	for(uint8_t count = 0; count<MaxParamS_BufSize; count++){
		DisplayMenu.Param.Buf[count]            = NO;
	}


	//19 Automatic detection of an OLED display connection and its initialization
	Automatic_Detection_Oled_Display_And_Initialization();

	//20 Automatic detection of TEXT display connection and its initialization
	Automatic_Detection_Text_Display_And_Initialization();

	//20 Automatic detection of Nextion display connection and its initialization
	Nextion_Display_Automatic_Detection();
}


void ActionMenu(short Action, short channel){


	if(Action_Set == Action && Dixom.Module.Bluetooth.InMode == IncomingCall){
		Bluetooth_PhoneReply();
	}else if(Action_Set == Action && (Dixom.Module.Bluetooth.InMode == OutgoingCall || Dixom.Module.Bluetooth.InMode == ActiveCall)){
		Bluetooth_PhoneCancel();
	}else if(DisplayMenu.MenuOpen==YES){

		DisplayMenu.MenuAction = Action;

		if(DisplayMenu.MenuAction != Action_None ){

			switch(DisplayMenu.MenuAction)
			{
			case Action_Left :
				Menu_Navigate(MENU_PARENT);
				DisplayMenu.MenuStatus = Status_Default;
				break;
			case Action_Up:
				if(DisplayMenu.MenuStatus != Status_Setup ){
					Menu_Navigate(MENU_PREVIOUS);
				}else{
					Menu_SelectCallback();
				}
				break;
			case Action_Down:
				if(DisplayMenu.MenuStatus != Status_Setup ){
					Menu_Navigate(MENU_NEXT);
				}else{
					Menu_SelectCallback();
				}
				break;
			case Action_Right:
				Menu_Navigate(MENU_CHILD);

				break;
			case Action_Set:
				Menu_EnterCurrentItem();
				break;
			default:
				break;
			}

			DisplayMenu.MenuAction = Action_None;
			MenuBreakTimerUpdate();
		}

		DisplayMenu.RefreshDisplay = YES;

	}else{
		switch(Action)
		{
		case Action_Left :	 DSP_Action_ControlSource (0, 0, 0, 0, 0, YES);   break;
		case Action_Up:  	 DSP_Action_ControlVolum  (0, 0, 0, 0, 0, YES);   break;
		case Action_Down:    DSP_Action_ControlVolum  (1, 0, 0, 0, 0, YES);   break;
		case Action_Right:   DSP_Action_ControlSource (0, 1, 0, 0, 0, YES);   break;
		case Action_Set:     DSP_Action_ControlSource (0, 1, 0, 0, 0, YES);   break;
		default:                                						   break;
		}
	}
}

void SetDisplayMenu(sDisplayMenu *DisplayMenu){
	DisplayMenu->RefreshDisplay = YES;
}

void RefreshDisplay(void){
	DisplayMenu.RefreshDisplay = YES;
}

void DisplayRefresh(void){

	if(ResetMenu == 1){
		DisplayCloseMenu();
		ResetMenu = 0;
		MenuBreakTimerUpdate();
	}

//	if(DisplayMenu.RefreshDisplay == YES){

		DisplayMenu.MenuBusy = YES;

		if(Dixom.DevInfo.InMode == PowerDown){

			if(dDisplay.StateTextDisplay==ON) TextDisplayPowerDown(&DisplayMenu);
			if(dDisplay.StateOledDisplay==ON) OledMenuPowerDown(&DisplayMenu);

		}
		else if(Dixom.Module.Bluetooth.InMode==OutgoingCall || Dixom.Module.Bluetooth.InMode==IncomingCall || Dixom.Module.Bluetooth.InMode==ActiveCall){
			OledMenuBluetoothPhone(&DisplayMenu);
		}
		else if(DisplayMenu.MenuEvent == YES  &&  DisplayMenu.MenuLevel_1 == 0){

			if(DisplayMenu.Event.timer == 0 ){
				DisplayMenu.Event.timer = dMenuEventDuration;
				DisplayMenu.MenuEvent = NO;
				DisplayMenu.Event.blinkState = ON;
			}

			if(dDisplay.StateTextDisplay==ON) TextDisplayEvent(&DisplayMenu);
			if(dDisplay.StateOledDisplay==ON) OledDisplayEvent(&DisplayMenu);


		}else if(DisplayMenu.MenuLevel_1 == 0){

			if(dDisplay.StateTextDisplay==ON) TextMenuMain(&DisplayMenu);
			if(dDisplay.StateOledDisplay==ON) OledMenuMain(&DisplayMenu);

		}else{
			Menu_SelectCallback();
		}

		//OledMenuLevel1(&DisplayMenu);


		DisplayMenu.MenuBusy = NO;
		DisplayMenu.RefreshDisplay = NO;
		dMenuAutoUpdatePeriod = MenuAutoUpdatePeriod * TimerFactor;


	//}
/*
	if(DisplayMenu.MenuAction != Action_None ){
		switch(DisplayMenu.MenuAction)
		{
		case Action_Left :
			Menu_Navigate(MENU_PARENT);
			DisplayMenu.MenuStatus = Status_Default;
			break;
		case Action_Up:
			if(DisplayMenu.MenuStatus != Status_Setup ){
				Menu_Navigate(MENU_PREVIOUS);
			}else{
				Menu_SelectCallback();
			}
			break;
		case Action_Down:
			if(DisplayMenu.MenuStatus != Status_Setup ){
				Menu_Navigate(MENU_NEXT);
			}else{
				Menu_SelectCallback();
			}
			break;
		case Action_Right:
			Menu_Navigate(MENU_CHILD);

			break;
		case Action_Set:
			Menu_EnterCurrentItem();
			break;
		default:
			break;
		}

		DisplayMenu.MenuAction = Action_None;
		MenuBreakTimerUpdate();
	}*/
}


void MenuBreakTimerUpdate(){
	DisplayMenu.TimerBreak = MenuBreakTime * TimerFactor;
}





void DisplayEvent(sDisplayMenu *DisplayMenu){
	DisplayMenu->RefreshDisplay = YES;
}

void DisplaySet(sDisplayMenu *DisplayMenu){
	DisplayMenu->RefreshDisplay = YES;
}

float ParamEventBlinkTimer = 0;

void TIM6_DisplayTimer(){

	if(DisplayMenu.MenuLevel_1 != Level0 ){
		if(DisplayMenu.TimerBreak != 0 ){
			DisplayMenu.TimerBreak--;
		}else{
			ResetMenu = 1;
			DisplayMenu.MenuStatus = Status_Default;
		}
	}

	/////////// EVENT TIMER/////////
	if(DisplayMenu.MenuEvent== YES){
		if(DisplayMenu.Event.timer != 0 ){
			DisplayMenu.Event.timer--;
		}
	}

	/////////// BLINK TIMER/////////
	if(ParamEventBlinkTimer == dEventBlinkPeriod && DisplayMenu.Event.blinkState == OFF){
		DisplayMenu.Event.blinkState = ON;
		if(DisplayMenu.Event.type == EventBlink) DisplayMenu.RefreshDisplay = YES;
	}else if(ParamEventBlinkTimer == dEventBlinkPeriod*2){
		DisplayMenu.Event.blinkState = OFF;
		if(DisplayMenu.Event.type == EventBlink) DisplayMenu.RefreshDisplay = YES;
		ParamEventBlinkTimer = 0;
	}else{
		ParamEventBlinkTimer++;
	}

	/////////// AUTO UPDATE/////////
	if(dMenuAutoUpdatePeriod == 0){
		DisplayMenu.RefreshDisplay = YES;
	}else{
		dMenuAutoUpdatePeriod--;
	}

}


void Initialization_Display_Menu(void){
	InitDisplayMenu();
}


short TimeSecondUpdate = 0;
uint8_t SecondTick = 0;



void TIM6_mSecondOledDisplay(){
	if(Dixom.DevInfo.InMode != Initialization){

		TimeSecondUpdate++;
		mSecondOledTimer();
		TIM6_DisplayTimer();

		if(TimeSecondUpdate>500){

			if(SecondTick == 0){
				SecondTick = 1;
				PointPick[0] = ' ';
			}else{
				SecondTick = 0;
				PointPick[0] = ':';
			}
			TimeSecondUpdate = 0;
			RefreshDisplay();
		}
	}
}



/*

#define MenuAcationUp    SetDisplay->SetActionCurrent == DisplayActionUp
#define MenuAcationDown  SetDisplay->SetActionCurrent == DisplayActionDown*/
/*
void DisplayAction(tSetDisplay *SetDisplay){

	if(SetDisplay->SetMenuCurrent == DisplayMenuMain || SetDisplay->SetMenuCurrent == DisplayMenuFast ){      // Цикл главного экрана с быстрым меню
		if(SetDisplay->SetActionCurrent == DisplayActionUp){
			VolumUP(DacVolum);
			SetDisplay->MenuBteakTimer = MenuBreakTime;
		}else if(SetDisplay->SetActionCurrent == DisplayActionDown){
			VolumDown(DacVolum);
			SetDisplay->MenuBteakTimer = MenuBreakTime;
		}else if(SetDisplay->SetActionCurrent == DisplayActionSet){
			SoursRouting(1);
			SetDisplay->SetMenuCurrent = DisplayMenuFast;
			SetDisplay->MenuBteakTimer = 0;
		}
	}else{

		if(SetDisplay->SetActionCurrent == DisplayActionBack){
			SetDisplay->MenuBteakTimer = MenuBreakTime;
		}

		if(SetDisplay->MenuLevel == DisplayMenuLevel0 && SetDisplay->SetActionCurrent == DisplayActionSet){
			SetDisplay->MenuLevel = DisplayMenuLevel1;
			SetDisplay->Setup = 1;
		}else if(SetDisplay->MenuLevel == DisplayMenuLevel1 && SetDisplay->SetActionCurrent == DisplayActionSet){
			SetDisplay->MenuLevel = DisplayMenuLevel0;
			SetDisplay->Setup = 0;
		}

		if(SetDisplay->MenuLevel == DisplayMenuLevel0){
			if(MenuAcationUp){

				SetDisplay->SetMenuCurrent++;
				if(SetDisplay->SetMenuCurrent > DisplayMenuCount-1){
					SetDisplay->SetMenuCurrent = DisplayMenuSource;
				}
			}else if(MenuAcationDown){

				SetDisplay->SetMenuCurrent--;
				if(SetDisplay->SetMenuCurrent < DisplayMenuSource){
					SetDisplay->SetMenuCurrent = DisplayMenuCount -1;
				}
			}
		}else{
			if(SetDisplay->SetMenuCurrent==DisplayMenuSource){	            if(MenuAcationUp){ SoursRouting(1);	         }else if(MenuAcationDown){ SoursRouting(0);            }
			}else if(SetDisplay->SetMenuCurrent==DisplayMenuVolGroup1){		if(MenuAcationUp){ VolumUP(29);		         }else if(MenuAcationDown){ VolumDown(29);	            }
			}else if(SetDisplay->SetMenuCurrent==DisplayMenuVolGroup2){		if(MenuAcationUp){ VolumUP(32);	             }else if(MenuAcationDown){ VolumDown(32);	            }
			}else if(SetDisplay->SetMenuCurrent==DisplayMenuVolGroup3){		if(MenuAcationUp){ VolumUP(35);	             }else if(MenuAcationDown){ VolumDown(35);              }
			}else if(SetDisplay->SetMenuCurrent==DisplayMenuVolGroup4){		if(MenuAcationUp){ VolumUP(38);	             }else if(MenuAcationDown){ VolumDown(38);	            }
			}else if(SetDisplay->SetMenuCurrent==DisplayMenuVolGroup5){		if(MenuAcationUp){ VolumUP(41);    	         }else if(MenuAcationDown){ VolumDown(41);	            }
			}else if(SetDisplay->SetMenuCurrent==DisplayMenuVolGroup6){		if(MenuAcationUp){ VolumUP(44);	             }else if(MenuAcationDown){ VolumDown(44);	            }
			}else if(SetDisplay->SetMenuCurrent==DisplayMenuVolGroupSPDIF){	if(MenuAcationUp){ VolumUP(47);	             }else if(MenuAcationDown){ VolumDown(47);	            }
			}else if(SetDisplay->SetMenuCurrent==DisplayMenuPresetEQ){ 		if(MenuAcationUp){ SetEqPresetDir(1);        }else if(MenuAcationDown){ SetEqPresetDir(0);	        }
			}else if(SetDisplay->SetMenuCurrent==DisplayMenuPresetCROSS){	if(MenuAcationUp){ SetCrossPresetDir(1);     }else if(MenuAcationDown){	SetCrossPresetDir(0);		}
			}else if(SetDisplay->SetMenuCurrent==DisplayMenuBrightness){    if(MenuAcationUp){ SetDisplayBrightness(1);  }else if(MenuAcationDown){ SetDisplayBrightness(0);    }
			}else if(SetDisplay->SetMenuCurrent==DisplayMenuVolControl){    if(MenuAcationUp){ VolumeControlSelect(1);	 }else if(MenuAcationDown){ VolumeControlSelect(0);	    }
			}else if(SetDisplay->SetMenuCurrent==DisplayMenuTimeset){       TimeSetOnDisplay(SetDisplay);
		    }else if(SetDisplay->SetMenuCurrent==DisplayMenuEQ){            EqSetupOnDisplay(SetDisplay);
		    }
		}
		SetDisplay->MenuBteakTimer = 0;
	}
}
*/
/*

void DisplayRefresh(void){ //заходим сюда каждую милисекунду

	//RefreshDisplay();

	if(SetDisplay.RefreshDisplay == YES){

		SetDisplay.SetBusy = YES;
		if(LcdLoadingState == NO && SetDisplay.SetMenuCurrent == DisplayMenuMain){        // Цикл главного экрана

			OledMenuMain(&SetDisplay);
			TextMenuMain(&SetDisplay);

		}else if(LcdLoadingState == NO && SetDisplay.SetMenuCurrent == DisplayMenuFast){  //Цикл быстрого меню

			OledMenuSource(&SetDisplay);
			TextMenuMain(&SetDisplay);

		}else if(LcdLoadingState == NO && SetDisplay.SetMenuCurrent == DisplayEventVolume){  //Цикл отображения событий изменения громкости

			OledEventVolume(&SetDisplay);
			TextEventVolume(&SetDisplay);

		}else if(LcdLoadingState == NO){                                                  // Цикл отображения меню

			switch(SetDisplay.SetMenuCurrent){
			case DisplayMenuSource        : OledMenuSource(&SetDisplay);          TextMenuSource(&SetDisplay);             break;
			case DisplayMenuVolGroup1     : OledMenuVolGroup(&SetDisplay, 29);    TextMenuVolGroup(&SetDisplay, 29);       break;
			case DisplayMenuVolGroup2     : OledMenuVolGroup(&SetDisplay, 32);    TextMenuVolGroup(&SetDisplay, 32);       break;
			case DisplayMenuVolGroup3     : OledMenuVolGroup(&SetDisplay, 35);    TextMenuVolGroup(&SetDisplay, 35);       break;
			case DisplayMenuVolGroup4     : OledMenuVolGroup(&SetDisplay, 38);    TextMenuVolGroup(&SetDisplay, 38);       break;
			case DisplayMenuVolGroup5     : OledMenuVolGroup(&SetDisplay, 41);    TextMenuVolGroup(&SetDisplay, 41);       break;
			case DisplayMenuVolGroup6     : OledMenuVolGroup(&SetDisplay, 44);    TextMenuVolGroup(&SetDisplay, 44);       break;
			case DisplayMenuVolGroupSPDIF : OledMenuVolGroup(&SetDisplay, 47);    TextMenuVolGroup(&SetDisplay, 47);       break;
			case DisplayMenuPresetEQ      : OledMenuPresetEq(&SetDisplay);        TextMenuPresetEq(&SetDisplay);           break;
			case DisplayMenuPresetCROSS   : OledMenuPresetCross(&SetDisplay);     TextMenuPresetCross(&SetDisplay);        break;
			case DisplayMenuBrightness    : OledMenuBrightness(&SetDisplay);      TextMenuBrightness(&SetDisplay);         break;
			case DisplayMenuVolControl    : OledMenuLoudness(&SetDisplay);        TextMenuLoudness(&SetDisplay);           break;
			case DisplayMenuTimeset       : OledMenuTimeset(&SetDisplay);         TextMenuTimeset(&SetDisplay);            break;
			case DisplayMenuEQ            : TextMenuEqSetup(&SetDisplay);   //     TextMenuLoudness(&SetDisplay);        break;

			}
		}
		TextTimerUpdate(&SetDisplay);
		OledTimerUpdate(&SetDisplay);
		SetDisplay.SetBusy = NO;
		SetDisplay.RefreshDisplay = NO;
	}
}
*/


/*
void EqSetupOnDisplay(tSetDisplay *SetDisplay){

	if(SetDisplay->Setup == ON){

		SetDisplay->MenuBteakTimer = 0;
		SetDisplay->MenuLevel = 1;
		SetDisplay->Setup = OFF;
		osDelay(500);
	}

	int8_t control;
	if(SetDisplay->SetActionCurrent == DisplayActionUp){
		control = SetDisplay->SetMenuCurrentLvl1;
		control++;
		if(control > PMenuEQCount-1) control = 0;
		SetDisplay->SetMenuCurrentLvl1 = control;
	}else if(SetDisplay->SetActionCurrent == DisplayActionDown){
		control = SetDisplay->SetMenuCurrentLvl1;
		control--;
		if(control < 0) control = PMenuEQCount - 1;
		SetDisplay->SetMenuCurrentLvl1 = control;
	}

}*/
































