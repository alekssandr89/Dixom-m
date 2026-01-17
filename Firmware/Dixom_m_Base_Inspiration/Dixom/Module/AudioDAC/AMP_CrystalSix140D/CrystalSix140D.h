/*
 * Tas3251x3_Amplifier.h
 *
 *  Created on: 26 ����. 2022 �.
 *      Author: 79825
 */

#ifndef MODULE_AUDIODAC_TAS_3251X3_AMPLIFIER_TAS3251X3_H_
#define MODULE_AUDIODAC_TAS_3251X3_AMPLIFIER_TAS3251X3_H_

#include "DIX_i2c.h"

void Mute_AMPLIFIER_3xTAS3251(uint8_t state);
void Init_AMPLIFIER_3xTAS3251();

/*
#define DEVICE_ADDR_TAS3251_CH1_2        0x4A   //74 ���� 1
#define DEVICE_ADDR_TAS3251_CH3_4        0x4B   //75 ���� 3
#define DEVICE_ADDR_TAS3251_CH5_6        0x4C   //74 ���� 3
#define DEVICE_ADDR_GPIO_IN              0x7E   //52 ���� 3
#define DEVICE_ADDR_GPIO_OUT             0x28   //21 ���� 3
#define DEVICE_ADDR_TEMOMETER_LM75A      0x48   //11 ���� 3
#define DEVICE_ADDR_DISPLAY              0x3C   //11 ���� 5*/

#define CrystalSix140D_TCA9548  0x70    //112

#define Port1_CH1_2_TAS3251     0x4A
#define Port3_CH3_4_TAS3251     0x4B
#define Port3_CH5_6_TAS3251     0x4A
#define Port3_GPIO_IN_PCF8574   0x27
#define Port3_GPIO_OUT_PCF8574  0x23
#define Port3_THERMOMETER_LM75A 0x48
#define Port5_OLED_DISPLAY      0x3C

#define I2C_PORT_1              0x02
#define I2C_PORT_3              0x08
#define I2C_PORT_5              0x20


typedef struct
{
	uint8_t  ADDR;                   // Register address 0x00
	uint8_t  RSTN             :1;    // Internal Timing Reset
	uint8_t  DIF0             :1;    // Audio Data Interface Modes
	uint8_t  DIF1             :1;    // Audio Data Interface Modes
	uint8_t  DIF2             :1;    // Audio Data Interface Modes
	uint8_t  NotUsed          :3;    // NotUsed
	uint8_t  ACKS             :1;    // Master Clock Frequency Auto Setting Mode Enable (PCM only)
} tI2CHUB;

typedef struct
{
	uint8_t  Amp1_Fault        :1;    // Internal Timing Reset
	uint8_t  Amp1_Cliping      :1;    // Audio Data Interface Modes
	uint8_t  Amp2_Fault        :1;    // Audio Data Interface Modes
	uint8_t  Amp2_Cliping      :1;    // Audio Data Interface Modes
	uint8_t  Amp3_Cliping      :1;    // NotUsed
	uint8_t  AmpBtnUp          :1;    // Master Clock Frequency Auto Setting Mode Enable (PCM only)
	uint8_t  AmpBtnDown        :1;    // NotUsed
	uint8_t  AmpBtnMenu        :1;    // Master Clock Frequency Auto Setting Mode Enable (PCM only)
} tGpioIN;

typedef struct
{
	uint8_t  Amp1_Mute         :1;    // Internal Timing Reset
	uint8_t  Amp1_Reset        :1;    // Audio Data Interface Modes
	uint8_t  Amp2_Mute         :1;    // Audio Data Interface Modes
	uint8_t  Amp2_Reset        :1;    // Audio Data Interface Modes
	uint8_t  Amp3_Mute         :1;    // Audio Data Interface Modes
	uint8_t  Amp3_Reset        :1;    // Audio Data Interface Modes
	uint8_t  Reserved          :2;    // NotUsed
} tGpioOut;

typedef struct
{
	uint8_t  Shutdow           :1;
	uint8_t  Comparato         :1;
	uint8_t  OSPolarit         :1;
	uint8_t  FaultQueue1       :1;
	uint8_t  FaultQueue2       :1;
	uint8_t  Reserved          :3;
} tLm75Config;

typedef struct
{
	float       Temperature;
	int8_t      TempHysteresis;
	int8_t      TempOS;
	uint8_t     FanState;
	tLm75Config Config;
} sTempControl;

typedef struct
{
	uint8_t      CurrentPort; //Current I2C Port
	uint8_t      DsplayStatus; //Current I2C Port
	tGpioIN      GpioIN;
	tGpioOut     GpioOut;
	sTempControl TempControl;
} sAmpTAS3251x3;

#endif /* MODULE_AUDIODAC_TAS_3251X3_AMPLIFIER_TAS3251X3_H_ */
