/*
 * Eeprom.h
 *
 *  Created on: 14 февр. 2022 г.
 *      Author: 79825
 */

#ifndef EXCHANGE_INC_MEMORY_EEPROM_H_
#define EXCHANGE_INC_MEMORY_EEPROM_H_

#include "Constants.h"
#include "stdint.h"

void       EEPROM_Write_CheckSumm(uint32_t* flash_checkSum, uint32_t MemAddress, uint8_t* pData, uint16_t Size, uint16_t Timeout);
void       EEPROM_Read_CheckSumm(uint32_t* read_checkSum, uint32_t MemAddress, uint8_t* pData, uint16_t Size, uint16_t Timeout);
void       Transmit_EEPROM(uint16_t MemAddress, uint8_t *pData,  uint16_t Size, uint16_t Timeout);
WorkRes    Receiver_EEPROM( uint16_t MemAddress,  uint8_t* pData, uint16_t Size, uint16_t Timeout);
WorkRes    Receiver_EEPROM_DMA( uint16_t MemAddress,  uint8_t* pData, uint16_t Size, uint16_t Timeout);

#endif /* EXCHANGE_INC_MEMORY_EEPROM_H_ */
