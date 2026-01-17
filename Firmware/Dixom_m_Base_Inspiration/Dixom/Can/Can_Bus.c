/*
 * Can.c
 *
 *  Created on: Jun 3, 2022
 *      Author: 79825
 */
#include "Structures.h"
#include "DIX_can.h"
#include "Exchange.h"
#include <Memory_Stm.h>
#include "Can_Constant.h"
#include "Description.h"
#include "Can_Action.h"

extern sDixom Dixom;

#define dCan Dixom.Can

uint8_t CAN_Initialization_Final(){

	dCan.MsgOut.Buffer[0][0] = '3';
	dCan.MsgOut.Buffer[0][1] = ' ';
	dCan.MsgOut.Buffer[0][2] = '0';
	dCan.MsgOut.Buffer[0][3] = ' ';
	dCan.MsgOut.Buffer[0][4] = '0';
	dCan.MsgOut.Buffer[0][5] = ' ';

	dCan.MsgOut.FrameInfo.MsgCount = 0;
	dCan.MsgOut.Len = 6;
	Dixom.Can.Status = NotConnected;

	Dixom.Can.ActionQueue.RxPointer = 0;
	Dixom.Can.ActionQueue.TxPointer = 0;

	UpdateCarFunctionPointers();


	if(Dixom.Can.Settings[CanAutoConnect].param == ON){
		return SetCanStart();
	}else{
		return WR_OK;
	}
}


uint8_t CAN_Struct_Flash_Read(){

	FlashReadCRC_Check(&Dixom.Service.CheckSum.RX, FLASH_ADDR_CAN, (uint32_t *)&dCan, CanStructSize()/4);

	if(Dixom.Service.CheckSum.RX == dCan.CheckSum){
		Dixom.Service.Status.Struct[CAN_enum] = ReadyForWork;
		return WR_OK;
	}else{
		CAN_Struct_Default();
		TransmitDataOutputs((uint8_t*)CanNotifcation_3_1_4, AutoLenght,  false,  true, allAvailable);
		return WR_ERROR;
	}
}

uint8_t Can_Struct_Flash_Write(){
	FlashWriteCRC_Check( &dCan.CheckSum, FLASH_ADDR_CAN, (void *)&dCan, CanStructSize()/4);
	return CAN_Struct_Flash_Read();
}





void CAN_Show_Settings(uint8_t ShowType){
    for (short point = 0; point<NUM_ELEM_CAN_SETTINGS; point++){
    	if(dCan.Settings[point].param !=DEFAULT_CAN_SETTINGS[point] || ShowType == All){
			sprintf((char*)Dixom.Buff, CanSettings_3_2, point, dCan.Settings[point].param);
	    	TransmitDataOutputs(Dixom.Buff, AutoLenght,  false,  true, allAvailable);
    	}
    }
}

void Can_Settings_Default(){
    for (short point = 0; point<NUM_ELEM_CAN_SETTINGS; point++){
    	dCan.Settings[point].param = DEFAULT_CAN_SETTINGS[point];
    }
}

uint8_t CAN_Struct_Default(){

	Dixom.Service.Status.Struct[CAN_enum] = DefaultStruct;

	Save_Structure_When_Power_Off();

	Can_Settings_Default();

    for (short point = 0; point<NUM_ELEM_FILER; point++){
    	dCan.Filter[point].IdHigh = 0;
    	dCan.Filter[point].IdLow = 0;
    	dCan.Filter[point].MaskIdHigh = 0;
    	dCan.Filter[point].MaskIdLow = 0;
    	dCan.Filter[point].Mode = FILTER_MODE_MASK;
    	dCan.Filter[point].Scale = FILTERSCALE_16BIT;
    	dCan.Filter[point].IdType = ID_TYPE_STANDARD;
    	dCan.Filter[point].Activation = NO;
    }

	dCan.Filter[0].Activation = YES;

    dCan.Car.Manufacturer = 0;
    dCan.Car.Model = 0;
    dCan.Car.Generation = 0;
    dCan.Car.Year = 0;
    dCan.Car.DriveUnit = 0;
    dCan.Car.HandlebarSide = 0;
    dCan.Car.Transmission = 0;



    UpdateCarFunctionPointers();
	TransmitDataOutputs((uint8_t*)CanNotifcation_3_1_3, AutoLenght,  false,  true, allAvailable);
	return WR_OK;
}

uint16_t CanStructSize(){

	uint16_t size;
	size = sizeof(dCan.Settings);
	size = size + sizeof(dCan.Filter);
	size = size + sizeof(dCan.Car);
	return size+4;
}

void Can_Interaction_Loop(){

	while(dCan.ActionQueue.TxPointer != dCan.ActionQueue.RxPointer){
		Analog_ActionCH(5, dCan.ActionQueue.Button[dCan.ActionQueue.RxPointer].ButtonID, dCan.ActionQueue.Button[dCan.ActionQueue.RxPointer].Pressed);

		dCan.ActionQueue.RxPointer++;
		if(dCan.ActionQueue.RxPointer >= CAN_ACTION_QUEUE_COUNT){
			dCan.ActionQueue.RxPointer = 0;
		}
		Delay(1, FreeRTOS);
	}
}





su32_to_u8 u32_to_u8;
su16_to_u8 u16_to_u8;

const uint8_t buffSize = 60;
const uint8_t buffCount = 40;
//static uint8_t canFrame[buffCount][buffSize];
uint8_t canRxPtr = 0;
uint8_t canTxPtr = 0;

void canTxLoop(){

}
void canRxLoop(){
	while(canRxPtr!= canTxPtr){
		TransmitDataOutputs(&Dixom.Can.MsgOut.Buffer[canRxPtr][0], Dixom.Can.MsgOut.Buffer[canRxPtr][59],  true,  false, allAvailable);
		canRxPtr++;
		if(canRxPtr >= buffCount){
			canRxPtr = 0;
		}
	}
}


void Receiver_CAN1(uint8_t msgType, uint8_t dlc, uint32_t id, uint8_t *data){

	dCan.Car.Button(msgType, dlc, id, data);
	dCan.Car.Climate(msgType, dlc, id, data);
	dCan.Car.Dashboard(msgType, dlc, id, data);

	if(dCan.Settings[CanDataOutPermision].param == YES){

		dCan.MsgOut.FrameInfo.Dlc = dlc;
		dCan.MsgOut.FrameInfo.FrameIdType = msgType;
		dCan.MsgOut.FrameInfo.MsgCount++;

		Dixom.Can.MsgOut.Buffer[canTxPtr][dCan.MsgOut.Len] = Dixom.Can.MsgOut.FrameInfo.Byte;

		Dixom.Can.MsgOut.Buffer[canTxPtr][0] = '3';
		Dixom.Can.MsgOut.Buffer[canTxPtr][1] = ' ';
		Dixom.Can.MsgOut.Buffer[canTxPtr][2] = '0';
		Dixom.Can.MsgOut.Buffer[canTxPtr][3] = ' ';
		Dixom.Can.MsgOut.Buffer[canTxPtr][4] = '0';
		Dixom.Can.MsgOut.Buffer[canTxPtr][5] = ' ';

		if(msgType == ID_TYPE_STANDARD){
			u16_to_u8.ui16 = id;
			dCan.MsgOut.Len++;
			Dixom.Can.MsgOut.Buffer[canTxPtr][dCan.MsgOut.Len] = u16_to_u8.ui8[0];
			dCan.MsgOut.Len++;
			Dixom.Can.MsgOut.Buffer[canTxPtr][dCan.MsgOut.Len] = u16_to_u8.ui8[1];
		}else{
			u32_to_u8.ui32 = id;
			dCan.MsgOut.Len++;
			Dixom.Can.MsgOut.Buffer[canTxPtr][dCan.MsgOut.Len] = u32_to_u8.ui8[0];
			dCan.MsgOut.Len++;
			Dixom.Can.MsgOut.Buffer[canTxPtr][dCan.MsgOut.Len] = u32_to_u8.ui8[1];
			dCan.MsgOut.Len++;
			Dixom.Can.MsgOut.Buffer[canTxPtr][dCan.MsgOut.Len] = u32_to_u8.ui8[2];
			dCan.MsgOut.Len++;
			Dixom.Can.MsgOut.Buffer[canTxPtr][dCan.MsgOut.Len] = u32_to_u8.ui8[3];
		}

		for(uint8_t i = 0; i < dlc; i++){
			if(dCan.MsgOut.Len < sizeof(Dixom.Can.MsgOut.Buffer)){
				dCan.MsgOut.Len++;
				Dixom.Can.MsgOut.Buffer[canTxPtr][dCan.MsgOut.Len] = data[i];
			}
		}

		if(dCan.MsgOut.FrameInfo.MsgCount >= dCan.Settings[CanMsgCount].param || dCan.MsgOut.Len > 46){

			Dixom.Can.MsgOut.FrameInfo.Byte = Dixom.Can.MsgOut.Buffer[canTxPtr][6];
			Dixom.Can.MsgOut.Buffer[canTxPtr][6] = Dixom.Can.MsgOut.FrameInfo.Byte;


			Dixom.Can.MsgOut.Buffer[canTxPtr][59] = dCan.MsgOut.Len+1;
			canTxPtr++;
			if(canTxPtr >= buffCount){
				canTxPtr = 0;
			}
			dCan.MsgOut.FrameInfo.MsgCount = 0;
				dCan.MsgOut.Len = 6;

		}else{
			dCan.MsgOut.Len++;
		}
	}
}






