/*
 * Eeprom.c
 *
 *  Created on: 14 февр. 2022 г.
 *      Author: 79825
 */
#include <Memory_Eeprom.h>
#include "DIX_i2c.h"
#include "Functions.h"
#include "Structures.h"

extern sDixom Dixom;

void EEPROM_Write_CheckSumm(uint32_t* flash_checkSum, uint32_t MemAddress, uint8_t* pData, uint16_t Size, uint16_t Timeout){

	*flash_checkSum = 0xFFFFFFFF;

	for (uint16_t byte = 4; byte < Size; byte++) {
		*flash_checkSum = Dixom.Service.crc_table[(*flash_checkSum ^ *(pData + byte)) & 0xFF] ^ (*flash_checkSum >> 8);
	}

	*flash_checkSum ^= 0xFFFFFFFF;

	Transmit_EEPROM(MemAddress, pData, Size, Timeout);
}


void EEPROM_Read_CheckSumm(uint32_t* read_checkSum, uint32_t MemAddress, uint8_t* pData, uint16_t Size, uint16_t Timeout){


	Receiver_EEPROM(MemAddress, pData, Size, Timeout);

	*read_checkSum = 0xFFFFFFFF;

	for (uint16_t byte = 4; byte < Size; byte++) {
		*read_checkSum = Dixom.Service.crc_table[(*read_checkSum ^ *(pData + byte)) & 0xFF] ^ (*read_checkSum >> 8);
	}

	*read_checkSum ^= 0xFFFFFFFF;
}



void Transmit_EEPROM(uint16_t MemAddress, uint8_t *pData,  uint16_t Size, uint16_t Timeout){

	uint16_t Page_Size = 64;	                                             // размер одной страницы EEPROM
	uint16_t Max_First_Page_Len = Page_Size - (MemAddress % Page_Size);	     // Остаток свободных ячеек на странице в указаном адресе EEPROM
	uint16_t Write_Data_Len;                                                 // Размер записываемых данных на страницу в EEPROM
	uint16_t General_Data_Len = 0;                                           // Общее количество записанных данных

	if(Size>Max_First_Page_Len && Max_First_Page_Len != Page_Size){
		Write_Data_Len = Max_First_Page_Len;                                 // Если размер дынных больше чем остаток свободных ячеек, то записываем на все свободные ячейки
	}else{
		if(Size>Page_Size){
			Write_Data_Len = Page_Size;                                      // Если размер данных больше чем размер страницы то записываем всю страницу
		}else{
			Write_Data_Len = Size;                                           // Если размер дынных мельше чем размер страницы то записываем остаток данных
		}
	}

	while (Write_Data_Len){

		ExchangeStatus(	Transmit_I2C2(DEVICE_ADDR_EEPROM_24LC256 << 1, MemAddress+General_Data_Len, 2, pData+General_Data_Len, Write_Data_Len, 200, ExchangeMemMainSteam),	EEPROM_WRITE);

		Delay(10, FreeRTOS);
		General_Data_Len = General_Data_Len+Write_Data_Len;                  // Указываем общее количество записанных данных
		Size = Size - Write_Data_Len;                                        // Убираем записанные данные из размера

		if(Size>Page_Size){
			Write_Data_Len = Page_Size;                                      // Если размер данных больше чем размер страницы то записываем всю страницу
		}else{
			Write_Data_Len = Size;                                           // Если размер дынных мельше чем размер страницы то записываем остаток данных
		}
	}
}

WorkRes Receiver_EEPROM( uint16_t MemAddress,  uint8_t* pData, uint16_t Size, uint16_t Timeout){

	ExchangeStatus(	Receiver_I2C2(DEVICE_ADDR_EEPROM_24LC256 << 1, MemAddress, 2, pData,	Size, Timeout, ExchangeMemMainSteam), EEPROM_READ);

	return WR_ERROR;
}

WorkRes Receiver_EEPROM_DMA( uint16_t MemAddress,  uint8_t* pData, uint16_t Size, uint16_t Timeout){

	ExchangeStatus(	Receiver_I2C2(DEVICE_ADDR_EEPROM_24LC256 << 1, MemAddress, 2, pData,	Size, Timeout, ExchangeMemMainSteamDMA), EEPROM_READ);

	return WR_ERROR;
}


