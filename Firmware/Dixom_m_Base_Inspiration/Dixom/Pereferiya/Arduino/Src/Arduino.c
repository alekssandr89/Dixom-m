#include "Arduino.h"
#include "Exchange.h"
#include "Nextion_LCD.h"
#include "Main_Menu.h"
#include <string.h>
//#include "StructSettings.h"

short ArduinoDisplayStatus = OFF;
short ArduinoDisplayType = 0;

int Recive_Arduino_Len = 0;
uint8_t Recive_Arduino[100];
uint8_t AdrunioData[255];

void Arduino_Data_Transmit(uint8_t *Recive_data, uint8_t len){

	if (len<250){
		for(uint8_t byte = 0; byte <len; byte++){
			AdrunioData[byte] = Recive_data[byte];
		}
		AdrunioData[len]  = 32;
		AdrunioData[len+1]= 10;
		AdrunioData[len+2]= 13;
		UART3_Transmit(&AdrunioData[0], (len+3), 0, FreeRTOS);
	}
}

#define  ArduinoBufSize 1000

int      ArduinoMsgCountW = 0,
         ArduinoMsgCountR = 0,
         ArduinoRxPointer = 0,
         ArduinoTxPointer = 0;

uint8_t  ArduinoRNScanBuf[2],
         ArduinoBuffer[ArduinoBufSize],
         ArduinoMsgBuf[255],
		 ArduinoMsgLen  = 0;

void Arduino_Data_Resive(uint8_t *data, uint8_t from){

	ArduinoRNScanBuf[0] = ArduinoRNScanBuf[1];
	ArduinoRNScanBuf[1] = data[0];

	if (ArduinoRNScanBuf[0] == 13 && ArduinoRNScanBuf[1] == 10){
		//����� ������
		ArduinoBuffer[ArduinoTxPointer] = 1;
		ArduinoUpWritePointer();
		ArduinoMsgCountW ++;

	}else if(data[0] != 10 && data[0] != 13)	{
		//���������� ������
		ArduinoBuffer[ArduinoTxPointer] =  data[0];
		ArduinoUpWritePointer();
	}
}

void Arduino_Loop(void){

	if(ArduinoMsgCountW != ArduinoMsgCountR){

		ArduinoMsgLen = 0;

		for(uint16_t i = 0; i<ArduinoBufSize; i++){

			if(ArduinoMsgLen <255){
				ArduinoMsgBuf[ArduinoMsgLen] = ArduinoBuffer[ArduinoRxPointer];
			}


			if(ArduinoBuffer[ArduinoRxPointer]==1){
				i = ArduinoBufSize;
			}else{
				ArduinoMsgLen++;
			}

			ArduinoRxPointer++;

			if(ArduinoRxPointer>=ArduinoBufSize) {
				ArduinoRxPointer = 0;
			}
		}

		if(strncmp((char*)ArduinoMsgBuf,"MENU ",5) == 0){ //MAIN MENU

			main_Menu(&ArduinoMsgBuf[5]);

		}else if(strncmp((char*)ArduinoMsgBuf, "ARD ",4) == 0){ //ARDUINO

			ArduinoSignature();
			UART3_Transmit(&ArduinoMsgBuf[4], ArduinoMsgLen-4, 0, FreeRTOS);

		}else if(strncmp((char*)ArduinoMsgBuf, "NEX ",4) == 0){ //NWXTION

			ArduinoSignature();
			NEXTION_LCD_Transmit(&ArduinoMsgBuf[4], ArduinoMsgLen-4);

		}else if(strncmp((char*)ArduinoMsgBuf, "USB ",4) == 0){ //USB

			ArduinoSignature();
			if(ArduinoMsgLen>62){
				ArduinoMsgLen=58;
			}
 			TransmitDataOutputs((uint8_t *)&ArduinoMsgBuf[4],  ArduinoMsgLen-4,  false,  true, allAvailable);



		}else if(strncmp((char*)ArduinoMsgBuf, "BLT ",4) == 0){ //BLUETOOTH

			ArduinoSignature();
			FSC_Bluetooth_Transmit(&ArduinoMsgBuf[4], ArduinoMsgLen-4, 0, FreeRTOS);
		}
		ArduinoMsgCountR++;
	}
}

void ArduinoSignature(void){
	ArduinoMsgBuf[0] = 'A';
	ArduinoMsgBuf[1] = 'R';
	ArduinoMsgBuf[2] = 'D';
}

void ArduinoUpWritePointer(void){

	ArduinoTxPointer++;
	if (ArduinoTxPointer>=ArduinoBufSize){
		ArduinoTxPointer=0;
	}
}
void ArduinoDownWritePointer(void){

	ArduinoTxPointer--;
	if (ArduinoTxPointer < 0){
		ArduinoTxPointer=ArduinoBufSize;
	}
}

void ArduinoDisplayStartInit(void){

}

void ArduinoDisplayDoneInit(void){

}
