#include <Structures.h>
#include "DAC_TAS3251.h"
#include "Exchange.h"
#include <Structures.h>

extern sDixom Dixom;


void Mute_DAC_TAS3251(uint8_t state){


	if(state == ON){
		Dixom.Module.AudioDAC.TAS3251.Register2.RQPD = 1;
	}else{
		Dixom.Module.AudioDAC.TAS3251.Register2.RQPD = 0;
	}
	ExchangeStatus(Transmit_I2C2(Port1_CH1_2_TAS3251<<1, NOT_USED, NOT_USED, (uint8_t *)&Dixom.Module.AudioDAC.TAS3251.Register2, 2, 200, ExchangeMainSteam), DAC_WRITE);

}

void Volume_DAC_TAS3251(uint8_t *pData){
	//ExchangeStatus(Transmit_I2C2(DEVICE_ADDR_DAC_TAS3251<<1, NOT_USED, NOT_USED, pData, 2, 200, ExchangeMainSteam), DAC_WRITE);
}

/**
 * ExampleUseThe followingis a samplescriptfor usingthe DSPhostmemoryto changethe fine volumeon the deviceon I2Cslaveaddress0x90to the defaultvalueof 0 dB:
 * w 90 00 00 #Go to page0
 * w 90 7f 8C #Changethe bookto 0x8C
 * w 90 00 21 #Go to page0x21
 * w 90 34 40 00 00 00 #FinevolumeLeft
 * w 90 38 40 00 00 00 #FinevolumeRights#Runthe swapflagfor the DSP to workon the new coefficients
 * w 90 00 00 #Go to page0
 * w 90 7f 8C #Changethe bookto 0x8C
 * w 90 00 05 #Go to page0x05
 * w 90 7C 00 00 00 01 #Swapflag
 *
 *
 * The followingis a samplescriptfor settingup the bothleft and rightchannelsfor (½L + ½R) or (L + R) / 2:
 * w 90 00 00 # Go to page0w 90 7f 8C #Changethe bookto 0x8C
 * w 90 00 21 #Go to page0x21
 * w 90 3C 00 40 26 E7 #Inputmixerleftin to leftout gain
 * w 90 40 00 40 26 E7 #Inputmixerrightin to leftout gain
 * w 90 44 00 40 26 E7 #Inputmixerleftin to rightout gain
 * w 90 48 00 40 26 E7 #Inputmixerrightin to rightout gain#Runthe swapflagfor the DSP to workon the new coefficients
 * w 90 00 00 #Go to page0
 * w 90 7f 8C #Changethe bookto 0x8C
 * w 90 00 05 #Go to page0x05
 * w 90 7C 00 00 00 01 #Swapflag
 */


void Init_DAC_TAS3251(void){
/*

	Transmit_I2C2(DEVICE_ADDR_AMPLIFIER_3xTAS3251<<1, NOT_USED, NOT_USED, (uint8_t*)Port1, 1, 200, ExchangeMainSteam);

	Dixom.Module.AudioDAC.TAS3251.Register2.ADDR       = 0x02;
	Dixom.Module.AudioDAC.TAS3251.Register2.DSPR       = 0;
	Dixom.Module.AudioDAC.TAS3251.Register2.RQPD       = 1;
	Dixom.Module.AudioDAC.TAS3251.Register2.RQST       = 0;
	Dixom.Module.AudioDAC.TAS3251.Register2.Reserved1  = 0;
	Dixom.Module.AudioDAC.TAS3251.Register2.Reserved2  = 0;

    uint8_t InitData[2];
    InitData[1] = 0;
	InitData[0] = TAS3251_CHANGE_PAGE_REG;
	Transmit_I2C2(Port1_TAS3251_CH1_2<<1, NOT_USED, NOT_USED, &InitData, 2, 200, ExchangeMainSteam);

	InitData[0] = TAS3251_CHANGE_BOOK_REG;
	Transmit_I2C2(Port1_TAS3251_CH1_2<<1, NOT_USED, NOT_USED, &InitData, 2, 200, ExchangeMainSteam);

	InitData[0] = TAS3251_OPERATION_MODES_REG;
	Transmit_I2C2(Port1_TAS3251_CH1_2<<1, NOT_USED, NOT_USED, &InitData, 2, 200, ExchangeMainSteam);
*/

/*
	Transmit_I2C2(DEVICE_ADDR_AMPLIFIER_3xTAS3251<<1, NOT_USED, NOT_USED, (uint8_t*)Port3, 1, 200, ExchangeMainSteam);

	InitData[0] = TAS3251_CHANGE_PAGE_REG;
	Transmit_I2C2(Port3_TAS3251_CH3_4<<1, NOT_USED, NOT_USED, &InitData, 2, 200, ExchangeMainSteam);

	InitData[0] = TAS3251_CHANGE_BOOK_REG;
	Transmit_I2C2(Port3_TAS3251_CH3_4<<1, NOT_USED, NOT_USED, &InitData, 2, 200, ExchangeMainSteam);

	InitData[0] = TAS3251_OPERATION_MODES_REG;
	Transmit_I2C2(Port3_TAS3251_CH3_4<<1, NOT_USED, NOT_USED, &InitData, 2, 200, ExchangeMainSteam);

	InitData[0] = TAS3251_CHANGE_PAGE_REG;
	Transmit_I2C2(Port3_TAS3251_CH5_6<<1, NOT_USED, NOT_USED, &InitData, 2, 200, ExchangeMainSteam);

	InitData[0] = TAS3251_CHANGE_BOOK_REG;
	Transmit_I2C2(Port3_TAS3251_CH5_6<<1, NOT_USED, NOT_USED, &InitData, 2, 200, ExchangeMainSteam);

	InitData[0] = TAS3251_OPERATION_MODES_REG;
	Transmit_I2C2(Port3_TAS3251_CH5_6<<1, NOT_USED, NOT_USED, &InitData, 2, 200, ExchangeMainSteam);
*/
//	Mute_DAC_TAS3251(1);





}
