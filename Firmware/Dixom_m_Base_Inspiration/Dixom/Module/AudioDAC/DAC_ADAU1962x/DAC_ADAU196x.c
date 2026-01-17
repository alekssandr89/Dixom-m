#include <Sigma.h>
#include "DAC_ADAU196x.h"
#include "Exchange.h"
#include "Structures.h"

extern sDixom Dixom;

void Init_DAC_ADAU196x(){

	tPLL_CLK_CTRL0 PLL_CLK_CTRL0;
	PLL_CLK_CTRL0.ADDR          = 0x00;
	PLL_CLK_CTRL0.PLLIN_0       = 0;      // PLL Input Select 00 MCLKI or XTALI. 01 DLRCLK. 10 Reserved. 11 Reserved.
	PLL_CLK_CTRL0.PLLIN_1       = 0;      // PLL Input Select 00 MCLKI or XTALI. 01 DLRCLK. 10 Reserved. 11 Reserved.
	PLL_CLK_CTRL0.XTAL_SET_0    = 1;      // XTAL Oscillator Setting. XTALO pin status. 00 XTAL Oscillator Enabled. 01 Reserved. 10 Reserved. 11 XTALO Off.
	PLL_CLK_CTRL0.XTAL_SET_1    = 1;      // XTAL Oscillator Setting. XTALO pin status. 00 XTAL Oscillator Enabled. 01 Reserved. 10 Reserved. 11 XTALO Off.
	PLL_CLK_CTRL0.SOFT_RST      = 0;      // Software Reset Control. 0 Normal Operation. 1 Device in Reset.
	PLL_CLK_CTRL0.MCS_0         = 1;      // Master Clock Select 00 256xFS, 01 384xFS, 10 512xFS, 11 768xFS
	PLL_CLK_CTRL0.MCS_1         = 0;      // Master Clock Select 00 256xFS, 01 384xFS, 10 512xFS, 11 768xFS
	PLL_CLK_CTRL0.PUP           = 1;      // Master Power-Up Control 0 Master Power-Down 1 Master Power-Up

	tPLL_CLK_CTRL1 PLL_CLK_CTRL1;
	PLL_CLK_CTRL1.ADDR          = 0x01;
	PLL_CLK_CTRL1.LOPWR_MODE_0  = 0;      // Global Power/Performance Adjust 00 I2	C register settings. 01 Reserved. 10 Low Power. 11 Lowest Power.
	PLL_CLK_CTRL1.LOPWR_MODE_1  = 0;      // Global Power/Performance Adjust 00 I2	C register settings. 01 Reserved. 10 Low Power. 11 Lowest Power.
	PLL_CLK_CTRL1.MCLKO_SEL_0   = 1;      // MCLKO Output Frequency
	PLL_CLK_CTRL1.MCLKO_SEL_1   = 1;      // MCLKO Output Frequency
	PLL_CLK_CTRL1.PLL_MUTE      = 0;      // PLL Automute Enable/Lock 0 No DAC Automute. 1 DAC Automute on PLL Unlock.
	PLL_CLK_CTRL1.PLL_LOCK      = 0;      // PLL Lock Indicator. 0 PLL Not Locked.  1 PLL Locked.
	PLL_CLK_CTRL1.VREF_EN       = 1;      // Internal Voltage Reference Enable 0 Disabled. 1 Enabled.
	PLL_CLK_CTRL1.CLK_SEL       = 1;      // DAC Clock Select. 0 MCLK from PLL. 1 MCLK from MCLKI or XTALI.

	tPDN_THRMSENS_CTRL_1 PDN_THRMSENS_CTRL_1;
	PDN_THRMSENS_CTRL_1.ADDR         = 0x02;
	PDN_THRMSENS_CTRL_1.THRM_RATE_0  = 1;   // Conversion Time Interval 00 4 sec/Conversion. 01 0.5 sec/Conversion. 10 1 sec/Conversion. 11 2 sec/Conversion.
	PDN_THRMSENS_CTRL_1.THRM_RATE_1  = 1;   // Conversion Time Interval 00 4 sec/Conversion. 01 0.5 sec/Conversion. 10 1 sec/Conversion. 11 2 sec/Conversion.
	PDN_THRMSENS_CTRL_1.THRM_MODE    = 1;   // Continuous vs. One Shot. 0 Continuous Operation. 1 One Shot Mode.
	PDN_THRMSENS_CTRL_1.THRM_GO      = 0;   // One Shot Conversion Mode 0 Reset. 1 Convert Temperature.
	PDN_THRMSENS_CTRL_1.RESERVED     = 0;
	PDN_THRMSENS_CTRL_1.TS_PDN       = 1;   // Temperature Sensor Power-Down. 0 Temperature Sensor On. 1 Temperature Sensor Power-Down.
	PDN_THRMSENS_CTRL_1.PLL_PDN      = 0;   // PLL Power-Down 0 PLL Normal Operation. 1 PLL Power-Down.
	PDN_THRMSENS_CTRL_1.VREG_PDN     = 1;   // Voltage Regulator Power-Down. 0 Voltage Regulator Normal Operation. 1 Voltage Regulator Power-Down.

	tPDN_CTRL2 PDN_CTRL2;
	PDN_CTRL2.ADDR              = 0x03;
	PDN_CTRL2.DAC08_PDN         = 0;     // Power-Down  0 Normal Operation  1 Power-Down
	PDN_CTRL2.DAC07_PDN         = 0;     // Power-Down  0 Normal Operation  1 Power-Down
	PDN_CTRL2.DAC06_PDN         = 0;     // Power-Down  0 Normal Operation  1 Power-Down
	PDN_CTRL2.DAC05_PDN         = 0;     // Power-Down  0 Normal Operation  1 Power-Down
	PDN_CTRL2.DAC04_PDN         = 0;     // Power-Down  0 Normal Operation  1 Power-Down
	PDN_CTRL2.DAC03_PDN         = 0;     // Power-Down  0 Normal Operation  1 Power-Down
	PDN_CTRL2.DAC02_PDN         = 0;     // Power-Down  0 Normal Operation  1 Power-Down
	PDN_CTRL2.DAC01_PDN         = 0;     // Power-Down  0 Normal Operation  1 Power-Down

	tPDN_CTRL3 PDN_CTRL3;
	PDN_CTRL3.ADDR              = 0x04;
	PDN_CTRL3.RESERVED          = 0;
	PDN_CTRL3.DAC09_PDN         = 0;     // Power-Down  0 Normal Operation  1 Power-Down
	PDN_CTRL3.DAC10_PDN         = 0;     // Power-Down  0 Normal Operation  1 Power-Down
	PDN_CTRL3.DAC11_PDN         = 0;     // Power-Down  0 Normal Operation  1 Power-Down
	PDN_CTRL3.DAC12_PDN         = 0;     // Power-Down  0 Normal Operation  1 Power-Down

	tDAC_CTRL0     DAC_CTRL0;
	DAC_CTRL0.ADDR        = 0x06;
	DAC_CTRL0.SDATA_FMT_0 = 0;            // SDATA Format 00 I2	S = 1 BCLK Cycle Delay.  01 Left Justified = 0 BCLK Cycle Delay. 10 Right Justified 24-Bit Data = 8 BCLK Cycle Delay  11 Right Justified 16-Bit Data = 16 BCLK Cycle Delay
	DAC_CTRL0.SDATA_FMT_0 = 0;            // SDATA Format 00 I2	S = 1 BCLK Cycle Delay.  01 Left Justified = 0 BCLK Cycle Delay. 10 Right Justified 24-Bit Data = 8 BCLK Cycle Delay  11 Right Justified 16-Bit Data = 16 BCLK Cycle Delay
	DAC_CTRL0.SAI_0       = 0;            // Serial Audio Interface
	DAC_CTRL0.SAI_1       = 1;            // Serial Audio Interface
	DAC_CTRL0.SAI_2       = 0;            // Serial Audio Interface
	DAC_CTRL0.FS_0        = 0;            // Sample Rate Select.
	DAC_CTRL0.FS_1        = 0;            // Sample Rate Select.
	DAC_CTRL0.MMUTE       = 0;            // DAC Master Mute  0 Normal Operation. 1 All Channels Muted.

	tDAC_CTRL1 DAC_CTRL1;
	DAC_CTRL1.ADDR              = 0x07;
	DAC_CTRL1.BCLK_GEN          = 0;     // DBCLK Generation 0 Normal OperationвЂ”DBCLK. 1 Internal DBCLK Generation.
	DAC_CTRL1.LRCLK_MODE        = 0;     // DLRCLK Mode Select 0 50% Duty Cycle DLRCLK. 1 Pulse Mode.
	DAC_CTRL1.LRCLK_POL         = 0;     // DLRCLK Polarity.  0 Left/Odd Channels Are DLRCLK Low (Normal). 1 Left/Odd Channels Are DLRCLK High (Inverted).
	DAC_CTRL1.SAI_MSB           = 0;     // MSB Position.  0 MSB First DSDATAx. 1 LSB First DSDATAx.
	DAC_CTRL1.RESERVED          = 0;
	DAC_CTRL1.BCLK_RATE         = 0;     // DBCLK Rate 0 32 Cycles per Frame. 1 16 Cycles per Frame.
	DAC_CTRL1.BCLK_EDGE         = 0;     // DBCLK Active Edge 0 Latch in Rising Edge. 1 Latch in Falling Edge.
	DAC_CTRL1.SAI_MS            = 0;     // Serial Interface Master 0 DLRCLK/DBCLK Slave. 1 DLRCLK/DBCLK Master.

	tDAC_CTRL2 DAC_CTRL2;
	DAC_CTRL2.ADDR              = 0x08;
	DAC_CTRL2.RESERVED          = 0;
	DAC_CTRL2.VREG_CTRL_0       = 0;     // Voltage Regulator Control  00 Regulator Output = 2.5 V. 01 Regulator Output = 2.75 V. 10 Regulator Output = 3.0 V. 11 Regulator Output = 3.3 V.
	DAC_CTRL2.VREG_CTRL_1       = 0;     // Voltage Regulator Control  00 Regulator Output = 2.5 V. 01 Regulator Output = 2.75 V. 10 Regulator Output = 3.0 V. 11 Regulator Output = 3.3 V.
	DAC_CTRL2.BCLK_TDMC         = 0;     // DBCLK Rate in TDM Mode 0 32 BCLK Cycles/Channel Slot. 1 16 BCLK Cycles/Channel Slot.
	DAC_CTRL2.DAC_POL           = 0;     // DAC Output Polarity 0 Noninverted DAC Output. 1 Inverted DAC Output.
	DAC_CTRL2.AUTO_MUTE_EN      = 1;     // Automute Enable 0 Auto-Zero Input Mute Disabled. 1 Auto-Zero Input Mute Enabled.
	DAC_CTRL2.DAC_OSR           = 1;     // DAC Oversampling Rate (OSR). OSR selection. 0 256 Г— fS DAC Oversampling. 1 128 Г— fS DAC Oversampling.
	DAC_CTRL2.DE_EMP_EN         = 0;     // De-Emphasis Enable. 0 No De-Emphasis/Flat. 1 De-Emphasis Enabled.

	tDAC_MUTE1 DAC_MUTE1;
	DAC_MUTE1.ADDR              = 0x09;
	DAC_MUTE1.DAC08_MUTE        = 0;     // Soft Mute 0 Normal Operation 1 Mute
	DAC_MUTE1.DAC07_MUTE        = 0;     // Soft Mute 0 Normal Operation 1 Mute
	DAC_MUTE1.DAC06_MUTE        = 0;     // Soft Mute 0 Normal Operation 1 Mute
	DAC_MUTE1.DAC05_MUTE        = 0;     // Soft Mute 0 Normal Operation 1 Mute
	DAC_MUTE1.DAC04_MUTE        = 0;     // Soft Mute 0 Normal Operation 1 Mute
	DAC_MUTE1.DAC03_MUTE        = 0;     // Soft Mute 0 Normal Operation 1 Mute
	DAC_MUTE1.DAC02_MUTE        = 0;     // Soft Mute 0 Normal Operation 1 Mute
	DAC_MUTE1.DAC01_MUTE        = 0;     // Soft Mute 0 Normal Operation 1 Mute

	tDAC_MUTE2 DAC_MUTE2;
	DAC_MUTE2.ADDR              = 0x0A;
	DAC_MUTE2.DAC16_MUTE        = 0;     // Soft Mute 0 Normal Operation 1 Mute
	DAC_MUTE2.DAC15_MUTE        = 0;     // Soft Mute 0 Normal Operation 1 Mute
	DAC_MUTE2.DAC14_MUTE        = 0;     // Soft Mute 0 Normal Operation 1 Mute
	DAC_MUTE2.DAC13_MUTE        = 0;     // Soft Mute 0 Normal Operation 1 Mute
	DAC_MUTE2.DAC12_MUTE        = 0;     // Soft Mute 0 Normal Operation 1 Mute
	DAC_MUTE2.DAC11_MUTE        = 0;     // Soft Mute 0 Normal Operation 1 Mute
	DAC_MUTE2.DAC10_MUTE        = 0;     // Soft Mute 0 Normal Operation 1 Mute
	DAC_MUTE2.DAC09_MUTE        = 0;     // Soft Mute 0 Normal Operation 1 Mute

	tCM_SEL_PAD_STRGTH CM_SEL_PAD_STRGTH;
	CM_SEL_PAD_STRGTH.ADDR      = 0x1C;
	CM_SEL_PAD_STRGTH.RESERVED  = 0;
	CM_SEL_PAD_STRGTH.PAD_DRV   = 1;     //Output Pad Drive Strength Control 0 4 mA Drive for All Pads. 1 8 mA Drive for All Pads.
	CM_SEL_PAD_STRGTH.RESERVED1 = 0;
	CM_SEL_PAD_STRGTH.RESERVED2 = 0;
	CM_SEL_PAD_STRGTH.RESERVED3 = 0;
	CM_SEL_PAD_STRGTH.RESERVED4 = 0;
	CM_SEL_PAD_STRGTH.RESERVED5 = 0;
	CM_SEL_PAD_STRGTH.RESERVED6 = 0;

	tDAC_POWER1 DAC_POWER1;
	DAC_POWER1.ADDR             = 0x1D;
	DAC_POWER1.DAC04_POWER      = 2;     // DAC Power Control 00 Low Power 01 Lowest Power  10 Best Performance 11 Good Performance
	DAC_POWER1.DAC03_POWER      = 2;     // DAC Power Control 00 Low Power 01 Lowest Power  10 Best Performance 11 Good Performance
	DAC_POWER1.DAC02_POWER      = 2;     // DAC Power Control 00 Low Power 01 Lowest Power  10 Best Performance 11 Good Performance
	DAC_POWER1.DAC01_POWER      = 2;     // DAC Power Control 00 Low Power 01 Lowest Power  10 Best Performance 11 Good Performance

	tDAC_POWER2 DAC_POWER2;
	DAC_POWER2.ADDR             = 0x1E;
	DAC_POWER2.DAC08_POWER      = 2;     // DAC Power Control 00 Low Power 01 Lowest Power  10 Best Performance 11 Good Performance
	DAC_POWER2.DAC07_POWER      = 2;     // DAC Power Control 00 Low Power 01 Lowest Power  10 Best Performance 11 Good Performance
	DAC_POWER2.DAC06_POWER      = 2;     // DAC Power Control 00 Low Power 01 Lowest Power  10 Best Performance 11 Good Performance
	DAC_POWER2.DAC05_POWER      = 2;     // DAC Power Control 00 Low Power 01 Lowest Power  10 Best Performance 11 Good Performance

	tDAC_POWER3 DAC_POWER3;
	DAC_POWER3.ADDR             = 0x1F;
	DAC_POWER3.DAC12_POWER      = 2;     // DAC Power Control 00 Low Power 01 Lowest Power  10 Best Performance 11 Good Performance
	DAC_POWER3.DAC11_POWER      = 2;     // DAC Power Control 00 Low Power 01 Lowest Power  10 Best Performance 11 Good Performance
	DAC_POWER3.DAC10_POWER      = 2;     // DAC Power Control 00 Low Power 01 Lowest Power  10 Best Performance 11 Good Performance
	DAC_POWER3.DAC09_POWER      = 2;     // DAC Power Control 00 Low Power 01 Lowest Power  10 Best Performance 11 Good Performance

/*
	Transmit_DAC(NOT_USED, (uint8_t *)&PLL_CLK_CTRL0,        2, 200, ExchangeMainSteam );
	Transmit_DAC(NOT_USED, (uint8_t *)&PLL_CLK_CTRL1,        2, 200, ExchangeMainSteam );
	Transmit_DAC(NOT_USED, (uint8_t *)&PLL_CLK_CTRL1,        2, 200, ExchangeMainSteam );
	Transmit_DAC(NOT_USED, (uint8_t *)&PDN_THRMSENS_CTRL_1,  2, 200, ExchangeMainSteam );
	Transmit_DAC(NOT_USED, (uint8_t *)&PDN_CTRL2,            2, 200, ExchangeMainSteam );
	Transmit_DAC(NOT_USED, (uint8_t *)&PDN_CTRL3,            2, 200, ExchangeMainSteam );
	Transmit_DAC(NOT_USED, (uint8_t *)&DAC_CTRL1,            2, 200, ExchangeMainSteam );
	Transmit_DAC(NOT_USED, (uint8_t *)&DAC_CTRL2,            2, 200, ExchangeMainSteam );
	Transmit_DAC(NOT_USED, (uint8_t *)&DAC_MUTE1,            2, 200, ExchangeMainSteam );
	Transmit_DAC(NOT_USED, (uint8_t *)&DAC_MUTE2,            2, 200, ExchangeMainSteam );
	Transmit_DAC(NOT_USED, (uint8_t *)&CM_SEL_PAD_STRGTH,    2, 200, ExchangeMainSteam );
	Transmit_DAC(NOT_USED, (uint8_t *)&DAC_POWER1,           2, 200, ExchangeMainSteam );
	Transmit_DAC(NOT_USED, (uint8_t *)&DAC_POWER2,           2, 200, ExchangeMainSteam );
	Transmit_DAC(NOT_USED, (uint8_t *)&DAC_POWER3,           2, 200, ExchangeMainSteam);
*/
}


void Mute_DAC_ADAU1962(uint8_t state){

	if(state == ON){
		Dixom.Module.AudioDAC.ADAU1962x.DAC_CTRL0.MMUTE = 1;
	}else{
		Dixom.Module.AudioDAC.ADAU1962x.DAC_CTRL0.MMUTE = 0;
	}
	ExchangeStatus(Transmit_I2C2(DEVICE_ADDR_DAC_ADAU1962, NOT_USED, NOT_USED, (uint8_t *)&Dixom.Module.AudioDAC.ADAU1962x.DAC_CTRL0, 2, 200, ExchangeMainSteam), DAC_WRITE);

}

void Volume_DAC_ADAU1962(uint8_t *pData){
	ExchangeStatus(Transmit_I2C2(DEVICE_ADDR_DAC_ADAU1962, NOT_USED, NOT_USED, pData, 2, 200, ExchangeMainSteam), DAC_WRITE);
}





