#ifndef __DIXOMMAIN__
#define __DIXOMMAIN__
#include "stdint.h"




void             Task_Main_Setup();
void             Task_Main_Infinite_Loop();
void             AppCameraAutomatic();
void             setTestCount(uint32_t cnt, boolean response);
void             Task_Perifer_Service_Infinite_Loop();

void             Task_Execution_With_Time_Interval();

void             Task_Display_Infinite_Loop();

void             Task_Interaction_Infinite_Loop();

void             Task_Flashing_LED_Indicator_Infinite_Loop();


#endif
