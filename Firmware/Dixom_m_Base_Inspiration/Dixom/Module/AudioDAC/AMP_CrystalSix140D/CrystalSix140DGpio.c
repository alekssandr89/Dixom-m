/*
 * GpioControl.c
 *
 *  Created on: 29 мая 2023 г.
 *      Author: alekk
 */
#include <Structures.h>
#include "Description.h"
#include "CrystalSix140DExchange.h"
#include <CrystalSix140DThermometer.h>

extern sDixom Dixom;
#define Amplifier  Dixom.Module.AudioDAC.TAS3251x3


typedef union {
	uint8_t  Bytes;
	tGpioIN  StructIn;
	tGpioOut StructOut;
} _byteStruct;

void InitCrystalSixGpio(){

	_byteStruct converter;

	AmplifierReciever(I2C_PORT_3, Port3_GPIO_IN_PCF8574, NOT_USED, &converter.Bytes, 1, ExchangeMainSteam);
	Amplifier.GpioIN = converter.StructIn;

	Amplifier.GpioOut.Amp1_Mute = ON;
	Amplifier.GpioOut.Amp1_Reset = ON;
	Amplifier.GpioOut.Amp2_Mute = ON;
	Amplifier.GpioOut.Amp2_Reset = ON;
	Amplifier.GpioOut.Amp3_Mute = ON;
	Amplifier.GpioOut.Amp3_Reset = ON;

	converter.StructOut = Amplifier.GpioOut;
	AmplifierTransmit(I2C_PORT_3, Port3_GPIO_OUT_PCF8574, NOT_USED, &converter.Bytes, 1, ExchangeMemMainSteam);


}

