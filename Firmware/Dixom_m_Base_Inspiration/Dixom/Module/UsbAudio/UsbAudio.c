/*
 * UsbAudio.c
 *
 *  Created on: 12 ���. 2022 �.
 *      Author: 79825
 */
#include <Description.h>
#include <Structures.h>
#include "UsbAudio.h"
#include "string.h"
#include "stdlib.h"
#include "Exchange.h"

extern sDixom Dixom;

#define dUsbAudio Dixom.Module.UsbAudio


void USB_AUDIO_CARD_STRING_CONTROL(uint8_t *Received_String){

	      if(strncmp((char*)Received_String, "WRITE",          5) == 0)      {  Transmit_UsbSoundCard(Received_String+6, DirectionTypeWrite);                      } //
     else if(strncmp((char*)Received_String, "READ",           4) == 0)      {  GET_Receiver_UsbSoundCard(Received_String+5, strlen((char*)Received_String)-5);    } //
	 else if(strncmp((char*)Received_String, "UPDATE_INFO",   11) == 0)      {  Update_AudioCard_Information(YES);                                                 } //
	 else{
 	     TransmitDataOutputs((uint8_t *)GeneralNotifi_CommandError, AutoLenght,  false,  true, allAvailable);

	 }
}

void Automatic_Detection_USB_Audio_Card_And_Initialization(){

	if (DevState_I2C2(DEVICE_ADDR_DIXOM_UsbAudioBoot<<1, 1, 100)== WR_OK) {
		Dixom.Module.UsbAudio.InMode = BootLoader;
		Dixom.Module.UsbAudio.DeviceInfo.Model = Dixom_HiRes_Audio;
		if(Dixom.Basis.Settings.Param[SettingsStateSoundCard].element == ON){
			if(Transmit_UsbSoundCard((uint8_t*)"3", DirectionTypeWrite)== WR_OK){
				Delay(500, FreeRTOS);
			}
		}
	}

	if (DevState_I2C2(DEVICE_ADDR_DIXOM_UsbAudio<<1, 1, 100)== WR_OK) {
		Dixom.Module.UsbAudio.InMode = ReadyForWork;
		Dixom.Module.UsbAudio.DeviceInfo.Model = Dixom_HiRes_Audio;
	}else{
		Dixom.Module.UsbAudio.InMode = NotFound;
		Dixom.Module.UsbAudio.DeviceInfo.Model = Nothing_Yet;
		if (DevState_I2C2(DEVICE_ADDR_DIXOM_UsbAudioBoot<<1, 1, 100)== WR_OK) {
			Dixom.Module.UsbAudio.InMode = BootLoader;
			Dixom.Module.UsbAudio.DeviceInfo.Model = Dixom_HiRes_Audio;
		}
	}
}

void InitDixomHiResAudio(){
	if(GetSettings(SettingsStateSoundCard)==ON){
		Transmit_UsbSoundCard((uint8_t*)"3", DirectionTypeWrite);
	}
}

void Update_AudioCard_Information(uint8_t GetState){

	    /* New version firmware*/
	if (DevState_I2C2(DEVICE_ADDR_DIXOM_UsbAudio<<1, 1, 100)== WR_OK) {
		Dixom.Module.UsbAudio.InMode = ReadyForWork;

		/* Old version firmware and bootloader*/
	}else if(DevState_I2C2(DEVICE_ADDR_DIXOM_UsbAudioBoot<<1, 1, 100)== WR_OK){
		Dixom.Module.UsbAudio.InMode = BootLoader;

		 /* Audio Card not found*/
	}else{
		Dixom.Module.UsbAudio.InMode = NotFound;
		Dixom.Module.UsbAudio.DeviceInfo.Model = Nothing_Yet;
	}

	    /* New version firmware*/
	if(Dixom.Module.UsbAudio.InMode == ReadyForWork){
		ExchangeStatus(	Receiver_I2C2( DEVICE_ADDR_DIXOM_UsbAudio<<1, ADDR_REG_USB_AUDIO_STATE, 1, (uint8_t*)&dUsbAudio.State,
				sizeof(dUsbAudio.State), 300, ExchangeMemMainSteam), USB_SOUND_CARD_READ);
		ExchangeStatus(	Receiver_I2C2( DEVICE_ADDR_DIXOM_UsbAudio<<1, ADDR_REG_USB_AUDIO_DEVICE, 1, (uint8_t*)&dUsbAudio.DeviceInfo,
				sizeof(dUsbAudio.DeviceInfo), 300, ExchangeMemMainSteam), USB_SOUND_CARD_READ);

		sprintf((char *)Dixom.Module.UsbAudio.VerFrimware,"%.3f", Dixom.Module.UsbAudio.DeviceInfo.Version_Main*0.001);
		sprintf((char *)Dixom.Module.UsbAudio.VerBootloader,"%.2f", Dixom.Module.UsbAudio.DeviceInfo.Version_Bootloader*0.01);

		/* Old version firmware and bootloader*/
	}else if(Dixom.Module.UsbAudio.InMode == BootLoader){

		uint8_t ForProcessing[5];

		Receiver_UsbSoundCard((uint8_t *)"0", 1, &Dixom.Buff[0]);

		Dixom.Module.UsbAudio.VerFrimware[0] = '0';
		Dixom.Module.UsbAudio.VerFrimware[1] = '.';
		Dixom.Module.UsbAudio.VerFrimware[2] = '0';
		Dixom.Module.UsbAudio.VerFrimware[3] = '0';
		Dixom.Module.UsbAudio.VerFrimware[4] = '0';


		Dixom.Module.UsbAudio.VerBootloader[0] = Dixom.Buff[27];
		Dixom.Module.UsbAudio.VerBootloader[1] = Dixom.Buff[28];
		Dixom.Module.UsbAudio.VerBootloader[2] = Dixom.Buff[29];
		Dixom.Module.UsbAudio.VerBootloader[3] = Dixom.Buff[30];
		Dixom.Module.UsbAudio.VerBootloader[4] = 0;

		Dixom.Module.UsbAudio.DeviceInfo.Version_Main = 0;

		ForProcessing[0] = Dixom.Module.UsbAudio.VerFrimware[0];
		ForProcessing[1] = Dixom.Module.UsbAudio.VerFrimware[2];
		ForProcessing[2] = Dixom.Module.UsbAudio.VerFrimware[3];
		ForProcessing[3] = 0;

		Dixom.Module.UsbAudio.DeviceInfo.Version_Bootloader = atoi ((char*)ForProcessing);
	}

	/* Send information*/
	if(GetState == YES){
		 Get_AudioCard_Information();
	}
}


void Get_AudioCard_Information(){

	if(dUsbAudio.InMode != NotFound){
		sprintf((char *)Dixom.Buff, System_Info_1_3_3, dUsbAudio.VerFrimware, dUsbAudio.VerBootloader, dUsbAudio.DeviceInfo.Model, dUsbAudio.InMode);
 	    TransmitDataOutputs(Dixom.Buff, AutoLenght,  false,  true, allAvailable);

	}
}


WorkRes Transmit_UsbSoundCard( uint8_t* pData, uint8_t type ){

	if(strncmp((char*)pData, "RESET",          5) == 0){
		 Dixom.Basis.Settings.Param[SettingsAutocSoundCardPolling].element = OFF;
	}

	uint8_t SendData[DATA_LEN_DIXOM_HIRES_AUDIO_BOOT];
	SendData[0] = type; //0
	for(int Position=0; Position<DATA_LEN_DIXOM_HIRES_AUDIO_BOOT-1; Position++){
		SendData[Position+1] = pData[Position];
	}

	/* Old version firmware and bootloader*/
	if(dUsbAudio.InMode == BootLoader){

		return ExchangeStatus(	Transmit_I2C2( DEVICE_ADDR_DIXOM_UsbAudioBoot<<1, NOT_USED, NOT_USED, &SendData[0], DATA_LEN_DIXOM_HIRES_AUDIO_BOOT, 300, ExchangeMainSteam), USB_SOUND_CARD_WRITE);

	}else if(dUsbAudio.InMode == ReadyForWork){

		return ExchangeStatus(	Transmit_I2C2( DEVICE_ADDR_DIXOM_UsbAudio<<1, NOT_USED, NOT_USED, &pData[0], 5, 300, ExchangeMainSteamDMA), USB_SOUND_CARD_WRITE);
	}else{
		return WR_ERROR;
	}
}


void GET_Receiver_UsbSoundCard(uint8_t *pDataIN, uint8_t DataSendLen){
	uint8_t dataRead[56];
	Receiver_UsbSoundCard(pDataIN , DataSendLen, dataRead);
	if(Dixom.Module.UsbAudio.InMode != BootLoader){
  	    TransmitDataOutputs((uint8_t *)dataRead, AutoLenght,  false,  true, allAvailable);

	}
}

void Receiver_UsbSoundCard(  uint8_t* pDataSend, uint8_t DataSendLen, uint8_t *pDataRead){

	if(dUsbAudio.InMode == BootLoader){
		uint8_t SendData[DATA_LEN_DIXOM_HIRES_AUDIO_BOOT];
		SendData[0] = DirectionTypeRead; //1

		if(DataSendLen<DATA_LEN_DIXOM_HIRES_AUDIO_BOOT){
			for(int Position=0; Position<DataSendLen; Position++){
				SendData[Position+1] = pDataSend[Position];
			}
			ExchangeStatus(	Transmit_I2C2( DEVICE_ADDR_DIXOM_UsbAudioBoot<<1, NOT_USED, NOT_USED, SendData, DATA_LEN_DIXOM_HIRES_AUDIO_BOOT, 300, ExchangeMainSteam), USB_SOUND_CARD_WRITE);
			ExchangeStatus(	Receiver_I2C2( DEVICE_ADDR_DIXOM_UsbAudioBoot<<1, NOT_USED, NOT_USED, pDataRead, DATA_LEN_DIXOM_HIRES_AUDIO_BOOT, 500, ExchangeMainSteam), USB_SOUND_CARD_READ);

			if(pDataRead[17] == 82 && pDataRead[18] == 69 && pDataRead[19] == 65 && pDataRead[20] == 68){
 		  	    TransmitDataOutputs((uint8_t *)MessageUpdate_2_1_0, AutoLenght,  false,  true, allAvailable);

			}else if (pDataRead[17] == 78 && pDataRead[18] == 79 && pDataRead[19] == 84 && pDataRead[20] == 95){
 		  	    TransmitDataOutputs((uint8_t *)MessageUpdate_2_1_14, AutoLenght,  false,  true, allAvailable);

			}


		}
	}else if(dUsbAudio.InMode == ReadyForWork){
		if(pDataSend[0] == 101 || pDataSend[0] == 150){

		//	ExchangeStatus(	Receiver_I2C2( DEVICE_ADDR_DIXOM_UsbAudioBoot<<1, pDataSend, 2, pDataRead, DataSendLen, 500, ExchangeMainSteamDMA), USB_SOUND_CARD_READ);
			ExchangeStatus(	Transmit_I2C2( DEVICE_ADDR_DIXOM_UsbAudio<<1, NOT_USED, NOT_USED, pDataSend, DATA_LEN_DIXOM_HIRES_AUDIO_NORMAL, 300, ExchangeMainSteam), USB_SOUND_CARD_WRITE);
		    ExchangeStatus(	Receiver_I2C2( DEVICE_ADDR_DIXOM_UsbAudio<<1, NOT_USED, NOT_USED, pDataRead, DATA_LEN_DIXOM_HIRES_AUDIO_NORMAL, 500, ExchangeMainSteam), USB_SOUND_CARD_READ);
		}
	}


}

