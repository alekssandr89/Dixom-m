/*
 * DSP_Exchange.h
 *
 *  Created on: 16 но€б. 2021 г.
 *      Author: 79825
 */

#ifndef MODULE_DSP_DSP_EXCHANGE_H_
#define MODULE_DSP_DSP_EXCHANGE_H_

#include "DSP.h"

typedef const unsigned char       ADI_REG_TYPE;


void        Transmit_Sigma( uint16_t devAddress, uint32_t podAddress, uint32_t dataLen, ADI_REG_TYPE *data);
void        Transmit_Sigma1701( uint16_t devAddress, uint16_t podAddress, uint16_t dataLen, ADI_REG_TYPE *data);
void        Transmit_DSP(uint16_t MemAddress, uint8_t* pData,  uint16_t Size, uint16_t Timeout);
void        Transmit_DSP_SafeLoad( uint8_t *pData,  uint16_t Size, uint16_t MemAddress);
void        Receiver_DSP( uint16_t MemAddress,  uint8_t* pData, uint16_t Size, uint16_t Timeout);
void        SPI_mode_DSP(void);

void        I2C_Send_to_DSP(uint16_t MemAddress, uint8_t* pData,  uint16_t Size, uint16_t Timeout);
void        I2C_Reading_from_DSP( uint16_t MemAddress,  uint8_t* pData, uint16_t Size, uint16_t Timeout);


WorkRes     Struct_Flash_SPI_Write(uint32_t* CheckSum, uint32_t PageAddr, uint32_t* Struct, uint16_t lenght);
WorkRes     Struct_Flash_SPI_Read(uint32_t* CheckSum, uint32_t PageAddr, uint32_t* Struct, uint16_t lenght);


WorkRes     Struct_Flash_STM_Read(uint32_t* CheckSum, uint32_t StmAddr, uint32_t* str, uint16_t lenght);
WorkRes     Struct_Flash_STM_Write(uint32_t* CheckSum, uint32_t StmAddr, uint32_t* Struct, uint16_t lenght);




#endif /* MODULE_DSP_DSP_EXCHANGE_H_ */
