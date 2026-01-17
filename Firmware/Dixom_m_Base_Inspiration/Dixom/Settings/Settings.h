/*
 * Settings.h
 *
 *  Created on: 8 но€б. 2021 г.
 *      Author: 79825
 */
#ifndef SETTINGS_SETTINGS_H_
#define SETTINGS_SETTINGS_H_

#include <Settings_Constant.h>
#include "stdint.h"

void             SETTINGS(uint8_t ControlByte, uint8_t *Recive_USB);
void             GET_ALL_SETTINGS(uint8_t AllOrModified);
void             SETTINGS_SHOW_PARAMETERS(uint8_t AllOrModified);

void             SET_NS_SETTINGS(uint8_t ControlByte, uint8_t *Recive_USB);
void             TIMER(uint8_t ControlByte, uint8_t *Recive_USB);
void             TIMER_SHOW_PARAMETERS(uint8_t AllOrModified);


WorkRes          Settings_Initialization_Final();
WorkRes          Settings_Struct_EEPROM_Read();
WorkRes          Settings_Struct_EEPROM_Write();
WorkRes          Settings_Struct_Flash_Read();
WorkRes          Settings_Struct_Flash_Write();
WorkRes          Settings_Struct_Default();
WorkRes          Settings_Default();
WorkRes          Timer_Default();
void             StructSettingsInit();
void             StructSettingsDefault();
void             StructSettingsWrite();
void             SetTimer(uint16_t point, uint32_t data);
uint32_t         GetTimer(uint16_t point);
void             SetSettings(uint16_t point, uint8_t data);
uint8_t          GetSettings(uint16_t point);
void             ActionSettings(uint8_t point, uint8_t data);
void             ActionSettingsStateSoundCard(uint8_t point, uint8_t data);

typedef struct
{
	uint8_t element;
} sParametr;

typedef struct
{
	uint32_t element;
} sTimer;

typedef struct
{
	uint8_t element;
} sParamNoSaving;

typedef struct
{
	uint8_t element;
} sTimerNoSaving;

typedef struct
{
	uint32_t        CheckSum;
	sStructInfo     StructInfo;
	sTimer          Timer[UNIQ_ELEM_TIMER];
	sParametr       Param[UNIQ_ELEM_SETTINGS];
	sParamNoSaving  ParamNoSave[UNIQ_ELEM_SETTINGS_NoSave];
	sTimerNoSaving  TimerNoSave[UNIQ_ELEM_TIMER_NoSave];
} sSettings;

#endif /* SETTINGS_SETTINGS_H_ */
