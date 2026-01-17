#include <Description.h>
#include <Structures.h>
#include <Memory_Stm.h>
#include "Settings_Default.h"
#include "Exchange.h"
#include "stdio.h"
#include "GPIO.h"

extern sDixom Dixom;
#define dSettings Dixom.Basis.Settings

WorkRes Settings_Struct_Flash_Read(){
	FlashReadCRC_Check(&Dixom.Service.CheckSum.RX, FLASH_ADDR_SETTINGS, (uint32_t *)&dSettings, sizeof(dSettings)/4);
	dSettings.StructInfo.Size = sizeof(dSettings);
	if(Dixom.Service.CheckSum.RX == dSettings.CheckSum){
		Dixom.Service.Status.Struct[Settings_enum] = ReadyForWork;
		return WR_OK;
	}else{
		Settings_Struct_Default();
		return WR_ERROR;
	}
}


WorkRes Settings_Struct_EEPROM_Read(){

	return WR_OK;
}


WorkRes Settings_Initialization_Final(void){

	Dixom.Exchange.Interface.Interface_selection_mode = dSettings.Param[Setting_current_selection_mode].element;
	dSettings.ParamNoSave[SettingsNoSave_Task_Execution_State].element = ON;
	Dixom.Exchange.Interface.Manual_tx_usb_hid = dSettings.Param[Setting_manual_tx_usb_hid].element;
	Dixom.Exchange.Interface.Manual_tx_usb_cdc = dSettings.Param[Setting_manual_tx_usb_cdc].element;
	Dixom.Exchange.Interface.Manual_tx_bluetooth = dSettings.Param[Setting_manual_tx_usb_bluetooth].element;


	return WR_OK;
}


WorkRes Settings_Struct_Flash_Write(void){

	FlashWriteCRC_Check( &dSettings.CheckSum, FLASH_ADDR_SETTINGS, (void *)&dSettings, sizeof(dSettings)/4);

	return Settings_Struct_Flash_Read();
}

WorkRes Settings_Struct_EEPROM_Write(){
	return WR_OK;
}

WorkRes Timer_Default(){

	Dixom.Service.Status.Struct[Settings_enum] = DefaultStruct;
	Save_Structure_When_Power_Off();

    for (short point = 0; point<UNIQ_ELEM_TIMER; point++){
    	dSettings.Timer[point].element = DEFAULT_TIMER[point];
    }

	return WR_OK;
}

WorkRes Settings_Default(){

	Dixom.Service.Status.Struct[Settings_enum] = DefaultStruct;
	Save_Structure_When_Power_Off();

    for (short point = 0; point<UNIQ_ELEM_SETTINGS; point++){
    	SetSettings(point, DEFAULT_SETTINGS[point]);
    }
    Settings_Initialization_Final();

	return WR_OK;
}

WorkRes Settings_Struct_Default(){

	Timer_Default();
	Settings_Default();

	return WR_OK;
}


void TIMER(uint8_t ControlByte, uint8_t *Recive_USB){

	short      point=0;
    uint32_t   data=0;

    sscanf((char *)Recive_USB, "%hu %ld", &point, &data);

    if(point<0 || point>UNIQ_ELEM_TIMER ){

    	TransmitDataOutputs((uint8_t *)GeneralNotifi_CommandError, AutoLenght,  false,  true, allAvailable);

    }else{
        if(ControlByte == DATA_SET){ //SET
        	if(data<0 || data>4294967294 ){
             	TransmitDataOutputs((uint8_t *)GeneralNotifi_CommandError, AutoLenght,  false,  true, allAvailable);

        	}else{
        		SetTimer( point, data );
        	}
        }else if(ControlByte == DATA_GET_ALL){
        	TIMER_SHOW_PARAMETERS(All);
        }else if(ControlByte == DATA_GET_MODIFIED){
        	TIMER_SHOW_PARAMETERS(Modified);
        }else if(ControlByte == DATA_DEFAULT){
        	Timer_Default();
        }else if(ControlByte == DATA_GET){ //GET
        	sprintf( (char*)Dixom.Buff, Basis_Timer_1_12_0,point, dSettings.Timer[point]);
         	TransmitDataOutputs(Dixom.Buff, AutoLenght,  false,  true, allAvailable);

        }else{
         	TransmitDataOutputs((uint8_t *)GeneralNotifi_CtlBitNotSupported, AutoLenght,  false,  true, allAvailable);

        }
    }
}

void  TIMER_SHOW_PARAMETERS(uint8_t AllOrModified){

	for(uint8_t point = 0; point < UNIQ_ELEM_TIMER; point ++){
		if(AllOrModified == All || GetTimer( point ) != DEFAULT_TIMER[point]){
			sprintf( (char *) Dixom.Buff, Basis_Timer_1_12_0,point, GetTimer( point ));
         	TransmitDataOutputs(Dixom.Buff, AutoLenght,  false,  true, allAvailable);
		}
	}
}

void SETTINGS(uint8_t ControlByte, uint8_t *Recive_USB){

	short      point=0;
	short      data=0;

    sscanf((char *)Recive_USB, "%hu %hu", &point, &data);

    if(point<0 || point>UNIQ_ELEM_SETTINGS){
       	TransmitDataOutputs((uint8_t *)GeneralNotifi_CommandError, AutoLenght,  false,  true, allAvailable);
    }else{
        if(ControlByte == DATA_SET){ //SET
        	if(data<0 || data>255 ){
               	TransmitDataOutputs((uint8_t *)GeneralNotifi_CommandError, AutoLenght,  false,  true, allAvailable);

        	}else{
        		ActionSettings(point, data);
              	TransmitDataOutputs((uint8_t *)GeneralNotifi_CommandOk, AutoLenght,  false,  true, allAvailable);

        	}
        }else if(ControlByte == DATA_GET_ALL){
        	SETTINGS_SHOW_PARAMETERS(All);
        }else if(ControlByte == DATA_GET_MODIFIED){
        	SETTINGS_SHOW_PARAMETERS(Modified);
        }else if(ControlByte == DATA_DEFAULT){
        	Settings_Default();
        }else if(ControlByte == DATA_GET){ //GET
    		sprintf( (char *) Dixom.Buff, Basis_Settings_1_2_0 ,point, GetSettings( point ));
         	TransmitDataOutputs(Dixom.Buff, AutoLenght,  false,  true, allAvailable);
        }else{
          	TransmitDataOutputs((uint8_t *)GeneralNotifi_CtlBitNotSupported, AutoLenght,  false,  true, allAvailable);

        }
    }
}

void SETTINGS_SHOW_PARAMETERS(uint8_t AllOrModified){

	for(uint16_t point = 0; point < UNIQ_ELEM_SETTINGS; point ++){
		if(AllOrModified == All || GetSettings( point ) != DEFAULT_SETTINGS[point]){
			sprintf( (char *) Dixom.Buff, Basis_Settings_1_2_0, point, Dixom.Basis.Settings.Param[point].element);
         	TransmitDataOutputs(Dixom.Buff, AutoLenght,  false,  true, allAvailable);
		}
	}
}

void SET_NS_SETTINGS(uint8_t ControlByte, uint8_t *Recive_USB){

	short      point=0;
	short      data=0;

    sscanf((char *)Recive_USB, "%hu %hu", &point, &data);

    if(point<0 || point>UNIQ_ELEM_SETTINGS){
      	TransmitDataOutputs((uint8_t *)GeneralNotifi_CommandError, AutoLenght,  false,  true, allAvailable);

    }else{
        if(ControlByte == DATA_SET){ //SET
        	Dixom.Basis.Settings.ParamNoSave[point].element = data;
        }else if(ControlByte == DATA_GET_ALL){
        	//GET_ALL_SETTINGS(All);
        }else if(ControlByte == DATA_GET_MODIFIED){
        	//GET_ALL_SETTINGS(Modified);
        }else if(ControlByte == DATA_DEFAULT){
        	//Settings_Default();
        }else{ //GET
    		sprintf( data, Basis_Settings_1_2_1, point, Dixom.Basis.Settings.ParamNoSave[point].element);

         	TransmitDataOutputs((uint8_t *)data, AutoLenght,  false,  true, allAvailable);

        }
    }
}






void SetTimer(uint16_t point, uint32_t data){
	dSettings.Timer[point].element = data;
}

uint32_t GetTimer(uint16_t point){
	return dSettings.Timer[point].element;
}

void SetSettings(uint16_t point, uint8_t data){

	dSettings.Param[point].element = data;

	if(point == SettingsDisplayBrightness){
		Dixom.Module.Display.Brightness = data;
	}

	Save_Structure_When_Power_Off();
}

uint8_t GetSettings(uint16_t point){
	return dSettings.Param[point].element;
}


void ActionSettings(uint8_t point, uint8_t data){

	switch (point){
	    case SettingsStateSoundCard            :  ActionSettingsStateSoundCard(point, data);                  break;
	    case SettingsRemLowVoltageVal          :                                                              break;
	    case SettingsDisplayBrightness         :  SetBrightness(data);                                        break;
	    case Setting_manual_tx_usb_hid         :  Dixom.Exchange.Interface.Manual_tx_usb_hid = data;          break;
	    case Setting_manual_tx_usb_cdc         :  Dixom.Exchange.Interface.Manual_tx_usb_cdc = data;          break;
	    case Setting_manual_tx_usb_bluetooth   :  Dixom.Exchange.Interface.Manual_tx_bluetooth = data;        break;
	    case Setting_current_selection_mode    :  Dixom.Exchange.Interface.Interface_selection_mode = data;   break;
	    case SettingsModeOnUSB1                :  if(data == ON || data == OFF) Power_USB1(data);             break;
	    case SettingsModeOnUSB2                :  if(data == ON || data == OFF) Power_USB2(data);             break;




	}
	SetSettings( point, data );
}


void ActionSettingsStateSoundCard(uint8_t point, uint8_t data){
	if(data==ON){
		Transmit_UsbSoundCard((uint8_t*)"3", DirectionTypeWrite);
		Delay(500, FreeRTOS);
		Update_AudioCard_Information(YES);
	}else{
		//Dixom.Module.UsbAudio.InMode = Mode_BootLoader;
		//Delay(5, FreeRTOS);
		Transmit_UsbSoundCard((uint8_t *)"RESET", DirectionTypeWrite);

	}
}




























