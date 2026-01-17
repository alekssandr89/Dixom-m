
#ifndef __DISPLAYMAIN_H__
#define __DISPLAYMAIN__

#include "stdint.h"


void             SetBrightness(uint8_t value);
void             DISPLAY(uint8_t *Recive_USB);
void             SET_Brightness(uint8_t *Recive_USB);
void             Display_Loading_Complete(void);
void             DISPLAY(uint8_t *Recive_USB);

void             Display_While(void);
#endif
