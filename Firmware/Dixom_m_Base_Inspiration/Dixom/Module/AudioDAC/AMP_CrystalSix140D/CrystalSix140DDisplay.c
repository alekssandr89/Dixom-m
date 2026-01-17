/*
 * AmpDisplay.c
 *
 *  Created on: 29 мая 2023 г.
 *      Author: alekk
 */
#include <Structures.h>
#include "Description.h"
#include "CrystalSix140DExchange.h"
#include <CrystalSix140DDisplay.h>

extern sDixom Dixom;
#define Amplifier  Dixom.Module.AudioDAC.TAS3251x3

//void ssd1306_I2C_Write(uint8_t address, uint8_t reg, uint8_t data)
//организован обмен in file ssd1306.c

void InitCrystalSixDisplay(){


	/*Amplifier.TempControl.TempHysteresis = Dixom.Module.DSP.Settings[pTempHysteresis].Data;
	Amplifier.TempControl.TempOS = Dixom.Module.DSP.Settings[psTempOS].Data;
	uint8_t inData[2];
	inData[0] = 0x04;
	inData[1] = 0x00;
	AmplifierTransmit(I2C_PORT_3, DEVICE_ADDR_TEMOMETER_LM75A, LM75_REG_CONF, &inData[0], 1, ExchangeMemMainSteam);
	inData[0] = Amplifier.TempControl.TempHysteresis;
	AmplifierTransmit(I2C_PORT_3, DEVICE_ADDR_TEMOMETER_LM75A, LM75_REG_T_HYS, &inData[0], 2, ExchangeMemMainSteam);
	inData[0] = Amplifier.TempControl.TempOS;
	AmplifierTransmit(I2C_PORT_3, DEVICE_ADDR_TEMOMETER_LM75A, LM75_REG_T_OS, &inData[0], 2, ExchangeMemMainSteam);*/
}

