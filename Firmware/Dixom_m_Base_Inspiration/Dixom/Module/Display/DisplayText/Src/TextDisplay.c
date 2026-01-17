#include <stdio.h>
#include "i2c-lcd.h"
#include "TextDisplay.h"
#include "DSP_InternalCalls.h"
#include "main_Action.h"
#include "Exchange.h"
#include <stdio.h>
#include <math.h>
#include <RDA_5807.h>
#include <Structures.h>

extern sDixom Dixom;

//short TextDisplayStatus = OFF;
short TextDisplayType = 0;

typedef struct
{
	uint8_t  RS           :1;    // Master Power-Up Control.
	uint8_t  RW           :1;    // Master Clock Select
	uint8_t  E            :1;    // Master Clock Select
	uint8_t  data         :1;    // PLL Input Select.
} tLCD;


void fillBar2(uint8_t start_pos, uint8_t row, uint8_t bar_length, uint8_t fill_percent);

typedef enum{
	right_empty = 1,
	left_empty,
	center_empty,
	left_full,
	right_full,
	center_full
} progres_bar;
uint8_t view[7] = {1, 2, 3, 4, 5, 6};

uint8_t right_empty2 = 1,
left_empty2= 2,
center_empty2= 3,
left_full2 = 4,
right_full2 = 5,
center_full2 = 6;

void Automatic_Detection_Text_Display_And_Initialization(void){

	if (DevState_I2C1(DEVICE_ADDR_LCD_HD44780, 1, 100) == WR_OK) {
		Dixom.Module.Display.StateTextDisplay = ON;

		lcd_send_cmd (0x28);  //4-4-6. FUNCTION SET  modes to 4-bit (DB7-DB4)
		lcd_send_cmd (0x02);  //4-4-2. CURSOR HOME
		lcd_send_cmd (0x0C);  //4-4-4. DISPLAY ON/OFF CONTROL
		lcd_send_cmd (0x28);  //4-4-6. FUNCTION SET  Sets the luminance level to 100%
		lcd_send_cmd (0x01);  //4-4-1. CLEAR DISPLAY

		uint8_t right_empty[8] =  {0b11111, 0b00001, 0b00001, 0b00001, 0b00001, 0b00001, 0b00001, 0b11111};
		uint8_t left_empty[8] =   {0b11111, 0b10000, 0b10000, 0b10000, 0b10000, 0b10000, 0b10000, 0b11111};
		uint8_t center_empty[8] = {0b11111, 0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b11111};
		uint8_t left_full[8] =    {0b11111, 0b10000, 0b10111, 0b10111, 0b10111, 0b10111, 0b10000, 0b11111};
		uint8_t right_full[8] =   {0b11111, 0b00001, 0b11101, 0b11101, 0b11101, 0b11101, 0b00001, 0b11111};
		uint8_t center_full[8] =  {0b11111, 0b00000, 0b11111, 0b11111, 0b11111, 0b11111, 0b00000, 0b11111};

		Display_Text_Character_Creator (1, &left_empty[0]);
		Display_Text_Character_Creator (2, &center_empty[0]);
		Display_Text_Character_Creator (3, &right_empty[0]);
		Display_Text_Character_Creator (4, &left_full[0]);
		Display_Text_Character_Creator (5, &center_full[0]);
		Display_Text_Character_Creator (6, &right_full[0]);

		Transmit_Display_Text (2, 0, "   LOADING...   ");

	}else{
		Dixom.Module.Display.StateTextDisplay = OFF;
	}
}



void Display_Text_ProgressBarr(uint8_t start_pos, uint8_t row, uint8_t bar_length, uint8_t fill_max, uint8_t fill_percent) {
  uint8_t infill = round((float)bar_length * fill_percent / fill_max);

  Display_Text_Set_Cursor(row, start_pos);
  if (infill == 0) ProgressBarr_Display_Text(1);
  else ProgressBarr_Display_Text(4);
  for (int n = 1; n < bar_length - 1; n++) {
    if (n < infill) ProgressBarr_Display_Text(5);
    if (n >= infill) ProgressBarr_Display_Text(2);
  }
  if (infill == bar_length) ProgressBarr_Display_Text(6);
  else ProgressBarr_Display_Text(3);
}


void ProgressBarr_Display_Text(uint8_t pData){


	uint8_t byteH, byteL;
	uint8_t data_t[4];

	byteH = pData&0xf0;
	byteL = (pData<<4)&0xf0;


	if(Dixom.Module.Display.Brightness > 0){
		data_t[0] = byteH|0x0D;  //en=1, rs=0
		data_t[1] = byteH|0x09;  //en=0, rs=0
		data_t[2] = byteL|0x0D;  //en=1, rs=0
		data_t[3] = byteL|0x09;  //en=0, rs=0
	}else{
		data_t[0] = byteH|0x05;  //en=1, rs=0
		data_t[1] = byteH|0x01;  //en=0, rs=0
		data_t[2] = byteL|0x05;  //en=1, rs=0
		data_t[3] = byteL|0x01;  //en=0, rs=0
	}
	ExchangeStatus(	Transmit_I2C1( DEVICE_ADDR_LCD_HD44780, NOT_USED, NOT_USED, (uint8_t *) &data_t[0], 4, 300, ExchangeMainSteamDMA), DISPLAY_TEXT_LCD_WRITE);
}

void Display_Text_Character_Creator(uint8_t addr, uint8_t *pData){

	uint8_t byte, byteH, byteL;
	uint8_t data_t[4];

	lcd_send_cmd (64+(addr*8));  //4-4-7 set addr CGRAM=8

	for (uint8_t i=0; i<8; i++){
		byte = *pData;

		byteH = byte&0xf0;
		byteL = (byte<<4)&0xf0;


		if(Dixom.Module.Display.Brightness > 0){
			data_t[0] = byteH|0x0D;  //en=1, rs=0
			data_t[1] = byteH|0x09;  //en=0, rs=0
			data_t[2] = byteL|0x0D;  //en=1, rs=0
			data_t[3] = byteL|0x09;  //en=0, rs=0
		}else{
			data_t[0] = byteH|0x05;  //en=1, rs=0
			data_t[1] = byteH|0x01;  //en=0, rs=0
			data_t[2] = byteL|0x05;  //en=1, rs=0
			data_t[3] = byteL|0x01;  //en=0, rs=0
		}

		ExchangeStatus(	Transmit_I2C1( DEVICE_ADDR_LCD_HD44780, NOT_USED, NOT_USED, (uint8_t *) &data_t[0], 4, 300, ExchangeMainSteamDMA), DISPLAY_TEXT_LCD_WRITE);
		pData++;
	}
}



void TextDisplayDoneInit(void){

	if (DevState_I2C1(DEVICE_ADDR_LCD_HD44780, 1, 100) == WR_OK) {
		TextDisplayType = GetSettings(SettingsTypeTextLCD);

		if(TextDisplayType==0) TextDisplayMenu();   // меню первого типа
	}
}

char TextDisplaySend[40];



# define macross_BackSelect ">Back   <--    <"
# define macross_BackNotSel " Back   <--    <"
# define macross_Selection  ">"
# define macross_NotSelect  " "
# define macross_ViewError  "%s%d C%hu E%lu B%lu T%lu  "



void TextMenuLevel1(sDisplayMenu *DisplayMenu){

	switch (DisplayMenu->MenuLevel_1) {
	case 1:
		Transmit_Display_Text(1, 0, macross_BackSelect);
		Transmit_Display_Text(2, 0, " Fast settigns >");
		break;
	case 2:
		Transmit_Display_Text(1, 0, ">Fast settigns >");
		Transmit_Display_Text(2, 0, " Sound setup   >");
		break;
	case 3:
		Transmit_Display_Text(1, 0, ">Sound setup   >");
		Transmit_Display_Text(2, 0, " Platform setup>");
		break;
	case 4:
		Transmit_Display_Text(1, 0, ">Platform setup>");
		Transmit_Display_Text(2, 0, " Reinit        >");
		break;
	case 5:
		Transmit_Display_Text(1, 0, ">Reinit        >");
		Transmit_Display_Text(2, 0, " Error View    >");
		break;
	case 6:
		Transmit_Display_Text(1, 0, ">Error View    >");
		Transmit_Display_Text(2, 0, macross_BackNotSel);
		break;
	}

}

void TextMenuLevel2_1(sDisplayMenu *DisplayMenu){

	if(Dixom.Module.Display.StateTextDisplay == ON){
		switch (DisplayMenu->MenuLevel_2){
		case 1 :
			lcd_send_cmd (0x80);
			lcd_send_string (macross_BackSelect);
			lcd_send_cmd (0xC0);
			lcd_send_string (" Source        >");
			break;
		case 2 :
			lcd_send_cmd (0x80);
			lcd_send_string (">Source        >");
			lcd_send_cmd (0xC0);
			lcd_send_string (" Group vol     >");
			break;
		case 3 :
			lcd_send_cmd (0x80);
			lcd_send_string (">Group vol     >");
			lcd_send_cmd (0xC0);
			lcd_send_string (" Lcd Brightness>");
		break;               break;
		case 4 :
			lcd_send_cmd (0x80);
			lcd_send_string (">Lcd Brightness>");
			lcd_send_cmd (0xC0);
			lcd_send_string (macross_BackNotSel);
			break;
		}
	}
}

void TextSourceSetup(sDisplayMenu *DisplayMenu) {

	if (DisplayMenu->MenuStatus != Status_Setup) {
		lcd_send_cmd(0x80);
		lcd_send_string(">Source        >");
		lcd_send_cmd(0xC0);
		lcd_send_string(" Group vol     >");
	} else {

		lcd_send_cmd(0x80);
		lcd_send_string(">Source setup  <");
		lcd_send_cmd(0xC0);
		switch (Dixom.Module.DSP.Settings[SoundSource].Data) {

		case SoursUsb:
			lcd_send_string("   USB AUDIO    ");
			break;
		case SoursBluetoooth:
			lcd_send_string("   BLUETOOTH    ");
			break;
		case SoursAux:
			lcd_send_string("   AUX INPUT    ");
			break;
		case SoursRadio:
			lcd_send_string("     RADIO      ");
			break;
		case SoursSpdif:
			lcd_send_string("   SPDIF INPUT  ");
			break;
		case SoursMic:
			lcd_send_string("   MICROPHONE   ");
			break;
		}
	}
}



void TextBrightness(sDisplayMenu *DisplayMenu){

	if (DisplayMenu->MenuStatus != Status_Setup) {
		lcd_send_cmd (0x80);
		lcd_send_string (">Lcd Brightness>");
		lcd_send_cmd (0xC0);
		lcd_send_string (macross_BackNotSel);
	} else {

		lcd_send_cmd(0x80);
		lcd_send_string(">Lcd Brightness>");
		lcd_send_cmd(0xC0);

		if(Dixom.Module.Display.Brightness==0){
			sprintf(TextDisplaySend, "        %s       ", StrMIN);
		}else if(Dixom.Module.Display.Brightness==16){
			sprintf(TextDisplaySend, "        %s       ", StrMAX);
		}else{
			sprintf(TextDisplaySend, "        %d       ", Dixom.Module.Display.Brightness);
		}

		lcd_send_string(TextDisplaySend);
	}
	/*
	if(SetDisplay->Setup == ON){
		SSD1306_GotoXY(90,50);
		SSD1306_Puts("SETUP", &Font_7x10, 1);
	}

	SSD1306_GotoXY(0,0);
	SSD1306_Puts("BRIGHTNESS", &Font_11x18, 1);

	if(GetSettings(SettingsDisplayBrightness)==0){
		sprintf(DisplaySend, "%s", StrMIN);
	}else if(GetSettings(SettingsDisplayBrightness)==16){
		sprintf(DisplaySend, "%s", StrMAX);
	}else{
		sprintf(DisplaySend, "%d", GetSettings(SettingsDisplayBrightness));
	}

	SSD1306_GotoXY(45,26);
	SSD1306_Puts(DisplaySend, &Font_16x26, 1);
	SSD1306_UpdateScreen();
	SSD1306_Fill(0);*/
}

#define  DetailLog Dixom.Exchange.Logs.Detail

void TextMenuViewError(sDisplayMenu *DisplayMenu){
	if(Dixom.Module.Display.StateTextDisplay == ON){
		switch (DisplayMenu->MenuLevel_2){
		case 1 :
			Transmit_Display_Text(1, 0, macross_BackSelect);
			sprintf(TextDisplaySend, macross_ViewError ,macross_NotSelect, 1, DetailLog.Interface[0].Code, DetailLog.Interface[0].Count_WR_ERROR, DetailLog.Interface[0].Count_WR_BUSY, DetailLog.Interface[0].Count_WR_TIMEOUT);
			Transmit_Display_Text(2, 0, TextDisplaySend);
			break;
		case 2 :
			sprintf(TextDisplaySend, macross_ViewError, macross_Selection, 1, DetailLog.Interface[0].Code, DetailLog.Interface[0].Count_WR_ERROR, DetailLog.Interface[0].Count_WR_BUSY, DetailLog.Interface[0].Count_WR_TIMEOUT);
			Transmit_Display_Text(1, 0, TextDisplaySend);
			sprintf(TextDisplaySend, macross_ViewError, macross_NotSelect, 2, DetailLog.Interface[1].Code, DetailLog.Interface[1].Count_WR_ERROR, DetailLog.Interface[1].Count_WR_BUSY, DetailLog.Interface[1].Count_WR_TIMEOUT);
			Transmit_Display_Text(2, 0, TextDisplaySend);
			break;
		case 3 :
			sprintf(TextDisplaySend, macross_ViewError, macross_Selection, 2, DetailLog.Interface[1].Code, DetailLog.Interface[1].Count_WR_ERROR, DetailLog.Interface[1].Count_WR_BUSY, DetailLog.Interface[1].Count_WR_TIMEOUT);
			Transmit_Display_Text(1, 0, TextDisplaySend);
			sprintf(TextDisplaySend, macross_ViewError, macross_NotSelect, 3, DetailLog.Interface[2].Code, DetailLog.Interface[2].Count_WR_ERROR, DetailLog.Interface[2].Count_WR_BUSY, DetailLog.Interface[2].Count_WR_TIMEOUT);
			Transmit_Display_Text(2, 0, TextDisplaySend);
			break;
		case 4 :
			sprintf(TextDisplaySend, macross_ViewError, macross_Selection, 3, DetailLog.Interface[2].Code, DetailLog.Interface[2].Count_WR_ERROR, DetailLog.Interface[2].Count_WR_BUSY, DetailLog.Interface[2].Count_WR_TIMEOUT);
			Transmit_Display_Text(1, 0, TextDisplaySend);
			sprintf(TextDisplaySend, macross_ViewError, macross_NotSelect, 4, DetailLog.Interface[3].Code, DetailLog.Interface[3].Count_WR_ERROR, DetailLog.Interface[3].Count_WR_BUSY, DetailLog.Interface[3].Count_WR_TIMEOUT);
			Transmit_Display_Text(2, 0, TextDisplaySend);
			break;
		case 5 :
			sprintf(TextDisplaySend, macross_ViewError, macross_Selection, 4, DetailLog.Interface[3].Code, DetailLog.Interface[3].Count_WR_ERROR, DetailLog.Interface[3].Count_WR_BUSY, DetailLog.Interface[3].Count_WR_TIMEOUT);
			Transmit_Display_Text(1, 0, TextDisplaySend);
			sprintf(TextDisplaySend, macross_ViewError, macross_NotSelect, 5, DetailLog.Interface[4].Code, DetailLog.Interface[4].Count_WR_ERROR, DetailLog.Interface[4].Count_WR_BUSY, DetailLog.Interface[4].Count_WR_TIMEOUT);
			Transmit_Display_Text(2, 0, TextDisplaySend);
			break;
		case 6 :
			sprintf(TextDisplaySend, macross_ViewError, macross_Selection, 5, DetailLog.Interface[4].Code, DetailLog.Interface[4].Count_WR_ERROR, DetailLog.Interface[4].Count_WR_BUSY, DetailLog.Interface[4].Count_WR_TIMEOUT);
			Transmit_Display_Text(1, 0, TextDisplaySend);
			sprintf(TextDisplaySend, macross_ViewError, macross_NotSelect, 6, DetailLog.Interface[5].Code, DetailLog.Interface[5].Count_WR_ERROR, DetailLog.Interface[5].Count_WR_BUSY, DetailLog.Interface[5].Count_WR_TIMEOUT);
			Transmit_Display_Text(2, 0, TextDisplaySend);
			break;
		case 7 :
			sprintf(TextDisplaySend, macross_ViewError, macross_Selection, 6, DetailLog.Interface[5].Code, DetailLog.Interface[5].Count_WR_ERROR, DetailLog.Interface[5].Count_WR_BUSY, DetailLog.Interface[5].Count_WR_TIMEOUT);
			Transmit_Display_Text(1, 0, TextDisplaySend);
			sprintf(TextDisplaySend, macross_ViewError, macross_NotSelect, 7, DetailLog.Interface[6].Code, DetailLog.Interface[6].Count_WR_ERROR, DetailLog.Interface[6].Count_WR_BUSY, DetailLog.Interface[6].Count_WR_TIMEOUT);
			Transmit_Display_Text(2, 0, TextDisplaySend);
			break;
		case 8 :
			sprintf(TextDisplaySend, macross_ViewError, macross_Selection, 7, DetailLog.Interface[6].Code, DetailLog.Interface[6].Count_WR_ERROR, DetailLog.Interface[6].Count_WR_BUSY, DetailLog.Interface[6].Count_WR_TIMEOUT);
			Transmit_Display_Text(1, 0, TextDisplaySend);
			Transmit_Display_Text(2, 0, macross_BackNotSel);
			break;
		}
	}

}


void TextMenuError(sDisplayMenu *DisplayMenu){

	if(Dixom.Module.Display.StateTextDisplay == ON){
		switch (DisplayMenu->MenuLevel_2){
		case 1 :
			lcd_send_cmd (0x80);        lcd_send_string (macross_BackSelect);
			lcd_send_cmd (0xC0);        lcd_send_string (" REINIT DAC   OK");
			break;
		case 2 :
			lcd_send_cmd (0x80);        lcd_send_string (">REINIT DAC   OK");
			lcd_send_cmd (0xC0);        lcd_send_string (" REINIT DSP   OK");
			break;
		case 3 :
			lcd_send_cmd (0x80);        lcd_send_string (">REINIT DSP   OK");
			lcd_send_cmd (0xC0);        lcd_send_string (" REINIT SOUND OK");
			break;
		case 4 :
			lcd_send_cmd (0x80);        lcd_send_string (">REINIT SOUND OK");
			lcd_send_cmd (0xC0);        lcd_send_string (macross_BackNotSel);
			break;
		}
	}
}



void TextGroupVol(sDisplayMenu *DisplayMenu){
	/*
	if(DisplayMenu->MenuStatus == Status_Setup){
		SSD1306_GotoXY(0,20);
		sprintf(TextDisplaySend, ">%s", DSP_GetGroupName(0));
		SSD1306_Puts(TextDisplaySend, &Font_11x18, 1);
		SSD1306_GotoXY(0,40);
		sprintf(TextDisplaySend, "      %hhu ", DSP_GetVolume(29));
		SSD1306_Puts(TextDisplaySend, &Font_11x18, 1);
	}else{
		SSD1306_GotoXY(0,20);
		sprintf(TextDisplaySend, ">%s", DSP_GetGroupName(0));
		SSD1306_Puts(TextDisplaySend, &Font_11x18, 1);
		SSD1306_GotoXY(0,40);
		sprintf(TextDisplaySend, " %s", DSP_GetGroupName(1));
		SSD1306_Puts(TextDisplaySend, &Font_11x18, 1);
	}*/

	if(Dixom.Module.Display.StateTextDisplay == ON){
		switch (DisplayMenu->MenuLevel_3){
		case 1 :
			lcd_send_cmd (0x80);        lcd_send_string (macross_BackSelect);
			lcd_send_cmd (0xC0);        sprintf(TextDisplaySend, " %s >", DSP_GetGroupName(0));   	lcd_send_string (TextDisplaySend);
			break;
		case 2 :
			lcd_send_cmd (0x80);
			sprintf(TextDisplaySend, ">%s >", DSP_GetGroupName(0));   	lcd_send_string (TextDisplaySend);
			lcd_send_cmd (0xC0);

			if(DisplayMenu->MenuStatus == Status_Setup){
				sprintf(TextDisplaySend, "      %hhu           ", DSP_GetVolume(29));
				lcd_send_string (TextDisplaySend);
			}else{
				lcd_send_cmd (0xC0);		sprintf(TextDisplaySend, " %s >", DSP_GetGroupName(1));   	lcd_send_string (TextDisplaySend);
			}
			break;
		case 3 :
			lcd_send_cmd (0x80);
			sprintf(TextDisplaySend, ">%s >", DSP_GetGroupName(1));
			lcd_send_string (TextDisplaySend);
			lcd_send_cmd (0xC0);

			if(DisplayMenu->MenuStatus == Status_Setup){
				sprintf(TextDisplaySend, "      %hhu           ", DSP_GetVolume(32));
				lcd_send_string (TextDisplaySend);
			}else{
				lcd_send_cmd (0xC0);		sprintf(TextDisplaySend, " %s >", DSP_GetGroupName(2));   	lcd_send_string (TextDisplaySend);
			}
			break;
		case 4 :
			lcd_send_cmd (0x80);
			sprintf(TextDisplaySend, ">%s >", DSP_GetGroupName(2));   	lcd_send_string (TextDisplaySend);
			lcd_send_cmd (0xC0);

			if(DisplayMenu->MenuStatus == Status_Setup){
				sprintf(TextDisplaySend, "      %hhu           ", DSP_GetVolume(35));
				lcd_send_string (TextDisplaySend);
			}else{
				lcd_send_cmd (0xC0);		sprintf(TextDisplaySend, " %s >", DSP_GetGroupName(3));   	lcd_send_string (TextDisplaySend);
			}
			break;
		case 5 :
			lcd_send_cmd (0x80);
			sprintf(TextDisplaySend, ">%s >", DSP_GetGroupName(3));   	lcd_send_string (TextDisplaySend);
			lcd_send_cmd (0xC0);

			if(DisplayMenu->MenuStatus == Status_Setup){
				sprintf(TextDisplaySend, "      %hhu           ", DSP_GetVolume(38));
				lcd_send_string (TextDisplaySend);
			}else{
				lcd_send_cmd (0xC0);		sprintf(TextDisplaySend, " %s >", DSP_GetGroupName(4));   	lcd_send_string (TextDisplaySend);
			}
			break;
		case 6 :
			lcd_send_cmd (0x80);
			sprintf(TextDisplaySend, ">%s >", DSP_GetGroupName(4));   	lcd_send_string (TextDisplaySend);
			lcd_send_cmd (0xC0);

			if(DisplayMenu->MenuStatus == Status_Setup){
				sprintf(TextDisplaySend, "      %hhu           ", DSP_GetVolume(41));
				lcd_send_string (TextDisplaySend);
			}else{
				lcd_send_cmd (0xC0);		sprintf(TextDisplaySend, " %s >", DSP_GetGroupName(5));   	lcd_send_string (TextDisplaySend);
			}
			break;
		case 7 :
			lcd_send_cmd (0x80);
			sprintf(TextDisplaySend, ">%s >", DSP_GetGroupName(5));   	lcd_send_string (TextDisplaySend);
			lcd_send_cmd (0xC0);

			if(DisplayMenu->MenuStatus == Status_Setup){
				sprintf(TextDisplaySend, "      %hhu           ", DSP_GetVolume(44));
				lcd_send_string (TextDisplaySend);
			}else{
				lcd_send_cmd (0xC0);		sprintf(TextDisplaySend, " %s >", DSP_GetGroupName(6));   	lcd_send_string (TextDisplaySend);
			}
			break;
		case 8 :
			lcd_send_cmd (0x80);
			sprintf(TextDisplaySend, ">%s >", DSP_GetGroupName(6));   	lcd_send_string (TextDisplaySend);
			lcd_send_cmd (0xC0);

			if(DisplayMenu->MenuStatus == Status_Setup){
				sprintf(TextDisplaySend, "      %hhu           ", DSP_GetVolume(47));
				lcd_send_string (TextDisplaySend);
			}else{
				lcd_send_cmd (0xC0);		lcd_send_string (macross_BackNotSel);
			}
			break;
		}
	}
}
/*
short DTchannel = 0;
short DTvalue = 0;
short DTpressed = 0;
short DTaction = 0;

//void setTexLcdRES(short channel, short value, short pressed){
	DTchannel = channel;
	DTvalue = value;
	DTpressed = pressed;
}

void SetTextLcdData(short action){
	DTaction = action;
}*/


uint32_t smpling =0;
uint8_t  bitRare;
void TextMenuMain(sDisplayMenu *DisplayMenu){

	if(Dixom.Module.Display.StateTextDisplay == ON){

		if(Dixom.Module.UsbAudio.State.BitRate == BIT_16){
			bitRare = 16;
		}else if(Dixom.Module.UsbAudio.State.BitRate == BIT_24){
			bitRare = 24;
		}

		if(Dixom.Module.UsbAudio.State.SamplingFreq == SFreq_48000){
			smpling = 48000;
		}else if(Dixom.Module.UsbAudio.State.SamplingFreq == SFreq_96000){
			smpling = 96000;
		}


		switch (Dixom.Module.DSP.Settings[SoundSource].Data){
		case SoursUsb        :
			if(Dixom.Module.UsbAudio.State.Playback == YES){
				Transmit_Display_Text(1, 0, " USB AUDIO  PLAYING ");
			}else{
				Transmit_Display_Text(1, 0, " USB AUDIO   PAUSE  ");
			}
		break;

		case SoursBluetoooth : 		Transmit_Display_Text(1, 0, " BLUETOOTH      ");                break;
		case SoursAux        : 		Transmit_Display_Text(1, 0, " AUX INPUT      ");                break;
		case SoursRadio      :
		sprintf(TextDisplaySend,                     " RADIO %.2f     ", Dixom.Module.FmRadio.FmPreset[0].Freq * FConv);
		Transmit_Display_Text(1, 0, TextDisplaySend);
		break;
		case SoursSpdif      :		Transmit_Display_Text(1, 0, "  SPDIF INPUT   ");                break;
		case SoursMic        :		Transmit_Display_Text(1, 0, "  MICROPHONE    ");                break;
		}


	//	sprintf( TextDisplaySend,"W%hu E%lu B%lu T%lu     ", Dixom.ExchangeState.WarningCount, Dixom.ExchangeState.TotalErrorCount, Dixom.ExchangeState.TotalBusyCount, Dixom.ExchangeState.TotalTimeoutCount);
	//	Transmit_Display_Text(1, 18, TextDisplaySend);

	//	sprintf( TextDisplaySend,"CH%hu VE%hu AN%hu     ", DTchannel, DTvalue, DTaction);
		//Transmit_Display_Text(2, 18, TextDisplaySend);





		//sprintf( TextDisplaySend," %hu-Bit / %lu-Hz",bitRare, smpling);
		//Transmit_Display_Text(4, 0, TextDisplaySend);

	//	Transmit_Display_Text(3, 0, " Privet  ");
	//	Transmit_Display_Text(4, 0, " Ghbdtn  ");

		/*if(Dixom.Module.UsbAudio.StatePlayback == YES){
			Transmit_Display_Text(4, 10, "PLAYING  ");
		}else{
			Transmit_Display_Text(4, 10, "PAUSE  ");
		}*/


		if(DSP_GetVolume (DacVolum)==0){
			sprintf(TextDisplaySend, " Volume %s %hu/%lu", StrMIN, bitRare, smpling);
		}else if(DSP_GetVolume (DacVolum)==60){
			sprintf(TextDisplaySend, " Volume %s %hu/%lu", StrMAX, bitRare, smpling);
		}else{
			sprintf(TextDisplaySend, " Volume %hu %hu/%lu ", DSP_GetVolume (DacVolum), bitRare, smpling);
		}
		Transmit_Display_Text(2, 0, TextDisplaySend);


		Display_Text_ProgressBarr(0, 3, 20, 60,  DSP_GetVolume (DacVolum));
		Display_Text_ProgressBarr(0, 4, 20, 255,  Dixom.Module.DSP.LvlDetec[0].data);
	}

}

void TextEventVolume(tSetDisplay *SetDisplay){

}

void TextDisplayClear(){

}

void TextMenuSource(tSetDisplay *SetDisplay){

}

void TextTimerUpdate(tSetDisplay *SetDisplay){

}

void TextMenuTimeset(tSetDisplay *SetDisplay){

}



void TextMenuVolGroup(tSetDisplay *SetDisplay, short VolNum){

}

void TextMenuPresetEq(tSetDisplay *SetDisplay){

}

void TextMenuPresetCross(tSetDisplay *SetDisplay){

}

void TextMenuBrightness(tSetDisplay *SetDisplay){

}

void TextMenuLoudness(tSetDisplay *SetDisplay){

}

void TIM6_mSecondMenu(){


}
void TextDisplayMenu(){

}

void LcdGeneralWhile_Text(){

}


void Menu1set(void){

}

void Menu1up(void){

}

void Menu1down(void){

}

void Menu1Function(void){


}

void Menu1FunctionAction(void){

}


void TextMenuEqSetup(tSetDisplay *SetDisplay){

}

void TextDisplayPowerDown(sDisplayMenu *DisplayMenu){

	lcd_send_cmd (0x80);
	lcd_send_string ("  GOODBUY...                            ");
	lcd_send_cmd (0xC0);
	lcd_send_string ("                                        ");
}



void TextDisplayEvent(sDisplayMenu *DisplayMenu) {

	if (Dixom.Module.Display.StateTextDisplay == ON) {
		if (DisplayMenu->Event.type == EventBlink) {
			lcd_send_string("STATIK                  ");

		} else if (DisplayMenu->Event.type == EventStatik) {

			if (DisplayMenu->Event.whence == ChangeSource) {
				lcd_send_cmd(0x80);

				switch (Dixom.Module.DSP.Settings[SoundSource].Data) {
				case SoursUsb:
					lcd_send_string("SOURCE USB         ");
					break;
				case SoursBluetoooth:
					lcd_send_string("SOURCE BLUETOOTH   ");
					break;
				case SoursAux:
					lcd_send_string("SOURCE AUX         ");
					break;
				case SoursRadio:
					lcd_send_string("SOURCE RADIO       ");
					break;
				case SoursSpdif:
					lcd_send_string("SOURCE SPDIF       ");
					break;
				case SoursMic:
					lcd_send_string("SOURCE MIC         ");
					break;
				}
			}
			if (DisplayMenu->Event.whence == ChangeVolume) {

				lcd_send_cmd(0x80);
				if (DisplayMenu->Param.Num[0] == VolGroup1) {
					sprintf(TextDisplaySend, "%s               ",
							DSP_GetGroupName(0));
					lcd_send_string(TextDisplaySend);
				} else if (DisplayMenu->Param.Num[0] == VolGroup2) {
					sprintf(TextDisplaySend, "%s               ",
							DSP_GetGroupName(1));
					lcd_send_string(TextDisplaySend);
				} else if (DisplayMenu->Param.Num[0] == VolGroup3) {
					sprintf(TextDisplaySend, "%s               ",
							DSP_GetGroupName(2));
					lcd_send_string(TextDisplaySend);
				} else if (DisplayMenu->Param.Num[0] == VolGroup4) {
					sprintf(TextDisplaySend, "%s               ",
							DSP_GetGroupName(3));
					lcd_send_string(TextDisplaySend);
				} else if (DisplayMenu->Param.Num[0] == VolGroup5) {
					sprintf(TextDisplaySend, "%s               ",
							DSP_GetGroupName(4));
					lcd_send_string(TextDisplaySend);
				} else if (DisplayMenu->Param.Num[0] == VolGroup6) {
					sprintf(TextDisplaySend, "%s               ",
							DSP_GetGroupName(5));
					lcd_send_string(TextDisplaySend);
				} else if (DisplayMenu->Param.Num[0] == VolGroup7) {
					sprintf(TextDisplaySend, "%s               ",
							DSP_GetGroupName(6));
					lcd_send_string(TextDisplaySend);
				} else if (DisplayMenu->Param.Num[0] == VolSoursUsb) {
					lcd_send_string("VOLUME USB                ");
				} else if (DisplayMenu->Param.Num[0] == VolSoursBluetoooth) {
					lcd_send_string("VOLUME BT                 ");
				} else if (DisplayMenu->Param.Num[0] == VolSoursAux) {
					lcd_send_string("VOLUME AUX                ");
				} else if (DisplayMenu->Param.Num[0] == VolSoursRadio) {
					lcd_send_string("VOLUME RADIO              ");
				} else if (DisplayMenu->Param.Num[0] == VolSoursSpdif) {
					lcd_send_string("VOLUME SPDIF              ");
				} else if (DisplayMenu->Param.Num[0] == VolSoursMic) {
					lcd_send_string("VOLUME MIC                ");
				} else if (DisplayMenu->Param.Num[0] == MinVolUsb) {
					lcd_send_string("MIN VOL USB               ");
				} else if (DisplayMenu->Param.Num[0] == MinVolBluetoooth) {
					lcd_send_string("MIN VOL BT                ");
				} else if (DisplayMenu->Param.Num[0] == MinVolAux) {
					lcd_send_string("MIN VOL AUX               ");
				} else if (DisplayMenu->Param.Num[0] == MinVolsRadio) {
					lcd_send_string("MIN VOL FM                ");
				} else if (DisplayMenu->Param.Num[0] == MinVolSpdif) {
					lcd_send_string("MIN VOL SPDIF             ");
				} else if (DisplayMenu->Param.Num[0] == MinVolMic) {
					lcd_send_string("MIN VOL MIC               ");
				} else {
					sprintf(TextDisplaySend, "VOLUME %d       ",
							DisplayMenu->Param.Num[0]);
					lcd_send_string(TextDisplaySend);
				}

				sprintf(TextDisplaySend, "Volume %hu      ",
						DisplayMenu->Param.Num[1]);

				lcd_send_cmd(0xC0);
				lcd_send_string(TextDisplaySend);

			}

		} else {

		}
	}

}
