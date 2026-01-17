/*
 * FmRadio.c
 *
 *  Created on: 7 ����. 2021 �.
 *      Author: 79825
 */

#include <Description.h>
#include <FmRadio_Control.h>
#include <RDA_5807.h>
#include <Structures.h>
#include "Exchange.h"
#include "String.h"
#include <Memory_Eeprom.h>


extern sDixom Dixom;
extern sSiRadio Si47xx;

#define Fm_Radio Dixom.Module.FmRadio

///////////////////////////////////////////////////////// STRING CONTROL RADIO MENU /////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////    SET PARAMETERS IN RADIO    ///////////////////////////////////////////////////////////////////////////////////
void RADIO_SET_STRING_CONTROL(uint8_t ControlByte, uint8_t Function, uint8_t *Received_String){

	if(Dixom.Module.FmRadio.RadioModule != NO_RADIO_CHIP){

		if(Function == RADIO_FREQ)                  { RADIO_SET_FREQ(ControlByte, Received_String);       } // OK Save EEPROM //��������� ������� FM: 870...1080
		else if(Function == RADIO_AUTO_SEARCH)      { RADIO_SET_AUTO_SEARCH(ControlByte, Received_String);    } // OK Save EEPROM //�������������� ����� ����� �������
		else if(Function == RADIO_MANUAL_SEARCH)    { RADIO_SET_MANUAL_SEARCH(ControlByte, Received_String);  } // OK Save EEPROM //������ ����� ����� �������
		else if(Function == RADIO_SETTINGS)         { RADIO_SET_SETTINGS(ControlByte, Received_String);     	} // OK Save EEPROM //��������� ����� ����
		else if(Function == RADIO_TABLE_PRESET)     { RADIO_SET_PRESET(ControlByte, Received_String);         } // OK Save EEPROM ����� �������
		else if(Function == RADIO_DELETE_PRESET)    { RADIO_SET_DELETE_PRESET(ControlByte, Received_String);  } // OK Save EEPROM ������� ������ �� ��������� ������
		else if(Function == RADIO_SAVE_PRESET)      { RADIO_SET_SAVE_PRESET(ControlByte, Received_String);    } // OK Save EEPROM ��������� ������ � ��������� ������
		else if(Function == RADIO_AUTO_SAVE_PRESET) { RADIO_SET_AUTO_SAVE_PRESET();                 	} // OK Save EEPROM �������� ������ � ���������� ��������� ������
		else if(Function == RADIO_DELETE_PRESET)    { RADIO_SET_DELETE_PRESET(ControlByte, Received_String);  } // OK Save EEPROM ������� ������ �� ��������� ������
		else if(Function == RADIO_CURRENT_PRESET)   { RADIO_SET_CURRENT_PRESET(ControlByte, Received_String); } // OK Save EEPROM ������� ������ �� ��������� ������
		else if(Function == RADIO_CHIP)             { RADIO_GET_MODULE_INFO();                          } // ������� ���������� � ������
		else if(Function == RADIO_GET_ALL_MODIFIED) { FM_RADIO_SHOW_PARAMETERS(Modified);           	} // ������� ��������� ����������
		else if(Function == RADIO_MESSAGE)          { RADIO_GET_SERVICE_INFORMATION();                  } // ������� ��������� ����������
		else{
			TransmitDataOutputs((uint8_t *)GeneralNotifi_NoPackageFunction, AutoLenght,  false,  true, allAvailable);
		}
	}else{
 		TransmitDataOutputs((uint8_t *)GeneralNotifi_NotFound, AutoLenght,  false,  true, allAvailable);

	}
}


WorkRes FM_Radio_EEPROM_Read(){

	uint32_t    Rx_CheckSum;

	Fm_Radio.DataSaveSize = sizeof(Fm_Radio.Settings)+sizeof(Fm_Radio.FmPreset)+sizeof(Fm_Radio.AmPreset);

	EEPROM_Read_CheckSumm(&Rx_CheckSum, EEPROM_ADDR_RADIO, (uint8_t *)&Fm_Radio, Fm_Radio.DataSaveSize, 300);

	if(Rx_CheckSum == Fm_Radio.CheckSum){

		Dixom.Service.Status.Struct[EEPROM_DSP_enum] = ReadyForWork;
		return WR_OK;

	}else{
		FM_Radio_Default();
		Dixom.Service.Status.Struct[EEPROM_RADIO_enum] = SafeMode;
		return WR_ERROR;
	}
}

WorkRes FM_Radio_EEPROM_Write(){

	WorkRes State = WR_OK;

	if(Fm_Radio.NeedToSave == YES){
		Fm_Radio.NeedToSave = NO;
		EEPROM_Write_CheckSumm(&Fm_Radio.CheckSum, EEPROM_ADDR_RADIO, (uint8_t *)&Fm_Radio, Fm_Radio.DataSaveSize, 300);
	}

    return State;
}

WorkRes FM_Radio_Default(){

    for (short point = 0; point<NUM_RADIO_PRESET; point++){
    	Fm_Radio.FmPreset[point].Freq = DEFAULT_RADIO_PRESET_FREQ;
    	Fm_Radio.AmPreset[point].Freq = DEFAULT_RADIO_PRESET_FREQ;
    	Fm_Radio.FmPreset[point].editing = YES;
    	Fm_Radio.AmPreset[point].editing = YES;
    }
	Fm_Radio.FmPreset[0].Freq = DEFAULT_RADIO_FM_FREQ;
	Fm_Radio.AmPreset[0].Freq = DEFAULT_RADIO_AM_FREQ;

	Fm_Radio.Settings.AutoData = DEFAULT_RADIO_AUDIO_DATA;
	Fm_Radio.Settings.Mode_AM_FM = DEFAULT_RADIO_MODE_AM_FM;
	Fm_Radio.Settings.PTY = DEFAULT_RADIO_PTY;
	Fm_Radio.Settings.RDS = DEFAULT_RADIO_RDS;
	Fm_Radio.Settings.STEREO = DEFAULT_RADIO_STEREO;
	Fm_Radio.Settings.SelectedPreset = DEFAULT_RADIO_PRESET;
	Fm_Radio.Settings.TA = DEFAULT_RADIO_TA;
	Fm_Radio.Settings.editing = NO;
	Fm_Radio.Settings.Spasing = DEFAULT_RADIO_SPASING;
	Fm_Radio.Settings.Band = DEFAULT_RADIO_BAND;
	Fm_Radio.Settings.reserved0 = 0;
	Fm_Radio.Settings.reserved1 = 0;
	Fm_Radio.Settings.reserved2 = 0;
	Fm_Radio.NeedToSave = YES;

	return WR_OK;
}


void Automatic_Detection_FM_Radio_And_Initialization(){

	if (DevState_I2C1(DEVICE_ADDR_RADIO_RDA5807<<1, 1, 200) == WR_OK) {
		Dixom.Module.FmRadio.RadioModule = RDA5807FP;
		StructRadioInit_rda5807();
	}else if(DevState_I2C1(DEVICE_ADDR_RADIO_SI4735<<1, 1, 200) == WR_OK) {
		Dixom.Module.FmRadio.RadioModule = SI4741;
		Init_SI47xx();
	}else{
		Dixom.Module.FmRadio.InMode = NotFound;
		Dixom.Module.FmRadio.RadioModule = NO_RADIO_CHIP;
	}
}


void FM_RADIO_SHOW_PARAMETERS(uint8_t ShowType){

	if(Dixom.Module.FmRadio.RadioModule != NO_RADIO_CHIP){
		RADIO_GET_MODULE_INFO();
		RADIO_GET_FREQ(All);
		RADIO_GET_TABLE_PRESET(ShowType);
		RADIO_GET_CURRENT_PRESET(ShowType);
		RADIO_GET_SETTINGS(ShowType);
	}
}


void FM_Radio_Loop(){
	if(Dixom.Module.FmRadio.RadioModule == RDA5807FP){
		RadioAutoSearchTimer();
	}
}


void FM_Radio_Execution_With_Time_Interval_2000ms(){

	if(Fm_Radio.Settings.AutoData == ON){

		if(Dixom.Module.FmRadio.RadioModule == SI4741 &&  Dixom.Module.FmRadio.InMode == ReadyForWork){
			GetRsqStatus(YES);
		}
	}
}



