#include <Memory_Stm.h>
#include "stm32f4xx_hal.h"
#include <Structures.h>

extern sDixom Dixom;

FLASH_EraseInitTypeDef EraseInitStruct;

WorkRes  Init_StmFlash(){

	EraseInitStruct.TypeErase    = FLASH_TYPEERASE_SECTORS;
 	EraseInitStruct.VoltageRange = FLASH_VOLTAGE_RANGE_3;
 	return WR_OK;
}

WorkRes Flash_Unlock(){

	WorkRes status = WR_ERROR;
	uint8_t attempt = NumAttempt;

	while (status != WR_OK || attempt !=0 ) {
		attempt--;
		status = ExchangeStatus( HAL_FLASH_Unlock(), STM_FLASH_UNCLOCK);
	}
	return status;
}

WorkRes Flash_Lock(){

	WorkRes status = WR_ERROR;
	uint8_t attempt = NumAttempt;

	while (status != WR_OK || attempt !=0 ) {
		attempt--;
		status = ExchangeStatus( HAL_FLASH_Lock(), STM_FLASH_LOCK);
	}
	return status;
}

WorkRes Flash_Erase(uint32_t sector, uint8_t numLis){

	WorkRes status = WR_ERROR;
	uint8_t attempt = NumAttempt;
	uint32_t error = WR_ERROR;

	EraseInitStruct.Sector       = sector;
	EraseInitStruct.NbSectors    = numLis;

	while (status != WR_OK || attempt !=0 ) {
		attempt--;
		status = ExchangeStatus( HAL_FLASHEx_Erase(&EraseInitStruct, &error), STM_FLASH_ERASE);
	}
	return status;
}

WorkRes FlashWrite(uint32_t flash_address, uint32_t* data_address, uint16_t length){

	WorkRes status = WR_OK;

	for (uint16_t word = 0; word < length; word++) {
		status = ExchangeStatus( HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, flash_address + (word*4), *(data_address + word)), STM_FLASH_WRITE);
	}
	return status;
}

WorkRes FlashRead(uint32_t flash_address, uint32_t* data_address, uint16_t length){

	for (uint16_t word=0; word < length; ++word) {
		uint32_t data =  *(__IO uint32_t*)(flash_address + word*4);
		*(data_address + word) = data;
	}
	ExchangeStatus( WR_OK, STM_FLASH_READ);

	return WR_OK;
}

/*
 * FlashWriteCRC_Check function for writing structures to the STM32 internal flash memory
 *
 * flash_checkSum link - where the calculated checksum will be stored, which will be written to the STM32 flash memory.
 * flash_address - is the stm32 flash address where the structure will be stored.
 * data_address - is a link to the structure to be saved.
 * length - structure size
 */

WorkRes FlashWriteCRC_Check(uint32_t *flash_checkSum, uint32_t flash_address, uint32_t* data_address, uint16_t length){

	WorkRes Result = WR_OK;

	//Checksum calculation before writing structure to STM32 flash memory
	*flash_checkSum = 0xFFFFFFFF;
	for (uint16_t word = 1; word < length; word++) {
		*flash_checkSum = Dixom.Service.crc_table[(*flash_checkSum ^ *(data_address + word)) & 0xFF] ^ (*flash_checkSum >> 8);
	}
	*flash_checkSum ^= 0xFFFFFFFF;

	//Write structure to STM32 flash memory with error monitoring.
	for (uint16_t word = 0; word < length; word++) {
		if(ExchangeStatus( HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, flash_address + (word*4), *(data_address + word)), STM_FLASH_WRITE) != WR_OK){
			Result = WR_ERROR;
		}
	}
	return Result;
}

void FlashReadCRC_Check(uint32_t *read_checkSum, uint32_t flash_address, uint32_t* data_address, uint16_t length){

	*read_checkSum = 0xFFFFFFFF;

	for (uint16_t word=0; word < length; ++word) {
		uint32_t data =  *(__IO uint32_t*)(flash_address + word*4);
		*(data_address + word) = data;
		if(word>0){
			*read_checkSum = Dixom.Service.crc_table[(*read_checkSum ^ data) & 0xFF] ^ (*read_checkSum >> 8);
		}
	}
	*read_checkSum ^= 0xFFFFFFFF;
}

void Write_DeviceInfo(){

	uint8_t attempt = NumAttempt;
	Flash_Unlock();
	Flash_Erase(2, 1);
	Flash_Unlock();
	do{
		attempt--;
	}while(FlashWriteCRC_Check( &Dixom.DevInfo.CheckSum, FLASH_ADDR_DEVICE_INFO, (void *)&Dixom.DevInfo, sizeof(Dixom.DevInfo)/4) != WR_OK || attempt !=0 );
	Flash_Lock();
}























