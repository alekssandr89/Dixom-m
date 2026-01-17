/*
 * DSP_Exchange.c
 *
 *  Created on: 16 но€б. 2021 г.
 *      Author: 79825
 */
#include <Constants.h>
#include <Memory_Spi.h>
#include <Memory_Stm.h>
#include "Exchange.h"
#include "DSP_Exchange.h"
#include "GPIO.h"
#include "DSP.h"
#include <Structures.h>

extern sDixom Dixom;

#define dDSP Dixom.Module.DSP


void Transmit_Sigma( uint16_t devAddress, uint32_t podAddress, uint32_t dataLen, ADI_REG_TYPE *data){

	uint16_t maxLen  = 25;
	uint16_t count   = 4;
	uint16_t i = dataLen/(maxLen*count);
	uint16_t j = dataLen%(maxLen*count);
	for (uint16_t step = 0; step < i; step++) {
		Transmit_DSP((podAddress + maxLen * step), (uint8_t *)(data + maxLen*step*count), (maxLen*count), 1500);
	}
	if (j>0){
		Transmit_DSP((podAddress + maxLen * i), (uint8_t *)(data + maxLen*i*count), j, 1500);
	}
}

void Transmit_Sigma1701( uint16_t devAddress, uint16_t podAddress, uint16_t dataLen, ADI_REG_TYPE *data){

	uint16_t maxLen  = 25;
	uint16_t count   = 4;
	uint16_t i = dataLen/(maxLen*count);
	uint16_t j = dataLen%(maxLen*count);
	for (uint16_t step = 0; step < i; step++) {
		ExchangeStatus(Transmit_I2C1(DEVICE_ADDR_RADIO_ADAU1701<<1, (podAddress + maxLen * step),2, (uint8_t *)(data + maxLen*step*count), (maxLen*count), 1500, ExchangeMemMainSteam), MiniDSP_WRITE);
	}
	if (j>0){
		ExchangeStatus(Transmit_I2C1(DEVICE_ADDR_RADIO_ADAU1701<<1, (podAddress + maxLen * i),2, (uint8_t *)(data + maxLen*i*count), j, 1500, ExchangeMemMainSteam), MiniDSP_WRITE);
	}
}

void Transmit_DSP(uint16_t MemAddress, uint8_t* pData,  uint16_t Size, uint16_t Timeout){

	uint8_t Subaddress[3] = {0x00 ,MemAddress >> 8, MemAddress & 0xff};
    Switch_CS_DSP(OFF);
	ExchangeStatus(Transmit_SPI2(Subaddress, 3, 200, ExchangeMainSteam), DSP_WRITE);
	ExchangeStatus(Transmit_SPI2(pData, Size, Timeout, ExchangeMainSteam), DSP_WRITE);
	Switch_CS_DSP(ON);
}

void Transmit_DSP_SafeLoad(uint8_t *pData, uint16_t Size, uint16_t MemAddress) {

	uint16_t SafeLoadAddr = 24576;
	uint8_t datToSend[28] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
	for (int i = 0; i < Size; i++) {
		datToSend[i] = pData[i];
	}
	datToSend[23] = MemAddress;
	datToSend[22] = MemAddress >> 8;
	datToSend[27] = Size / 4;
	Transmit_DSP(SafeLoadAddr, datToSend,  28, 200);
}

void Receiver_DSP( uint16_t MemAddress,  uint8_t* pData, uint16_t Size, uint16_t Timeout){

	uint8_t Subaddress[3] = {0x01, MemAddress >> 8, MemAddress & 0xff};
	Switch_CS_DSP(OFF);
	ExchangeStatus(Transmit_SPI2(Subaddress, 3, Timeout, ExchangeMainSteam), DSP_WRITE);
	ExchangeStatus(Receiver_SPI2(NOT_USED, pData, Size, Timeout, ExchangeMainSteam), DSP_READ);
	Switch_CS_DSP(ON);
}

void SPI_mode_DSP(void){

	uint8_t SpiWrite[1] = {0x00};
	Switch_CS_DSP(OFF);
	ExchangeStatus(Transmit_SPI2(SpiWrite, 1, 500, ExchangeMainSteam), DSP_WRITE);
	Switch_CS_DSP(ON);
	Switch_CS_DSP(OFF);
	ExchangeStatus(Transmit_SPI2(SpiWrite, 1, 500, ExchangeMainSteam), DSP_WRITE);
	Switch_CS_DSP(ON);
	Switch_CS_DSP(OFF);
	ExchangeStatus(Transmit_SPI2(SpiWrite, 1, 500, ExchangeMainSteam), DSP_WRITE);
	Switch_CS_DSP(ON);
}

void I2C_Send_to_DSP(uint16_t MemAddress, uint8_t* pData,  uint16_t Size, uint16_t Timeout){
	ExchangeStatus(Transmit_I2C2(DEVICE_ADDR_DSP_ADAU1452, MemAddress, 2, pData, Size, Timeout, ExchangeMemMainSteam), DSP_WRITE);
}

void I2C_Reading_from_DSP( uint16_t MemAddress,  uint8_t* pData, uint16_t Size, uint16_t Timeout){
	ExchangeStatus(Receiver_I2C2(DEVICE_ADDR_DSP_ADAU1452, MemAddress, 2, pData, Size, Timeout, ExchangeMemMainSteam), DSP_READ);
}

WorkRes Struct_Flash_SPI_Write(uint32_t* CheckSum, uint32_t PageAddr, uint32_t* Struct, uint16_t lenght){

	Spi_Flash_Write_CheckSumm(CheckSum, PageAddr, Struct, lenght);
	Delay(30, FreeRTOS);
	return Struct_Flash_SPI_Read(CheckSum, PageAddr, Struct, lenght);
}

WorkRes Struct_Flash_SPI_Read(uint32_t* CheckSum, uint32_t PageAddr, uint32_t* str, uint16_t lenght){

	Spi_Flash_Read_CheckSumm(&Dixom.Service.CheckSum.RX, PageAddr, str, lenght);
	if(Dixom.Service.CheckSum.RX == *CheckSum){
		Dixom.Service.Status.Struct[Module_DSP_enum] = ReadyForWork;
		return WR_OK;
	}else{
		Dixom.Service.Status.Struct[Module_DSP_enum] = SafeMode;
		return WR_ERROR;
	}
}

WorkRes Struct_Flash_STM_Write(uint32_t* CheckSum, uint32_t StmAddr, uint32_t* Struct, uint16_t lenght){

	Flash_Unlock();
	FlashWriteCRC_Check( CheckSum, StmAddr, Struct, lenght);
	Flash_Lock();
	Delay(30, FreeRTOS);
	return Struct_Flash_STM_Read(CheckSum, StmAddr, Struct, lenght);
}

WorkRes Struct_Flash_STM_Read(uint32_t* CheckSum, uint32_t StmAddr, uint32_t* Struct, uint16_t lenght){

	FlashReadCRC_Check(&Dixom.Service.CheckSum.RX, StmAddr, Struct, lenght);
	if(Dixom.Service.CheckSum.RX == *CheckSum){
		Dixom.Service.Status.Struct[Module_DSP_enum] = ReadyForWork;
		return WR_OK;
	}else{
		Dixom.Service.Status.Struct[Module_DSP_enum] = SafeMode;
		return WR_ERROR;
	}
}




