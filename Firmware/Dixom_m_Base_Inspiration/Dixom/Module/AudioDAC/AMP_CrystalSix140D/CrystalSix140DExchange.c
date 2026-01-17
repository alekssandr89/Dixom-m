/*
 * AmplifierExchange.c
 *
 *  Created on: 29 мая 2023 г.
 *      Author: alekk
 */
#include <Structures.h>
#include "DIX_i2c.h"
#include "Constants.h"
#include <CrystalSix140D.h>
#include <CrystalSix140DExchange.h>

extern sDixom Dixom;
#define Amplifier  Dixom.Module.AudioDAC.TAS3251x3


int AmplifierTransmit(uint8_t Port, uint16_t DevAddress, uint16_t MemAddress, uint8_t *pData, uint16_t Size, uint8_t ExchangeType){
	if(Size == 0){
		return WR_ERROR;
	}

	if(Amplifier.CurrentPort != Port){
		uint8_t Data[1];
		Data[0] = Port;
		Amplifier.CurrentPort = Port;
		ExchangeStatus(Transmit_I2C2(DEVICE_ADDR_AMPLIFIER_3xTAS3251<<1, NOT_USED, NOT_USED, &Data[0], 1, 200, ExchangeMainSteam), DAC_WRITE);
	}

	if(Port == I2C_PORT_5){
		int hh = 1;
	}

	return ExchangeStatus(Transmit_I2C2(DevAddress<<1, MemAddress, 1, pData, Size, 200, ExchangeType), DAC_WRITE);
}

int AmplifierReciever(uint8_t Port, uint16_t DevAddress, uint16_t MemAddress, uint8_t *pData, uint16_t Size, uint8_t ExchangeType){
	if(Size == 0){
		return WR_ERROR;
	}
	uint8_t Data[Size];
	if(Amplifier.CurrentPort != Port){
		Data[0] = Port;
		Amplifier.CurrentPort = Port;
		ExchangeStatus(Transmit_I2C2(DEVICE_ADDR_AMPLIFIER_3xTAS3251<<1, NOT_USED, NOT_USED, &Data[0], 1, 200, ExchangeMainSteam), DAC_WRITE);
	};

	return ExchangeStatus(Receiver_I2C2(DevAddress<<1, MemAddress, 1, pData, Size, 200, ExchangeType), DAC_READ);

}
