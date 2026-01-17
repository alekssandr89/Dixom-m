#include <Description.h>
#include <Exchange.h>
#include <Structures.h>
#include <Memory_Eeprom.h>
#include <Memory_Stm.h>

sDixom Dixom;

// --At startup, Functions are called in strict order (1 to 4)--

// --1--
WorkRes Structures_Initialization_From_Flash (){

	WorkRes WorkRes = WR_ERROR;

	WorkRes = Settings_Struct_Flash_Read();
	WorkRes = DSP_Struct_Flash_Read();
	WorkRes = Control_Structures_Initialization_From_Flash();
	WorkRes = CAN_Struct_Flash_Read();

	return WorkRes;
}

// --2--
WorkRes Structures_Initialization_From_EEPROM(){

	WorkRes WorkRes = WR_ERROR;

	Receiver_EEPROM( EEPROM_ADDR_PLATFORM_LIFE, (uint8_t *)&Dixom.DevInfo.PlatormLife, sizeof(Dixom.DevInfo.PlatormLife), EEPROM_TIME_OUT );

	WorkRes = DSP_Struct_EEPROM_Read();
	WorkRes = Settings_Struct_EEPROM_Read();
	WorkRes = FM_Radio_EEPROM_Read();
	WorkRes = Bluetooth_EEPROM_Read();

	return WorkRes;
}


// --3--
WorkRes Structures_Initialization_Final(){

	WorkRes WorkRes = WR_ERROR;

	WorkRes = Init_StmFlash();
	WorkRes = Settings_Initialization_Final();
	WorkRes = DSP_Module_Initialization_Final();
	WorkRes = Control_Structures_Initialization_Final();
	WorkRes = CAN_Initialization_Final();

	return WorkRes;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////

void Save_Structure_When_Power_Off()
{
	Dixom.Service.NeedToSave = Save;
}

WorkRes Structures_Write_To_Flash(){

	WorkRes WorkRes = WR_ERROR;
	Dixom.Service.Status.Firmaware = DataSave;
	Dixom.DevInfo.InMode = DataSave;

	Init_StmFlash();

	if(Dixom.Service.NeedToSave == Save){
		Dixom.Service.NeedToSave = Not_Save;
		WorkRes = WR_OK;;
		WorkRes = Flash_Unlock();
		if(WorkRes == WR_OK){
			WorkRes = Flash_Erase(4, 1);
			if(WorkRes == WR_OK){
				Dixom.DevInfo.PlatormLife.Num_Flash_Write = Dixom.DevInfo.PlatormLife.Num_Flash_Write+1;
				WorkRes = Settings_Struct_Flash_Write();
				WorkRes = DSP_Struct_Flash_Write();
				WorkRes = Control_Structures_Write_To_Flash();
				WorkRes = Can_Struct_Flash_Write();
			}
			WorkRes = Flash_Lock();
		}
		Dixom.Service.Status.Firmaware = ReadyForWork;
		Dixom.DevInfo.InMode = ReadyForWork;

		if(WorkRes == WR_OK){
 	 	    TransmitDataOutputs((uint8_t *)Basis_Notification_1_1_11_1_0, AutoLenght,  false,  true, allAvailable);

		}else{
 	 	    TransmitDataOutputs((uint8_t *)Basis_Notification_1_1_11_1_1, AutoLenght,  false,  true, allAvailable);

		}
	}
	return WorkRes;
}


WorkRes Structures_Write_To_EEPROM(){

	Dixom.DevInfo.InMode = DataSave;
	uint8_t WorkRes = WR_ERROR;

	Dixom.DevInfo.PlatormLife.Num_Eeprom_Write = Dixom.DevInfo.PlatormLife.Num_Eeprom_Write+1;

	Transmit_EEPROM( EEPROM_ADDR_PLATFORM_LIFE, (uint8_t *)&Dixom.DevInfo.PlatormLife, sizeof(Dixom.DevInfo.PlatormLife), EEPROM_TIME_OUT );

	WorkRes = DSP_Struct_EEPROM_Write();
	WorkRes = Settings_Struct_EEPROM_Write();
	WorkRes = FM_Radio_EEPROM_Write();
	WorkRes = Bluetooth_EEPROM_Write();

	Dixom.DevInfo.InMode = ReadyForWork;

	if(WorkRes == WR_OK){

		TransmitDataOutputs((uint8_t *)Basis_Notification_1_1_11_0_0, AutoLenght,  false,  true, allAvailable);

 	}else{
		TransmitDataOutputs((uint8_t *)Basis_Notification_1_1_11_0_1, AutoLenght,  false,  true, allAvailable);

 	}
	return WorkRes;
}

void SET_DEFAULT_STRUCTURES(uint8_t ControlByte, uint8_t *Received_String){
	uint8_t   Structures_enum  = All_enum;
	sscanf((char *)Received_String, "%hhu ", &Structures_enum);

	if(ControlByte == DATA_SET){ //SET
		Structures_Default(Structures_enum);
	}else{
 		TransmitDataOutputs((uint8_t *)GeneralNotifi_CtlBitNotSupported, AutoLenght,  false,  true, allAvailable);

    }
}


WorkRes Structures_Default(uint8_t Structures_enum){

	uint8_t WorkRes = WR_ERROR;
	Save_Structure_When_Power_Off();

	switch(Structures_enum){

	case All_enum:
		WorkRes = Settings_Struct_Default(NO);
		WorkRes = Control_Structures_Default(NO);
		WorkRes = DSP_Struct_Default(NO);
		WorkRes = DSP_Chip_Init(NO);
		DSP_SetVolControl(VolumControlOnLoudness);
		WorkRes = FM_Radio_Default();
		WorkRes = CAN_Struct_Default();
		WorkRes = Bluetooth_Settings_Default();
		break;

	case Settings_enum:
		WorkRes = Settings_Struct_Default(NO);
		break;

	case Control_enum:
		WorkRes = Control_Structures_Default(NO);
		break;

	case Module_DSP_enum:
		WorkRes = DSP_Struct_Default(NO);
		WorkRes = DSP_Chip_Init(NO);
		DSP_SetVolControl(VolumControlOnLoudness);
		break;

	case Module_FmRadio_enum:
		WorkRes = FM_Radio_Default();
		break;
	case CAN_enum:
		WorkRes = CAN_Struct_Default();
		break;
	case BLUETOOTH_enum:
		WorkRes = Bluetooth_Settings_Default();
		break;
	}

	if(WorkRes == WR_OK){
  		TransmitDataOutputs((uint8_t *)Basis_Notification_1_8_0, AutoLenght,  false,  true, allAvailable);

	}else{
  		TransmitDataOutputs((uint8_t *)Basis_Notification_1_8_1, AutoLenght,  false,  true, allAvailable);

	}
	return WorkRes;
}




void Loading(uint8_t Func, uint8_t status){

	if(status == WR_OK){
		Dixom.Service.Loading[Func] = YES;
	}else{
		Dixom.Service.Loading[Func] = NO;
	}
}


