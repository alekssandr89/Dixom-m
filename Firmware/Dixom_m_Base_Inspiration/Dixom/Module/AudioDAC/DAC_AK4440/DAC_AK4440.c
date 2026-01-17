#include "DAC_AK4440.h"
#include "Exchange.h"
#include <Structures.h>

extern sDixom Dixom;

void Init_DAC_AK4440(void){

	Dixom.Module.AudioDAC.AK4440.Control2.ADDR         = 0x01;    // Register address
	Dixom.Module.AudioDAC.AK4440.Control2.SMUTE        = 0;       // Soft Mute Enable 0: Normal operation 1: DAC outputs soft-muted
	Dixom.Module.AudioDAC.AK4440.Control2.DEM0         = 1;       // De-emphasis Response  Default: “01”, OFF
	Dixom.Module.AudioDAC.AK4440.Control2.DEM1         = 0;       // De-emphasis Response  Default: “01”, OFF
	Dixom.Module.AudioDAC.AK4440.Control2.DFS0         = 0;       // Sampling speed control 00: Normal speed 01: Double speed 10: Quad speed
	Dixom.Module.AudioDAC.AK4440.Control2.DFS1         = 0;       // Sampling speed control 00: Normal speed 01: Double speed 10: Quad speed
	Dixom.Module.AudioDAC.AK4440.Control2.SLOW         = 0;       // Slow Roll-off Filter Enable 0: Sharp Roll-off Filter 1: Slow Roll-off Filter
	Dixom.Module.AudioDAC.AK4440.Control2.NotUsed      = 0;       // Not Used
	Dixom.Module.AudioDAC.AK4440.Control2.RRST         = 0;       // All registers are initialized 0: Normal Operation 1: Reset. All registers are initialized.

	Dixom.Module.AudioDAC.AK4440.Control1.ADDR              = 0x00;   // Register address
	Dixom.Module.AudioDAC.AK4440.Control1.RSTN              = 1;      // Internal timing reset 0: Reset. Any registers are not initialized. 1: Normal operation
	Dixom.Module.AudioDAC.AK4440.Control1.NotUsed           = 0;      // Not Used
	Dixom.Module.AudioDAC.AK4440.Control1.DIF0              = 1;      // Audio data interface modes (Table 8) Default: “010”
	Dixom.Module.AudioDAC.AK4440.Control1.DIF1              = 1;      // Audio data interface modes (Table 8) Default: “010”
	Dixom.Module.AudioDAC.AK4440.Control1.DIF2              = 0;      // Audio data interface modes (Table 8) Default: “010”
	Dixom.Module.AudioDAC.AK4440.Control1.TDM0              = 0;      // TDM Mode Select
	Dixom.Module.AudioDAC.AK4440.Control1.TDM1              = 0;      // TDM Mode Select
	Dixom.Module.AudioDAC.AK4440.Control1.ACKS              = 0;      // Master Clock Frequency Auto Setting Mode Enable 0: Disable, Manual Setting Mode 1: Enable, Auto Setting Mode

	Dixom.Module.AudioDAC.AK4440.PwrDwnCon.ADDR     = 0x02;   // Register address
	Dixom.Module.AudioDAC.AK4440.PwrDwnCon.PW1      = 1;      // Power down control of DAC1 (0: Power-down, 1: Power-up)
	Dixom.Module.AudioDAC.AK4440.PwrDwnCon.PW2      = 1;      // Power down control of DAC2 (0: Power-down, 1: Power-up)
	Dixom.Module.AudioDAC.AK4440.PwrDwnCon.PW3      = 1;      // Power down control of DAC3 (0: Power-down, 1: Power-up)
	Dixom.Module.AudioDAC.AK4440.PwrDwnCon.PW4      = 1;      // Power down control of DAC4 (0: Power-down, 1: Power-up)
	Dixom.Module.AudioDAC.AK4440.PwrDwnCon.NotUsed  = 0;

	Dixom.Module.AudioDAC.AK4440.DEMControl.ADDR           = 0x03;   // Register address
	Dixom.Module.AudioDAC.AK4440.DEMControl.DEMD           = 0;      // De-emphasis Enable bit of DAC1/2/3/4 0: Disable  1: Enable
	Dixom.Module.AudioDAC.AK4440.DEMControl.DEMC           = 0;      // De-emphasis Enable bit of DAC1/2/3/4 0: Disable  1: Enable
	Dixom.Module.AudioDAC.AK4440.DEMControl.DEMB           = 0;      // De-emphasis Enable bit of DAC1/2/3/4 0: Disable  1: Enable
	Dixom.Module.AudioDAC.AK4440.DEMControl.DEMA           = 0;      // De-emphasis Enable bit of DAC1/2/3/4 0: Disable  1: Enable
	Dixom.Module.AudioDAC.AK4440.DEMControl.NotUsed        = 0;      // De-emphasis Enable bit of DAC1/2/3/4 0: Disable  1: Enable

	ExchangeStatus(Transmit_I2C2(DEVICE_ADDR_DAC_AK4440<<1, NOT_USED, NOT_USED, (uint8_t *)&Dixom.Module.AudioDAC.AK4440.Control1, 2, 200, ExchangeMainSteam), DAC_WRITE);
	ExchangeStatus(Transmit_I2C2(DEVICE_ADDR_DAC_AK4440<<1, NOT_USED, NOT_USED, (uint8_t *)&Dixom.Module.AudioDAC.AK4440.PwrDwnCon, 2, 200, ExchangeMainSteam), DAC_WRITE);
	ExchangeStatus(Transmit_I2C2(DEVICE_ADDR_DAC_AK4440<<1, NOT_USED, NOT_USED, (uint8_t *)&Dixom.Module.AudioDAC.AK4440.DEMControl, 2, 200, ExchangeMainSteam), DAC_WRITE);
/*
    uint8_t Control1            [2] = {0x00,0x0D}; //1000 1101 ///24-bit I2S    MANUAL MOD
    uint8_t PowerDownControl    [2] = {0x02,0x0F}; //0000 1111
    uint8_t DEMControl          [2] = {0x03,0x0F}; //0000 0000

    I2C_2_Write((uint16_t) DEVICE_ADDR_DAC_AK4440<<1, Control1,             2, 100);
    I2C_2_Write((uint16_t) DEVICE_ADDR_DAC_AK4440<<1, PowerDownControl,     2, 100);
    I2C_2_Write((uint16_t) DEVICE_ADDR_DAC_AK4440<<1, DEMControl,           2, 100);
*/
}



void Mute_DAC_AK4440(uint8_t state){

	if(state == ON){
		Dixom.Module.AudioDAC.AK4440.Control2.SMUTE = 1;
	}else{
		Dixom.Module.AudioDAC.AK4440.Control2.SMUTE = 0;
	}
	ExchangeStatus(Transmit_I2C2(DEVICE_ADDR_DAC_AK4440<<1, NOT_USED, NOT_USED, (uint8_t *)&Dixom.Module.AudioDAC.AK4440.Control2, 2, 200, ExchangeMainSteam), DAC_WRITE);
}
















