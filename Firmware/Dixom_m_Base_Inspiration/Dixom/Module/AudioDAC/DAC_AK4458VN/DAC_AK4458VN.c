#include "DAC_AK4458VN.h"
#include "Exchange.h"
#include <Structures.h>

extern sDixom Dixom;
#define dAK4458VN Dixom.Module.AudioDAC.AK4458VN

void Init_DAC_AK4458VN(void){

	dAK4458VN.Control1.ADDR      = 0x00;  // Register address
	dAK4458VN.Control1.RSTN      = 1;     // 0: Reset (default)  1: Normal Operation
	dAK4458VN.Control1.DIF0      = 1;     // Audio Data Interface Modes
	dAK4458VN.Control1.DIF1      = 1;     // Audio Data Interface Modes
	dAK4458VN.Control1.DIF2      = 1;     // Audio Data Interface Modes
	dAK4458VN.Control1.NotUsed   = 0;     // Not Used
	dAK4458VN.Control1.ACKS      = 0;     // Master Clock Frequency Auto Setting Mode Enable (PCM only) 0: Disable : Manual Setting Mode (default)  1: Enable : Auto Setting Mode
	ExchangeStatus(Transmit_I2C2(DEVICE_ADDR_DAC_AK4458VN<<1, NOT_USED, NOT_USED, (uint8_t *)&dAK4458VN.Control1, 2, 200, ExchangeMainSteam), DAC_WRITE);

	dAK4458VN.Control2.ADDR      = 0x01;
	dAK4458VN.Control2.SMUTE     = 0;      //Soft Mute Enable 0: Normal Operation (default)  1: DAC outputs soft-muted.
	dAK4458VN.Control2.DEM10     = 1;      //DAC1 De-emphasis Response  Default value is “01” (OFF).
	dAK4458VN.Control2.DEM11     = 0;      //DAC1 De-emphasis Response  Default value is “01” (OFF).
	dAK4458VN.Control2.DFS0      = 0;      //Sampling Speed Control Default value is “00” (Normal Speed).
	dAK4458VN.Control2.DFS1      = 0;      //Sampling Speed Control Default value is “00” (Normal Speed).
	dAK4458VN.Control2.SD        = 1;      //***Short delay Filter Enable  0: Sharp roll-off filter  1: Short delay filter (default)
	dAK4458VN.Control2.NotUsed   = 0;      //Not USED
	ExchangeStatus(Transmit_I2C2(DEVICE_ADDR_DAC_AK4458VN<<1, NOT_USED, NOT_USED, (uint8_t *)&dAK4458VN.Control2, 2, 200, ExchangeMainSteam), DAC_WRITE);

	dAK4458VN.Control3.ADDR      = 0x02;   // Register address
	dAK4458VN.Control3.SLOW      = 0;      // ***Slow Roll-off Filter Enable.  0: Sharp roll-off filter (default)  1: Slow roll-off filter
	dAK4458VN.Control3.SELLR1    = 0;      // The data selection of DAC1 L channel and R channel, when MONO mode  Default value is “0”.
	dAK4458VN.Control3.DZFB      = 0;      // Inverting Enable of DZF 0: DZF pin goes “H” at Zero Detection (default)  1: DZF pin goes “L” at Zero Detection
	dAK4458VN.Control3.MONO1     = 0;      // DAC1 enters monaural output mode when MONO bit = “1”.  0: Stereo mode (default) 1: MONO mode
	dAK4458VN.Control3.DCKB      = 0;      // Polarity of DCLK (DSD Only) DSD data is output from DCLK falling edge. (default)  DSD data is output from DCLK rising edge.
	dAK4458VN.Control3.DCKS      = 0;      // Master Clock Frequency Select at DSD mode (DSD only) 0: 512fs (default) 1: 768fs
	dAK4458VN.Control3.NotUsed   = 0;      // Not Used
	dAK4458VN.Control3.DP        = 0;      // DSD/PCM Mode Select 0: PCM Mode (default) 1: DSD Mode
	ExchangeStatus(Transmit_I2C2(DEVICE_ADDR_DAC_AK4458VN<<1, NOT_USED, NOT_USED, (uint8_t *)&dAK4458VN.Control3, 2, 200, ExchangeMainSteam), DAC_WRITE);

	dAK4458VN.Control4.ADDR      = 0x05;   // Register address
	dAK4458VN.Control4.SSLOW     = 0;      // Digital Filter Bypass Mode Enable 0: Enable digital filter selected by SD and SLOW bits (default) 1: Super Slow Roll-off Mode
	dAK4458VN.Control4.DFS2      = 0;      // Sampling Speed Control Default value is “0” (Normal Speed). See also register address 01H for DFS1-0 A click noise occurs when switching DFS2-0 bits setting.
	dAK4458VN.Control4.NotUsed   = 0;      // Not Used
	dAK4458VN.Control4.SELLR2    = 0;      // Data Selection of DAC2 L channel and R channel, when MONO mode Default value is “0”.
	dAK4458VN.Control4.INVR2     = 1;      // AOUTL1 Output Phase Inverting Bit 0: Normal (default) 1: Inverted
	dAK4458VN.Control4.INVL2     = 0;      // AOUTR1 Output Phase Inverting Bit 0: Normal (default) 1: Inverted
	dAK4458VN.Control4.INVR1     = 1;      // AOUTL2 Output Phase Inverting Bit 0: Normal (default) 1: Inverted
	dAK4458VN.Control4.INVL1     = 0;      // AOUTR2 Output Phase Inverting Bit 0: Normal (default) 1: Inverted
	ExchangeStatus(Transmit_I2C2(DEVICE_ADDR_DAC_AK4458VN<<1, NOT_USED, NOT_USED, (uint8_t *)&dAK4458VN.Control4, 2, 200, ExchangeMainSteam), DAC_WRITE);

	dAK4458VN.Control5.ADDR      = 0x07;   // Register address
	dAK4458VN.Control5.SYNCE     = 1;      // SYNC Mode Enable 0: SYNC Mode Disable 1: SYNC Mode Enable (default)
	dAK4458VN.Control5.One       = 1;      //
	dAK4458VN.Control5.NotUsed   = 0;      // Not Used
	dAK4458VN.Control5.R4        = 0;      // Zero Detect Flag Enable Bit for the DZF pin 0: Disable (default) 1: Enable
	dAK4458VN.Control5.L4        = 0;      // Zero Detect Flag Enable Bit for the DZF pin 0: Disable (default) 1: Enable
	dAK4458VN.Control5.R3        = 0;      // Zero Detect Flag Enable Bit for the DZF pin 0: Disable (default) 1: Enable
	dAK4458VN.Control5.L3        = 0;      // Zero Detect Flag Enable Bit for the DZF pin 0: Disable (default) 1: Enable
	ExchangeStatus(Transmit_I2C2(DEVICE_ADDR_DAC_AK4458VN<<1, NOT_USED, NOT_USED, (uint8_t *)&dAK4458VN.Control5, 2, 200, ExchangeMainSteam), DAC_WRITE);

	dAK4458VN.SoundControl.ADDR  = 0x08;   // Register address
	dAK4458VN.SoundControl.SC0   = 0;      // Sound Control l (Table 26) Default value is “00”.
	dAK4458VN.SoundControl.SC1   = 0;      // Sound Control l (Table 26) Default value is “00”.
	dAK4458VN.SoundControl.NotUsed = 0;    // Not Used
	dAK4458VN.SoundControl.R2    = 0;      // Zero Detect Flag Enable Bit for the DZF pin 0: Disable (default)  1: Enable
	dAK4458VN.SoundControl.L2    = 0;      // Zero Detect Flag Enable Bit for the DZF pin 0: Disable (default)  1: Enable
	dAK4458VN.SoundControl.R1    = 0;      // Zero Detect Flag Enable Bit for the DZF pin 0: Disable (default)  1: Enable
	dAK4458VN.SoundControl.L1    = 0;      // Zero Detect Flag Enable Bit for the DZF pin 0: Disable (default)  1: Enable
	ExchangeStatus(Transmit_I2C2(DEVICE_ADDR_DAC_AK4458VN<<1, NOT_USED, NOT_USED, (uint8_t *)&dAK4458VN.SoundControl, 2, 200, ExchangeMainSteam), DAC_WRITE);

	dAK4458VN.Control6.ADDR      = 0x0A;   // Register address
	dAK4458VN.Control6.DEM20     = 1;      // DAC2 De-emphasis Response (Table 17) Default value is “01”. (OFF)
	dAK4458VN.Control6.DEM21     = 0;      // DAC2 De-emphasis Response (Table 17) Default value is “01”. (OFF)
	dAK4458VN.Control6.PW1       = 1;      // Power Down control for DAC 0: power OFF 1: power ON (default)
	dAK4458VN.Control6.PW2       = 1;      // Power Down control for DAC 0: power OFF 1: power ON (default)
	dAK4458VN.Control6.SDS2      = 0;      // Data Select 0: Normal Operation 1: Output Other Slot Data (Table 14) Default value is “000”.
	dAK4458VN.Control6.SDS1      = 0;      // Data Select 0: Normal Operation 1: Output Other Slot Data (Table 14) Default value is “000”.
	dAK4458VN.Control6.TDM0      = 0;      // TDM Mode Select Default value is “00”.
	dAK4458VN.Control6.TDM1      = 0;      // TDM Mode Select Default value is “00”.
	ExchangeStatus(Transmit_I2C2(DEVICE_ADDR_DAC_AK4458VN<<1, NOT_USED, NOT_USED, (uint8_t *)&dAK4458VN.Control6, 2, 200, ExchangeMainSteam), DAC_WRITE);

	dAK4458VN.Control7.ADDR     = 0x0B;   // Register address
	dAK4458VN.Control7.NotUsed  = 0;      // Not Used
	dAK4458VN.Control7.DCHAIN   = 0;      // Daisy Chain Mode Enable 0: Daisy Chain Mode Disable (default) 1: Daisy Chain Mode Enable
	dAK4458VN.Control7.PW3      = 1;      // Power Down control for DAC 0: power OFF 1: power ON (default)
	dAK4458VN.Control7.PW4      = 1;      // Power Down control for DAC 0: power OFF 1: power ON (default)
	dAK4458VN.Control7.SDS0     = 0;      // Data Select 0: Normal Operation 1: Output Other Slot Data (Table 14) Default value is “000”.
	dAK4458VN.Control7.NotUsed2 = 0;      // Not Used
	dAK4458VN.Control7.ATS0     = 0;      // Digital attenuator transition time setting  (Table 19) Default value is “00”.
	dAK4458VN.Control7.ATS1     = 0;      // Digital attenuator transition time setting  (Table 19) Default value is “00”.
	ExchangeStatus(Transmit_I2C2(DEVICE_ADDR_DAC_AK4458VN<<1, NOT_USED, NOT_USED, (uint8_t *)&dAK4458VN.Control7, 2, 200, ExchangeMainSteam), DAC_WRITE);

	dAK4458VN.Control8.ADDR     = 0x0C;   // Register address
	dAK4458VN.Control8.FIR0     = 0;      // FIR Filter Control Default value is “000”.
	dAK4458VN.Control8.FIR1     = 0;      // FIR Filter Control Default value is “000”.
	dAK4458VN.Control8.FIR2     = 0;      // FIR Filter Control Default value is “000”.
	dAK4458VN.Control8.NotUsed  = 0;      // Not Used
	dAK4458VN.Control8.INVL3    = 1;      // Output Phase Inverting Bit 0: Normal (default)  1: Inverted
	dAK4458VN.Control8.INVR3    = 0;      // Output Phase Inverting Bit 0: Normal (default)  1: Inverted
	dAK4458VN.Control8.INVL4    = 1;      // Output Phase Inverting Bit 0: Normal (default)  1: Inverted
	dAK4458VN.Control8.INVR4    = 0;      // Output Phase Inverting Bit 0: Normal (default)  1: Inverted
	ExchangeStatus(Transmit_I2C2(DEVICE_ADDR_DAC_AK4458VN<<1, NOT_USED, NOT_USED, (uint8_t *)&dAK4458VN.Control8, 2, 200, ExchangeMainSteam), DAC_WRITE);

	dAK4458VN.Control9.ADDR     = 0x0D;   // Register address
	dAK4458VN.Control9.NotUsed  = 0;      // Not Used
	dAK4458VN.Control9.SELLR3   = 0;      // The data selection of DAC3 L channel and R channel, when MONO mode (Table 24) Default value is “0”.
	dAK4458VN.Control9.SELLR4   = 0;      // The data selection of DAC4 L channel and R channel, when MONO mode (Table 25) Default value is “0”.
	dAK4458VN.Control9.NotUsed2 = 0;      // Not Used
	dAK4458VN.Control9.MONO2    = 0;      // DAC2 enters Mono output mode when MONO2 bit =“1”. (Table 23) 0: Stereo mode (default) 1: MONO mode
	dAK4458VN.Control9.MONO3    = 0;      // DAC3 enters Mono output mode when MONO2 bit =“1”. (Table 23) 0: Stereo mode (default) 1: MONO mode
	dAK4458VN.Control9.MONO4    = 0;      // DAC4 enters Mono output mode when MONO2 bit =“1”. (Table 23) 0: Stereo mode (default) 1: MONO mode
	ExchangeStatus(Transmit_I2C2(DEVICE_ADDR_DAC_AK4458VN<<1, NOT_USED, NOT_USED, (uint8_t *)&dAK4458VN.Control9, 2, 200, ExchangeMainSteam), DAC_WRITE);

	dAK4458VN.Control10.ADDR    = 0x0E;   // Register address
	dAK4458VN.Control10.NotUsed = 0;      // Not Used
	dAK4458VN.Control10.DEM30   = 1;      // DAC3 De-emphasis Response (Table 17) Default value is “01”. (OFF)
	dAK4458VN.Control10.DEM31   = 0;      // DAC3 De-emphasis Response (Table 17) Default value is “01”. (OFF)
	dAK4458VN.Control10.DEM40   = 1;      // DAC4 De-emphasis Response (Table 17) Default value is “01”. (OFF)
	dAK4458VN.Control10.DEM41   = 0;      // DAC4 De-emphasis Response (Table 17) Default value is “01”. (OFF)
	ExchangeStatus(Transmit_I2C2(DEVICE_ADDR_DAC_AK4458VN<<1, NOT_USED, NOT_USED, (uint8_t *)&dAK4458VN.Control10, 2, 200, ExchangeMainSteam), DAC_WRITE);

}


void Mute_DAC_AK4458(uint8_t state){

	if(state == ON){
		dAK4458VN.Control2.SMUTE = 1;
	}else{
		dAK4458VN.Control2.SMUTE = 0;
	}
	ExchangeStatus(Transmit_I2C2(DEVICE_ADDR_DAC_AK4458VN<<1, NOT_USED, NOT_USED, (uint8_t *)&dAK4458VN.Control2, 2, 200, ExchangeMainSteam), DAC_WRITE);
}

void Volume_DAC_AK4458VN(uint8_t *pData){
	uint8_t AK4458VNCH1_2[3] = { 0x03, pData[0], pData[0] };
	uint8_t AK4458VNCH3_8[7] = { 0x0F, pData[0], pData[0], pData[0], pData[0], pData[0], pData[0] };

	ExchangeStatus(Transmit_I2C2(DEVICE_ADDR_DAC_AK4458VN<<1, NOT_USED, NOT_USED, AK4458VNCH1_2, 3, 200, ExchangeMainSteam), DAC_WRITE);
	ExchangeStatus(Transmit_I2C2(DEVICE_ADDR_DAC_AK4458VN<<1, NOT_USED, NOT_USED, AK4458VNCH3_8, 7, 400, ExchangeMainSteam), DAC_WRITE);
}















