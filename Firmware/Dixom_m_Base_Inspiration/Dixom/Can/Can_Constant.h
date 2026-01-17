/*
 * Can_Constant.h
 *
 *  Created on: 8 ���. 2022 �.
 *      Author: 79825
 */

#ifndef CAN_CAN_CONSTANT_H_
#define CAN_CAN_CONSTANT_H_

#include "Constants.h"

#define NUM_ELEM_CAN_SETTINGS  24
#define NUM_ELEM_CAN_BIT_TIME  9


typedef enum
{
	CAN_SPEED_10kBit             =0,
	CAN_SPEED_20kBit             =1,
	CAN_SPEED_50kBit             =2,
	CAN_SPEED_100kBit            =3,
	CAN_SPEED_125kBit            =4,
	CAN_SPEED_250kBit            =5,
	CAN_SPEED_500kBit            =6,
	CAN_SPEED_800kBit            =7,
	CAN_SPEED_1000kBit           =8,
	CAN_SPEED_Custom             =9
}tCanSpeed;

static const uint16_t  DEFAULT_CAN_SETTINGS[NUM_ELEM_CAN_SETTINGS] = {
		7,   // 0  Maximum number of messages in one batch
		CAN_SPEED_100kBit,   // 1  CanSpeed default 100kBit/s
		ON, // 2  Can tx permision
		OFF, // 3  Auto connect 0 no, 1 yes
		OFF, // 4  Can data out permision
		0, // 5
		1, // 6
		0, // 7
		1, // 8
		0, // 9
		0, // 10
		0, // 11
		0, // 12
		0, // 13
		0, // 14
		0, // 15
		0, // 16
		0, // 17
		0, // 18
		0, // 19
		0, // 20
		0, // 21
		0, // 22
		0, // 23
};




static const uint16_t  CAN_BIT_TIME[NUM_ELEM_CAN_BIT_TIME*3] = {

// Pre-scaler    Segment 1   Segment 2
   280,            12,          2,  // 10 kBit/s
   140,            12,          2,  // 20 kBit/s
    56,            12,          2,  // 50 kBit/s
    28,            12,          2,  // 100 kBit/s
    21,            13,          2,  // 125 kBit/s
    12,            11,          2,  // 250 kBit/s
     6,            11,          2,  // 500 kBit/s
     4,            11,          2,  // 800 kBit/s
     3,            11,          2,  // 1000 kBit/s
};


#endif /* CAN_CAN_CONSTANT_H_ */
