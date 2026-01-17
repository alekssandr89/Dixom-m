#include "Nextion_LCD.h"
#include "Exchange.h"
#include "Main_Menu.h"
#include <string.h>
#include <Structures.h>

extern sDixom Dixom;
//#include "StructSettings.h"

short NextionDisplayStatus = OFF;
short NextionDisplayType = 0;

int Recive_Nextion_Len = 0;
uint8_t Recive_Nextion[100];
uint8_t NextionData[255];

void NEXTION_LCD_Transmit(uint8_t *Recive_data, uint8_t len){

	if (len<250){
		;
		for(uint8_t byte = 0; byte <len; byte++){
			NextionData[byte] = Recive_data[byte];
		}
		NextionData[len]  = 0xFF;
		NextionData[len+1]= 0xFF;
		NextionData[len+2]= 0xFF;
		UART5_Transmit(&NextionData[0], (len+3), 0, FreeRTOS);
	}
}

#define  NextionBufSize 1000

int      NextionMsgCountW = 0,
         NextionMsgCountR = 0,
         NextionRxPointer = 0,
         NextionTxPointer = 0;

uint8_t  NextionRNScanBuf[2],
         NextionBuffer[NextionBufSize],
         NextionMsgBuf[255];
uint16_t NextionMsgLen  = 0;

void NEXTION_LCD_Resive(uint8_t *data, uint8_t from){

	NextionRNScanBuf[0] = NextionRNScanBuf[1];
	NextionRNScanBuf[1] = data[0];

	if (NextionRNScanBuf[0] == 13 && NextionRNScanBuf[1] == 10){
		//����� ������
		NextionBuffer[NextionTxPointer] = 1;
		NextionUpWritePointer();
		NextionMsgCountW ++;

	}else if(data[0] != 10 && data[0] != 13 && data[0] != 255)	{
		//���������� ������
		NextionBuffer[NextionTxPointer] =  data[0];
		NextionUpWritePointer();
	}
}

void Nextion_Loop(void){

	if(NextionMsgCountW != NextionMsgCountR){

		NextionMsgLen = 0;

		for(uint16_t i = 0; i<NextionBufSize; i++){

			if(NextionMsgLen<255){
				NextionMsgBuf[NextionMsgLen] = NextionBuffer[NextionRxPointer];
			}

			if(NextionBuffer[NextionRxPointer]==1){
				i = NextionBufSize;
			}else{
				NextionMsgLen++;
			}

			NextionRxPointer++;

			if(NextionRxPointer>=NextionBufSize) {
				NextionRxPointer = 0;
			}
		}

		if(strncmp((char*)NextionMsgBuf,"MENU ",5) == 0){ //MAIN MENU

			main_Menu(&NextionMsgBuf[5]);

		}else if(strncmp((char*)NextionMsgBuf, "ARD ",4) == 0){ //ARDUINO

			UART3_Transmit(&NextionMsgBuf[4], NextionMsgLen+4, 0, FreeRTOS);

		}else if(strncmp((char*)NextionMsgBuf, "NEX ",4) == 0){ //NEXTION

			NEXTION_LCD_Transmit(&NextionMsgBuf[4], NextionMsgLen+4);

		}else if(strncmp((char*)NextionMsgBuf, "USB ",4) == 0){ // USB

			if(NextionMsgLen>62){
				NextionMsgLen=58;
			}
 	   	 	TransmitDataOutputs((uint8_t *)NextionMsgBuf+4, NextionMsgLen-4,  false,  true, allAvailable);


		}else if(strncmp((char*)NextionMsgBuf, "BLT ",4) == 0){ //BLUETOOTH

			FSC_Bluetooth_Transmit(&NextionMsgBuf[4], NextionMsgLen-4, 0, FreeRTOS);
		}
		NextionMsgCountR++;
	}
}

void NextionSignature(void){
	NextionMsgBuf[0] = 'N';
	NextionMsgBuf[1] = 'E';
	NextionMsgBuf[2] = 'X';
}

void NextionUpWritePointer(void){

	NextionTxPointer++;
	if (NextionTxPointer>=NextionBufSize){
		NextionTxPointer=0;
	}
}
void NextionDownWritePointer(void){

	NextionTxPointer--;
	if (NextionTxPointer < 0){
		NextionTxPointer=NextionBufSize;
	}
}


void Nextion_Display_Automatic_Detection(void){
	if( GetSettings(SettingsNextionLCD) == ON ){
		NextionDisplayStatus=ON;

	}
}

void NextionDisplayDoneInit(void){
	if( NextionDisplayStatus == ON ){
		NextionDisplayType = GetSettings(SettingsTypeNextionLCD);
	}
}


