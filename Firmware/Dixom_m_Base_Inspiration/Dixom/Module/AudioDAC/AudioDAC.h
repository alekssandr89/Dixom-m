/*
 * AudioDAC.h
 *
 *  Created on: 17 ����. 2021 �.
 *      Author: 79825
 */

#ifndef MODULE_AUDIODAC_AUDIODAC_H_
#define MODULE_AUDIODAC_AUDIODAC_H_

#include "DAC_ADAU196x.h"
#include "DAC_AK4440.h"
#include "DAC_AK4458VN.h"
#include "DAC_TAS3251.h"
#include "DAC_PCM1690.h"
#include "CrystalSix140D.h"

void       DAC_Mute(uint8_t state);
void       DAC_Volume(uint8_t *pData);
void       DAC_Restart();
void       Automatic_Detection_Audio_DAC_And_Initialization();

typedef struct
{
	sDAC_AK4458VN  AK4458VN;
	sDAC_ADAU1962x ADAU1962x;
	sDAC_AK4440    AK4440;
	sDAC_TAS3251   TAS3251;
	sDAC_PCM1690   PCM1690;
	sAmpTAS3251x3  TAS3251x3;
	uint8_t        DAC_Type;      // DAC chip model
	uint8_t        Num_Channel;   // Number of audio channels on a chip
	uint8_t        Num_Chips;     // Number of chips on the DAC board
} sAudioDAC;





#endif /* MODULE_AUDIODAC_AUDIODAC_H_ */
