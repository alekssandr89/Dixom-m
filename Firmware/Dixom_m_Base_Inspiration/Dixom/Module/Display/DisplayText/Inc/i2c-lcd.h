#ifndef __I2C_LCD_H__
#define __I2C_LCD__

#include "stdint.h"


void lcd_init (void);   // initialize lcd

void lcd_send_cmd (uint8_t cmd);  // send command to the lcd

void lcd_send_data (char data);  // send data to the lcd

void lcd_send_string (char *str);  // send string to the lcd

void TextLCD_SetBrightness(uint8_t Value);


void                   Transmit_Display_Text(uint8_t Line, uint8_t Indent, char *pData);
void                   Display_Text_Set_Cursor(uint8_t Line, uint8_t Indent);





#endif
