#include <Description.h>
#include "DisplayMain.h"
#include "OledDisplay.h"
#include "TextDisplay.h"
#include "Nextion_LCD.h"
#include "ssd1306.h"
#include "Exchange.h"
#include "i2c-lcd.h"
//#include "StructSettings.h"
#include <string.h>
#include <stdio.h>
#include <Structures.h>

extern sDixom Dixom;

void DISPLAY(uint8_t *Recive_USB){

	 if(strncmp((char *)Recive_USB,      "BRIGHTNESS ",              11) == 0)      {  SET_Brightness(Recive_USB+11);            } //
	 else if(strncmp((char *)Recive_USB, "INIT",                     4) == 0)       {                                            } //
	 else{
		 Transmit_Data_Out((uint8_t *)GeneralNotifi_CommandError, AutoLenght, NO, FreeRTOS, NO);
	 }
}

void SET_Brightness(uint8_t *Recive_USB){

	short Brightness;
	sscanf((char *)Recive_USB, "%hu", &Brightness);

	if( Brightness<0 || Brightness>MaxBrightness){
		Transmit_Data_Out((uint8_t *)GeneralNotifi_CommandError, AutoLenght, NO, FreeRTOS, NO);
	}else{
		SetBrightness(Brightness);
	}
}

void SetDisplayBrightness(uint8_t direction){
	uint8_t data;
	data = Dixom.Module.Display.Brightness;

	if(direction==0 || direction==1){
		if(direction==1){
			data++;
			if(data > MaxBrightness) data = MaxBrightness;
		}else if(direction==0){
			data--;
			if(data > MaxBrightness) data = 0;
		}

		SetBrightness(data);
	}
}

void SetBrightness(uint8_t value){

	//SetSettings(SettingsDisplayBrightness, value);
	OLED_SetBrightness(value);
	TextLCD_SetBrightness(value);

}

void Display_Loading_Complete(void){
	TextDisplayDoneInit();                  // ↓ 1  Отображаем выбранное меню после загрузки платформы
	OledDisplayDoneInit();                  // ↓ 2  Отображаем выбранное меню после загрузки платформы
	NextionDisplayDoneInit();               // ↓ 3  Отображаем выбранное меню после загрузки платформы
}






