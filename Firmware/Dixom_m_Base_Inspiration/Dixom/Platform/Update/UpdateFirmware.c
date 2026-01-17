#include <Constants.h>
#include <Description.h>
#include <Memory_Spi.h>
#include <Structures.h>
#include <UpdateFirmware.h>
#include "GPIO.h"
#include "Functions.h"

extern sDixom Dixom;
#define MIN_APPLICATION_ADDRESS        0x08020000              //↓ Адрес начала программы  134742016 защи от стирания загрузчика
#define UpdateInfo Dixom.DevInfo.Firmware_UpdateInfo

uint8_t   FlashSectorData[256];

uint16_t  counter;
uint16_t  FlashSector;
uint8_t   sendData = 0;
uint8_t   FrimwareUpdateLock = 0,
          UpdateFlag     = 20;

uint32_t  RealFrimwareSize,
          CriptFrimwareSize,
          FirmwareSizeCounter,
          CheckCRC,
          FrimwareCRC,
          CalculateCRC;
//

void  SYSTEM(uint8_t ControlByte, uint8_t Function, uint8_t *Received_data){
	//FU FIRMWARE UPDATE
         if(Function == SYSTEM_FU_FIRMWARE_DOWNLOAD) {  FIRMWARE_DOWNLOAD (Received_data);                       } //
    else if(Function == SYSTEM_FU_USB_SOUND_CARD)    {  USB_AUDIO_CARD_STRING_CONTROL(Received_data);            } //
	else if(Function == SYSTEM_INFORMATION)          {  DIXOM_VERSION();                                         } //
    else if(Function == SYSTEM_FU_PREPARING_UPDATE)  {  PREPARING_UPDATE (Received_data);                        } //
    else if(Function == SYSTEM_ACCESS_CONTROL)       {  UPDATE_SYSTEM_UNLOCK(Received_data);                     } //
    else if(Function == SYSTEM_FU_CHECK_FIRMWARE)    {  CHECK_FIRMWARE();                                        } //
    else if(Function == SYSTEM_FU_COMPLETE_UPDATE)   {  COMPLETE_UPDATE();                                       } //
    else if(Function == SYSTEM_FU_CANCEL)            {  UPDATE_CANCEL();                                         } //
    else if(Function == SYSTEM_FU_BOOTLOADER)        {  ProtectionBootUpdate();  	                             } //
	else if(Function == SYSTEM_SPI_FLASH_ERASE)      {  ProtectionSpiFlashErase();                               } //
	else if(Function == SYSTEM_LICENSE_KEY)          {  ProtectionSetKey(Received_data);                         } //
	else if(Function == SYSTEM_PCB_REVISION)         {  ProtectionSetPcb(Received_data);                         } //
	else if(Function == SYSTEM_RESTART)              {  PlatformRestart(FreeRTOS);                               } //
	else if(Function == SYSTEM_EXCHANGE_INTERFACE)   {  CURRENT_EXCHANGE_INTERFACE(ControlByte, Received_data);  } //0 5
	else if(Function == SYSTEM_CONNECTION)           {  ConnectionSetup(ControlByte, Received_data);             } //0 5
    else{
 		TransmitDataOutputs((uint8_t *)GeneralNotifi_NoPackageFunction, AutoLenght,  false,  true, allAvailable);
    }
}

void ConnectionSetup(uint8_t ControlByte, uint8_t *Received_data){
	if(Received_data[0] == (usbHid+48)){
		Dixom.Exchange.State.USB.ConnectionHid = NotConnected;
	}else if(Received_data[0] == (USB_CDC+48)){
		Dixom.Exchange.State.USB.ConnectionCdc = NotConnected;
	}else if(Received_data[0] == (BLUETOOTH_SPP+48)){
	}
	GetCurrentExchangeInterface();
}


void ProtectionBootUpdate(void){
	if(FrimwareUpdateLock == 1){
		BootUpdate();

	}else{
 		TransmitDataOutputs((uint8_t *)SystemNotifi_SysCommandBlocked, AutoLenght,  false,  true, allAvailable);
	}
}

void ProtectionSpiFlashErase(void){
	if(FrimwareUpdateLock == 1){
		SpiFlashErase();
	}else{
  		TransmitDataOutputs((uint8_t *)SystemNotifi_SysCommandBlocked, AutoLenght,  false,  true, allAvailable);

	}
}

void ProtectionSetKey(uint8_t *Received_data){
	if(FrimwareUpdateLock == 1){
		DIXOM_KEY(Received_data);
	}else{
  		TransmitDataOutputs((uint8_t *)SystemNotifi_SysCommandBlocked, AutoLenght,  false,  true, allAvailable);
	}
}

void ProtectionSetPcb(uint8_t *Received_data){
	if(FrimwareUpdateLock == 1){
		DIXOM_PCB(Received_data);
	}else{
  		TransmitDataOutputs((uint8_t *)SystemNotifi_SysCommandBlocked, AutoLenght,  false,  true, allAvailable);
	}
}



void  FIRMWARE_DOWNLOAD(uint8_t *Received_data){
	if(FrimwareUpdateLock == 1){
		for (short i = 0; i<58;i++){
			FlashSectorData[counter]=Received_data[i];
			FirmwareSizeCounter++;
			counter++;
			if (counter==256){
		       FlashMemoryWrite(FlashSector, &(FlashSectorData[0]), 256);
			   counter=0;
			   FlashSector++;
			}
		}
  		TransmitDataOutputs((uint8_t *)MessageUpdate_2_1_16, AutoLenght,  false,  true, allAvailable);

 	}else{
  		TransmitDataOutputs((uint8_t *)MessageUpdate_0_1_20, AutoLenght,  false,  true, allAvailable);

 	}
}




void  PREPARING_UPDATE(uint8_t *Received_data){

	if(FrimwareUpdateLock == 1){
		UpdateInfo.Firmware_Size   =  Received_data[0]  |(Received_data[1] << 8) |(Received_data[2] << 16) |(Received_data[3] << 24);     //↓ Получаем реальный размер прошивки
		CriptFrimwareSize          =  Received_data[4]  |(Received_data[5] << 8) |(Received_data[6] << 16) |(Received_data[7] << 24);     //↓ Получаемразмер прошивки в зашифрованной форме
		CheckCRC                   =  Received_data[8]  |(Received_data[9] << 8)|(Received_data[10] << 16)|(Received_data[11] << 24);    //↓ Получаем кнотрольную сумму расчитанную в VisualStudio для проверки
		UpdateInfo.Frimware_CRC    =  Received_data[12] |(Received_data[13] << 8)|(Received_data[14] << 16)|(Received_data[15] << 24);    //↓ Получаем контрольную сумму расшифрованной прошивки для бутлоадера//Получаем контрольную сумму расшифрованной прошивки для бутлоадера
		UpdateInfo.ApplicationAddr =  Received_data[16] |(Received_data[17] << 8)|(Received_data[18] << 16)|(Received_data[19] << 24);    //↓ Получаем адрес рассположения оснвоной прошивки
		UpdateInfo.Erase_Sector    =  Received_data[20];                                                                         //↓ Получаем начальны сектор откуда начинаем стирать.
		UpdateInfo.Erase_Count     =  Received_data[21];                                                                         //↓ Получаем данные сколько нужно стирать
		if(UpdateInfo.ApplicationAddr < MIN_APPLICATION_ADDRESS || UpdateInfo.Erase_Sector <2 || UpdateInfo.Erase_Count>10){
 	  		TransmitDataOutputs((uint8_t *)MessageUpdate_0_1_19, AutoLenght,  false,  true, allAvailable);

		}else{
			FlashMemoryWriteEnable();                                                                                        //↓ Включаем запись во влешь память
			for(short i=0; i<255; i++){                                                                                      //↓
				FlashSectorErase(i);                                                                                         //↓ Полностью очищаем флешь микросхему
			}                                                                                                                //↓			                                                                                                                         //
			FirmwareSizeCounter=0;                                                                                           //↓ Очищаем счётчик прошивки
			counter=0;                                                                                                       //↓ Очищаем счётчик пачки прошивки
			FlashSector=0;                                                                                                   //↓ Очищаем счётчик секторов
			FlashMemoryWriteEnable();                                                                                        //↓ Включаем запись во влешь память
			TransmitDataOutputs((uint8_t *)MessageUpdate_2_1_0, AutoLenght,  false,  true, allAvailable);                    //↓ Отправляем сообщение в USB что готовы принимать прошивку
		}
	}else{
 		TransmitDataOutputs((uint8_t *)SystemNotifi_SysCommandBlocked, AutoLenght,  false,  true, allAvailable);
	}
}




void  UPDATE_SYSTEM_UNLOCK(uint8_t *Received_data){
	if(Received_data[0] ==  '5' && Received_data[1] ==  '1' && Received_data[2] =='0' && Received_data[3] =='2' && Received_data[4] =='8' && Received_data[5] =='6'){
		Dixom.DevInfo.InMode = FirmwareUpdate;
		FrimwareUpdateLock = 1;
  		TransmitDataOutputs((uint8_t *)MessageUpdate_2_1_9, AutoLenght,  false,  true, allAvailable);

	}else{
		FrimwareUpdateLock = 0;
		Dixom.DevInfo.InMode = ReadyForWork;
  		TransmitDataOutputs((uint8_t *)MessageUpdate_2_1_10, AutoLenght,  false,  true, allAvailable);

 	}
}



void UPDATE_CANCEL(){
	if(FrimwareUpdateLock == 1){
		Dixom.DevInfo.InMode = ReadyForWork;
  		TransmitDataOutputs((uint8_t *)MessageUpdate_2_1_8, AutoLenght,  false,  true, allAvailable);

 	}else{
  		TransmitDataOutputs((uint8_t *)SystemNotifi_SysCommandBlocked, AutoLenght,  false,  true, allAvailable);

 	}
}

void CHECK_FIRMWARE(){

	if (FirmwareSizeCounter>=CriptFrimwareSize){
		FlashMemoryWriteDisable();
	    CalculateCRC = 0xFFFFFFFF;
		for(uint32_t i=0;i<CriptFrimwareSize;i+=256){
			FlashMemoryRead( i/256, &(FlashSectorData[0]), 256);
			for (uint16_t y= 0; y<256;y++){
				CalculateCRC = Dixom.Service.crc_table[(CalculateCRC ^ FlashSectorData[y]) & 0xFF] ^ (CalculateCRC >> 8);
			}
 	    }
		CalculateCRC ^= 0xFFFFFFFF;

    	if( CheckCRC == CalculateCRC){
       		TransmitDataOutputs((uint8_t *)MessageUpdate_2_1_1, AutoLenght,  false,  true, allAvailable);

    	}else{
       		TransmitDataOutputs((uint8_t *) MessageUpdate_2_1_2, AutoLenght,  false,  true, allAvailable);

    	}
	} else {
   		TransmitDataOutputs((uint8_t *)MessageUpdate_2_1_3, AutoLenght,  false,  true, allAvailable);

	}
}

void COMPLETE_UPDATE(){
	if(FrimwareUpdateLock == 1){
		if( CheckCRC == CalculateCRC){
			FlashMemoryWriteEnable();
			FlashSectorErase(255);
			FlashMemoryWriteEnable();
			UpdateInfo.State = New_Frimware;
			Spi_Flash_Write_CheckSumm(&UpdateInfo.CheckSum, SpiFlashAddrUpdateInfo, (uint32_t*)&UpdateInfo, sizeof(UpdateInfo)/4);
			FlashMemoryWriteDisable();
 	   		TransmitDataOutputs((uint8_t *)MessageUpdate_2_1_7, AutoLenght,  false,  true, allAvailable);

			Delay(1000, FreeRTOS);
			PlatformRestart(FreeRTOS);
		}else{
 	   		TransmitDataOutputs((uint8_t *)MessageUpdate_2_1_2, AutoLenght,  false,  true, allAvailable);

			UPDATE_CANCEL();
		}
	}else{
    		TransmitDataOutputs((uint8_t *)SystemNotifi_SysCommandBlocked, AutoLenght,  false,  true, allAvailable);

	}

}





