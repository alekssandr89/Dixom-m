/*
 * FmRadio.h
 *
 *  Created on: 7 нояб. 2021 г.
 *      Author: 79825
 */

#ifndef MODULE_FMRADIO_FMRADIO_H_
#define MODULE_FMRADIO_FMRADIO_H_

#include "SI47xx.h"


void RADIO_SET_STRING_CONTROL(uint8_t ControlByte, uint8_t Function, uint8_t *Received_String);



void           FM_Radio_Loop();
void           RADIO(uint8_t *Received_String);
WorkRes        FM_Radio_EEPROM_Read();
WorkRes        FM_Radio_EEPROM_Write();
WorkRes        FM_Radio_Default();
void           Automatic_Detection_FM_Radio();
void           FM_Radio_Execution_With_Time_Interval_2000ms();
void           Automatic_Detection_FM_Radio_And_Initialization();
void           FM_RADIO_SHOW_PARAMETERS(uint8_t ShowType);

/***********radio chip************/
typedef enum
{
	NO_RADIO_CHIP                = 0,  // Нет радио модуля
	RDA5807FP                    = 1,  // Модуль радио RDA5807
	SI4741                       = 2   // Модуль радио RDA4741
}enum_Radio_Chip;

#define NUM_RADIO_PRESET            11

#define MAX_RADIO_FM_FREQ           10800
#define MIN_RADIO_FM_FREQ           8750

#define DEFAULT_RADIO_PRESET_FREQ   0
#define DEFAULT_RADIO_FM_FREQ       8750
#define DEFAULT_RADIO_AM_FREQ       4745
#define DEFAULT_RADIO_AUDIO_DATA    OFF
#define DEFAULT_RADIO_MODE_AM_FM    1
#define DEFAULT_RADIO_PTY           OFF
#define DEFAULT_RADIO_RDS           OFF
#define DEFAULT_RADIO_STEREO        ON
#define DEFAULT_RADIO_PRESET        0
#define DEFAULT_RADIO_TA            OFF
#define DEFAULT_RADIO_SPASING       10
#define DEFAULT_RADIO_BAND          0


/* RDS and RBDS data */
typedef struct{
	uint16_t programId; //Program Identification (PI) code - unique code assigned to program.
   //In the US, except for simulcast stations, each station has a unique PI.
   //PI = 0 if no RDS info received.
   /* groupA and groupB indicate if the station has broadcast one or more of each RDS group type and version.
    * There is one bit for each group type.  Bit number 0 is for group type 0, and so on.
    * groupA gives version A groups (packets), groupB gives version B groups.
    * If a bit is true then one or more of that group type and version has been received.
    * Example:  If (groupA & 1<<4) is true then at least one Group type 4, version A group (packet)
    * has been received.
    * Note: If the RDS signal is weak, many bad packets will be received.  Sometimes, the packets are so
    * corrupted that the radio thinks the bad data is OK.  This can cause false information to be recorded
    * in the groupA and groupB variables.
    */
   uint16_t       groupA;               //One bit for each group type, version A
   uint16_t       groupB;               //One bit for each group type, version B
   uint8_t        RDSSignal;            //True if RDS (or RBDS) signal currently detected
   uint8_t        RBDS;                 //True if station using RBDS, else using RDS
   uint8_t        programType;          //Program Type (PTY) code - identifies program format - call getProgramTypeStr()
   uint8_t        extendedCountryCode;  //Extended Country Code (ECC) - constants defined above
   uint8_t        language;             //Language Code - constants defined above
   signed char    trafficProgram;       //Traffic Program flag - True if station gives Traffic Alerts
   signed char    trafficAlert;         //Traffic Alert flag - True if station currently broadcasting Traffic Alert
   signed char    music;                //Music/speech flag - True if broadcasting music, false if speech
   signed char    dynamicPTY;           //Dynamic PTY flag - True if dynamic (changing) PTY, false if static PTY
   signed char    compressedAudio;      //Compressed audio flag - True if compressed audio, false if not compressed
   signed char    binauralAudio;        //Binaural audio flag - True if binaural audio, false if not binaural audio
   signed char    RDSStereo;            //RDS stereo/mono flag - True if RDS info says station is stereo, false if mono
   char           programService[9];    //Station's name or slogan - usually used like Radio Text
   uint8_t        radioTextLen;         //Length of Radio Text message
   char           radioText[65];        //Descriptive message from station
   char           programTypeName[9];   //Program Type Name (PTYN)
   unsigned long  MJD;                  //UTC Modified Julian Date - origin is November 17, 1858
   uint8_t        hour;                 //UTC Hour
   uint8_t        minute;               //UTC Minute
   signed char    offset;               //Offset measured in half hours to convert UTC to local time.
                                        //If offset==NO_DATE_TIME then MJD, hour, minute are invalid.
} sRds;

typedef struct
{
	uint8_t AutoData :1;
	uint8_t RDS :1;
	uint8_t STEREO :1;
	uint8_t PTY :1;
	uint8_t TA :1;
	uint8_t Mode_AM_FM :1;
	uint8_t Band :2;           //max 3!!!
	uint8_t Spasing:5;
	uint8_t reserved0:3;
	uint8_t SelectedPreset:7;
	uint8_t editing :1;
	uint8_t reserved1;
	uint8_t reserved2;


} sFmRadioSettings;

typedef struct
{
	uint16_t    Freq:15;
	uint8_t     editing:1;
	uint8_t     programService[9];
} sFmRadioPreset;

typedef struct
{
	uint16_t    Freq:15;
	uint8_t     editing:1;
} sAmRadioPreset;

typedef struct
{
	sFM_Radio_SI4735 SI_4741;
} sRadioRegister;

typedef struct
{
	uint8_t  RSSI;
	uint8_t  SNR;
	uint8_t  MULT;
	uint8_t  STEREO;
	uint8_t  STEREO_BLEND;
	uint8_t  FREQOFF;
} sRadioSI;

typedef struct
{
	uint32_t         CheckSum;
	sFmRadioSettings Settings;
	sFmRadioPreset   FmPreset[NUM_RADIO_PRESET];
	sAmRadioPreset   AmPreset[NUM_RADIO_PRESET];
	uint8_t          InMode;
	uint8_t          RadioModule:7;
	uint8_t          NeedToSave:1;
	uint16_t         DataSaveSize;
	sRds             Rds;
	sRadioSI         ServiceInfo;
	sRadioRegister   Register;
} sFmRadio;


#endif /* MODULE_FMRADIO_FMRADIO_H_ */
