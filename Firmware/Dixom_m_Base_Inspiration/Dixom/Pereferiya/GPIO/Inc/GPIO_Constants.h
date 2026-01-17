/*
 * GPIO_Contstants.h
 *
 *  Created on: 7 февр. 2022 г.
 *      Author: 79825
 */

#ifndef PEREFERIYA_GPIO_INC_GPIO_CONSTANTS_H_
#define PEREFERIYA_GPIO_INC_GPIO_CONSTANTS_H_


typedef enum
{
	Usb1          = 0,
	Usb2          = 1,
	Vbus          = 2,
	Trigger1      = 3,
	Trigger2      = 4,
	Trigger3      = 5,
	DacReset      = 6,
	Usb_Otg       = 7,
	Cinv12v       = 8,
	Remout        = 9,
	Camera        = 10,
	CS_DSP        = 11,
	CS_FLASH      = 12,
	Led           = 13,
	PowerDCDC     = 14,
	SPI           = 15,
	Modules_Reset = 16
}enumPeripheryStatus;


#endif /* PEREFERIYA_GPIO_INC_GPIO_CONSTANTS_H_ */
