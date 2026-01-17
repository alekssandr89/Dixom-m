#include <Description.h>
#include <Information.h>
#include <Memory_Eeprom.h>
#include "Exchange.h"
#include "licens.h"
#include "Functions.h"
#include "BootloaderVersion.h"
#include "Structures.h"
#include <stdlib.h>

extern sDixom Dixom;

uint16_t FrimwareVer = 5006;
#define click System_Info_1_3_2

#define DevType  "Platform"
#define DevModel "Dixom-m"
#define FirmName "Inspiration"

void Initialization_Firmware_Information(){

	Dixom.DevInfo.dVerFrimware = FrimwareVer;

	Dixom.DevInfo.Firmware_UpdateInfo.ApplicationAddr = MAIN_APLICATION_ADDR;

	sprintf((char *)Dixom.DevInfo.VerFrimware,"%.3f", Dixom.DevInfo.dVerFrimware*0.001);
	sprintf((char *)Dixom.DevInfo.VerBootloader,"%.2f", Dixom.DevInfo.Firmware_UpdateInfo.Boot_VerFrimware*0.01);

	Dixom.DevInfo.dVerFrimware = FrimwareVer;
	Dixom.DevInfo.dType = Device_Type_Platform;
	Dixom.DevInfo.dModel = Model_DIXOM_m;
	Dixom.DevInfo.dFrimwareName = FirmwareName_AsIwant;

	sprintf((char *)Dixom.DevInfo.Model,"%s", DevModel);
	sprintf((char *)Dixom.DevInfo.Type,"%s", DevType);
	sprintf((char *)Dixom.DevInfo.FirmwareName,"%s", FirmName);

}

void Initialization_Device_Information(){

	uint8_t ForProcessing[42];

	Receiver_EEPROM( EEPROM_ADDR_PCB_REV, Dixom.DevInfo.RevPCB, 3, 100 );

	ForProcessing[0] = Dixom.DevInfo.RevPCB[0];
	ForProcessing[1] = Dixom.DevInfo.RevPCB[2];
	ForProcessing[2] = 0;

	Dixom.DevInfo.dRevPCB = atoi ((char*)ForProcessing);

	Update_AudioCard_Information(NO);

	//Dixom.Module.Bluetooth.dModel = NoBluetoothDevice;
	//Dixom.Module.Bluetooth.ConnDevInfo.A2dpDecoder = BTDecoder_NO;

	//Dixom.Module.Bluetooth.dModel = BLUETOOTH_FSC_BT802;

	//FSC_Bluetooth_Transmit((uint8_t *)"AT+VER", 6, 0, FreeRTOS);
}


void DIXOM_PCB(uint8_t ControlByte,  uint8_t* data){
	if(ControlByte == DATA_SET){ //SET
		Transmit_EEPROM(EEPROM_ADDR_PCB_REV, data, 4, 10 );
	}else{
 	 	 TransmitDataOutputs((uint8_t*)GeneralNotifi_CtlBitNotSupported, AutoLenght,  false,  true, allAvailable);

	}
}



void DIXOM_VERSION(){

	sprintf((char *)Dixom.DevInfo.VerFrimware,"%.3f", Dixom.DevInfo.dVerFrimware*0.001);
	sprintf((char *)Dixom.DevInfo.VerBootloader,"%.2f", Dixom.DevInfo.Firmware_UpdateInfo.Boot_VerFrimware*0.01);

	sprintf((char *)Dixom.Buff, System_Info_1_3_0,
			Dixom.DevInfo.dType,
			Dixom.DevInfo.dModel,
			Dixom.DevInfo.dFrimwareName,
			Dixom.DevInfo.VerFrimware,
			Dixom.DevInfo.VerBootloader,
			Dixom.DevInfo.RevPCB,
			Dixom.DevInfo.Type,
			Dixom.DevInfo.Model,
			Dixom.DevInfo.FirmwareName);
	 TransmitDataOutputs(Dixom.Buff, AutoLenght,  false,  true, allAvailable);

	sprintf((char *)Dixom.Buff, System_Info_1_3_1,
			Dixom.DevInfo.SerialNumber,
			Dixom.DevInfo.LicenseKey);
	 TransmitDataOutputs(Dixom.Buff, AutoLenght,  false,  true, allAvailable);

	sprintf((char *)Dixom.Buff, System_Info_1_3_2,
			Dixom.DevInfo.PlatormLife.UseExperimentalFunctions,
			Dixom.DevInfo.PlatormLife.Num_Flash_Write,
			Dixom.DevInfo.PlatormLife.Num_Eeprom_Write,
			Dixom.DevInfo.PlatormLife.Working_Hours);
	 TransmitDataOutputs(Dixom.Buff, AutoLenght,  false,  true, allAvailable);

	Get_AudioCard_Information();

	GetCurrentExchangeInterface();

	sprintf((char *)Dixom.Buff, System_Info_1_3_4,
			Dixom.Module.AudioDAC.DAC_Type,
			Dixom.Module.AudioDAC.Num_Chips,
			Dixom.Module.AudioDAC.Num_Channel);
	 TransmitDataOutputs(Dixom.Buff, AutoLenght,  false,  true, allAvailable);

	if(Dixom.Module.Bluetooth.dModel != NoBluetoothDevice){
		sprintf((char *)Dixom.Buff, MaskGetInfoBtModule, Dixom.Module.Bluetooth.dModel, Dixom.Module.Bluetooth.MacAddr);
	 	 TransmitDataOutputs(Dixom.Buff, AutoLenght,  false,  true, allAvailable);
		sprintf((char *)Dixom.Buff, MaskGetInfoBtName, Dixom.Module.Bluetooth.Name);
	 	 TransmitDataOutputs(Dixom.Buff, AutoLenght,  false,  true, allAvailable);
		sprintf((char *)Dixom.Buff, MaskGetInfoBtPass, Dixom.Module.Bluetooth.Pass);
	 	 TransmitDataOutputs(Dixom.Buff, AutoLenght,  false,  true, allAvailable);
	}

	if(Dixom.Module.Bluetooth.InMode == Connected){

		sprintf((char *)Dixom.Buff, MaskGetInfoBtDevConn, Dixom.Module.Bluetooth.InMode);
	 	 TransmitDataOutputs(Dixom.Buff, AutoLenght,  false,  true, allAvailable);

		if(Dixom.Module.Bluetooth.ConnDevInfo.MacAddr[0]!=0){
			sprintf((char *)Dixom.Buff, MaskGetInfoBtDevAddr, Dixom.Module.Bluetooth.ConnDevInfo.MacAddr);
		 	 TransmitDataOutputs(Dixom.Buff, AutoLenght,  false,  true, allAvailable);
		}
		if(Dixom.Module.Bluetooth.ConnDevInfo.Name[0]!=0){
			sprintf((char *)Dixom.Buff, MaskGetInfoBtDevName, Dixom.Module.Bluetooth.ConnDevInfo.Name);
		 	 TransmitDataOutputs(Dixom.Buff, AutoLenght,  false,  true, allAvailable);
		}
		if(Dixom.Module.Bluetooth.ConnDevInfo.NetworkName[0]!=0){
			sprintf((char *)Dixom.Buff, MaskGetInfoBtDeNetName, Dixom.Module.Bluetooth.ConnDevInfo.NetworkName);
		 	 TransmitDataOutputs(Dixom.Buff, AutoLenght,  false,  true, allAvailable);
		}

	}


	sprintf((char *)Dixom.Buff, MaskNewBootloader, VER_BOOTLOADER * 0.01);
	TransmitDataOutputs(Dixom.Buff, AutoLenght,  false,  true, allAvailable);

}

typedef enum  {
	  totalInfo, //0
	  detailInfo, // 1
	  done, //2
}ExchangeLog;

void EXHANGE_LOG(uint8_t ControlByte, uint8_t *Received_String){

	uint8_t exchangeLog = Received_String[0]-48;

	if(exchangeLog == totalInfo){
		sprintf((char*) Dixom.Buff, General_1_Log_0,
				Dixom.Exchange.Logs.Detail.PresenceOfErrors,
				Dixom.Exchange.Logs.Detail.InterfacesInAction,
				Dixom.Exchange.Logs.Detail.Count_Total_WR_ERROR,
				Dixom.Exchange.Logs.Detail.Count_Total_WR_BUSY,
				Dixom.Exchange.Logs.Detail.Count_Total_WR_TIMEOUT,
				Dixom.Exchange.Logs.Detail.Count_Total_WR_OK,
				Dixom.Exchange.Logs.Detail.Count_Total_WR_QUEUE_TIMEOUT,
				Dixom.Exchange.Logs.Detail.Count_Total_WR_NO_CONNECTION);
	 	 TransmitDataOutputs(Dixom.Buff, AutoLenght,  false,  true, allAvailable);
	}else if(exchangeLog == detailInfo){
		for(uint8_t iCode = 0; iCode<NumInterfaceCode; iCode++){
			if(Dixom.Exchange.Logs.Detail.Interface[iCode].Code != EMPTY){
				sprintf( (char*)Dixom.Buff,General_1_Log_1,
						Dixom.Exchange.Logs.Detail.Interface[iCode].Code,
						Dixom.Exchange.Logs.Detail.Interface[iCode].Count_WR_ERROR,
						Dixom.Exchange.Logs.Detail.Interface[iCode].Count_WR_BUSY,
						Dixom.Exchange.Logs.Detail.Interface[iCode].Count_WR_TIMEOUT,
						Dixom.Exchange.Logs.Detail.Interface[iCode].Count_WR_OK,
						Dixom.Exchange.Logs.Detail.Interface[iCode].Count_WR_QUEUE_TIMEOUT,
						Dixom.Exchange.Logs.Detail.Interface[iCode].Count_WR_NO_CONNECTION);
			 	 TransmitDataOutputs(Dixom.Buff, AutoLenght,  false,  true, allAvailable);
			}
		}
 	 	 TransmitDataOutputs((uint8_t*)General_1_Log_2, AutoLenght,  false,  true, allAvailable);

	}

}




