#include <Description.h>
#include <Memory_Spi.h>
#include "GPIO.h"
#include "Exchange.h"
#include "stdint.h"
#include <Structures.h>

extern sDixom Dixom;

union {
	uint8_t all;
    STATUS_REG1_STRUCT_t bit;
} FlashMemoryRegStatus;

uint8_t W25_READ_STATUS_1[1] = {0x05};
uint8_t W25_CHIP_ERASE[1]    = {0xC7};
uint8_t W25_WRITE_DISABLE[1] = {0x04};
uint8_t W25_WRITE_ENABLE[1]  = {0x06};

short FlashMemoryTimeOut = 1000;

void FlashMemoryWrite(short page, uint8_t* data, short len){
	uint32_t addr =page*256;
	uint8_t WriteAddr[4]  = {0x02, (addr >> 16) & 0xFF, (addr >> 8) & 0xFF, addr & 0xFF};
	FlashMemoryBusyStatus();
	if(FlashMemoryRegStatus.bit.write_enable==0) {
		FlashMemoryWriteEnable();
	}
	Switch_CS_FLASH(OFF);
	Transmit_SPI2(WriteAddr, 4, FlashMemoryTimeOut, ExchangeMainSteam);
	Transmit_SPI2(data, len, FlashMemoryTimeOut, ExchangeMainSteam);
	Switch_CS_FLASH(ON);
}

void FlashMemoryRead(short page, uint8_t* readData, short readLen){
	uint32_t addr =page*256;
	uint8_t ReadAddr[4]  = {0x03,(addr >> 16) & 0xFF, (addr >> 8) & 0xFF, addr & 0xFF};
	FlashMemoryBusyStatus();
	Switch_CS_FLASH(OFF);
	Transmit_SPI2(ReadAddr, 4, FlashMemoryTimeOut, ExchangeMainSteam);
	Receiver_SPI2 (NOT_USED, readData,  readLen, FlashMemoryTimeOut, ExchangeMainSteam);
	Switch_CS_FLASH(ON);
}

void FlashRegistrWrite(uint8_t* data, short len){
	Switch_CS_FLASH(OFF);
	Transmit_SPI2(data, len, FlashMemoryTimeOut, ExchangeMainSteam);
	Switch_CS_FLASH(ON);
}

void FlashRegistrRead(uint8_t* sendData, short sendLen, uint8_t* readData, short readLen){
	Switch_CS_FLASH(OFF);
	Transmit_SPI2(sendData, sendLen, FlashMemoryTimeOut, ExchangeMainSteam);
	Receiver_SPI2 (NOT_USED, readData, readLen, FlashMemoryTimeOut, ExchangeMainSteam);
	Switch_CS_FLASH(ON);
}

void FlashSectorErase(short sector){

	uint32_t addr =sector*4096;
	uint8_t EraseAddr[4]  = {0x20, (addr >> 16) & 0xFF, (addr >> 8) & 0xFF, addr & 0xFF};
	FlashMemoryBusyStatus();
	if(FlashMemoryRegStatus.bit.write_enable==0){
		FlashMemoryWriteEnable();
	}
	Switch_CS_FLASH(OFF);
	Transmit_SPI2(EraseAddr, 4, FlashMemoryTimeOut, ExchangeMainSteam);
	Switch_CS_FLASH(ON);
}

void FlashMemoryErase(void){
	FlashMemoryBusyStatus();
	if(FlashMemoryRegStatus.bit.write_enable==0)  FlashMemoryWriteEnable();
	FlashRegistrWrite(W25_CHIP_ERASE, 1);
}

void FlashMemoryWriteEnable(void){
	do{
		FlashRegistrWrite(W25_WRITE_ENABLE,  1);
		FlashRegistrRead(W25_READ_STATUS_1, 1, &FlashMemoryRegStatus.all, 1);
	}while(FlashMemoryRegStatus.bit.write_enable==0);
}

void FlashMemoryWriteDisable(void){
	do{
		FlashRegistrWrite(W25_WRITE_DISABLE, 1);
		FlashRegistrRead(W25_READ_STATUS_1, 1, &FlashMemoryRegStatus.all, 1);
	}while(FlashMemoryRegStatus.bit.write_enable==1);
}


void FlashMemoryBusyStatus(void){
	do{
		FlashRegistrRead(W25_READ_STATUS_1, 1, &FlashMemoryRegStatus.all, 1);
	}while(FlashMemoryRegStatus.bit.busy==1);
}

void SpiFlashErase(){
	FlashMemoryWriteEnable();
	for(short i=0; i<256; i++){
	  FlashSectorErase(i);
    }
	TransmitDataOutputs((uint8_t *)Basis_Notification_1_8_0, AutoLenght,  false,  true, allAvailable);

}

void Spi_Flash_Write_CheckSumm(uint32_t *flash_checkSum, uint32_t page_address, uint32_t* data_address, uint16_t length){

	uint16_t PageLen = 256; //Flash page len
	uint8_t VarType = 4;    //4 - uint32_t, 2 - uint16_t, 1 - uint8_t
	uint8_t WriteData[256];
	uint16_t WriteDataLen;
	uint32_t data;
	uint8_t NumSector = length/(PageLen/VarType);
	uint16_t RemainLen = length;
	*flash_checkSum = 0xFFFFFFFF;

	for (uint16_t word = 1; word < length; word++) {
		*flash_checkSum = Dixom.Service.crc_table[(*flash_checkSum ^ *(data_address + word)) & 0xFF] ^ (*flash_checkSum >> 8);
	}
	*flash_checkSum ^= 0xFFFFFFFF;
	for(uint8_t page = 0; page <= NumSector; page++){
		WriteDataLen = 0;
		for(uint16_t byte = 0; byte < RemainLen; byte++){
			if((PageLen/VarType)>byte){
				data = *((data_address + byte)+page*(PageLen/VarType));
				Converter_u32_to_u8(data, &WriteData[byte*4]);
				WriteDataLen++;
			}else{
				byte = length;
				RemainLen = RemainLen-(PageLen/VarType);
			}
		}
		FlashMemoryWrite(page+page_address, &WriteData[0], PageLen);
	}
}


void Spi_Flash_Read_CheckSumm(uint32_t *read_checkSum, uint32_t page_address, uint32_t* data_address, uint16_t length){

	uint16_t PageLen = 256;   //Flash page len
	uint8_t VarType = 4;      //4 - uint32_t, 2 - uint16_t, 1 - uint8_t
	uint8_t ReadData[256];
	uint32_t data;
	uint8_t NumSector = length/(PageLen/VarType);
	uint16_t RemainLen = length;
	*read_checkSum = 0xFFFFFFFF;
	for(uint8_t page = 0; page <= NumSector; page++){
		FlashMemoryRead((page+page_address), &ReadData[0], PageLen);
		for(uint16_t byte = 0; byte < RemainLen; byte++){
			if((PageLen/VarType)>byte){
				data = Converter_8_to_u32(&ReadData[byte*4]);
				*((data_address + byte)+page*(PageLen/VarType)) = data;
				if(page!=0 || byte!=0){
					*read_checkSum = Dixom.Service.crc_table[(*read_checkSum ^ data) & 0xFF] ^ (*read_checkSum >> 8);
				}
			}else{
				RemainLen = RemainLen-(PageLen/VarType);
				byte = RemainLen;
			}
		}
	}
	*read_checkSum ^= 0xFFFFFFFF;
}






