#ifndef __NEXTION_LCD_H__
#define __NEXTION_LCD__
#include "stdint.h"

void             NEXTION_LCD_Transmit(uint8_t *Recive_data, uint8_t len);
void             NEXTION_LCD_Resive(uint8_t *data, uint8_t from);
void             NextionUpWritePointer();
void             NextionDownWritePointer(void);
void             NextionSignature();
void             Nextion_Loop(void);
void             Nextion_Display_Automatic_Detection(void);
void             NextionDisplayDoneInit(void);

#endif
