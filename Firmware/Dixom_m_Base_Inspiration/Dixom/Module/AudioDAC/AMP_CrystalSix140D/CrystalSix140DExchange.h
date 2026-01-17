/*
 * AmplifierExchange.h
 *
 *  Created on: 29 мая 2023 г.
 *      Author: alekk
 */

#ifndef MODULE_AUDIODAC_TAS_3251X3_AMPLIFIEREXCHANGE_H_
#define MODULE_AUDIODAC_TAS_3251X3_AMPLIFIEREXCHANGE_H_


int AmplifierTransmit(uint8_t Port, uint16_t DevAddress, uint16_t MemAddress, uint8_t *pData, uint16_t Size, uint8_t ExchangeType);
int AmplifierReciever(uint8_t Port, uint16_t DevAddress, uint16_t MemAddress, uint8_t *pData, uint16_t Size, uint8_t ExchangeType);

#endif /* MODULE_AUDIODAC_TAS_3251X3_AMPLIFIEREXCHANGE_H_ */
