/*
 * TemperatireControl.h
 *
 *  Created on: 28 мая 2023 г.
 *      Author: alekk
 */

#ifndef MODULE_AUDIODAC_TAS_3251X3_TEMPERATIRECONTROL_H_
#define MODULE_AUDIODAC_TAS_3251X3_TEMPERATIRECONTROL_H_

#define DEFAULT_HYS_TEMP 60
#define DEFAULT_OS_TEMP 70
#define MIN_TEMP 10
#define MAX_TEMP 90

void InitCrystalSixThermometer();
void ShowAmplifierTemperature();
void GetRealTemperature();
void CrystalSix140DTemperatureReadStream();
void CONFIG_CRYSTALSIX140D_THERMOMETER(uint8_t ControlByte, uint8_t *Received_String);

#endif /* MODULE_AUDIODAC_TAS_3251X3_TEMPERATIRECONTROL_H_ */
