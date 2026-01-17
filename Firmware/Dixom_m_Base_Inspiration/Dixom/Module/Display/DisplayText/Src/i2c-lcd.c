#include "stdint.h"
#include "i2c-lcd.h"
#include "Exchange.h"
#include <Structures.h>

extern sDixom Dixom;

uint8_t data_u, data_l;
//uint8_t data_t[4];

uint8_t LedState;
uint8_t TextLcdBackLight = 1;

void TextLCD_SetBrightness(uint8_t Value){
	if(Value>=1){
		TextLcdBackLight = 1;
	}else{
		TextLcdBackLight = 0;
	}
}

uint8_t data_t_cmd[4], byte_H, byte_L, byteT;

void lcd_send_cmd (uint8_t cmd)
{


	byte_H = (cmd&0xf0);
	byteT = cmd<<4;
	byte_L = ((cmd<<4)&0xf0);


	if(TextLcdBackLight == 0){
		data_t_cmd[0] = byte_H|0x0C;  //en=1, rs=0
		data_t_cmd[1] = byte_H|0x08;  //en=0, rs=0
		data_t_cmd[2] = byte_L|0x0C;  //en=1, rs=0
		data_t_cmd[3] = byte_L|0x08;  //en=0, rs=0
	}else{
		data_t_cmd[0] = byte_H|0x04;  //en=1, rs=0
		data_t_cmd[1] = byte_H|0x00;  //en=0, rs=0
		data_t_cmd[2] = byte_L|0x04;  //en=1, rs=0
		data_t_cmd[3] = byte_L|0x00;  //en=0, rs=0
	}
	ExchangeStatus(Transmit_I2C1(DEVICE_ADDR_LCD_HD44780, NOT_USED, NOT_USED, &data_t_cmd[0], 4, 150,  ExchangeMainSteamDMA),DISPLAY_TEXT_LCD_WRITE);
	Delay (1, FreeRTOS);

}

uint8_t data_t[4];

void lcd_send_data (char data){

	data_u = (data&0xf0);
	data_l = ((data<<4)&0xf0);

	if(TextLcdBackLight == 1){
		data_t[0] = data_u|0x0D;  //en=1, rs=0
		data_t[1] = data_u|0x09;  //en=0, rs=0
		data_t[2] = data_l|0x0D;  //en=1, rs=0
		data_t[3] = data_l|0x09;  //en=0, rs=0
	}else{
		data_t[0] = data_u|0x05;  //en=1, rs=0
		data_t[1] = data_u|0x01;  //en=0, rs=0
		data_t[2] = data_l|0x05;  //en=1, rs=0
		data_t[3] = data_l|0x01;  //en=0, rs=0
	}
	ExchangeStatus(Transmit_I2C1(DEVICE_ADDR_LCD_HD44780, NOT_USED, NOT_USED, (uint8_t *) data_t, 4, 150,  ExchangeMainSteamDMA), DISPLAY_TEXT_LCD_WRITE);
}


uint8_t data_t_cursor[4];
void Display_Text_Set_Cursor(uint8_t Line, uint8_t Indent){

	uint8_t cmdData = 0, byteH, byteL;


	if(Line == 1){
		cmdData = 0x80 + Indent ;
	}else if(Line == 2){
		cmdData = 0xC0 + Indent;
	}else if(Line == 3){
		cmdData = 0x94 + Indent;
	}else if(Line == 4){
		cmdData = 0xD4 + Indent;
	}

	byteH = cmdData&0xf0;
	byteL = ((cmdData<<4)&0xf0);
	if(Dixom.Module.Display.Brightness > 0){
		data_t_cursor[0] = byteH|0x0C;  //en=1, rs=0
		data_t_cursor[1] = byteH|0x08;  //en=0, rs=0
		data_t_cursor[2] = byteL|0x0C;  //en=1, rs=0
		data_t_cursor[3] = byteL|0x08;  //en=0, rs=0
	}else{
		data_t_cursor[0] = byteH|0x04;  //en=1, rs=0
		data_t_cursor[1] = byteH|0x00;  //en=0, rs=0
		data_t_cursor[2] = byteL|0x04;  //en=1, rs=0
		data_t_cursor[3] = byteL|0x00;  //en=0, rs=0
	}
	ExchangeStatus(	Transmit_I2C1( DEVICE_ADDR_LCD_HD44780, NOT_USED, NOT_USED, (uint8_t *) &data_t_cursor, 4, 300, ExchangeMainSteamDMA), DISPLAY_TEXT_LCD_WRITE);
}


uint8_t data_t_text[4];
void Transmit_Display_Text(uint8_t Line, uint8_t Indent, char *pData){


	uint8_t byte, byteH, byteL;

	uint8_t dataen = strlen(pData);


	if(Line !=0){
		Display_Text_Set_Cursor(Line, Indent);
	}

	//Delay (1, FreeRTOS);/
/*	for(uint8_t i = 0; i<dataen; i++){

		byteH = (*pData+i)&0xf0;
		byteL = ((*pData+i)<<4)&0xf0;


		if(GetSettings(SettingsDisplayBrightness) > 0){
			data_t_text[i*4] = byteH|0x0D;  //en=1, rs=0
			data_t_text[i*4+1] = byteH|0x09;  //en=0, rs=0
			data_t_text[i*4+2] = byteL|0x0D;  //en=1, rs=0
			data_t_text[i*4+3] = byteL|0x09;  //en=0, rs=0
		}else{
			data_t_text[i*4] = byteH|0x05;  //en=1, rs=0
			data_t_text[i*4+1] = byteH|0x01;  //en=0, rs=0
			data_t_text[i*4+2] = byteL|0x05;  //en=1, rs=0
			data_t_text[i*4+3] = byteL|0x01;  //en=0, rs=0
		}
	}

	ExchangeStatus(	Transmit_I2C1( DEVICE_ADDR_LCD_HD44780, NOT_USED, NOT_USED, (uint8_t *) &data_t_text[0], dataen*4, 300, ExchangeMainSteamDMADMA), DISPLAY_TEXT_LCD_WRITE);
*/
	while (*pData){

		byteH = *pData&0xf0;
		byteL = (*pData<<4)&0xf0;
        *pData++;

		if(Dixom.Module.Display.Brightness > 0){
			data_t_text[0] = byteH|0x0D;  //en=1, rs=0
			data_t_text[1] = byteH|0x09;  //en=0, rs=0
			data_t_text[2] = byteL|0x0D;  //en=1, rs=0
			data_t_text[3] = byteL|0x09;  //en=0, rs=0
		}else{
			data_t_text[0] = byteH|0x05;  //en=1, rs=0
			data_t_text[1] = byteH|0x01;  //en=0, rs=0
			data_t_text[2] = byteL|0x05;  //en=1, rs=0
			data_t_text[3] = byteL|0x01;  //en=0, rs=0
		}


		ExchangeStatus(	Transmit_I2C1( DEVICE_ADDR_LCD_HD44780, NOT_USED, NOT_USED, (uint8_t *) &data_t_text[0], 4, 300, ExchangeMainSteamDMA), DISPLAY_TEXT_LCD_WRITE);

	}
}


void lcd_init (void)
{
	lcd_send_cmd (0x01);
	lcd_send_cmd (0x02);
	lcd_send_cmd (0x0C);
	lcd_send_cmd (0x3C);
	lcd_send_cmd (0xE0);
}

void lcd_send_string (char *str)
{
	while (*str) lcd_send_data (*str++);
}
