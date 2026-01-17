/*
 * FmRadio_StringControl.c
 *
 *  Created on: 13 ����. 2022 �.
 *      Author: 79825
 */

#include <Description.h>
#include <Description.h>
#include <Structures.h>
#include <RDA_5807.h>
#include <SI47xx.h>
#include <FmRadio_Control.h>

extern sDixom Dixom;
#define dFm_Radio Dixom.Module.FmRadio

void RADIO_SET_FREQ(uint8_t ControlByte, uint8_t *Received_String){

	if(ControlByte == DATA_SET){ //SET
		float    fmFreq = 0;
		sscanf((char *)Received_String, "%f", &fmFreq );
		uint16_t  u16_Freq = fmFreq/FConv;

		if( u16_Freq<MIN_RADIO_FM_FREQ || u16_Freq>MAX_RADIO_FM_FREQ ){
 	 	   	TransmitDataOutputs((uint8_t *)GeneralNotifi_CommandError, AutoLenght,  false,  true, allAvailable);

		}else{
			FmRadio_Set_Frequency(u16_Freq);
		}
	}else{
		RADIO_GET_FREQ(All);
	}
}

void RADIO_SET_AUTO_SEARCH(uint8_t ControlByte, uint8_t *Received_String){

	if(ControlByte == DATA_SET){ //SET
		uint16_t    Direction = 0;
		sscanf((char *)Received_String, "%hu", &Direction );

		if( Direction<0 || Direction>1 ){
  	 	   	TransmitDataOutputs((uint8_t *)GeneralNotifi_CommandError, AutoLenght,  false,  true, allAvailable);

		}else{
			FmRadio_Set_Auto_Search(Direction);
		}
	}else{
 	 	   	TransmitDataOutputs((uint8_t *)GeneralNotifi_NoReturnData, AutoLenght,  false,  true, allAvailable);

	}
}

void RADIO_SET_MANUAL_SEARCH(uint8_t ControlByte, uint8_t *Received_String){

	if(ControlByte == DATA_SET){ //SET
		uint16_t    Direction = 0;
		sscanf((char *)Received_String, "%hu", &Direction );

		if( Direction<0 || Direction>1 ){
  	 	   	TransmitDataOutputs((uint8_t *)GeneralNotifi_CommandError, AutoLenght,  false,  true, allAvailable);
		}else{
			FmRadio_Set_Marual_Search(Direction);
		}
	}else{
 	 	TransmitDataOutputs((uint8_t *)GeneralNotifi_NoReturnData, AutoLenght,  false,  true, allAvailable);

	}
}

void RADIO_SET_SETTINGS(uint8_t ControlByte, uint8_t *Received_String){

	if(ControlByte == DATA_SET){ //SET
		uint8_t Mode_AM_FM = 0;
		uint8_t Spasing = 0;
		uint8_t AutoData = 0;
		uint8_t STEREO = 0;
		uint8_t Band = 0;
		uint8_t PTY = 0;
		uint8_t TA = 0;

		sscanf((char *)Received_String, "%hhu %hhu %hhu %hhu %hhu %hhu %hhu", &Mode_AM_FM, &Spasing, &AutoData, &STEREO, &Band, &PTY, &TA );

		if( Mode_AM_FM<0||Mode_AM_FM>1||Spasing<5||Spasing >20||AutoData>1||AutoData<0||STEREO>1||STEREO<0||Band>4||Band<0||PTY>1||PTY<0||TA>1||TA<0){
  	 	   	TransmitDataOutputs((uint8_t *)GeneralNotifi_CommandError, AutoLenght,  false,  true, allAvailable);
		}else{
			Dixom.Module.FmRadio.Settings.Mode_AM_FM = Mode_AM_FM;
			Dixom.Module.FmRadio.Settings.Spasing = Spasing;
			Dixom.Module.FmRadio.Settings.AutoData = AutoData;
			Dixom.Module.FmRadio.Settings.STEREO = STEREO;
			Dixom.Module.FmRadio.Settings.Band = Band;
			Dixom.Module.FmRadio.Settings.PTY = PTY;
			Dixom.Module.FmRadio.Settings.TA = TA;
			FmRadio_Set_Settings();
		}
	}else{
		RADIO_GET_SETTINGS(All);
	}
}

void RADIO_SET_PRESET(uint8_t ControlByte, uint8_t *Received_String){

	if(ControlByte == DATA_SET){ //SET
		short    preset = 0;
		sscanf((char *)Received_String, "%hu", &preset );

		if( preset<0 || preset>NUM_RADIO_PRESET ){
   	 	   	TransmitDataOutputs((uint8_t *)GeneralNotifi_CommandError, AutoLenght,  false,  true, allAvailable);
		}else{
			if(Dixom.Module.FmRadio.FmPreset[preset].Freq != DEFAULT_RADIO_PRESET_FREQ){
				Dixom.Module.FmRadio.FmPreset[0].Freq = Dixom.Module.FmRadio.FmPreset[preset].Freq;
				FmRadio_Set_Frequency(Dixom.Module.FmRadio.FmPreset[0].Freq);
				Dixom.Module.FmRadio.Settings.SelectedPreset = preset;
				sprintf( (char *)Dixom.Buff, MaskGetRadioCurentPreset, Dixom.Module.FmRadio.Settings.SelectedPreset, Dixom.Module.FmRadio.FmPreset[0].Freq*FConv);
 	   	 	   	TransmitDataOutputs(Dixom.Buff, AutoLenght,  false,  true, allAvailable);

			}
		}
	}else{
		RADIO_GET_TABLE_PRESET(All);
	}
}

void RADIO_SET_CURRENT_PRESET(uint8_t ControlByte, uint8_t *Received_String){

	if(ControlByte == DATA_SET){ //SET
		short    preset = 0;
		sscanf((char *)Received_String, "%hu", &preset );

		if( preset<0 || preset>NUM_RADIO_PRESET ){
  	 	   	TransmitDataOutputs((uint8_t *)GeneralNotifi_CommandError, AutoLenght,  false,  true, allAvailable);
		}else{
			if(Dixom.Module.FmRadio.FmPreset[preset].Freq != DEFAULT_RADIO_PRESET_FREQ){
				Dixom.Module.FmRadio.FmPreset[0].Freq = Dixom.Module.FmRadio.FmPreset[preset].Freq;
				FmRadio_Set_Frequency(Dixom.Module.FmRadio.FmPreset[0].Freq);
				Dixom.Module.FmRadio.Settings.SelectedPreset = preset;
				sprintf( (char *)Dixom.Buff, MaskGetRadioCurentPreset, Dixom.Module.FmRadio.Settings.SelectedPreset, Dixom.Module.FmRadio.FmPreset[0].Freq*FConv);
  	   	 	   	TransmitDataOutputs(Dixom.Buff, AutoLenght,  false,  true, allAvailable);

			}
		}
	}else{
		RADIO_GET_CURRENT_PRESET(All);
	}
}

void RADIO_SET_SAVE_PRESET(uint8_t ControlByte, uint8_t *Received_String){

	if(ControlByte == DATA_SET){ //SET
		uint8_t    preset = 0;
		sscanf((char *)Received_String, "%hhu", &preset );

		if( preset<1 || preset>NUM_RADIO_PRESET ){
  	 	   	TransmitDataOutputs((uint8_t *)GeneralNotifi_CommandError, AutoLenght,  false,  true, allAvailable);
		}else{
			Dixom.Module.FmRadio.FmPreset[preset].Freq = Dixom.Module.FmRadio.FmPreset[0].Freq;
			Dixom.Module.FmRadio.FmPreset[preset].editing = YES;
			sprintf( (char *)Dixom.Buff, MaskGetRadioPreset,  preset, Dixom.Module.FmRadio.FmPreset[preset].Freq*FConv);
	   	 	TransmitDataOutputs(Dixom.Buff, AutoLenght,  false,  true, allAvailable);
			Radio_Eeprom_NeedToSave();
		}
	}else{

		TransmitDataOutputs((uint8_t *)GeneralNotifi_NoReturnData, AutoLenght,  false,  true, allAvailable);

	}
}

void RADIO_SET_AUTO_SAVE_PRESET(){

	uint8_t already=0;

	for(short point=1; point<UNIQ_ELEM_RADIO-1; point++ ){
		if(Dixom.Module.FmRadio.FmPreset[point].Freq == Dixom.Module.FmRadio.FmPreset[RadioFreq].Freq ) already = point;
	}

	if(already==0){
		for(short point=1; point<NUM_RADIO_PRESET; point++){

			if(Dixom.Module.FmRadio.FmPreset[point].Freq==0) {
				Dixom.Module.FmRadio.FmPreset[point].Freq = Dixom.Module.FmRadio.FmPreset[RadioFreq].Freq;
				Dixom.Module.FmRadio.FmPreset[point].editing = YES;
				sprintf((char*)Dixom.Buff, MaskGetRadioCurentPreset,  point, Dixom.Module.FmRadio.FmPreset[RadioFreq].Freq*FConv);
 		   	 	TransmitDataOutputs(Dixom.Buff, AutoLenght,  false,  true, allAvailable);

				return;
			}
		}
	}else{
		sprintf((char*)Dixom.Buff, MaskGetRadioNotifiPointOccup, already );
   	 	TransmitDataOutputs(Dixom.Buff, AutoLenght,  false,  true, allAvailable);
	}
}

void RADIO_SET_DELETE_PRESET(uint8_t ControlByte, uint8_t *Received_String){

	if(ControlByte == DATA_SET){ //SET
		uint8_t    preset = 0;
		sscanf((char *)Received_String, "%hhu", &preset );
		if( preset<1 || preset>20 ){
  	 	   	TransmitDataOutputs((uint8_t *)GeneralNotifi_CommandError, AutoLenght,  false,  true, allAvailable);
		}else{
			Dixom.Module.FmRadio.FmPreset[preset].Freq = 0;
			Dixom.Module.FmRadio.FmPreset[preset].editing = YES;
			sprintf( (char *)Dixom.Buff, MaskGetRadioDelPreset, Dixom.Module.FmRadio.Settings.SelectedPreset);
	   	 	TransmitDataOutputs(Dixom.Buff, AutoLenght,  false,  true, allAvailable);
			Radio_Eeprom_NeedToSave();
		}
	}else{
    	TransmitDataOutputs((uint8_t *)GeneralNotifi_NoReturnData, AutoLenght,  false,  true, allAvailable);

	}
}

void RADIO_GET_FREQ(uint8_t All_Or_Modified){

	if(dFm_Radio.RadioModule == SI4741){
		 SI47xx_GetFreq();
	}else if(dFm_Radio.RadioModule == RDA5807FP){

	}

	if(All_Or_Modified == All || Dixom.Module.FmRadio.FmPreset[0].Freq != DEFAULT_RADIO_FM_FREQ){
	    sprintf((char *) Dixom.Buff, MaskGetRadioFreq, Dixom.Module.FmRadio.FmPreset[0].Freq*FConv );
   	 	TransmitDataOutputs(Dixom.Buff, AutoLenght,  false,  true, allAvailable);
	}
}

void RADIO_GET_SETTINGS(uint8_t All_Or_Modified){

	if(All_Or_Modified == All || Dixom.Module.FmRadio.Settings.Mode_AM_FM != DEFAULT_RADIO_MODE_AM_FM
			                  || Dixom.Module.FmRadio.Settings.Spasing != DEFAULT_RADIO_SPASING
							  || Dixom.Module.FmRadio.Settings.AutoData != DEFAULT_RADIO_AUDIO_DATA
							  || Dixom.Module.FmRadio.Settings.STEREO != DEFAULT_RADIO_STEREO
							  || Dixom.Module.FmRadio.Settings.Band != DEFAULT_RADIO_BAND
							  || Dixom.Module.FmRadio.Settings.PTY !=DEFAULT_RADIO_PTY
							  || Dixom.Module.FmRadio.Settings.TA != DEFAULT_RADIO_TA
							  || Dixom.Module.FmRadio.Settings.RDS != DEFAULT_RADIO_RDS){

		sprintf( (char *)Dixom.Buff, MaskGetRadioSettings,
				Dixom.Module.FmRadio.Settings.Mode_AM_FM,
				Dixom.Module.FmRadio.Settings.Spasing,
				Dixom.Module.FmRadio.Settings.AutoData,
				Dixom.Module.FmRadio.Settings.STEREO,
				Dixom.Module.FmRadio.Settings.Band,
				Dixom.Module.FmRadio.Settings.PTY,
				Dixom.Module.FmRadio.Settings.TA,
				Dixom.Module.FmRadio.Settings.RDS);

    	 	TransmitDataOutputs(Dixom.Buff, AutoLenght,  false,  true, allAvailable);

	}


}

void RADIO_GET_TABLE_PRESET(uint8_t All_Or_Modified){

	for(uint8_t preset = 1; preset < NUM_RADIO_PRESET; preset++){
		if(All_Or_Modified == All || dFm_Radio.FmPreset[preset].Freq != DEFAULT_RADIO_PRESET_FREQ){
			sprintf( (char *)Dixom.Buff, MaskGetRadioPreset, preset, Dixom.Module.FmRadio.FmPreset[preset].Freq*FConv );
	   	 	TransmitDataOutputs(Dixom.Buff, AutoLenght,  false,  true, allAvailable);
		}
	}
}

void RADIO_GET_CURRENT_PRESET(uint8_t All_Or_Modified){

	if(All_Or_Modified == All || Dixom.Module.FmRadio.Settings.SelectedPreset != DEFAULT_RADIO_PRESET){
		sprintf((char *)Dixom.Buff, MaskGetRadioCurentPreset, Dixom.Module.FmRadio.Settings.SelectedPreset,  Dixom.Module.FmRadio.FmPreset[0].Freq*FConv);
   	 	TransmitDataOutputs(Dixom.Buff, AutoLenght,  false,  true, allAvailable);
	}
}

void RADIO_GET_MODULE_INFO(){
	sprintf((char *)Dixom.Buff, MaskGetRadioModule, Dixom.Module.FmRadio.RadioModule, Dixom.Module.FmRadio.Settings.Mode_AM_FM);
	TransmitDataOutputs(Dixom.Buff, AutoLenght,  false,  true, allAvailable);
}

void RADIO_GET_SERVICE_INFORMATION(){
	sprintf((char *) Dixom.Buff, MaskGetRadioServiceInfo,

			dFm_Radio.ServiceInfo.RSSI,
			dFm_Radio.ServiceInfo.SNR,
			dFm_Radio.ServiceInfo.MULT,
			dFm_Radio.ServiceInfo.STEREO,
			dFm_Radio.ServiceInfo.STEREO_BLEND,
			dFm_Radio.ServiceInfo.FREQOFF);

	TransmitDataOutputs(Dixom.Buff, AutoLenght,  false,  true, allAvailable);
}

void FmRadio_Set_Frequency(uint16_t Freq){

	Dixom.Module.FmRadio.FmPreset[0].Freq = Freq;
	Dixom.Module.FmRadio.FmPreset[0].editing = YES;
	Dixom.Module.FmRadio.Settings.SelectedPreset = 0;
	Dixom.Module.FmRadio.Settings.editing = YES;

	if(dFm_Radio.RadioModule == SI4741){
		SI47xx_setFreq();
	}else if(dFm_Radio.RadioModule == RDA5807FP){
		RDA5807_setFreq();
	}
	Radio_Eeprom_NeedToSave();
}

void FmRadio_Set_Auto_Search(uint16_t Direction){

	Dixom.Module.FmRadio.Settings.SelectedPreset = 0;
	if(dFm_Radio.RadioModule == SI4741){
		SI47xx_AutoSearch(Direction);
	}else if(dFm_Radio.RadioModule == RDA5807FP){
		RadioAutoSearch(Direction);
	}
	Radio_Eeprom_NeedToSave();
}

void FmRadio_Set_Marual_Search(uint16_t Direction){
	Dixom.Module.FmRadio.Settings.SelectedPreset = 0;
	if(dFm_Radio.RadioModule == SI4741){
		SI47xx_ManualSearch(Direction);
	}else if(dFm_Radio.RadioModule == RDA5807FP){
		RadioManualSearch(Direction);
	}
	Radio_Eeprom_NeedToSave();
}

void FmRadio_Set_Settings(){
	Dixom.Module.FmRadio.Settings.SelectedPreset = 0;
	if(dFm_Radio.RadioModule == SI4741){
		SI_4741_Set_Spacing();
	}else if(dFm_Radio.RadioModule == RDA5807FP){

	}
	Radio_Eeprom_NeedToSave();
}


void FmRadio_Preset_Routing(uint8_t action, short direction, uint8_t pressed) {

	if (pressed == YES) {
		if (action == 0) {
			short preset = Dixom.Module.FmRadio.Settings.SelectedPreset;
			int compare = 0;
			if (direction == 1) {

				for (short count = 1; count < NUM_RADIO_PRESET - 1; count++) {
					preset++;
					if (preset > NUM_RADIO_PRESET - 1)
						preset = 1;
					compare =  Dixom.Module.FmRadio.FmPreset[preset].Freq * 1000;
					if (compare != 0) {
						SetRadioPreset(preset);
						count = NUM_RADIO_PRESET;
						FmRadio_Get_Curent_Preset();
					}
				}
			} else {
				for (short count = 0; count < NUM_RADIO_PRESET - 2; count++) {
					preset--;
					if (preset < 1)
						preset = NUM_RADIO_PRESET - 1;
					compare = Dixom.Module.FmRadio.FmPreset[preset].Freq * 1000;
					if (compare != 0) {
						SetRadioPreset(preset);
						count = NUM_RADIO_PRESET;
						FmRadio_Get_Curent_Preset();
					}
				}
			}
		} else {
			if (direction > 20)
				direction = 20;
			if (direction < 1)
				direction = 1;
			SetRadioPreset(direction);
			FmRadio_Get_Curent_Preset();
		}
	}
}

void Radio_Eeprom_NeedToSave(){
	dFm_Radio.NeedToSave = YES;
}

void Radio_Flash_NeedToSave(){

}

void FmRadio_Get_Curent_Preset(){
	sprintf((char *)Dixom.Buff, MaskGetRadioCurentPreset, Dixom.Module.FmRadio.Settings.SelectedPreset, Dixom.Module.FmRadio.FmPreset[0].Freq*FConv);
	TransmitDataOutputs(Dixom.Buff, AutoLenght,  false,  true, allAvailable);
	RADIO_GET_FREQ(All);
}


