/*
 * FmRadio_StringControl.h
 *
 *  Created on: 13 ����. 2022 �.
 *      Author: 79825
 */

#ifndef MODULE_FMRADIO_FMRADIO_CONTROL_H_
#define MODULE_FMRADIO_FMRADIO_CONTROL_H_

#include "stdint.h"

void RADIO_SET_FREQ(uint8_t ControlByte, uint8_t *Received_String);
void RADIO_SET_AUTO_SEARCH(uint8_t ControlByte, uint8_t *Received_String);
void RADIO_SET_MANUAL_SEARCH(uint8_t ControlByte, uint8_t *Received_String);
void RADIO_SET_SETTINGS(uint8_t ControlByte, uint8_t *Received_String);
void RADIO_SET_PRESET(uint8_t ControlByte, uint8_t *Received_String);
void RADIO_SET_CURRENT_PRESET(uint8_t ControlByte, uint8_t *Received_String);
void RADIO_SET_SAVE_PRESET(uint8_t ControlByte, uint8_t *Received_String);
void RADIO_SET_AUTO_SAVE_PRESET();
void RADIO_SET_DELETE_PRESET(uint8_t ControlByte, uint8_t *Received_String);

void RADIO_GET_FREQ(uint8_t All_Or_Modified);
void RADIO_GET_SETTINGS(uint8_t All_Or_Modified);
void RADIO_GET_TABLE_PRESET(uint8_t All_Or_Modified);
void RADIO_GET_CURRENT_PRESET(uint8_t All_Or_Modified);
void RADIO_GET_MODULE_INFO();
void RADIO_GET_SERVICE_INFORMATION();



void FmRadio_Set_Frequency(uint16_t Freq);
void FmRadio_Set_Auto_Search(uint16_t Direction);
void FmRadio_Set_Marual_Search(uint16_t Direction);
void FmRadio_Set_Settings();
void FmRadio_Preset_Routing(uint8_t action, short direction, uint8_t pressed);
void FmRadio_Get_Curent_Preset();
void Radio_Eeprom_NeedToSave();

#endif /* MODULE_FMRADIO_FMRADIO_CONTROL_H_ */
