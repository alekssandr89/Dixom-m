#include <Description.h>
#include <RDA_5807.h>
#include <stdio.h>
#include <string.h>
#include "Exchange.h"
#include "GPIO.h"
#include "Nextion_LCD.h"
#include "cmsis_os.h"
#include "SI47xx.h"
#include <Structures.h>

extern sDixom Dixom;

float MaxFreq;
float MinFreq;
float RadioSpacing;
short FreqCoef=0;
uint8_t SignalLevelStatus =0;

//int tuneok,nochannel,power,volume,softblend,bass,mute,softmute,mono,afc,stereo,signal,fmready,fmstation;
int rdsblockerror,rdssynchro,rdsready,rds,block1,block2,block3,block4;

char    StationName[10];    // Station Name. 8 characters
char    RDSText[66];        // RDS test message 64 characters
char    CTtime[12];         // CT time string formatted as 'CT hh:mm'
int     minutes;            // CT minutes transmitted on the minute
float   freq;               // Tuned frequency


void Transmit_RADIO(uint16_t MemAddress, uint16_t *pData){

	uint8_t radioData    [3] = {MemAddress, *pData >> 8, *pData & 0xff};
	ExchangeStatus(	Transmit_I2C1(DEVICE_ADDR_RADIO_RDA5807<<1, NOT_USED, NOT_USED, radioData, 3, 200, ExchangeMainSteam), RADIO_WRITE);
}

void Receiver_RADIO(uint16_t MemAddress, uint16_t *pData, uint8_t RegNum ){

	ExchangeStatus(	Receiver_I2C1(DEVICE_ADDR_RADIO_RDA5807<<1, (uint16_t)(MemAddress & 0xff), 1, (uint8_t *)pData, RegNum << 1, 200, ExchangeMemMainSteam), RADIO_READ);
	Receiver_RADIO_bytesChange((uint8_t *)pData, RegNum << 1);
}

void Receiver_RADIO_bytesChange(uint8_t *pData, uint8_t Count)
{
  while (Count > 1)
  {
    uint8_t Temp = *(pData+1);
    *(pData+1) = *pData;
    *pData = Temp;
    pData += 2;
    Count -= 2;
  }
}


//--------------------------------------------------------------


    unsigned int RDA5807M_WriteReg[8],RDA5807M_ReadReg[7],RDA5807M_RDS[32];

    // ----- actual RDS values
    int textAB, lasttextAB, lastTextIDX;
    int rdsGroupType, rdsTP, rdsPTY;

    int mins;           // RDS time in minutes
    int lastmins;
    int i,n;
    int idx;            // index of rdsText
    int offset;         // RDS time offset and sign
    char c1, c2;
    char RDSTxt[66];
    char PSName[10];    // including trailing '\00' character.
    char PSName1[10];
    char PSName2[10];


uint16_t rda5807_rds_A, rda5807_rds_B, rda5807_rds_C, rda5807_rds_D;
uint16_t rda5807_rds_PI=0x0000;
uint8_t  rda5807_rds_PTY=0, rda5807_rds_grouptype, rda5807_rds_textpos;

uint8_t rds_buf_t0[8+1];   //group type 0 (station name): 8 characters
uint8_t rds_buf_t2[64+1];  //group type 2 (radiotext): 64 characters

//#define RDS_RAW_JDATE
uint32_t jdate;  // 17-bit modified Julian date
uint8_t rds_hours, rds_minutes;
signed char local_offset;  //in units of half-hour, with a sign

//RadioStruct      Radio[UNIQ_ELEM_RADIO];
tReg0Bh     Reg0B;
tReg0Ah     Reg0A;


void StructRadioInit_rda5807(void){

    for (short point = 0; point<UNIQ_ELEM_RADIO; point++){
//    	Receiver_EEPROM( EEPROM_ADDR_RADIO + point*4,   (uint8_t *)&Dixom.Module.FmRadio.FmPreset[point].Freq,    4, I2C2_TIME_OUT );
//    	Dixom.Module.FmRadio.FmPreset[point].editing = NO;
    }

  tReg02h     Reg02;
  tReg03h     Reg03;
  tReg04h     Reg04;
  tReg05h     Reg05;
  tReg06h     Reg06;
  tReg07h     Reg07;


  //Registr 0x02
  Reg02.bENABLE            = 1;   //00    Power Up Enable: 0 = Disabled; 1 = Enabled
  Reg02.bSOFT_RESET        = 0;   //01    Soft reset: 0 not reset; 1 reset
  Reg02.bNEW_METHOD        = 1;   //02    New Demodulate Method Enable, can improve  the receive sensitivity about 1dB.
  Reg02.bRDS_EN            = 1;   //03    RDS/RBDS enable  If 1, rds/rbds enable
  Reg02.bCLK_MODE          = 0;   //4-6   Xtal freq 000=32.768kHz  001=12Mhz  101=24Mhz  010=13Mhz  110=26Mhz  011=19.2Mhz  111=38.4Mhz
  Reg02.bSKMODE            = 0;   //07    Seek Mode  0 = wrap at the upper or lower band limit and  continue seeking  1 = stop seeking at the upper or lower band  limit
  Reg02.bSEEK              = 0;   //08    Seek Mode  0 = wrap at the upper or lower band limit and  continue seeking  1 = stop seeking at the upper or lower band  limit
  Reg02.bSEEKUP            = 1;   //09    Seek.  0 = Disable stop seek; 1 = Enable  Seek begins in the direction specified by  SEEKUP and ends when a channel is found,
                                  //      or the entire band has been searched.  The SEEK bit is set low and the STC bit is set  high when the seek operation completes
  Reg02.bRCLK_DirectInput  = 0;   //10    1=RCLK clock use the directly input mode
  Reg02.bRCLK_NonCalibMode = 1;   //11    0=RCLK clock is always supply  1=RCLK clock is not always supply when FM  work ( when 1, RDA5807FP can’t directly
                                  //      support -20 ℃ ~70 ℃ temperature. Only  suppory ±20℃ temperature swing from tune  point)
  Reg02.bBASS              = 0;   //12    Bass Boost.  0 = Disabled; 1 = Bass boost enabled
  Reg02.bMONO              = 0;   //13    Mono Select.  0 = Stereo; 1 = Force mono
  Reg02.bDMUTE             = 1;   //14    Mute Disable.  0 = Mute; 1 = Normal operation
  Reg02.bDHIZ              = 1;   //15    Audio Output High-Z Disable.  0 = High impedance; 1 = Normal operation
  //Registr 0x03
  Reg03.bSPACE             = 0;   //1-0   Channel Spacing.  00 = 100 kHz  01 = 200 kHz  10 = 50kHz  11 = 25KHz
  Reg03.bBAND              = 0;   //3-2   Band Select.  00 = 87–108 MHz (US/Europe)  01 = 76–91 MHz (Japan)  10 = 76–108 MHz (world wide)  11 = 65 –76 MHz （East Europe） or 50-65MHz
  Reg03.bTUNE              = 1;   //04    Tune  0 = Disable  1 = Enable  The tune operation begins when the TUNE bit  is set high. The STC bit is set high when the
                                  //      tune operation completes.  The tune bit is reset to low automatically when  the tune operation completes..   Reg03.bDIRECT_MODE       = 0;
  Reg03.bDIRECT_MODE       = 0;   //05    Directly Control Mode, Only used when test. 0
  Reg03.bCHAN              = Dixom.Module.FmRadio.FmPreset[RadioFreq].Freq;   //6-15  Channel Select.  BAND = 0  Frequency =  Channel Spacing (kHz) x CHAN+ 87.0 MHz  BAND = 1or 2  Frequency =  Channel Spacing (kHz) x CHAN + 76.0 MHz
                                  //      BAND = 3  Frequency =  Channel Spacing (kHz) x CHAN + 65.0 MHz  CHAN is updated after a seek operation.
  //Registr 0x04
  Reg04.bGPIO1             = 0;   //0-1   General Purpose I/O 1.  00 = High impedance  01 = Reserved  10 = Low  11 = High
  Reg04.bGPIO2             = 0;   //2-3   General Purpose I/O 1.  00 = High impedance  01 = Reserved  10 = Low  11 = High
  Reg04.bGPIO3             = 0;   //4-5   General Purpose I/O 1.  00 = High impedance  01 = Reserved  10 = Low  11 = High
  Reg04.bI2S_ENABLED       = 1;   //06    I2S bus enable  If 0, disabled;  If 1, enabled.
  Reg04.bRSVD1             = 0;   //07    Reserved
  Reg04.bAFCD              = 0;   //08    AFC disable (0 = afc work; 1 = afc disabled)
  Reg04.bSOFTMUTE_EN       = 1;   //09    If 1, softmute enable
  Reg04.bRSVD2             = 0;   //10    Reserved
  Reg04.bDE                = 0;   //11    De-emphasis (0 = 75 Вµs; 1 = 50 Вµs)
  Reg04.bRSVD3             = 0;   //12-13 Reserved
  Reg04.bSTCIEN            = 0;   //14    Seek/Tune Complete Interrupt Enable.  0 = Disable Interrupt  1 = Enable Interrupt  Setting STCIEN = 1 will generate a low pulse on  GPIO2 when the interrupt occurs.
  Reg04.bRSVD4             = 0;   //15    Reserved
  //Registr 0x05
  Reg05.bVOLUME            = 15;  //0-3   DAC Gain Control Bits (Volume).  0000=min; 1111=max  Volume scale is logarithmic  When 0000, output mute and output  impedance is very large
  Reg05.bRSVD1             = 0;   //4-5   Reserved
  Reg05.bLNAPORTSEL        = 2;   //6-7   LNA input port selection bit:  10: FMIN
  Reg05.bSEEKTH            = 8;   //8-11  Seek SNR threshold value
  Reg05.bRSVD2             = 0;   //12-14 Reserved
  Reg05.bINT_MODE          = 1;   //15    INT MODE (0 = generate 5ms interrupt; 1 = interrupt last until read reg0CH action occurs)
  //Registr 0x06
  Reg06.bRDELY             = 0;   //00    If 1, R channel data delay 1T.
  Reg06.bLDELY             = 0;   //01    If 1, L channel data delay 1T
  Reg06.bSCLKOEDGE         = 0;   //02    If 1, invert sclk output when as master
  Reg06.bSWOEDGE           = 0;   //03    If 1, invert ws output when as master.
  Reg06.bI2SSWCNT          = 8;   //4-7   4'b1000: WS_STEP_48;
  Reg06.bWSIEDGE           = 0;   //08    If 0, use normal ws internally;   If 1, inverte ws internally.
  Reg06.bDATASIGNED        = 1;   //09    If 0, I2S output unsigned 16-bit audio data. If 1, I2S output signed 16-bit audio data.
  Reg06.bSCLKIEDGE         = 0;   //10    When I2S enable  If 0, use normal sclk internally; If 1, inverte sclk internally.
  Reg06.bSWLR              = 0;   //11    Ws relation to l/r channel.   If 0, ws=0 ->r, ws=1 ->l;  If 1, ws=0 ->l, ws=1 ->r
  Reg06.bI2SMODE           = 1;   //12    If 0, master mode;  If 1, slave mode.
  Reg06.bOPEN_MODE         = 0;   //13-14 Open reserved register mode (11 = open behind registers writing function others: only open behind registers reading function
  Reg06.bRSVD1             = 0;   //15    Reserved
  //Registr 0x07
  Reg07.bFREQ_MODE         = 0;   //00    If 1, then freq setting changed.  Freq = 76000(or 87000) kHz + freq_direct (08H)  kHz.
  Reg07.bSOFTBLEND_EN      = 1;   //01    If 1, Softblend enable
  Reg07.bSEEK_TH_OLD       = 0;   //2-7   Seek threshold for old seek mode, Valid when  Seek_Mode=001
  Reg07.bRSVD1             = 0;   //08    Reserved
  Reg07.b65M_50M_MODE      = 1;   //09    Valid when band[1:0] = 2’b11 (0x03H_bit<3:2>)  1 = 65~76 MHz;  0 = 50~76 MHz
  Reg07.bTH_SOFRBLEND      = 16;  //10-14 Threshold for noise soft blend setting, unit 2dB 1
  Reg07.bRSVD2             = 0;   //15    Reserved

  Transmit_RADIO(Reg02Addr, (uint16_t *) &(Reg02));
	Transmit_RADIO(Reg03Addr, (uint16_t *) &(Reg03));
	Transmit_RADIO(Reg04Addr, (uint16_t *) &(Reg04));
	Transmit_RADIO(Reg05Addr, (uint16_t *) &(Reg05));
	Transmit_RADIO(Reg06Addr, (uint16_t *) &(Reg06));
	Transmit_RADIO(Reg07Addr, (uint16_t *) &(Reg07));

	updateRadioSpacing();

	if (Dixom.Module.FmRadio.Settings.Band == 0) {
		MaxFreq = MaxFreqEur;
		MinFreq = MinFreqEur;
	}   //default Europe
	if (Dixom.Module.FmRadio.Settings.Band == 1) {
		MaxFreq = MaxFreqJap;
		MinFreq = MinFreqJap;
	}
	if (Dixom.Module.FmRadio.Settings.Band == 2) {
		MaxFreq = MaxFreqWw;
		MinFreq = MinFreqWw;
	}
	if (Dixom.Module.FmRadio.Settings.Band == 3) {
		MaxFreq = MaxFreqEEur;
		MinFreq = MinFreqEEur;
	}
	osDelay(100);
	RDA5807_setFreq();


	Dixom.Module.FmRadio.InMode = ReadyForWork;

}



void GET_RADIO_ALL(){

	char     dataSend[25];

	RadioGetFreq();

	sprintf( dataSend,"RADIO_DEV %hhu %hhu %.2f", Dixom.Module.FmRadio.RadioModule, Dixom.Module.FmRadio.Settings.Band, RadioSpacing );
/*
	Delay(20, FreeRTOS);

	 sprintf( dataSend,"RADIO_MODULE %hu", GetSettings(SettingsSelectRadio) );
	 Transmit_Data_Out((uint8_t *)dataSend, AutoLenght, 20, FreeRTOS, NO);

	if(SignalLevelStatus == 1){
		Transmit_Data_Out((uint8_t *)"RADIO_STATE_SIGNAL_LEVEL 1", AutoLenght, 20, FreeRTOS, NO);
	}else{
		Transmit_Data_Out((uint8_t *)"RADIO_STATE_SIGNAL_LEVEL 0", AutoLenght, 20, FreeRTOS, NO);
	}

	sprintf( dataSend,"RADIO_BAND %hu", GetSettings(SettingsRadioBand) );
	//USB_CDC_TransmitRN((uint8_t *)dataSend, strlen (dataSend), 5);

	Transmit_Data_Out((uint8_t *)dataSend, AutoLenght, 20, FreeRTOS, NO);

	sprintf( dataSend,"RADIO_SPACING %.2f", RadioSpacing );
	//USB_CDC_TransmitRN((uint8_t *)dataSend, strlen (dataSend), 5);
	Transmit_Data_Out((uint8_t *)dataSend, AutoLenght, 20, FreeRTOS, NO);
*/
	GET_PRESET_RADIO();
	Delay(20, FreeRTOS);

	GET_ALL_FREQ_PRESET();
}
/*
void StructRadioDefault(){

	Dixom.Module.FmRadio.Settings.Spasing = 0;
	Dixom.Module.FmRadio.Settings.Band = 0;
	StructRadioInit_rda5807();
    for (short point = 1; point<UNIQ_ELEM_RADIO; point++){
    	Dixom.Module.FmRadio.FmPreset[point].Freq = 0;
    	Dixom.Module.FmRadio.FmPreset[point].editing = YES;
    }
    Dixom.Module.FmRadio.FmPreset[RadioFreq].Freq = MinFreq;
    RDA5807_setFreq();
}*/
/*
void StructRadioWrite(){
    for (short point = 0; point<UNIQ_ELEM_RADIO; point++){
  //  	Transmit_EEPROM( EEPROM_ADDR_RADIO + point*4,   (uint8_t *)&Dixom.Module.FmRadio.FmPreset[point].Freq,    4, I2C2_TIME_OUT );
  // 	Dixom.Module.FmRadio.FmPreset[point].editing=NO;
    }
}*/

void SET_RADIO_FREQ( uint8_t *Recive_USB ){
	float    fmFreq = 0;
	sscanf((char *)Recive_USB, "%f", &fmFreq );

	if( fmFreq<MinFreq || fmFreq>MaxFreq ){
		//USB_CDC_TransmitRN((uint8_t *)CommanError, LenCommanError, 1);
		Transmit_Data_Out((uint8_t *)GeneralNotifi_CommandError, AutoLenght, 0, FreeRTOS, NO);
	}else{
		Dixom.Module.FmRadio.FmPreset[RadioFreq].Freq = fmFreq;
		Dixom.Module.FmRadio.FmPreset[RadioFreq].editing = YES;
		RDA5807_setFreq();
		Dixom.Module.FmRadio.Settings.SelectedPreset = 0;
	}
}

void GET_RADIO_FREQ(){
	RadioGetFreq();
}

void GET_SIGNAL_LEVEL( uint8_t *Recive_USB ){

	uint16_t    LevelStatus = 0;
	sscanf((char *)Recive_USB, "%hu", &LevelStatus );

	if( LevelStatus<0 || LevelStatus>1 ){
		//USB_CDC_TransmitRN((uint8_t *)CommanError, LenCommanError, 1);
		Transmit_Data_Out((uint8_t *)GeneralNotifi_CommandError, AutoLenght, 0, FreeRTOS, NO);
	}else{
		SignalLevelStatus = LevelStatus;

		if(SignalLevelStatus == 1){
 		  	TransmitDataOutputs((uint8_t *)"RADIO_STATE_SIGNAL_LEVEL 1", AutoLenght,  false,  true, allAvailable);

		//	USB_CDC_TransmitRN("<RADIO_STATE_SIGNAL_LEVEL 1", 27, 1);
		}else{
 			TransmitDataOutputs((uint8_t *)"RADIO_STATE_SIGNAL_LEVEL 0", AutoLenght,  false,  true, allAvailable);
	//		USB_CDC_TransmitRN("<RADIO_STATE_SIGNAL_LEVEL 0", 27, 1);
		}
	}
}

/*
void RADIO_AUTO_SEARCH( uint8_t *Recive_USB ){
	uint16_t    seek = 0;
	sscanf((char *)Recive_USB, "%hu", &seek );

	if( seek<0 || seek>1 ){
		//USB_CDC_TransmitRN((uint8_t *)CommanError, LenCommanError, 1);
		Transmit_Data_Out((uint8_t *)GeneralNotifi_CommandError, AutoLenght, 0, FreeRTOS, NO);
	}else{
		RadioAutoSearch(seek);
		Dixom.Module.FmRadio.Settings.SelectedPreset = 0;
	}
}*//*
void SET_PRESET_RADIO(uint8_t *Recive_USB){
	short    preset = 0;
	sscanf((char *)Recive_USB, "%hu", &preset );

	if( preset<0 || preset>UNIQ_ELEM_RADIO ){
		//USB_CDC_TransmitRN((uint8_t *)CommanError, LenCommanError, 1);
		Transmit_Data_Out((uint8_t *)GeneralNotifi_CommandError, AutoLenght, 0, FreeRTOS, NO);
	}else{
		char data[30];
		SetRadioPreset(preset);
		//sprintf( data,"<RADIO_PRESET %hhu", GetSettings(SettingsRadioPreset) );
		sprintf( data,"RADIO_CURENT_PRESET %hhu %.3f", Dixom.Module.FmRadio.Settings.SelectedPreset, Dixom.Module.FmRadio.FmPreset[Dixom.Module.FmRadio.Settings.SelectedPreset].Freq*FConv);
		//USB_CDC_TransmitRN((uint8_t *)data, strlen (data), 1);
		Transmit_Data_Out((uint8_t *)data, AutoLenght, 0, FreeRTOS, NO);
	}
}*/

void GET_PRESET_RADIO(void){
	char data[30];
	sprintf( data,"RADIO_CURENT_PRESET %hhu %.3f", Dixom.Module.FmRadio.Settings.SelectedPreset, Dixom.Module.FmRadio.FmPreset[Dixom.Module.FmRadio.Settings.SelectedPreset].Freq*FConv );
	//USB_CDC_TransmitRN((uint8_t *)data, strlen (data), 1);
 	TransmitDataOutputs((uint8_t *)data, AutoLenght,  false,  true, allAvailable);

}
/*
void GET_ALL_FREQ_PRESET(void){
	char data[30];
    for (short point = 0; point<UNIQ_ELEM_RADIO; point++){
    	sprintf( data,"RADIO_PRESET %hhu %.3f",point, Dixom.Module.FmRadio.FmPreset[point].Freq*FConv  );
    	//USB_CDC_TransmitRN((uint8_t *)data, strlen (data), 1);
    	Transmit_Data_Out((uint8_t *)data, AutoLenght, 10, FreeRTOS, NO);
    	//osDelay(10);
    }
}*/
/*
void AUTO_SAVE_PRESET_RADIO(void){
	char    data[40];
	uint8_t already=0;

	for(short point=1; point<UNIQ_ELEM_RADIO-1; point++ ){
		if(Dixom.Module.FmRadio.FmPreset[point].Freq == Dixom.Module.FmRadio.FmPreset[RadioFreq].Freq ) already = point;
	}

	if(already==0){
		for(short point=1; point<UNIQ_ELEM_RADIO; point++){

			if(Dixom.Module.FmRadio.FmPreset[point].Freq==0) {
				Dixom.Module.FmRadio.FmPreset[point].Freq = Dixom.Module.FmRadio.FmPreset[RadioFreq].Freq;
				Dixom.Module.FmRadio.FmPreset[point].editing = YES;
				sprintf( data,"RADIO_PRESET %hu %.3f",  point, Dixom.Module.FmRadio.FmPreset[RadioFreq].Freq*FConv);
				//USB_CDC_TransmitRN((uint8_t *)data, strlen (data), 1);
				Transmit_Data_Out((uint8_t *)data, AutoLenght, 10, FreeRTOS, NO);
				return;
			}
		}
	}else{
		sprintf( data,"PRESET_ALREADY_SAVER_IN_POINT %hhu", already );
		//USB_CDC_TransmitRN((uint8_t *)data, strlen (data), 1);
		Transmit_Data_Out((uint8_t *)data, AutoLenght, 10, FreeRTOS, NO);
	}
}

void SAVE_PRESET_RADIO(uint8_t *Recive_USB){
	uint8_t    preset = 0;
	char    data[40];
	sscanf((char *)Recive_USB, "%hhu", &preset );

	if( preset<1 || preset>NUM_RADIO_PRESET ){
		Transmit_Data_Out((uint8_t *)GeneralNotifi_CommandError, AutoLenght, 0, FreeRTOS, NO);
	}else{

		Dixom.Module.FmRadio.FmPreset[preset].Freq = Dixom.Module.FmRadio.FmPreset[0].Freq;
		Dixom.Module.FmRadio.FmPreset[preset].editing = YES;

		sprintf( data,"RADIO_PRESET %hu %.2f",  preset, Dixom.Module.FmRadio.FmPreset[preset].Freq*FConv);
		//USB_CDC_TransmitRN((uint8_t *)data, strlen (data), 1);
		Transmit_Data_Out((uint8_t *)data, AutoLenght, 10, FreeRTOS, NO);
	}
}

void DELETE_PRESET_RADIO(uint8_t *Recive_USB){
	uint8_t    preset = 0;

	sscanf((char *)Recive_USB, "%hhu", &preset );

	if( preset<1 || preset>20 ){
		//USB_CDC_TransmitRN((uint8_t *)CommanError, LenCommanError, 1);
		Transmit_Data_Out((uint8_t *)GeneralNotifi_CommandError, AutoLenght, 0, FreeRTOS, NO);
	}else{
		char data[25];
		Dixom.Module.FmRadio.FmPreset[preset].Freq = 0.000;
		Dixom.Module.FmRadio.FmPreset[preset].editing = YES;
		sprintf( data,"RADIO_PRESET %hhu 0.000", Dixom.Module.FmRadio.Settings.SelectedPreset );
		//USB_CDC_TransmitRN((uint8_t *)data, strlen (data), 1);
		Transmit_Data_Out((uint8_t *)data, AutoLenght, 10, FreeRTOS, NO);
	}
}*/
/*
void RADIO_MANUAL_SEARCH( uint8_t *Recive_USB ){
	short    direction = 0;
	sscanf((char *)Recive_USB, "%hu", &direction );

	if( direction<0 || direction>1 ){
		//USB_CDC_TransmitRN((uint8_t *)CommanError, LenCommanError, 1);
		Transmit_Data_Out((uint8_t *)GeneralNotifi_CommandError, AutoLenght, 0, FreeRTOS, NO);
	}else{
		RadioManualSearch(direction);
		Dixom.Module.FmRadio.Settings.SelectedPreset = 0;
	}
}*/

void SetRadioPreset(short preset){

	Dixom.Module.FmRadio.Settings.SelectedPreset = preset;
	Dixom.Module.FmRadio.FmPreset[RadioFreq].Freq = Dixom.Module.FmRadio.FmPreset[preset].Freq;

	if(Dixom.Module.FmRadio.RadioModule == SI4741){

		SI47xx_setFreq(Dixom.Module.FmRadio.FmPreset[RadioFreq].Freq);
	}else{

		RDA5807_setFreq();

	}



	RadioGetFreq();
	/*sprintf( data,"<RADIO PRESET %hhu =%.3f>", preset, Radio[preset].Data  );
	USB_CDC_TransmitRN(data, strlen (data), 1);*/
}

void RadioAutoSearch(short direction){
	if(Dixom.Module.FmRadio.RadioModule == RDA5807FP){
		RDA5807_autoSearch(direction);
	}else if(Dixom.Module.FmRadio.RadioModule == SI4741){
		SI47xx_AutoSearch(direction);
	}
}

void RadioPresetRouting(uint8_t action, short direction, uint8_t pressed) {

	if (pressed == YES) {
		if (action == 0) {
			short preset = Dixom.Module.FmRadio.Settings.SelectedPreset;
			int compare = 0;
			if (direction == 1) {

				for (short count = 1; count < UNIQ_ELEM_RADIO - 1; count++) {
					preset++;
					if (preset > UNIQ_ELEM_RADIO - 1)
						preset = 1;
					compare =  Dixom.Module.FmRadio.FmPreset[preset].Freq * 1000;
					if (compare != 0) {
						SetRadioPreset(preset);
						count = UNIQ_ELEM_RADIO;
						GET_PRESET_RADIO();
					}
				}
			} else {
				for (short count = 0; count < UNIQ_ELEM_RADIO - 2; count++) {
					preset--;
					if (preset < 1)
						preset = UNIQ_ELEM_RADIO - 1;
					compare = Dixom.Module.FmRadio.FmPreset[preset].Freq * 1000;
					if (compare != 0) {
						SetRadioPreset(preset);
						count = UNIQ_ELEM_RADIO;
						GET_PRESET_RADIO();
					}
				}
			}
		} else {
			if (direction > 20)
				direction = 20;
			if (direction < 1)
				direction = 1;
			SetRadioPreset(direction);
			GET_PRESET_RADIO();
		}
	}
}

float GetRadioFreq(){
	return Dixom.Module.FmRadio.FmPreset[RadioFreq].Freq;
}

void RadioGetFreq(){

	uint8_t SettingsSpacing = Dixom.Module.FmRadio.Settings.SelectedPreset;

	if(SettingsSpacing==0 || SettingsSpacing==1){

		sprintf((char *)Dixom.Buff,"RADIO_FREQ %.1f", Dixom.Module.FmRadio.FmPreset[RadioFreq].Freq*FConv );
		//NextionFreqFM(Radio[RadioFreq].Data);


	}else if (Dixom.Module.FmRadio.Settings.Spasing==2){

		sprintf((char *)Dixom.Buff,"RADIO_FREQ %.2f", Dixom.Module.FmRadio.FmPreset[RadioFreq].Freq*FConv );

	}else if(Dixom.Module.FmRadio.Settings.Spasing==3) {

		sprintf((char *)Dixom.Buff,"RADIO_FREQ %.3f", Dixom.Module.FmRadio.FmPreset[RadioFreq].Freq*FConv );
	}

  	TransmitDataOutputs(Dixom.Buff, AutoLenght,  false,  true, allAvailable);

}

void RadioManualSearch(short direction){

	if(direction==1){
		Dixom.Module.FmRadio.FmPreset[RadioFreq].Freq = Dixom.Module.FmRadio.FmPreset[RadioFreq].Freq + RadioSpacing;
		if(Dixom.Module.FmRadio.FmPreset[RadioFreq].Freq > MaxFreq) Dixom.Module.FmRadio.FmPreset[RadioFreq].Freq = MinFreq;
	}else{
		Dixom.Module.FmRadio.FmPreset[RadioFreq].Freq = Dixom.Module.FmRadio.FmPreset[RadioFreq].Freq - RadioSpacing;
		if(Dixom.Module.FmRadio.FmPreset[RadioFreq].Freq < MinFreq) Dixom.Module.FmRadio.FmPreset[RadioFreq].Freq = MaxFreq;
	}

	Dixom.Module.FmRadio.FmPreset[RadioFreq].editing = YES;

	RDA5807_setFreq();
	RadioGetFreq();
}
/*
void SET_RADIO_BAND( uint8_t *Recive_USB )
{
	char     dataSend[25];
	short    data = 0;
	sscanf((char *)Recive_USB, "%hu", &data );

	if( data<0 || data>3 ){
		//USB_CDC_TransmitRN((uint8_t *)CommanError, LenCommanError, 1);
		Transmit_Data_Out((uint8_t *)GeneralNotifi_CommandError, AutoLenght, 0, FreeRTOS, NO);
	}else{

		Dixom.Module.FmRadio.Settings.Band = data;

		updateRadioBand();

		sprintf( dataSend,"RADIO_BAND %hu", Dixom.Module.FmRadio.Settings.Band );
		//USB_CDC_TransmitRN((uint8_t *)dataSend, strlen (dataSend), 1);
		Transmit_Data_Out((uint8_t *)dataSend, AutoLenght, 10, FreeRTOS, NO);
		sprintf( dataSend,"RADIO_FREQ %.2f", Dixom.Module.FmRadio.FmPreset[RadioFreq].Freq*FConv );
	//	USB_CDC_TransmitRN((uint8_t *)dataSend, strlen (dataSend), 1);
		Transmit_Data_Out((uint8_t *)dataSend, AutoLenght, 0, FreeRTOS, NO);
	}
}*/
/*
void SET_RADIO_SPACING( uint8_t *Recive_USB ){

	char     dataSend[25];
	short    data = 0;
	sscanf((char *)Recive_USB, "%hu", &data );

	if( data<0 || data>3 ){
		//USB_CDC_TransmitRN((uint8_t *)CommanError, LenCommanError, 1);
		Transmit_Data_Out((uint8_t *)GeneralNotifi_CommandError, AutoLenght, 0, FreeRTOS, NO);
	}else{

		Dixom.Module.FmRadio.Settings.Spasing = data;

		updateRadioSpacing();

		sprintf( dataSend,"RADIO_SPACING %.2f", RadioSpacing );
		//USB_CDC_TransmitRN((uint8_t *)dataSend, strlen (dataSend), 1);
		Transmit_Data_Out((uint8_t *)dataSend, AutoLenght, 0, FreeRTOS, NO);
	}
}
*/



void updateRadioSpacing(void){
	if(Dixom.Module.FmRadio.Settings.Spasing==0){   RadioSpacing = RadioSpacing100; FreqCoef = 10;  } //default 100kHz
	if(Dixom.Module.FmRadio.Settings.Spasing==1){   RadioSpacing = RadioSpacing200; FreqCoef = 10;  }
	if(Dixom.Module.FmRadio.Settings.Spasing==2){   RadioSpacing = RadioSpacing50;  FreqCoef = 100; }
	if(Dixom.Module.FmRadio.Settings.Spasing==3){   RadioSpacing = RadioSpacing25;  FreqCoef = 1000;}

	 RDA5807_setSpacing();
}

void updateRadioBand(void){
	if(Dixom.Module.FmRadio.Settings.Band==0){    MaxFreq=MaxFreqEur;      MinFreq=MinFreqEur; }//default Europe
	if(Dixom.Module.FmRadio.Settings.Band==1){    MaxFreq=MaxFreqJap;      MinFreq=MinFreqJap; }
	if(Dixom.Module.FmRadio.Settings.Band==2){    MaxFreq=MaxFreqWw;       MinFreq=MinFreqWw;  }
	if(Dixom.Module.FmRadio.Settings.Band==3){    MaxFreq=MaxFreqEEur;     MinFreq=MinFreqEEur;}

	Dixom.Module.FmRadio.FmPreset[RadioFreq].Freq = MinFreq;
	Dixom.Module.FmRadio.FmPreset[RadioFreq].editing = YES;

	RDA5807_setFreq();
}

uint8_t TuneStatus =0;
short signalLevelTimer = 0;
uint8_t oldSignalLevel;
short AutoSearchTimer=0;


/*
uint16_t block1;
uint16_t block2;
uint16_t block3;
uint16_t block4;
uint16_t rdsready, rdssynchro, rdsblockerror;
*/

void RadioAutoSearchTimer(void){

	if(TuneStatus==1){
		tReg0Ah     Reg0A;
		tReg03h     Reg03;

		AutoSearchTimer++;
		if(AutoSearchTimer==200){
			AutoSearchTimer=0;
			Receiver_RADIO(Reg0AAddr, (uint16_t *) &Reg0A, 1);
			if(Reg0A.bSTC==1){
				TuneStatus=0;
				Receiver_RADIO(Reg03Addr, (uint16_t *) &Reg03, 1);
				Dixom.Module.FmRadio.FmPreset[RadioFreq].Freq = ((Reg03.bCHAN + MinFreq*FreqCoef));
				Dixom.Module.FmRadio.FmPreset[RadioFreq].editing = YES;
				RadioGetFreq();
			}
		}
	}

	if(SignalLevelStatus == 1){
		signalLevelTimer++;

		if( signalLevelTimer> 1500){
			signalLevelTimer = 0;

			char data[30];

			Receiver_RADIO(Reg0AAddr, (uint16_t *) &Reg0A, 1);
			Receiver_RADIO(Reg0BAddr, (uint16_t *) &Reg0B, 1);


			if(oldSignalLevel!=Reg0B.bRSSI){
				sprintf( data,"RADIO_SERVICE_DETAILS %hhu %hhu %hhu %hhu %hhu %hhu", Reg0B.bRSSI, Reg0A.bRDSR, Reg0A.bST, Reg0A.bRDSS, Reg0B.bBLERA, Reg0B.bBLERB  );
				//USB_CDC_TransmitRN((uint8_t *)data, strlen (data), 1);
 	 		  	TransmitDataOutputs((uint8_t *)data, AutoLenght,  false,  true, allAvailable);

			}

			oldSignalLevel = Reg0B.bRSSI;
		}
	}
}




void RDA5807_setFreq(void){

    tReg03h     Reg03;
    uint32_t    Freq = (uint32_t)(Dixom.Module.FmRadio.FmPreset[0].Freq);

    Freq -= MinFreq*FreqCoef;

	Receiver_RADIO(Reg03Addr, (uint16_t *) &Reg03, 1);
    Reg03.bCHAN = Freq;
    Reg03.bTUNE = 1;
    Transmit_RADIO(Reg03Addr,(uint16_t *) &(Reg03));
}

void RDA5807_setBand(void){

    tReg03h     Reg03;
	Receiver_RADIO(Reg03Addr, (uint16_t *) &Reg03, 1);
    Reg03.bBAND = Dixom.Module.FmRadio.Settings.Band;
    Transmit_RADIO(Reg03Addr,(uint16_t *) &(Reg03));
}

void RDA5807_setSpacing(void){
    tReg03h     Reg03;
	Receiver_RADIO(Reg03Addr, (uint16_t *) &Reg03, 1);
    Reg03.bSPACE = Dixom.Module.FmRadio.Settings.Spasing;
    Transmit_RADIO(Reg03Addr,(uint16_t *) &(Reg03));
}

void RDA5807_autoSearch(short direction){
	TuneStatus=1;
    tReg02h     Reg02;
	Receiver_RADIO(Reg02Addr,(uint16_t *) &Reg02, 1);
	Reg02.bSKMODE = 1;
	Reg02.bSEEK = 1;
	Reg02.bSEEKUP = direction;          //0 = Search down; 1 = Search up
	Transmit_RADIO(Reg02Addr,(uint16_t *) &(Reg02));
}


char    StationName[10];    // Station Name. 8 characters
char    RDSText[66];        // RDS test message 64 characters
char    CTtime[12];         // CT time string formatted as 'CT hh:mm'
int     minutes;            // CT minutes transmitted on the minute
float   freq;               // Tuned frequency


void Readregisters(){
//   int i;
//   char rcv[12];
  /* i2c.read(0x20, rcv,12);   // read 12 bytes for reg 0x0A .. reg 0x0F
        for(i=0; i<6; i++){
            RDA5807M_ReadReg[i] = ((rcv[i *2] << 8) | rcv [(i*2) +1] );
        }*//*
    block1 = RDA5807M_ReadReg[2];
    block2 = RDA5807M_ReadReg[3];
    block3 = RDA5807M_ReadReg[4];
    block4 = RDA5807M_ReadReg[5];
    */


	rdsready = RDA5807M_ReadReg[0] & 0x8000;            //if rdsready != 0 rds data are ready
    rdssynchro = RDA5807M_ReadReg[0] & 0x1000;         //if rdssynchro = 1000 rds decoder syncrhonized
    rdsblockerror = RDA5807M_ReadReg[1] & 0x000C;       //check for rds blocks errors

	Receiver_RADIO(Reg0CAddr, (uint16_t *) &block1, 1);
	Receiver_RADIO(Reg0DAddr, (uint16_t *) &block2, 1);
	Receiver_RADIO(Reg0EAddr, (uint16_t *) &block3, 1);
	Receiver_RADIO(Reg0FAddr, (uint16_t *) &block4, 1);


                                                        //00= 0 errors,01= 1~2 errors requiring correction
                                                        //10= 3~5 errors requiring correction
                                                        //11= 6+ errors or error in checkword, correction not possible.
}

void ProcessData()
{
    Readregisters();
 /*   if (rdssynchro != 0x1000){  // reset all the RDS info.
        //RDSinit();
        return;
    }*/
  // analyzing Block 2
    rdsGroupType = 0x0A | ((block2 & 0xF000) >> 8) | ((block2 & 0x0800) >> 11);
    rdsTP = (block2 & 0x0400);
    rdsPTY = (block2 & 0x0400);

    switch (rdsGroupType) {
        case 0x0A:
        case 0x0B:
        // The data received is part of the Service Station Name
            idx = 2 * (block2 & 0x0003);
            // new data is 2 chars from block 4
            c1 = block4 >> 8;
            c2 = block4 & 0x00FF;
        // check that the data was received successfully twice
        // before sending the station name
        if ((PSName1[idx] == c1) && (PSName1[idx + 1] == c2)) {
            // retrieve the text a second time: store to _PSName2
            PSName2[idx] = c1;
            PSName2[idx + 1] = c2;
            PSName2[8] = '\0';
            if (strcmp(PSName1, PSName2) == 0) {
                // populate station name
                n=0;
                for(i=0;i<(8);i++){ // remove non-printable error ASCCi characters
                    if(PSName2[i] > 31 && PSName2[i] < 127){
                        StationName[n] = PSName2[i];
                        n++;
                    }
                }
            }
        }
        if ((PSName1[idx] != c1) || (PSName1[idx + 1] != c2)) {
            PSName1[idx] = c1;
            PSName1[idx + 1] = c2;
            PSName1[8] = '\0';
        }
    break;

    case 0x2A:
        // RDS text
    textAB = (block2 & 0x0010);
    idx = 4 * (block2 & 0x000F);
    if (idx < lastTextIDX) {
      // The existing text might be complete because the index is starting at the beginning again.
      // Populate RDS text array.
        n=0;
        for(i=0;i<strlen(RDSTxt);i++){
            if(RDSTxt[i] > 31 && RDSTxt[i] < 127){    // remove any non printable error charcters
                RDSText[n] = RDSTxt[i];
                n++;
            }
        }
      }
    lastTextIDX = idx;
    if (textAB != lasttextAB) {
      // when this bit is toggled the whole buffer should be cleared.
      lasttextAB = textAB;
      memset(RDSTxt, 0, sizeof(RDSTxt));
      memset(RDSText, '\0', sizeof(RDSText));
    }
    if(rdsblockerror < 4){
        // new data is 2 chars from block 3
        RDSTxt[idx] = (block3 >> 8);     idx++;
        RDSTxt[idx] = (block3 & 0x00FF); idx++;
        // new data is 2 chars from block 4
        RDSTxt[idx] = (block4 >> 8); idx++;
        RDSTxt[idx] = (block4 & 0x00FF); idx++;
    }
    break;

  case 0x4A:
    // Clock time and date
    if(rdsblockerror <3){ // allow limited RDS data errors as we have no correctioin code
        offset = (block4) & 0x3F; // 6 bits
        mins = (block4 >> 6) & 0x3F; // 6 bits
        mins += 60 * (((block3 & 0x0001) << 4) | ((block4 >> 12) & 0x0F));
    }
    // adjust offset
    if (offset & 0x20) {
      mins -= 30 * (offset & 0x1F);
    }
    else {
      mins += 30 * (offset & 0x1F);
    }
    if(mins == lastmins+1){ // get CT time twice before populating time
        minutes=mins;
    }
        lastmins=mins;
    if(rdssynchro == 0x1000){


    	if(minutes>0 && minutes<1500){

        	sprintf(CTtime, "CT %2d:%02d",(minutes/60),(minutes%60));

    	}

    }

    else{
        	minutes=0;
        	sprintf(CTtime, "CT --:--");
        } // CT time formatted string
    break;

  case 0x6A:
    // IH
    break;

  case 0x8A:
    // TMC
    break;

  case 0xAA:
    // TMC
    break;

  case 0xCA:
    // TMC
    break;

  case 0xEA:
    // IH
    break;

  default:

    break;
  }
}






