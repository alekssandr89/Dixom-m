/*
 * DSP_internalCalls.h
 *
 *  Created on: 10 ����. 2021 �.
 *      Author: 79825
 */


#ifndef MODULE_DSP_INTERNALCALLS_H_
#define MODULE_DSP_INTERNALCALLS_H_

#include <Constants.h>
#include "stdint.h"

//#define MaskGetEq                   "EQ %hu %hu %hu %hu %.2f %hu %.2f"

typedef struct
{
	uint16_t  startAddess;        //   2 byte   Parameter address in DSP chip
	uint16_t  Freq;               //   2 byte   Frequency
	int16_t   Q;                  //   2 byte 	Q factor
	int16_t   Boost; 	          //   2 byte   Boost
	uint8_t   Type          :4;   // |          Filter type
	uint8_t   Order         :4;   // | 2 byte   Filter order
	uint8_t   OnOff         :2;   // |          Filter on or off
	uint8_t   reserved1     :6;   // |          Reserved
	uint16_t  reserved2;          //   2 byte   Reserved
}sFiltrParam; //TOTAL SIZE 3 word

typedef struct{
	uint16_t addrInDsp;
	uint16_t DataFreq;
	uint16_t DataGain;
	uint16_t reserved1;
} sPhatStereo; //TOTAL SIZE 2 word

WorkRes DSP_Chip_Init(uint8_t ChangePreset);
WorkRes DSP_Set_Preset(uint8_t preset);
void DSP_Save_Preset(uint8_t preset);
WorkRes DSP_Default_Preset();

;
WorkRes DSP_Struct_Default();
WorkRes DSP_Gain_Default();
WorkRes DSP_Volume_Default();
WorkRes DSP_Settings_Default();
WorkRes DSP_Group_Ch_Default();
WorkRes DSP_Grouping_Default();
WorkRes DSP_Invert_Default();
WorkRes DSP_Delay_Default();
WorkRes DSP_DelayGroup_Default();
WorkRes DSP_Volume_Chip_Init();
WorkRes DSP_Loudness_Default();

void DSP_StructsSoundDefault(void) ;

uint16_t DSP_getGroupSettings(uint16_t filtr);
uint16_t DSP_getSettingsGroup(uint16_t filtr);


void DSP_SetEqGain(int8_t point, float data);
void DSP_GroupSetGain( uint16_t filtr, float dataGain, short UsbSend);

void DSP_SETTINGS_SHOW_PARAMETERS(uint8_t AllOrModified);

void DSP_SoursRouting(short direction);
void DSP_SoursSelect(short sours, short oldSource);
void DSP_SourceVolSet( short point, short data);
void DSP_SendDataVol(short point, short DataVol);
short DSP_GetVolume ( short point);
void DSP_SetVolume ( short point, short data);
void DSP_Get_All_Volume(uint8_t AllOrModified);
void DSP_SetMasterVolume (short data );
void DSP_NoiseSwitch (uint8_t data);
void DSP_SetSinGen (uint8_t state, uint16_t freq, uint16_t phase);
void DSP_DirectVolumControl(short VolNum, uint8_t VolumLvl);
void DSP_VolumUP(short VolNum);
void DSP_VolumDown(short VolNum);
void DSP_VolumeControlSelect(uint8_t direction);
void DSP_SetVolControl(uint8_t data);
void DSP_GetVolControl(uint8_t AllOrModified);
void DSP_StartVolControlSettings();
void DSP_Mute(void);
void DSP_ATT(void);
void DSP_ATTRear(short status);
void DSP_GroupSetEq( short filtr, short point, uint8_t onOff, short type,  float boost, short freq, float Q, uint8_t UsbSend);
void DSP_GroupUpdate( short filtr, short point, uint8_t onOff, short type,  float boost, short freq, float Q, uint8_t UsbSend);
void DSP_SetEQ( uint8_t eq_channel, uint8_t band, uint8_t onOff, uint8_t type,  uint16_t boost, uint16_t freq, uint16_t Q);
void DSP_UpdatePointEQ(uint8_t filtr, uint8_t point, uint8_t onOff, uint8_t type,  uint16_t boost, uint16_t freq, uint16_t Q, uint8_t order);
void DSP_UpdatePointSourceEQ(uint8_t filtr, uint8_t point, uint8_t onOff, uint8_t type,  uint16_t boost, uint16_t freq, uint16_t Q, uint8_t order);
void DSP_SetSourceEQ( uint8_t eq_channel, uint8_t band, uint8_t onOff, uint8_t type,  uint16_t boost, uint16_t freq, uint16_t Q);
uint8_t DSP_GetBypassStatus(uint8_t Channel);
void DSP_SetBypassEq(short filtr, short status);
void DSP_GroupSetBypassEq( short filtr, short status, short UsbSend);
sFiltrParam  DSP_GetEQ( short filtr, short point );
uint8_t DefaultState_EQ(uint8_t channel, uint8_t band);
uint8_t DefaultState_Source_EQ(uint8_t channel, uint8_t band);
void  DSP_EqGetDataOut ( uint8_t eq_channel, uint8_t band );
sFiltrParam  DSP_GetCrossover ( short filtr, short point );
void DSP_SetCrossover(uint8_t filtr, uint8_t point, uint8_t onOff, uint8_t type,  uint16_t freq, uint8_t order, uint16_t Q);
void DSP_GroupSetCross( uint8_t channel, uint8_t band, uint8_t onOff, uint8_t type,  uint16_t freq, uint8_t order, uint16_t Q, uint8_t UsbSend);
void DSP_UpdatePoint_Cross(uint8_t channel, uint8_t band, uint8_t onOff, uint8_t type,  uint16_t boost, uint16_t freq, uint16_t Q, uint8_t order);
void DSP_setCrossoverOrderedStructAndCalcRes(sFiltrParam sourcePoint, uint8_t *result);
void DSP_SetParamLoudness(int8_t point, short FreqLP, short LevelLP, short FreqHP, short LevelHP);
void DSP_SetParamLimiter(int8_t point, short Threshold, short RmsTC, short Decay);
float DSP_GetSoundDelay ( short point);
void  DSP_SetSoundDelay ( uint8_t point, uint16_t data );
void  DSP_SetWideStereo ( uint8_t point, uint8_t data );
uint8_t DSP_GetWideStereo ( uint8_t point);
uint8_t DefaultState_WideStereo(uint8_t point);
uint8_t DefaultState_Volume(uint8_t point);
short  DSP_GetInvert ( short point);
void  DSP_SetInvert ( short point, short data );
void DSP_SetPhatStereo  ( short point, short gain, short freq );
uint8_t DefaultState_PhatStereo(uint8_t point);
sPhatStereo DSP_GetPhatStereo  ( short point);
void DSP_SetSupperBass (uint8_t point,  uint8_t state, uint8_t Level,  uint16_t freq, uint8_t Intensiv);
uint8_t DSP_GetSupperBass ( uint8_t point );
uint8_t DefaultState_SupperBass(uint8_t point);
void DSP_Default_Eq(uint8_t eq_channel, uint8_t dataReturn);
void DSP_Default_Source_Eq(uint8_t eq_channel, uint8_t dataReturn);
void DSP_Default_Crossover(uint8_t cross_channel);

uint8_t DefaultState_Cross(uint8_t channel, uint8_t band);
uint8_t DefaultState_Loudness(uint8_t point);
void Dsp_GetDelayGroup(uint8_t AllOrModified);
void DSP_SetLoudness(uint8_t source, uint8_t Kuda, int16_t data);
void DSP_Action_ControlSource(uint16_t action, uint16_t data2, uint16_t data3, uint16_t data4, uint16_t channel, uint16_t pressed);
void DSP_Action_ControlVolum(uint8_t direction, uint16_t NumVol1, uint16_t NumVol2, uint16_t NumVol3, uint16_t channel, uint16_t pressed);
void DSP_Backup_Data(uint8_t ReadWrite);
uint8_t *DSP_GetGroupName(uint8_t num);
void DSP_PresetRouting(uint8_t action, uint8_t data, uint8_t preset);









#endif /* MODULE_DSP_DSP_INTERNALCALLS_H_ */
