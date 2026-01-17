/*
 * Control.c
 *
 *  Created on: 23 ����. 2021 �.
 *      Author: 79825
 */
#include <Memory_Stm.h>
#include "Control.h"
#include <Structures.h>

extern sDixom Dixom;

 WorkRes Control_Structures_Initialization_From_Flash(){

	 FlashReadCRC_Check(&Dixom.Service.CheckSum.RX, FLASH_ADDR_CONTROL, (uint32_t *)&Dixom.Control, sizeof(Dixom.Control)/4);
	 Dixom.Control.StructInfo.Size = sizeof(Dixom.Control);
	 if(Dixom.Service.CheckSum.RX == Dixom.Control.CheckSum){
		 Control_Structures_Default(YES);
		 Dixom.Service.Status.Struct[Control_enum] = ReadyForWork;
		 return WR_OK;
	 }else{
		 Control_Structures_Default(NO);
		 return WR_ERROR;
	 }
 }

 WorkRes Control_Structures_Initialization_Final(){

 	AnalogControl_Initialization_Final();
 	return  WR_OK;
 }

 WorkRes Control_Structures_Write_To_Flash(){

	 FlashWriteCRC_Check( &Dixom.Control.CheckSum, FLASH_ADDR_CONTROL, (void *)&Dixom.Control, sizeof(Dixom.Control)/4);

	 FlashReadCRC_Check(&Dixom.Service.CheckSum.RX, FLASH_ADDR_CONTROL, (uint32_t *)&Dixom.Control, sizeof(Dixom.Control)/4);
	 if(Dixom.Service.CheckSum.RX == Dixom.Control.CheckSum){
		 return WR_OK;
	 }else{
		 return WR_ERROR;
	 }
 }

 WorkRes Control_Structures_Default(uint8_t OnlyEditingReset){

	 if(OnlyEditingReset == NO){
		 Dixom.Service.Status.Struct[Control_enum] = DefaultStruct;
	 }
	 setDefaultControlSettings();
	 AnalogControl_Struct_Default(OnlyEditingReset);
	 return  WR_OK;
 }


 void Control_Loop(){
	 Action_Analog_Control_Loop();
 }
