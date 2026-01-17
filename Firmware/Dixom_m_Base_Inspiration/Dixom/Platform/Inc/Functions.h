/*
 * Main_Functions.h
 *
 *  Created on: Feb 8, 2022
 *      Author: 79825
 */

#ifndef PLATFORM_INC_FUNCTIONS_H_
#define PLATFORM_INC_FUNCTIONS_H_

#include "stdint.h"

void Main_Thread();

void Main_Function_Loop();
void Power_Control_Loop();

void CURRENT_EXCHANGE_INTERFACE(uint8_t ControlByte, uint8_t *Received_String);
void GetCurrentExchangeInterface();

void      CMD_IWDG_SET(uint8_t ControlByte, uint8_t *Received_String);
void SleepSTM32();
void                   PlatformHrdRestart();
void                   PlatformRestart(uint8_t from);
void                   Delay(uint32_t timeDelay, uint8_t from);


void Add_Commands_Execution_In_Queue(uint8_t* data, int len);
void Add_Packet_To_CircularBuffer_UsbStringRx(uint8_t* data, uint8_t len, uint8_t interface);





#endif /* PLATFORM_INC_FUNCTIONS_H_ */
