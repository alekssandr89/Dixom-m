#ifndef __MAIN_ACTION_H__
#define __MAIN_ACTION_H__
#include "stdint.h"

void             ActionSend(uint8_t ControlByte, uint8_t *Recive_USB);

void             Action(short action, short data1, short data2, short data3,  short data4, short channel, short pressed);
/*
typedef struct
{
	int8_t    Action;
	int8_t    data1;
	int8_t    data2;
	int8_t    data3;
	int8_t    data4;
	uint16_t  channel;
	int8_t    ClickMetod;
} tActionStruct;

*/


void             Next(void);
void             Previous(void);
void             HoldNext(void);
void             HoldPrevious(void);

void             Action_MultiWheel(short data1, short data2, short data3, short data4, short channel, short pressed);

#endif
