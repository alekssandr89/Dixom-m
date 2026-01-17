#include <DSP_InternalCalls.h>
#include <DSP_Exchange.h>
#include <DSP_Calculations.h>
#include <Memory_Eeprom.h>
#include <Memory_Stm.h>
#include <Structures.h>
#include <Sigma.h>

extern sDixom Dixom;

#define dDSP Dixom.Module.DSP


WorkRes DSP_Struct_Flash_Read(){
	Dixom.Service.PeripheryState[SPI] = 1;
	FlashReadCRC_Check(&Dixom.Service.CheckSum.RX, FLASH_ADDR_DSP, (uint32_t *)&dDSP, sizeof(dDSP)/4);
	dDSP.StructInfo.Size = sizeof(dDSP);
	if(Dixom.Service.CheckSum.RX == dDSP.CheckSum){
		Dixom.Service.Status.Struct[Module_DSP_enum] = ReadyForWork;
		return WR_OK;
	}else{
		DSP_Struct_Default();
		return WR_ERROR;
	}
}


WorkRes DSP_Struct_EEPROM_Read(){

	uint32_t    Rx_CheckSum;
	sDSP_EEPROM DSP_EEPROM;

	EEPROM_Read_CheckSumm(&Rx_CheckSum, EEPROM_ADDR_DSP, (uint8_t *)&DSP_EEPROM, sizeof(DSP_EEPROM), 300);

	if(Rx_CheckSum == DSP_EEPROM.CheckSum){

		for(uint8_t volume = 0; volume<NUM_ELEM_VOL_EEPROM; volume++ ){
			dDSP.Volume[volume].DataVol = DSP_EEPROM.Volume[volume];
		}
		dDSP.Settings[EqStandardPresets].Data = DSP_EEPROM.EqStandardPresets;
		dDSP.Settings[UserPresset].Data = DSP_EEPROM.UserPresset;
		dDSP.Settings[SoundSource].Data = DSP_EEPROM.SoundSource;

		Dixom.Service.Status.Struct[EEPROM_DSP_enum] = ReadyForWork;
		return WR_OK;

	}else{
		Dixom.Service.Status.Struct[EEPROM_DSP_enum] = SafeMode;
		return WR_ERROR;
	}
}


WorkRes DSP_Module_Initialization_Final(){

	if(dDSP.Settings[UserPresset].Data != Main_Preset && dDSP.Settings[UserPresset].Data <COUNT_USER_PRESET-1){
		dDSP.Preset.Data[Main_Preset] = dDSP.Preset.Data[dDSP.Settings[UserPresset].Data];
	}

	dDSP.StructInfo.NeedToSave = Not_Save;

	return WR_OK;
}

void DSP_Flash_NeedToSave(){
	Save_Structure_When_Power_Off();
}


void DSP_EEPROM_NeedToSave(){
	dDSP.StructInfo.NeedToSave = YES;
}

WorkRes DSP_Struct_Flash_Write(){

	WorkRes Result = WR_OK;

	Result = FlashWriteCRC_Check( &dDSP.CheckSum, FLASH_ADDR_DSP, (void *)&dDSP, sizeof(dDSP)/4);


	return Result;
}


WorkRes DSP_Struct_EEPROM_Write(){

	WorkRes State = WR_OK;

	sDSP_EEPROM DSP_EEPROM;

	if(dDSP.StructInfo.NeedToSave == YES){

		for(uint8_t volume = 0; volume<NUM_ELEM_VOL_EEPROM; volume++ ){
			 DSP_EEPROM.Volume[volume] = dDSP.Volume[volume].DataVol;
		}
		DSP_EEPROM.EqStandardPresets = dDSP.Settings[EqStandardPresets].Data;
	    DSP_EEPROM.UserPresset = dDSP.Settings[UserPresset].Data;
		DSP_EEPROM.SoundSource = dDSP.Settings[SoundSource].Data;


		dDSP.StructInfo.NeedToSave = Not_Save;

		EEPROM_Write_CheckSumm(&DSP_EEPROM.CheckSum, EEPROM_ADDR_DSP, (uint8_t *)&DSP_EEPROM, sizeof(DSP_EEPROM), 300);
	}

	return State;
}


WorkRes DSP_Struct_Chip_Init(){

	DSP_Chip_Init(NO);
	return WR_OK;
}


void DSP_Loop(){
	if(Dixom.Service.PeripheryState[SPI] == 1){
		LoopLevelDetector();
	}
}






