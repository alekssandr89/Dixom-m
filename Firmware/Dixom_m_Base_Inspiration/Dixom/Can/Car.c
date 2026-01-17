/*
 * Car.c
 *
 *  Created on: 12 ���. 2022 �.
 *      Author: 79825
 */
#include <Can_Bus.h>
#include "Can_Constant.h"
#include "Structures.h"
#include "Car.h"

extern sDixom Dixom;
#define dCan Dixom.Can

void CanNotUsed(uint8_t msgType, uint8_t dlc, uint32_t id, uint8_t *data){}

/*Reconfiguration of functions for the selected vehicle*/
void UpdateCarFunctionPointers(){

	dCan.Car.Button = CanNotUsed;
	dCan.Car.Climate = CanNotUsed;
	dCan.Car.Dashboard = CanNotUsed;

	uint8_t CarSelectedState = NO;

	/*         ---===SKODA===---            */
	if(dCan.Car.Manufacturer == SKODA){
		if(dCan.Car.Model == OCTAVIA){
			if(dCan.Car.Generation == THIRD_A7_2013){
				dCan.Car.Button = Button_skoda_octavia_A7;
				dCan.Car.Climate = Climate_skoda_octavia_A7;
				dCan.Car.Dashboard = Dashboard_skoda_octavia_A7;
				dCan.Filter[0].Activation = YES;
				dCan.Filter[0].Mode = ID_TYPE_STANDARD;
				dCan.Filter[0].Scale = FILTERSCALE_16BIT;
		    	dCan.Filter[0].Mode = FILTER_MODE_LISTID;
		    	dCan.Filter[0].IdHigh = SKODA_OCTAVIA_A7_ID_CAN[0];
		    	dCan.Filter[0].MaskIdHigh = SKODA_OCTAVIA_A7_ID_CAN[1];
		    	CarSelectedState = YES;
		    	Dixom.Can.Settings[CanSpeed].param = CAN_SPEED_500kBit;
		    	//SetCanSpeed(6, 6, 11,  2);
			}
		}
	}else{
		if(dCan.Car.Manufacturer == AUDI){
			if(dCan.Car.Model == A4){
				if(dCan.Car.Generation == SECOND_B6_2000_2006){
					dCan.Car.Button = Button_audi_A4_B6;
					dCan.Car.Climate = Climate_audi_A4_B6;
					dCan.Car.Dashboard = Dashboard_audi_A4_B6;
					dCan.Filter[0].Activation = YES;
					dCan.Filter[0].Mode = ID_TYPE_STANDARD;
					dCan.Filter[0].Scale = FILTERSCALE_16BIT;
			    	dCan.Filter[0].Mode = FILTER_MODE_LISTID;
			    	dCan.Filter[0].IdHigh = AUDI_A4_B6[0];
			    	dCan.Filter[0].MaskIdHigh = AUDI_A4_B6[1];
			    	CarSelectedState = YES;
			    	Dixom.Can.Settings[CanSpeed].param = CAN_SPEED_100kBit;
			    //	SetCanSpeed(3, 28, 12,  2);
				}
			}
		}
	}

	if(CarSelectedState == NO){
		dCan.Car.Button = CanNotUsed;
		dCan.Car.Climate = CanNotUsed;
		dCan.Car.Dashboard = CanNotUsed;

		dCan.Filter[0].Activation = YES;
		dCan.Filter[0].Mode = ID_TYPE_STANDARD;
		dCan.Filter[0].Scale = FILTERSCALE_16BIT;
    	dCan.Filter[0].Mode = FILTER_MODE_MASK;
    	dCan.Filter[0].IdHigh = 0x0000;
    	dCan.Filter[0].IdLow = 0x0000;
    	dCan.Filter[0].MaskIdHigh = 0x0000;
    	dCan.Filter[0].MaskIdLow = 0x0000;
	}
}


uint16_t Add_can_action_queue(uint16_t ButtonID, uint16_t Pressed){
	dCan.ActionQueue.Button[dCan.ActionQueue.TxPointer].ButtonID = ButtonID;
	dCan.ActionQueue.Button[dCan.ActionQueue.TxPointer].Pressed = Pressed;
	if(Pressed == YES){
		Dixom.Control.Analog.CH[5].HoldStatus = 1;
	}
	dCan.ActionQueue.TxPointer++;
	if(dCan.ActionQueue.TxPointer >= CAN_ACTION_QUEUE_COUNT){
		dCan.ActionQueue.TxPointer = 0;
	}
	return ButtonID;
}



void Button_skoda_octavia_A7(uint8_t msgType, uint8_t dlc, uint32_t id, uint8_t *data){

	static uint16_t oldButton;

	if(msgType == ID_TYPE_STANDARD){
		if(dlc == 4){
			if(id == SKODA_OCTAVIA_A7_ID_CAN[0]){
				if(data[0] == 0){
					if(oldButton != 4444){
						Add_can_action_queue(oldButton, 0);
						Add_can_action_queue(4444, 0);
						oldButton = 4444;
					}
				}else if(data[0] == 0x15){  // 0 Pressed
					oldButton = Add_can_action_queue(50, 1);
				}else if(data[0] == 0x16){ // 1 media next
					oldButton = Add_can_action_queue(100, 1);
				}else if(data[0] == 0x13){ // 3 media previus
					oldButton = Add_can_action_queue(150, 1);
				}else if(data[0] == 0x12 && data[2] == 0x01 ){// 4 media encoder
					Add_can_action_queue(200, 1);
					Add_can_action_queue(200, 0);
					Add_can_action_queue(4444, 0);
					oldButton = 4444;
				}else if(data[0] == 0x12 && data[2] == 0x0F ){// 6 media encoder down
					Add_can_action_queue(250, 1);
					Add_can_action_queue(250, 0);
					Add_can_action_queue(4444, 0);
					oldButton = 4444;
				}else if(data[0] == 0x14){// 7 media source
					oldButton = Add_can_action_queue(300, 1);
				}
			}
		}
	}
}

void Climate_skoda_octavia_A7(uint8_t msgType, uint8_t dlc, uint32_t id, uint8_t *data){

}

void Dashboard_skoda_octavia_A7(uint8_t msgType, uint8_t dlc, uint32_t id, uint8_t *data){
	if(msgType == ID_TYPE_STANDARD){
		if(dlc == 8){
			if(id == SKODA_OCTAVIA_A7_ID_CAN[2]){
				if(data[3] == 2){
					SetRearState(ON);
				}else if(data[3] == 0){
					SetRearState(OFF);
				}
			}
		}
	}
}



void Button_audi_A4_B6(uint8_t msgType, uint8_t dlc, uint32_t id, uint8_t *data){

	static uint16_t oldButton;

	if(msgType == ID_TYPE_STANDARD){
		if(dlc == 2){
			if(id == AUDI_A4_B6[0]){
				if(data[0] == 0x39 && data[1] == 0x00){
					if(oldButton != 4444){
						Add_can_action_queue(oldButton, 0);
						Add_can_action_queue(4444, 0);
						oldButton = 4444;
					}
				}else if(data[0] == 0x39 && data[1] == 0x0B){  // 0 Pressed
					oldButton = Add_can_action_queue(50, 1);
				}else if(data[0] == 0x3A && data[1] == 0x1C){ // 1 media next
					oldButton = Add_can_action_queue(100, 1);
				}else if(data[0] == 0x39 && data[1] == 0x0C){ // 3 media previus
					oldButton = Add_can_action_queue(150, 1);
				}else if(data[0] == 0x39 && data[1] == 0x08){// 4 media encoder
					Add_can_action_queue(200, 1);
					Add_can_action_queue(200, 0);
					Add_can_action_queue(4444, 0);
					oldButton = 4444;
				}else if(data[0] == 0x39 && data[1] == 0x06){// 6 media encoder down
					oldButton = Add_can_action_queue(250, 1);
					Add_can_action_queue(250, 1);
					Add_can_action_queue(250, 0);
					Add_can_action_queue(4444, 0);
					oldButton = 4444;
				}else if(data[0] == 0x3C && data[1] == 0x2A){// 7 media source
					oldButton = Add_can_action_queue(300, 1);
				}else if(data[0] == 0x39 && data[1] == 0x07){// 8 media source
					oldButton = Add_can_action_queue(350, 1);
				}else if(data[0] == 0x3B && data[1] == 0x00){// 9 media source
					oldButton = Add_can_action_queue(400, 1);
				}
			}
		}
	}
}

void Climate_audi_A4_B6(uint8_t msgType, uint8_t dlc, uint32_t id, uint8_t *data){

}

void Dashboard_audi_A4_B6(uint8_t msgType, uint8_t dlc, uint32_t id, uint8_t *data){

}




