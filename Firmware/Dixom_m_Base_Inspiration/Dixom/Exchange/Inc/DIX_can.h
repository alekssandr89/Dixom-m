/*
 * DIX_can.h
 *
 *  Created on: Jun 3, 2022
 *      Author: 79825
 */

#ifndef EXCHANGE_INC_DIX_CAN_H_
#define EXCHANGE_INC_DIX_CAN_H_



WorkRes UpdateCanFilter();
WorkRes SetUpdateCanFilter();
WorkRes SetCanFilter();
WorkRes SetCanSpeed(uint8_t Speed);
WorkRes SetCanSpeedReg(uint8_t Prescaler, uint8_t Seg1,  uint8_t Seg2);
WorkRes SetCanStart();
WorkRes SetCanStop();
WorkRes Transmit_CAN1(uint8_t Rtr, uint8_t IdType, uint8_t Dlc, uint32_t Id, uint8_t Data[]);

typedef enum
{
	Standard          = 0,
	Extended          = 1,
}eIdType;


typedef enum
{
	KBIT_10          = 0,
	KBIT_20          = 1,
	KBIT_50          = 2,
	KBIT_100         = 3,
	KBIT_125         = 4,
	KBIT_250         = 5,
	KBIT_500         = 6,
	KBIT_800         = 7,
	KBIT_1000        = 8,
}eCanSpeed;






#endif /* EXCHANGE_INC_DIX_CAN_H_ */
