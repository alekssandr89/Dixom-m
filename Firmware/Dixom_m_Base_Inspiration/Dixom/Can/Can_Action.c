/*
 * Can_Action.c
 *
 *  Created on: 11 θών. 2022 γ.
 *      Author: 79825
 */
#include "Structures.h"


void CAN_Action(uint8_t msgType, uint8_t dlc, uint32_t id, uint8_t *data){

	Analog_ActionCH(0, 1000, 0);

}
