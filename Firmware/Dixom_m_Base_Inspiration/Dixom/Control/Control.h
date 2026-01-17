/*
 * Control.h
 *
 *  Created on: 23 ����. 2021 �.
 *      Author: 79825
 */

#ifndef CONTROL_CONTROL_H_
#define CONTROL_CONTROL_H_

#include "Analog.h"
#include <Control_Constant.h>

 WorkRes Control_Structures_Initialization_Final();
 WorkRes Control_Structures_Initialization_From_Flash();
 WorkRes Control_Structures_Write_To_Flash();
 WorkRes Control_Structures_Default(uint8_t OnlyEditingReset);
 void Control_Loop();

 typedef struct{
 	uint8_t param;
 } sControlSettings;



 typedef struct
 {
	uint32_t         CheckSum;
 	sStructInfo      StructInfo;                         // 3 word
 	sAnalog          Analog;
 	uint8_t          CanBus;
 	sControlSettings Settings[ANLOG_SETTINGS_COUNT];
 	uint8_t          Digital;
 	uint16_t         Reserved;
 } sControl;

#endif /* CONTROL_CONTROL_H_ */
