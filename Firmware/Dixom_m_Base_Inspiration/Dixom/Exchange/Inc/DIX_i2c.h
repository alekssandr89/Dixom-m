/*
 * DIX_i2c.h
 *
 *  Created on: 12 мар. 2022 г.
 *      Author: 79825
 */

#ifndef EXCHANGE_INC_DIX_I2C_H_
#define EXCHANGE_INC_DIX_I2C_H_

#include "Constants.h"


typedef struct
{

	uint8_t DMA_Transmit_Cplt;  // USB connection status to host

}sStateI2C ;

void       Scanning_I2C1(void);
void       Scanning_I2C2(void);

WorkRes    DevState_I2C1(uint16_t DevAddress, uint16_t Trials, uint16_t Timeout);
WorkRes    DevState_I2C2(uint16_t DevAddress, uint16_t Trials, uint16_t Timeout);
WorkRes    Transmit_I2C1(uint16_t DevAddress, uint16_t MemAddress, uint16_t MemAddSize, uint8_t *pData, uint16_t Size, uint32_t Timeout, uint8_t ExchangeType);
WorkRes    Transmit_I2C2(uint16_t DevAddress, uint16_t MemAddress, uint16_t MemAddSize, uint8_t *pData, uint16_t Size, uint32_t Timeout, uint8_t ExchangeType);
WorkRes    Receiver_I2C1(uint16_t DevAddress, uint16_t MemAddress, uint16_t MemAddSize, uint8_t *pData, uint16_t Size, uint32_t Timeout, uint8_t ExchangeType);
WorkRes    Receiver_I2C2(uint16_t DevAddress, uint16_t MemAddress, uint16_t MemAddSize, uint8_t *pData, uint16_t Size, uint32_t Timeout, uint8_t ExchangeType);
WorkRes    Receiver_I2C2_NoSemaphore(uint16_t DevAddress, uint16_t MemAddress, uint16_t MemAddSize, uint8_t *pData, uint16_t Size, uint32_t Timeout, uint8_t ExchangeType);


#endif /* EXCHANGE_INC_DIX_I2C_H_ */
