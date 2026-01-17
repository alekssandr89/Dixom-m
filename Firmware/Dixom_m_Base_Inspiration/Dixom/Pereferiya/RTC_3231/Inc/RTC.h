#ifndef RTC_H_
#define RTC_H_
#include "stdint.h"

void RTC_TIME(uint8_t ControlByte, uint8_t* dataIN);
void RTC_TEMPERATURE();
void RTC_GettingTime();
void RegisterToStruct( uint8_t Register, uint8_t *Struct);



typedef struct
{
	uint8_t  Seconds         :4;    // Seconds
	uint8_t  Seconds10       :3;    // 10 Seconds
	uint8_t  Reserved        :1;    // 0
} tSeconds;

typedef struct
{
	uint8_t  Minutes         :4;    // Minutes
	uint8_t  Minutes10       :3;    // Minutes
	uint8_t  Reserved        :1;    // 0
} tMinutes;

typedef struct
{
	uint8_t  Hour            :4;    // Hour
	uint8_t  Hour10          :1;    // 10 Hour
	uint8_t  Hour20          :1;    // 20 Hour && AM/PM
	uint8_t  set12_24        :1;    // 12/24
	uint8_t  Reserved        :1;    // 0
} tHours;

typedef struct
{
	uint8_t  Day             :3;    // Day
	uint8_t  Reserved        :5;    // 0
} tDay;

typedef struct
{
	uint8_t  Date            :4;    // Date
	uint8_t  Date10          :2;    // 10 Date
	uint8_t  Reserved        :2;    // 0
} tDate;

typedef struct
{
	uint8_t  Month           :4;    // Month
	uint8_t  Month10         :1;    // 10 Month
	uint8_t  Reserved        :2;    // 0
	uint8_t  Century         :1;    // Century
} tMonth;

typedef struct
{
	uint8_t  Year           :4;    // Master Power-Up Control.
	uint8_t  Year10         :4;    // Master Power-Up Control.
} tYear;

typedef struct
{
	uint8_t  Seconds         :4;    // Master Power-Up Control.
	uint8_t  Seconds10       :3;    // Master Clock Select
	uint8_t  A1M1            :1;    // Master Clock Select
} tAlarm1Seconds;

typedef struct
{
	uint8_t  Minutes         :4;    // Master Power-Up Control.
	uint8_t  Minutes10       :3;    // Master Clock Select
	uint8_t  A1M2            :1;    // Master Clock Select
} tAlarm1Minutes;

typedef struct
{
	uint8_t  Hour            :4;    // Master Power-Up Control.
	uint8_t  Hour10          :1;    // Master Clock Select
	uint8_t  Hour20          :1;    // Master Clock Select
	uint8_t  set12_24        :1;    // Master Clock Select
	uint8_t  A1M3            :1;    // Master Clock Select
} tAlarm1Hours;

typedef struct
{
	uint8_t  Day             :4;    // Master Power-Up Control.
	uint8_t  Date10          :2;    // Master Clock Select
	uint8_t  DYDT            :1;    // Master Clock Select
	uint8_t  A1M4            :1;    // Master Clock Select
} tAlarm1DayDate;

typedef struct
{
	uint8_t  Minutes         :4;    // Master Power-Up Control.
	uint8_t  Minutes10       :3;    // Master Clock Select
	uint8_t  A2M2            :1;    // Master Clock Select
} tAlarm2Minutes;

typedef struct
{
	uint8_t  Hour            :4;    // Master Power-Up Control.
	uint8_t  Hour10          :1;    // Master Clock Select
	uint8_t  Hour20          :1;    // Master Clock Select
	uint8_t  set12_24        :1;    // Master Clock Select
	uint8_t  A2M3            :1;    // Master Clock Select
} tAlarm2Hours;

typedef struct
{
	uint8_t  Day             :4;    // Master Power-Up Control.
	uint8_t  Date10          :2;    // Master Clock Select
	uint8_t  DYDT            :1;    // Master Clock Select
	uint8_t  A2M4            :1;    // Master Clock Select
} tAlarm2DayDate;

typedef struct
{
	uint8_t  A1IE            :1;    // Master Power-Up Control.
	uint8_t  A2IE            :1;    // Master Clock Select
	uint8_t  INTCN           :1;    // Master Clock Select
	uint8_t  RS1             :1;    // Master Clock Select
	uint8_t  RS2             :1;    // Master Power-Up Control.
	uint8_t  CONV            :1;    // Master Clock Select
	uint8_t  BBSQW           :1;    // Master Clock Select
	uint8_t  EOSC            :1;    // Master Clock Select
} tControl;

typedef struct
{
	uint8_t  A1F             :1;    // Master Power-Up Control.
	uint8_t  A2F             :1;    // Master Clock Select
	uint8_t  BSY             :1;    // Master Clock Select
	uint8_t  EN32kHz         :1;    // Master Clock Select
	uint8_t  Reserved        :3;    // Master Clock Select
	uint8_t  OSF             :1;    // Master Clock Select
} tControlStatus;

typedef struct
{
	uint8_t  DATA            :7;    // Master Power-Up Control.
	uint8_t  SIGN            :1;    // Master Clock Select
} tAgingOffset;

typedef struct
{
	uint8_t  DATA            :7;    // Master Power-Up Control.
	uint8_t  SIGN            :1;    // Master Clock Select
} tMSBTemp;

typedef struct
{
	uint8_t  Reserved        :6;    // Master Power-Up Control.
	uint8_t  DATA            :2;    // Master Clock Select
} tLSBTemp;

typedef struct
{
	tSeconds        Seconds;
	tMinutes        Minutes;
	tHours          Hours;
	tDay            Day;
	tDate           Date;
	tMonth          Month;
	tYear           Year;
	tMSBTemp        MSBTemp;
	tLSBTemp        LSBTemp;
	float           Chip_Temperature;
} sRegisterRtc;

typedef struct
{
	uint8_t         Seconds;
	uint8_t         Minutes;
	uint8_t         Hours;
	uint8_t         Day;
	uint8_t         Date;
	uint8_t         Month;
	uint16_t        Year;
	sRegisterRtc    RTC3231_Register;
} sDateTime;





#endif /* RTC_H_ */
