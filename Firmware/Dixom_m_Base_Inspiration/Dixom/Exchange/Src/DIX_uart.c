/*
 * DIX_Uart.c
 *
 *  Created on: Mar 11, 2022
 *      Author: 79825
 */

#include <Register_UART.h>
#include <Register_DMA.h>
#include "stdint.h"
#include "Constants.h"
#include "Structures.h"
#include "Exchange.h"
#include "Nextion_LCD.h"
#include "Arduino.h"
#include "FSC_BT806.h"
#include "cmsis_os.h"
#include <string.h>
#include "stm32f4xx_hal_uart.h"

extern sDixom Dixom;

extern osSemaphoreId BinarySem_TxUART1Handle;
extern osSemaphoreId BinarySem_RxUART1Handle;

extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef huart3;
extern UART_HandleTypeDef huart5;

uint8_t Uart1RxBuff[RX_BUFFER_LENGTH_UART1];
uint16_t uart1BuffDataSize = 0;

static uint8_t Nextion;
static uint8_t Arduino;
static uint8_t WirelessRF;

uint8_t btBuffTx[MAX_TX_DATA_LENGTH_UART1];

void UART_Initialization(){



	for(uint16_t i = 0; i < RX_BUFFER_LENGTH_UART1; i ++){
		Uart1RxBuff[i] = 0;
	}

	 /*Received UART1 DMA Stream*/
	rDMA2->Stream5.CR.DBM = 0;          // 0: No buffer switching at the end of transfer
	rDMA2->Stream5.CR.CIRC = 1;          // 0: Circular mode enable
	rDMA2->Stream5.NDTR.NDT = RX_BUFFER_LENGTH_UART1;        // Received data len
	rDMA2->Stream5.PAR = (uint32_t) &(rUART1->DR); // Uart ADRR
	rDMA2->Stream5.M0AR = (uint32_t) &Uart1RxBuff;   // ReadBuff
	rDMA2->Stream5.CR.DMEIE = 0;        // Direct mode error interrupts
	rDMA2->Stream5.CR.TEIE = 1;         // Transfer error interrupts
	rDMA2->Stream5.CR.TCIE = 0;         // Transfer complete interrupts
	rDMA2->Stream5.CR.HTIE = 0;         // Half-transfer interrupts
	rDMA2->Stream5.FCR.FEIE = 1;        // Fifo error interrupts
 	rDMA2->Stream5.CR.PL = 3; // very high
	rDMA2->Stream5.CR.EN = 1; // Clear Enable the Peripheral interrupt flags
	rUART1->CR1.IDLEIE = 1;
	rUART1->CR3.DMAR = 1;     // Enable the DMA transfer for the receiver request by setting the DMAR bit in the UART CR3 register

	/*Transmit UART1 DMA Stream*/
	rDMA2->Stream7.CR.DBM = 0;          // 0: No buffer switching at the end of transfer
	rDMA2->Stream7.CR.CIRC = 0;         // 0: Circular mode disabled
	rDMA2->Stream7.PAR = (uint32_t) &rUART1->DR;   // Uart ADRR
	rDMA2->Stream7.M0AR = (uint32_t)&btBuffTx; // Configure DMA Stream source address
	rDMA2->Stream7.CR.TEIE = 1;             // Transfer error interrupts
	rDMA2->Stream7.CR.TCIE = 1;             // Transfer complete interrupts
	rDMA2->Stream7.CR.HTIE = 0;             // Half-transfer interrupts
	rDMA2->Stream7.CR.DMEIE = 0;            // Direct mode error interrupts
	rDMA2->Stream7.FCR.FEIE = 0;            // Fifo error interrupts
	rUART1->CR3.DMAT = 1;                   // Enable the DMA transfer for transmit request by setting the DMAT bit in the UART CR3 register

	HAL_UART_Receive_IT(&huart2, &WirelessRF ,1);
	HAL_UART_Receive_IT(&huart3, &Arduino ,1);
	HAL_UART_Receive_IT(&huart5, &Nextion ,1);
}


void  TxCpltCallbackUart1(){

	if(rDMA2->HISR.TCIF7 == 1){ //complete transmit
		rDMA2->HIFCR.CTCIF7 = 1;
		ExchangeStatus(WR_OK, UART1_BLUETOOTH_WRITE);
		osSemaphoreRelease(BinarySem_TxUART1Handle);
	}

	if(rDMA2->HISR.TEIF7 == 1){ //error transmit
		rDMA2->HIFCR.CTEIF7 = 1;
		ExchangeStatus(WR_ERROR, UART1_BLUETOOTH_WRITE);
     }
}



extern boolean btDeliveryState;
void UART1_Transmit(uint8_t *data, uint8_t dataLen, uint8_t timeDelay, uint8_t from){

	if(BinarySem_TxUART1Handle != NULL && dataLen < MAX_TX_DATA_LENGTH_UART1){
		if(osSemaphoreWait(BinarySem_TxUART1Handle, 1000) == osOK){
			Dixom.Exchange.Interface.TimeutTimerUsbHidSend = 0;
			btDeliveryState = unknown;
		    do{
		     	volatile uint32_t tmpreg = 0x00U;
		    	tmpreg = rUART1->SR.TC;
			    (void)tmpreg;
		    } while(0U);

			for(uint8_t byte = 0; byte <dataLen; byte++){
				btBuffTx[byte] = data[byte];
			}

			rDMA2->Stream7.NDTR.NDT = dataLen;      // Configure DMA Stream data length
			rUART1->SR.TC = 0;
			rDMA2->Stream7.CR.EN = 1;               // Enable the Peripheral

		}else{
			osSemaphoreRelease(BinarySem_TxUART1Handle);
			ExchangeStatus(WR_QUEUE_TIMEOUT, UART1_BLUETOOTH_WRITE);
		}
	}else{
		ExchangeStatus(WR_ERROR, UART1_BLUETOOTH_WRITE);
	}
}





void RxIdleCallbackUart1(){

	extern boolean btDeliveryState;
	static uint16_t RxPointer = 0;
	const uint8_t targetString[2] = "K\r"; // \r\nOK\r\n  enough K\r
	uint8_t targetLength = 2;

	if(rUART1->SR.IDLE == 1){ //idle
		uart1BuffDataSize = RX_BUFFER_LENGTH_UART1 - rDMA2->Stream5.NDTR.NDT;
		while(RxPointer != uart1BuffDataSize){

			if(Uart1RxBuff[RxPointer] == Ack){
				btDeliveryState = true;
			}else if(Uart1RxBuff[RxPointer] == Nak){
				btDeliveryState = false;
			}else{

				// Part of the target string that goes through the end of the buffer
				uint16_t remainingLength = (RX_BUFFER_LENGTH_UART1 - RxPointer);
				// Check if there is enough data in the buffer starting from the current RxPointer

				if (remainingLength >= targetLength) {

					// Compare part of the buffer, starting at the current RxPointer, with the target string
					if (strncmp((char *)&Uart1RxBuff[RxPointer], (char *)targetString, targetLength) == 0) {
						btDeliveryState = true;
					}
				} else {

					// Compare the rest of the line with the beginning of the buffer
					if (strncmp((char *)&Uart1RxBuff[RxPointer], (char *)targetString, remainingLength) == 0 &&
							strncmp((char *)&Uart1RxBuff[0], (char *)(targetString + remainingLength), targetLength - remainingLength) == 0) {
						btDeliveryState = true;
					}
				}
			}
			RxPointer = (RxPointer + 1) % RX_BUFFER_LENGTH_UART1;
		}

	    do{
	     	volatile uint32_t tmpreg = 0x00U;
	    	tmpreg = rUART1->DR.DR;
		    (void)tmpreg;
	    } while(0U);
	}
}

void RxDmaCallbackUart1(){
	if(rDMA2->HISR.TCIF5 == 1){ //complete
		rDMA2->HIFCR.CTCIF5 = 1;
	}

	if(rDMA2->HISR.TEIF5 == 1){ //error receive
		rDMA2->HIFCR.CTEIF5 = 1;
		rDMA2->Stream5.CR.EN = 1;
	}

	if(rDMA2->HISR.FEIF5 == 1){ //error fifo
		rDMA2->HIFCR.CFEIF5 = 1;
		rDMA2->Stream5.CR.EN = 1;
	}
}


void UART2_Transmit(uint8_t *data, uint8_t dataLen, uint8_t timeDelay, uint8_t from){
	ExchangeStatus(HAL_UART_Transmit_IT(&huart2, data, dataLen), UART2_WIRELESS_RF_WRITE);
	Delay(timeDelay, from);
}

void UART3_Transmit(uint8_t *data, uint8_t dataLen, uint8_t timeDelay, uint8_t from){
	ExchangeStatus(HAL_UART_Transmit_IT(&huart3, data, dataLen), UART3_ARDUINO_WRITE);
	Delay(timeDelay, from);
}

void UART5_Transmit(uint8_t *data, uint8_t dataLen, uint8_t timeDelay, uint8_t from){
	ExchangeStatus(HAL_UART_Transmit_IT(&huart5, data, dataLen), UART5_NEXTION_WRITE);
	Delay(timeDelay, from);
}

void HAL_UART_AbortReceiveCpltCallback(UART_HandleTypeDef *huart){
	if(huart == &huart1){
		if(ExchangeStatus( HAL_UART_Receive_DMA(&huart1, &Uart1RxBuff, 1), UART1_BLUETOOTH_READ)== WR_BUSY){

		}
	}
}


//----- RECEIVING UART BYTE BY BYTE DATA VIA INTERRUPT----------------------------------------------------------------------------//

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){


	if(huart == &huart2){

		if(Dixom.DevInfo.InMode == ReadyForWork){

		}
		ExchangeStatus(HAL_UART_Receive_IT(&huart2, &WirelessRF ,1), UART2_WIRELESS_RF_READ);
	}else if(huart == &huart3){

		if(Dixom.DevInfo.InMode == ReadyForWork){
			Arduino_Data_Resive(&Arduino, Interrupt);
		}
		ExchangeStatus(HAL_UART_Receive_IT(&huart3, &Arduino ,1), UART3_ARDUINO_READ);
	}else if(huart == &huart5){

		if(Dixom.DevInfo.InMode == ReadyForWork){
			NEXTION_LCD_Resive(&Nextion, Interrupt);
		}
		ExchangeStatus(HAL_UART_Receive_IT(&huart5, &Nextion ,1), UART5_NEXTION_READ);
	}
}
