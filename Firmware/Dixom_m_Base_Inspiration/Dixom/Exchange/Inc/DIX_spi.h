/*
 * DIX_spi.h
 *
 *  Created on: 12 мар. 2022 г.
 *      Author: 79825
 */

#ifndef EXCHANGE_INC_DIX_SPI_H_
#define EXCHANGE_INC_DIX_SPI_H_

WorkRes    Transmit_SPI2(uint8_t *pTxData, uint16_t Size, uint32_t Timeout, uint8_t ExchangeType);
WorkRes    Receiver_SPI2(uint8_t *pTxData, uint8_t *pRxData, uint16_t Size, uint32_t Timeout, uint8_t ExchangeType);
void       SetStateSPI(uint8_t ControlByte, uint8_t *Recive_USB);
void       GetStateSPI();

#endif /* EXCHANGE_INC_DIX_SPI_H_ */
