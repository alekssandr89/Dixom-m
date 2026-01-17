#include <Description.h>
#include "FSC_BT806.h"
#include "Bluetooth.h"
#include "Exchange.h"
//#include "StructSettings.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <Structures.h>
#include <base64.h>
#include "cmsis_os.h"

extern sDixom Dixom;
#define dBluetooth Dixom.Module.Bluetooth
#define  BufSize 2000
#define FSC (dBluetooth.dModel == BLUETOOTH_FSC_BT806 || dBluetooth.dModel == BLUETOOTH_FSC_BT802 || dBluetooth.dModel == BLUETOOTH_FSC_BT1006C || dBluetooth.dModel == BLUETOOTH_FSC_BT1006A)

uint8_t  PBDNum = 0; //Phone Book Download

//tDEVSTAT DEVSTAT;

//extern osSemaphoreId BinarySem_DataOutHandle;

void ScanBtDeviceDisconnecting(){
	extern uint16_t uart1BuffDataSize;
	static uint16_t RxPointer = 0;
	const uint8_t targetString[10] = "+SPPSTAT=1";
	extern uint8_t Uart1RxBuff[RX_BUFFER_LENGTH_UART1];
	const uint8_t targetLength = 10;

	while (RxPointer != uart1BuffDataSize) {

		// Part of the target string that goes through the end of the buffer
		uint16_t remainingLength = (RX_BUFFER_LENGTH_UART1 - RxPointer);

		// Check if there is enough data in the buffer starting from the current RxPointer
		if (remainingLength >= targetLength) {
			// Compare part of the buffer, starting at the current RxPointer, with the target string
			if (strncmp((char *)&Uart1RxBuff[RxPointer], (char *)targetString, targetLength) == 0) {
				Dixom.Module.Bluetooth.Connection = NotConnected;
			}
		} else {
			// Compare the rest of the line with the beginning of the buffer
			if (strncmp((char *)&Uart1RxBuff[RxPointer], (char *)targetString, remainingLength) == 0 &&
					strncmp((char *)&Uart1RxBuff[0], (char *)(targetString + remainingLength), targetLength - remainingLength) == 0) {
				Dixom.Module.Bluetooth.Connection = NotConnected;
			}		}

		RxPointer = (RxPointer + 1) % RX_BUFFER_LENGTH_UART1;
	}
}
uint16_t getBufPtr(uint16_t ptr, uint8_t bias );

int LoopBT(){
	static uint16_t RxPointer = 0;
	static uint8_t uart1SendMsgSize = 0;
	static uint8_t SendMsgBuf[255] = {0};
	extern uint8_t Uart1RxBuff[RX_BUFFER_LENGTH_UART1];
	extern uint16_t uart1BuffDataSize;

	while(RxPointer != uart1BuffDataSize){

		if (Uart1RxBuff[RxPointer] == Stx){
			//Start of line
			SendMsgBuf[uart1SendMsgSize] = 0;
			uart1SendMsgSize = 0;
		}else if (Uart1RxBuff[RxPointer] == Eot){
			//End of line
			SendMsgBuf[uart1SendMsgSize] = 0;
			BluetoothMenu( SendMsgBuf, uart1SendMsgSize);
			uart1SendMsgSize = 0;
		}else if (Uart1RxBuff[getBufPtr(RxPointer, 2)] > Text &&  Uart1RxBuff[getBufPtr(RxPointer, 1)] == R &&  Uart1RxBuff[RxPointer] == N){
			//End of line
			SendMsgBuf[uart1SendMsgSize] = 0;
			BluetoothMenu( SendMsgBuf, uart1SendMsgSize);
			uart1SendMsgSize = 0;
		}else if(Uart1RxBuff[RxPointer] > Text){
			//Only ASCI II
			SendMsgBuf[uart1SendMsgSize] =  Uart1RxBuff[RxPointer];
			uart1SendMsgSize++;
		}

		RxPointer = (RxPointer + 1) % RX_BUFFER_LENGTH_UART1;
	}
	return WR_OK;
}

uint16_t getBufPtr(uint16_t ptr, uint8_t bias ){
	return ((ptr + RX_BUFFER_LENGTH_UART1 - bias) % RX_BUFFER_LENGTH_UART1);
}


uint8_t btMsgRxBuff[255];

int FscGetSppData(uint8_t* Recive_data, uint16_t len){

	if(Dixom.Exchange.Interface.Interface_selection_mode == AUTO_SELECT){
		Dixom.Exchange.Interface.Current_interface = BLUETOOTH_SPP;
	}

	if(len>1){
		len = len-2;
	}

	if(len>255){
		return WR_ERROR;
	}



	uint8_t msgType = Recive_data[0];
	uint8_t crc8 = Recive_data[1];


	if(msgType == BASE64_WHOLE){
		uint32_t decBuffSize = get_decode_base64_required_buffer_size(Recive_data+2, len);
		if(decBuffSize > 255){
			return WR_ERROR;
		}
		base64_decode(btMsgRxBuff,  &decBuffSize, Recive_data+2, len);
		btMsgRxBuff[decBuffSize] = 0;
		main_Menu(btMsgRxBuff, decBuffSize);
		return WR_OK;
	}else if (msgType == UTF8_WHOLE){
		Copy_ArrayToArray(Recive_data+2, 0,  btMsgRxBuff,  0, len, 255);
		btMsgRxBuff[len] = 0;
		main_Menu(btMsgRxBuff, len);
		return WR_OK;

	}
	return WR_ERROR;

}

void setTpMode(uint8_t* data, uint16_t len){
	Dixom.Module.Bluetooth.SppThroughputMode = data[0]-48;
}


uint8_t BluetoothMenu(  uint8_t* Recive_data, uint8_t len){

	if(len<1) return WR_ERROR;

	if(Dixom.Module.Bluetooth.Settings[BtControlDirectMode].Data == 1){
		sprintf((char*) Dixom.Buff, BluetoothDirectAccess, Recive_data);
 	 	TransmitDataOutputs(Dixom.Buff, AutoLenght,  false,  true, allAvailable);

	}

 	if(Recive_data[0] == UTF8_WHOLE ||  Recive_data[0] == BASE64_WHOLE           )  { FscGetSppData (Recive_data, len);             }
	else if(strncmp((char *)Recive_data,     "+SPPD",                       5) == 0)  {
		uint8_t bias = 10;
		while(Recive_data[bias] != Zy){
			bias++;
			if(bias>4) break;
		}
		 FscGetSppData (Recive_data+bias+1, len-bias);
	}
	else if(strncmp((char *)Recive_data,     "OK",                          2) == 0)  { /*NOT USED*/                       }
	else if(strncmp((char *)Recive_data,     "+HFPNET",                     7) == 0)  { FscGetNetworkName(Recive_data+8, len-8);           }
	else if(strncmp((char *)Recive_data,     "+TRACKINFO",                 10) == 0)  { FscMediaTrackInfo(Recive_data+11, len-11);         }
	else if(strncmp((char *)Recive_data,     "+TRACKSTAT",                 10) == 0)  { FscMediaTrackStat(Recive_data+11, len-11);         }
	else if(strncmp((char *)Recive_data,     "+HFPSIG",                     7) == 0)  { FscGetNetworkSignal(Recive_data+8, len-8);         }
	else if(strncmp((char *)Recive_data,     "+HFPDEV",                     7) == 0)  { FscGetDeviceAddName(Recive_data+8, len-8);         }
	else if(strncmp((char *)Recive_data,     "+A2DPSTAT",                   9) == 0)  { FscDeviceStandby(Recive_data+10, len-10);          }
	else if(strncmp((char *)Recive_data,     "+PLAYSTAT",                   9) == 0)  { FscMediaPlayStat(Recive_data+10, len-10);          }
	else if(strncmp((char *)Recive_data,     "+HFPCID",                     7) == 0)  { FscPhoneIncoming(Recive_data+8, len-8);            }
	else if(strncmp((char *)Recive_data,     "+HFPSTAT",                    8) == 0)  { FscPhoneState(Recive_data+9, len-9);               }
	else if(strncmp((char *)Recive_data,     "+PBDATA",                     7) == 0)  { FscPhoneBook(Recive_data+8, len-8);                }
	else if(strncmp((char *)Recive_data,     "+PBSTAT",                     7) == 0)  { FscBookState(Recive_data+8, len-8);                }
	else if(strncmp((char *)Recive_data,     "+A2DPDEC",                    8) == 0)  { FscA2dpCodec(Recive_data+9, len-9);                }
	else if(strncmp((char *)Recive_data,     "+VER",                        4) == 0)  { FscModuleVersion(Recive_data+5, len-5);            }
	else if(strncmp((char *)Recive_data,     "+NAME",                       4) == 0)  { FscModuleName(Recive_data+6, len-6);               }
	else if(strncmp((char *)Recive_data,     "+PIN",                        4) == 0)  { FscModulePass(Recive_data+5, len-5);               }
	else if(strncmp((char *)Recive_data,     "+ADDR",                       5) == 0)  { FscModuleAddr(Recive_data+6, len-6);               }
	else if(strncmp((char *)Recive_data,     "+STAT",                       5) == 0)  { FscModuleStat(Recive_data+6, len-6);               }
	else if(strncmp((char *)Recive_data,     "+PLIST",                      6) == 0)  { FscModulePlist(Recive_data+7, len-7);              }
	else if(strncmp((char *)Recive_data,     "+SPPSTAT",                    8) == 0)  { FscSppState(Recive_data+9, len-9);                 }
	else if(strncmp((char *)Recive_data,     "+TPMODE",                     7) == 0)  { setTpMode(Recive_data+8, len-8);                }
	else{
		if(Dixom.Module.Bluetooth.Settings[BtControlDirectMode].Data == 0){
			sprintf((char*) Dixom.Buff, BluetoothDirectAccess, Recive_data);
 			TransmitDataOutputs(Dixom.Buff, AutoLenght,  false,  true, allAvailable);


		}
	}

 	 return WR_OK;
}


void DOWNLOAD_PBOOK_FSC(uint8_t *Recive_data){

	PBDNum = 0;

	short DownCount  = 0;
	short DownFrom  = 0;
	char data[20];
	sscanf((char *)Recive_data, "%hu %hu", &DownFrom, &DownCount);

	if(DownCount==0){
		sprintf( data,"AT+PBDOWN=%hu",DownFrom);
		FSC_Bluetooth_Transmit((uint8_t*)&data[0], NO, 0, FreeRTOS);
	}else{
		sprintf( data,"AT+PBDOWN=%hu,%hu",DownFrom,DownCount);
		FSC_Bluetooth_Transmit((uint8_t*)&data[0], NO, 0, FreeRTOS);
	}
}

void MEDIA_FSC(uint8_t *Recive_data){
	     if(strncmp((char *)Recive_data,       MediaVolUp,             6) == 0)  {  	FSC_Bluetooth_Transmit((uint8_t *)"AT+SPKVOL=+", AutoLenght, 0, FreeRTOS);    }
	else if(strncmp((char *)Recive_data,       MediaVolDown,           8) == 0)  {  	FSC_Bluetooth_Transmit((uint8_t *)"AT+SPKVOL=-", AutoLenght, 0, FreeRTOS);    }
	else if(strncmp((char *)Recive_data,       MediaPlayPause,         8) == 0)  {  	FSC_Bluetooth_Transmit((uint8_t *)"AT+PLAYPAUSE", AutoLenght, 0, FreeRTOS);   }
	else if(strncmp((char *)Recive_data,       MediaMute,              4) == 0)  {  	                                                                              }
	else if(strncmp((char *)Recive_data,       MediaStop,              4) == 0)  {  	FSC_Bluetooth_Transmit((uint8_t *)"AT+STOP", AutoLenght, 0, FreeRTOS);        }
	else if(strncmp((char *)Recive_data,       MediaPlay,              4) == 0)  {  	FSC_Bluetooth_Transmit((uint8_t *)"AT+PLAY", AutoLenght, 0, FreeRTOS);        }
	else if(strncmp((char *)Recive_data,       MediaPause,             5) == 0)  {  	FSC_Bluetooth_Transmit((uint8_t *)"AT+PAUSE", AutoLenght, 0, FreeRTOS);       }
	else if(strncmp((char *)Recive_data,       MediaRewind,            6) == 0)  {  	FSC_Bluetooth_Transmit((uint8_t *)"AT+BACKWARD", AutoLenght, 0, FreeRTOS);    }
	else if(strncmp((char *)Recive_data,       MediaFast_Forward,     12) == 0)  {  	FSC_Bluetooth_Transmit((uint8_t *)"AT+FORWARD", AutoLenght, 0, FreeRTOS);     }
	else if(strncmp((char *)Recive_data,       MediaForward,           7) == 0)  {  	FSC_Bluetooth_Transmit((uint8_t *)"AT+FORWARD", AutoLenght, 0, FreeRTOS);     }
	else if(strncmp((char *)Recive_data,       MediaBackward,          8) == 0)  {  	FSC_Bluetooth_Transmit((uint8_t *)"AT+BACKWARD", AutoLenght, 0, FreeRTOS);    }
}

void PHONE_FSC(uint8_t *Recive_data){
	     if(strncmp((char *)Recive_data,       PhoneReply,             5) == 0)  {  	FSC_Bluetooth_Transmit((uint8_t *)"AT+HFPANSW", AutoLenght, 0, FreeRTOS);    }
	else if(strncmp((char *)Recive_data,       PhoneCancel,            6) == 0)  {  	FSC_Bluetooth_Transmit((uint8_t *)"AT+HFPCHUP", AutoLenght, 0, FreeRTOS);    }
	else if(strncmp((char *)Recive_data,       PhoneRedial,            6) == 0)  {
		if(dBluetooth.ConnDevInfo.LastPhoneNumber[0] != 0){
			FSC_Bluetooth_Transmit((uint8_t *)"AT+HFPDIAL", AutoLenght, 0, FreeRTOS);
		}
	}
}

void SPP_FSC(uint8_t *Recive_data){

	FSC_Bluetooth_TransmitSPP(Recive_data, AutoLenght, 0, FreeRTOS);
}
//Dixom.Module.Bluetooth.SendData

void FscGetNetworkName(uint8_t* data, short len){

	for(uint8_t i = 0; i < BtModuleInfoLen; i++){
		dBluetooth.ConnDevInfo.NetworkName[i] = 0;
	}
	if(sizeof(data) < BtModuleInfoLen){
		sprintf((char *)dBluetooth.ConnDevInfo.NetworkName,"%s", data);
		sprintf((char *)dBluetooth.bBuff, MaskGetInfoBtDeNetName, data);
	}
	TransmitDataOutputs(dBluetooth.bBuff, AutoLenght,  false,  true, allAvailable);

}

void FscGetNetworkSignal(uint8_t* data, short len){

	dBluetooth.ConnDevInfo.NetworkLevl = data[0];
	sprintf((char *)dBluetooth.bBuff, MaskGetInfoBtDeNetSignal, data[0]);
	TransmitDataOutputs(dBluetooth.bBuff, AutoLenght,  false,  true, allAvailable);


}
void FscGetDeviceAddName(uint8_t* data, short len){

	BLUETOOTH_Data_out((uint8_t *)MaskGetBtDevConn, 0, 10, FreeRTOS, NO);

	for(uint8_t byte = 0; byte<BtModuleInfoLen; byte++){
		Dixom.Module.Bluetooth.ConnDevInfo.MacAddr[byte] = data[byte];
	}
	sprintf((char *)dBluetooth.bBuff,MaskGetInfoBtDevAddr, Dixom.Module.Bluetooth.ConnDevInfo.MacAddr);
 	TransmitDataOutputs(dBluetooth.bBuff, AutoLenght,  false,  true, allAvailable);


	if(len-13 > 43) len = 56;

	for(uint8_t i = 0; i < 43; i++){
		Dixom.Module.Bluetooth.ConnDevInfo.Name[i] = 0;
	}

	for(uint8_t byte = 0; byte<len-13; byte++){
		Dixom.Module.Bluetooth.ConnDevInfo.Name[byte] = data[byte+13];
	}

	sprintf((char *)dBluetooth.bBuff,MaskGetInfoBtDevName, Dixom.Module.Bluetooth.ConnDevInfo.Name);
 	TransmitDataOutputs(dBluetooth.bBuff, AutoLenght,  false,  true, allAvailable);


	Dixom.Module.Bluetooth.InMode = Connected;
}



uint8_t startState = 0;

void FscDeviceStandby(uint8_t* data, short len){
	if(data[0] == 51) startState = 1;
	if(data[0] == 49 && startState == 1){
		startState = 0;

		BLUETOOTH_Data_out((uint8_t *)MaskGetBtDevConn0, 0, 10, FreeRTOS, NO);
		BLUETOOTH_Data_out((uint8_t *)MaskGetBtDevNameDis, 0, 10, FreeRTOS, NO);
		BLUETOOTH_Data_out((uint8_t *)MaskGetBtDevAddrDis, 0, 10, FreeRTOS, NO);
		BLUETOOTH_Data_out((uint8_t *)MaskGetBtDevNameDis, 0, 10, FreeRTOS, NO);
		BLUETOOTH_Data_out((uint8_t *)MaskGetBtDevNetDis, 0, 10, FreeRTOS, NO);


		Dixom.Module.Bluetooth.InMode = NotConnected;
		Dixom.Module.Bluetooth.ConnDevInfo.A2dpDecoder = BTDecoder_NO;
		Dixom.Module.Bluetooth.ConnDevInfo.NetworkLevl = '0';
		Dixom.Module.Bluetooth.ConnDevInfo.MediaState = '0';
		Dixom.Module.Bluetooth.ConnDevInfo.TimeProgress = 0;
		Dixom.Module.Bluetooth.ConnDevInfo.TimeTotal = 0;

		for(uint8_t byte = 0; byte<12; byte++){
			Dixom.Module.Bluetooth.ConnDevInfo.MacAddr[byte] = data[byte];
		}

		for(uint8_t i = 0; i < 58; i++){
			Dixom.Module.Bluetooth.ConnDevInfo.Name[i] = 0;
		}

		for(uint8_t i = 0; i < 58; i++){
			Dixom.Module.Bluetooth.ConnDevInfo.NetworkName[i] = 0;
		}

	}
}

void FscMediaTrackStat(uint8_t* data, short len){


	sscanf((char *)data+2, "%lu,%lu", &Dixom.Module.Bluetooth.ConnDevInfo.TimeProgress, &Dixom.Module.Bluetooth.ConnDevInfo.TimeTotal);
	sprintf((char *)dBluetooth.bBuff, MaskGetBtMediaTime, Dixom.Module.Bluetooth.ConnDevInfo.TimeProgress, Dixom.Module.Bluetooth.ConnDevInfo.TimeTotal);
 	TransmitDataOutputs(dBluetooth.bBuff, AutoLenght,  false,  true, allAvailable);

}




void FscMediaTrackInfo(uint8_t* data, short len){

	uint8_t TrackInfo[len+4];
	uint8_t MsgLen = 0;

	uint8_t state = 0;

	for(uint16_t byte = 0; byte < len; byte++){

		if(data[byte]==255) state++;

		TrackInfo[MsgLen+4] = data[byte];

		if(state == 1){
			Copy_ArrayToArray((uint8_t*)&MaskGetBtMediaTit, 0, &TrackInfo[0],  0, 4, len);
 			TransmitDataOutputs(&TrackInfo[0], MsgLen+4,  false,  true, allAvailable);

			MsgLen=0;
			state++;
		}else if(state == 3){
			Copy_ArrayToArray((uint8_t*)&MaskGetBtMediaArt, 0, &TrackInfo[0],  0, 4, len);
  			TransmitDataOutputs(&TrackInfo[0], MsgLen+4,  false,  true, allAvailable);

			state++;
			MsgLen=0;
		}else{
			MsgLen++;
		}
	}

	Copy_ArrayToArray((uint8_t*)&MaskGetBtMediaAlb, 0, &TrackInfo[0],  0, 4, 4);
	TransmitDataOutputs(&TrackInfo[0], MsgLen+4,  false,  true, allAvailable);



}

void FscMediaPlayStat(uint8_t* data, short len){

	if(Dixom.Module.Bluetooth.ConnDevInfo.A2dpDecoder == '0' && data[0]=='1'){
		FSC_Bluetooth_Transmit((uint8_t *)"AT+A2DPDEC", NO, 0, FreeRTOS);
	}

	if(data[0]!=3 &&data[0]!=4 ){
		Dixom.Module.Bluetooth.ConnDevInfo.MediaState = data[0];

		if(data[0] == PLAY_State_MediaStopped)              {Dixom.Module.Bluetooth.ConnDevInfo.MediaState = Mode_MediaStopped;	}
		else if(data[0] == PLAY_State_MediaPlaying)         {Dixom.Module.Bluetooth.ConnDevInfo.MediaState = Mode_MediaPlaying;}
		else if(data[0] == PLAY_State_MediaPaused)          {Dixom.Module.Bluetooth.ConnDevInfo.MediaState = Mode_MediaPaused;}
		else if(data[0] == PLAY_State_MediaFastForwarding)  {Dixom.Module.Bluetooth.ConnDevInfo.MediaState = Mode_MediaFastForwarding;}
		else if(data[0] == PLAY_State_MediaFastRewinding)   {Dixom.Module.Bluetooth.ConnDevInfo.MediaState = Mode_MediaFastRewinding;}

		Copy_ArrayToArray((uint8_t *)MaskGetBtMediaPst, 0, dBluetooth.bBuff,  0, 16, 0);
		dBluetooth.bBuff[4] = data[0];
		TransmitDataOutputs(dBluetooth.bBuff, 16,  false,  true, allAvailable);

	}
}

void FscPhoneIncoming(uint8_t* data, short len){

	for(uint8_t i = 0; i< sizeof(Dixom.Module.Bluetooth.ConnDevInfo.LastPhoneNumber); i++ ){
		Dixom.Module.Bluetooth.ConnDevInfo.LastPhoneNumber[i] = 0;
	}

	if(len<sizeof(Dixom.Module.Bluetooth.ConnDevInfo.LastPhoneNumber)){
		Copy_ArrayToArray(data, 0, Dixom.Module.Bluetooth.ConnDevInfo.LastPhoneNumber,  0, len, 0);
	}
	Copy_ArrayToArray((uint8_t *)MaskGetBtPhonePhn, 0, dBluetooth.bBuff,  0, 7, 0);

	Copy_ArrayToArray(data, 0, dBluetooth.bBuff,  7, len, 0);
	TransmitDataOutputs(dBluetooth.bBuff, len+7,  false,  true, allAvailable);
}

void FscPhoneState(uint8_t* data, short len){

	Copy_ArrayToArray((uint8_t *)MaskGetBtPhonePhs, 0, dBluetooth.bBuff,  0, 17, 0);
	if(data[0] == HFP_State_Connected)        {Dixom.Module.Bluetooth.InMode = Connected;	 Phone_CompletedCall(); Dixom.Module.Bluetooth.ConnDevInfo.SpeakerphoneState = ON;}
	else if(data[0] == HFP_State_OutgoingCall){Dixom.Module.Bluetooth.InMode = OutgoingCall; Phone_OutgoingCall();  }
	else if(data[0] == HFP_State_IncomingCall){Dixom.Module.Bluetooth.InMode = IncomingCall; Phone_IncomingCall();  }
	else if(data[0] == HFP_State_ActiveCall)  {Dixom.Module.Bluetooth.InMode = ActiveCall;   Phone_ActiveCall();    FscSpeakerphoneState(OFF);}

	dBluetooth.bBuff[4] = data[0];
	TransmitDataOutputs(dBluetooth.bBuff, len+17,  false,  true, allAvailable);
}

void FscSpeakerphoneState(uint8_t state){

	Dixom.Module.Bluetooth.ConnDevInfo.SpeakerphoneState = state;

	Copy_ArrayToArray((uint8_t *)"AT+HFPADTS=", 0, dBluetooth.bBuff,  0, 11, 65);
	dBluetooth.bBuff[12] = state;

	FSC_Bluetooth_Transmit(&dBluetooth.bBuff[0], 12, 0, FreeRTOS);

}




void FscPhoneBook(uint8_t* data, short len){

	uint8_t PBookInfo[len+15];
	uint8_t CountBuf[5];
	uint8_t MsgLen = 0;
	uint8_t state = 0;
	uint8_t NumBias = 1;

	PBookInfo[0] = data[0];
	PBookInfo[1] = ' ';
	PBookInfo[3] = ' ';

	itoa(PBDNum,(char *)CountBuf,10);

	     if(PBDNum >9999){NumBias = 5;}
	else if(PBDNum >999) {NumBias = 4;}
	else if(PBDNum >99)  {NumBias = 3;}
	else if(PBDNum >9)   {NumBias = 2;}

	PBDNum++;

	if(data[0]== 49 || data[0]== 48){

		for(uint8_t i = 0; i <NumBias; i++){
			PBookInfo[i+4] = CountBuf[i];
		}
		PBookInfo[NumBias+4] = ' ';
		MsgLen = 5+NumBias;
		for(uint16_t byte = 2; byte < len; byte++){

			if(data[byte]==255){ state++; }
			PBookInfo[MsgLen] = data[byte];

			if(state == 1){
				PBookInfo[MsgLen] = End_Mesage;
				PBookInfo[2] = '0';
				Transmit_Long_Data_Out((uint8_t *)MaskGetBtBookPbd, 7, PBookInfo, 20);

				MsgLen=5+NumBias;
				state++;
			}else if(byte == len-1){
				PBookInfo[MsgLen] = End_Mesage;
				PBookInfo[2] = '1';
				Transmit_Long_Data_Out((uint8_t *)MaskGetBtBookPbd, 7, PBookInfo, 20);
			}else{
				MsgLen++;
			}
		}

	}else{

		for(uint8_t i = 0; i <NumBias; i++){
			PBookInfo[i+4] = CountBuf[i];
		}
		PBookInfo[NumBias+4] = ' ';
		MsgLen = 5+NumBias;
		for(uint16_t byte = 2; byte < len; byte++){

			if(data[byte]==255){ state++; }
			PBookInfo[MsgLen] = data[byte];

			if(state == 1){
				PBookInfo[MsgLen] = End_Mesage;
				PBookInfo[2] = '0';
				Transmit_Long_Data_Out((uint8_t *)MaskGetBtBookPbd, 7, PBookInfo, 20);
				MsgLen=5+NumBias;
				state++;
			}else if(state == 3){
				PBookInfo[MsgLen] = ' ';
				PBookInfo[2] = '1';
				MsgLen++;
				state++;
			}else if(byte == len-1){

				PBookInfo[MsgLen+1] = End_Mesage;
				Transmit_Long_Data_Out((uint8_t *)MaskGetBtBookPbd, 7, PBookInfo, 20);

			}else if(state == 4){
				if(byte==len-12) {    MsgLen++;  PBookInfo[MsgLen] = '.';     }
				if(byte==len-10) {    MsgLen++;  PBookInfo[MsgLen] = '.';     }
				if(byte==len-7)  {               PBookInfo[MsgLen] = ' ';     }
				if(byte==len-5)  {    MsgLen++;  PBookInfo[MsgLen] = ':';     }
				if(byte==len-3)  {    MsgLen++;  PBookInfo[MsgLen] = ':';     }

				MsgLen++;
			}
			else{
				MsgLen++;
			}
		}
	}
}

void FscBookState(uint8_t* data, short len){

	Copy_ArrayToArray((uint8_t *)MaskGetBtBookState, 0, dBluetooth.bBuff,  0, 7, 0);
	dBluetooth.bBuff[7] = data[0];
	TransmitDataOutputs(dBluetooth.bBuff, 8,  false,  true, allAvailable);
}

void FscA2dpCodec(uint8_t* data, short len){

	Dixom.Module.Bluetooth.ConnDevInfo.A2dpDecoder = data[0];
	Copy_ArrayToArray((uint8_t *)MaskGetBtMediaCodec, 0, dBluetooth.bBuff,  0, 7, 0);
	dBluetooth.bBuff[7] = data[0];
	TransmitDataOutputs(dBluetooth.bBuff, 8,  false,  true, allAvailable);

}

void FscModuleVersion(uint8_t* data, short len){

	//INCOMING TEXT: FSC-BT806,V4.0.1,20190216
	//INCOMING TEXT: FSC-BT1006A,V4.2.1,20191108

	if(Dixom.Module.Bluetooth.Settings[BtThroughputMode].Data == 0){
		FSC_Bluetooth_Transmit((uint8_t*) "AT+TPMODE=0", AutoLenght, 0, FreeRTOS);
	}else{
		FSC_Bluetooth_Transmit((uint8_t*) "AT+TPMODE=1", AutoLenght, 0, FreeRTOS);
	}
	FSC_Bluetooth_Transmit((uint8_t*) "AT+TPMODE", AutoLenght, 0, FreeRTOS);

	uint8_t ForProcessing[3];

	if(data[6]=='8' && data[7]=='0' && data[8]=='6'){
		Dixom.Module.Bluetooth.dModel = BLUETOOTH_FSC_BT806;
	}else if (data[6]=='8' && data[7]=='0' && data[8]=='X'){
		Dixom.Module.Bluetooth.dModel = BLUETOOTH_FSC_BT802;
	}else if (data[6]=='1' && data[7]=='0' && data[8]=='0' && data[9]=='6' && data[10]=='C'){
		Dixom.Module.Bluetooth.dModel = BLUETOOTH_FSC_BT1006C;
	}else if (data[6]=='1' && data[7]=='0' && data[8]=='0' && data[9]=='6' && data[10]=='A'){
		Dixom.Module.Bluetooth.dModel = BLUETOOTH_FSC_BT1006A;
	}

	ForProcessing[0] = 0;
	ForProcessing[1] = 0;

	for(uint8_t byte = 0; byte<30; byte++){
		if(data[byte] == ',' || data[byte] == ' '){
			ForProcessing[0]++;
			byte++;
		}else if(ForProcessing[0] == 0){
			Dixom.Module.Bluetooth.Model[byte] = data[byte];
		}else if(ForProcessing[0] == 1){
			Dixom.Module.Bluetooth.VerFrimware[ForProcessing[1]] = data[byte];
			ForProcessing[1]++;
		}
	}

	ForProcessing[0] = Dixom.Module.Bluetooth.VerFrimware[0];
	ForProcessing[1] = Dixom.Module.Bluetooth.VerFrimware[2];
	ForProcessing[2] = Dixom.Module.Bluetooth.VerFrimware[4];
	Dixom.Module.Bluetooth.dVerFrimware = atoi ((char*)ForProcessing);

	FSC_Bluetooth_Transmit((uint8_t *)"AT+NAME", 7, 0, FreeRTOS);
}

void FscModuleName(uint8_t* data, short len){
//	sprintf((char *)Dixom.Module.Bluetooth.Name,"%s", data);
	for(uint8_t byte = 0; byte < len; byte++){
		Dixom.Module.Bluetooth.Name[byte] = data[byte];
	}
	//Delay(20, FreeRTOS);
	FSC_Bluetooth_Transmit((uint8_t *)"AT+PIN", 6, 0, FreeRTOS);
}

void FscModulePass(uint8_t* data, short len){
	//sprintf((char *)Dixom.Module.Bluetooth.Pass,"%s", data);

	for(uint8_t byte = 0; byte < len; byte++){
		Dixom.Module.Bluetooth.Pass[byte] = data[byte];
	}
	//Delay(20, FreeRTOS);
	FSC_Bluetooth_Transmit((uint8_t *)"AT+ADDR", 7, 0, FreeRTOS);
}

void FscModuleAddr(uint8_t* data, short len){
//	sprintf((char *)Dixom.Module.Bluetooth.MacAddr,"%s", data);
	for(uint8_t byte = 0; byte < len; byte++){
		Dixom.Module.Bluetooth.MacAddr[byte] = data[byte];
	}
	//Delay(20, FreeRTOS);
	FSC_Bluetooth_Transmit((uint8_t *)"AT+STAT", 7, 0, FreeRTOS);
}

void FscModuleStat(uint8_t* data, short len){
	if(data[0] == '7'){
		Dixom.Module.Bluetooth.InMode = NotConnected;
	}else{
		Dixom.Module.Bluetooth.InMode = Connected;
	}
	//Delay(20, FreeRTOS);
	//FSC_Bluetooth_Transmit((uint8_t *)"AT+PLIST", 8, 0, FreeRTOS);
}

void FscModulePlist(uint8_t* data, short len){

	if(data[0]-49 == 0){
		Dixom.Module.Bluetooth.ConnDevInfo.PairCount = 0;
	}

	for(uint8_t i = 0; i < sizeof(Dixom.Module.Bluetooth.ConnDevInfo.PairListAddr[0]); i++){
		Dixom.Module.Bluetooth.ConnDevInfo.PairListAddr[data[0]-49][i] = 0;
	}

	uint16_t MaxNameSize = sizeof(Dixom.Module.Bluetooth.ConnDevInfo.PairListName[0]);

	for(uint8_t i = 0; i < MaxNameSize; i++){
		Dixom.Module.Bluetooth.ConnDevInfo.PairListName[data[0]-49][i] = 0;
	}

	Copy_ArrayToArray(data, 2, Dixom.Module.Bluetooth.ConnDevInfo.PairListAddr[data[0]-49],  0, 12, 0);
	Copy_ArrayToArray(data, 15, Dixom.Module.Bluetooth.ConnDevInfo.PairListName[data[0]-49],  0, len-15, MaxNameSize);

	Dixom.Module.Bluetooth.ConnDevInfo.PairCount++;

	if(Dixom.Module.Bluetooth.InMode == Connected && data[0]-49 == 0){
		sprintf((char *)Dixom.Module.Bluetooth.ConnDevInfo.MacAddr,"%s",  Dixom.Module.Bluetooth.ConnDevInfo.PairListAddr[0]);
		sprintf((char *)Dixom.Module.Bluetooth.ConnDevInfo.Name,"%s",  Dixom.Module.Bluetooth.ConnDevInfo.PairListName[0]);
	}
}

uint8_t BluetoothMsgBuf[255];
uint8_t BluetoothMsgLen = 0;;
uint8_t BtBias = 2;

void FscSppData(uint8_t* data, short len){

	if(len < 254){
		sscanf((char *)data, "%hhu", &BluetoothMsgLen);

		if(BluetoothMsgLen<10){ BtBias = 2;}
		else if(BluetoothMsgLen<100){BtBias = 3;}
		else{BtBias = 4;}

		for(uint8_t i = 0; i < BluetoothMsgLen; i++){
			BluetoothMsgBuf[i] = data[i+BtBias];
		}
		BluetoothMsgBuf[BluetoothMsgLen] = 0;

		//GATEWAY(&BluetoothMsgBuf[0]);

		if(Dixom.Exchange.Interface.Interface_selection_mode == AUTO_SELECT){
			Dixom.Exchange.Interface.Current_interface = BLUETOOTH_SPP;
		}

		main_Menu(&BluetoothMsgBuf[0]);
/*
		if(strncmp((char*)BluetoothMsgBuf,"MENU ",5) == 0){ //MAIN MENU

			main_Menu(&BluetoothMsgBuf[0]+5);

		}else if(strncmp((char*)BluetoothMsgBuf, "ARD ",4) == 0){ //ARDUINO

			BluetoothSignature();
			UART3_Transmit(&BluetoothMsgBuf[0]+4, BluetoothMsgLen-4, 0, FreeRTOS);

		}else if(strncmp((char*)BluetoothMsgBuf, "NEX ",4) == 0){ //NWXTION

			BluetoothSignature();
			NEXTION_LCD_Transmit(&BluetoothMsgBuf[0]+4, BluetoothMsgLen-4);

		}else if(strncmp((char*)BluetoothMsgBuf, "USB ",4) == 0){ //USB

			BluetoothSignature();

			if(BluetoothMsgLen>62){
				BluetoothMsgLen=58;
			}
			Transmit_Data_Out((uint8_t *)BluetoothMsgBuf+4, BluetoothMsgLen-4, 0, FreeRTOS, NO);

		}else if(strncmp((char*)BluetoothMsgBuf, "BLT ",4) == 0){ //BLUETOOTH

			BluetoothSignature();
			FSC_Bluetooth_Transmit(&BluetoothMsgBuf[0]+4, BluetoothMsgLen-4, 0, FreeRTOS);
		}*/
	}
}

void FscSppState(uint8_t* data, short len){

	if(data[0]=='3'){
		Dixom.Module.Bluetooth.Connection = Connected;

	}else{
		Dixom.Module.Bluetooth.Connection = NotConnected;
	}
	 GetCurrentExchangeInterface();
}

void BluetoothSignature(void){
	BluetoothMsgBuf[0] = 'B';
	BluetoothMsgBuf[1] = 'L';
	BluetoothMsgBuf[2] = 'T';
}





void Transmit_Long_Data_Out(uint8_t* StartSuf, uint8_t StartSufLen, uint8_t* INdata, uint8_t delay){

	uint8_t SendMsgLen = 0;
	uint8_t ReadPointer = 0;
	uint8_t SendUsbBuf[65];

	Copy_ArrayToArray(StartSuf, 0, SendUsbBuf,  0, StartSufLen, 0);

	for(uint16_t i = 0; i<500; i++){

		SendUsbBuf[SendMsgLen + StartSufLen] = INdata[ReadPointer];
		SendMsgLen++;

    	if (SendMsgLen>=58-StartSufLen){
    		uint8_t tempByte = SendUsbBuf[57];
    		SendUsbBuf[57]=255;
    		TransmitDataOutputs(SendUsbBuf, 58,  false,  true, allAvailable);

    		SendUsbBuf[0+StartSufLen] = 255;
    		SendUsbBuf[1+StartSufLen] = tempByte;
    		SendMsgLen=2;
    	}

		if(INdata[ReadPointer]==End_Mesage){
			i = 505;
		}

		ReadPointer++;

		if(ReadPointer>=BufSize) {
			ReadPointer = 0;
		}
	}

	if(SendMsgLen-1>1){

		TransmitDataOutputs(SendUsbBuf, StartSufLen+SendMsgLen-1,  false,  true, allAvailable);

	}
}



