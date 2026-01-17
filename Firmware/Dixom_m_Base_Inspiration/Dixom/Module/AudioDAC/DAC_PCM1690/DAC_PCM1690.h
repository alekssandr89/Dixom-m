/*
 * DAC_PCM1690.h
 *
 *  Created on: 17 мар. 2022 г.
 *      Author: 79825
 */

#ifndef MODULE_AUDIODAC_PCM1690_DAC_PCM1690_H_
#define MODULE_AUDIODAC_PCM1690_DAC_PCM1690_H_
#include "stdint.h"

void Init_DAC_PC1690();
void Mute_DAC_PC1690(uint8_t state);
void Volume_DAC_PCM1690(uint8_t *pData);

typedef struct
{
	uint8_t  ADDR;                           // Register address 0x40 //64
	uint8_t  SRDA                      :2;   // Sampling mode selection
	uint8_t  AMC_SCKI                  :1;   // Analog mute function control
	uint8_t  AMC_ASINCHRONUS_DETECT    :1;   // Analog mute function control
	uint8_t  AMC_ZERO_DETECT           :1;   // Analog mute function control
	uint8_t  AMC_ZERO_DAC_DISABLE_CMD  :1;   // Analog mute function control
	uint8_t  SRST                      :1;   // System reset
	uint8_t  MRST                      :1;   // Mode control register reset
} sPCM1690_Reg64 ;

typedef struct
{
	uint8_t  ADDR;                           // Register address 0x41 //65
	uint8_t  FMTDA                     :1;   // Audio interface format selection
	uint8_t  Reserved                  :3;   // Reserved
	uint8_t  PSMDA                     :4;   // Power-save mode selection
} sPCM1690_Reg65 ;

typedef struct
{
	uint8_t  ADDR;                           // Register address 0x42 //66
	uint8_t  FLT_CH1_2                    :1;   // Digital filter roll-off control
	uint8_t  FLT_CH3_4                    :1;   // Digital filter roll-off control
	uint8_t  FLT_CH5_6                    :1;   // Digital filter roll-off control
	uint8_t  FLT_CH7_8                    :1;   // Digital filter roll-off control
	uint8_t  OPEDA_CH1_2                  :1;   // Operation control
	uint8_t  OPEDA_CH3_4                  :1;   // Operation control
	uint8_t  OPEDA_CH5_6                  :1;   // Operation control
	uint8_t  OPEDA_CH7_8                  :1;   // Operation control
} sPCM1690_Reg66;

typedef struct
{
	uint8_t  ADDR;                           // Register address 0x43 //67
	uint8_t  REVDA1                    :1;   // Output phase selection CH1
	uint8_t  REVDA2                    :1;   // Output phase selection CH2
	uint8_t  REVDA3                    :1;   // Output phase selection CH3
	uint8_t  REVDA4                    :1;   // Output phase selection CH4
	uint8_t  REVDA5                    :1;   // Output phase selection CH5
	uint8_t  REVDA6                    :1;   // Output phase selection CH6
	uint8_t  REVDA7                    :1;   // Output phase selection CH7
	uint8_t  REVDA8                    :1;   // Output phase selection CH8
} sPCM1690_Reg67;

typedef struct
{
	uint8_t  ADDR;                           // Register address 0x44 //68
	uint8_t  MUTDA;                          // Soft Mute control 0 mute off all channel, 255 mute on all channel
} sPCM1690_Reg68;

typedef struct
{
	uint8_t  ADDR;                           // Register address 0x45 //69
	uint8_t  ZERO1                    :1;   // Zero flag (read-only) CH1
	uint8_t  ZERO2                    :1;   // Zero flag (read-only) CH2
	uint8_t  ZERO3                    :1;   // Zero flag (read-only) CH3
	uint8_t  ZERO4                    :1;   // Zero flag (read-only) CH4
	uint8_t  ZERO5                    :1;   // Zero flag (read-only) CH5
	uint8_t  ZERO6                    :1;   // Zero flag (read-only) CH6
	uint8_t  ZERO7                    :1;   // Zero flag (read-only) CH7
	uint8_t  ZERO8                    :1;   // Zero flag (read-only) CH8
} sPCM1690_Reg69;

typedef struct
{
	uint8_t  ADDR;                           // Register address 0x46 //70
	uint8_t  ZREV                    :1;   // Zero flag polarity selection
	uint8_t  AZRO                    :2;   // Zero flag channel combination selection
	uint8_t  Reserved0               :1;   // Reserved
	uint8_t  DEMP                    :2;   // Digital de-emphasis function/sampling rate contro
	uint8_t  Reserved1               :1;   // Reserved
	uint8_t  DAMS                    :1;   // Digital attenuation mode
} sPCM1690_Reg70;

typedef struct
{
	sPCM1690_Reg64 Reg64;
	sPCM1690_Reg65 Reg65;
	sPCM1690_Reg66 Reg66;
	sPCM1690_Reg67 Reg67;
	sPCM1690_Reg68 Reg68;
	sPCM1690_Reg69 Reg69;
	sPCM1690_Reg70 Reg70;
} sDAC_PCM1690;

#endif /* MODULE_AUDIODAC_PCM1690_DAC_PCM1690_H_ */
