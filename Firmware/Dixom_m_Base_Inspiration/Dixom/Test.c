/*
 * Experiment.c
 *
 *  Created on: Jan 16, 2022
 *      Author: 79825
 */

#include <Constants.h>
#include <Structures.h>
#include "Test.h"
#include "string.h"
#include "stdio.h"
#include "Exchange.h"
#include "Description.h"
#include <base64.h>
#include "softCRC.h"

void TEST_Stream(){



}

void TEST(uint8_t ControlByte, uint8_t Function, uint8_t *InputData){

         if(Function == 1)      {  Test_1(InputData);     }
	else if(Function == 2)      {  Test_2(InputData);     }
	else if(Function == 3)      {  Test_3(InputData);     }
	else if(Function == 4)      {  Test_4(InputData);     }
	else if(Function == 5)      {  Test_5(InputData);     }
	else if(Function == 6)      {  Test_6(InputData);     }
	else if(Function == 7)      {  Test_7(InputData);     }
}


#define I2C_PORT_1              0x02
#define I2C_PORT_3              0x08
#define I2C_PORT_5              0x20

uint8_t Data[10];


extern CRC_HandleTypeDef hcrc;

uint8_t testBuff[256];

void Test_1 (uint8_t *InputData){

	for(uint8_t i = 0; i <255; i++){
		testBuff[i] = i;
	}
	uint32_t CRCVal = HAL_CRC_Calculate(&hcrc, (uint32_t *)testBuff, 255);

	uint32_t CRCVal2 =  ArcCrc16( testBuff, 255);

	CRCVal2 = CRCVal;
	CRCVal = CRCVal2;
}

void Test_2 (uint8_t *InputData){
	Data[0]=I2C_PORT_3;
	Transmit_I2C2(DEVICE_ADDR_AMPLIFIER_3xTAS3251<<1, NOT_USED, NOT_USED, &Data[0], 1, 200, ExchangeMainSteam);
}

#define SEND10 "AT+GATTSEND=10,0123456789"
#define SEND20 "AT+GATTSEND=20,01234567890123456789"
#define SEND30 "AT+GATTSEND=30,012345678901234567890123456789"

void Test_3 (uint8_t *InputData){
 	UART1_Transmit((uint8_t *)&SEND10, 25, 0, FreeRTOS);
}



void Test_4 (uint8_t *InputData){

 	UART1_Transmit((uint8_t *)&SEND20, 35, 0, FreeRTOS);

}

void Test_5 (uint8_t *InputData){

 	UART1_Transmit((uint8_t *)&SEND20, 45, 0, FreeRTOS);


}
#define sdsdsd "20 6 0 Ibiza Summer Mix 2023 🍓 Best Of Tropical Deep House Music Chill Out Mix 2023🍓 Chillout Lounge #210"
#define sdsdsd2 "20 6 0 Что такое mutex | Cинхронизация потоков | Многопоточное программирование | C++ #6"
void Test_6 (uint8_t *InputData){
	TransmitDataOutputs((uint8_t *)sdsdsd, AutoLenght,  false,  true, allAvailable);
}

void Test_7 (uint8_t *InputData){
	TransmitDataOutputs((uint8_t *)sdsdsd2, AutoLenght,  false,  true, allAvailable);


}



















