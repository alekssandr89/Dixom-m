/*
 * Can_StringControl.c
 *
 *  Created on: 8 ���. 2022 �.
 *      Author: 79825
 */
#include "Structures.h"
#include "Can_StringControl.h"
#include "Description.h"
#include "Can_Constant.h"
#include "DIX_can.h"
#include "Car.h"

extern sDixom Dixom;
uint8_t  TX_data[8];

///////////////////////////////////////////////////////    PARAMETERS CAN    ///////////////////////////////////////////////////////////////////////////////////

void CAN_STRING_CONTROL(uint8_t ControlByte, uint8_t Function, uint8_t *Received_String){ // 3 -

	if(Function == CAN_MESSAGE)             {  CAN_SendMessage(ControlByte, Received_String);        	      } // OK Not save
	else if(Function == CAN_SETTINGS)       {  CAN_Settings(ControlByte, Received_String);                  } // OK Save FLASH
	else if(Function == CAN_STATUS)         {  CAN_Status(ControlByte, Received_String);                      } // OK Save FLASH
	else if(Function == CAN_SPEED)          {  CAN_Speed(ControlByte, Received_String);                       } // OK Save FLASH
	else if(Function == CAN_FILTER)         {  CAN_Filter(ControlByte, Received_String);                    } // OK Save FLASH
	else if(Function == CAN_CAR)            {  CAN_Car(ControlByte, Received_String);                         } // OK Save FLASH
	else{
 		TransmitDataOutputs((uint8_t *)GeneralNotifi_NoPackageFunction, AutoLenght,  false,  true, allAvailable);

	}
}

void CAN_SHOW_PARAMETERS(uint8_t ShowType){

	CAN_Show_Settings(ShowType);
	CAN_Status(DATA_GET, 0);
	CAN_Car(DATA_GET, 0);
}


void CAN_Flash_NeedToSave(){
	Save_Structure_When_Power_Off();
}

void CAN_SendMessage(uint8_t ControlByte, uint8_t *Received_String){

	if(ControlByte == DATA_SET){ //SET
		uint8_t Rtr = Received_String[0];
		uint8_t IdType = Received_String[1];
		uint8_t DLC = Received_String[2];
		uint32_t ID = Received_String[3]  |(Received_String[4] << 8) |(Received_String[5] << 16) |(Received_String[6] << 24);
		static uint8_t Data[7];

		for(uint8_t i = 0; i<8; i++){
			Data[i] = Received_String[i+8];
		}
		//uint8_t Period = Received_String[15];
		Transmit_CAN1(Rtr, IdType, DLC, ID, Data);
	}else{
 		TransmitDataOutputs((uint8_t *)GeneralNotifi_CtlBitNotSupported, AutoLenght,  false,  true, allAvailable);

	}
}


void CAN_Settings(uint8_t ControlByte, uint8_t *Received_String){

	uint8_t data = 0;
	uint16_t param = 0;
	sscanf((char *)Received_String, "%hu %hhu", &param, &data);

	if( param<0 || param>NUM_ELEM_CAN_SETTINGS ){
 		TransmitDataOutputs((uint8_t *)GeneralNotifi_CommandError, AutoLenght,  false,  true, allAvailable);

	}else{
		if(ControlByte == DATA_SET){ //SET
			if( data<0 || data>255 ){
				Transmit_Data_Out((uint8_t *)GeneralNotifi_CommandError, AutoLenght, NO, FreeRTOS, YES);
			}else{

				Dixom.Can.Settings[param].param = data;

		    	if(param == CanSpeed){
		    		CAN_Settings_Speed();
		    	}else{

		    	}
				DSP_Flash_NeedToSave();
			}
		}else if(ControlByte == DATA_GET_MODIFIED){
			CAN_Show_Settings(Modified);
		}else if(ControlByte == DATA_GET_ALL){
			CAN_Show_Settings(All);
		}else if(ControlByte == DATA_DEFAULT){
			Can_Settings_Default();
		}else if(ControlByte == DATA_GET){
        	sprintf( (char *) Dixom.Buff, CanSettings_3_2, param,  Dixom.Can.Settings[param].param);
     		TransmitDataOutputs(Dixom.Buff, AutoLenght,  false,  true, allAvailable);
		}else{
	 		TransmitDataOutputs((uint8_t *)GeneralNotifi_CtlBitNotSupported, AutoLenght,  false,  true, allAvailable);

		}
	}
}

void CAN_Settings_Speed(){

	uint8_t oldState = Dixom.Can.Status;
	uint8_t status = WR_OK;

	if(Dixom.Can.Status == Connected ){
		status = SetCanStop();
	}

	if(status == WR_OK && oldState == Connected){
		status = SetCanStart();
	}

	if(status == WR_OK){
		sprintf( (char *) Dixom.Buff, CanSpeed_3_4, Dixom.Can.Settings[CanSpeed].param);
 		TransmitDataOutputs(Dixom.Buff, AutoLenght,  false,  true, allAvailable);

	}else{
 		TransmitDataOutputs((uint8_t*)CanNotifcation_3_1_2, AutoLenght,  false,  true, allAvailable);
	}
}


void CAN_Status(uint8_t ControlByte, uint8_t *Received_String){

	 if(ControlByte == DATA_SET){ //SET

		 uint8_t   status  = 0;
		 sscanf((char *)Received_String, "%hhu", &status);

		 if( status != Disconnecting && status != Connecting ){
			 TransmitDataOutputs((uint8_t *)GeneralNotifi_CommandError, AutoLenght,  false,  true, allAvailable);
		 }else{
			 if(Dixom.Can.Status == Connecting && status != Disconnecting){
				 TransmitDataOutputs((uint8_t *)CanNotifcation_3_1_5, AutoLenght,  false,  true, allAvailable);
				 return;
			 }else if( Dixom.Can.Status == Connected && status != Disconnecting){
				 GetCanStatus();
				 return;
			 }

			 uint8_t WorkState;
			 if(status == Connecting){
				 WorkState = SetCanStart();
				 if(WorkState == WR_ERROR){
					 Dixom.Can.Status = NotConnected;
					 TransmitDataOutputs((uint8_t *)CanNotifcation_3_1_0, AutoLenght,  false,  true, allAvailable);
				 }
			 }else{
				 WorkState = SetCanStop();
				 if(WorkState == WR_ERROR){
					 TransmitDataOutputs((uint8_t *)CanNotifcation_3_1_1, AutoLenght,  false,  true, allAvailable);
				 }
			 }
			 GetCanStatus();
			 DSP_Flash_NeedToSave();
		 }
	 }else if(ControlByte == DATA_GET){
		 GetCanStatus();
	 }else{
		 TransmitDataOutputs((uint8_t *)GeneralNotifi_CtlBitNotSupported, AutoLenght,  false,  true, allAvailable);
	 }
}


void GetCanStatus(){
	 sprintf( (char *) Dixom.Buff, CanStatus_3_3,
			 Dixom.Can.Status,
			 Dixom.Can.Settings[CanMsgCount].param,
			 Dixom.Can.Settings[CanSpeed].param,
			 Dixom.Can.Settings[CanTxPermision].param,
			 Dixom.Can.Settings[CanAutoConnect].param,
			 Dixom.Can.Settings[CanDataOutPermision].param,
			 Dixom.Can.Settings[Reserved0].param,
			 Dixom.Can.Settings[Reserved0].param,
			 Dixom.Can.Settings[Reserved0].param);
	 TransmitDataOutputs((uint8_t *)Dixom.Buff, AutoLenght,  false,  true, allAvailable);

}
/*CanMsgCount            =0,
CanSpeed               =1,
CanTxPermision         =2,
CanAutoConnect         =3,
CanDataOutPermision    =4,*/

void CAN_Speed(uint8_t ControlByte, uint8_t *Received_String){

	if(ControlByte == DATA_SET){ //SET

		uint8_t   speed  = 0;
		sscanf((char *)Received_String, "%hhu", &speed);

		if( speed<0 || speed>8 ){
			TransmitDataOutputs((uint8_t *)GeneralNotifi_CommandError, AutoLenght,  false,  true, allAvailable);

		}else{

			Dixom.Can.Settings[CanSpeed].param = speed;

			uint8_t oldState = Dixom.Can.Status;
			uint8_t status = WR_OK;

			if(Dixom.Can.Status == Connected ){
				status = SetCanStop();
			}

			if(status == WR_OK && oldState == Connected){
				status = SetCanStart();
			}

			if(status == WR_OK){
				sprintf( (char *) Dixom.Buff, CanSpeed_3_4, Dixom.Can.Settings[CanSpeed].param);
				TransmitDataOutputs(Dixom.Buff, AutoLenght,  false,  true, allAvailable);

			}else{
				TransmitDataOutputs((uint8_t *)CanNotifcation_3_1_2, AutoLenght,  false,  true, allAvailable);

			}
			DSP_Flash_NeedToSave();
		}
	}else if(ControlByte == DATA_GET){
		sprintf( (char *) Dixom.Buff, CanSpeed_3_4, Dixom.Can.Settings[CanSpeed].param);
 		TransmitDataOutputs(Dixom.Buff, AutoLenght,  false,  true, allAvailable);

	}else{
 		TransmitDataOutputs((uint8_t *)GeneralNotifi_CtlBitNotSupported, AutoLenght,  false,  true, allAvailable);

	}
}





void CAN_Car(uint8_t ControlByte, uint8_t *Received_String){

	if(ControlByte == DATA_SET){ //SET
		uint8_t  Manufacturer, Model, Generation, Year, Transmission, DriveUnit, HandlebarSide;
		sscanf((char *)Received_String, "%hhu %hhu %hhu %hhu %hhu %hhu %hhu", &Manufacturer, &Model, &Generation, &Year, &Transmission, &DriveUnit, &HandlebarSide);

		Dixom.Can.Car.Manufacturer = Manufacturer;
		Dixom.Can.Car.Model = Model;
		Dixom.Can.Car.Generation = Generation;
		Dixom.Can.Car.Year = Year;
		Dixom.Can.Car.Transmission = Transmission;
		Dixom.Can.Car.DriveUnit = DriveUnit;
		Dixom.Can.Car.HandlebarSide = HandlebarSide;
		UpdateCarFunctionPointers();
		CAN_Speed(DATA_GET, (uint8_t*)"");
		CAN_Status(ControlByte, (uint8_t*)"4");
		DSP_Flash_NeedToSave();
	}else if(ControlByte == DATA_GET){
		sprintf( (char *) Dixom.Buff, CanCar_3_6_0,
				Dixom.Can.Car.Manufacturer,
				Dixom.Can.Car.Model,
				Dixom.Can.Car.Generation,
				Dixom.Can.Car.Year,
				Dixom.Can.Car.Transmission,
				Dixom.Can.Car.DriveUnit,
				Dixom.Can.Car.HandlebarSide);
 		TransmitDataOutputs(Dixom.Buff, AutoLenght,  false,  true, allAvailable);

	}else{
 		TransmitDataOutputs((uint8_t *)GeneralNotifi_CtlBitNotSupported, AutoLenght,  false,  true, allAvailable);

	}
}

void CAN_Filter(uint8_t ControlByte, uint8_t *Received_String){

	uint8_t    FilterNum = 0;
	uint32_t   IdHigh = 0;
	uint32_t   IdLow = 0;
	uint32_t   MaskIdHigh = 0;
	uint32_t   MaskIdLow = 0;
	uint8_t    IdType = 0;
	uint8_t    Scale = 0;
	uint8_t    Mode = 0;
	uint8_t    Activation = 0;

	sscanf((char *)Received_String, "%hhu %hhu %hhu %hhu %hhu %lo %lo %lo %lo", &FilterNum, &Activation, &Mode, &Scale, &IdType, &IdHigh, &IdLow, &MaskIdHigh, &MaskIdLow);

	if(FilterNum>NUM_ELEM_FILER ){

 		TransmitDataOutputs((uint8_t *)GeneralNotifi_CommandError, AutoLenght,  false,  true, allAvailable);

	}else{


		if(ControlByte == DATA_SET){ //SET
			if(Activation >1 || Mode>1 || Scale>1 || IdType>1 || IdHigh> CAN_FILTER_MAX_ID || IdLow > CAN_FILTER_MAX_ID || MaskIdHigh > CAN_FILTER_MAX_ID || MaskIdLow> CAN_FILTER_MAX_ID){

		 		TransmitDataOutputs((uint8_t *)GeneralNotifi_CommandError, AutoLenght,  false,  true, allAvailable);

			}else{

				Dixom.Can.Filter[FilterNum].Activation = Activation;
				Dixom.Can.Filter[FilterNum].Mode = Mode;
				Dixom.Can.Filter[FilterNum].Scale = Scale;
				Dixom.Can.Filter[FilterNum].IdType = IdType;
				Dixom.Can.Filter[FilterNum].IdHigh = IdHigh;
				Dixom.Can.Filter[FilterNum].IdLow = IdLow;
				Dixom.Can.Filter[FilterNum].MaskIdHigh = MaskIdHigh;
				Dixom.Can.Filter[FilterNum].MaskIdLow = MaskIdLow;

				DSP_Flash_NeedToSave();
			}
		}else if(ControlByte == DATA_GET){
			sprintf( (char *) Dixom.Buff, CanCar_3_5_0,
					FilterNum,
					Dixom.Can.Filter[FilterNum].Activation,
					Dixom.Can.Filter[FilterNum].Mode,
					Dixom.Can.Filter[FilterNum].Scale,
					Dixom.Can.Filter[FilterNum].IdType,
					Dixom.Can.Filter[FilterNum].IdHigh,
					Dixom.Can.Filter[FilterNum].IdLow,
					Dixom.Can.Filter[FilterNum].MaskIdHigh,
					Dixom.Can.Filter[FilterNum].MaskIdLow);

	 		TransmitDataOutputs( Dixom.Buff, AutoLenght,  false,  true, allAvailable);

		}else{
	 		TransmitDataOutputs((uint8_t *)GeneralNotifi_CtlBitNotSupported, AutoLenght,  false,  true, allAvailable);

		}
	}
}

void canWakeUp(){

	TX_data[0] = 0;
	TX_data[1] = 0;
	TX_data[2] = 0;
	TX_data[3] = 0;
	TX_data[4] = 0x57;
	TX_data[5] = 0x2F;
	TX_data[6] = 0;
	TX_data[7] = 0x60;

	//Transmit_CAN1(0x560, TX_data, Standard);
}

void LightOn(){

	TX_data[0] = 0xFD;
	TX_data[1] = 0x55;
	TX_data[2] = 0xFF;
	TX_data[3] = 0x10;
	TX_data[4] = 0xFF;
	TX_data[5] = 0x12;
	TX_data[6] = 0x34;
	TX_data[7] = 0x56;

//	Transmit_CAN1(0x202, TX_data, Standard);
}



