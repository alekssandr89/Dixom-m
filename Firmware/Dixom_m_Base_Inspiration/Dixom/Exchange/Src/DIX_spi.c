/*
 * DIX_spi.c
 *
 *  Created on: Mar 11, 2022
 *      Author: 79825
 */
#include <Description.h>
#include "stdint.h"
#include "Constants.h"
#include "Structures.h"
#include "cmsis_os.h"
#include "DIX_spi.h"
#include "spi.h"
#include "stm32f4xx_hal_spi.h"

extern SPI_HandleTypeDef hspi2;
extern osSemaphoreId BinarySem_ExchangeSPI2Handle;
extern sDixom Dixom;


WorkRes Transmit_SPI2(uint8_t *pTxData, uint16_t Size, uint32_t Timeout, uint8_t ExchangeType)
{
	uint8_t State = WR_ERROR;

	if(BinarySem_ExchangeSPI2Handle != NULL){
		if(osSemaphoreWait(BinarySem_ExchangeSPI2Handle, 100) == osOK){
			if(ExchangeType == ExchangeMainSteam){
				State = ExchangeStatus(HAL_SPI_Transmit(&hspi2, pTxData, Size, Timeout), SPI2_WRITE);
			}else if(ExchangeType == ExchangeMemMainSteam){

			}else{
				State = ExchangeStatus(WR_ERROR, SPI2_WRITE);
			}
		}else{
			State = ExchangeStatus(WR_QUEUE_TIMEOUT, SPI2_WRITE);
		}
		osSemaphoreRelease(BinarySem_ExchangeSPI2Handle);
	}else{
		State = ExchangeStatus(WR_ERROR, SPI2_WRITE);
	}
	return State;
}

WorkRes Receiver_SPI2(uint8_t *pTxData, uint8_t *pRxData, uint16_t Size, uint32_t Timeout, uint8_t ExchangeType)
{
	uint8_t State = WR_ERROR;

	if(BinarySem_ExchangeSPI2Handle != NULL){
		if(osSemaphoreWait(BinarySem_ExchangeSPI2Handle, 100) == osOK){
			if(ExchangeType == ExchangeMainSteam){
				State = ExchangeStatus(HAL_SPI_Receive (&hspi2, pRxData, Size, Timeout), SPI2_READ);
			}else if(ExchangeType == ExchangeMemMainSteam){
				State =  ExchangeStatus(HAL_SPI_TransmitReceive(&hspi2, pTxData, pRxData, Size, Timeout), SPI2_READ);
			}else{
				State = ExchangeStatus(WR_ERROR, SPI2_READ);
			}
		}else{
			State = ExchangeStatus(WR_QUEUE_TIMEOUT, SPI2_READ);
		}
		osSemaphoreRelease(BinarySem_ExchangeSPI2Handle);
	}else{
		State = ExchangeStatus(WR_ERROR, SPI2_READ);
	}
	return State;
}

void SetStateSPI(uint8_t ControlByte, uint8_t *Recive_USB){

	if(ControlByte == DATA_GET){
		short state  = 0;

		sscanf((char *)Recive_USB,"%hu", &state);

		if(state<0 || state>1 ){
			TransmitDataOutputs((uint8_t *)GeneralNotifi_CommandError, AutoLenght,  false,  true, allAvailable);

		}else{

			 if(state == OFF){
				 Dixom.Service.PeripheryState[SPI] = 0;
				 SPI_Deinit();
				 GetStateSPI();
			 }else{
				 Dixom.Service.PeripheryState[SPI] = 1;
				 SPI_Init();
				 GetStateSPI();
			 }
		}
	}else if(ControlByte == DATA_GET){ //GET
		GetStateSPI();
	}else{
 		TransmitDataOutputs((uint8_t *)GeneralNotifi_CtlBitNotSupported, AutoLenght,  false,  true, allAvailable);

	}
}

void GetStateSPI(){
	sprintf( (char *) Dixom.Buff, General_State_SPI , Dixom.Service.PeripheryState[SPI]);
	TransmitDataOutputs(Dixom.Buff, AutoLenght,  false,  true, allAvailable);

}



