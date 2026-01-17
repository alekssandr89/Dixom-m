/*
 * DIX_can.c
 *
 *  Created on: Jun 3, 2022
 *      Author: 79825
 */


#include "Can_Constant.h"
#include "Can.h"
#include "DIX_can.h"
#include "Exchange.h"
#include "Structures.h"
#include "stm32f4xx.h"

extern CAN_HandleTypeDef hcan1;
extern sDixom Dixom;


CAN_TxHeaderTypeDef pTxHeader;
CAN_RxHeaderTypeDef pRxHeader;
uint32_t TxMailbox;
uint8_t canI=0, canR, canTX_data[8], canRX_data[8];
CAN_FilterTypeDef sFilterConfig;

CAN_FilterTypeDef CAN_FilterInitStructure;

void InitTxHeader(){

	pTxHeader.DLC = 8;                //Send data lenght
	pTxHeader.IDE = CAN_ID_STD;       //Standart
	pTxHeader.StdId = 0x0560;
	pTxHeader.RTR = CAN_RTR_DATA;
	pTxHeader.TransmitGlobalTime = DISABLE;
}

WorkRes UpdateCanFilter(){


	CAN_FilterInitStructure.FilterFIFOAssignment = CAN_RX_FIFO0;
	CAN_FilterInitStructure.SlaveStartFilterBank = 14;

	for(uint8_t FilterBank = 0; FilterBank<NUM_ELEM_FILER; FilterBank++){

		if(Dixom.Can.Filter[FilterBank].Activation == YES){

			CAN_FilterInitStructure.FilterBank = FilterBank;

			CAN_FilterInitStructure.FilterActivation = ENABLE;

			if(Dixom.Can.Filter[FilterBank].Mode == FILTER_MODE_MASK){
				CAN_FilterInitStructure.FilterMode = CAN_FILTERMODE_IDMASK;
			}else{
				CAN_FilterInitStructure.FilterMode = CAN_FILTERMODE_IDLIST;
			}

			if(Dixom.Can.Filter[FilterBank].Scale == FILTERSCALE_16BIT){
				CAN_FilterInitStructure.FilterScale = CAN_FILTERSCALE_16BIT;
			}else{
				CAN_FilterInitStructure.FilterScale = CAN_FILTERSCALE_32BIT;
			}

			if(Dixom.Can.Filter[FilterBank].IdType == ID_TYPE_STANDARD){

				CAN_FilterInitStructure.FilterIdHigh = (uint16_t)Dixom.Can.Filter[FilterBank].IdHigh<<5;
				CAN_FilterInitStructure.FilterIdLow = (uint16_t)Dixom.Can.Filter[FilterBank].IdLow<<5;
				CAN_FilterInitStructure.FilterMaskIdHigh = (uint16_t)Dixom.Can.Filter[FilterBank].MaskIdHigh<<5;
				CAN_FilterInitStructure.FilterMaskIdLow = (uint16_t)Dixom.Can.Filter[FilterBank].MaskIdLow<<5;

			}else{

				CAN_FilterInitStructure.FilterIdHigh = (uint16_t)Dixom.Can.Filter[FilterBank].IdHigh>>13;
				CAN_FilterInitStructure.FilterIdLow = (uint16_t)Dixom.Can.Filter[FilterBank].IdLow<<3 | 0b00000100;
				CAN_FilterInitStructure.FilterMaskIdHigh = (uint16_t)Dixom.Can.Filter[FilterBank].MaskIdHigh>>13;
				CAN_FilterInitStructure.FilterMaskIdLow = (uint16_t)Dixom.Can.Filter[FilterBank].MaskIdLow<<3 | 0b00000100;

			}

			if(HAL_CAN_ConfigFilter(&hcan1, &CAN_FilterInitStructure) == WR_OK){

			}

		}else{
			CAN_FilterInitStructure.FilterActivation = DISABLE;
		}
	}
	return WR_OK;
}


WorkRes SetCanFilter(){

	if(Dixom.Can.Status == Connected){
		return WR_ERROR;
		/*if(SetCanStop() != WR_OK){
			return WR_ERROR;
		}else{
			return SetCanStart();
		}*/
	}else{
		return UpdateCanFilter();
	}
}




WorkRes SetCanSpeed(uint8_t Speed){

	if(Dixom.Can.Status != Connected){

		return SetCanSpeedReg(CAN_BIT_TIME[Speed*3], CAN_BIT_TIME[Speed*3+1],  CAN_BIT_TIME[Speed*3+2]);

	}else{
		return WR_ERROR;
	}
}


WorkRes SetCanSpeedReg(uint8_t Prescaler, uint8_t Seg1,  uint8_t Seg2){

	hcan1.Init.Prescaler = Prescaler;

	if(Seg1 == 12){
		hcan1.Init.TimeSeg1 = CAN_BS1_12TQ; //0xb0000
	}else if(Seg1 == 13){
		hcan1.Init.TimeSeg1 = CAN_BS1_13TQ; //0xc0000
	}else if(Seg1 == 11){
		hcan1.Init.TimeSeg1 = CAN_BS1_11TQ; //0xa0000
	}

	/* Set the bit timing register */
	WRITE_REG(hcan1.Instance->BTR,
			  (uint32_t)(hcan1.Init.Mode |
			  hcan1.Init.SyncJumpWidth  |
			  hcan1.Init.TimeSeg1       |
			  hcan1.Init.TimeSeg2       |
	          (hcan1.Init.Prescaler - 1U)));

	return WR_OK;
}

WorkRes SetCanStart(){
	if(Dixom.Can.Status != Connected){
		Dixom.Can.Status = Connecting;
		if(UpdateCanFilter() == WR_OK){
			if(SetCanSpeed(Dixom.Can.Settings[CanSpeed].param)== WR_OK){
				if(HAL_CAN_Start(&hcan1)==WR_OK){
					if(HAL_CAN_ActivateNotification(&hcan1, CAN_IT_RX_FIFO0_MSG_PENDING | CAN_IT_TX_MAILBOX_EMPTY) == WR_OK){
						Dixom.Can.Status = Connected;
						return WR_OK;
					}else{
						return WR_ERROR;
					}
				}else{
					return WR_ERROR;
				}
			}else{
				return WR_ERROR;
			}
		}else{
			return WR_ERROR;
		}
	}else{
		return WR_ERROR;
	}
}

WorkRes SetCanStop(){

	if(Dixom.Can.Status != NotConnected){
		if(HAL_CAN_DeactivateNotification(&hcan1, CAN_IT_RX_FIFO0_MSG_PENDING | CAN_IT_TX_MAILBOX_EMPTY) == WR_OK){
			if(HAL_CAN_Stop(&hcan1)==WR_OK){
				Dixom.Can.Status = NotConnected;
				return WR_OK;
			}else{
				return WR_ERROR;
			}
		}else{
			return WR_ERROR;
		}
	}else{
		return WR_ERROR;
	}
}


void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan){

	if( HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &pRxHeader, canRX_data)){
		Error_Handler();
	}else{

		if(pRxHeader.IDE == CAN_ID_STD){
			Receiver_CAN1(0, pRxHeader.DLC, pRxHeader.StdId, canRX_data);
		}else{
			Receiver_CAN1(1, pRxHeader.DLC, pRxHeader.ExtId, canRX_data);
		}
	}
}

void HAL_CAN_TxMailbox0CompleteCallback(CAN_HandleTypeDef *hcan){

}

WorkRes Transmit_CAN1(uint8_t Rtr, uint8_t IdType, uint8_t Dlc, uint32_t Id, uint8_t Data[]){

	if(IdType == Standard){
		pTxHeader.IDE = CAN_ID_STD;
		pTxHeader.StdId = Id;
	}else{
		pTxHeader.IDE = CAN_ID_EXT;
		pTxHeader.ExtId = Id;
	}

	if(Rtr == 0){
		pTxHeader.RTR = CAN_RTR_DATA;
	}else{
		pTxHeader.RTR = CAN_RTR_REMOTE;
	}

	pTxHeader.DLC = Dlc;
	TxMailbox = 0;

	return ExchangeStatus(HAL_CAN_AddTxMessage(&hcan1, &pTxHeader, Data, &TxMailbox), CAN_SEND);

}



