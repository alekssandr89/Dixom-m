#ifndef __ADC_H__
#define __ADC_H__


void             ADC_Start_DMA();
void             ADC_READ(void);
void             SleepMode(int Btn1);
void             ChEvent (short CH);
void             Action_Analog_Control_Loop(void);
#endif
