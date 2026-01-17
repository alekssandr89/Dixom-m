/*
 * DIX_i2c.c
 *
 *  Created on: Mar 11, 2022
 *      Author: 79825
 */
#include <Description.h>
#include "stdint.h"
#include "Constants.h"
#include "Structures.h"
#include "Functions.h"
#include "cmsis_os.h"
#include "DIX_i2c.h"
#include "stm32f4xx_hal_i2c.h"
#include "stm32f4xx_hal_dma.h"


extern I2C_HandleTypeDef hi2c2;
extern I2C_HandleTypeDef hi2c1;
extern DMA_HandleTypeDef hdma_i2c1_tx;
extern osSemaphoreId BinarySem_ExchangeI2C1Handle;
extern osSemaphoreId BinarySem_ExchangeI2C2Handle;

extern sDixom Dixom;

uint16_t DevPrevAddres_I2C1;
uint16_t DevPrevAddres_I2C2;

WorkRes Waiting_I2C_For_Release(I2C_HandleTypeDef *hi2c, uint32_t DevAddress, uint32_t TimeOut );


//----- SCANNING ADDRESSES ON THE I2C BUS 1 AND 2----------------------------------------------------------------------------//

void Scanning_I2C1(void){
	if(BinarySem_ExchangeI2C1Handle != NULL){
		if(osSemaphoreWait(BinarySem_ExchangeI2C1Handle, 100) == osOK){
			sprintf( (char*)Dixom.Buff, Basis_Notification_1_14_0, 1);
 	 		TransmitDataOutputs(Dixom.Buff, AutoLenght,  false,  true, allAvailable);

			if(HAL_I2C_GetState(&hi2c1) == HAL_I2C_STATE_RESET){
				return;
			}
		    for(uint16_t i = 0; i < 128; i++) {
		        if(DevState_I2C1(i << 1, 1, 150) == WR_OK) {
		            snprintf((char*)Dixom.Buff, sizeof(Dixom.Buff), Basis_Notification_1_14_1, i, i);
  		 			TransmitDataOutputs(Dixom.Buff, AutoLenght,  false,  true, allAvailable);
		        }
		    }
		    osSemaphoreRelease(BinarySem_ExchangeI2C1Handle);
		}
		sprintf( (char*)Dixom.Buff, Basis_Notification_1_14_2, 1);
 		TransmitDataOutputs(Dixom.Buff, AutoLenght,  false,  true, allAvailable);

	}
}

void Scanning_I2C2(void){
	if(BinarySem_ExchangeI2C2Handle != NULL){
		if(osSemaphoreWait(BinarySem_ExchangeI2C2Handle, 100) == osOK){
			sprintf( (char*)Dixom.Buff, Basis_Notification_1_14_0, 2);
 			TransmitDataOutputs(Dixom.Buff, AutoLenght,  false,  true, allAvailable);

			if(HAL_I2C_GetState(&hi2c2) == HAL_I2C_STATE_RESET){
				return;
			}
		    for(uint16_t i = 0; i < 128; i++) {
		        if(DevState_I2C2(i << 1, 1, 150) == WR_OK) {
		            snprintf((char*)Dixom.Buff, sizeof(Dixom.Buff), Basis_Notification_1_14_1, i, i);
 		 			TransmitDataOutputs(Dixom.Buff, AutoLenght,  false,  true, allAvailable);
		        }
		    }
		    osSemaphoreRelease(BinarySem_ExchangeI2C2Handle);
		}
		sprintf( (char*)Dixom.Buff, Basis_Notification_1_14_2, 2);
 		TransmitDataOutputs(Dixom.Buff, AutoLenght,  false,  true, allAvailable);

	}
}

//----- SCAN DEVICE STATUS ON I2C BUS 1 AND 2----------------------------------------------------------------------------//

WorkRes DevState_I2C1(uint16_t DevAddress, uint16_t Trials, uint16_t Timeout){

	uint8_t State = WR_ERROR;
	if(BinarySem_ExchangeI2C1Handle != NULL){
		if(osSemaphoreWait(BinarySem_ExchangeI2C1Handle, 100) == osOK){
			switch (HAL_I2C_IsDeviceReady(&hi2c1, DevAddress, Trials, Timeout))
			{
			case HAL_OK       :  State =  WR_OK;           break;
			case HAL_ERROR    :  State =  WR_ERROR;        break;
			case HAL_BUSY     :  State =  WR_BUSY;         break;
			case HAL_TIMEOUT  :  State =  WR_TIMEOUT;      break;
			default           :  State =  WR_ERROR;        break;
			}
		}else{
			State = ExchangeStatus(WR_QUEUE_TIMEOUT, I2C1_READ);
		}
		osSemaphoreRelease(BinarySem_ExchangeI2C1Handle);
	}else{
		State = ExchangeStatus(WR_ERROR, I2C1_READ);
	}
	return State;
}

WorkRes DevState_I2C2(uint16_t DevAddress, uint16_t Trials, uint16_t Timeout){

	uint8_t State = WR_ERROR;
	if(BinarySem_ExchangeI2C2Handle != NULL){
		if(osSemaphoreWait(BinarySem_ExchangeI2C2Handle, 100) == osOK){
			switch (HAL_I2C_IsDeviceReady(&hi2c2, DevAddress, Trials, Timeout))
			{
			case HAL_OK       :  State =  WR_OK;           break;
			case HAL_ERROR    :  State =  WR_ERROR;        break;
			case HAL_BUSY     :  State =  WR_BUSY;         break;
			case HAL_TIMEOUT  :  State =  WR_TIMEOUT;      break;
			default           :  State =  WR_ERROR;        break;
			}
		}else{
			State = ExchangeStatus(WR_QUEUE_TIMEOUT, I2C2_READ);
		}
		osSemaphoreRelease(BinarySem_ExchangeI2C2Handle);
	}else{
		State = ExchangeStatus(WR_ERROR, I2C2_READ);
	}
	return State;
}

//----- DATA TRANSFER TO I2C 1 AND2-----------------------------------------------------------------------------------------//

WorkRes Transmit_I2C1(uint16_t DevAddress, uint16_t MemAddress, uint16_t MemAddSize, uint8_t *pData, uint16_t Size, uint32_t Timeout, uint8_t ExchangeType){

	uint8_t State = WR_ERROR;
	static uint8_t i2c_buff[255];

	if(BinarySem_ExchangeI2C1Handle != NULL){
		if(osSemaphoreWait(BinarySem_ExchangeI2C1Handle, 1000) == osOK){

			if(Waiting_I2C_For_Release(&hi2c1, DevPrevAddres_I2C1, 100000) == WR_TIMEOUT){
				State = ExchangeStatus(WR_TIMEOUT, I2C1_WRITE);
				osSemaphoreRelease(BinarySem_ExchangeI2C1Handle);
			}else{
				if(ExchangeType == ExchangeMainSteam){
					State =  ExchangeStatus(HAL_I2C_Master_Transmit(&hi2c1, DevAddress, pData, Size, Timeout), I2C1_WRITE);
					osSemaphoreRelease(BinarySem_ExchangeI2C1Handle);
				}else if(ExchangeType == ExchangeMainSteamDMA){
					for(uint8_t i=0; i<Size; i++){
						i2c_buff[i] = *pData++;
					}
					State =  ExchangeStatus(HAL_I2C_Master_Transmit_DMA(&hi2c1, DevAddress, &i2c_buff[0], Size), I2C1_WRITE);
				}else if(ExchangeType == ExchangeMemMainSteam){
					State =  ExchangeStatus(HAL_I2C_Mem_Write( &hi2c1, DevAddress, MemAddress, MemAddSize,  pData, Size, Timeout), I2C1_WRITE);
				    osSemaphoreRelease(BinarySem_ExchangeI2C1Handle);
				}else if(ExchangeType == ExchangeMemMainSteamDMA){
					for(uint8_t i=0; i<Size; i++){
						i2c_buff[i] = *pData++;
					}
					State =  ExchangeStatus(HAL_I2C_Mem_Write_DMA(&hi2c1, DevAddress, MemAddress, MemAddSize, &i2c_buff[0], Size), I2C1_WRITE);

				}else{
					State = ExchangeStatus(WR_ERROR, I2C1_WRITE);
					osSemaphoreRelease(BinarySem_ExchangeI2C1Handle);
				}
				DevPrevAddres_I2C1 = DevAddress;
			}

		}else{
			osSemaphoreRelease(BinarySem_ExchangeI2C1Handle);
			State = ExchangeStatus(WR_QUEUE_TIMEOUT, I2C1_WRITE);
		}
	}else{
		State = ExchangeStatus(WR_ERROR, I2C1_WRITE);
	}
	return State;
}

WorkRes Transmit_I2C2(uint16_t DevAddress, uint16_t MemAddress, uint16_t MemAddSize, uint8_t *pData, uint16_t Size, uint32_t Timeout, uint8_t ExchangeType){

	uint8_t State = WR_ERROR;
	static uint8_t i2c_buff[255];

	if(BinarySem_ExchangeI2C2Handle != NULL){
		if(osSemaphoreWait(BinarySem_ExchangeI2C2Handle, 1000) == osOK){

			if(Waiting_I2C_For_Release(&hi2c2, DevPrevAddres_I2C2, 100000) == WR_TIMEOUT){
				State = ExchangeStatus(WR_TIMEOUT, I2C2_WRITE);
				osSemaphoreRelease(BinarySem_ExchangeI2C2Handle);
			}else{
				if(ExchangeType == ExchangeMainSteam){
					State =  ExchangeStatus(HAL_I2C_Master_Transmit(&hi2c2, DevAddress, pData, Size, Timeout), I2C2_WRITE);
					osSemaphoreRelease(BinarySem_ExchangeI2C2Handle);
				}else if(ExchangeType == ExchangeMainSteamDMA){
					for(uint8_t i=0; i<Size; i++){
						i2c_buff[i] = *pData++;
					}
					State =  ExchangeStatus(HAL_I2C_Master_Transmit_DMA(&hi2c2, DevAddress, &i2c_buff[0], Size), I2C2_WRITE);
				}else if(ExchangeType == ExchangeMemMainSteam){

					State =  ExchangeStatus(HAL_I2C_Mem_Write( &hi2c2, DevAddress, MemAddress, MemAddSize,  pData, Size, Timeout), I2C2_WRITE);
				    osSemaphoreRelease(BinarySem_ExchangeI2C2Handle);
				}else if(ExchangeType == ExchangeMemMainSteamDMA){

					for(uint8_t i=0; i<Size; i++){
						i2c_buff[i] = *pData++;
					}
					State =  ExchangeStatus(HAL_I2C_Mem_Write_DMA(&hi2c2, DevAddress, MemAddress, MemAddSize, &i2c_buff[0], Size), I2C2_WRITE);

				}else{
					State = ExchangeStatus(WR_ERROR, I2C2_WRITE);
					osSemaphoreRelease(BinarySem_ExchangeI2C2Handle);
				}
				DevPrevAddres_I2C2 = DevAddress;
			}

		}else{
			State = ExchangeStatus(WR_QUEUE_TIMEOUT, I2C2_WRITE);
			osSemaphoreRelease(BinarySem_ExchangeI2C2Handle);
		}
	}else{
		State = ExchangeStatus(WR_ERROR, I2C2_WRITE);
	}
	return State;

}

//----- CLEARING THE QUEUE AFTER TRANSFERRING DATA VIA DMA-------------------------------------------------------//

void HAL_I2C_MasterTxCpltCallback(I2C_HandleTypeDef *hi2c){
	if(hi2c == &hi2c1){
		osSemaphoreRelease(BinarySem_ExchangeI2C1Handle);
	}else if(hi2c == &hi2c2){
		osSemaphoreRelease(BinarySem_ExchangeI2C2Handle);
	}
}

void HAL_I2C_MemTxCpltCallback(I2C_HandleTypeDef *hi2c){
	if(hi2c == &hi2c1){
		osSemaphoreRelease(BinarySem_ExchangeI2C1Handle);
	}else if(hi2c == &hi2c2){
		osSemaphoreRelease(BinarySem_ExchangeI2C2Handle);
	}
}

void HAL_I2C_ErrorCallback(I2C_HandleTypeDef *hi2c){
	if(hi2c == &hi2c1){
		osSemaphoreRelease(BinarySem_ExchangeI2C1Handle);
	}else if(hi2c == &hi2c2){
		osSemaphoreRelease(BinarySem_ExchangeI2C2Handle);
	}
}

void HAL_I2C_AbortCpltCallback(I2C_HandleTypeDef *hi2c){
	if(hi2c == &hi2c1){
		osSemaphoreRelease(BinarySem_ExchangeI2C1Handle);
	}else if(hi2c == &hi2c2){
		osSemaphoreRelease(BinarySem_ExchangeI2C2Handle);
	}
}

//----- RECEIVING DATA VIA THE I2C BUS 1 AND 2----------------------------------------------------------------------------//

WorkRes Receiver_I2C1(uint16_t DevAddress, uint16_t MemAddress, uint16_t MemAddSize, uint8_t *pData, uint16_t Size, uint32_t Timeout, uint8_t ExchangeType){

	uint8_t State = WR_ERROR;

	if(BinarySem_ExchangeI2C1Handle != NULL){
		if(osSemaphoreWait(BinarySem_ExchangeI2C1Handle, 100) == osOK){

			while(hi2c1.State !=  HAL_I2C_STATE_READY ){
				ExchangeStatus(WR_BUSY, I2C1_READ);Delay (1, FreeRTOS);
			}

			if(ExchangeType == ExchangeMainSteam){
				State =  ExchangeStatus(HAL_I2C_Master_Receive(&hi2c1, DevAddress, pData, Size, Timeout), I2C1_READ);
				osSemaphoreRelease(BinarySem_ExchangeI2C1Handle);
			}else if(ExchangeType == ExchangeMainSteamDMA){
				State =  ExchangeStatus(HAL_I2C_Master_Receive_DMA(&hi2c1, DevAddress, pData, Size), I2C1_READ);//Delay (1, FreeRTOS);
			}else if(ExchangeType == ExchangeMemMainSteam){
				State =  ExchangeStatus(HAL_I2C_Mem_Read( &hi2c1, DevAddress, MemAddress, MemAddSize,  pData, Size, Timeout), I2C1_READ);
				osSemaphoreRelease(BinarySem_ExchangeI2C1Handle);
			}else{
				State = ExchangeStatus(WR_ERROR, I2C1_READ);
				osSemaphoreRelease(BinarySem_ExchangeI2C1Handle);
			}

		}else{
			State = ExchangeStatus(WR_QUEUE_TIMEOUT, I2C1_READ);
		}
	}else{
		State = ExchangeStatus(WR_ERROR, I2C1_READ);
	}
	return State;
}


WorkRes Receiver_I2C2(uint16_t DevAddress, uint16_t MemAddress, uint16_t MemAddSize, uint8_t *pData, uint16_t Size, uint32_t Timeout, uint8_t ExchangeType){

	uint8_t State = WR_ERROR;

	if(BinarySem_ExchangeI2C2Handle != NULL){
		if(osSemaphoreWait(BinarySem_ExchangeI2C2Handle, 1000) == osOK){

			if(Waiting_I2C_For_Release(&hi2c2, DevPrevAddres_I2C2, 100000) == WR_TIMEOUT){

				State = ExchangeStatus(WR_TIMEOUT, I2C2_WRITE);
				osSemaphoreRelease(BinarySem_ExchangeI2C2Handle);

			}else{
				if(ExchangeType == ExchangeMainSteam){
					State =  ExchangeStatus(HAL_I2C_Master_Receive(&hi2c2, DevAddress, pData, Size, Timeout), I2C2_READ);
					osSemaphoreRelease(BinarySem_ExchangeI2C2Handle);
				}else if(ExchangeType == ExchangeMainSteamDMA){
					State =  ExchangeStatus(HAL_I2C_Master_Receive_DMA(&hi2c2, DevAddress, pData, Size), I2C2_READ);
				}else if(ExchangeType == ExchangeMemMainSteam){
					State =  ExchangeStatus(HAL_I2C_Mem_Read( &hi2c2, DevAddress, MemAddress, MemAddSize,  pData, Size, Timeout), I2C2_READ);
					osSemaphoreRelease(BinarySem_ExchangeI2C2Handle);
				}else if(ExchangeType == ExchangeMemMainSteamDMA){
					State =  ExchangeStatus(HAL_I2C_Mem_Read_DMA(&hi2c2, DevAddress, MemAddress, MemAddSize, pData, Size), I2C2_READ);
				}else{
					State = ExchangeStatus(WR_ERROR, I2C2_READ);
				}
				DevPrevAddres_I2C2 = DevAddress;
			}

		}else{
			State = ExchangeStatus(WR_QUEUE_TIMEOUT, I2C2_READ);
			osSemaphoreRelease(BinarySem_ExchangeI2C2Handle);
		}

	}else{
		State = ExchangeStatus(WR_ERROR, I2C2_READ);
	}
	return State;
}


void HAL_I2C_MasterRxCpltCallback(I2C_HandleTypeDef *hi2c){
	if(hi2c == &hi2c1){
		osSemaphoreRelease(BinarySem_ExchangeI2C1Handle);
	}else if(hi2c == &hi2c2){
		osSemaphoreRelease(BinarySem_ExchangeI2C2Handle);
	}
}

void HAL_I2C_MemRxCpltCallback(I2C_HandleTypeDef *hi2c){
	if(hi2c == &hi2c1){
		osSemaphoreRelease(BinarySem_ExchangeI2C1Handle);
	}else if(hi2c == &hi2c2){
		osSemaphoreRelease(BinarySem_ExchangeI2C2Handle);
	}
}


WorkRes Waiting_I2C_For_Release(I2C_HandleTypeDef *hi2c, uint32_t DevPrevAddres, uint32_t TimeOut ){

	while (HAL_I2C_GetState(hi2c) != HAL_I2C_STATE_READY){
		if(TimeOut == 0){
			HAL_I2C_Master_Abort_IT(hi2c, DevPrevAddres);
			return WR_TIMEOUT;
		}else{
			TimeOut--;
		}
	}

	return WR_OK;
}

WorkRes Receiver_I2C2_NoSemaphore(uint16_t DevAddress, uint16_t MemAddress, uint16_t MemAddSize, uint8_t *pData, uint16_t Size, uint32_t Timeout, uint8_t ExchangeType){
	if(ExchangeType == ExchangeMainSteam){
		return ExchangeStatus(HAL_I2C_Master_Receive(&hi2c2, DevAddress, pData, Size, Timeout), I2C2_READ);
	}
	else if(ExchangeType == ExchangeMemMainSteam){
		return ExchangeStatus(HAL_I2C_Mem_Read( &hi2c2, DevAddress, MemAddress, MemAddSize,  pData, Size, Timeout), I2C2_READ);
	}
	return ExchangeStatus(HAL_ERROR, I2C2_READ);
}


