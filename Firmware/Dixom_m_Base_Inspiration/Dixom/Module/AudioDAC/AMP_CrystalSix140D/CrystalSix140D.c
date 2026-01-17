/*
 * Tas3251x3_Amplifier.c
 *
 *  Created on: 26 ����. 2022 �.
 *      Author: 79825
 */
#include <CrystalSix140DGpio.h>
#include <Structures.h>
#include <CrystalSix140D.h>
#include <CrystalSix140DThermometer.h>
#include "CrystalSix140DExchange.h"
#include "CrystalSix140DDisplay.h"

extern sDixom Dixom;
#define dTAS3251x3  Dixom.Module.AudioDAC.TAS3251x3

#define Amplifier  Dixom.Module.AudioDAC.TAS3251x3

void Mute_AMPLIFIER_3xTAS3251(uint8_t state){

	if(state == ON){
		Dixom.Module.AudioDAC.TAS3251.Register2.RQPD = 1;
	}else{
		Dixom.Module.AudioDAC.TAS3251.Register2.RQPD = 0;
	}

	AmplifierTransmit(I2C_PORT_1, Port1_CH1_2_TAS3251, 0, (uint8_t *)&Dixom.Module.AudioDAC.TAS3251.Register2, 2, ExchangeMainSteam);
	AmplifierTransmit(I2C_PORT_3, Port3_CH3_4_TAS3251, 0, (uint8_t *)&Dixom.Module.AudioDAC.TAS3251.Register2, 2, ExchangeMainSteam);
	AmplifierTransmit(I2C_PORT_3, Port3_CH5_6_TAS3251, 0, (uint8_t *)&Dixom.Module.AudioDAC.TAS3251.Register2, 2, ExchangeMainSteam);
}

void Init_AMPLIFIER_3xTAS3251(){


	InitCrystalSixGpio();
	InitCrystalSixThermometer();
	InitCrystalSixDisplay();


	Dixom.Module.AudioDAC.TAS3251.Register2.ADDR       = 0x02;
	Dixom.Module.AudioDAC.TAS3251.Register2.DSPR       = 0;
	Dixom.Module.AudioDAC.TAS3251.Register2.RQPD       = 0;
	Dixom.Module.AudioDAC.TAS3251.Register2.RQST       = 0;
	Dixom.Module.AudioDAC.TAS3251.Register2.Reserved1  = 0;
	Dixom.Module.AudioDAC.TAS3251.Register2.Reserved2  = 0;


	uint8_t InitData[2];
    InitData[1] = 0;

    InitData[0] = TAS3251_CHANGE_PAGE_REG;
    AmplifierTransmit(I2C_PORT_1, Port1_CH1_2_TAS3251, 0, (uint8_t *)&InitData[0], 2, ExchangeMainSteam);

    InitData[0] = TAS3251_CHANGE_BOOK_REG;
    AmplifierTransmit(I2C_PORT_1, Port1_CH1_2_TAS3251, 0, (uint8_t *)&InitData[0], 2, ExchangeMainSteam);

    InitData[0] = TAS3251_OPERATION_MODES_REG;
    AmplifierTransmit(I2C_PORT_1, Port1_CH1_2_TAS3251, 0, (uint8_t *)&InitData[0], 2, ExchangeMainSteam);


    InitData[0] = TAS3251_CHANGE_PAGE_REG;
    AmplifierTransmit(I2C_PORT_3, Port3_CH3_4_TAS3251, 0, (uint8_t *)&InitData[0], 2, ExchangeMainSteam);
    AmplifierTransmit(I2C_PORT_3, Port3_CH5_6_TAS3251, 0, (uint8_t *)&InitData[0], 2, ExchangeMainSteam);

    InitData[0] = TAS3251_CHANGE_BOOK_REG;
    AmplifierTransmit(I2C_PORT_3, Port3_CH3_4_TAS3251, 0, (uint8_t *)&InitData[0], 2, ExchangeMainSteam);
    AmplifierTransmit(I2C_PORT_3, Port3_CH5_6_TAS3251, 0, (uint8_t *)&InitData[0], 2, ExchangeMainSteam);

    InitData[0] = TAS3251_OPERATION_MODES_REG;
    AmplifierTransmit(I2C_PORT_3, Port3_CH3_4_TAS3251, 0, (uint8_t *)&InitData[0], 2, ExchangeMainSteam);
    AmplifierTransmit(I2C_PORT_3, Port3_CH5_6_TAS3251, 0, (uint8_t *)&InitData[0], 2, ExchangeMainSteam);


    Mute_AMPLIFIER_3xTAS3251(0);
}


