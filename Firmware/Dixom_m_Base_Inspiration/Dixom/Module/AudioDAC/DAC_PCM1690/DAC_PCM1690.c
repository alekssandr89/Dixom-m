/*
 * DAC_PCM1690.c
 *
 *  Created on: 17 мар. 2022 г.
 *      Author: 79825
 */
#include "DAC_PCM1690.h"
#include "Exchange.h"
#include <Structures.h>

extern sDixom Dixom;
#define dPCM1690 Dixom.Module.AudioDAC.PCM1690

void Init_DAC_PC1690(){

	dPCM1690.Reg64.ADDR = 64;
	dPCM1690.Reg64.AMC_ASINCHRONUS_DETECT = 0;
	dPCM1690.Reg64.AMC_SCKI = 0;
	dPCM1690.Reg64.AMC_ZERO_DAC_DISABLE_CMD = 0;
	dPCM1690.Reg64.AMC_ZERO_DETECT = 0;
	dPCM1690.Reg64.SRST = 1;
	dPCM1690.Reg64.MRST = 1;

	dPCM1690.Reg65.ADDR = 65;
	dPCM1690.Reg65.FMTDA = 0;
	dPCM1690.Reg65.Reserved = 0;
	dPCM1690.Reg65.PSMDA = 0;

	dPCM1690.Reg66.ADDR = 66;
	dPCM1690.Reg66.FLT_CH1_2 = 0;
	dPCM1690.Reg66.FLT_CH3_4 = 0;
	dPCM1690.Reg66.FLT_CH5_6 = 0;
	dPCM1690.Reg66.FLT_CH7_8 = 0;
	dPCM1690.Reg66.OPEDA_CH1_2 = 0;
	dPCM1690.Reg66.OPEDA_CH3_4 = 0;
	dPCM1690.Reg66.OPEDA_CH5_6 = 0;
	dPCM1690.Reg66.OPEDA_CH7_8 = 0;

	dPCM1690.Reg67.ADDR = 67;
	dPCM1690.Reg67.REVDA1 = 0;
	dPCM1690.Reg67.REVDA2 = 1;
	dPCM1690.Reg67.REVDA3 = 0;
	dPCM1690.Reg67.REVDA4 = 1;
	dPCM1690.Reg67.REVDA5 = 0;
	dPCM1690.Reg67.REVDA6 = 1;
	dPCM1690.Reg67.REVDA7 = 0;
	dPCM1690.Reg67.REVDA8 = 1;

	ExchangeStatus(Transmit_I2C2(DEVICE_ADDR_DAC_PCM1690<<1, NOT_USED, NOT_USED, (uint8_t *)&dPCM1690.Reg67, 2, 100, ExchangeMainSteam), DAC_WRITE);

	dPCM1690.Reg68.ADDR = 68;
	dPCM1690.Reg68.MUTDA = 0;


	dPCM1690.Reg69.ADDR = 69;
	dPCM1690.Reg69.ZERO1 = 0;
	dPCM1690.Reg69.ZERO2 = 0;
	dPCM1690.Reg69.ZERO3 = 0;
	dPCM1690.Reg69.ZERO4 = 0;
	dPCM1690.Reg69.ZERO5 = 0;
	dPCM1690.Reg69.ZERO6 = 0;
	dPCM1690.Reg69.ZERO7 = 0;
	dPCM1690.Reg69.ZERO8 = 0;

	dPCM1690.Reg70.ADDR = 70;
	dPCM1690.Reg70.ZREV = 0;
	dPCM1690.Reg70.AZRO = 0;
	dPCM1690.Reg70.Reserved0 = 0;
	dPCM1690.Reg70.DEMP = 0;
	dPCM1690.Reg70.Reserved1 = 0;
	dPCM1690.Reg70.DAMS = 0;

}


void Mute_DAC_PC1690(uint8_t state){

	if(state == OFF){
		dPCM1690.Reg68.MUTDA = 0;

	}else{
		dPCM1690.Reg68.MUTDA = 255;
	}
	ExchangeStatus(Transmit_I2C2(DEVICE_ADDR_DAC_PCM1690<<1, NOT_USED, NOT_USED, (uint8_t *)&dPCM1690.Reg68, 2, 100, ExchangeMainSteam), DAC_WRITE);
}

void Volume_DAC_PCM1690(uint8_t *pData){

	uint8_t PCM1690_CH1_8[9] = { 0x48, pData[0], pData[0], pData[0], pData[0], pData[0], pData[0], pData[0], pData[0] };

	ExchangeStatus(Transmit_I2C2(DEVICE_ADDR_DAC_PCM1690<<1, NOT_USED, NOT_USED, PCM1690_CH1_8, 9, 100, ExchangeMainSteam), DAC_WRITE);
}



