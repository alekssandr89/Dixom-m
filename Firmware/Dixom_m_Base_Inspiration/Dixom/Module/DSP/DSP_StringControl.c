#include <Description.h>
#include <DSP_StringControl.h>
#include "DSP_InternalCalls.h"
#include <DSP_Constant.h>
#include <Exchange.h>
#include <Settings.h>
#include <string.h>
#include <stdio.h>
#include <Structures.h>
#include <DSP_Description.h>
#include <CrystalSix140DThermometer.h>


extern sDixom Dixom;

#define dDSP Dixom.Module.DSP


//else if(strncmp((char*)Received_String, "WIDESTEREO ",     11) == 0)      { DSP_SET_WIDESTEREO(Received_String+11);              DSP_Flash_NeedToSave();       } // Set sound effect wide stereo
//else if(strncmp((char*)Received_String, "SUPPERBASS ",     11) == 0)      { DSP_SET_SUPPERBASS(Received_String+11);              DSP_Flash_NeedToSave();       } // Set sound effect supper bass
//else if(strncmp((char*)Received_String, "WIDESTEREO ",      11) == 0)     { DSP_GET_WIDESTEREO(Received_String+11);                                            } // Get sound effect wide stereo
//else if(strncmp((char*)Received_String, "SUPPERBASS ",      11) == 0)     { DSP_GET_SUPPERBASS (Received_String+11);                                           } // Get sound effect supper bass

/////////////////////////////////////////////////////////////// STRING CONTROL DSP MENU /////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////    SET PARAMETERS IN DSP    ///////////////////////////////////////////////////////////////////////////////////
void SOUND_STRING_CONTROL(uint8_t ControlByte, uint8_t Function, uint8_t *Received_String){

	if(Function == SOUND_SOURCE)                     { DSP_Source(ControlByte, Received_String);              } // OK Save EEPROM
	else if(Function == SOUND_SC_SOURCE)             { DSP_SwitchControlSource(ControlByte, Received_String); } // OK Save FLASH
	else if(Function == SOUND_EQUALIZER)             { DSP_Equalizer (ControlByte, Received_String);          } // OK Save FLASH    STOP
	else if(Function == SOUND_SOURCE_EQUALIZER)      { DSP_SourceEqualizer (ControlByte, Received_String);    } // OK Save FLASH
	else if(Function == SOUND_CROSSOVER)             { DSP_Crossover(ControlByte, Received_String);           } // OK Save FLASH
	else if(Function == SOUND_VOLUME)                { DSP_Volume(ControlByte, Received_String);              } // OK Save EEPROM
	else if(Function == SOUND_SETTINGS)              { DSP_SETTINGS(ControlByte, Received_String);            } // OK Save EEPROM
	else if(Function == SOUND_PRESET)                { DSP_Preset(ControlByte, Received_String);              } // OK Save EEPROM
	else if(Function == SOUND_SC_PRESET)             { DSP_SwitchControlPreset(ControlByte, Received_String); } // OK Save EEPROM
	else if(Function == SOUND_SAVE_PRESET )          { DSP_SavePreset(ControlByte, Received_String);          } // OK Save EEPROM
	else if(Function == SOUND_BYPASS_CH_EQUALIZER)   { DSP_BypassEqualizer(ControlByte, Received_String);     } // OK Save FLASH
	else if(Function == SOUND_INVERT)                { DSP_INVERT(ControlByte, Received_String);              } // OK Save FLASH
	else if(Function == SOUND_DELAY)                 { DSP_Delay(ControlByte, Received_String);               } // OK Save FLASH
	else if(Function == SOUND_DELAY_GROUP_NAME)      { DSP_DELAY_GROUP_NAME(Received_String);                 } // OK Save FLASH
	else if(Function == SOUND_DELAY_GROUP)           { DSP_DELAY_GROUP(ControlByte, Received_String);         } // OK Save FLASH
	else if(Function == SOUND_LOUDNESS)              { DSP_SET_LOUDNESS(ControlByte, Received_String);        } // OK Save FLASH
	else if(Function == SOUND_CHANNEL_NAME)          { DSP_CHANNEL_NAME(Received_String);                     } // OK Save FLASH
	else if(Function == SOUND_PRESET_NAME)           { DSP_PRESET_NAME(Received_String);                      } // OK Save FLASH
	else if(Function == SOUND_GROUP_NAME)            { DSP_GROUP_NAME(ControlByte, Received_String);                       } // OK Save FLASH
	else if(Function == SOUND_GROUP_CH)              { DSP_CHANNEL_GROUPING(ControlByte, Received_String);    } // !!!!!!!!!!!!!!!!
	else if(Function == SOUND_COPY_CHANNEL)          { DSP_SET_COPY_EQ(ControlByte, Received_String);
	                                                   DSP_SET_COPY_CROSS(ControlByte, Received_String);
	                                                   DSP_Flash_NeedToSave();                                } // Set copy Eq and Cross channel to channel
	else if(Function == SOUND_GAIN)                  { DSP_EQ_GAIN(ControlByte, Received_String);             } // OK Save FLASH
	//else if(Function == SOUND_DEFAULT_CH_EQUALIZER)  { DSP_SET_EQ_DEFAULT(Received_String);                   } // OK Save FLASH
	//else if(Function == SOUND_DEF_SOURCE_EQUALIZER)  { DSP_SET_SOURCE_EQ_DEFAULT(Received_String);            } // OK Save FLASH
	//else if(Function == SOUND_DEFAULT_CROSSOVER)     { DSP_SET_CROSSOVER_DEFAULT(Received_String);            } // OK Save FLASH

	else if(Function == SOUND_TEMPERATURE_CONFIG)    { CONFIG_CRYSTALSIX140D_THERMOMETER(ControlByte, Received_String);  }
	else if(Function == SOUND_TEMPERATURE_SHOW)      { GetRealTemperature();                                  }

	else if(Function == SOUND_P_STEREO)              { DSP_SET_PHAT_STEREO(Received_String);             DSP_Flash_NeedToSave();       } // Set sound effect phat stereo
	//else if(Function == SOUND_COPY_CHANNEL)          { DSP_SET_COPY_CROSS(Received_String);              DSP_Flash_NeedToSave();       } // Set copy Crossover channel to channel

	else if(Function == SOUND_NOISE)                 { DSP_SET_NOISE(Received_String);                                                 } // Set On Off sound noise
	else if(Function == SOUND_SIN_GEN)               { DSP_SET_SINGEN(Received_String);                                                } // Set sinus generation
    else if(Function == SOUND_VOL_CONTROL)           { DSP_SET_VOLUME_CONTROL(Received_String);          DSP_EEPROM_NeedToSave();      } // Set sound control type


	else if(Function == SOUND_GET_ALL_MODIFIED_PARAM) { DSP_SHOW_PARAMETERS(Modified);                                                } // Get Modified DSP Parameters
	else{
   		TransmitDataOutputs((uint8_t *)GeneralNotifi_NoPackageFunction, AutoLenght,  false,  true, allAvailable);

	}

	if(ControlByte == DATA_SET){ //SET
	}else if(ControlByte == DATA_GET_ALL){
	}else if(ControlByte == DATA_GET_MODIFIED){
	}else if(ControlByte == DATA_DEFAULT){
	}else{ //GET
	}
}



void DSP_Source(uint8_t ControlByte, uint8_t *Received_String){

    if(ControlByte == DATA_SET){ //SET
    	uint8_t data  = 0;
		sscanf((char *)Received_String, "%hhu", &data);
		if( data<0 || data>5 ){
 	   		TransmitDataOutputs((uint8_t *)GeneralNotifi_CommandError, AutoLenght,  false,  true, allAvailable);

		}else{
			DSP_SoursSelect(data, dDSP.Settings[SoundSource].Data);
			DSP_EEPROM_NeedToSave();
		}
    }else if(ControlByte == DATA_GET_MODIFIED){
    	DSP_Source_Get(Modified);
    }else if(ControlByte == DATA_DEFAULT){
		DSP_SoursSelect(SoursUsb, dDSP.Settings[SoundSource].Data);
		sprintf( (char*)Dixom.Buff, DspNotificationDefault , SOUND_SOURCE, 0);
 		TransmitDataOutputs(Dixom.Buff, AutoLenght,  false,  true, allAvailable);
		DSP_EEPROM_NeedToSave();
    }else if(ControlByte == DATA_GET || ControlByte == DATA_GET_ALL ){
    	DSP_Source_Get(All);
    }else{
	 	TransmitDataOutputs((uint8_t *)GeneralNotifi_CtlBitNotSupported, AutoLenght,  false,  true, allAvailable);
    }
}

void DSP_Source_Get(uint8_t GetType){
	if(GetType == All || dDSP.Settings[SoundSource].Data != SoursUsb){
		sprintf( (char *) Dixom.Buff, MaskGetDspSource, dDSP.Settings[SoundSource].Data);
 	   	TransmitDataOutputs(Dixom.Buff, AutoLenght,  false,  true, allAvailable);

	}
}

void DSP_SwitchControlSource(uint8_t ControlByte, uint8_t *Received_String){

	if(ControlByte == DATA_SET){ //SET
		uint8_t USB = ON, Bluetooth = ON, Aux = ON, Radio = ON, Spdif = ON, Mic = ON;
		sscanf((char *)Received_String,"%hhu %hhu %hhu %hhu %hhu %hhu", &USB, &Bluetooth, &Aux, &Radio, &Spdif, &Mic);

		if( USB<OFF||USB>ON||Bluetooth<OFF||Bluetooth>ON||Aux<OFF||Aux>ON||Radio<OFF||Radio>ON||Spdif<OFF||Spdif>ON||Mic<OFF||Mic>ON ){
 	 	   	TransmitDataOutputs((uint8_t *)GeneralNotifi_CommandError, AutoLenght,  false,  true, allAvailable);

		}else{
			dDSP.Settings[Sound_Routing_USB].Data = USB;
			dDSP.Settings[Sound_Routing_Bluetooth].Data = Bluetooth;
			dDSP.Settings[Sound_Routing_Aux].Data = Aux;
			dDSP.Settings[Sound_Routing_Radio].Data = Radio;
			dDSP.Settings[Sound_Routing_Spdif].Data = Spdif;
			dDSP.Settings[Sound_Routing_Mic].Data = Mic;
			DSP_Flash_NeedToSave();
		}
	}else if(ControlByte == DATA_GET_MODIFIED){
		DSP_SwitchControlSource_Get(Modified);
	}else if(ControlByte == DATA_DEFAULT){
		dDSP.Settings[Sound_Routing_USB].Data = ON;
		dDSP.Settings[Sound_Routing_Bluetooth].Data = ON;
		dDSP.Settings[Sound_Routing_Aux].Data = ON;
		dDSP.Settings[Sound_Routing_Radio].Data = ON;
		dDSP.Settings[Sound_Routing_Spdif].Data = ON;
		dDSP.Settings[Sound_Routing_Mic].Data = ON;
		DSP_Flash_NeedToSave();
		sprintf( (char*)Dixom.Buff, DspNotificationDefault , SOUND_SC_SOURCE, 0);
 	 	TransmitDataOutputs(Dixom.Buff, AutoLenght,  false,  true, allAvailable);

	}else if(ControlByte == DATA_GET_ALL || ControlByte == DATA_GET){ //GET
		DSP_SwitchControlSource_Get(All);
	}else{
 	 	TransmitDataOutputs((uint8_t *)GeneralNotifi_CtlBitNotSupported, AutoLenght,  false,  true, allAvailable);

	}
}

void DSP_SwitchControlSource_Get(uint8_t GetType){
	if(GetType == All || dDSP.Settings[Sound_Routing_USB].Data != 1 ||
			dDSP.Settings[Sound_Routing_Bluetooth].Data != 1||
			dDSP.Settings[Sound_Routing_Aux].Data != 1||
			dDSP.Settings[Sound_Routing_Radio].Data != 1||
			dDSP.Settings[Sound_Routing_Spdif].Data != 1||
			dDSP.Settings[Sound_Routing_Mic].Data != 1
			){
		sprintf( (char*) Dixom.Buff, MaskGetDspSCSource,
				dDSP.Settings[Sound_Routing_USB].Data,
				dDSP.Settings[Sound_Routing_Bluetooth].Data,
				dDSP.Settings[Sound_Routing_Aux].Data ,
				dDSP.Settings[Sound_Routing_Radio].Data,
				dDSP.Settings[Sound_Routing_Spdif].Data,
				dDSP.Settings[Sound_Routing_Mic].Data);
 		TransmitDataOutputs(Dixom.Buff, AutoLenght,  false,  true, allAvailable);
	}
}



void DSP_SourceEqualizer(uint8_t ControlByte, uint8_t *Recive_USB ){

	uint8_t eq_source = EQ_GENERAL, band = FILTER_POINT_DEF, dataReturn = OFF, onOff = FILTER_EQ_ONOFF_DEF, type = FILTER_TYPE_DEF;
	float   fboost = FILTER_BOOST_DEF,	fQ =     FILTER_EQ_CH_Q_DEF;
	int16_t  boost, Q;
	uint16_t freq;

	sscanf((char *)Recive_USB, "%hhu %hhu %hhu %hhu %f %hu %f %hhu", &eq_source, &band, &onOff, &type, &fboost, &freq, &fQ, &dataReturn);

	if( (eq_source<0) || (eq_source>NUM_ELEM_SOURCE) || (band<0) || (band>NUM_ELEM_EQ_SOURCE_BAND)){
		TransmitDataOutputs((uint8_t *)GeneralNotifi_CommandError, AutoLenght,  false,  true, allAvailable);
	}else{
		if(ControlByte == DATA_SET){
			boost = fboost*CONV_INT;
			Q = fQ*CONV_INT;

			if( (freq>FREQ_MAX) || (freq<FREQ_MIN) || (boost>BOOST_MAX) || (boost<BOOST_MIN) || (Q>Q_MAX) || (Q<Q_MIN)){
				TransmitDataOutputs((uint8_t *)GeneralNotifi_CommandError, AutoLenght,  false,  true, allAvailable);
			}else{
				dDSP.Settings[UserPresset].Data = Main_Preset;
				DSP_SetSourceEQ ( eq_source, band, onOff, type,  boost, freq, Q );
				DSP_Flash_NeedToSave();
			}
		}else if(ControlByte == DATA_GET_ALL){
			DSP_Source_Equalizer_Get(eq_source, All);
		}else if(ControlByte == DATA_GET_MODIFIED){
			DSP_Source_Equalizer_Get(eq_source, Modified);
		}else if(ControlByte == DATA_DEFAULT){
			DSP_Default_Source_Eq(eq_source, dataReturn);
			sprintf( (char*)Dixom.Buff, DspNotificationDefault , SOUND_SOURCE_EQUALIZER, eq_source);
	 		TransmitDataOutputs(Dixom.Buff, AutoLenght,  false,  true, allAvailable);
			DSP_Flash_NeedToSave();
		}else{ //GET
			sprintf((char*)Dixom.Buff, MaskGetDspSourceEq, eq_source, band,
					dDSP.Preset.Data[Main_Preset].EQ_Source[eq_source].Band[band].OnOff,
					dDSP.Preset.Data[Main_Preset].EQ_Source[eq_source].Band[band].Type,
					(dDSP.Preset.Data[Main_Preset].EQ_Source[eq_source].Band[band].Boost*CONV_FLOAT),
					dDSP.Preset.Data[Main_Preset].EQ_Source[eq_source].Band[band].Freq,
					(dDSP.Preset.Data[Main_Preset].EQ_Source[eq_source].Band[band].Q*CONV_FLOAT) );
	 		TransmitDataOutputs(Dixom.Buff, AutoLenght,  false,  true, allAvailable);
		}
	}
}

void DSP_Source_Equalizer_Get(uint8_t channel, uint8_t GetType){

    for (short band = 0; band<NUM_ELEM_EQ_SOURCE_BAND; band++){
    	if(DefaultState_Source_EQ(channel, band) == NO || GetType == All){
			sprintf((char*)Dixom.Buff, MaskGetDspSourceEq, channel, band,
					dDSP.Preset.Data[Main_Preset].EQ_Source[channel].Band[band].OnOff,
					dDSP.Preset.Data[Main_Preset].EQ_Source[channel].Band[band].Type,
					(dDSP.Preset.Data[Main_Preset].EQ_Source[channel].Band[band].Boost*CONV_FLOAT),
					dDSP.Preset.Data[Main_Preset].EQ_Source[channel].Band[band].Freq,
					(dDSP.Preset.Data[Main_Preset].EQ_Source[channel].Band[band].Q*CONV_FLOAT) );
	 		TransmitDataOutputs(Dixom.Buff, AutoLenght,  false,  true, allAvailable);
    	}
    }
}

void DSP_GET_INITAL_SOURCE_EQ(uint8_t AllOrModified){

	for(uint8_t eq_channel=0; eq_channel<NUM_ELEM_EQ_SOURCE_BAND; eq_channel++){
		DSP_Source_Equalizer_Get(eq_channel, AllOrModified);
	}
}


void DSP_Equalizer(uint8_t ControlByte, uint8_t *Recive_USB ){

	uint8_t eq_channel = EQ_GENERAL, band = FILTER_POINT_DEF, dataReturn = OFF, onOff = FILTER_EQ_ONOFF_DEF, type = FILTER_TYPE_DEF;
	float   fboost = FILTER_BOOST_DEF,	fQ =     FILTER_EQ_CH_Q_DEF;
	int16_t  boost, Q;
	uint16_t freq;

	sscanf((char *)Recive_USB, "%hhu %hhu %hhu %hhu %f %hu %f %hhu", &eq_channel, &band, &onOff, &type, &fboost, &freq, &fQ, &dataReturn);

	if( (eq_channel<0) || (eq_channel>14) || ((eq_channel==0) & (band>27)) || ((eq_channel>0) & (band>14)) ){
		TransmitDataOutputs((uint8_t *)GeneralNotifi_CommandError, AutoLenght,  false,  true, allAvailable);
	}else{
		if(ControlByte == DATA_SET){
			boost = fboost*CONV_INT;
			Q = fQ*CONV_INT;

			if( (freq>FREQ_MAX) || (freq<FREQ_MIN) || (boost>BOOST_MAX) || (boost<BOOST_MIN) || (Q>Q_MAX) || (Q<Q_MIN)){
				TransmitDataOutputs((uint8_t *)GeneralNotifi_CommandError, AutoLenght,  false,  true, allAvailable);
			}else{

				dDSP.Settings[UserPresset].Data = Main_Preset;

				if(DSP_GetBypassStatus(eq_channel)==OFF){
					DSP_SetEQ ( eq_channel, band, onOff, type,  boost, freq, Q );
					DSP_GroupSetEq( eq_channel, band, onOff, type,  boost, freq, Q ,dataReturn);
				}else{
					DSP_UpdatePointEQ(eq_channel, band, onOff, type,  boost, freq, Q, NO);
					DSP_GroupUpdate( eq_channel, band, onOff, type,  boost, freq, Q ,dataReturn);
				}
				DSP_Flash_NeedToSave();
			}
		}else if(ControlByte == DATA_GET_ALL){
			DSP_Equalizer_Get(eq_channel, All);
		}else if(ControlByte == DATA_GET_MODIFIED){
			DSP_Equalizer_Get(eq_channel, Modified);
		}else if(ControlByte == DATA_DEFAULT){
			DSP_Default_Eq(eq_channel, dataReturn);
			sprintf( (char*)Dixom.Buff, DspNotificationDefault , SOUND_EQUALIZER, eq_channel);
	 		TransmitDataOutputs(Dixom.Buff, AutoLenght,  false,  true, allAvailable);
			DSP_Flash_NeedToSave();
		}else if(ControlByte == DATA_GET){
			sprintf((char*)Dixom.Buff, MaskGetDspEq, eq_channel, band,
					dDSP.Preset.Data[Main_Preset].EQ_CH[eq_channel].Band[band].OnOff,
					dDSP.Preset.Data[Main_Preset].EQ_CH[eq_channel].Band[band].Type,
					(dDSP.Preset.Data[Main_Preset].EQ_CH[eq_channel].Band[band].Boost*CONV_FLOAT),
					dDSP.Preset.Data[Main_Preset].EQ_CH[eq_channel].Band[band].Freq,
					(dDSP.Preset.Data[Main_Preset].EQ_CH[eq_channel].Band[band].Q*CONV_FLOAT) );

	 		TransmitDataOutputs(Dixom.Buff, AutoLenght,  false,  true, allAvailable);
		}else{
  	 	 	TransmitDataOutputs((uint8_t *)GeneralNotifi_CtlBitNotSupported, AutoLenght,  false,  true, allAvailable);

		}
	}
}

void DSP_Equalizer_Get(uint8_t channel, uint8_t GetType){

    for (short band = 0; band<NUM_ELEM_EQ_CH_BAND; band++){
    	if(DefaultState_EQ(channel, band) == NO || GetType == All){
			sprintf((char*)Dixom.Buff, MaskGetDspEq, channel, band,
					dDSP.Preset.Data[Main_Preset].EQ_CH[channel].Band[band].OnOff,
					dDSP.Preset.Data[Main_Preset].EQ_CH[channel].Band[band].Type,
					(dDSP.Preset.Data[Main_Preset].EQ_CH[channel].Band[band].Boost*CONV_FLOAT),
					dDSP.Preset.Data[Main_Preset].EQ_CH[channel].Band[band].Freq,
					(dDSP.Preset.Data[Main_Preset].EQ_CH[channel].Band[band].Q*CONV_FLOAT) );
	 		TransmitDataOutputs(Dixom.Buff, AutoLenght,  false,  true, allAvailable);
    	}
    }
}

void DSP_GET_INITAL_EQ(uint8_t AllOrModified){

	for(uint8_t eq_channel=0; eq_channel<NUM_ELEM_EQ_CH; eq_channel++){
		DSP_Equalizer_Get(eq_channel, AllOrModified);
	}
}




void DSP_Volume(uint8_t ControlByte, uint8_t *Recive_USB){

	int16_t data = 0;
	uint16_t point = 0;
	sscanf((char *)Recive_USB, "%hu %hd", &point,  &data);

	if( point<0 || point>NUM_ELEM_VOL ){
		TransmitDataOutputs((uint8_t *)GeneralNotifi_CommandError, AutoLenght,  false,  true, allAvailable);
	}else{
		if(ControlByte == DATA_SET){ //SET
			if( data<VOL_MIN || data>VOL_MAX ){
				TransmitDataOutputs((uint8_t *)GeneralNotifi_CommandError, AutoLenght,  false,  true, allAvailable);
			}else{
				DSP_SetVolume ( point, data);
				DSP_Volume_Response(point, allAvailableExceptCurrent);
				DSP_EEPROM_NeedToSave();
			}
		}else if(ControlByte == DATA_GET){
			 DSP_Volume_Response(point, allAvailable);
		}else if(ControlByte == DATA_GET_ALL){
			DSP_Volume_Get(All);
		}else if(ControlByte == DATA_GET_MODIFIED){
			DSP_Volume_Get(Modified);
		}else if(ControlByte == DATA_DEFAULT){
			DSP_Volume_Default();
			sprintf( (char*)Dixom.Buff, DspNotificationDefault , SOUND_VOLUME, 0);
	 		TransmitDataOutputs(Dixom.Buff, AutoLenght,  false,  true, allAvailable);
		}else{ //GET
			sprintf( (char*)Dixom.Buff, MaskGetDspVol ,point, dDSP.Volume[point].DataVol);
	 		TransmitDataOutputs(Dixom.Buff, AutoLenght,  false,  true, allAvailable);
		}
	}
}

void DSP_Volume_Get(uint8_t GetType){
	for (short point = 0; point < NUM_ELEM_VOL; point++) {
		if((DefaultState_Volume(point)==NO || GetType == All) &&  point != 10){
			sprintf( (char *)Dixom.Buff, MaskGetDspVol ,point, dDSP.Volume[point].DataVol);
	 		TransmitDataOutputs(Dixom.Buff, AutoLenght,  false,  true, allAvailable);
		}
	}
}

void DSP_Volume_Response(uint8_t point, uint8_t intreface){

	sprintf( (char *)Dixom.Buff, MaskGetDspVol ,point, dDSP.Volume[point].DataVol);
		TransmitDataOutputs(Dixom.Buff, AutoLenght,  false,  true, intreface);
}


void DSP_DELAY_GROUP_NAME(uint8_t *Received_String){

	uint16_t group = 0;
	sscanf((char *)Received_String, "%hu", &group);
	uint8_t NewData = Received_String[1];

	if(group>NUM_ELEM_GROUP_DELAY){
		TransmitDataOutputs((uint8_t *)GeneralNotifi_CommandError, AutoLenght,  false,  true, allAvailable);
	}else{
		uint8_t len = (strlen(Received_String)-2);

		if(NewData == DATA_SET){ //SET
			for(uint8_t i = 0; i<NAME_MAX_LEN; i++){
				if(i<len){
					dDSP.Lable.Group[group][i] = Received_String[i+2];
				}else{
					dDSP.Lable.Group[group][i] = 0;
				}
			}
			 DSP_Flash_NeedToSave();
		}else{ //GET
			sprintf( (char*)Dixom.Buff, MaskGetDspDelayGroupLable, group, dDSP.Lable.Group[group]);
	 		TransmitDataOutputs(Dixom.Buff, AutoLenght,  false,  true, allAvailable);
		}
	}
}

void DSP_PRESET_NAME(uint8_t *Received_String){

	uint16_t preset = 0;
	sscanf((char *)Received_String, "%hu", &preset);
	uint8_t NewData = Received_String[1];

	if(preset>NUM_PRESET){
		TransmitDataOutputs((uint8_t *)GeneralNotifi_CommandError, AutoLenght,  false,  true, allAvailable);
	}else{
		uint8_t len = (strlen(Received_String)-2);

		if(NewData == DATA_SET){ //SET
			for(uint8_t i = 0; i<NAME_MAX_LEN; i++){
				if(i<len){
					dDSP.Lable.Preset[preset][i] = Received_String[i+2];
				}else{
					dDSP.Lable.Preset[preset][i] = 0;
				}
			}
			 DSP_Flash_NeedToSave();
		}else{ //GET
			sprintf( (char*)Dixom.Buff, MaskGetDspPresetLable, preset, dDSP.Lable.Preset[preset]);
	 		TransmitDataOutputs(Dixom.Buff, AutoLenght,  false,  true, allAvailable);
		}
	}
}

void DSP_CHANNEL_NAME(uint8_t *Received_String){

	uint16_t channel = 0;
	uint8_t Bias = 1;
	sscanf((char *)Received_String, "%hu", &channel);
	if(channel>9)Bias++;
	uint8_t NewData = Received_String[Bias];
	Bias++;

	if(channel>NUM_ELEM_EQ_CH){
		TransmitDataOutputs((uint8_t *)GeneralNotifi_CommandError, AutoLenght,  false,  true, allAvailable);
	}else{
		uint8_t len = (strlen(Received_String)-Bias);

		if(NewData == DATA_SET){ //SET
			for(uint8_t i = 0; i<NAME_MAX_LEN; i++){
				if(i<len){
					dDSP.Lable.Channel[channel][i] = Received_String[i+Bias];
				}else{
					dDSP.Lable.Channel[channel][i] = 0;
				}
			}
			 DSP_Flash_NeedToSave();
		}else{ //GET
			sprintf( (char*)Dixom.Buff, MaskSspGetChannelName, channel, dDSP.Lable.Channel[channel]);
	 		TransmitDataOutputs(Dixom.Buff, AutoLenght,  false,  true, allAvailable);
		}
	}
}

void DSP_DELAY_GROUP(uint8_t ControlByte, uint8_t *Received_String){
	uint8_t groupNum = 0;
	uint8_t channel = 0;
	sscanf((char *)Received_String, "%hhu %hhu", &channel, &groupNum);

	if(channel>NUM_ELEM_EQ_CH || groupNum>NUM_ELEM_GROUP_DELAY){
		TransmitDataOutputs((uint8_t *)GeneralNotifi_CommandError, AutoLenght,  false,  true, allAvailable);
	}else{
		if(ControlByte == DATA_SET){ //SET
			dDSP.Group.Delay[channel] = groupNum;
			DSP_Flash_NeedToSave();
		}else if(ControlByte == DATA_GET_ALL){
			Dsp_GetDelayGroup(All);
		}else if(ControlByte == DATA_GET_MODIFIED){
			Dsp_GetDelayGroup(Modified);
		}else if(ControlByte == DATA_DEFAULT){
			DSP_DelayGroup_Default();
		}else{ //GET
			sprintf( (char*)Dixom.Buff, MaskGetDelayGroup, channel, dDSP.Group.Delay[channel]);
	 		TransmitDataOutputs(Dixom.Buff, AutoLenght,  false,  true, allAvailable);
		}
	}
}


void DSP_SwitchControlPreset(uint8_t ControlByte, uint8_t *Received_String){

	if(ControlByte == DATA_SET){ //SET
		uint8_t preset1  = ON,
				preset2  = ON,
				preset3  = ON;
		sscanf((char *)Received_String+1, "%hhu %hhu %hhu", &preset1, &preset2, &preset3);
		if( preset1<0 || preset1>1 || preset2<0 || preset2>1 ||preset3<0 || preset3>1){
			TransmitDataOutputs((uint8_t *)GeneralNotifi_CommandError, AutoLenght,  false,  true, allAvailable);
		}else{
			dDSP.Settings[Preset_Routing_User1].Data = preset1;
			dDSP.Settings[Preset_Routing_User2].Data = preset1;
			dDSP.Settings[Preset_Routing_User2].Data = preset1;
			 DSP_EEPROM_NeedToSave();
		}
	}else if(ControlByte == DATA_DEFAULT){
		dDSP.Settings[Preset_Routing_User1].Data = ON;
		dDSP.Settings[Preset_Routing_User2].Data = ON;
		dDSP.Settings[Preset_Routing_User2].Data = ON;
		DSP_EEPROM_NeedToSave();
		sprintf( (char *) Dixom.Buff, DspNotificationDefault, SOUND_SC_SOURCE,  0);
 		TransmitDataOutputs(Dixom.Buff, AutoLenght,  false,  true, allAvailable);
	}else if(ControlByte == DATA_GET_MODIFIED){
		if(dDSP.Settings[Preset_Routing_User1].Data!=ON || dDSP.Settings[Preset_Routing_User2].Data!=ON|| dDSP.Settings[Preset_Routing_User2].Data!=ON){
			sprintf( (char*)Dixom.Buff, MaskGetDspSCPreset, dDSP.Settings[Preset_Routing_User1].Data, dDSP.Settings[Preset_Routing_User2].Data, dDSP.Settings[Preset_Routing_User2].Data );
	 		TransmitDataOutputs(Dixom.Buff, AutoLenght,  false,  true, allAvailable);
		}
	}else if(ControlByte == DATA_GET || ControlByte == DATA_GET_ALL){
		sprintf( (char*)Dixom.Buff, MaskGetDspSCPreset, dDSP.Settings[Preset_Routing_User1].Data, dDSP.Settings[Preset_Routing_User2].Data, dDSP.Settings[Preset_Routing_User2].Data );
 		TransmitDataOutputs(Dixom.Buff, AutoLenght,  false,  true, allAvailable);
	}
}



void DSP_SHOW_PARAMETERS(uint8_t ShowType){

	//DSP_GET_SOURCE(Modified);                    // ��������� �������� �����
	DSP_Source_Get(ShowType);
	DSP_GET_SINGEN(ShowType);
	DSP_GET_NOISE(ShowType);
	//DSP_GET_ALL_SUPPERBASS(Modified);            // ��������� ��� ��������� ���������
	//DSP_GET_ALL_WIDESTEREO(Modified);            // ��������� ��� ��������� ������ �����
	DSP_GET_INITAL_EQ(ShowType);                 // ��������� ������ ������
	DSP_GET_INITAL_SOURCE_EQ(ShowType);
	DSP_Preset(DATA_GET_MODIFIED, (uint8_t*)"0");                   // ��������� ������ ������ ������������ ������������
	//DSP_GET_ALL_GROUP_CHANNEL(ShowType);         // ����� ���������� �������
	DSP_GET_ALL_CHANNEL_GROUPING(ShowType);
	DSP_GET_ALL_GROUP_NAME(ShowType);            // ����� �������� ���� ��������
	DSP_EQ_BYPASS_SHOW_PARAMETERS(ShowType);             // ��������� ������������
	DSP_GET_INITAL_GAIN(ShowType);               // ������ ����
	DSP_Volume_Get(ShowType);                    // ���������
	DSP_GET_DELAY(ShowType);                 // ���������
	DSP_INVERT_SHOW_PARAMETERS(ShowType);                // ����������� ����
	DSP_GET_ALL_LOUDNESS_F(ShowType);            // ��������� ��������������
	DSP_GET_ALL_PHAT_STEREO(ShowType);           // ��������� ����������� ����������
	DSP_GET_ALL_CROSS(ShowType);                 // ��������� �����������
	DSP_SwitchControlSource_Get(ShowType);      // ������������ ���������� ����� � ������� ������ ��� ������
	DSP_SwitchControlPreset(DATA_GET_MODIFIED, (uint8_t*)"0");      // ������������ ������� � ������� ������ ��� ������
	DSP_GetVolControl(ShowType);            // ��� ���������� ����������
	DSP_SETTINGS_SHOW_PARAMETERS(ShowType);

	if(Dixom.Module.AudioDAC.DAC_Type == AMPLIFIER_3xTAS3251){
		ShowAmplifierTemperature();
	}


}


void DSP_SETTINGS(uint8_t ControlByte, uint8_t *Received_String){
	uint8_t data = 0;
	uint16_t param = 0;
	sscanf((char *)Received_String, "%hu %hhu", &param, &data);
	DSP_Settings(ControlByte, param, data);
}

void DSP_Settings(uint8_t ControlByte, uint8_t param, uint8_t data){

	if( param<0 || param>NUM_ELEM_DSP_SETTINGS ){
		TransmitDataOutputs((uint8_t *)GeneralNotifi_CommandError, AutoLenght,  false,  true, allAvailable);
	}else{
		if(ControlByte == DATA_SET){ //SET
			if( data<0 || data>255 ){
				TransmitDataOutputs((uint8_t *)GeneralNotifi_CommandError, AutoLenght,  false,  true, allAvailable);
			}else{
				dDSP.Settings[param].Data = data;

				switch (param){
				    case VolumeControlMode          :  	DSP_SetVolControl(data);                        break;
				}
				DSP_EEPROM_NeedToSave();
			}
		}else if(ControlByte == DATA_GET_ALL){
			DSP_SETTINGS_SHOW_PARAMETERS(All);
		}else if(ControlByte == DATA_GET_MODIFIED){
			DSP_SETTINGS_SHOW_PARAMETERS(Modified);
		}else if(ControlByte == DATA_DEFAULT){
			DSP_Settings_Default();
			DSP_EEPROM_NeedToSave();
			sprintf( (char *) Dixom.Buff, DspNotificationDefault, SOUND_SETTINGS,  param);
	 		TransmitDataOutputs(Dixom.Buff, AutoLenght,  false,  true, allAvailable);
		}else if(ControlByte == DATA_GET){
			sprintf( (char *) Dixom.Buff, MaskDspGetSettings, param,  dDSP.Settings[param].Data);
	 		TransmitDataOutputs(Dixom.Buff, AutoLenght,  false,  true, allAvailable);
		}else{
			TransmitDataOutputs((uint8_t *)GeneralNotifi_CommandError, AutoLenght,  false,  true, allAvailable);
		}
	}
}





void DSP_Crossover_Get(uint8_t channel, uint8_t AllOrModified){
	for (uint8_t band = 0; band<2; band ++){
		if(AllOrModified == All ||DefaultState_Cross(channel, band) == NO)
		{
			sprintf( (char*)Dixom.Buff, MaskGetDspCross, channel, band,
					dDSP.Preset.Data[Main_Preset].Crossover[channel].Filter[band].OnOff,
					dDSP.Preset.Data[Main_Preset].Crossover[channel].Filter[band].Type,
					dDSP.Preset.Data[Main_Preset].Crossover[channel].Filter[band].Freq,
					dDSP.Preset.Data[Main_Preset].Crossover[channel].Filter[band].Order,
				   (dDSP.Preset.Data[Main_Preset].Crossover[channel].Filter[band].Q*CONV_FLOAT) );

	 		TransmitDataOutputs(Dixom.Buff, AutoLenght,  false,  true, allAvailable);
		}
	}
}

void DSP_GET_ALL_CROSS(uint8_t AllOrModified){
	for(uint8_t channel = 0; channel < NUM_ELEM_EQ_CH; channel ++){
		DSP_Crossover_Get(channel, AllOrModified);
	}
}



void DSP_Crossover(uint8_t ControlByte, uint8_t *Recive_USB){


	uint16_t freq  = FILTER_FREQ_DEF;
	uint16_t Q  = FILTER_FREQ_DEF;
	float   fQ =     FILTER_CROSS_Q_DEF;
	uint8_t band = FILTER_POINT_DEF;
	uint8_t type  = FILTER_TYPE_DEF;
	uint8_t order = NO;
	uint8_t UsbSend = 0;
	uint8_t channel = CH1;
	uint8_t onOff = FILTER_CROSS_ONOFF_DEF;

	sscanf((char *)Recive_USB, "%hhu %hhu %hhu %hhu %hu %hhu %f %hhu",&channel, &band, &onOff, &type, &freq, &order, &fQ, &UsbSend );

	if(channel<0 || channel> 14 || band>2 || band<0){
		TransmitDataOutputs((uint8_t *)GeneralNotifi_CommandError, AutoLenght,  false,  true, allAvailable);
	}else{
		if(ControlByte == DATA_SET){

			Q = fQ*CONV_INT;

			if(freq>FREQ_MAX || freq<FREQ_MIN || Q>Q_MAX || Q<Q_MIN || order>ORDER_MAX || order<ORDER_MIN ){
				TransmitDataOutputs((uint8_t *)GeneralNotifi_CommandError, AutoLenght,  false,  true, allAvailable);
			}else{
				//SetSettings(SettingsCROSSPreset, CrossoverPresetOff);
				DSP_SetCrossover(  channel, band, onOff, type,  freq, order, Q );
				if(dDSP.Group.State[channel/2]){
					DSP_GroupSetCross(  channel, band, onOff, type,  freq, order, Q, UsbSend );
				}
				DSP_Flash_NeedToSave();
			}
		}else if(ControlByte == DATA_GET_ALL){
			DSP_Crossover_Get(channel, All);
		}else if(ControlByte == DATA_GET_MODIFIED){
			DSP_Crossover_Get(channel, Modified);
		}else if(ControlByte == DATA_DEFAULT){
			DSP_Default_Crossover(channel);
			if(dDSP.Group.Channel[channel] != NO_GROUP){
				DSP_Default_Crossover(dDSP.Group.Channel[channel]);
			}
			sprintf( (char*)Dixom.Buff, DspNotificationDefault , SOUND_CROSSOVER, channel);
	 		TransmitDataOutputs(Dixom.Buff, AutoLenght,  false,  true, allAvailable);
			DSP_Flash_NeedToSave();
		}else{ //GET
			sprintf( (char*)Dixom.Buff, MaskGetDspCross, channel, band,
					dDSP.Preset.Data[Main_Preset].Crossover[channel].Filter[band].OnOff,
					dDSP.Preset.Data[Main_Preset].Crossover[channel].Filter[band].Type,
					dDSP.Preset.Data[Main_Preset].Crossover[channel].Filter[band].Freq,
					dDSP.Preset.Data[Main_Preset].Crossover[channel].Filter[band].Order,
				   (dDSP.Preset.Data[Main_Preset].Crossover[channel].Filter[band].Q*CONV_FLOAT) );

	 		TransmitDataOutputs(Dixom.Buff, AutoLenght,  false,  true, allAvailable);
		}
	}
}






void DSP_EQ_GAIN(uint8_t ControlByte, uint8_t *Recive_USB){

	uint8_t channel = 0, UsbSend  = 0;
	float gain = FILTER_BOOST_DEF;
	sscanf((char *)Recive_USB, "%hhu %f %hhu", &channel, &gain, &UsbSend);

	if( (channel < 0 || channel > NUM_ELEM_GAIN) && gain > GAIN_MAX){
		TransmitDataOutputs((uint8_t *)GeneralNotifi_CommandError, AutoLenght,  false,  true, allAvailable);
	}else{
		if(ControlByte == DATA_SET){ //SET
			 DSP_SetEqGain(channel, gain);
			 DSP_GroupSetGain( channel, gain, UsbSend);
			 DSP_Flash_NeedToSave();
		}else if(ControlByte == DATA_GET_ALL){
			DSP_GET_INITAL_GAIN(All);
		}else if(ControlByte == DATA_GET_MODIFIED){
			DSP_GET_INITAL_GAIN(Modified);
		}else if(ControlByte == DATA_DEFAULT){
			DSP_Gain_Default();
			DSP_Flash_NeedToSave();
		}else{  //GET
			sprintf( (char*)Dixom.Buff, MaskGetDspGain, channel, dDSP.Preset.Data[Main_Preset].EQ_CH[channel].Gain.DataGain);
	 		TransmitDataOutputs(Dixom.Buff, AutoLenght,  false,  true, allAvailable);
		}
	}
}


void DSP_GET_INITAL_GAIN(uint8_t AllOrModified){

	for (uint8_t CH = 0; CH < NUM_ELEM_EQ_CH; CH++){

		if(dDSP.Preset.Data[Main_Preset].EQ_CH[CH].Gain.DataGain != 0 || AllOrModified == All){
					sprintf( (char*)Dixom.Buff, MaskGetDspGain,CH, dDSP.Preset.Data[Main_Preset].EQ_CH[CH].Gain.DataGain);
			 		TransmitDataOutputs(Dixom.Buff, AutoLenght,  false,  true, allAvailable);
		}
	}
}






void DSP_INVERT_SHOW_PARAMETERS(uint8_t AllOrModified){

	for (short point = 0; point < NUM_ELEM_INVERT; point++) {
		if(dDSP.Preset.Data[Main_Preset].Invert[point].DataInvert != DEFAULT_INVERT || AllOrModified == All){
			sprintf( (char *)Dixom.Buff, MaskGetDspInvert ,point, dDSP.Preset.Data[Main_Preset].Invert[point].DataInvert);
	 		TransmitDataOutputs(Dixom.Buff, AutoLenght,  false,  true, allAvailable);
		}
	}
}

void DSP_INVERT(uint8_t ControlByte, uint8_t *Recive_USB){

	uint8_t point = 0, data = 0;
	sscanf((char *)Recive_USB, "%hhu %hhu", &point, &data);

	if(point > NUM_ELEM_INVERT){
		TransmitDataOutputs((uint8_t *)GeneralNotifi_CommandError, AutoLenght,  false,  true, allAvailable);
	}else{
		if(ControlByte == DATA_SET){ //SET
			DSP_SetInvert ( point, data );
			DSP_Flash_NeedToSave();
		}else if(ControlByte == DATA_GET_ALL){
			DSP_INVERT_SHOW_PARAMETERS(All);
		}else if(ControlByte == DATA_GET_MODIFIED){
			DSP_INVERT_SHOW_PARAMETERS(Modified);
		}else if(ControlByte == DATA_DEFAULT){
			DSP_Invert_Default();
			sprintf( (char*)Dixom.Buff, DspNotificationDefault , SOUND_INVERT, point);
	 		TransmitDataOutputs(Dixom.Buff, AutoLenght,  false,  true, allAvailable);
		}else if(ControlByte == DATA_GET){//GET
			sprintf( (char *)Dixom.Buff, MaskGetDspInvert,point, dDSP.Preset.Data[Main_Preset].Invert[point].DataInvert);
	 		TransmitDataOutputs(Dixom.Buff, AutoLenght,  false,  true, allAvailable);
		}else{
			TransmitDataOutputs((uint8_t *)GeneralNotifi_CommandError, AutoLenght,  false,  true, allAvailable);
		}
	}
}



void DSP_GET_DELAY(uint8_t AllOrModified){

	for (short point = 0; point < NUM_ELEM_DELAY; point++) {
		if(dDSP.Preset.Data[Main_Preset].SoundDelay[point].DataDelay != DEFAULT_DELAY || AllOrModified == All){
			sprintf((char *)Dixom.Buff, MaskGetDspDelay,point, (dDSP.Preset.Data[Main_Preset].SoundDelay[point].DataDelay*CONV_FLOAT));
	 		TransmitDataOutputs(Dixom.Buff, AutoLenght,  false,  true, allAvailable);
		}
	}
}



void DSP_Delay(uint8_t ControlByte, uint8_t *Recive_USB){

	uint16_t point = 0;
	float data  = 0;
	sscanf((char *)Recive_USB, "%hu %f", &point, &data );

	if( point > NUM_ELEM_DELAY || data > SOUND_DELAY_MAX/100 || point > NUM_ELEM_EQ_CH){
		TransmitDataOutputs((uint8_t *)GeneralNotifi_CommandError, AutoLenght,  false,  true, allAvailable);
	}else{
		if(ControlByte == DATA_SET){ //SET
			DSP_SetSoundDelay ( point, (data*CONV_INT));
			DSP_Flash_NeedToSave();
		}else if(ControlByte == DATA_DEFAULT){
			DSP_Delay_Default();
		}else if(ControlByte == DATA_GET_MODIFIED){
			DSP_GET_DELAY(Modified);
		}else if(ControlByte == DATA_GET_ALL){
			DSP_GET_DELAY(All);
		}else{
			sprintf((char *)Dixom.Buff, MaskGetDspDelay,point, (dDSP.Preset.Data[Main_Preset].SoundDelay[point].DataDelay*CONV_FLOAT));
	 		TransmitDataOutputs(Dixom.Buff, AutoLenght,  false,  true, allAvailable);
		}
	}
}


void DSP_SET_NOISE(uint8_t *Recive_USB){
	uint8_t   state = 0;

	sscanf((char *)Recive_USB, "%hhu", &state);

	if( state<0 || state>1){
		TransmitDataOutputs((uint8_t *)GeneralNotifi_CommandError, AutoLenght,  false,  true, allAvailable);
	}else{
		DSP_NoiseSwitch(state);
	}
}

void DSP_GET_NOISE(uint8_t AllOrModified){
	if(AllOrModified == All || dDSP.NoiseGenerator.State != 0){
		sprintf((char*) Dixom.Buff, MaskGetDspNoise, dDSP.NoiseGenerator.State);
 		TransmitDataOutputs(Dixom.Buff, AutoLenght,  false,  true, allAvailable);
	}
}


void DSP_SET_SINGEN(uint8_t *Recive_USB){

	uint16_t Freq, State;

	sscanf((char *)Recive_USB, "%hu %hu", &State, &Freq);


	if( State<0 || State>1 || Freq<3 || Freq>24000){
		TransmitDataOutputs((uint8_t *)GeneralNotifi_CommandError, AutoLenght,  false,  true, allAvailable);
	}else{
		DSP_SetSinGen(State, Freq, 0);
	}
}

void DSP_GET_SINGEN(uint8_t AllOrModified){
	if(AllOrModified == All || dDSP.SineGenerator.State != 0 || dDSP.SineGenerator.Freq != DEF_SINGEN_FREQ ){
		sprintf((char*) Dixom.Buff, MaskGetDspSinGen, dDSP.SineGenerator.State, dDSP.SineGenerator.Freq);
 		TransmitDataOutputs(Dixom.Buff, AutoLenght,  false,  true, allAvailable);
	}
}


void DSP_GET_ALL_EQ(uint8_t *Recive_USB){

	uint8_t eq_channel = EQ_GENERAL;

	sscanf((char *)Recive_USB, "%hhu", &eq_channel);

	if( eq_channel<0 || eq_channel>14){
		TransmitDataOutputs((uint8_t *)GeneralNotifi_CommandError, AutoLenght,  false,  true, allAvailable);
	}else{

	    for (short band = 0; band<NUM_ELEM_EQ_CH; band++){
			sprintf((char*)Dixom.Buff, MaskGetDspEq, eq_channel, band,
					dDSP.Preset.Data[Main_Preset].EQ_CH[eq_channel].Band[band].OnOff,
					dDSP.Preset.Data[Main_Preset].EQ_CH[eq_channel].Band[band].Type,
					(dDSP.Preset.Data[Main_Preset].EQ_CH[eq_channel].Band[band].Boost*CONV_FLOAT),
					dDSP.Preset.Data[Main_Preset].EQ_CH[eq_channel].Band[band].Freq,
					(dDSP.Preset.Data[Main_Preset].EQ_CH[eq_channel].Band[band].Q*CONV_FLOAT) );

	 		TransmitDataOutputs(Dixom.Buff, AutoLenght,  false,  true, allAvailable);
	    }
	}
}


void DSP_CHANNEL_GROUPING(uint8_t ControlByte, uint8_t *Received_String){
	uint8_t group = 0,
			state = 0,
			fromChannel = 0;
	sscanf((char *)Received_String, "%hhu %hhu %hhu", &group, &state, &fromChannel);

	if(fromChannel == 0){
		fromChannel = group * 2;
	}
	uint8_t toChannel = dDSP.Group.Channel[fromChannel];

	if(group < 0 || group > NUM_ELEM_GROUP || state < OFF || state > ON || fromChannel < 0 || fromChannel > NUM_ELEM_EQ_CH){
		TransmitDataOutputs((uint8_t *)GeneralNotifi_CommandError, AutoLenght,  false,  true, allAvailable);
	}else{
		if(ControlByte == DATA_SET){ //SET
			dDSP.Group.State[group] = state;
			if(state == ON){
				//Copy from bypass to bypass
				dDSP.Preset.Data[Main_Preset].EQ_CH[toChannel].Bypass = dDSP.Preset.Data[Main_Preset].EQ_CH[fromChannel].Bypass;
				//Copy from equalizer to equalizer
				sFiltrParam fromFltr;
				for (short point = 0; point<NUM_ELEM_EQ_CH; point++){
					fromFltr = dDSP.Preset.Data[Main_Preset].EQ_CH[fromChannel].Band[point];
					DSP_SetEQ ( toChannel, point, fromFltr.OnOff, fromFltr.Type,  fromFltr.Boost, fromFltr.Freq, fromFltr.Q );
			 	}
				//Copy from crossover to crossover
				for (short point = 0; point<NUM_ELEM_CROSSOVER_TYPE; point++){
					fromFltr = dDSP.Preset.Data[Main_Preset].Crossover[fromChannel].Filter[point];
					DSP_SetCrossover (toChannel, point, fromFltr.OnOff, fromFltr.Type, fromFltr.Freq, fromFltr.Order, fromFltr.Q );
			 	}
				//Copy from gain to gain
				DSP_SetEqGain(toChannel, dDSP.Preset.Data[Main_Preset].EQ_CH[fromChannel].Gain.DataGain);
			}
			DSP_Flash_NeedToSave();
		}else if(ControlByte == DATA_GET_ALL){
			DSP_GET_ALL_CHANNEL_GROUPING(All);
		}else if(ControlByte == DATA_GET_MODIFIED){
			DSP_GET_ALL_CHANNEL_GROUPING(Modified);
		}else if(ControlByte == DATA_DEFAULT){
			DSP_Grouping_Default();
			DSP_Flash_NeedToSave();
		}else{ //GET
	 		sprintf( (char *)Dixom.Buff, MaskGetDspGroupCH, group, dDSP.Group.State[group]);
	 		TransmitDataOutputs(Dixom.Buff, AutoLenght,  false,  true, allAvailable);
		}
	}
}

void  DSP_GET_ALL_CHANNEL_GROUPING(uint8_t AllOrModified){
	for(uint8_t i=0; i<NUM_ELEM_GROUP; i++){
		if( (dDSP.Group.State[i] != ON) || AllOrModified == All){
			sprintf( (char *)Dixom.Buff, MaskGetDspGroupCH,i, dDSP.Group.State[i]);
	 		TransmitDataOutputs(Dixom.Buff, AutoLenght,  false,  true, allAvailable);
		}
	}
}


/*
void DSP_ChannelGroup(uint8_t ControlByte, uint8_t *Received_String){

	uint8_t fromFltr = 0,
			toFiltr = NUM_ELEM_EQ_CH,
	        UsbSend = 0;

	sscanf((char *)Received_String, "%hhu %hhu %hhu", &fromFltr, &toFiltr, &UsbSend );

	if(fromFltr < 0 || fromFltr > NUM_ELEM_EQ_CH || toFiltr < 0 || (toFiltr > NUM_ELEM_EQ_CH && toFiltr != NO_GROUP) || fromFltr == toFiltr){
		TransmitDataOutputs((uint8_t *)GeneralNotifi_CommandError, AutoLenght,  false,  true, allAvailable);
	}else{
		if(ControlByte == DATA_SET){ //SET
			if(toFiltr == NO_GROUP){
				toFiltr = dDSP.Group.Channel[fromFltr];
				dDSP.Group.Channel[toFiltr] = NO_GROUP;
				dDSP.Group.Channel[fromFltr] = NO_GROUP;
			}else{
				dDSP.Group.Channel[fromFltr] = toFiltr;
				dDSP.Group.Channel[toFiltr] = fromFltr;
				DSP_SET_COPY_EQ(ControlByte, Received_String);
				DSP_SET_COPY_CROSS(ControlByte, Received_String);
				DSP_SetEqGain(toFiltr, dDSP.Preset.Data[Main_Preset].EQ_CH[fromFltr].Gain.DataGain);
				if(dDSP.Preset.Data[Main_Preset].EQ_CH[fromFltr].Bypass != dDSP.Preset.Data[Main_Preset].EQ_CH[toFiltr].Bypass){
					DSP_BypassEqualizer(ControlByte, Received_String);
					bypassEqualizer(DATA_SET, toFiltr, dDSP.Preset.Data[Main_Preset].EQ_CH[fromFltr].Bypass, 0);
				}
			}
			if(UsbSend == 1){
				sprintf( (char*)Dixom.Buff, MaskGetDspGroupCH,toFiltr, fromFltr);
				TransmitDataOutputs(Dixom.Buff, AutoLenght,  false,  true, allAvailable);
			}
			DSP_Flash_NeedToSave();
		}else if(ControlByte == DATA_GET_ALL){
			DSP_GET_ALL_GROUP_CHANNEL(All);
		}else if(ControlByte == DATA_GET_MODIFIED){
			DSP_GET_ALL_GROUP_CHANNEL(Modified);
		}else if(ControlByte == DATA_DEFAULT){
			DSP_Group_Ch_Default();
			DSP_Flash_NeedToSave();
		}else{ //GET
	 		sprintf( (char *)Dixom.Buff, MaskGetDspGroupCH, fromFltr, dDSP.Group.Channel[fromFltr]);
	 		TransmitDataOutputs(Dixom.Buff, AutoLenght,  false,  true, allAvailable);
		}
	}
}

void  DSP_GET_ALL_GROUP_CHANNEL(uint8_t AllOrModified){
	int8_t add = 1;
	for(uint8_t i=0; i<NUM_ELEM_EQ_CH; i++){
		if( (dDSP.Group.Channel[i] != add+i) || AllOrModified == All){
			sprintf( (char *)Dixom.Buff, MaskGetDspGroupCH,i, dDSP.Group.Channel[i]);
	 		TransmitDataOutputs(Dixom.Buff, AutoLenght,  false,  true, allAvailable);
		}
		if(add == 1){
			add = -1;
		}else{
			add = 1;
		}
	}
}*/

void DSP_GROUP_NAME(uint8_t ControlByte, uint8_t *Received_String){

	uint16_t group = 0;
	sscanf((char *)Received_String, "%hu", &group);

	if(group>NUM_ELEM_GROUP_NAME){
		TransmitDataOutputs((uint8_t *)GeneralNotifi_CommandError, AutoLenght,  false,  true, allAvailable);
	}else{
		int len = strlen(Received_String)-2;

		if(ControlByte == DATA_SET){ //SET
			for(uint8_t i = 0; i<NAME_MAX_LEN; i++){
				if(i<len){
					dDSP.Lable.Group[group][i] = Received_String[i+2];
				}else{
					dDSP.Lable.Group[group][i] = 0;
				}
			}
			DSP_Flash_NeedToSave();
		}else{ //GET
			sprintf( (char*)Dixom.Buff, MaskSspGetGroupName, group, dDSP.Lable.Group[group]);
	 		TransmitDataOutputs(Dixom.Buff, AutoLenght,  false,  true, allAvailable);
		}
	}
}


void DSP_GET_GROUP_NAME(uint8_t *Recive_USB){

	short      gNum = 0;

	sscanf((char *) Recive_USB, "%hu", &gNum);

	if(gNum <1 || gNum >NUM_ELEM_GROUP_NAME ){

		TransmitDataOutputs((uint8_t *)GeneralNotifi_CommandError, AutoLenght,  false,  true, allAvailable);

	}else{

		Dixom.Buff[0] = 'D';
		Dixom.Buff[1] = 'S';
		Dixom.Buff[2] = 'P';
		Dixom.Buff[3] = ' ';
		Dixom.Buff[4] = 'G';
		Dixom.Buff[5] = '_';
		Dixom.Buff[6] = 'N';
		Dixom.Buff[7] = 'A';
		Dixom.Buff[8] = 'M';
		Dixom.Buff[9] = 'E';
		Dixom.Buff[10] = ' ';
		Dixom.Buff[11] = gNum+48;
		Dixom.Buff[12] = ' ';


		for(uint8_t count = 0; count < NAME_MAX_LEN; count ++){
			Dixom.Buff[13+count] = dDSP.Lable.Group[gNum-1][count];
		}

 		TransmitDataOutputs(Dixom.Buff, AutoLenght,  false,  true, allAvailable);
	}
}

void DSP_GET_ALL_GROUP_NAME(uint8_t AllOrModified){

	for(uint8_t gNum = 1; gNum < NUM_ELEM_GROUP_NAME; gNum ++){


		if(AllOrModified == All ||(
		   dDSP.Lable.Group[gNum-1][0] !=(gNum+48)||
		   dDSP.Lable.Group[gNum-1][1] !=' '||
		   dDSP.Lable.Group[gNum-1][2] !='G'||
		   dDSP.Lable.Group[gNum-1][3] !='r'||
		   dDSP.Lable.Group[gNum-1][4] !='o'||
		   dDSP.Lable.Group[gNum-1][5] !='u'||
		   dDSP.Lable.Group[gNum-1][6] !='p'||
		   dDSP.Lable.Group[gNum-1][7] != 0)){

			Dixom.Buff[0] = 'D';
			Dixom.Buff[1] = 'S';
			Dixom.Buff[2] = 'P';
			Dixom.Buff[3] = ' ';
			Dixom.Buff[4] = 'G';
			Dixom.Buff[5] = '_';
			Dixom.Buff[6] = 'N';
			Dixom.Buff[7] = 'A';
			Dixom.Buff[8] = 'M';
			Dixom.Buff[9] = 'E';
			Dixom.Buff[10] = ' ';
			Dixom.Buff[11] = gNum+48;
			Dixom.Buff[12] = ' ';


			for(uint8_t count = 0; count < 30; count ++){
				Dixom.Buff[13+count] = dDSP.Lable.Group[gNum-1][count];
			}

	 		TransmitDataOutputs(Dixom.Buff, AutoLenght,  false,  true, allAvailable);
		}
	}
}

void DSP_BypassEqualizer(uint8_t ControlByte, uint8_t *Recive_USB){
	uint16_t   channel = 0,
			   UsbSend = 0,
			   status = OFF;
	sscanf((char *)Recive_USB, "%hu %hu %hu", &channel,  &status, &UsbSend );
	bypassEqualizer(ControlByte, channel, status, UsbSend);
}

void bypassEqualizer(uint8_t ControlByte, uint16_t channel, uint16_t status, uint16_t UsbSend){
	if(ControlByte == DATA_SET){ //SET
		DSP_SetBypassEq(channel, status);
		DSP_GroupSetBypassEq( channel, status, UsbSend);
		DSP_Flash_NeedToSave();
	}else if(ControlByte == DATA_GET_ALL){
		DSP_EQ_BYPASS_SHOW_PARAMETERS(All);
	}else if(ControlByte == DATA_GET_MODIFIED){
		DSP_EQ_BYPASS_SHOW_PARAMETERS(Modified);
	}else if(ControlByte == DATA_GET){
		sprintf( (char *) Dixom.Buff, MaskGetDspStateEQ, channel, DSP_GetBypassStatus(channel));
 		TransmitDataOutputs(Dixom.Buff, AutoLenght,  false,  true, allAvailable);
	}else{
		TransmitDataOutputs((uint8_t *)GeneralNotifi_CommandError, AutoLenght,  false,  true, allAvailable);
	}
}


void DSP_EQ_BYPASS_SHOW_PARAMETERS(uint8_t AllOrModified){

	for(uint8_t EQ = 0; EQ < NUM_ELEM_EQ_CH; EQ++){
		if(DSP_GetBypassStatus(EQ) != OFF || AllOrModified == All){
			sprintf( (char *) Dixom.Buff, MaskGetDspStateEQ, EQ, DSP_GetBypassStatus(EQ));
	 		TransmitDataOutputs(Dixom.Buff, AutoLenght,  false,  true, allAvailable);
		}
	}
}

void DSP_SET_COPY_EQ(uint8_t ControlByte, uint8_t *Received_String){
	short   fromFltr = 0,
			toFiltr = 0,
			UsbSend = 0;
	sscanf((char *)Received_String, "%hu %hu %hu", &fromFltr, &toFiltr, &UsbSend );

	if(ControlByte == DATA_SET && fromFltr!= toFiltr && fromFltr != 0 && fromFltr != 0 && fromFltr < 15 && toFiltr < 15 ){
		sFiltrParam fromFltrEQ;
		for (short point = 0; point<NUM_ELEM_EQ_CH; point++){
			fromFltrEQ = DSP_GetEQ ( fromFltr, point );
			if(DSP_GetBypassStatus(toFiltr)==OFF){
				DSP_SetEQ ( toFiltr, point, fromFltrEQ.OnOff, fromFltrEQ.Type,  fromFltrEQ.Boost, fromFltrEQ.Freq, fromFltrEQ.Q );
			}else{
				DSP_UpdatePointEQ(toFiltr, point, fromFltrEQ.OnOff, fromFltrEQ.Type,  fromFltrEQ.Boost, fromFltrEQ.Freq, fromFltrEQ.Q, NO);
			}
 		}
		DSP_SetEqGain(toFiltr, dDSP.Preset.Data[Main_Preset].EQ_CH[fromFltr].Gain.DataGain);
		DSP_SetBypassEq(toFiltr, DSP_GetBypassStatus(fromFltr));
		if(UsbSend != 0){
			sprintf( (char*)Dixom.Buff, MaskGetDspStateEQ, toFiltr, DSP_GetBypassStatus(toFiltr));
 	 	 	TransmitDataOutputs(Dixom.Buff, AutoLenght,  false,  true, allAvailable);
			sprintf( (char*)Dixom.Buff, MaskGetDspGain,fromFltr, dDSP.Preset.Data[Main_Preset].EQ_CH[fromFltr].Gain.DataGain);
 	 	 	TransmitDataOutputs(Dixom.Buff, AutoLenght,  false,  true, allAvailable);
		}
		sprintf( (char*)Dixom.Buff, MaskGetDspCopyChannel, fromFltr, toFiltr);
 	 	TransmitDataOutputs((uint8_t *)Dixom.Buff, AutoLenght,  false,  true, allAvailable);
	}else{
		TransmitDataOutputs((uint8_t *)GeneralNotifi_CommandError, AutoLenght,  false,  true, allAvailable);
	}
}

void DSP_SET_COPY_CROSS(uint8_t ControlByte, uint8_t *Received_String){
	short   fromFltr = 0,
			toFiltr = 0;
	sscanf((char *)Received_String, "%hu %hu", &fromFltr, &toFiltr );
	if(ControlByte == DATA_SET && fromFltr != toFiltr && fromFltr != 0 && fromFltr != 0 && fromFltr < 15 && toFiltr < 15 ){
		sFiltrParam fromFltrCROSS;
		for (short point = 0; point<NUM_ELEM_CROSSOVER_TYPE; point++){
			fromFltrCROSS = DSP_GetCrossover ( fromFltr, point );
			DSP_SetCrossover (toFiltr, point, fromFltrCROSS.OnOff, fromFltrCROSS.Type, fromFltrCROSS.Freq, fromFltrCROSS.Order, fromFltrCROSS.Q );
 		}
		sprintf((char*)Dixom.Buff, MaskGetDspCopyChannel, fromFltr, toFiltr);
 	 	TransmitDataOutputs(Dixom.Buff, AutoLenght,  false,  true, allAvailable);
	}else{
		TransmitDataOutputs((uint8_t *)GeneralNotifi_CommandError, AutoLenght,  false,  true, allAvailable);
	}
}

void DSP_SOURCE_ROUTING(uint8_t *Recive_USB){

	short   data  = 0;
	sscanf((char *)Recive_USB, "%hu", &data);

	if( data<0 || data>5 ){
		TransmitDataOutputs((uint8_t *)GeneralNotifi_CommandError, AutoLenght,  false,  true, allAvailable);
	}else{
		DSP_SoursSelect(data, dDSP.Settings[SoundSource].Data);
	}
}



void DSP_SET_PHAT_STEREO(uint8_t *Recive_USB){

	uint16_t point = 0,
		     gain = 0,
		     freq = 0;
	sscanf((char *)Recive_USB, "%hu %hu %hu", &point, &gain, &freq);

	if( point<0 || point>NUM_ELEM_PHATSTEREO || gain<0 || gain>VOL_MAX || freq<FREQ_MIN || gain>FREQ_MAX ){
		TransmitDataOutputs((uint8_t *)GeneralNotifi_CommandError, AutoLenght,  false,  true, allAvailable);
	}else{

		 DSP_SetPhatStereo  ( point, gain, freq );

	}
}

void DSP_GET_PHAT_STEREO(uint8_t *Recive_USB){


	uint16_t point = 0;

	sscanf((char *)Recive_USB, "%hu", &point);

	if( point<0 || point>NUM_ELEM_PHATSTEREO ){
		TransmitDataOutputs((uint8_t *)GeneralNotifi_CommandError, AutoLenght,  false,  true, allAvailable);
	}else{
		char data[30];
		sprintf( data, MaskGetDspPhatStereo, point, dDSP.PhatStereo[point].DataGain, dDSP.PhatStereo[point].DataFreq );
 	 	TransmitDataOutputs((uint8_t *)data, AutoLenght,  false,  true, allAvailable);

	}
}

void DSP_GET_ALL_PHAT_STEREO(uint8_t AllOrModified){

	for(uint8_t point = 0; point < NUM_ELEM_PHATSTEREO; point++){

		if(DefaultState_PhatStereo(point) == NO || AllOrModified == All){
			sprintf( (char *)Dixom.Buff, MaskGetDspPhatStereo, point, dDSP.PhatStereo[point].DataGain, dDSP.PhatStereo[point].DataFreq );
	 		TransmitDataOutputs(Dixom.Buff, AutoLenght,  false,  true, allAvailable);
		}
	}
}


void DSP_SET_WIDESTEREO(uint8_t *Recive_USB){

	short point = 0;
	short data  = 0;

	sscanf((char *)Recive_USB, "%hu %hu", &point, &data);

	DSP_SetWideStereo ( point, data );
}

void DSP_GET_WIDESTEREO(uint8_t *Recive_USB){

	short point = 0;

	sscanf((char *) Recive_USB, "%hu", &point);

	if(point < 0 || point > NUM_ELEM_WIDESTEREO){
		TransmitDataOutputs((uint8_t *)GeneralNotifi_CommandError, AutoLenght,  false,  true, allAvailable);
	}else{
		sprintf( (char *)Dixom.Buff, MaskGetDspWideStereo, point, dDSP.WideStereo[point].Gain);
 		TransmitDataOutputs(Dixom.Buff, AutoLenght,  false,  true, allAvailable);
	}
}

void DSP_GET_ALL_WIDESTEREO(uint8_t AllOrModified){

	for (short point = 0; point < NUM_ELEM_WIDESTEREO; point++) {
		if(DefaultState_WideStereo(point) == NO || AllOrModified == All){
			sprintf((char *)Dixom.Buff, MaskGetDspWideStereo, point, dDSP.WideStereo[point].Gain);
	 		TransmitDataOutputs(Dixom.Buff, AutoLenght,  false,  true, allAvailable);
		}
	}
}


void DSP_SET_SUPPERBASS(uint8_t *Recive_USB){

	uint8_t  Point  = 0;
	uint8_t  State  = 0;
	uint16_t Freq  = 50;
	uint8_t  Intensiv  = 13;
	uint8_t  Level  = 12;

	sscanf((char *)Recive_USB, "%hhu %hhu %hhu %hhu %hu", &Point, &State, &Level, &Intensiv, &Freq);

	Point = Recive_USB[0]-48;

	if(Point < 0 || Point > NUM_ELEM_SUPPERBASS ||State < 0 || State > 1 || Freq<SBASSFREQ_MIN || Freq >SBASSFREQ_MAX || Intensiv<SBASSINTESIV_MIN || Intensiv >SBASSINTESIV_MAX|| Level<SBASSLVL_MIN || Level >SBASSLVL_MAX ){
		TransmitDataOutputs((uint8_t *)GeneralNotifi_CommandError, AutoLenght,  false,  true, allAvailable);
	}else{
		DSP_SetSupperBass(Point, State, Level, Freq, Intensiv);
	}
}

void DSP_GET_SUPPERBASS(uint8_t *Recive_USB){

	uint8_t point = 0;
	sscanf((char *) Recive_USB, "%hhu", &point);

	if(point < 0 || point > NUM_ELEM_SUPPERBASS){
		TransmitDataOutputs((uint8_t *)GeneralNotifi_CommandError, AutoLenght,  false,  true, allAvailable);
	}else{
		sprintf( (char *) Dixom.Buff, MaskGetDspSupperBass, point,
				dDSP.SupperBass[point].State,
				dDSP.SupperBass[point].Gain,
				dDSP.SupperBass[point].Intensity,
				dDSP.SupperBass[point].Freq);
 		TransmitDataOutputs(Dixom.Buff, AutoLenght,  false,  true, allAvailable);
	}
}

void DSP_GET_ALL_SUPPERBASS(uint8_t AllOrModified){
	for (short point = 0; point < NUM_ELEM_SUPPERBASS; point++) {
		if(DefaultState_SupperBass(point) == NO ||  AllOrModified == All){

			sprintf( (char *) Dixom.Buff, MaskGetDspSupperBass, point,
					dDSP.SupperBass[point].State,
					dDSP.SupperBass[point].Gain,
					dDSP.SupperBass[point].Intensity,
					dDSP.SupperBass[point].Freq);

	 		TransmitDataOutputs(Dixom.Buff, AutoLenght,  false,  true, allAvailable);
		}
	}
}

void DSP_SET_EQ_DEFAULT(uint8_t *Recive_USB){

	short eq_channel = 0;
	short dataReturn = 0;
	uint8_t NewData = Recive_USB[0];

	sscanf((char *)Recive_USB+1, "%hu %hu", &eq_channel, &dataReturn);

	if(eq_channel>NUM_ELEM_EQ_CH){
		TransmitDataOutputs((uint8_t *)GeneralNotifi_CommandError, AutoLenght,  false,  true, allAvailable);
	}else{
		if(NewData == DATA_SET){
			DSP_Default_Eq(eq_channel, dataReturn);
			DSP_Flash_NeedToSave();
		}else{
 	 	  	TransmitDataOutputs((uint8_t *)GeneralNotifi_NoReturnData, AutoLenght,  false,  true, allAvailable);
		}
	}
}

void DSP_SET_SOURCE_EQ_DEFAULT(uint8_t *Recive_USB){

	short eq_channel = 0;
	short dataReturn = 0;
	uint8_t NewData = Recive_USB[0];

	sscanf((char *)Recive_USB+1, "%hu %hu", &eq_channel, &dataReturn);

	if(eq_channel>NUM_ELEM_SOURCE){
		TransmitDataOutputs((uint8_t *)GeneralNotifi_CommandError, AutoLenght,  false,  true, allAvailable);
	}else{
		if(NewData == DATA_SET){
			DSP_Default_Source_Eq(eq_channel, dataReturn);
			DSP_Flash_NeedToSave();
		}else{
 	 	  	TransmitDataOutputs((uint8_t *)GeneralNotifi_NoReturnData, AutoLenght,  false,  true, allAvailable);
		}
	}
}

void DSP_SET_CROSSOVER_DEFAULT(uint8_t *Recive_USB){

	uint16_t channel = 0;
	uint8_t NewData = Recive_USB[0];

	sscanf((char *)Recive_USB+1, "%hu", &channel);

	if(channel>NUM_ELEM_EQ_CH){
		TransmitDataOutputs((uint8_t *)GeneralNotifi_CommandError, AutoLenght,  false,  true, allAvailable);
	}else{
		if(NewData == DATA_SET){
			DSP_Default_Crossover(channel);
			if(dDSP.Group.Channel[channel] != NO_GROUP){
				DSP_Default_Crossover(dDSP.Group.Channel[channel]);
			}
			DSP_Flash_NeedToSave();
		}else{
 	 	  	TransmitDataOutputs((uint8_t *)GeneralNotifi_NoReturnData, AutoLenght,  false,  true, allAvailable);

		}
	}
}

void DSP_SET_LOUDNESS(uint8_t ControlByte, uint8_t *Recive_USB){

	short source  = 0;
	short param  = 0;
	short data  = 0;
	sscanf((char *)Recive_USB, "%hu %hu %hu", &source, &param, &data);

	if(param < 0 || param > 3 || source<0 || source >=NUM_ELEM_LOUDNESS){
		TransmitDataOutputs((uint8_t *)GeneralNotifi_CommandError, AutoLenght,  false,  true, allAvailable);
		return;
	}

	if(ControlByte == DATA_SET){ //SET
		DSP_SetLoudness(source, param, data);
	}else if(ControlByte == DATA_GET_MODIFIED){
		DSP_GET_ALL_LOUDNESS_F(Modified);
	}else if(ControlByte == DATA_DEFAULT){
		DSP_Loudness_Default();
	}else if(ControlByte == DATA_GET){
		DSP_GET_LOUDNESS(source);
	}else if(ControlByte == DATA_GET_ALL){ //GET
		DSP_GET_ALL_LOUDNESS_F(All);
	}else{
 	 	TransmitDataOutputs((uint8_t *)GeneralNotifi_CtlBitNotSupported, AutoLenght,  false,  true, allAvailable);
	}
	DSP_Flash_NeedToSave();
}

void DSP_GET_ALL_LOUDNESS_F(uint8_t AllOrModified){
	for (short source = 0; source < NUM_ELEM_LOUDNESS; source++) {
		if(DefaultState_Loudness(source) == NO || AllOrModified == All){
			DSP_GET_LOUDNESS(source);
		}
	}
}

void DSP_GET_LOUDNESS(uint8_t source){
	sprintf((char *) Dixom.Buff, MaskGetDspLoudness, source, dDSP.Loudness[source].FreqLP, dDSP.Loudness[source].LevelLP, dDSP.Loudness[source].FreqHP, dDSP.Loudness[source].LevelHP);
	TransmitDataOutputs(Dixom.Buff, AutoLenght,  false,  true, allAvailable);
}

void DSP_SET_VOLUME_CONTROL(uint8_t *Recive_USB){

	short data  = 0;

	sscanf((char *)Recive_USB, "%hu", &data);

	if(data < 0 || data > 2){
		TransmitDataOutputs((uint8_t *)GeneralNotifi_CommandError, AutoLenght,  false,  true, allAvailable);
	}else{
		if(data == 0){
			if(Dixom.Module.AudioDAC.DAC_Type==DAC_AK4440){
 		  		TransmitDataOutputs((uint8_t *)GeneralNotifi_NotSupported, AutoLenght,  false,  true, allAvailable);

				DSP_GetVolControl(All);
			}else{
				DSP_SetVolControl(data);
			}
		}else{
			DSP_SetVolControl(data);
		}

	}
}





void DSP_SavePreset(uint8_t ControlByte, uint8_t *Received_String){

	if(ControlByte == DATA_SET){ //SET
		uint8_t   preset = 0;
		sscanf((char *) Received_String, "%hhu", &preset);
		if(preset<1 || preset >3){
			TransmitDataOutputs((uint8_t *)GeneralNotifi_CommandError, AutoLenght,  false,  true, allAvailable);
		}else{
			DSP_Save_Preset(preset);
			DSP_Preset(DATA_GET, (uint8_t*)"0");
		}
	}else{
  		TransmitDataOutputs((uint8_t *)GeneralNotifi_NoReturnData, AutoLenght,  false,  true, allAvailable);

	}
}


void DSP_Preset(uint8_t ControlByte, uint8_t *Received_String){

	if(ControlByte == DATA_SET){ //SET
		uint8_t preset = 0;
		sscanf((char *)Received_String, "%hhu", &preset);
		if(preset<1 || preset >COUNT_USER_PRESET-1){
			TransmitDataOutputs((uint8_t *)GeneralNotifi_CommandError, AutoLenght,  false,  true, allAvailable);
		}else{
			DSP_Set_Preset(preset);
			DSP_EEPROM_NeedToSave();
		}
	}else if(ControlByte == DATA_GET_MODIFIED){
		if(dDSP.Settings[UserPresset].Data != Main_Preset){
			sprintf((char *) Dixom.Buff, MaskGetDspPreset, dDSP.Settings[UserPresset].Data );
	 		TransmitDataOutputs(Dixom.Buff, AutoLenght,  false,  true, allAvailable);
		}
	}else if(ControlByte == DATA_GET || ControlByte == DATA_GET_ALL){
		sprintf((char *) Dixom.Buff, MaskGetDspPreset, dDSP.Settings[UserPresset].Data );
 		TransmitDataOutputs(Dixom.Buff, AutoLenght,  false,  true, allAvailable);
	}else{
		TransmitDataOutputs((uint8_t *)GeneralNotifi_CommandError, AutoLenght,  false,  true, allAvailable);
	}
}






