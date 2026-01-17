/*
 * TemperatireControl.c
 *
 *  Created on: 28 мая 2023 г.
 *      Author: alekk
 */
#include <Structures.h>
#include "Description.h"
#include "CrystalSix140DExchange.h"
#include "DSP.h"
#include <CrystalSix140DThermometer.h>
#include <DSP_StringControl.h>

extern sDixom Dixom;
#define Amplifier  Dixom.Module.AudioDAC.TAS3251x3

#define LM75_REG_TEMP      		   	     0x00
#define LM75_REG_CONF      			     0x01
#define LM75_REG_T_HYS      	         0x02
#define LM75_REG_T_OS      			     0x03

typedef union {
	uint8_t Bites[2];
	int16_t Word;
} _TwoBites_int;

uint16_t oldTemp = 0;
uint16_t temReadDelayValue = 0;

void SetConfigTemperature (int8_t TempHysteresis, int8_t TempOS);
void ThermometerConfigRead();
void CurentTemperatureRead();
void GetConfigThermometer();
void GetTemperature();


void CrystalSix140DTemperatureReadStream(){
	if(Dixom.Module.AudioDAC.DAC_Type == AMPLIFIER_3xTAS3251){
		temReadDelayValue++;
		if(temReadDelayValue> 3000){
			temReadDelayValue = 0;
			CurentTemperatureRead();
		   if(oldTemp != Amplifier.TempControl.Temperature*10){
			   oldTemp = Amplifier.TempControl.Temperature*10;
			   GetTemperature();
		   }
		}
	}
}

void GetRealTemperature(){
	if(Dixom.Module.AudioDAC.DAC_Type == AMPLIFIER_3xTAS3251){
		CurentTemperatureRead();
		GetTemperature();
	}else{
 		TransmitDataOutputs((uint8_t *)GeneralNotifi_NotSupported, AutoLenght,  false,  true, allAvailable);

	}
}

void ShowAmplifierTemperature(){
	ThermometerConfigRead();
	GetConfigThermometer();
	CurentTemperatureRead();
	GetTemperature();
}

void GetTemperature(){
	sprintf((char*)Dixom.Buff, DspNotificationAmplifier ,
			Amplifier.TempControl.Temperature,
			Amplifier.TempControl.FanState);
	TransmitDataOutputs(Dixom.Buff, AutoLenght,  false,  true, allAvailable);

}

void GetConfigThermometer(){
	sprintf((char*)Dixom.Buff, DspTemperatureControl ,
			Amplifier.TempControl.TempHysteresis,
			Amplifier.TempControl.TempOS);
 	TransmitDataOutputs(Dixom.Buff, AutoLenght,  false,  true, allAvailable);

}

void CurentTemperatureRead(){
	uint8_t inData[2];
	_TwoBites_int Result_t;
	AmplifierReciever(I2C_PORT_3, Port3_THERMOMETER_LM75A, LM75_REG_TEMP, &inData[0], 2, ExchangeMemMainSteam);
	Result_t.Bites[1] = inData[0];
    Result_t.Bites[0] = inData[1];
	Amplifier.TempControl.Temperature = ((Result_t.Word>>7) & 0x1FF) * 0.5 ;
}

void ThermometerConfigRead(){
	uint8_t inData[2];
	AmplifierReciever(I2C_PORT_3, Port3_THERMOMETER_LM75A, LM75_REG_CONF, &inData[0], 1, ExchangeMemMainSteam);
	AmplifierReciever(I2C_PORT_3, Port3_THERMOMETER_LM75A, LM75_REG_T_HYS, &inData[0], 2, ExchangeMemMainSteam);
	Amplifier.TempControl.TempHysteresis = (int8_t) inData[0];
	AmplifierReciever(I2C_PORT_3, Port3_THERMOMETER_LM75A, LM75_REG_T_OS, &inData[0], 2, ExchangeMemMainSteam);
	Amplifier.TempControl.TempOS = (int8_t) inData[0];
}

void SetConfigTemperature (int8_t TempHysteresis, int8_t TempOS){
	uint8_t outData[2];
	outData[0] = TempHysteresis;
	outData[1] = 0x00;
	AmplifierTransmit(I2C_PORT_3, Port3_THERMOMETER_LM75A, LM75_REG_T_HYS, &outData[0], 2, ExchangeMemMainSteam);
	outData[0] = TempOS;
	outData[1] = 0x00;
	AmplifierTransmit(I2C_PORT_3, Port3_THERMOMETER_LM75A, LM75_REG_T_OS, &outData[0], 2, ExchangeMemMainSteam);
	GetConfigThermometer();
}

void InitCrystalSixThermometer(){
	Amplifier.TempControl.TempHysteresis = Dixom.Module.DSP.Settings[pTempHysteresis].Data;
	Amplifier.TempControl.TempOS = Dixom.Module.DSP.Settings[psTempOS].Data;
	uint8_t inData[2];
	inData[0] = 0x04;
	inData[1] = 0x00;
	AmplifierTransmit(I2C_PORT_3, Port3_THERMOMETER_LM75A, LM75_REG_CONF, &inData[0], 1, ExchangeMemMainSteam);
	inData[0] = Amplifier.TempControl.TempHysteresis;
	AmplifierTransmit(I2C_PORT_3, Port3_THERMOMETER_LM75A, LM75_REG_T_HYS, &inData[0], 2, ExchangeMemMainSteam);
	inData[0] = Amplifier.TempControl.TempOS;
	AmplifierTransmit(I2C_PORT_3, Port3_THERMOMETER_LM75A, LM75_REG_T_OS, &inData[0], 2, ExchangeMemMainSteam);
}

void CONFIG_CRYSTALSIX140D_THERMOMETER(uint8_t ControlByte, uint8_t *Received_String){
	if(Dixom.Module.AudioDAC.DAC_Type == AMPLIFIER_3xTAS3251){
	    if(ControlByte == DATA_SET){ //SET
			int8_t   TempHysteresis = DEFAULT_HYS_TEMP, TempOS = DEFAULT_OS_TEMP;
			sscanf((char *)Received_String, "%hhd %hhd", &TempHysteresis, &TempOS);
			if( (TempHysteresis<MIN_TEMP || TempHysteresis>MAX_TEMP) &&  (TempOS<MIN_TEMP || TempOS>MAX_TEMP) ){
				TransmitDataOutputs((uint8_t *)GeneralNotifi_CommandError, AutoLenght,  false,  true, allAvailable);

			}else{
				DSP_Settings(DATA_SET, pTempHysteresis, TempHysteresis);
				DSP_Settings(DATA_SET, psTempOS, TempOS);
				SetConfigTemperature (TempHysteresis, TempOS);
			}
	    }else if(ControlByte == DATA_DEFAULT){
			DSP_Settings(DATA_SET, pTempHysteresis, DEFAULT_HYS_TEMP);
			DSP_Settings(DATA_SET, psTempOS, DEFAULT_OS_TEMP);
			SetConfigTemperature (DEFAULT_HYS_TEMP, DEFAULT_OS_TEMP);
	    }else{
	    	ThermometerConfigRead();
	    	GetConfigThermometer();
	    }
	}else{
 		TransmitDataOutputs((uint8_t *)GeneralNotifi_CommandError, AutoLenght,  false,  true, allAvailable);

	}
}
