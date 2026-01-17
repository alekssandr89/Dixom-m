/*
 * AudioDAC.c
 *
 *  Created on: 17 ����. 2021 �.
 *      Author: 79825
 */
#include <Description.h>
#include "Exchange.h"
#include "AudioDAC.h"
#include <stdint.h>
#include <Structures.h>
#include <Functions.h>

extern sDixom Dixom;

#define dAudioDAC Dixom.Module.AudioDAC


void DAC_Mute(uint8_t state){

	switch(dAudioDAC.DAC_Type){
    case DAC_ADAU1962        : Mute_DAC_ADAU1962(state);          break;
    case DAC_AK4458VN        : Mute_DAC_AK4458(state);            break;
    case DAC_AK4440          : Mute_DAC_AK4440(state);            break;
    case DAC_TAS3251         : Mute_DAC_TAS3251(state);           break;
    case DAC_PCM1690         : Mute_DAC_PC1690(state);            break;
    case AMPLIFIER_3xTAS3251 : Mute_AMPLIFIER_3xTAS3251(state);   break;
	}
}

void DAC_Volume(uint8_t *pData){

	switch(dAudioDAC.DAC_Type){
    case DAC_ADAU1962    : Volume_DAC_ADAU1962(pData);    break;
    case DAC_AK4458VN    : Volume_DAC_AK4458VN(pData);    break;
    case DAC_AK4440      : /* NOT SUPPORT*/               break;
    case DAC_TAS3251     : Volume_DAC_TAS3251(pData);     break;
    case DAC_PCM1690     : Volume_DAC_PCM1690(pData);     break;
	}
}

void DAC_Restart(){
	Switch_Dac_Reset(OFF);
	Delay(500, FreeRTOS);
	Switch_Dac_Reset(ON);
	Delay(1000, FreeRTOS);
	Automatic_Detection_Audio_DAC_And_Initialization();
	DAC_Mute(OFF);
	TransmitDataOutputs((uint8_t *)General_DacReboot_Ok, AutoLenght,  false,  true, allAvailable);

}
#define Port1 0x02
#define Port3 0x08
void Automatic_Detection_Audio_DAC_And_Initialization(){

	dAudioDAC.Num_Chips = 0;

	if(DevState_I2C2(DEVICE_ADDR_DAC_ADAU1962, 1, 100)     == WR_OK) {

		dAudioDAC.Num_Chips++;
		dAudioDAC.Num_Channel = 12;
		dAudioDAC.DAC_Type = DAC_ADAU1962;

		Init_DAC_ADAU196x();

	}else if(DevState_I2C2(DEVICE_ADDR_DAC_AK4458VN<<1, 1, 100)  == WR_OK) {

		dAudioDAC.Num_Chips++;
		dAudioDAC.Num_Channel = 8;
		dAudioDAC.DAC_Type = DAC_AK4458VN;

		Init_DAC_AK4458VN();

	}else if(DevState_I2C2(DEVICE_ADDR_DAC_TAS3251<<1, 1, 100)  == WR_OK) {

		dAudioDAC.Num_Chips++;
		dAudioDAC.Num_Channel = 8;
		dAudioDAC.DAC_Type = DAC_TAS3251;

		Init_DAC_TAS3251();


	}else if(DevState_I2C2(DEVICE_ADDR_AMPLIFIER_3xTAS3251<<1, 1, 100)  == WR_OK) {

		dAudioDAC.Num_Chips++;
		dAudioDAC.Num_Channel = 8;
		dAudioDAC.DAC_Type = AMPLIFIER_3xTAS3251;

		Init_AMPLIFIER_3xTAS3251();

	}else if(DevState_I2C2(DEVICE_ADDR_DAC_AK4440<<1, 1, 100)    == WR_OK) {

		dAudioDAC.Num_Chips++;
		dAudioDAC.Num_Channel = 8;
		dAudioDAC.DAC_Type = DAC_AK4440;

		if(Dixom.Module.DSP.Settings[VolumeControlMode].Data == VolumControlOnDAC){
			/*does not support DAC volume control*/
			Dixom.Module.DSP.Settings[VolumeControlMode].Data = VolumControlOnLoudness;
		}

		Init_DAC_AK4440();

	}else if(DevState_I2C2(DEVICE_ADDR_DAC_PCM1690<<1, 1, 100)    == WR_OK) {

		dAudioDAC.Num_Chips++;
		dAudioDAC.Num_Channel = 8;
		dAudioDAC.DAC_Type = DAC_PCM1690;

		Init_DAC_PC1690();

	}/*else if(DevState_I2C2(DEVICE_ADDR_AMPLIFIER_3xTAS3251<<1, 1, 100)    == WR_OK) {

		dAudioDAC.Num_Chips++;
		dAudioDAC.Num_Channel = 8;
		dAudioDAC.DAC_Type = AMPLIFIER_3xTAS3251;

		Init_AMPLIFIER_3xTAS3251();

	//Transmit_I2C2(DEVICE_ADDR_AMPLIFIER_3xTAS3251<<1, NOT_USED, NOT_USED, (uint8_t*)0x08, 1, 200, ExchangeMainSteam);
//	Init_DAC_TAS3251();


	}*/
	else{
		dAudioDAC.Num_Chips = 0;
		dAudioDAC.Num_Channel = 0;
		dAudioDAC.DAC_Type = DAC_NotConnected;
	}
}





