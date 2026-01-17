#include <DSP_InternalCalls.h>
#include "Bluetooth.h"
#include "Exchange.h"
#include "Description.h"
#include <string.h>
#include "FSC_BT806.h"
#include <Structures.h>
#include <Memory_Eeprom.h>

extern sDixom Dixom;

#define dBluetooth Dixom.Module.Bluetooth


#define FSC (Dixom.Module.Bluetooth.dModel == BLUETOOTH_FSC_BT806 || Dixom.Module.Bluetooth.dModel == BLUETOOTH_FSC_BT802 || Dixom.Module.Bluetooth.dModel == BLUETOOTH_FSC_BT1006C || Dixom.Module.Bluetooth.dModel == BLUETOOTH_FSC_BT1006A)

uint32_t TimeTotal = 0;
uint32_t TimeProgress = 0;

uint8_t NetvorkSignal =0;

uint8_t BtA2dpCodec = '0';

uint8_t BtDeviceConState = '0';

void UpdteNetvorkSignal(uint32_t Signal){
	NetvorkSignal = Signal;
}


void Phone_IncomingCall(void){
	DSP_SetVolume(VolumBluetooth, MaxVolume);
}

void Phone_OutgoingCall(void){
	DSP_SetVolume(VolumUsb, DSP_GetVolume(MinVolumUsb));
	DSP_SetVolume(VolumBluetooth, MaxVolume);
	DSP_SetVolume(VolumAux, DSP_GetVolume(MinVolumAux));
	DSP_SetVolume(VolumRadio, DSP_GetVolume(MinVolumRadio));
	DSP_SetVolume(VolumSpdif, DSP_GetVolume(MinVolumSpdif));
	DSP_SetVolume(VolumSoursMic, DSP_GetVolume(MinVolumMic));
}

void Phone_CompletedCall(void){
	DSP_SoursSelect(Dixom.Module.DSP.Settings[SoundSource].Data, Dixom.Module.DSP.Settings[SoundSource].Data);
}

void Phone_ActiveCall(void){
	DSP_SetVolume(VolumUsb, DSP_GetVolume(MinVolumUsb));
	DSP_SetVolume(VolumBluetooth, MaxVolume);
	DSP_SetVolume(VolumAux, DSP_GetVolume(MinVolumAux));
	DSP_SetVolume(VolumRadio, DSP_GetVolume(MinVolumRadio));
	DSP_SetVolume(VolumSpdif, DSP_GetVolume(MinVolumSpdif));
	DSP_SetVolume(VolumSoursMic, DSP_GetVolume(MinVolumMic));
}

void BtDiviceState(uint8_t state){
	BtDeviceConState = state;
}

void SetA2dpCodec(uint8_t state){
	BtA2dpCodec = state;
}

uint32_t GetBtDiviceState(){
	return BtDeviceConState;
}


void BT_module_param_clear(){

	for(uint8_t i = 0; i < BtModuleInfoLen; i++){
		Dixom.Module.Bluetooth.Name[i] = 0;
		Dixom.Module.Bluetooth.Pass[i] = 0;
		Dixom.Module.Bluetooth.MacAddr[i] = 0;
		Dixom.Module.Bluetooth.ConnDevInfo.LastPhoneNumber[i] = 0;
		Dixom.Module.Bluetooth.ConnDevInfo.NetworkName[i] = 0;;
		Dixom.Module.Bluetooth.ConnDevInfo.Name[i] = 0;
		Dixom.Module.Bluetooth.ConnDevInfo.MacAddr[i] = 0;
	}

	Dixom.Module.Bluetooth.ConnDevInfo.PairCount = 0;
	for(uint8_t list = 0; list < BtMaxPairCount; list++){

		for(uint8_t byte = 0; byte < BtPairInfoAddrLen; byte++){
			Dixom.Module.Bluetooth.ConnDevInfo.PairListAddr[list][byte] = 0;
		}

		for(uint8_t byte = 0; byte < BtPairInfoNameLen; byte++){
			Dixom.Module.Bluetooth.ConnDevInfo.PairListName[list][byte] = 0;
		}
	}

}



void BLUETOOTH_Data_out(uint8_t* data, uint8_t dataLen, uint16_t timeDelay, uint8_t from, uint8_t param){

	TransmitDataOutputs(data, dataLen,  false,  true, allAvailable);


}

void Bluetooth_Loop(void){
	LoopBT();

}





void Initialization_Bluetooth(void){

	Dixom.Module.Bluetooth.BT_Control_Mode = Dixom.Module.Bluetooth.Settings[BtControlDirectMode].Data;
	Dixom.Module.Bluetooth.SppThroughputMode = Dixom.Module.Bluetooth.Settings[BtThroughputMode].Data;
	Dixom.Module.Bluetooth.ConnDevInfo.A2dpDecoder = BTDecoder_NO;
	Dixom.Module.Bluetooth.dModel = BLUETOOTH_FSC_BT802;

	FSC_Bluetooth_Transmit((uint8_t*) "AT+VER", AutoLenght, 0, FreeRTOS);


}



void BLUETOOTH_SETTINGS(uint8_t ControlByte, uint8_t *Received_String){
	uint8_t data = 0;
	uint16_t param = 0;
	sscanf((char *)Received_String, "%hu %hhu", &param, &data);
	Bluetooth_Settings(ControlByte, param, data);
}

WorkRes Bluetooth_EEPROM_Read(){

	uint32_t    Rx_CheckSum;

	dBluetooth.DataSaveSize = sizeof(dBluetooth.Settings);

	EEPROM_Read_CheckSumm(&Rx_CheckSum, EEPROM_ADDR_BLUETOOTH, (uint8_t *)&dBluetooth, dBluetooth.DataSaveSize, 300);

	if(Rx_CheckSum == dBluetooth.CheckSum){

		Dixom.Service.Status.Struct[BLUETOOTH_enum] = ReadyForWork;
		return WR_OK;

	}else{
		Bluetooth_Settings_Default();
		Dixom.Service.Status.Struct[BLUETOOTH_enum] = SafeMode;
		return WR_ERROR;
	}
}

WorkRes Bluetooth_EEPROM_Write(){

	WorkRes State = WR_OK;

	if(dBluetooth.NeedToSave == YES){
		dBluetooth.NeedToSave = NO;
		EEPROM_Write_CheckSumm(&dBluetooth.CheckSum, EEPROM_ADDR_BLUETOOTH, (uint8_t *)&dBluetooth, dBluetooth.DataSaveSize, 300);
	}

    return State;
}

WorkRes Bluetooth_Settings_Default(){
	for(uint8_t set = 0; set < NUM_ELEM_BT_SETTINGS; set++){
		dBluetooth.Settings[set].Data = DEFAULT_BT_SETTINGS[set];
	}
	dBluetooth.NeedToSave = YES;
	return WR_OK;
}

void Bluetooth_Settings(uint8_t ControlByte, uint8_t param, uint8_t data){

	if( param<0 || param>NUM_ELEM_BT_SETTINGS  ){
		TransmitDataOutputs((uint8_t*)GeneralNotifi_CommandError, AutoLenght,  false,  true, allAvailable);

	}else{
		if(ControlByte == DATA_SET){ //SET

			dBluetooth.Settings[param].Data = data;
			dBluetooth.NeedToSave = YES;
			if(param == BtThroughputMode){
				Structures_Write_To_EEPROM();
				TransmitDataOutputs((uint8_t*)GeneralNotifi_RebootToApply, AutoLenght,  false,  true, allAvailable);
				PlatformRestart(FreeRTOS);
			}else if(param == BtControlDirectMode){
				Dixom.Module.Bluetooth.BT_Control_Mode = data;
			}else if(param == BtBaudrate){

			}else if(param == BtDeliveryGuarantee){

			}

			TransmitDataOutputs((uint8_t*)GeneralNotifi_CommandOk, AutoLenght,  false,  true, allAvailable);


		}else if(ControlByte == DATA_GET_ALL){
		//	DSP_SETTINGS_SHOW_PARAMETERS(All);
		}else if(ControlByte == DATA_GET_MODIFIED){
		//	DSP_SETTINGS_SHOW_PARAMETERS(Modified);
		}else if(ControlByte == DATA_DEFAULT){
			Bluetooth_Settings_Default();
			DSP_EEPROM_NeedToSave();
			sprintf( (char *) Dixom.Buff, BtNotificationDefault, BT_SETTINGS);
 			TransmitDataOutputs(Dixom.Buff, AutoLenght,  false,  true, allAvailable);

		}else if(ControlByte == DATA_GET){
			sprintf( (char *) Dixom.Buff, BluetoothSettings, param,  dBluetooth.Settings[param].Data);
 			TransmitDataOutputs(Dixom.Buff, AutoLenght,  false,  true, allAvailable);

			if(param == BtThroughputMode){
				sprintf( (char *) Dixom.Buff, BtNotificationThroughput, Dixom.Module.Bluetooth.SppThroughputMode);
 				TransmitDataOutputs(Dixom.Buff, AutoLenght,  false,  true, allAvailable);

			}
		}else{
 			TransmitDataOutputs((uint8_t*)GeneralNotifi_CommandError, AutoLenght,  false,  true, allAvailable);

		}
	}

}




void BLUETOOTH_Menu(uint8_t ControlByte, uint8_t Function, uint8_t *Recive_data){

	if(Function == BT_DIRECT_ACCESS){
/*
		uint8_t dataLen = strlen((char*)Recive_data);
		uint8_t msg[dataLen+2];
		for(uint8_t i =0; i <dataLen; i++){
			msg[i] = Recive_data[i];
		}
		msg[dataLen] = '\r';
		msg[dataLen+1] = '\n';
*/
		FSC_Bluetooth_Transmit(Recive_data, AutoLenght, 0, FreeRTOS);
	}
	else if(Function == BT_SETTINGS){  BLUETOOTH_SETTINGS(ControlByte, Recive_data);   }
	else if(Function == BT_MEDIA){     MEDIA_FSC(Recive_data);                         }

	//if(Dixom.Module.Bluetooth.BT_Control_Mode == PlatformAccess || Dixom.Module.Bluetooth.BT_Control_Mode == AnyAccess ){






/*
		if(strncmp((char *)Recive_data,      "MEDIA ",               6) == 0) {
			if(Dixom.Module.Bluetooth.dModel == BLUETOOTH_WT32_iWRAP4)        {                                                 }
			else if(FSC)                                                      { MEDIA_FSC(Recive_data+6);                                                          }
		}else if(strncmp((char *)Recive_data,"PHONE",                6) == 0) {
			if(Dixom.Module.Bluetooth.dModel == BLUETOOTH_WT32_iWRAP4)        {                                                                                    }
			else if(FSC)                                                      { PHONE_FSC(Recive_data+6);                                                          }
		}
	    else if(strncmp((char *)Recive_data, "SPP ",                 4) == 0) {
	    	if(Dixom.Module.Bluetooth.dModel == BLUETOOTH_WT32_iWRAP4)        {                                                                                   }
	    	else if(FSC)                                                      { SPP_FSC(Recive_data+4);                                                                                   }
	    }
	    else if(strncmp((char *)Recive_data, "RESET TO DEFAULT",    16) == 0) {
	    	if(Dixom.Module.Bluetooth.dModel == BLUETOOTH_WT32_iWRAP4)                             {                                               }
	    	else if(Dixom.Module.Bluetooth.dModel == BLUETOOTH_FSC_BT806)                          {                                                                                    }
	    	BLUETOOTH_Data_out((uint8_t *)"BLUETOOTH DEFAULT DONE", 0, 0, FreeRTOS, NO); }
	    else if(strncmp((char *)Recive_data, "CALL ",                5) == 0) {
	    	if(Dixom.Module.Bluetooth.dModel == BLUETOOTH_WT32_iWRAP4)                             {                                                }
	    	else if(Dixom.Module.Bluetooth.dModel == BLUETOOTH_FSC_BT806)                          {                                                                                    }}
	    else if(strncmp((char *)Recive_data, "INIT ALL",             8) == 0) {
	    	if(Dixom.Module.Bluetooth.dModel == BLUETOOTH_WT32_iWRAP4)                             {                                                        }
	    	else if(Dixom.Module.Bluetooth.dModel == BLUETOOTH_FSC_BT806)                          {                                                                                    }}
	    else if(strncmp((char *)Recive_data, "DIRECT ",              7) == 0) {
	    	if(Dixom.Module.Bluetooth.dModel == BLUETOOTH_WT32_iWRAP4)                             {                       }
	    	else if(Dixom.Module.Bluetooth.dModel == BLUETOOTH_FSC_BT806)                          {                                                                                    }}

	    else if(strncmp((char *)Recive_data, "LOAD PBOOK ",         11) == 0) {
	    	if(Dixom.Module.Bluetooth.dModel == BLUETOOTH_WT32_iWRAP4)                             {                                                                                    }
	    	else if(FSC)    {
	    		DOWNLOAD_PBOOK_FSC(Recive_data+10);
	    	}}
	    else{
			if(FSC){
				FSC_Bluetooth_Transmit(Recive_data, NO, 0, FreeRTOS);
			}else if(Dixom.Module.Bluetooth.dModel == BLUETOOTH_WT32_iWRAP4){

			}
	    }*/

	/*}else if(Dixom.Module.Bluetooth.BT_Control_Mode == DirectAccrs){
		if(Function == BT_DIRECT_ACCESS){  FSC_Bluetooth_Transmit(Recive_data, AutoLenght, 0, FreeRTOS); }
	/*	if(Dixom.Module.Bluetooth.dModel == BLUETOOTH_WT32_iWRAP4){
			//BT_WT32_iWRAP4_Transmit(Recive_data, NO, 0, FreeRTOS);
		}else if(FSC){
			FSC_Bluetooth_Transmit(Recive_data, NO, 0, FreeRTOS);
		}
	}*/
}



void BluetoothAction(uint8_t action){

}



void Bluetooth_Play_Pause(void){

	if(Dixom.Module.Bluetooth.dModel == BLUETOOTH_WT32_iWRAP4){
		/*BT_WT32_iWRAP4_Transmit((uint8_t*) "AVRCP FORWARD", AutoLenght, 0, FreeRTOS);*/
	}else if(FSC){
		FSC_Bluetooth_Transmit((uint8_t *)"AT+PLAYPAUSE", NO, 0, FreeRTOS);
	}
}

void Bluetooth_Play(void){
	if(Dixom.Module.Bluetooth.dModel == BLUETOOTH_WT32_iWRAP4){
		//BT_WT32_iWRAP4_Transmit((uint8_t*) "AVRCP REWIND", AutoLenght, 0, FreeRTOS);
	}else if(FSC){
		FSC_Bluetooth_Transmit((uint8_t *)"AT+PLAY", AutoLenght, 0, FreeRTOS);
	}
}

void Bluetooth_Pause(void){
	if(Dixom.Module.Bluetooth.dModel == BLUETOOTH_WT32_iWRAP4){
		//BT_WT32_iWRAP4_Transmit((uint8_t*) "AVRCP REWIND", AutoLenght, 0, FreeRTOS);
	}else if(FSC){
		FSC_Bluetooth_Transmit((uint8_t *)"AT+PAUSE", AutoLenght, 0, FreeRTOS);
	}
}

void Bluetooth_Stop(void){
	if(Dixom.Module.Bluetooth.dModel == BLUETOOTH_WT32_iWRAP4){
		//BT_WT32_iWRAP4_Transmit((uint8_t*) "AVRCP REWIND", AutoLenght, 0, FreeRTOS);
	}else if(FSC){
		FSC_Bluetooth_Transmit((uint8_t *)"AT+STOP", AutoLenght, 0, FreeRTOS);
	}
}

void Bluetooth_Track_Next(void){

	if(Dixom.Module.Bluetooth.dModel == BLUETOOTH_WT32_iWRAP4){
		//BT_WT32_iWRAP4_Transmit((uint8_t*) "AVRCP FORWARD", AutoLenght, 0, FreeRTOS);
	}else if(FSC){
		FSC_Bluetooth_Transmit((uint8_t *)"AT+FORWARD", NO, 0, FreeRTOS);
	}
}

void Bluetooth_Track_Prev(void){
	if(Dixom.Module.Bluetooth.dModel == BLUETOOTH_WT32_iWRAP4){
		//BT_WT32_iWRAP4_Transmit((uint8_t*) "AVRCP BACKWARD", AutoLenght, 0, FreeRTOS);
	}else if(FSC){
		FSC_Bluetooth_Transmit((uint8_t *)"AT+BACKWARD", NO, 0, FreeRTOS);
	}
}

void Bluetooth_Forward(void){
	if(Dixom.Module.Bluetooth.dModel == BLUETOOTH_WT32_iWRAP4){
		//BT_WT32_iWRAP4_Transmit((uint8_t*) "AVRCP FAST_FORWARD", AutoLenght, 0, FreeRTOS);
	}else if(FSC){
		FSC_Bluetooth_Transmit((uint8_t *)"AT+FORWARD", NO, 0, FreeRTOS);
	}
}

void Bluetooth_Rewind(void){
	if(Dixom.Module.Bluetooth.dModel == BLUETOOTH_WT32_iWRAP4){
		//BT_WT32_iWRAP4_Transmit((uint8_t*) "AVRCP REWIND", AutoLenght, 0, FreeRTOS);
	}else if(FSC){
		FSC_Bluetooth_Transmit((uint8_t *)"AT+BACKWARD", AutoLenght, 0, FreeRTOS);
	}
}

void Bluetooth_PhoneReply(void){
	if(Dixom.Module.Bluetooth.dModel == BLUETOOTH_WT32_iWRAP4){

	}else if(FSC){
		FSC_Bluetooth_Transmit((uint8_t *)"AT+HFPANSW", AutoLenght, 0, FreeRTOS);
	}
}

void Bluetooth_PhoneCancel(void){
	if(Dixom.Module.Bluetooth.dModel == BLUETOOTH_WT32_iWRAP4){

	}else if(FSC){
		FSC_Bluetooth_Transmit((uint8_t *)"AT+HFPCHUP", AutoLenght, 0, FreeRTOS);
	}
}

void Bluetooth_PhoneRedial(void){
	if(Dixom.Module.Bluetooth.dModel == BLUETOOTH_WT32_iWRAP4){

	}else if(FSC){
		if(Dixom.Module.Bluetooth.ConnDevInfo.LastPhoneNumber[0] != 0){
			FSC_Bluetooth_Transmit((uint8_t *)"AT+HFPDIAL", AutoLenght, 0, FreeRTOS);
		}
	}
}






void FSC_Bluetooth_Transmit(uint8_t *data, uint8_t dataLen, uint16_t timeDelay, uint8_t from){

	if(dataLen == AutoLenght){
		dataLen = strlen((char*)data);
	}
	uint8_t DataSend[dataLen+2];
	for(uint8_t byte = 0; byte<dataLen; byte ++){
		DataSend[byte] = data[byte];
	}
	DataSend[dataLen] = '\r';
	DataSend[dataLen+1] = '\n';
	UART1_Transmit(&DataSend[0], dataLen+2, timeDelay, FreeRTOS);
}
















/*
uint8_t dasdads = "SET BT PAIR *"; //������� ��� ������������ ����������
uint8_t dasdads = "SET BT SSP 3 0"; //����������� ��� ������
uint8_t dasdads = "SET BT AUTH * 0000"; //����������� � ������� 0000
uint8_t dasdads = "AVRCP PDU 20 1 1"; //����������� � ������� 0000
*/
