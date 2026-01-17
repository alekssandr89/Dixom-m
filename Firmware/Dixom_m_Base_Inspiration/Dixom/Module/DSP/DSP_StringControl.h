/*
 * DSP_ExternalCalls.h
 *
 *  Created on: 10 ����. 2021 �.
 *      Author: 79825
 */

#ifndef MODULE_DSP_DSP_STRINGCONTROL_H_
#define MODULE_DSP_DSP_STRINGCONTROL_H_

#include "stdint.h"


void SOUND_STRING_CONTROL(uint8_t ControlByte, uint8_t Function, uint8_t *Received_String);

void DSP_Source(uint8_t ControlByte, uint8_t *Received_String);
void DSP_Source_Get(uint8_t GetType);
void DSP_SwitchControlSource(uint8_t ControlByte, uint8_t *Received_String);
void DSP_SwitchControlSource_Get(uint8_t GetType);

void DSP_Volume(uint8_t ControlByte, uint8_t *Recive_USB);
void DSP_Volume_Get(uint8_t GetType);
void DSP_Volume_Response(uint8_t point, uint8_t intreface);

void DSP_GET_INITAL_SOURCE_EQ(uint8_t AllOrModified);

void DSP_CHANNEL_NAME(uint8_t *Received_String);
void DSP_PRESET_NAME(uint8_t *Received_String);
void DSP_DELAY_GROUP_NAME(uint8_t *Received_String);
void DSP_DELAY_GROUP(uint8_t ControlByte, uint8_t *Received_String);

void DSP_SwitchControlPreset(uint8_t ControlByte, uint8_t *Received_String);
void DSP_SETTINGS(uint8_t ControlByte, uint8_t *Received_String);
void DSP_Settings(uint8_t ControlByte, uint8_t param, uint8_t data);


void DSP_Equalizer(uint8_t ControlByte, uint8_t *Recive_USB );
void DSP_Equalizer_Get(uint8_t channel, uint8_t GetType);
void DSP_SourceEqualizer(uint8_t ControlByte, uint8_t *Recive_USB );
void DSP_Source_Equalizer_Get(uint8_t channel, uint8_t GetType);
void DSP_Crossover(uint8_t ControlByte, uint8_t *Recive_USB);

void DSP_BypassEqualizer(uint8_t ControlByte, uint8_t *Recive_USB);
void bypassEqualizer(uint8_t ControlByte, uint16_t channel, uint16_t status, uint16_t UsbSend);
void DSP_ChannelGroup(uint8_t ControlByte, uint8_t *Received_String);

void DSP_CHANNEL_GROUPING(uint8_t ControlByte, uint8_t *Received_String);
void DSP_GET_ALL_CHANNEL_GROUPING(uint8_t AllOrModified);



void DSP_GET_SOURCE_ROUTING(uint8_t AllOrModified);
void DSP_EQ_GAIN(uint8_t ControlByte, uint8_t *Recive_USB);

void DSP_GET_INITAL_GAIN(uint8_t AllOrModified);


void DSP_INVERT_SHOW_PARAMETERS(uint8_t AllOrModified);
void DSP_INVERT(uint8_t ControlByte, uint8_t *Recive_USB);
void DSP_GET_INVERT(uint8_t *Recive_USB);

void DSP_Delay(uint8_t ControlByte, uint8_t *Recive_USB);
void DSP_GET_DELAY(uint8_t AllOrModified);


void DSP_SET_NOISE(uint8_t *Recive_USB);
void DSP_GET_NOISE(uint8_t AllOrModified);
void DSP_SET_SINGEN(uint8_t *Recive_USB);
void DSP_GET_SINGEN(uint8_t AllOrModified);


void DSP_GET_ALL_EQ(uint8_t *Recive_USB);
void DSP_GET_INITAL_EQ(uint8_t AllOrModified);



//void DSP_GET_GROUP_CH(uint8_t *Recive_USB);
//void DSP_GET_ALL_GROUP_CHANNEL(uint8_t AllOrModified);
void DSP_GROUP_NAME(uint8_t ControlByte, uint8_t *Received_String);
void DSP_GET_GROUP_NAME(uint8_t *Recive_USB);
void DSP_GET_ALL_GROUP_NAME(uint8_t AllOrModified);

//void DSP_GroupSetStatusEq( short filtr, short status, short UsbSend);

void DSP_EQ_BYPASS_SHOW_PARAMETERS(uint8_t AllOrModified);
void DSP_SET_COPY_EQ(uint8_t ControlByte, uint8_t *Received_String);
void DSP_SET_COPY_CROSS(uint8_t ControlByte, uint8_t *Received_String);
void DSP_SOURCE_ROUTING(uint8_t *Recive_USB);



void DSP_SET_PHAT_STEREO(uint8_t *Recive_USB);
void DSP_GET_PHAT_STEREO(uint8_t *Recive_USB);
void DSP_GET_ALL_PHAT_STEREO(uint8_t AllOrModified);

void DSP_SET_WIDESTEREO(uint8_t *Recive_USB);
void DSP_GET_WIDESTEREO(uint8_t *Recive_USB);
void DSP_GET_ALL_WIDESTEREO(uint8_t AllOrModified);

void DSP_SET_SUPPERBASS(uint8_t *Recive_USB);
void DSP_GET_SUPPERBASS(uint8_t *Recive_USB);
void DSP_GET_ALL_SUPPERBASS(uint8_t AllOrModified);

void DSP_SET_EQ_DEFAULT(uint8_t *Recive_USB);
void DSP_SET_SOURCE_EQ_DEFAULT(uint8_t *Recive_USB);
void DSP_SET_CROSSOVER_DEFAULT(uint8_t *Recive_USB);

void DSP_SET_CROSS(uint8_t *Recive_USB);
void DSP_GET_CROSS(uint8_t *Recive_USB);
void DSP_GET_ALL_CROSS(uint8_t AllOrModified);
void DSP_Crossover_Get(uint8_t channel, uint8_t AllOrModified);

void DSP_SET_LOUDNESS(uint8_t ControlByte, uint8_t *Recive_USB);

void DSP_GET_ALL_LOUDNESS_F(uint8_t AllOrModified);
void DSP_GET_LOUDNESS(uint8_t source);
void DSP_SET_VOLUME_CONTROL(uint8_t *Recive_USB);



void DSP_SavePreset(uint8_t ControlByte, uint8_t *Recive_USB);
void DSP_Preset(uint8_t ControlByte, uint8_t *Recive_USB);






#endif /* MODULE_DSP_DSP_STRINGCONTROL_H_ */
