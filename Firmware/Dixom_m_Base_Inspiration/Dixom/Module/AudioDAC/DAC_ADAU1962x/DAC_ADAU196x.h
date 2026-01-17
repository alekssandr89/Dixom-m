#ifndef __ADAU196X_H__
#define __ADAU196X_H__
#include <stdint.h>


void             Init_DAC_ADAU196x();
void             Mute_DAC_ADAU1962(uint8_t state);
void             Volume_DAC_ADAU1962(uint8_t *pData);

/* Serial Audio Interface
 000 Stereo (I2S, left justified, right justified).
 001 TDM2 = Octal Line.
 010 TDM4 = Quad Line.
 011 TDM8 = Dual Line.
 100 TDM16 = Single Line (48 kHz).
 101 Reserved.
 110 Reserved.
 111 Reserved.


 Sample Rate Select.
 00 32 kHz/44.1 kHz/48 kHz.
 01 64 kHz/88.2 kHz/96 kHz.
 10 128 kHz/176.4 kHz/192 kHz.
 11 128 kHz/176.4 kHz/192 kHz Low Propagation Delay.
 */

typedef struct
{
	uint8_t  ADDR;                  // Register address 0x00
	uint8_t  PUP             :1;    // Master Power-Up Control.
	uint8_t  MCS_0           :1;    // Master Clock Select
	uint8_t  MCS_1           :1;    // Master Clock Select
	uint8_t  SOFT_RST        :1;    // Software Reset Control
	uint8_t  XTAL_SET_0      :1;    // Audio Data Interface Modes
	uint8_t  XTAL_SET_1      :1;    // Audio Data Interface Modes
	uint8_t  PLLIN_0         :1;    // PLL Input Select.
	uint8_t  PLLIN_1         :1;    // PLL Input Select.
} tPLL_CLK_CTRL0;

typedef struct
{
	uint8_t  ADDR;                  // Register address 0x01
	uint8_t  CLK_SEL         :1;
	uint8_t  VREF_EN         :1;
	uint8_t  PLL_LOCK        :1;
	uint8_t  PLL_MUTE        :1;
	uint8_t  MCLKO_SEL_0     :1;
	uint8_t  MCLKO_SEL_1     :1;
	uint8_t  LOPWR_MODE_0    :1;
	uint8_t  LOPWR_MODE_1    :1;
} tPLL_CLK_CTRL1;

typedef struct
{
	uint8_t  ADDR;                  // Register address 0x02
	uint8_t  VREG_PDN        :1;
	uint8_t  PLL_PDN         :1;
	uint8_t  TS_PDN          :1;
	uint8_t  RESERVED        :1;
	uint8_t  THRM_GO         :1;
	uint8_t  THRM_MODE       :1;
	uint8_t  THRM_RATE_0     :1;
	uint8_t  THRM_RATE_1     :1;
} tPDN_THRMSENS_CTRL_1;

typedef struct
{
	uint8_t  ADDR;                  // Register address 0x03
	uint8_t  DAC01_PDN       :1;
	uint8_t  DAC02_PDN       :1;
	uint8_t  DAC03_PDN       :1;
	uint8_t  DAC04_PDN       :1;
	uint8_t  DAC05_PDN       :1;
	uint8_t  DAC06_PDN       :1;
	uint8_t  DAC07_PDN       :1;
	uint8_t  DAC08_PDN       :1;
} tPDN_CTRL2;

typedef struct
{
	uint8_t  ADDR;                  // Register address 0x04
	uint8_t  DAC09_PDN       :1;
	uint8_t  DAC10_PDN       :1;
	uint8_t  DAC11_PDN       :1;
	uint8_t  DAC12_PDN       :1;
	uint8_t  RESERVED        :4;
} tPDN_CTRL3;

typedef struct
{
	uint8_t  ADDR;                  // Register address 0x05
	uint8_t  TEMP            :8;
} tTHRM_TEMP_STAT;

typedef struct
{
	uint8_t  ADDR;                  // Register address 0x06
	uint8_t  MMUTE           :1;
	uint8_t  FS_0            :1;
	uint8_t  FS_1            :1;
	uint8_t  SAI_0           :1;
	uint8_t  SAI_1           :1;
	uint8_t  SAI_2           :1;
	uint8_t  SDATA_FMT_0     :1;
	uint8_t  SDATA_FMT_1     :1;
} tDAC_CTRL0;

typedef struct
{
	uint8_t  ADDR;                  // Register address 0x07
	uint8_t  SAI_MS          :1;
	uint8_t  BCLK_EDGE       :1;
	uint8_t  BCLK_RATE       :1;
	uint8_t  RESERVED        :1;
	uint8_t  SAI_MSB         :1;
	uint8_t  LRCLK_POL       :1;
	uint8_t  LRCLK_MODE      :1;
	uint8_t  BCLK_GEN        :1;
} tDAC_CTRL1;

typedef struct
{
	uint8_t  ADDR;                  // Register address 0x08
	uint8_t  DE_EMP_EN       :1;
	uint8_t  DAC_OSR         :1;
	uint8_t  AUTO_MUTE_EN    :1;
	uint8_t  DAC_POL         :1;
	uint8_t  BCLK_TDMC       :1;
	uint8_t  VREG_CTRL_0     :1;
	uint8_t  VREG_CTRL_1     :1;
	uint8_t  RESERVED        :1;
} tDAC_CTRL2;

typedef struct
{
	uint8_t  ADDR;                  // Register address 0x09
	uint8_t  DAC01_MUTE      :1;
	uint8_t  DAC02_MUTE      :1;
	uint8_t  DAC03_MUTE      :1;
	uint8_t  DAC04_MUTE      :1;
	uint8_t  DAC05_MUTE      :1;
	uint8_t  DAC06_MUTE      :1;
	uint8_t  DAC07_MUTE      :1;
	uint8_t  DAC08_MUTE      :1;
} tDAC_MUTE1;

typedef struct
{
	uint8_t  ADDR;                  // Register address 0x0A
	uint8_t  DAC09_MUTE      :1;
	uint8_t  DAC10_MUTE      :1;
	uint8_t  DAC11_MUTE      :1;
	uint8_t  DAC12_MUTE      :1;
	uint8_t  DAC13_MUTE      :1;
	uint8_t  DAC14_MUTE      :1;
	uint8_t  DAC15_MUTE      :1;
	uint8_t  DAC16_MUTE      :1;
} tDAC_MUTE2;

typedef struct
{
	uint8_t  ADDR;                  // Register address 0x1C
	uint8_t  RESERVED        :1;
	uint8_t  RESERVED1       :1;
	uint8_t  RESERVED2       :1;
	uint8_t  RESERVED3       :1;
	uint8_t  RESERVED4       :1;
	uint8_t  PAD_DRV         :1;
	uint8_t  RESERVED5       :1;
	uint8_t  RESERVED6       :1;
} tCM_SEL_PAD_STRGTH;

typedef struct
{
	uint8_t  ADDR;                  // Register address 0x1D
	uint8_t  DAC01_POWER     :2;
	uint8_t  DAC02_POWER     :2;
	uint8_t  DAC03_POWER     :2;
	uint8_t  DAC04_POWER     :2;
} tDAC_POWER1;

typedef struct
{
	uint8_t  ADDR;                  // Register address 0x1E
	uint8_t  DAC05_POWER     :2;
	uint8_t  DAC06_POWER     :2;
	uint8_t  DAC07_POWER     :2;
	uint8_t  DAC08_POWER     :2;
} tDAC_POWER2;

typedef struct
{
	uint8_t  ADDR;                  // Register address 0x1F
	uint8_t  DAC09_POWER     :2;
	uint8_t  DAC10_POWER     :2;
	uint8_t  DAC11_POWER     :2;
	uint8_t  DAC12_POWER     :2;
} tDAC_POWER3;



typedef struct
{
	tPLL_CLK_CTRL0 PLL_CLK_CTRL0;
	tPLL_CLK_CTRL1 PLL_CLK_CTRL1;
	tPDN_THRMSENS_CTRL_1 PDN_THRMSENS_CTRL_1;
	tPDN_CTRL2 PDN_CTRL2;
	tPDN_CTRL3 PDN_CTRL3;
	tTHRM_TEMP_STAT THRM_TEMP_STAT;
	tDAC_CTRL0 DAC_CTRL0;
	tDAC_CTRL1 DAC_CTRL1;
	tDAC_CTRL2 DAC_CTRL2;
	tDAC_MUTE1 DAC_MUTE1;
	tDAC_MUTE2 DAC_MUTE2;
	tCM_SEL_PAD_STRGTH CM_SEL_PAD_STRGTH;
	tDAC_POWER1 DAC_POWER1;
	tDAC_POWER2 DAC_POWER2;
	tDAC_POWER3 DAC_POWER3;
} sDAC_ADAU1962x;































#endif
