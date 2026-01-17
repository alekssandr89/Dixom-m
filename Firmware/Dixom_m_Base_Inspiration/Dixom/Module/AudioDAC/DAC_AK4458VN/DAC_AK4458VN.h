#ifndef __AK4458VN_H__
#define __AK4458VN_H__
#include <stdint.h>

void             Init_DAC_AK4458VN(void);
void             Mute_DAC_AK4458(uint8_t state);
void             Volume_DAC_AK4458VN(uint8_t *pData);

typedef struct
{
	uint8_t  ADDR;                   // Register address 0x00
	uint8_t  RSTN             :1;    // Internal Timing Reset
	uint8_t  DIF0             :1;    // Audio Data Interface Modes
	uint8_t  DIF1             :1;    // Audio Data Interface Modes
	uint8_t  DIF2             :1;    // Audio Data Interface Modes
	uint8_t  NotUsed          :3;    // NotUsed
	uint8_t  ACKS             :1;    // Master Clock Frequency Auto Setting Mode Enable (PCM only)
} tControl1 ;

typedef struct
{
	uint8_t  ADDR;                    // Register address 0x01
	uint8_t  SMUTE             :1;    // Soft Mute Enable.
	uint8_t  DEM10             :1;    // DAC1 De-emphasis Response
	uint8_t  DEM11             :1;    // DAC1 De-emphasis Response
	uint8_t  DFS0              :1;    // Sampling Speed Control
	uint8_t  DFS1              :1;    // Sampling Speed Control
	uint8_t  SD                :1;    // Short delay Filter Enable
	uint8_t  NotUsed           :2;    // NotUsed
} tControl2AK ;

typedef struct
{
	uint8_t  ADDR;                    // Register address 0x02
	uint8_t  SLOW              :1;    // Slow Roll-off Filter Enable
	uint8_t  NotUsed           :1;    // NotUsed
	uint8_t  SELLR1            :1;    // The data selection of DAC1 L channel and R channel, when MONO mode
	uint8_t  DZFB              :1;    // Inverting Enable of DZF
	uint8_t  MONO1             :1;    // DAC1 enters monaural output mode when MONO bit = “1”
	uint8_t  DCKB              :1;    // Polarity of DCLK (DSD Only)
	uint8_t  DCKS              :1;    // Master Clock Frequency Select at DSD mode (DSD only)
	uint8_t  DP                :1;    // DSD/PCM Mode Select
} tControl3 ;

typedef struct
{
	uint8_t  ADDR;                    // Register address0x05
	uint8_t  SSLOW             :1;    // Digital Filter Bypass Mode Enable
	uint8_t  DFS2              :1;    // Sampling Speed Control
	uint8_t  NotUsed           :1;    // NotUsed
	uint8_t  SELLR2            :1;    // Data Selection of DAC2 L channel and R channel, when MONO mode
	uint8_t  INVL1             :1;    // AOUTL1 Output Phase Inverting Bit
	uint8_t  INVR1             :1;    // AOUTR1 Output Phase Inverting Bit
	uint8_t  INVL2             :1;    // AOUTL2 Output Phase Inverting Bit
	uint8_t  INVR2             :1;    // AOUTR2 Output Phase Inverting Bit
} tControl4 ;

typedef struct
{
	uint8_t  ADDR;                    // Register address 0x06
	uint8_t  DSDSEL0           :1;    // DSD sampling speed control
	uint8_t  DSDD              :1;    // DSD play back path control
	uint8_t  NotUsed           :1;    // NotUsed
	uint8_t  DMRE              :1;    // DSD mute release
	uint8_t  DMC               :1;    // DSD mute control
	uint8_t  DMR1              :1;    //
	uint8_t  DML1              :1;    //
	uint8_t  DDM               :1;    // DSD Data Mute
} tDSD1;

typedef struct
{
	uint8_t  ADDR;                    // Register address 0x07
	uint8_t  SYNCE             :1;    // SYNC Mode Enable
	uint8_t  One               :1;    // ?
	uint8_t  NotUsed           :2;    // NotUsed
	uint8_t  R4                :1;    // Zero Detect Flag Enable Bit for the DZF pin
	uint8_t  L4                :1;    // Zero Detect Flag Enable Bit for the DZF pin
	uint8_t  R3                :1;    // Zero Detect Flag Enable Bit for the DZF pin
	uint8_t  L3                :1;    // Zero Detect Flag Enable Bit for the DZF pin

} tControl5 ;

typedef struct
{
	uint8_t  ADDR;                    // Register address 0x08
	uint8_t  SC0               :1;    // ?
	uint8_t  SC1               :1;    // DSD play back path control
	uint8_t  NotUsed           :2;    // NotUsed
	uint8_t  R2                :1;    // Zero Detect Flag Enable Bit for the DZF pin
	uint8_t  L2                :1;    // Zero Detect Flag Enable Bit for the DZF pin
	uint8_t  R1                :1;    // Zero Detect Flag Enable Bit for the DZF pin
	uint8_t  L1                :1;    // Zero Detect Flag Enable Bit for the DZF pin
} tSoundControl ;

typedef struct
{
	uint8_t  ADDR;                    // Register address 0x09
	uint8_t  DSDSEL1           :1;    // DSD Sampling Speed Control
	uint8_t  DSDF              :1;    // DSD Filter Select
	uint8_t  DMR4              :1;    //
	uint8_t  DML4              :1;    //
	uint8_t  DMR3              :1;    //
	uint8_t  DML3              :1;    //
	uint8_t  DMR2              :1;    //
	uint8_t  DML2              :1;    //
} tDSD2 ;

typedef struct
{
	uint8_t  ADDR;                    // Register address 0x0A
	uint8_t  DEM20             :1;    // DAC2 De-emphasis Response
	uint8_t  DEM21             :1;    // DAC2 De-emphasis Response
	uint8_t  PW1               :1;    // Power Down control for DAC
	uint8_t  PW2               :1;    // Power Down control for DAC
	uint8_t  SDS2              :1;    // Data Select
	uint8_t  SDS1              :1;    // Data Select
	uint8_t  TDM0              :1;    // TDM Mode Select
	uint8_t  TDM1              :1;    // TDM Mode Select

} tControl6 ;

typedef struct
{
	uint8_t  ADDR;                    // Register address 0x0B
	uint8_t  NotUsed           :1;    // NotUsed
	uint8_t  DCHAIN            :1;    // Daisy Chain Mode Enable
	uint8_t  PW3               :1;    // Power Down control for DAC
	uint8_t  PW4               :1;    // Power Down control for DAC
	uint8_t  SDS0              :1;    // Data Select
	uint8_t  NotUsed2          :1;    // NotUsed
	uint8_t  ATS0              :1;    // DAC Digital attenuator transition time setting
	uint8_t  ATS1              :1;    // DAC Digital attenuator transition time setting
} tControl7 ;


typedef struct
{
	uint8_t  ADDR;                    // Register address 0x0C
	uint8_t  FIR0              :1;    // FIR Filter Control
	uint8_t  FIR1              :1;    // FIR Filter Control
	uint8_t  FIR2              :1;    // FIR Filter Control
	uint8_t  NotUsed           :1;    // NotUsed
	uint8_t  INVL3             :1;    // AOUTL3 Output Phase Inverting Bit
	uint8_t  INVR3             :1;    // AOUTR3 Output Phase Inverting Bit
	uint8_t  INVL4             :1;    // AOUTL4 Output Phase Inverting Bit
	uint8_t  INVR4             :1;    // AOUTR4 Output Phase Inverting Bit
} tControl8 ;

typedef struct
{
	uint8_t  ADDR;                    // Register address 0x0D
	uint8_t  NotUsed           :2;    // NotUsed
	uint8_t  SELLR3            :1;    // The data selection of DAC3 L channel and R channel, when MONO mode
	uint8_t  SELLR4            :1;    // The data selection of DAC4 L channel and R channel, when MONO mode
	uint8_t  NotUsed2          :1;    // NotUsed
	uint8_t  MONO2             :1;    // DAC2 enters Mono output mode when MONO2 bit =“1”
	uint8_t  MONO3             :1;    // DAC3 enters Mono output mode when MONO3 bit =“1”
	uint8_t  MONO4             :1;    // DAC4 enters Mono output mode when MONO4 bit =“1”
} tControl9 ;

typedef struct
{
	uint8_t  ADDR;                    // Register address 0x0E
	uint8_t  NotUsed           :4;    // NotUsed
	uint8_t  DEM30             :1;    // DAC3 De-emphasis Response
	uint8_t  DEM31             :1;    // DAC3 De-emphasis Response
	uint8_t  DEM40             :1;    // DAC4 De-emphasis Response
	uint8_t  DEM41             :1;    // DAC4 De-emphasis Response
} tControl10 ;




typedef struct
{
	tControl1 Control1;
	tControl2AK Control2;
	tControl3 Control3;
	tControl4 Control4;
	tDSD1 DSD1;
	tControl5 Control5;
	tSoundControl SoundControl;
	tDSD2 DSD2;
	tControl6 Control6;
	tControl7 Control7;
	tControl8 Control8;
	tControl9 Control9;
	tControl10 Control10;
} sDAC_AK4458VN;































#endif
