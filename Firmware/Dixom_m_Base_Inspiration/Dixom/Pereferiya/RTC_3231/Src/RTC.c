#include <Description.h>
#include "RTC.h"
#include "fonts.h"
#include "Exchange.h"
#include <stdio.h>
#include <Structures.h>

#define ConstYear 2020
extern sDixom Dixom;
#define DateTime Dixom.DevInfo.DateTime


void RTC_TIME(uint8_t ControlByte, uint8_t* dataIN){
	sDateTime NewDataTime;
	NewDataTime.Hours = DateTime.Hours,
	NewDataTime.Minutes = DateTime.Minutes,
	NewDataTime.Seconds = DateTime.Seconds,
	NewDataTime.Day = DateTime.Day,
	NewDataTime.Date = DateTime.Date,
	NewDataTime.Month = DateTime.Month,
	NewDataTime.Year = DateTime.Year,

	sscanf((char *)dataIN, "%hhu %hhu %hhu %hhu %hhu %hhu %hu",
			&NewDataTime.Hours,
			&NewDataTime.Minutes,
			&NewDataTime.Seconds,
			&NewDataTime.Day,
			&NewDataTime.Date,
			&NewDataTime.Month,
			&NewDataTime.Year);
    if(ControlByte == DATA_SET){ //SET
    	if(NewDataTime.Hours > 59 || NewDataTime.Hours < 0 || NewDataTime.Minutes > 59 || NewDataTime.Minutes < 0 || NewDataTime.Seconds > 59 || NewDataTime.Seconds < 0 ||
    			NewDataTime.Day > 7      || NewDataTime.Day < 1 || NewDataTime.Date > 31    || NewDataTime.Date < 1	|| NewDataTime.Month > 12   || NewDataTime.Month < 1 ||
				NewDataTime.Year > 2100  || NewDataTime.Year < ConstYear){
    		TransmitDataOutputs((uint8_t *)GeneralNotifi_CommandError, AutoLenght,  false,  true, allAvailable);
    		return;
    	}else{
    		NewDataTime.RTC3231_Register.Seconds.Seconds = NewDataTime.Seconds % 10;
    		NewDataTime.RTC3231_Register.Seconds.Seconds10 = NewDataTime.Seconds / 10;

    		NewDataTime.RTC3231_Register.Minutes.Minutes = NewDataTime.Minutes % 10;
    		NewDataTime.RTC3231_Register.Minutes.Minutes10 = NewDataTime.Minutes / 10;

    		NewDataTime.RTC3231_Register.Hours.Hour = NewDataTime.Hours % 10;
    		NewDataTime.RTC3231_Register.Hours.Hour10 = NewDataTime.Hours / 10;

    		NewDataTime.RTC3231_Register.Day.Day = NewDataTime.Day;

    		NewDataTime.RTC3231_Register.Date.Date = NewDataTime.Date % 10;
    		NewDataTime.RTC3231_Register.Date.Date10 = NewDataTime.Date / 10;

    		NewDataTime.RTC3231_Register.Month.Month = NewDataTime.Month % 10;
    		NewDataTime.RTC3231_Register.Month.Month10 = NewDataTime.Month / 10;

    		NewDataTime.RTC3231_Register.Year.Year = (NewDataTime.Year - ConstYear)  % 10;
    		NewDataTime.RTC3231_Register.Year.Year10 = (NewDataTime.Year - ConstYear) / 10;

    		ExchangeStatus(Transmit_I2C2((uint16_t)DEVICE_ADDR_RTC_DS3231, 0, 1, (uint8_t*)&NewDataTime.RTC3231_Register, 7, 200, ExchangeMemMainSteam), RTC_WRITE);
    		RTC_GettingTime();
    	}
	}else if(ControlByte == DATA_GET){ //GET
    	sprintf( (char *)Dixom.Buff, macros_rtc_data_time,
    			DateTime.Hours,
    			DateTime.Minutes,
    			DateTime.Seconds,
    			DateTime.Day,
    			DateTime.Date,
    			DateTime.Month,
    			DateTime.Year);

    	TransmitDataOutputs(Dixom.Buff, AutoLenght,  false,  true, allAvailable);
    }else{
    	TransmitDataOutputs((uint8_t *)GeneralNotifi_CtlBitNotSupported, AutoLenght,  false,  true, allAvailable);
    }
}

void RTC_TEMPERATURE(){
	ExchangeStatus(	Receiver_I2C2((uint16_t)DEVICE_ADDR_RTC_DS3231, 17, 1, (uint8_t*) &DateTime.RTC3231_Register.MSBTemp, 2, 200, ExchangeMemMainSteamDMA), RTC_READ);
	DateTime.RTC3231_Register.Chip_Temperature = DateTime.RTC3231_Register.MSBTemp.DATA + DateTime.RTC3231_Register.LSBTemp.DATA*0.25;
	sprintf( (char *)Dixom.Buff, macros_rtc_temperature, DateTime.RTC3231_Register.Chip_Temperature);
 	TransmitDataOutputs(Dixom.Buff, AutoLenght,  false,  true, allAvailable);
}

void RTC_GettingTime() {
	ExchangeStatus(	Receiver_I2C2((uint16_t)DEVICE_ADDR_RTC_DS3231, 0, 1, (uint8_t*)&DateTime.RTC3231_Register, 7, 200, ExchangeMemMainSteamDMA), RTC_READ);

	DateTime.Seconds = DateTime.RTC3231_Register.Seconds.Seconds10*10 + DateTime.RTC3231_Register.Seconds.Seconds;
	DateTime.Minutes = DateTime.RTC3231_Register.Minutes.Minutes10*10 + DateTime.RTC3231_Register.Minutes.Minutes;
	DateTime.Hours   = DateTime.RTC3231_Register.Hours.Hour20*20 + DateTime.RTC3231_Register.Hours.Hour10*10 + DateTime.RTC3231_Register.Hours.Hour;
	DateTime.Day     = DateTime.RTC3231_Register.Day.Day;
	DateTime.Date    = DateTime.RTC3231_Register.Date.Date10*10 + DateTime.RTC3231_Register.Date.Date;
	DateTime.Month   = DateTime.RTC3231_Register.Month.Month10*10 + DateTime.RTC3231_Register.Month.Month;
	DateTime.Year    = DateTime.RTC3231_Register.Year.Year10*10 + DateTime.RTC3231_Register.Year.Year + ConstYear;
}

void RegisterToStruct( uint8_t Register, uint8_t *Struct){
	*Struct = Register;
}





