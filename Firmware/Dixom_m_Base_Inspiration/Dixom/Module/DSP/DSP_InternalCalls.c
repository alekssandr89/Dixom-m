#include <Description.h>
#include "DSP_Exchange.h"
#include "Exchange.h"
#include "display_Menu.h"
#include "DSP_Calculations.h"
#include "DSP_StringControl.h"
#include "DSP_InternalCalls.h"
#include "DSP_ElementAddres.h"
#include <stdio.h>
#include <String.h>
#include <Structures.h>

extern sDixom Dixom;


#define dDSP Dixom.Module.DSP
extern  sDisplayMenu DisplayMenu;

/*
 * DSP_Chip_Init() - function of loading from the DSP structure into the DSP audio processor AUAU1452.
 * The result of the work will be reflected in the returned value of WorkRes.
 *
 * ChangePreset - When changing a preset, not all settings need to be loaded into the DSP.
 * 1 (true) load preset only,
 * 2 (false) load all
 */
WorkRes DSP_Chip_Init(uint8_t ChangePreset){

	uint8_t result[20];

	for(uint8_t channel = 0; channel<NUM_ELEM_SOURCE; channel++){

		for(uint8_t band = 0; band<NUM_ELEM_EQ_SOURCE_BAND; band++){
			if( dDSP.Preset.Data[Main_Preset].EQ_CH[channel].Bypass == OFF /*&& DefaultState_EQ(channel, band) == NO*/) {
				DSP_SetEQ(channel, band, dDSP.Preset.Data[Main_Preset].EQ_CH[channel].Band[band].OnOff,
							             dDSP.Preset.Data[Main_Preset].EQ_CH[channel].Band[band].Type,
							             dDSP.Preset.Data[Main_Preset].EQ_CH[channel].Band[band].Boost,
						                 dDSP.Preset.Data[Main_Preset].EQ_CH[channel].Band[band].Freq,
							             dDSP.Preset.Data[Main_Preset].EQ_CH[channel].Band[band].Q);
			}
		}
	}

	for(uint8_t channel = 0; channel<NUM_ELEM_EQ_CH; channel++){

		DSP_GainCalculation (dDSP.Preset.Data[Main_Preset].EQ_CH[channel].Gain.DataGain,  (uint8_t *)&result );
	    Transmit_DSP_SafeLoad( (uint8_t *)&result, 4, dDSP.Preset.Data[Main_Preset].EQ_CH[channel].Gain.addrInDsp );

		for(uint8_t band = 0; band<NUM_ELEM_EQ_CH_BAND; band++){
			if( dDSP.Preset.Data[Main_Preset].EQ_CH[channel].Bypass == OFF /*&& DefaultState_EQ(channel, band) == NO*/) {
				DSP_SetEQ(channel, band, dDSP.Preset.Data[Main_Preset].EQ_CH[channel].Band[band].OnOff,
							             dDSP.Preset.Data[Main_Preset].EQ_CH[channel].Band[band].Type,
							             dDSP.Preset.Data[Main_Preset].EQ_CH[channel].Band[band].Boost,
						                 dDSP.Preset.Data[Main_Preset].EQ_CH[channel].Band[band].Freq,
							             dDSP.Preset.Data[Main_Preset].EQ_CH[channel].Band[band].Q);
			}
		}
	}

	for(uint8_t channel = 0; channel<NUM_ELEM_CROSSOVER; channel++){
		for(uint8_t band = 0; band<NUM_ELEM_CROSSOVER_TYPE; band++){
			DSP_SetCrossover( channel, band, dDSP.Preset.Data[Main_Preset].Crossover[channel].Filter[band].OnOff,
						                     dDSP.Preset.Data[Main_Preset].Crossover[channel].Filter[band].Type,
						                     dDSP.Preset.Data[Main_Preset].Crossover[channel].Filter[band].Freq,
					                         dDSP.Preset.Data[Main_Preset].Crossover[channel].Filter[band].Order,
						                     dDSP.Preset.Data[Main_Preset].Crossover[channel].Filter[band].Q);
		}
	}
/*
	for (short point = 0; point<NUM_ELEM_LIMITER; point++){
	}
*/
    for (short point = 0; point<NUM_ELEM_DELAY; point++){
    	DSP_DelayCalculation (dDSP.Preset.Data[Main_Preset].SoundDelay[point].DataDelay,  (uint8_t *)&result );
    	Transmit_DSP_SafeLoad( (uint8_t *)&result, 4, dDSP.Preset.Data[Main_Preset].SoundDelay[point].addrInDsp );
    }


    for (short point = 0; point<NUM_ELEM_INVERT; point++){
    	DSP_setFloatPointInDataArray( (float)dDSP.Preset.Data[Main_Preset].Invert[point].DataInvert, (uint8_t *)&result );
    	Transmit_DSP_SafeLoad( (uint8_t *)&result, 4, dDSP.Preset.Data[Main_Preset].Invert[point].addrInDsp );
    }


    if(ChangePreset == NO){

    	DSP_Volume_Chip_Init();


    	/*for (short point = 0; point<NUM_ELEM_PHATSTEREO; point++) {
    		DSP_SetPhatStereo  ( point, dDSP.PhatStereo[point].DataGain, dDSP.PhatStereo[point].DataFreq );
    	}*/
/*
        for (short point = 0; point<NUM_ELEM_WIDESTEREO; point++){
        	DSP_SetWideStereo  ( point, dDSP.WideStereo[point].Gain);
        }*/
/*
        for (short point = 0; point<NUM_ELEM_SUPPERBASS; point++){
        	DSP_SetSupperBass (point,  dDSP.SupperBass[point].State, dDSP.SupperBass[point].Gain,  dDSP.SupperBass[point].Freq, dDSP.SupperBass[point].Intensity);
        }*/
    }
	return WR_OK;
}

/*
 * DSP_Struct_Default - function for resetting DSP structure to standard parameters.
 * OnlyEditingReset - If 1 (true), only edit signals will be reset and DSP addresses will be set. If 0 (false), all settings will be deleted. *
 * The result of the work will be reflected in the returned value of WorkRes.
 */

WorkRes DSP_Volume_Chip_Init(){

	uint8_t result[20];

	for (short point = 0; point<NUM_ELEM_VOL; point++) {
	    if(point>4){
	        DSP_VolCalculation(dDSP.Volume[point].DataVol, (uint8_t *)&result);
	        Transmit_DSP_SafeLoad( (uint8_t *)&result, 4, dDSP.Volume[point].addrInDsp );
	    }else if(point==0){
	    	DAC_VolCalculation(dDSP.Volume[point].DataVol, (uint8_t *)&result);
	    	//Transmit_DAC(dDSP.Volume[point].addrInDsp,  (uint8_t *)&result,  1, 150, ExchangeMemMainSteam);
	    	DAC_Volume((uint8_t *)&result[0]);
	    }
	}

    for (short point = 0; point<NUM_ELEM_LOUDNESS; point++){
		DSP_LoudnessLevelCalculation(point);
		DSP_LoudnessFreqCalculation(point);
    }

    DSP_SetVolControl(dDSP.Settings[VolumeControlMode].Data);
    return  WR_OK;
}

WorkRes DSP_Volume_Default(){
	for (short point = 0; point<NUM_ELEM_VOL; point++) {
		dDSP.Volume[point].addrInDsp = VOLUM_DSP_ADDR[point];
		dDSP.Volume[point].DataVol = DSP_VOL_DEFAULT[point];
	}
	return DSP_Volume_Chip_Init();
}

WorkRes DSP_Loudness_Default(){
	for (short source = 0; source<NUM_ELEM_LOUDNESS; source++) {
		dDSP.Loudness[source].startAddess = LOUDNESS_DSP_ADDR[source];
		DSP_SetParamLoudness (source, LOUDNESS_DEFAULT[source*4], LOUDNESS_DEFAULT[source*4+1], LOUDNESS_DEFAULT[source*4+2], LOUDNESS_DEFAULT[source*4+3] );
		DSP_SetLoudness(source, 0, LOUDNESS_DEFAULT[source*4]);
		DSP_SetLoudness(source, 1, LOUDNESS_DEFAULT[source*4+1]);
		DSP_SetLoudness(source, 2, LOUDNESS_DEFAULT[source*4+2]);
		DSP_SetLoudness(source, 3, LOUDNESS_DEFAULT[source*4+3]);

	}
	return WR_OK;
}


WorkRes DSP_Settings_Default(){
	for(uint8_t set = 0; set < NUM_ELEM_DSP_SETTINGS; set++){
		dDSP.Settings[set].Data = DEFAULT_DSP_SETTINGS[set];
	}
	return WR_OK;
}

WorkRes DSP_Group_Ch_Default(){
	for(uint8_t channel = 0; channel < NUM_ELEM_EQ_CH; channel++){
		dDSP.Group.Channel[channel] = DEFAULT_CH_GROUP[channel];
	}
	return WR_OK;
}

WorkRes DSP_Grouping_Default(){
	for(uint8_t group = 0; group < NUM_ELEM_GROUP; group++){
		dDSP.Group.State[group] = ON;
	}
	return WR_OK;
}


WorkRes DSP_Invert_Default(){
	for (short point = 0; point<NUM_ELEM_INVERT; point++) {
		dDSP.Preset.Data[Main_Preset].Invert[point].addrInDsp = INVERT_DSP_ADDR[point];
		dDSP.Preset.Data[Main_Preset].Invert[point].DataInvert = DEFAULT_INVERT;
		dDSP.Preset.Data[Main_Preset].Invert[point].reserved1 = NO;
	}
	return WR_OK;
}

WorkRes DSP_Delay_Default(){
	for (short point = 0; point<NUM_ELEM_DELAY; point++) {
		dDSP.Preset.Data[Main_Preset].SoundDelay[point].addrInDsp = DELAY_DSP_ADDR[point];
		dDSP.Preset.Data[Main_Preset].SoundDelay[point].DataDelay = DEFAULT_DELAY;
	}
	return WR_OK;
}

WorkRes DSP_DelayGroup_Default(){
	for (short point = 0; point<NUM_ELEM_DELAY; point++) {
		dDSP.Group.Delay[point] = DEFAULT_DELAY_GROUP[point];
	}
	return WR_OK;
}

WorkRes DSP_DelayGroupName_Default(){
	for (short point = 0; point<NUM_ELEM_GROUP_NAME; point++) {
		for (short simbol = 0; simbol<NAME_MAX_LEN; simbol++){
			dDSP.Lable.Delay[point][simbol] = 0;
		}
		sprintf((char*)dDSP.Lable.Delay[point], DelayGroupName, point+1);
	}
	return WR_OK;
}

WorkRes DSP_GroupName_Default(){
	for (short point = 0; point<NUM_ELEM_GROUP_DELAY; point++) {
		for (short simbol = 0; simbol<NAME_MAX_LEN; simbol++){
			dDSP.Lable.Group[point][simbol] = 0;
		}
	}
	sprintf( (char*)dDSP.Lable.Group[0], GroupName1);
	sprintf( (char*)dDSP.Lable.Group[1], GroupName2);
	sprintf( (char*)dDSP.Lable.Group[2], GroupName3);
	sprintf( (char*)dDSP.Lable.Group[3], GroupName4);
	sprintf( (char*)dDSP.Lable.Group[4], GroupName5);
	sprintf( (char*)dDSP.Lable.Group[5], GroupName6);
	sprintf( (char*)dDSP.Lable.Group[6], GroupName7);
	return WR_OK;
}

WorkRes DSP_ChannelName_Default(){
	for (short point = 0; point<NUM_ELEM_EQ_CH; point++) {
		for (short simbol = 0; simbol<NAME_MAX_LEN; simbol++){
			dDSP.Lable.Channel[point][simbol] = 0;
		}
		sprintf( (char*)dDSP.Lable.Channel[point], ChannelName1, point+1);
	}
	return WR_OK;
}

WorkRes DSP_PresetName_Default(){
	for (short point = 0; point<NUM_PRESET; point++) {
		for (short simbol = 0; simbol<NAME_MAX_LEN; simbol++){
			dDSP.Lable.Channel[point][simbol] = 0;
		}
		sprintf( (char*)dDSP.Lable.Preset[point], PresetName, point+1);
	}
	return WR_OK;
}



WorkRes DSP_Gain_Default(){
	for (short channel = 0; channel<NUM_ELEM_EQ_CH; channel++) {
		dDSP.Preset.Data[Main_Preset].EQ_CH[channel].Gain.DataGain = 0;
		dDSP.Preset.Data[Main_Preset].EQ_CH[channel].Gain.addrInDsp = GAIN_DSP_ADDR[channel];
	}
	return WR_OK;
}


WorkRes DSP_Struct_Default(){

	//----- LABEL NAME----------------------------------------------------------------------------//
	DSP_DelayGroupName_Default();
	DSP_GroupName_Default();
	DSP_ChannelName_Default();
	DSP_PresetName_Default();


	//----- STRUCTURE INFORMATION----------------------------------------------------------------------------//

	DSP_Flash_NeedToSave();
	DSP_EEPROM_NeedToSave();
	dDSP.SAMPLE_RATE = SAMPLE_RATE_48000;
	dDSP.StructInfo.Size = sizeof(dDSP);
	dDSP.StructInfo.UserPresetSize = sizeof(dDSP.Preset);
	dDSP.StructInfo.reserved2 = NO;
	dDSP.Preset.Resreved0 = 0;
	Dixom.Service.Status.Struct[Module_DSP_enum] = DefaultStruct;


	 //----- SINE GENERATOR----------------------------------------------------------------------------------//

	dDSP.SineGenerator.addrFreqInDsp = DSP_SinGenFreqAddr;
	dDSP.SineGenerator.addrSwitchInDsp = DSP_SinGenSwitchAddr;
	dDSP.SineGenerator.Freq = DEF_SINGEN_FREQ;
	dDSP.SineGenerator.State = OFF;


    //----- NOISE GENERATOR-----------------------------------------------------------------------------------//

	dDSP.NoiseGenerator.Gain = 0;
	dDSP.NoiseGenerator.State = OFF;
	dDSP.NoiseGenerator.addrGainInDsp = DSP_NoiseGainAddr;
	dDSP.NoiseGenerator.addrSwitchInDsp = DSP_NoiseSwitchAddr;


	//----- DSP SETTINGS -------------------------------------------------------------------------------------//
	DSP_Settings_Default();
	DSP_Gain_Default();

	//----- DSP GROUP CHANNEL -------------------------------------------------------------------------------------//
	DSP_Group_Ch_Default();
	DSP_Grouping_Default();

	//----- SOURCE EQUALIZER---------------------------------------------------------------------------------//

	for (short Filter = 0; Filter<NUM_ELEM_SOURCE; Filter++) {
		for (short Band = 0; Band<NUM_ELEM_EQ_SOURCE_BAND; Band++){
			dDSP.Preset.Data[Main_Preset].EQ_Source[Filter].Band[Band].startAddess = DSP_GENERAL_EQ_ADDR[(Filter*NUM_ELEM_EQ_SOURCE_BAND)+Band];
			DSP_UpdatePointSourceEQ(Filter, Band, FILTER_EQ_ONOFF_DEF, FILTER_TYPE_DEF,  FILTER_BOOST_DEF, EQ_SOURCE_DEFAULT[Band], FILTER_EQ_SOURCE_Q_DEF, NO);
		}
	}

	//----- CHANNEL EQUALIZER----------------------------------------------------------------------------------//

	for (short Filter = 0; Filter<NUM_ELEM_EQ_CH; Filter++) {
		dDSP.Preset.Data[Main_Preset].EQ_CH[Filter].Bypass = OFF;
		for (short Band = 0; Band<NUM_ELEM_EQ_CH_BAND; Band++){
			dDSP.Preset.Data[Main_Preset].EQ_CH[Filter].Band[Band].startAddess = DSP_CH_EQ_ADDR[(Filter*NUM_ELEM_EQ_CH_BAND)+Band];
			DSP_UpdatePointEQ(Filter, Band, FILTER_EQ_ONOFF_DEF, FILTER_TYPE_DEF,  FILTER_BOOST_DEF, EQ_CH_DEFAULT[Band], FILTER_EQ_CH_Q_DEF, NO);
		}
	}

	//----- VOLUME CONTROLLER----------------------------------------------------------------------------------//
	DSP_Volume_Default();

	//----- LOUDNESS-------------------------------------------------------------------------------------------//

	DSP_Loudness_Default();

	//----- AUDIO LIMITER----------------------------------------------------------------------------------------//

	for (short point = 0; point<NUM_ELEM_LIMITER; point++) {
		dDSP.Limiter[point].startAddess = LIMITER_DSP_ADDR[point];
		DSP_SetParamLimiter (point, LIMITER_DEFAULT[point*3], LIMITER_DEFAULT[point*3+1], LIMITER_DEFAULT[point*3+2]);
	}

	//----- SOUND DELAY-----------------------------------------------------------------------------------------//

	DSP_Delay_Default();
	DSP_DelayGroup_Default();

	//----- PHASE INVERT----------------------------------------------------------------------------------------//
	DSP_Invert_Default();
	//----- SOUND EFECT PHAT STEREO------------------------------------------------------------------------------//

	for (short point = 0; point<NUM_ELEM_PHATSTEREO; point++) {
		dDSP.PhatStereo[point].addrInDsp = PHATSTEREO_DSP_ADDR[point];
		dDSP.PhatStereo[point].DataGain = PHATSTEREO_DEFAULT[point*2];
		dDSP.PhatStereo[point].DataFreq = PHATSTEREO_DEFAULT[point*2+1];
		dDSP.PhatStereo[point].reserved1 = NO;
	}

	//----- SOUND EFECT WIDE STEREO------------------------------------------------------------------------------//

	for (short point = 0; point<NUM_ELEM_WIDESTEREO; point++) {
		dDSP.WideStereo[point].addrInDsp = WIDESTEREO_DSP_ADDR[point];
		dDSP.WideStereo[point].Gain = WIDESTEREO_DEFAULT[point];
	}

	//----- SOUND EFECT SUPPER BASS------------------------------------------------------------------------------//

	for (short point = 0; point<NUM_ELEM_SUPPERBASS; point++) {
		dDSP.SupperBass[point].dspAddessFreq = SUPPERBASS_DSP_ADDR[point*3];
		dDSP.SupperBass[point].dspAddessGain = SUPPERBASS_DSP_ADDR[point*3+1];
		dDSP.SupperBass[point].dspAddessInte = SUPPERBASS_DSP_ADDR[point*3+2];
		dDSP.SupperBass[point].State = SUPPERBASS_DEFAULT[point*4];
		dDSP.SupperBass[point].Gain = SUPPERBASS_DEFAULT[point*4+1];
		dDSP.SupperBass[point].Freq = SUPPERBASS_DEFAULT[point*4+2];
		dDSP.SupperBass[point].Intensity = SUPPERBASS_DEFAULT[point*4+3];
	}

	//----- CHANNEL CROSSOVER------------------------------------------------------------------------------------//

	for (uint8_t channel = 0; channel<NUM_ELEM_CROSSOVER; channel++) {
		for (uint8_t band = 0; band<NUM_ELEM_CROSSOVER_TYPE; band++){
			dDSP.Preset.Data[Main_Preset].Crossover[channel].Filter[band].startAddess = CROSSOVER_DSP_ADDR[channel*2+band];
			DSP_UpdatePoint_Cross(channel, band, FILTER_CROSS_ONOFF_DEF, DEFAULT_CROSSOVER_TYPE[band], NO, DEFAULT_CROSSOVER_FREQ[band], FILTER_CROSS_Q_DEF, FILTER_ORDER_DEF);
		}
	}

	//----- CHANEL GROUP NAME-------------------------------------------------------------------------------------//

	char data[NAME_MAX_LEN];
	for (uint8_t point = 0; point<NUM_ELEM_GROUP_NAME; point++) {

		for(uint8_t i = 0; i<NAME_MAX_LEN; i++){
			data[i] = 0;
		}
		sprintf( data,"%hhu Group", point+1);

		for(uint8_t i = 0; i<NAME_MAX_LEN; i++){
			dDSP.Lable.Group[point][i] = data[i];
		}
	}

	dDSP.Preset.Data[User_preset1] = dDSP.Preset.Data[Main_Preset];
	dDSP.Preset.Data[User_preset2] = dDSP.Preset.Data[Main_Preset];
	dDSP.Preset.Data[User_preset3] = dDSP.Preset.Data[Main_Preset];
	return  WR_OK;
}


void DSP_SETTINGS_SHOW_PARAMETERS(uint8_t AllOrModified){

	for(uint8_t Elem = 0; Elem < NUM_ELEM_DSP_SETTINGS; Elem++){
		if(dDSP.Settings[Elem].Data != DEFAULT_DSP_SETTINGS[Elem] || AllOrModified == All){
			sprintf( (char *) Dixom.Buff, MaskDspGetSettings, Elem,  dDSP.Settings[Elem].Data);
 			TransmitDataOutputs(Dixom.Buff, AutoLenght,  false,  true, allAvailable);

		}
	}
}


void DSP_SetEqGain(int8_t point, float data){

	uint8_t result [4];
	DSP_GainCalculation(data, (uint8_t *)&result);

	dDSP.Preset.Data[Main_Preset].EQ_CH[point].Gain.DataGain = data;
	Transmit_DSP_SafeLoad( (uint8_t *)&result, 4,  dDSP.Preset.Data[Main_Preset].EQ_CH[point].Gain.addrInDsp);

}

void DSP_GroupSetGain( uint16_t filtr, float dataGain, short UsbSend){

	if(dDSP.Group.State[filtr/2]){
		DSP_SetEqGain(dDSP.Group.Channel[filtr], dataGain);

		if(UsbSend != 0){
			sprintf( (char*)Dixom.Buff, MaskGetDspGain, dDSP.Group.Channel[filtr], dDSP.Preset.Data[Main_Preset].EQ_CH[dDSP.Group.Channel[filtr]].Gain.DataGain);
  			TransmitDataOutputs(Dixom.Buff, AutoLenght,  false,  true, allAvailable);

			if(UsbSend ==2){
				sprintf( (char*)Dixom.Buff, MaskGetDspGain,filtr, dDSP.Preset.Data[Main_Preset].EQ_CH[filtr].Gain.DataGain);
 			 	TransmitDataOutputs(Dixom.Buff, AutoLenght,  false,  true, allAvailable);

			}
		}
	}
}

void DSP_Action_ControlSource(uint16_t action, uint16_t data2, uint16_t data3, uint16_t data4, uint16_t channel, uint16_t pressed){

	if(pressed == YES){
		if(action == 0){
			DSP_SoursRouting(data2);
		}else if(action == 1){
			DSP_SoursSelect(data2, dDSP.Settings[SoundSource].Data);
			DSP_Source(DATA_SET, (uint8_t*)"0");
		}else if(action == 2){

			DSP_EEPROM_NeedToSave();

			if(data2 == SoursUsb){
				if( dDSP.Volume[VolumUsb].DataVol>VOL_MIN ){
					DSP_SetVolume(VolumUsb, VOL_MIN );
				}else{
					DSP_SetVolume(VolumUsb,  VOL_MAX);
				}
			}
			if(data2 == SoursBluetoooth){
				if( dDSP.Volume[VolumBluetooth].DataVol>VOL_MIN ){
					DSP_SetVolume(VolumBluetooth, VOL_MIN );
				}else{
					DSP_SetVolume(VolumBluetooth,  VOL_MAX);
				}
			}
			if(data2 == SoursAux){
				if( dDSP.Volume[VolumAux].DataVol>VOL_MIN ){
					DSP_SetVolume(VolumAux, VOL_MIN);
				}else{
					DSP_SetVolume(VolumAux,  VOL_MAX);
				}
			}
			if(data2 == SoursRadio){
				if( dDSP.Volume[VolumRadio].DataVol>VOL_MIN ){
					DSP_SetVolume(VolumRadio, VOL_MIN);
				}else{
					DSP_SetVolume(VolumRadio,  VOL_MAX);
				}
			}
			if(data2 == SoursSpdif){
				if( dDSP.Volume[VolumSpdif].DataVol>VOL_MIN ){
					DSP_SetVolume(VolumSpdif, VOL_MIN);
				}else{
					DSP_SetVolume(VolumRadio, VOL_MAX);
				}
			}
		}
	}
}

void DSP_SoursRouting(short direction){

	short oldSource = dDSP.Settings[SoundSource].Data;
	short i=0;
   	while( i<6 ){
   		i++;
   	   	if(direction==0){
   	   	    dDSP.Settings[SoundSource].Data--;
   	   	}else{
   	      	dDSP.Settings[SoundSource].Data++;
   	   	}
   	   	if(dDSP.Settings[SoundSource].Data>5) dDSP.Settings[SoundSource].Data=0;
   	    if(dDSP.Settings[SoundSource].Data<0) dDSP.Settings[SoundSource].Data=5;

   		switch(dDSP.Settings[SoundSource].Data){
   		case SoursUsb:
   			if( dDSP.Settings[Sound_Routing_USB].Data == ON){
   				DSP_SoursSelect(SoursUsb, oldSource);
   				i=30;
   			}
   			break;
   		case SoursBluetoooth:
   			if( dDSP.Settings[Sound_Routing_Bluetooth].Data == ON ){
   				DSP_SoursSelect(SoursBluetoooth, oldSource);
   				i=30;
   			}
   			break;
   		case SoursAux:
   			if( dDSP.Settings[Sound_Routing_Aux].Data == ON ){
   				DSP_SoursSelect(SoursAux, oldSource);
   				i=30;
   			}
   			break;
   		case SoursRadio:
   			if( dDSP.Settings[Sound_Routing_Radio].Data == ON ){
   				DSP_SoursSelect(SoursRadio, oldSource);
   				i=30;
   			}
   			break;
   		case SoursSpdif:
   			if( dDSP.Settings[Sound_Routing_Spdif].Data == ON ){
   				DSP_SoursSelect(SoursSpdif, oldSource);
   				i=30;
   			}
   			break;
   		case SoursMic:
   			if( dDSP.Settings[Sound_Routing_Mic].Data == ON ){
   				DSP_SoursSelect(SoursMic, oldSource);
   				i=30;
   			}
   			break;
   		}
   	}
   	DSP_EEPROM_NeedToSave();
   	DSP_Source_Get(All);
}


void DSP_SoursSelect(short sours, short oldSource){

	dDSP.Settings[SoundSource].Data = sours;
	switch(oldSource){
	case SoursUsb:		      DSP_SourceVolSet (VolumUsb,       dDSP.Volume[MinVolumUsb].DataVol  );  break;
	case SoursBluetoooth:	  DSP_SourceVolSet (VolumBluetooth, dDSP.Volume[MinVolumBluetooth].DataVol );  break;
	case SoursAux:	          DSP_SourceVolSet (VolumAux,       dDSP.Volume[MinVolumAux].DataVol   );  break;
	case SoursRadio:	      DSP_SourceVolSet (VolumRadio,     dDSP.Volume[MinVolumRadio].DataVol   );  break;
	case SoursSpdif:	      DSP_SourceVolSet (VolumSpdif,     dDSP.Volume[MinVolumSpdif].DataVol  );  break;
	case SoursMic:	          DSP_SourceVolSet (VolumSoursMic,  dDSP.Volume[MinVolumMic].DataVol     );  break;
	}
	switch(sours){
	case SoursUsb:		      DSP_SourceVolSet (VolumUsb,       VOL_MAX);	    break;
	case SoursBluetoooth:	  DSP_SourceVolSet (VolumBluetooth, VOL_MAX);      break;
	case SoursAux:	          DSP_SourceVolSet (VolumAux,       VOL_MAX);      break;
	case SoursRadio:	      DSP_SourceVolSet (VolumRadio,     VOL_MAX);      break;
	case SoursSpdif:	      DSP_SourceVolSet (VolumSpdif,     VOL_MAX);      break;
	case SoursMic:	          DSP_SourceVolSet (VolumSoursMic,  VOL_MAX);      break;
	}
	if(DisplayMenu.MenuStatus == Status_Default){
		DisplayMenu.MenuEvent = YES;
		DisplayMenu.Event.type = EventStatik;
		DisplayMenu.Event.whence = ChangeSource;
		SetDisplayMenu(&DisplayMenu);
	}
}

void DSP_SourceVolSet( short point, short data){

	uint8_t result [4];
	dDSP.Volume[point].DataVol = data;
	DSP_VolCalculation(data, (uint8_t *)&result);
	Transmit_DSP_SafeLoad( (uint8_t *)&result, 4,  dDSP.Volume[point].addrInDsp);
}

void DSP_Action_ControlVolum(uint8_t direction, uint16_t NumVol1, uint16_t NumVol2, uint16_t NumVol3, uint16_t channel, uint16_t pressed){

	if(pressed == YES){
		uint8_t Down = 0 ;
		uint8_t Up = 1;
		DSP_EEPROM_NeedToSave();
		if(NumVol1 > 10 && NumVol1 < NUM_ELEM_VOL && NumVol1 != 0){
			if(direction == Up){
				dDSP.Volume[NumVol1].DataVol ++;
				if(dDSP.Volume[NumVol1].DataVol>VOL_MAX){
					dDSP.Volume[NumVol1].DataVol = VOL_MAX;
				}
			}else if (direction == Down){
				dDSP.Volume[NumVol1].DataVol --;
				if(dDSP.Volume[NumVol1].DataVol<VOL_MIN){
					dDSP.Volume[NumVol1].DataVol = VOL_MIN;
				}
			}
			DSP_SetVolume (NumVol1, dDSP.Volume[NumVol1].DataVol);
			DSP_SendDataVol(NumVol1, dDSP.Volume[NumVol1].DataVol);
		}
		if(NumVol2 > 10 && NumVol2 < NUM_ELEM_VOL && NumVol2 != 0){
			if(direction == Up){
				dDSP.Volume[NumVol2].DataVol ++;
				if(dDSP.Volume[NumVol2].DataVol>VOL_MAX){
					dDSP.Volume[NumVol2].DataVol = VOL_MAX;
				}
			}else if (direction == Down){
				dDSP.Volume[NumVol2].DataVol --;
				if(dDSP.Volume[NumVol2].DataVol<VOL_MIN){
					dDSP.Volume[NumVol2].DataVol = VOL_MIN;
				}
			}
			DSP_SetVolume (NumVol2, dDSP.Volume[NumVol2].DataVol);
			DSP_SendDataVol(NumVol2, dDSP.Volume[NumVol2].DataVol);
		}
		if(NumVol3 > 10 && NumVol3 < NUM_ELEM_VOL && NumVol3 != 0){
			if(direction == Up){
				dDSP.Volume[NumVol3].DataVol ++;
				if(dDSP.Volume[NumVol3].DataVol>VOL_MAX){
					dDSP.Volume[NumVol3].DataVol = VOL_MAX;
				}
			}else if (direction == Down){
				dDSP.Volume[NumVol3].DataVol --;
				if(dDSP.Volume[NumVol3].DataVol<VOL_MIN){
					dDSP.Volume[NumVol3].DataVol = VOL_MIN;
				}
			}
			DSP_SetVolume (NumVol3, dDSP.Volume[NumVol3].DataVol);
			DSP_SendDataVol(NumVol3, dDSP.Volume[NumVol3].DataVol);
		}
		if(NumVol1 == 0){
			dDSP.MuteATT.StateMute=0;
			dDSP.MuteATT.StateAtt =0;
			if(direction == Up){
				dDSP.Volume[NumVol1].DataVol ++;
				if(dDSP.Volume[NumVol1].DataVol>VOL_MAX){
					dDSP.Volume[NumVol1].DataVol = VOL_MAX;
				}
			}else if (direction == Down){
				dDSP.Volume[NumVol1].DataVol --;
				if(dDSP.Volume[NumVol1].DataVol<VOL_MIN){
					dDSP.Volume[NumVol1].DataVol = VOL_MIN;
				}
			}
			DSP_SetMasterVolume (dDSP.Volume[NumVol1].DataVol);
			char data[16];
			sprintf( data, MaskGetDspVol, NumVol1, dDSP.Volume[NumVol1].DataVol);
  			TransmitDataOutputs((uint8_t*)data, AutoLenght,  false,  false, allAvailable);

		}
	}
}

void DSP_SendDataVol(short point, short DataVol){
	char data[20];
	sprintf( data, MaskGetDspVol, point, DataVol);
 	TransmitDataOutputs((uint8_t*)data, AutoLenght,  false,  true, allAvailable);

}

short  DSP_GetVolume ( short point){
	return dDSP.Volume[point].DataVol;
}

void  DSP_SetVolume ( short point, short data){

	dDSP.MuteATT.StateMute=0;
	dDSP.MuteATT.StateAtt =0;
	uint8_t result [4];
	dDSP.Volume[point].DataVol = data;
	if(point>4){
		DSP_VolCalculation(data, (uint8_t *)&result);
		Transmit_DSP_SafeLoad( (uint8_t *)&result, 4,  dDSP.Volume[point].addrInDsp);
	}else if(point==0){
		Dixom.Module.DSP.MuteATT.TouchedVolControl = YES;
		DSP_SetMasterVolume (data );
	}
	if(DisplayMenu.MenuStatus == Status_Default){
		DisplayMenu.MenuEvent = YES;
		DisplayMenu.Event.type = EventStatik;
		DisplayMenu.Event.whence = ChangeVolume;
		DisplayMenu.Param.Num[0] = point;
		DisplayMenu.Param.Num[1] = data;
	}
	SetDisplayMenu(&DisplayMenu);
}

void  DSP_SetMasterVolume (short data ){

	uint8_t result [4];
	Dixom.Module.DSP.MuteATT.TouchedVolControl = YES;
	dDSP.Volume[0].DataVol = data;
	dDSP.Volume[10].DataVol = data;
	if(dDSP.Settings[VolumeControlMode].Data  == VolumControlOnDAC){
		DAC_VolCalculation(data, (uint8_t *)&result);
		//Transmit_DAC(dDSP.Volume[0].addrInDsp,  (uint8_t *)&result,  1, 150, ExchangeMemMainSteam);
		DAC_Volume((uint8_t *)&result[0]);
	}else if(dDSP.Settings[VolumeControlMode].Data == VolumControlOnDSP){
		DSP_VolCalculation(data, (uint8_t *)&result);
		Transmit_DSP_SafeLoad( (uint8_t *)&result, 4,  dDSP.Volume[10].addrInDsp);
	}else if(dDSP.Settings[VolumeControlMode].Data == VolumControlOnLoudness){
		DSP_LoudnessVolumCalculation(data);	}
	SetDisplayMenu(&DisplayMenu);
}

void DSP_NoiseSwitch (uint8_t data){

	dDSP.NoiseGenerator.State = data;
	uint8_t sendData[4] = {data,0,0,0};
	Transmit_DSP_SafeLoad(&sendData[0], 4, dDSP.NoiseGenerator.addrSwitchInDsp);
}



void DSP_SetSinGen (uint8_t state, uint16_t freq, uint16_t phase){

	//y=0.0000416667xв€’0.0000000000  X = freq   FREQ
	//y=5.6903089575xв€’0.7730984590  X = Phase  Phase 1
	//y=5.6903089575xв€’0.7730984590  X = Phase  Phase 1
	float fData = 0;
	dDSP.SineGenerator.State = state;
	dDSP.SineGenerator.Freq = freq;
	uint8_t sendData[4] = {state,0,0,0};
	Transmit_DSP_SafeLoad(&sendData[0], 4, dDSP.SineGenerator.addrSwitchInDsp);
	fData = 0.0000416667*freq-0.0000000000;
	DSP_setFloatPointInDataArray     ( fData, sendData);
	Transmit_DSP_SafeLoad(&sendData[0], 4, dDSP.SineGenerator.addrFreqInDsp);
}

void DSP_DirectVolumControl(short VolNum, uint8_t VolumLvl){

	dDSP.MuteATT.StateMute=0;
	dDSP.MuteATT.StateAtt =0;
	if( VolumLvl>VOL_MAX ) VolumLvl = VOL_MAX;
	if( VolumLvl<VOL_MIN ) VolumLvl = VOL_MIN;
	if(VolNum == DacVolum){
		DSP_SetMasterVolume (VolumLvl);
	}else{
		DSP_SetVolume ( VolNum, VolumLvl);
	}
	char data[16];
	sprintf( data, MaskGetDspVol, VolNum, VolumLvl);
	TransmitDataOutputs((uint8_t *)data, AutoLenght,  false,  true, allAvailable);

}

void DSP_VolumUP(short VolNum){


	dDSP.Volume[VolNum].DataVol++;
	if( dDSP.Volume[VolNum].DataVol>VOL_MAX ) dDSP.Volume[VolNum].DataVol = VOL_MAX;
	if(VolNum == DacVolum){
		DSP_SetMasterVolume (dDSP.Volume[VolNum].DataVol);
	}else{
		DSP_SetVolume ( VolNum, dDSP.Volume[VolNum].DataVol);
	}
	char data[16];
	sprintf( data, MaskGetDspVol, VolNum, dDSP.Volume[VolNum].DataVol);
	TransmitDataOutputs((uint8_t*)data, AutoLenght,  false,  false, allAvailable);


	//FSC_Bluetooth_Transmit((uint8_t *)data, AutoLenght, NO, FreeRTOS);

	//FSC_Bluetooth_TransmitSPP((uint8_t *)"normalno 132 33 45", AutoLenght, 0, FreeRTOS);
}

void DSP_VolumDown(short VolNum){

	dDSP.Volume[VolNum].DataVol--;
	if( dDSP.Volume[VolNum].DataVol<VOL_MIN ) dDSP.Volume[VolNum].DataVol = VOL_MIN;
	if(VolNum == DacVolum){
		DSP_SetMasterVolume (dDSP.Volume[VolNum].DataVol);
	}else{
		DSP_SetVolume ( VolNum, dDSP.Volume[VolNum].DataVol);
	}
	char data[16];
	sprintf( data, MaskGetDspVol, VolNum, dDSP.Volume[VolNum].DataVol);
 	TransmitDataOutputs((uint8_t*)data, AutoLenght,  false,  true, allAvailable);

	//FSC_Bluetooth_Transmit((uint8_t *)data, AutoLenght, NO, FreeRTOS);

	//FSC_Bluetooth_TransmitSPP((uint8_t *)"Kak dela 100", AutoLenght, 0, FreeRTOS);
	//Transmit_USB_CDC((uint8_t *)"<Kak dela 100 >\r\n", 18);

}

void DSP_VolumeControlSelect(uint8_t direction){

	if(direction == 0){
		dDSP.Settings[VolumeControlMode].Data--;
		if(dDSP.Settings[VolumeControlMode].Data<VolumControlOnDAC) dDSP.Settings[VolumeControlMode].Data = VolumControlOnLoudness;
	}else{
		dDSP.Settings[VolumeControlMode].Data++;
		if(dDSP.Settings[VolumeControlMode].Data>VolumControlOnLoudness) dDSP.Settings[VolumeControlMode].Data = VolumControlOnDAC;
	}
	DSP_SetVolControl(dDSP.Settings[VolumeControlMode].Data);
}


void DSP_SetVolControl(uint8_t data){

	uint8_t result [4];
	if(data < 0 || data > 2){
 		TransmitDataOutputs((uint8_t*)GeneralNotifi_CommandError, AutoLenght,  false,  true, allAvailable);
	}else{
		if(data == 0 && Dixom.Module.AudioDAC.DAC_Type==DAC_AK4440){
 			TransmitDataOutputs((uint8_t *)GeneralNotifi_NotSupported, AutoLenght,  false,  true, allAvailable);
		}else{
			//We set the minimum volume on all volume controls.
			dDSP.Settings[VolumeControlMode].Data = data;
			DAC_VolCalculation(VOL_MIN, (uint8_t *)&result);
			if(Dixom.Module.AudioDAC.DAC_Type != DAC_AK4440){
				//Transmit_DAC(dDSP.Volume[0].addrInDsp,  (uint8_t *)&result,  1, 150, ExchangeMemMainSteam);
				DAC_Volume((uint8_t *)&result[0]);
			}
			DSP_VolCalculation(VOL_MIN, (uint8_t *)&result);
			Transmit_DSP_SafeLoad( (uint8_t *)&result, 4,  dDSP.Volume[10].addrInDsp);
			DSP_LoudnessVolumCalculation(VOL_MIN);
			//Let's wait until the sound turns off
			Delay(200, FreeRTOS);
			if(dDSP.Settings[VolumeControlMode].Data == VolumControlOnDAC && Dixom.Module.AudioDAC.DAC_Type != DAC_AK4440){
				DAC_VolCalculation(dDSP.Volume[0].DataVol, (uint8_t *)&result);
				//Transmit_DAC(dDSP.Volume[0].addrInDsp,  (uint8_t *)&result,  1, 150, ExchangeMemMainSteam);
				DAC_Volume((uint8_t *)&result[0]);
				DSP_VolCalculation(VOL_MAX, (uint8_t *)&result);
				Transmit_DSP_SafeLoad( (uint8_t *)&result, 4,  dDSP.Volume[10].addrInDsp);
				DSP_LoudnessVolumCalculation(VOL_MAX);
			}else if(dDSP.Settings[VolumeControlMode].Data == VolumControlOnDSP){
				DSP_VolCalculation(dDSP.Volume[0].DataVol, (uint8_t *)&result);
				Transmit_DSP_SafeLoad( (uint8_t *)&result, 4,  dDSP.Volume[10].addrInDsp);
				DAC_VolCalculation(VOL_MAX, (uint8_t *)&result);
				//Transmit_DAC(dDSP.Volume[0].addrInDsp,  (uint8_t *)&result,  1, 150, ExchangeMemMainSteam);
				DAC_Volume((uint8_t *)&result[0]);
				DSP_LoudnessVolumCalculation(VOL_MAX);
			}else if(dDSP.Settings[VolumeControlMode].Data == VolumControlOnLoudness){
				DSP_LoudnessVolumCalculation(dDSP.Volume[0].DataVol);
				//Delay(300, FreeRTOS);
				DAC_VolCalculation(VOL_MAX, (uint8_t *)&result);
				//Transmit_DAC(dDSP.Volume[0].addrInDsp,  (uint8_t *)&result,  1, 150, ExchangeMemMainSteam);
				DAC_Volume((uint8_t *)&result[0]);
				DSP_VolCalculation(VOL_MAX, (uint8_t *)&result);
				Transmit_DSP_SafeLoad( (uint8_t *)&result, 4,  dDSP.Volume[10].addrInDsp);
			}
		}
	}
}

void DSP_GetVolControl(uint8_t AllOrModified){
	if(AllOrModified == All ||  dDSP.Settings[VolumeControlMode].Data != DEFAULT_DSP_SETTINGS[VolumeControlMode]){
		sprintf( (char*)Dixom.Buff, MaskGetDspVolControl, dDSP.Settings[VolumeControlMode].Data);

		TransmitDataOutputs(Dixom.Buff, AutoLenght,  false,  true, allAvailable);
	}
}

void DSP_StartVolControlSettings(){

	uint8_t result [4];
	if(dDSP.Settings[VolumeControlMode].Data == VolumControlOnDAC){
		DAC_VolCalculation(dDSP.Volume[0].DataVol, (uint8_t *)&result);
		//Transmit_DAC(dDSP.Volume[0].addrInDsp,  (uint8_t *)&result,  1, 150, ExchangeMemMainSteam);
		DAC_Volume((uint8_t *)&result[0]);
		DSP_VolCalculation(VOL_MAX, (uint8_t *)&result);
		Transmit_DSP_SafeLoad( (uint8_t *)&result, 4,  dDSP.Volume[10].addrInDsp);
		DSP_LoudnessVolumCalculation(VOL_MAX);
	}else if(dDSP.Settings[VolumeControlMode].Data == VolumControlOnDSP){
		DSP_VolCalculation(dDSP.Volume[0].DataVol, (uint8_t *)&result);
		Transmit_DSP_SafeLoad( (uint8_t *)&result, 4,  dDSP.Volume[10].addrInDsp);
		DAC_VolCalculation(VOL_MAX, (uint8_t *)&result);
		//Transmit_DAC(dDSP.Volume[0].addrInDsp,  (uint8_t *)&result,  1, 150, ExchangeMemMainSteam);
		DAC_Volume((uint8_t *)&result[0]);
		DSP_LoudnessVolumCalculation(VOL_MAX);
	}else if(dDSP.Settings[VolumeControlMode].Data == VolumControlOnLoudness){
		DSP_LoudnessVolumCalculation(dDSP.Volume[0].DataVol);
		DAC_VolCalculation(VOL_MAX, (uint8_t *)&result);
		//Transmit_DAC(dDSP.Volume[0].addrInDsp,  (uint8_t *)&result,  1, 150, ExchangeMemMainSteam);
		DAC_Volume((uint8_t *)&result[0]);
		DSP_VolCalculation(VOL_MAX, (uint8_t *)&result);
		Transmit_DSP_SafeLoad( (uint8_t *)&result, 4,  dDSP.Volume[10].addrInDsp);
	}
}





void DSP_Mute(void){

	if(dDSP.MuteATT.StateMute==OFF){
		dDSP.MuteATT.StateMute = ON;
		dDSP.MuteATT.oldAttVolum = dDSP.Volume[DacVolum].DataVol;
		DSP_SetMasterVolume (0);
	}else{
		dDSP.MuteATT.StateMute=OFF;
		DSP_SetMasterVolume (dDSP.MuteATT.oldAttVolum);
	}
	DSP_Volume(DATA_GET, (uint8_t*)"0");
}

void DSP_ATT(void){

	if(dDSP.MuteATT.StateAtt==0){
		dDSP.MuteATT.StateAtt = 1;
		dDSP.MuteATT.oldAttVolum = dDSP.Volume[DacVolum].DataVol;
		if(dDSP.MuteATT.oldAttVolum !=0){
			DSP_SetMasterVolume (dDSP.MuteATT.oldAttVolum/2);
		}
	}else{
		dDSP.MuteATT.StateAtt=0;
		DSP_SetMasterVolume (dDSP.MuteATT.oldAttVolum);
	}
	DSP_Volume(DATA_GET, (uint8_t*)"0");
}

void DSP_ATTRear(short status){
	if(status==1){
		dDSP.MuteATT.oldAttRearVolum = dDSP.Volume[DacVolum].DataVol;
		DSP_SetMasterVolume (5);
	}else{
		DSP_SetMasterVolume (dDSP.MuteATT.oldAttRearVolum);
	}
}

void DSP_GroupSetEq( short filtr, short point, uint8_t onOff, short type,  float boost, short freq, float Q, uint8_t UsbSend){

	if(dDSP.Group.State[filtr/2]){

		DSP_SetEQ ( dDSP.Group.Channel[filtr], point, onOff, type,  boost, freq, Q );

		if(UsbSend == 1){
			sprintf( (char*)Dixom.Buff, MaskGetDspEq, dDSP.Group.Channel[filtr], point, onOff, type, boost*CONV_FLOAT, freq, Q*CONV_FLOAT );
 			TransmitDataOutputs(Dixom.Buff, AutoLenght,  false,  true, allAvailable);
		}
	}
}

void DSP_GroupUpdate( short filtr, short point, uint8_t onOff, short type,  float boost, short freq, float Q, uint8_t UsbSend){

	if(dDSP.Group.State[filtr/2]){

		DSP_UpdatePointEQ(dDSP.Group.Channel[filtr], point, onOff, type,  boost, freq, Q, NO);

		if(UsbSend == 1){
			sprintf(  (char*)Dixom.Buff, MaskGetDspEq, dDSP.Group.Channel[filtr], point, onOff, type, boost*CONV_FLOAT, freq, Q*CONV_FLOAT );
 			TransmitDataOutputs(Dixom.Buff, AutoLenght,  false,  true, allAvailable);
		}
	}
}

void DSP_SetEQ( uint8_t eq_channel, uint8_t band, uint8_t onOff, uint8_t type,  uint16_t boost, uint16_t freq, uint16_t Q){

	uint8_t result[20];

	DSP_UpdatePointEQ(eq_channel, band, onOff, type,  boost, freq, Q, NO);

	if(onOff==ON){
		DSP_CalculateFilter_POINT ( dDSP.Preset.Data[Main_Preset].EQ_CH[eq_channel].Band[band], (uint8_t *)&result );
		Transmit_DSP_SafeLoad( (uint8_t *)&result, 20, dDSP.Preset.Data[Main_Preset].EQ_CH[eq_channel].Band[band].startAddess );
	}else{
		Transmit_DSP_SafeLoad( (uint8_t *)&DEF_POINT_FILTR, 20, dDSP.Preset.Data[Main_Preset].EQ_CH[eq_channel].Band[band].startAddess );
	}
}

void DSP_UpdatePointEQ(uint8_t filtr, uint8_t point, uint8_t onOff, uint8_t type,  uint16_t boost, uint16_t freq, uint16_t Q, uint8_t order){
	dDSP.Preset.Data[Main_Preset].EQ_CH[filtr].Band[point].OnOff    = onOff;
	dDSP.Preset.Data[Main_Preset].EQ_CH[filtr].Band[point].Type     = type;
	dDSP.Preset.Data[Main_Preset].EQ_CH[filtr].Band[point].Q        = Q;
	dDSP.Preset.Data[Main_Preset].EQ_CH[filtr].Band[point].Boost    = boost;
	dDSP.Preset.Data[Main_Preset].EQ_CH[filtr].Band[point].Freq     = freq;
	dDSP.Preset.Data[Main_Preset].EQ_CH[filtr].Band[point].Order    = order;
	dDSP.Preset.Data[Main_Preset].EQ_CH[filtr].Band[point].reserved1 = NO;
	dDSP.Preset.Data[Main_Preset].EQ_CH[filtr].Band[point].reserved2 = NO;
}

void DSP_SetSourceEQ( uint8_t eq_channel, uint8_t band, uint8_t onOff, uint8_t type,  uint16_t boost, uint16_t freq, uint16_t Q){

	uint8_t result[20];

	DSP_UpdatePointSourceEQ(eq_channel, band, onOff, type,  boost, freq, Q, NO);

	if(onOff==ON){
		DSP_CalculateFilter_POINT ( dDSP.Preset.Data[Main_Preset].EQ_Source[eq_channel].Band[band], (uint8_t *)&result );
		Transmit_DSP_SafeLoad( (uint8_t *)&result, 20, dDSP.Preset.Data[Main_Preset].EQ_Source[eq_channel].Band[band].startAddess );
	}else{
		Transmit_DSP_SafeLoad( (uint8_t *)&DEF_POINT_FILTR, 20, dDSP.Preset.Data[Main_Preset].EQ_Source[eq_channel].Band[band].startAddess );
	}
}

void DSP_UpdatePointSourceEQ(uint8_t filtr, uint8_t point, uint8_t onOff, uint8_t type,  uint16_t boost, uint16_t freq, uint16_t Q, uint8_t order){
	dDSP.Preset.Data[Main_Preset].EQ_Source[filtr].Band[point].OnOff    = onOff;
	dDSP.Preset.Data[Main_Preset].EQ_Source[filtr].Band[point].Type     = type;
	dDSP.Preset.Data[Main_Preset].EQ_Source[filtr].Band[point].Q        = Q;
	dDSP.Preset.Data[Main_Preset].EQ_Source[filtr].Band[point].Boost    = boost;
	dDSP.Preset.Data[Main_Preset].EQ_Source[filtr].Band[point].Freq     = freq;
	dDSP.Preset.Data[Main_Preset].EQ_Source[filtr].Band[point].Order    = order;
	dDSP.Preset.Data[Main_Preset].EQ_Source[filtr].Band[point].reserved1 = NO;
	dDSP.Preset.Data[Main_Preset].EQ_Source[filtr].Band[point].reserved2 = NO;
}


uint8_t DSP_GetBypassStatus(uint8_t Channel){
	return dDSP.Preset.Data[Main_Preset].EQ_CH[Channel].Bypass;
}

void DSP_SetBypassEq(short filtr, short status){

	short PointCount = 0;
	uint8_t result[20];

	dDSP.Settings[UserPresset].Data = Main_Preset;

	dDSP.Preset.Data[Main_Preset].EQ_CH[filtr].Bypass = status;
	PointCount = NUM_ELEM_EQ_CH;

	if( DSP_GetBypassStatus(filtr)==OFF ) {
		for (short Point  = 0; Point  < PointCount; Point++ ) {
			if(dDSP.Preset.Data[Main_Preset].EQ_CH[filtr].Band[Point].OnOff == ON){
				DSP_CalculateFilter_POINT ( dDSP.Preset.Data[Main_Preset].EQ_CH[filtr].Band[Point], (uint8_t *)&result );
			}else{
				memcpy( result,DEF_POINT_FILTR, 20 );
			}
			Transmit_DSP_SafeLoad( (uint8_t *)&result, 20, dDSP.Preset.Data[Main_Preset].EQ_CH[filtr].Band[Point].startAddess );
		}
	}else{
		for (short Point  = 0; Point  < PointCount; Point++ ) {
			memcpy( result,DEF_POINT_FILTR, 20 );
			Transmit_DSP_SafeLoad( (uint8_t *)&result, 20, dDSP.Preset.Data[Main_Preset].EQ_CH[filtr].Band[Point].startAddess );
		}
	}
}

void DSP_GroupSetBypassEq( short channel, short status, short UsbSend){

	if(dDSP.Group.State[channel/2]){

		DSP_SetBypassEq(dDSP.Group.Channel[channel], status);

		if(UsbSend != 0){


			sprintf( (char*)Dixom.Buff, MaskGetDspStateEQ, dDSP.Group.Channel[channel], dDSP.Preset.Data[Main_Preset].EQ_CH[dDSP.Group.Channel[channel]].Bypass);
 			TransmitDataOutputs(Dixom.Buff, AutoLenght,  false,  true, allAvailable);

			if(UsbSend ==2){
				sprintf( (char*)Dixom.Buff, MaskGetDspStateEQ, channel, dDSP.Preset.Data[Main_Preset].EQ_CH[channel].Bypass);
 				TransmitDataOutputs(Dixom.Buff, AutoLenght,  false,  true, allAvailable);
			}
		}
	}
}

sFiltrParam  DSP_GetEQ ( short filtr, short point ){
	return dDSP.Preset.Data[Main_Preset].EQ_CH[filtr].Band[point];
}

void  DSP_EqGetDataOut ( uint8_t eq_channel, uint8_t band ){
	sprintf((char*)Dixom.Buff, MaskGetDspEq, eq_channel, band,
			dDSP.Preset.Data[Main_Preset].EQ_CH[eq_channel].Band[band].OnOff,
			dDSP.Preset.Data[Main_Preset].EQ_CH[eq_channel].Band[band].Type,
			dDSP.Preset.Data[Main_Preset].EQ_CH[eq_channel].Band[band].Boost,
			dDSP.Preset.Data[Main_Preset].EQ_CH[eq_channel].Band[band].Freq,
			dDSP.Preset.Data[Main_Preset].EQ_CH[eq_channel].Band[band].Q );

 	TransmitDataOutputs(Dixom.Buff, AutoLenght,  false,  true, allAvailable);
}


sFiltrParam  DSP_GetCrossover ( short filtr, short point ){

	return dDSP.Preset.Data[Main_Preset].Crossover[filtr].Filter[point];

}


void DSP_SetCrossover( uint8_t channel, uint8_t band, uint8_t onOff, uint8_t type,  uint16_t freq, uint8_t order, uint16_t Q ){

	uint8_t result[100];

	DSP_UpdatePoint_Cross(channel, band, onOff, type, NO, freq, Q, order);

	if(onOff==YES){
		DSP_setCrossoverOrderedStructAndCalcRes( dDSP.Preset.Data[Main_Preset].Crossover[channel].Filter[band], (uint8_t *)&result);
	}else{
		for (short i=0;i<5;i++){
			memcpy(result+(i*20),DEF_POINT_FILTR, 20 );
		}
	}

	for (short i=0;i<5;i++){
		Transmit_DSP_SafeLoad( &result[i*20], 20, dDSP.Preset.Data[Main_Preset].Crossover[channel].Filter[band].startAddess+i*5);
	}
}


void DSP_UpdatePoint_Cross(uint8_t channel, uint8_t band, uint8_t onOff, uint8_t type,  uint16_t boost, uint16_t freq, uint16_t Q, uint8_t order){
	dDSP.Preset.Data[Main_Preset].Crossover[channel].Filter[band].OnOff = onOff;
	dDSP.Preset.Data[Main_Preset].Crossover[channel].Filter[band].Type    = type;
	dDSP.Preset.Data[Main_Preset].Crossover[channel].Filter[band].Q       = Q;
	dDSP.Preset.Data[Main_Preset].Crossover[channel].Filter[band].Boost   = boost;
	dDSP.Preset.Data[Main_Preset].Crossover[channel].Filter[band].Freq    = freq;
	dDSP.Preset.Data[Main_Preset].Crossover[channel].Filter[band].Order   = order;
	dDSP.Preset.Data[Main_Preset].Crossover[channel].Filter[band].reserved1 = NO;
	dDSP.Preset.Data[Main_Preset].Crossover[channel].Filter[band].reserved2 = NO;
}


void DSP_setCrossoverOrderedStructAndCalcRes(sFiltrParam sourcePoint, uint8_t *result){
	sFiltrParam crossoverOrderedPoints[5];

	for (short i = 0; i<5; i++){
		crossoverOrderedPoints[i].Boost       = sourcePoint.Boost;
		crossoverOrderedPoints[i].Freq        = sourcePoint.Freq;
		crossoverOrderedPoints[i].OnOff       = sourcePoint.OnOff;
		crossoverOrderedPoints[i].Order       = sourcePoint.Order;
		crossoverOrderedPoints[i].Q           = sourcePoint.Q;
		if (i>0){
			crossoverOrderedPoints[i].Type        = FILTER_OFF;
		}else {
			crossoverOrderedPoints[i].Type        = sourcePoint.Type;
		}
		crossoverOrderedPoints[i].startAddess = sourcePoint.startAddess;
	}


	switch(sourcePoint.Order){
	case(CROSS_ORDER_1):

		 if(sourcePoint.Type==LOWPASS || sourcePoint.Type==BUTTERWORTH_LP || sourcePoint.Type==BESSEL_LP || sourcePoint.Type == LOWPASS_FIRST){
			 crossoverOrderedPoints[0].Type = LOWPASS_FIRST;
         }else{
             crossoverOrderedPoints[0].Type = HIGHPASS_FIRST;
         }

	     break;
	case(CROSS_ORDER_2):
		     crossoverOrderedPoints[0].Type = sourcePoint.Type;
		 break;
	case(CROSS_ORDER_3):
		     crossoverOrderedPoints[0].Type = sourcePoint.Type;
	     if(sourcePoint.Type==LOWPASS || sourcePoint.Type==BUTTERWORTH_LP || sourcePoint.Type==BESSEL_LP || sourcePoint.Type == LOWPASS_FIRST){
	         crossoverOrderedPoints[1].Type = LOWPASS_FIRST;
         }else{
             crossoverOrderedPoints[1].Type = HIGHPASS_FIRST;
         }

	     break;
	case(CROSS_ORDER_4):
			 crossoverOrderedPoints[0].Type = sourcePoint.Type;
		     crossoverOrderedPoints[1].Type = sourcePoint.Type;
	     break;
	case(CROSS_ORDER_5):
		     crossoverOrderedPoints[0].Type = sourcePoint.Type;
		     crossoverOrderedPoints[1].Type = sourcePoint.Type;
		 if(sourcePoint.Type==LOWPASS || sourcePoint.Type==BUTTERWORTH_LP || sourcePoint.Type==BESSEL_LP || sourcePoint.Type == LOWPASS_FIRST){
		     crossoverOrderedPoints[2].Type = LOWPASS_FIRST;
         }else{
             crossoverOrderedPoints[2].Type = HIGHPASS_FIRST;
         }
		 break;
	case(CROSS_ORDER_6):
	         crossoverOrderedPoints[0].Type = sourcePoint.Type;
	         crossoverOrderedPoints[1].Type = sourcePoint.Type;
	         crossoverOrderedPoints[2].Type = sourcePoint.Type;
	     break;
	case(CROSS_ORDER_7):
	         crossoverOrderedPoints[0].Type = sourcePoint.Type;
	         crossoverOrderedPoints[1].Type = sourcePoint.Type;
		     crossoverOrderedPoints[2].Type = sourcePoint.Type;
		 if(sourcePoint.Type==LOWPASS || sourcePoint.Type==BUTTERWORTH_LP || sourcePoint.Type==BESSEL_LP || sourcePoint.Type == LOWPASS_FIRST){
		     crossoverOrderedPoints[3].Type = LOWPASS_FIRST;
         }else{
             crossoverOrderedPoints[3].Type = HIGHPASS_FIRST;
         }
		 break;
	case(CROSS_ORDER_8):
	         crossoverOrderedPoints[0].Type = sourcePoint.Type;
		     crossoverOrderedPoints[1].Type = sourcePoint.Type;
		     crossoverOrderedPoints[2].Type = sourcePoint.Type;
		     crossoverOrderedPoints[3].Type = sourcePoint.Type;
		 break;
	case(CROSS_ORDER_9):
	         crossoverOrderedPoints[0].Type = sourcePoint.Type;
		     crossoverOrderedPoints[1].Type = sourcePoint.Type;
	         crossoverOrderedPoints[2].Type = sourcePoint.Type;
		     crossoverOrderedPoints[3].Type = sourcePoint.Type;
		 if(sourcePoint.Type==LOWPASS || sourcePoint.Type==BUTTERWORTH_LP || sourcePoint.Type==BESSEL_LP || sourcePoint.Type == LOWPASS_FIRST){
		     crossoverOrderedPoints[4].Type = LOWPASS_FIRST;
         }else{
             crossoverOrderedPoints[4].Type = HIGHPASS_FIRST;
         }
		 break;
	 case(CROSS_ORDER_10):
	         crossoverOrderedPoints[0].Type = sourcePoint.Type;
		     crossoverOrderedPoints[1].Type = sourcePoint.Type;
		     crossoverOrderedPoints[2].Type = sourcePoint.Type;
		     crossoverOrderedPoints[3].Type = sourcePoint.Type;
		     crossoverOrderedPoints[4].Type = sourcePoint.Type;
		break;
	    }

	for (short i=0;i<5;i++){
	  if ((i==0) ||  (( i>0 ) && ( crossoverOrderedPoints[0].Type!=crossoverOrderedPoints[i].Type ))){
		  DSP_CalculateFilter_POINT ( crossoverOrderedPoints[i], result+ (i*20) );
	  }else {
		memcpy(result+ (i*20),result, 20);
	  }
    }
}


void DSP_SetParamLoudness(int8_t point, short FreqLP, short LevelLP, short FreqHP, short LevelHP){
	dDSP.Loudness[point].LevelHP = LevelHP;
	dDSP.Loudness[point].FreqHP =  FreqHP;
	dDSP.Loudness[point].LevelLP = LevelLP;
	dDSP.Loudness[point].FreqLP =  FreqLP;
	dDSP.Loudness[point].reserved1 = NO;
}

void DSP_SetParamLimiter(int8_t point, short Threshold, short RmsTC, short Decay){
	dDSP.Limiter[point].Threshold = Threshold;
	dDSP.Limiter[point].RmsTC =  RmsTC;
	dDSP.Limiter[point].Decay = Decay;
	dDSP.Limiter[point].reserved = NO;
}

float DSP_GetSoundDelay ( short point){
	return dDSP.Preset.Data[Main_Preset].SoundDelay[point].DataDelay;
}

void  DSP_SetSoundDelay ( uint8_t point, uint16_t data ){

	uint8_t result [4];

	uint16_t MAX_DELAY = ((DELAY_SAMPLE/(dDSP.SAMPLE_RATE/100))*1000);

	if(point > NUM_ELEM_DELAY || data<SOUND_DELAY_MIN || data> MAX_DELAY){
 		TransmitDataOutputs((uint8_t *)GeneralNotifi_CommandError, AutoLenght,  false,  true, allAvailable);
	}else{

		int16_t difference = data - dDSP.Preset.Data[Main_Preset].SoundDelay[point].DataDelay;

	//	if(difference <0) difference = difference*(-1);

		dDSP.Settings[UserPresset].Data = Main_Preset;
		dDSP.Preset.Data[Main_Preset].SoundDelay[point].DataDelay = data;
		DSP_DelayCalculation( data, (uint8_t *)&result );
		Transmit_DSP_SafeLoad( (uint8_t *)&result, 4,  dDSP.Preset.Data[Main_Preset].SoundDelay[point].addrInDsp);

		/*for(uint8_t channel = 0; channel< NUM_ELEM_DELAY; channel++){
			if(channel != point && dDSP.Group.Delay[point] == dDSP.Group.Delay[channel]){

				int16_t delayData = dDSP.Preset.Data[Main_Preset].SoundDelay[channel].DataDelay + difference;
				if (delayData < 0) delayData = 0;
				if (delayData > MAX_DELAY) delayData = MAX_DELAY;
				dDSP.Preset.Data[Main_Preset].SoundDelay[channel].DataDelay = delayData;
				DSP_DelayCalculation( delayData, (uint8_t *)&result );
				Transmit_DSP_SafeLoad( (uint8_t *)&result, 4,  dDSP.Preset.Data[Main_Preset].SoundDelay[channel].addrInDsp);
			}
		}*/
	}
}

void  DSP_SetWideStereo ( uint8_t point, uint8_t data ){

	uint8_t result  [4];
	uint8_t result1 [4]={0,255,85,206};
	uint8_t result2 [4]={0,246,52,252};

	if(point < 0 || point > NUM_ELEM_WIDESTEREO || data<WIDESTEREO_MIN || data> WIDESTEREO_MAX ){
  		TransmitDataOutputs((uint8_t *)GeneralNotifi_CommandError, AutoLenght,  false,  true, allAvailable);

	}else{
		dDSP.WideStereo[point].Gain = data;

		DSP_WideStereoCalculation( data, (uint8_t *)&result );
		Transmit_DSP_SafeLoad( (uint8_t *)&result, 4,  dDSP.WideStereo[point].addrInDsp);
		Transmit_DSP_SafeLoad( (uint8_t *)&result1, 4,  dDSP.WideStereo[point].addrInDsp+1);
		Transmit_DSP_SafeLoad( (uint8_t *)&result2, 4,  dDSP.WideStereo[point].addrInDsp+4);

	}
}

uint8_t DSP_GetWideStereo ( uint8_t point){
	return dDSP.WideStereo[point].Gain;
}

short  DSP_GetInvert ( short point){
	return dDSP.Preset.Data[Main_Preset].Invert[point].DataInvert;
}

void  DSP_SetInvert ( short point, short data ){

	uint8_t result [4];

	if(point > NUM_ELEM_INVERT || data<0 || data> 1){
  		TransmitDataOutputs((uint8_t *)GeneralNotifi_CommandError, AutoLenght,  false,  true, allAvailable);

	}else{

		dDSP.Settings[UserPresset].Data = Main_Preset;
		dDSP.Preset.Data[Main_Preset].Invert[point].DataInvert = data;
		DSP_setFloatPointInDataArray( (float)data, &result[0] );
		if(data==1){
			result[0]= 0;
			result[1]= 0;
			result[2]= 0;
			result[3]= 1;
		}else{
			result[0]= 0;
			result[1]= 0;
			result[2]= 0;
			result[3]= 0;
		}
		Transmit_DSP_SafeLoad( (uint8_t *)&result, 4,  dDSP.Preset.Data[Main_Preset].Invert[point].addrInDsp);
	}
}

void DSP_SetPhatStereo  ( short point, short gain, short freq ){

	uint8_t result [4];

	dDSP.PhatStereo[point].DataGain = gain;
	dDSP.PhatStereo[point].DataFreq = freq;

	DSP_VolCalculation(gain, (uint8_t *)&result);
	Transmit_DSP_SafeLoad( (uint8_t *)&result, 4,  dDSP.PhatStereo[point].addrInDsp + 2*point);

	DSP_PhatStereoCalculation(freq, (uint8_t *)&result);
	Transmit_DSP_SafeLoad( (uint8_t *)&result, 4,  dDSP.PhatStereo[point].addrInDsp + 2*point +1);

}

uint8_t DefaultState_EQ(uint8_t channel, uint8_t band){

	uint8_t state = YES;

	if(dDSP.Preset.Data[Main_Preset].EQ_CH[channel].Band[band].OnOff != FILTER_EQ_ONOFF_DEF || dDSP.Preset.Data[Main_Preset].EQ_CH[channel].Band[band].Freq !=EQ_CH_DEFAULT[band]
															       || dDSP.Preset.Data[Main_Preset].EQ_CH[channel].Band[band].Type !=FILTER_TYPE_DEF
															       || dDSP.Preset.Data[Main_Preset].EQ_CH[channel].Band[band].Boost !=FILTER_BOOST_DEF
															       || dDSP.Preset.Data[Main_Preset].EQ_CH[channel].Band[band].Q !=FILTER_EQ_CH_Q_DEF)
	{
		state = NO;
	}else{
		state = YES;
	}

	if( state == NO){
		return NO;
	}else{
		return YES;
	}
}

uint8_t DefaultState_Source_EQ(uint8_t channel, uint8_t band){

	uint8_t state = YES;

	if(dDSP.Preset.Data[Main_Preset].EQ_Source[channel].Band[band].OnOff != FILTER_EQ_ONOFF_DEF
			|| dDSP.Preset.Data[Main_Preset].EQ_Source[channel].Band[band].Freq !=EQ_SOURCE_DEFAULT[band]
			|| dDSP.Preset.Data[Main_Preset].EQ_Source[channel].Band[band].Type !=FILTER_TYPE_DEF
			|| dDSP.Preset.Data[Main_Preset].EQ_Source[channel].Band[band].Boost !=FILTER_BOOST_DEF
			|| dDSP.Preset.Data[Main_Preset].EQ_Source[channel].Band[band].Q !=FILTER_CROSS_Q_DEF)
	{
		state = NO;
	}else{
		state = YES;
	}

	if( state == NO){
		return NO;
	}else{
		return YES;
	}
}

uint8_t DefaultState_Cross(uint8_t channel, uint8_t band){

	if(dDSP.Preset.Data[Main_Preset].Crossover[channel].Filter[band].OnOff != OFF
			|| dDSP.Preset.Data[Main_Preset].Crossover[channel].Filter[band].Type !=DEFAULT_CROSSOVER_TYPE[band]
			|| dDSP.Preset.Data[Main_Preset].Crossover[channel].Filter[band].Freq !=DEFAULT_CROSSOVER_FREQ[band]
			|| dDSP.Preset.Data[Main_Preset].Crossover[channel].Filter[band].Order !=FILTER_ORDER_DEF
			|| dDSP.Preset.Data[Main_Preset].Crossover[channel].Filter[band].Q !=FILTER_CROSS_Q_DEF	)

	{
		return NO;
	}else{
		return YES;
	}
}

uint8_t DefaultState_PhatStereo(uint8_t point){
	if(dDSP.PhatStereo[point].DataGain != PHATSTEREO_DEFAULT[point*2] || dDSP.PhatStereo[point].DataFreq != PHATSTEREO_DEFAULT[point*2+1]){
		return NO;
	}else{
		return YES;
	}
}

uint8_t DefaultState_Loudness(uint8_t point){
	if(dDSP.Loudness[point].FreqLP  != LOUDNESS_DEFAULT[point*4]||
			dDSP.Loudness[point].LevelLP != LOUDNESS_DEFAULT[point*4+1]||
			dDSP.Loudness[point].FreqHP  != LOUDNESS_DEFAULT[point*4+2]||
			dDSP.Loudness[point].LevelHP != LOUDNESS_DEFAULT[point*4+3])
	{
		return NO;
	}else{
		return YES;
	}
}

uint8_t DefaultState_WideStereo(uint8_t point){
	if(dDSP.WideStereo[point].Gain != WIDESTEREO_DEFAULT[point]){
		return NO;
	}else{
		return YES;
	}
}

uint8_t DefaultState_Volume(uint8_t point){
	if(dDSP.Volume[point].DataVol != DSP_VOL_DEFAULT[point]){
		return NO;
	}else{
		return YES;
	}
}


uint8_t DefaultState_SupperBass(uint8_t point){
	if(dDSP.SupperBass[point].State != SUPPERBASS_DEFAULT[point*4] ||
			dDSP.SupperBass[point].Gain != SUPPERBASS_DEFAULT[point*4+1] ||
			dDSP.SupperBass[point].Freq != SUPPERBASS_DEFAULT[point*4+2] ||
			dDSP.SupperBass[point].Intensity != SUPPERBASS_DEFAULT[point*4+3]){
		return NO;
	}else{
		return YES;
	}
}



sPhatStereo DSP_GetPhatStereo  ( short point){

	return dDSP.PhatStereo[point];
}

void DSP_SetSupperBass (uint8_t point,  uint8_t state, uint8_t Level,  uint16_t freq, uint8_t Intensiv){

	uint8_t sData[4] = {0,0,0,0};

	dDSP.SupperBass[point].State = state;
	dDSP.SupperBass[point].Gain = Level;
	dDSP.SupperBass[point].Freq = freq;
	dDSP.SupperBass[point].Intensity = Intensiv;

	if(state == 0){
		sData[0] = 1;
	}else{
		sData[0] = 0;
	}
	Transmit_DSP_SafeLoad(&sData[0], 4,  dDSP.SupperBass[point].dspAddessInte+2);
	DSP_setFloatPointInDataArray(Level*0.1,&sData[0]);
	Transmit_DSP_SafeLoad(&sData[0], 4,  dDSP.SupperBass[point].dspAddessGain);
	DSP_setFloatPointInDataArray(Intensiv*0.1,&sData[0]);
	Transmit_DSP_SafeLoad(&sData[0], 4,  dDSP.SupperBass[point].dspAddessInte);
}


uint8_t DSP_GetSupperBass ( uint8_t point ){

	return 1;
}

WorkRes DSP_Set_Preset(uint8_t preset){

	dDSP.Settings[UserPresset].Data = preset;

	if(preset <COUNT_USER_PRESET){
		dDSP.Preset.Data[Main_Preset] = dDSP.Preset.Data[preset];
		return DSP_Chip_Init(YES);
	}else{
		return WR_OK;
	}
}

void DSP_Save_Preset(uint8_t preset){

	dDSP.Settings[UserPresset].Data = preset;
	dDSP.Preset.Data[preset] = dDSP.Preset.Data[Main_Preset];
}



void DSP_Default_Eq(uint8_t eq_channel, uint8_t dataReturn){

	for (short band = 0; band<NUM_ELEM_EQ_CH_BAND; band++){
		DSP_SetEQ ( eq_channel, band, FILTER_EQ_ONOFF_DEF, FILTER_TYPE_DEF,  FILTER_BOOST_DEF,  EQ_CH_DEFAULT[band], FILTER_EQ_CH_Q_DEF );

		if(dDSP.Group.Channel[eq_channel] != NO_GROUP){
			DSP_SetEQ ( dDSP.Group.Channel[eq_channel], band, FILTER_EQ_ONOFF_DEF, FILTER_TYPE_DEF,  FILTER_BOOST_DEF, EQ_CH_DEFAULT[band], FILTER_EQ_CH_Q_DEF );

			if(dataReturn !=0){
				sprintf( (char*)Dixom.Buff, MaskGetDspEq, dDSP.Group.Channel[eq_channel], band, FILTER_EQ_ONOFF_DEF, FILTER_TYPE_DEF, FILTER_BOOST_DEF*CONV_FLOAT, EQ_CH_DEFAULT[band], FILTER_EQ_CH_Q_DEF*CONV_FLOAT );
		  		TransmitDataOutputs(Dixom.Buff, AutoLenght,  false,  true, allAvailable);
			}
		}

		if(dataReturn ==2){
			sprintf( (char*)Dixom.Buff, MaskGetDspEq, eq_channel, band, FILTER_EQ_ONOFF_DEF, FILTER_TYPE_DEF, FILTER_BOOST_DEF*CONV_FLOAT, EQ_CH_DEFAULT[band], FILTER_EQ_CH_Q_DEF*CONV_FLOAT );
	  		TransmitDataOutputs(Dixom.Buff, AutoLenght,  false,  true, allAvailable);
		}
	}
	DSP_SetBypassEq(eq_channel, OFF);
	DSP_GroupSetBypassEq( eq_channel, OFF, dataReturn);
	DSP_SetEqGain(eq_channel, 0);
    DSP_GroupSetGain( eq_channel, 0, dataReturn);
}


void DSP_Default_Source_Eq(uint8_t eq_channel, uint8_t dataReturn){

	for (short band = 0; band<NUM_ELEM_EQ_SOURCE_BAND; band++){
		DSP_SetSourceEQ ( eq_channel, band, FILTER_EQ_ONOFF_DEF, FILTER_TYPE_DEF,  FILTER_BOOST_DEF,  EQ_SOURCE_DEFAULT[band], FILTER_EQ_CH_Q_DEF );
	}
}

void DSP_Default_Crossover(uint8_t cross_channel){

	for (uint8_t band = 0; band<NUM_ELEM_CROSSOVER_TYPE; band++){
		DSP_SetCrossover(cross_channel, band, FILTER_CROSS_ONOFF_DEF, DEFAULT_CROSSOVER_TYPE[band],  DEFAULT_CROSSOVER_FREQ[band], FILTER_ORDER_DEF, FILTER_CROSS_Q_DEF);
	}
}



void DSP_GroupSetCross( uint8_t channel, uint8_t band, uint8_t onOff, uint8_t type,  uint16_t freq, uint8_t order, uint16_t Q, uint8_t UsbSend){

	channel = dDSP.Group.Channel[channel];
	DSP_SetCrossover(  channel, band, onOff, type,  freq, order, Q );
	if(UsbSend == 1){
		sprintf( (char*)Dixom.Buff, MaskGetDspCross, channel, band,
				dDSP.Preset.Data[Main_Preset].Crossover[channel].Filter[band].OnOff,
				dDSP.Preset.Data[Main_Preset].Crossover[channel].Filter[band].Type,
				dDSP.Preset.Data[Main_Preset].Crossover[channel].Filter[band].Freq,
				dDSP.Preset.Data[Main_Preset].Crossover[channel].Filter[band].Order,
			   (dDSP.Preset.Data[Main_Preset].Crossover[channel].Filter[band].Q*CONV_FLOAT) );
  		TransmitDataOutputs(Dixom.Buff, AutoLenght,  false,  true, allAvailable);

	}
}

void Dsp_GetDelayGroup(uint8_t AllOrModified){

	for (short point = 0; point < NUM_ELEM_DELAY; point++) {
		if(dDSP.Group.Delay[point] != DEFAULT_DELAY_GROUP[point] || AllOrModified == All){
			sprintf((char *)Dixom.Buff, MaskGetDelayGroup ,point, dDSP.Group.Delay[point]);
	  		TransmitDataOutputs(Dixom.Buff, AutoLenght,  false,  true, allAvailable);
		}
	}
}

void DSP_SetLoudness(uint8_t source, uint8_t Kuda, int16_t data){
	if(Kuda == 0){
		if(data>60) data = 60;
		dDSP.Loudness[source].FreqLP = data;
		DSP_LoudnessFreqCalculation(source);
	}else if(Kuda == 1){
		if(data>96) data = 96;
		dDSP.Loudness[source].LevelLP = data;
		DSP_LoudnessLevelCalculation(source);
	}else if(Kuda == 2){
		if(data<7000) data = 7000;
		dDSP.Loudness[source].FreqHP = data;
		DSP_LoudnessFreqCalculation(source);
	}else if(Kuda == 3){
		if(data>96) data = 96;
		dDSP.Loudness[source].LevelHP = data;
		DSP_LoudnessLevelCalculation(source);
	}
}

uint8_t *DSP_GetGroupName(uint8_t num){
	return dDSP.Lable.Group[num];
}

void DSP_PresetRouting(uint8_t action, uint8_t data, uint8_t preset){
/*
	if(preset == YES){

		short preset = 1;
			short i = 0;

			if (action == 0 || action == 2 ) {
				preset = GetSettings(SettingsEQPreset);
				while (i < 22) {

					i++;

					if(data == 1){
						preset++;

						if (preset > 20 && action == 0){
							preset = 1;
						}else if(preset > 5 && action == 2){
							preset = 1;
						}

					}else{
						preset--;
						if (preset < 1 && action == 0){
							preset = 20;
						}else if(preset < 1  && action == 2){
							preset = 5;
						}
					}

					switch (preset) {
					case PresetUser1:
						if (GetSettings(SettingsPresetUser1) == ON) {
							SetSettings(SettingsEQPreset, PresetUser1);
							EqPresetUpdate();
							i = 30;
						}
						break;
					case PresetUser2:
						if (GetSettings(SettingsPresetUser2) == ON) {
							SetSettings(SettingsEQPreset, PresetUser2);
							EqPresetUpdate();
							i = 30;
						}
						break;
					case PresetUser3:
						if (GetSettings(SettingsPresetUser3) == ON) {
							SetSettings(SettingsEQPreset, PresetUser3);
							EqPresetUpdate();
							i = 30;
						}
						break;
					case PresetUser4:
						if (GetSettings(SettingsPresetUser4) == ON) {
							SetSettings(SettingsEQPreset, PresetUser4);
							EqPresetUpdate();
							i = 30;
						}
						break;
					case PresetUser5:
						if (GetSettings(SettingsPresetUser5) == ON) {
							SetSettings(SettingsEQPreset, PresetUser5);
							EqPresetUpdate();
							i = 30;
						}
						break;
					case PresetFlat:
						if (GetSettings(SettingsPresetFlat) == ON) {
							SetSettings(SettingsEQPreset, PresetFlat);
							EqPresetUpdate();
							i = 30;
						}
						break;
					case PresetBass:
						if (GetSettings(SettingsPresetBass) == ON) {
							SetSettings(SettingsEQPreset, PresetBass);
							EqPresetUpdate();
							i = 30;
						}
						break;
					case PresetBassExtreme:
						if (GetSettings(SettingsPresetBassExtreme) == ON) {
							SetSettings(SettingsEQPreset, PresetBassExtreme);
							EqPresetUpdate();
							i = 30;
						}
						break;
					case PresetBassAndHigh:
						if (GetSettings(SettingsPresetBassAndHigh) == ON) {
							SetSettings(SettingsEQPreset, PresetBassAndHigh);
							EqPresetUpdate();
							i = 30;
						}
						break;
					case PresetHigh:
						if (GetSettings(SettingsPresetHigh) == ON) {
							SetSettings(SettingsEQPreset, PresetHigh);
							EqPresetUpdate();
							i = 30;
						}
						break;
					case PresetClassic:
						if (GetSettings(SettingsPresetClassic) == ON) {
							SetSettings(SettingsEQPreset, PresetClassic);
							EqPresetUpdate();
							i = 30;
						}
						break;
					case PresetDancing:
						if (GetSettings(SettingsPresetDancing) == ON) {
							SetSettings(SettingsEQPreset, PresetDancing);
							EqPresetUpdate();
							i = 30;
						}
						break;
					case PresetRock:
						if (GetSettings(SettingsPresetRock) == ON) {
							SetSettings(SettingsEQPreset, PresetRock);
							EqPresetUpdate();
							i = 30;
						}
						break;
					case PresetTehno:
						if (GetSettings(SettingsPresetTehno) == ON) {
							SetSettings(SettingsEQPreset, PresetTehno);
							EqPresetUpdate();
							i = 30;
						}
						break;
					case PresetSpeaker:
						if (GetSettings(SettingsPresetSpeaker) == ON) {
							SetSettings(SettingsEQPreset, PresetSpeaker);
							EqPresetUpdate();
							i = 30;
						}
						break;
					case PresetLive:
						if (GetSettings(SettingsPresetLive) == ON) {
							SetSettings(SettingsEQPreset, PresetLive);
							EqPresetUpdate();
							i = 30;
						}
						break;
					case PresetMedium:
						if (GetSettings(SettingsPresetMedium) == ON) {
							SetSettings(SettingsEQPreset, PresetMedium);
							EqPresetUpdate();
							i = 30;
						}
						break;
					case PresetSoft:
						if (GetSettings(SettingsPresetSoft) == ON) {
							SetSettings(SettingsEQPreset, PresetSoft);
							EqPresetUpdate();
							i = 30;
						}
						break;
					case PresetSoftBass:
						if (GetSettings(SettingsPresetSoftBass) == ON) {
							SetSettings(SettingsEQPreset, PresetSoftBass);
							EqPresetUpdate();
							i = 30;
						}
						break;
					case PresetSoftHigh:
						if (GetSettings(SettingsPresetSoftHigh) == ON) {
							SetSettings(SettingsEQPreset, PresetSoftHigh);
							EqPresetUpdate();
							i = 30;
						}
						break;
					}
				}
			} else {
				preset = data;
				if (preset > 20)
					preset = 20;
				if (preset < 1)
					preset = 1;
				SetSettings(SettingsEQPreset, preset);
				EqPresetUpdate();
			}



			//char dataSend[50];
			sprintf((char *)Dixom.Buff, "PRESET_EQ_ROUTING %hu", preset);
			Transmit_Data_Out((uint8_t *) Dixom.Buff, AutoLenght, NO, FreeRTOS, NO);

	}*/
}



