#include <Description.h>
#include <stdio.h>
#include <string.h>
#include "SI47xx.h"
#include "Exchange.h"
#include "GPIO.h"
#include "Nextion_LCD.h"
#include "cmsis_os.h"
#include "cmsis_os.h"
#include "SI47xxReg.h"
#include <stdbool.h>
#include <Structures.h>

extern sDixom Dixom;

#define Fm_Radio Dixom.Module.FmRadio

#define dFM_RSQ_STATUS Dixom.Module.FmRadio.Register.SI_4741.RSQ_STATUS

#define dSI_4741 Dixom.Module.FmRadio.Register.SI_4741

uint8_t SiBuff[20];
sSiRadio Si47xx;

tGET_INT_STATUS Get;
uint8_t _extendedCountryCode_count;
uint8_t _language_count;
signed char _abProgramTypeName;
signed char _abRadioText;       //Indicates new radioText[] string

xx sdfg;

WorkRes SetCMD(uint8_t* Property, uint8_t Lenght);
WorkRes SetProperties(uint8_t* Property);
WorkRes GetStateCTS(uint8_t timeOut);
WorkRes GetProperties(uint8_t* Property);


void SI4741_PowerpUP();



void Init_SI47xx(void){



	//dSI_4741.STATUS.CMD = 0x14;
	dSI_4741.RSQ_STATUS.CMD = 0x23;
	dSI_4741.TUNE_FREQ.CMD = 0x20;
	dSI_4741.SEEK_START.CMD =  0x21;
	dSI_4741.TUNE_STATUS.CMD = 0x22;
	dSI_4741.SET_PROPERTY.CMD = 0x12;
	dSI_4741.GET_PROPERTY.CMD = 0x13;


	dSI_4741.REFCLK_FREQ.Property = 0x0202;
	dSI_4741.DEEMPHASIS.Property = 0x1100;

/*	uint8_t FM_AGC_OVERRIDE[3] = {0x28, 0x00, 0x00};
	uint8_t FM_AGC_ATTACK_RATE[6] = {0x12, 0x00, 0x17, 0x00, 0x00, 0x00};
	uint8_t FM_AGC_RELEASE_RATE[6] = {0x12, 0x00, 0x17, 0x01, 0x00, 0x00};

	uint8_t FM_BLEND_RSSI_ATTACK_RATE[6] = {0x12, 0x00, 0x18, 0x02, 0x00, 0x00};
	uint8_t FM_BLEND_RSSI_RELEASE_RATE[6] = {0x12, 0x00, 0x18, 0x03, 0x00, 0x00};

	uint8_t FM_RDS_INT_SOURCE[6] = {0x12, 0x00, 0x15, 0x00, 0x00, 0x01};  // Enable RDSRECV interrupt (set RDSINT bit when RDS has filled the FIFO by the amount set on FM_RDS_INTERRUPT_FIFO_COUNT Reply Status. Clear-to-send high
	uint8_t FM_RDS_INT_FIFO_COUNT[6] = {0x12, 0x00, 0x15, 0x01, 0x00, 0x04}; //Set the minimum number of RDS groups stored in the RDS FIFO before	RDSRECV is set	Reply Status. Clear-to-send high
	uint8_t FM_RDS_CONFIG[6] = {0x12, 0x00, 0x15, 0x02, 0xEF, 0x01}; //Set Block Error A,B,C,D to 3,2,3,3	Enable RDS	Reply Status. Clear-to-send high



		Transmit_I2C1(DEVICE_ADDR_RADIO_SI4735<<1, NOT_USED, NOT_USED, &FM_AGC_OVERRIDE[0], 3, 200, ExchangeMainSteamDMA); Delay(50, FreeRTOS);
		Transmit_I2C1(DEVICE_ADDR_RADIO_SI4735<<1, NOT_USED, NOT_USED, &FM_RDS_INT_SOURCE[0], 6, 200, ExchangeMainSteamDMA); Delay(50, FreeRTOS);
		Transmit_I2C1(DEVICE_ADDR_RADIO_SI4735<<1, NOT_USED, NOT_USED, &FM_RDS_INT_FIFO_COUNT[0], 6, 200, ExchangeMainSteamDMA); Delay(50, FreeRTOS);
		Transmit_I2C1(DEVICE_ADDR_RADIO_SI4735<<1, NOT_USED, NOT_USED, &FM_RDS_CONFIG[0], 6, 200, ExchangeMainSteamDMA); Delay(50, FreeRTOS);
		Transmit_I2C1(DEVICE_ADDR_RADIO_SI4735<<1, NOT_USED, NOT_USED, &FM_AGC_ATTACK_RATE[0], 6, 200, ExchangeMainSteamDMA); Delay(50, FreeRTOS);


		Transmit_I2C1(DEVICE_ADDR_RADIO_SI4735<<1, NOT_USED, NOT_USED, &FM_AGC_RELEASE_RATE[0],6, 200, ExchangeMainSteamDMA); Delay(150, FreeRTOS);

		Transmit_I2C1(DEVICE_ADDR_RADIO_SI4735<<1, NOT_USED, NOT_USED, &FM_BLEND_RSSI_ATTACK_RATE[0], 6, 200, ExchangeMainSteamDMA); Delay(50, FreeRTOS);

		Transmit_I2C1(DEVICE_ADDR_RADIO_SI4735<<1, NOT_USED, NOT_USED, &FM_BLEND_RSSI_RELEASE_RATE[0], 6, 200, ExchangeMainSteam); Delay(150, FreeRTOS);*/

	SI4741_PowerpUP();



	SI47xx_setFreq();
	Dixom.Module.FmRadio.InMode = ReadyForWork;
}

void SI4741_PowerpUP(){

	dSI_4741.POWER_UP.CMD = 0x01;   // Command

	dSI_4741.POWER_UP.CTSIEN = 1;  // 1 = CTS interrupt enabled
	dSI_4741.POWER_UP.GPO2OEN = 1; // 1 = GPO2 output disabled.
	dSI_4741.POWER_UP.PATCH = 0;   // 0 = Boot normally.
	dSI_4741.POWER_UP.XOSCEN = 0;  // 1 = Use crystal oscillator (RCLK and GPO3/DCLK with external 32.768 kHz crystal and OPMODE=00000101).
	dSI_4741.POWER_UP.FUNC = 0;    // 0 = FM Receive.
	dSI_4741.POWER_UP.OPMODE = 176; //10110000 = Digital audio outputs (DCLK, DFS, DIO)

	//ExchangeStatus(Transmit_I2C1(DEVICE_ADDR_RADIO_SI4735<<1, NOT_USED, NOT_USED, (uint8_t *)&dSI_4741.POWER_UP, 3, 200, ExchangeMainSteamDMA),RADIO_WRITE);Delay (1, FreeRTOS);
	SetCMD((uint8_t *)&dSI_4741.POWER_UP, 3);
	Delay(700, FreeRTOS);

	dSI_4741.GET_REV.CMD = 0x10;   // Command
	ExchangeStatus(Receiver_I2C1(DEVICE_ADDR_RADIO_SI4735<<1, dSI_4741.GET_REV.CMD, 1, (uint8_t *)&dSI_4741.GET_REV.Response, 9, 200, ExchangeMemMainSteam),RADIO_READ);

	dSI_4741.GPO_IEN.Property = 0x0001;
	dSI_4741.GPO_IEN.ARG1.CTSIEN = 1;  //Seek/Tune Complete Interrupt Enable.
	dSI_4741.GPO_IEN.ARG1.RDSIEN = 1;  //RDS Interrupt Enable
	dSI_4741.GPO_IEN.ARG1.RSQIEN = 1;  //RSQ Interrupt Enable
	dSI_4741.GPO_IEN.ARG1.ERRIEN = 1;  //ERR Interrupt Enable
	dSI_4741.GPO_IEN.ARG1.STCIEN = 1;  //CTS Interrupt Enable. After PowerUp, this bit reflects the CTSIEN bit in ARG1 of	PowerUp Command.
	dSI_4741.GPO_IEN.ARG1.STCREP = 1;  //STC Interrupt Repeat.
	dSI_4741.GPO_IEN.ARG1.RDSREP = 1;  //RDS Interrupt Repeat
	dSI_4741.GPO_IEN.ARG1.RSQREP = 1;  //RSQ Interrupt Repeat

	dSI_4741.REFCLK_FREQ.Property = 0x0201;
	dSI_4741.REFCLK_FREQ.REFCLKF = 31250;	//Frequency of Reference Clock in Hz.
	SetProperties((uint8_t *)&dSI_4741.REFCLK_FREQ);

	dSI_4741.REFCLK_PRESCALE.Property = 0x0202;
	dSI_4741.REFCLK_PRESCALE.ARG1.RCLK_SEL = 0;   //0 = RCLK pin is clock source.
	dSI_4741.REFCLK_PRESCALE.ARG1.REFCLKP = 384; //Prescaler for Reference Clock.
	SetProperties((uint8_t *)&dSI_4741.REFCLK_PRESCALE);

	dSI_4741.DIGITAL_OUTPUT_FORMAT.Property = 0x0102;
	dSI_4741.DIGITAL_OUTPUT_FORMAT.ARG1.OFALL = 0;    // 0 = use DCLK rising edge
	dSI_4741.DIGITAL_OUTPUT_FORMAT.ARG1.OMODE = 0;    // 0000 = I2S
	dSI_4741.DIGITAL_OUTPUT_FORMAT.ARG1.OMONO = 0;    // 0 = Use mono/stereo blend (per blend thresholds)
	dSI_4741.DIGITAL_OUTPUT_FORMAT.ARG1.OSIZE = 2;    // 2 = 24-bits
	SetProperties((uint8_t*)&dSI_4741.DIGITAL_OUTPUT_FORMAT);

	dSI_4741.DIGITAL_OUTPUT_SAMPLE_RATE.Property = 0x0104;
	dSI_4741.DIGITAL_OUTPUT_SAMPLE_RATE.DOSR = 48000;          //48000 Sample Rate
	SetProperties((uint8_t*)&dSI_4741.DIGITAL_OUTPUT_SAMPLE_RATE);

	dSI_4741.DEEMPHASIS.Property = 0x1100;
	dSI_4741.DEEMPHASIS.ARG1.DEEMPH = 1;      //01 = 50 �s. Used in Europe, Australia, Japan
	SetProperties((uint8_t*)&dSI_4741.DEEMPHASIS);

	dSI_4741.RX_VOLUME.Property = 0x1100;
	dSI_4741.RX_VOLUME.ARG1.VOL = 63;          //Output Volume
	SetProperties((uint8_t*)&dSI_4741.RX_VOLUME);

	dSI_4741.FM_SEEK_BAND_BOTTOM.Property = 0x1400;
	dSI_4741.FM_SEEK_BAND_BOTTOM.FMSKFREQL = MIN_RADIO_FM_FREQ; //87.5 MHz FM Seek Band Bottom Frequency
	SetProperties((uint8_t*)&dSI_4741.FM_SEEK_BAND_BOTTOM);

	dSI_4741.FM_SEEK_BAND_TOP.Property = 0x1401;
	dSI_4741.FM_SEEK_BAND_TOP.FMSKFREQH  = MAX_RADIO_FM_FREQ; //108.0 MHz FM Seek Band Top Frequency
	SetProperties((uint8_t*)&dSI_4741.FM_SEEK_BAND_BOTTOM);

	SI_4741_Set_Spacing();

	dSI_4741.FM_SEEK_TUNE_RSSI_THRESHOLD.Property = 0x1403;
	dSI_4741.FM_SEEK_TUNE_RSSI_THRESHOLD.ARG1.SKRSSI = 3; //FM Seek/Tune SNR Threshold Specified in units of dB in 1 dB steps (0�127). Default is 3 dB
	SetProperties((uint8_t*)&dSI_4741.FM_SEEK_TUNE_RSSI_THRESHOLD);

	dSI_4741.FM_SEEK_TUNE_SNR_THRESHOLD.Property = 0x1404;
	dSI_4741.FM_SEEK_TUNE_SNR_THRESHOLD.ARG1.SKSNR = 20; //FM Seek/Tune Received Signal Strength Threshold Specified in units of dB�V in 1 dB�V steps (0�127). Default is 20 dB�V.
	SetProperties((uint8_t*)&dSI_4741.FM_SEEK_TUNE_RSSI_THRESHOLD);


	dSI_4741.FM_CHANNEL_FILTER.Property = 0x1102;
	dSI_4741.FM_CHANNEL_FILTER.FMCHFILT = 0; //0 = Automatically select proper channel filter.
	SetProperties((uint8_t*)&dSI_4741.FM_CHANNEL_FILTER);


}

WorkRes SetCMDnoDMA(uint8_t* Property, uint8_t Lenght);
WorkRes SetPropertiesNoDMA(uint8_t* Property);

void Test1(){
	dSI_4741.POWER_UP.CMD = 0x01;   // Command
	dSI_4741.POWER_UP.CTSIEN = 1;  // 1 = CTS interrupt enabled
	dSI_4741.POWER_UP.GPO2OEN = 1; // 1 = GPO2 output disabled.
	dSI_4741.POWER_UP.PATCH = 0;   // 0 = Boot normally.
	dSI_4741.POWER_UP.XOSCEN = 0;  // 1 = Use crystal oscillator (RCLK and GPO3/DCLK with external 32.768 kHz crystal and OPMODE=00000101).
	dSI_4741.POWER_UP.FUNC = 0;    // 0 = FM Receive.
	dSI_4741.POWER_UP.OPMODE = 176; //10110000 = Digital audio outputs (DCLK, DFS, DIO)
	SetCMD((uint8_t *)&dSI_4741.POWER_UP, 3);
}

void Test2(){
	dSI_4741.FM_CHANNEL_FILTER.Property = 0x1102;
	dSI_4741.FM_CHANNEL_FILTER.FMCHFILT = 0; //0 = Automatically select proper channel filter.
	SetProperties((uint8_t*)&dSI_4741.FM_CHANNEL_FILTER);
}

void Test3(){
	dSI_4741.POWER_UP.CMD = 0x01;   // Command
	dSI_4741.POWER_UP.CTSIEN = 1;  // 1 = CTS interrupt enabled
	dSI_4741.POWER_UP.GPO2OEN = 1; // 1 = GPO2 output disabled.
	dSI_4741.POWER_UP.PATCH = 0;   // 0 = Boot normally.
	dSI_4741.POWER_UP.XOSCEN = 0;  // 1 = Use crystal oscillator (RCLK and GPO3/DCLK with external 32.768 kHz crystal and OPMODE=00000101).
	dSI_4741.POWER_UP.FUNC = 0;    // 0 = FM Receive.
	dSI_4741.POWER_UP.OPMODE = 176; //10110000 = Digital audio outputs (DCLK, DFS, DIO)
	SetCMDnoDMA((uint8_t *)&dSI_4741.POWER_UP, 3);
}

void Test4(){
	dSI_4741.FM_CHANNEL_FILTER.Property = 0x1102;
	dSI_4741.FM_CHANNEL_FILTER.FMCHFILT = 0; //0 = Automatically select proper channel filter.
	SetPropertiesNoDMA((uint8_t*)&dSI_4741.FM_CHANNEL_FILTER);
}

WorkRes SetCMD(uint8_t* Property, uint8_t Lenght){

	if(ExchangeStatus(Transmit_I2C1(DEVICE_ADDR_RADIO_SI4735<<1, NOT_USED, NOT_USED, Property, Lenght, 200, ExchangeMainSteam),RADIO_WRITE) == WR_OK){//ExchangeMainSteamDMA
		return WR_OK;
	}else{
		return WR_ERROR;
	}
}

WorkRes SetProperties(uint8_t* Property){

	uint16_t CMD = 0x1200;
	uint8_t Property_Data[4] = {Property[1],Property[0],Property[3],Property[2]};


	if(ExchangeStatus(Transmit_I2C1(DEVICE_ADDR_RADIO_SI4735<<1, CMD, 2, &Property_Data[0], 4, 200, ExchangeMemMainSteam),RADIO_WRITE) == WR_OK){//ExchangeMemMainSteamDMA
		Delay(15, FreeRTOS);
		return WR_OK;
	}else{
		return WR_ERROR;
	}
}

WorkRes SetCMDnoDMA(uint8_t* Property, uint8_t Lenght){

	if(ExchangeStatus(Transmit_I2C1(DEVICE_ADDR_RADIO_SI4735<<1, NOT_USED, NOT_USED, Property, Lenght, 200, ExchangeMainSteam),RADIO_WRITE) == WR_OK){
		return WR_OK;
	}else{
		return WR_ERROR;
	}
}


WorkRes SetPropertiesNoDMA(uint8_t* Property){

	uint16_t CMD = 0x1200;
	uint8_t Property_Data[4] = {Property[1],Property[0],Property[3],Property[2]};


	if(ExchangeStatus(Transmit_I2C1(DEVICE_ADDR_RADIO_SI4735<<1, CMD, 2, &Property_Data[0], 4, 200, ExchangeMemMainSteam),RADIO_WRITE) == WR_OK){
		return WR_OK;
		Delay(15, FreeRTOS);
	}else{
		return WR_ERROR;
	}
}

WorkRes GetProperties(uint8_t* Property){

	uint16_t CMD = 0x1300;

	if(ExchangeStatus(Receiver_I2C1(DEVICE_ADDR_RADIO_SI4735<<1, CMD, 2, Property, 4, 200, ExchangeMemMainSteam),RADIO_READ)){
		return WR_OK;
		Delay(15, FreeRTOS);
	}else{
		return WR_ERROR;
	}
}

WorkRes GetStateCTS(uint8_t timeOut){

	while (dSI_4741.STATUS.CTS==0 && timeOut > 0){

		ExchangeStatus(Receiver_I2C1(DEVICE_ADDR_RADIO_SI4735<<1,  0x14, 1, (uint8_t*)&dSI_4741.STATUS, 1, 200, ExchangeMemMainSteam),RADIO_READ);
		timeOut--;
		Delay(1, FreeRTOS);
	}

	if(timeOut==0){
		return WR_TIMEOUT;
	}else{
		return WR_OK;
	}
}

uint8_t GetStatus(uint8_t GetInt){



	ExchangeStatus(Receiver_I2C1(DEVICE_ADDR_RADIO_SI4735<<1,  0x14, 1, (uint8_t*)&dSI_4741.STATUS, 1, 200, ExchangeMemMainSteam),RADIO_READ);

	switch (GetInt){
	case CTS     :   return Si47xx.Register.STATUS.CTS;      break;
	case ERR     :   return Si47xx.Register.STATUS.ERR;      break;
	case RSQINT  :   return Si47xx.Register.STATUS.RSQINT;   break;
	case RDSINT  :   return Si47xx.Register.STATUS.RDSINT;   break;
	case STCINT  :   return Si47xx.Register.STATUS.STCINT;   break;
	default      :   return 100;                             break;
	}
}

void SI_4741_Set_Spacing(){

	dSI_4741.FM_SEEK_FREQ_SPACING.Property = 0x1402;
	dSI_4741.FM_SEEK_FREQ_SPACING.ARG1.SKSPACE = Fm_Radio.Settings.Spasing; //FM Seek Frequency Spacing. 5 (50 kHz), 10 (100 kHz), and 20 (200 kHz). Default is 10
	SetProperties((uint8_t*)&dSI_4741.FM_SEEK_FREQ_SPACING);
}


void GetRevision(void){

	uint8_t GET_CMD = {0x10};
	uint8_t GET_DATA [15];

	ExchangeStatus(Receiver_I2C1(DEVICE_ADDR_RADIO_SI4735<<1, GET_CMD, 1, &GET_DATA[0], 9, 200, ExchangeMemMainSteam),RADIO_READ);

	RegisterToStruct(GET_DATA[0],(uint8_t*)&Si47xx.Register.STATUS);
	Si47xx.Register.REV.PN = GET_DATA[1];
	Si47xx.Register.REV.FWMAJOR = GET_DATA[2];
	Si47xx.Register.REV.FWMINOR = GET_DATA[3];
	Si47xx.Register.REV.PATCHH = GET_DATA[4];
	Si47xx.Register.REV.PATCHL = GET_DATA[5];
	Si47xx.Register.REV.CMPMAJOR = GET_DATA[6];
	Si47xx.Register.REV.CMPMINOR = GET_DATA[7];
	Si47xx.Register.REV.CHIPREV = GET_DATA[8];
}

void SI47xx_AutoSearch(uint8_t seek){

	uint8_t FreqAddr = {0x21};
	uint8_t  FreeqSeek;

	uint16_t time = 100;

	if(seek == 1){
		FreeqSeek = 12; //up auto search
	}else if(seek == 0){
		FreeqSeek = 4; //down auto search
	}

	if(seek==1 || seek == 0){

		if(time !=0){

			ExchangeStatus(Transmit_I2C1(DEVICE_ADDR_RADIO_SI4735<<1, FreqAddr, 1, &FreeqSeek, 1, 200, ExchangeMemMainSteam),RADIO_WRITE);//ExchangeMemMainSteamDMA
			GetTuneStatus();
		}
	}
}


#define MAX_RADIO_FM_FREQ           10800
#define MIN_RADIO_FM_FREQ           8750

void SI47xx_ManualSearch(uint8_t seek){

	if(seek == 1){

		Dixom.Module.FmRadio.FmPreset[0].Freq = Dixom.Module.FmRadio.FmPreset[0].Freq + Fm_Radio.Settings.Spasing;

		if(Dixom.Module.FmRadio.FmPreset[0].Freq > MAX_RADIO_FM_FREQ){
			Dixom.Module.FmRadio.FmPreset[0].Freq = MIN_RADIO_FM_FREQ;
		}
	}else{

		Dixom.Module.FmRadio.FmPreset[0].Freq = Dixom.Module.FmRadio.FmPreset[0].Freq - Fm_Radio.Settings.Spasing;

		if(Dixom.Module.FmRadio.FmPreset[0].Freq < MIN_RADIO_FM_FREQ){
			Dixom.Module.FmRadio.FmPreset[0].Freq = MAX_RADIO_FM_FREQ;
		}
	}
	SI47xx_setFreq();
    RADIO_GET_FREQ(All);
}


void SI47xx_setFreq(){

	if(Dixom.Module.FmRadio.FmPreset[0].Freq>100){
	    uint8_t byte[5] = {0x20, 0x00, Dixom.Module.FmRadio.FmPreset[0].Freq >> 8, Dixom.Module.FmRadio.FmPreset[0].Freq,  0x00};

	    ExchangeStatus(Transmit_I2C1(DEVICE_ADDR_RADIO_SI4735<<1, NOT_USED, NOT_USED, &byte[0], 5, 200, ExchangeMainSteam),RADIO_WRITE);//ExchangeMainSteamDMA
	    Delay(100, FreeRTOS);
	}
}

void GetTuneStatus(void){

	int8_t timeOut = 100;

	while (dSI_4741.STATUS.CTS==0 && timeOut > 0){
		Delay(1, FreeRTOS);
		ExchangeStatus(Receiver_I2C1(DEVICE_ADDR_RADIO_SI4735<<1,  0x14, 1, (uint8_t*)&dSI_4741.STATUS, 1, 200, ExchangeMemMainSteam),RADIO_READ);
		timeOut--;
	}

	Delay(1, FreeRTOS);
	if(timeOut !=0){
		do {
			uint8_t GET_CMD  = {0x22};
			uint8_t GET_DATA [7];

			ExchangeStatus(Receiver_I2C1(DEVICE_ADDR_RADIO_SI4735<<1, GET_CMD, 1, &GET_DATA[0], 7, 200, ExchangeMemMainSteam),RADIO_READ);

			RegisterToStruct(GET_DATA[0],(uint8_t*)&Si47xx.Register.STATUS);
			RegisterToStruct(GET_DATA[1],(uint8_t*)&Si47xx.Register.FM_TUNE_STATUS.RESP1);
			Si47xx.Register.FM_TUNE_STATUS.READFREQH = GET_DATA[2];
			Si47xx.Register.FM_TUNE_STATUS.READFREQL = GET_DATA[3];
			Si47xx.Register.FM_TUNE_STATUS.RSSI = GET_DATA[4];
			Si47xx.Register.FM_TUNE_STATUS.SNR = GET_DATA[5];
			Si47xx.Register.FM_TUNE_STATUS.MULT = GET_DATA[6];
			Si47xx.Freq = Si47xx.Register.FM_TUNE_STATUS.READFREQH << 8  | (Si47xx.Register.FM_TUNE_STATUS.READFREQL);



		    timeOut--;

		} while (Si47xx.Register.FM_TUNE_STATUS.RESP1.VALID == 0 && timeOut > 0);

		Delay(1000, FreeRTOS);
		RADIO_GET_FREQ(All);
	}
}



void GetRsqStatus(uint8_t Get){

	uint8_t GET_CMD  = {0x23};

	ExchangeStatus(Receiver_I2C1(DEVICE_ADDR_RADIO_SI4735<<1, GET_CMD, 1, (uint8_t *)&dFM_RSQ_STATUS.Response, sizeof(dFM_RSQ_STATUS.Response), 200, ExchangeMemMainSteam),RADIO_READ);

    if(Get == YES){
        sprintf((char *) Dixom.Buff, MaskGetRadioServiceInfo,
           		dFM_RSQ_STATUS.Response.RSSI,
    			dFM_RSQ_STATUS.Response.SNR,
    			dFM_RSQ_STATUS.Response.MULT,
    			dFM_RSQ_STATUS.Response.RESP3.PILOT,
    			dFM_RSQ_STATUS.Response.RESP3.STBLEND,
    			dFM_RSQ_STATUS.Response.FREQOFF);

        TransmitDataOutputs(Dixom.Buff, AutoLenght,  false,  true, allAvailable);

    }
}

uint8_t RDS_DATA [64];

static char make_printable(char ch);
static char make_printable(char ch){
   //Replace non-ASCII char with space
   if(ch<32 || 126<ch) ch=' ';
   return ch;
}

void GetRdsStatus(void){
	uint8_t GetCount = 15;
	uint8_t GET_CMD  = 0x24;
	uint8_t GET_DATA [GetCount];

	uint8_t SetArg[2];
	SetArg [0] = 0x24;
	SetArg [1] = 0x01;
/*

	//uint16_t GET_CMD = (GET_DATA[6]  | GET_DATA[7] << 8);

//	Transmit_I2C1(DEVICE_ADDR_RADIO_SI4735<<1, NOT_USED, NOT_USED, &SetArg[0], 2, 200, ExchangeMainSteam);
//	Receiver_I2C1(DEVICE_ADDR_RADIO_SI4735<<1, NOT_USED, NOT_USED, &GET_DATA[0], GetCount, 200, ExchangeMainSteam);

//	Receiver_I2C1(DEVICE_ADDR_RADIO_SI4735<<1, GET_CMD, 1, &GET_DATA[0], GetCount, 200, ExchangeMemMainSteam);


	RegisterToStruct(GET_DATA[0],(uint8_t*)&Si47xx.Register.STATUS);

	RegisterToStruct(GET_DATA[1],(uint8_t*)&Si47xx.Register.FM_RDS_STATUS.RESP1);
	RegisterToStruct(GET_DATA[2],(uint8_t*)&Si47xx.Register.FM_RDS_STATUS.RESP2);
	RegisterToStruct(GET_DATA[3],(uint8_t*)&Si47xx.Register.FM_RDS_STATUS.RDSFIFOUSED);
	RegisterToStruct(GET_DATA[12],(uint8_t*)&Si47xx.Register.FM_RDS_STATUS.RESP12);

	Si47xx.Register.FM_RDS_STATUS.RESP4_BLOCKA1 = GET_DATA[4];
	Si47xx.Register.FM_RDS_STATUS.RESP5_BLOCKA2 = GET_DATA[5];
	Si47xx.Register.FM_RDS_STATUS.RESP6_BLOCKB1 = GET_DATA[6];
	Si47xx.Register.FM_RDS_STATUS.RESP7_BLOCKB2 = GET_DATA[7];
	Si47xx.Register.FM_RDS_STATUS.RESP8_BLOCKC1 = GET_DATA[8];
	Si47xx.Register.FM_RDS_STATUS.RESP9_BLOCKC2 = GET_DATA[9];
	Si47xx.Register.FM_RDS_STATUS.RESP10_BLOCKD1 = GET_DATA[10];
	Si47xx.Register.FM_RDS_STATUS.RESP11_BLOCKD2 = GET_DATA[11];



	char str[2];

	str[0] = GET_DATA[10];
	str[1] = GET_DATA[11];

	uint16_t blok2 = (GET_DATA[6]  | GET_DATA[7] << 8);

	int rdsGroupType = (0x0A | ((blok2 & 0xF000) >> 8) | ((blok2 & 0x0800) >> 11));

	if(rdsGroupType == 0x0A){

		if((GET_DATA[7]*4)+4 < 64){
			RDS_DATA[(GET_DATA[7]*4)+1] = GET_DATA[8];
			RDS_DATA[(GET_DATA[7]*4)+2] = GET_DATA[9];
			RDS_DATA[(GET_DATA[7]*4)+3] = GET_DATA[10];
			RDS_DATA[(GET_DATA[7]*4)+4] = GET_DATA[11];
		}
	}



	//RegisterToStruct(GET_DATA[12],(uint8_t*)&Si47xx.Register.FM_RDS_STATUS.RESP12);


    sprintf((char *) Dixom.Buff,"RDS %s", (char *)&RDS_DATA );
    Transmit_Data_Out(Dixom.Buff, AutoLenght, 0, FreeRTOS, NO);

*/

    uint8_t response[13];  //Returned RDS info
       //Indices for group data in response[]
       enum {
          PI_H=4,  //Also "Block A"
          PI_L,
          Block_B_H,
          Block_B_L,
          Block_C_H,
          Block_C_L,
          Block_D_H,
          Block_D_L
       };
       uint8_t segment;  //Current segment
       uint8_t new_info = 0;  //Return value - true if new RDS info has been collected

       #define FIELD_RDS_STATUS_RESP2_SYNC              0b00000001
       #define FIELD_RDS_STATUS_RESP12_BLOCK_A          0b11000000
       #define RDS_STATUS_RESP12_BLOCK_A_UNCORRECTABLE  3U<<6


       //Check for FM mode
    //   if(_mode!=FM) return false;
       //Clear local RDS interrupt
    //   clearInterrupts(RDS_MASK);
       //Read in all pending RDS groups (packets)
       while(1){
         // Ask for next RDS group and clear RDS interrupt
         // static const byte PROGMEM FM_RDS_STATUS[]={CMD_FM_RDS_STATUS, RDS_STATUS_ARG1_CLEAR_INT};
         // sendCommand_P(FM_RDS_STATUS, sizeof(FM_RDS_STATUS));
         // getResponse(response, sizeof(response));

          Transmit_I2C1(DEVICE_ADDR_RADIO_SI4735<<1, NOT_USED, NOT_USED, &SetArg[0], 2, 200, ExchangeMainSteam); Delay(1, FreeRTOS);
          Receiver_I2C1(DEVICE_ADDR_RADIO_SI4735<<1, NOT_USED, NOT_USED, &response[0], sizeof(response), 200, ExchangeMainSteam);

          //Check for RDS signal
          Dixom.Module.FmRadio.Rds.RDSSignal = response[2] & FIELD_RDS_STATUS_RESP2_SYNC;
          //Get number of RDS groups (packets) available
          uint8_t num_groups=response[3];
          //Stop if nothing returned
          if(!num_groups) break;

          /* Because PI is resent in every packet's Block A, we told the radio its OK to
           * give us packets with a corrupted Block A.
           */
          //Check if PI received is valid
          if((response[12] & FIELD_RDS_STATUS_RESP12_BLOCK_A) != RDS_STATUS_RESP12_BLOCK_A_UNCORRECTABLE){
             //Get PI code
        	  Dixom.Module.FmRadio.Rds.programId = ((response[PI_H] << 8U) | response[PI_L]);
          }
          //Get PTY code
          Dixom.Module.FmRadio.Rds.programType = ((response[Block_B_H] & 0b00000011) << 3U) | (response[Block_B_L] >> 5U);
          //Get Traffic Program bit
          Dixom.Module.FmRadio.Rds.trafficProgram = (response[Block_B_H] & 0b00000100);

          //Get group type (0-15)
          uint8_t type = response[Block_B_H]>>4U;
          //Get group version (0=A, 1=B)
          bool version = response[Block_B_H] & 0b00001000;

          //Save which group type and version was received
          if(version){
        	  Dixom.Module.FmRadio.Rds.groupB |= 1U<<type;
          }else{
        	  Dixom.Module.FmRadio.Rds.groupA |= 1U<<type;
          }

          //Groups 0A & 0B - Basic tuning and switching information
          //Group 15B - Fast basic tuning and switching information
          /* Note: We support both Groups 0 and 15B in case the station has poor
           * reception and RDS packets are barely getting through.  This increases
           * the chances of receiving this info.
           */
          if(type==0 || (type==15 && version)){
             //Various flags
        	 Dixom.Module.FmRadio.Rds.trafficAlert = (response[Block_B_L] & 0b00010000);
        	 Dixom.Module.FmRadio.Rds.music =        (response[Block_B_L] & 0b00001000);
             uint8_t DI =               response[Block_B_L] & 0b00000100;

             //Get segment number
             segment =               response[Block_B_L] & 0b00000011;
             //Handle DI code
             switch(segment){
             case 0:
                Dixom.Module.FmRadio.Rds.dynamicPTY=DI;
                break;
             case 1:
                Dixom.Module.FmRadio.Rds.compressedAudio=DI;
                break;
             case 2:
                Dixom.Module.FmRadio.Rds.binauralAudio=DI;
                break;
             case 3:
                Dixom.Module.FmRadio.Rds.RDSStereo=DI;
                break;
             }

             //Groups 0A & 0B
             if(type==0){
                //Program Service
                char *ps = &Dixom.Module.FmRadio.Rds.programService[segment*2];
                *ps++ = make_printable(response[Block_D_H]);
                *ps   = make_printable(response[Block_D_L]);
             }
             new_info=true;
          }
          //Group 1A - Extended Country Code (ECC) and Language Code
          else if(type==1 && version==0){
             //We are only interested in the Extended Country Code (ECC) and
             //Language Code for this Group.

             //Get Variant code
             switch(response[Block_C_H] & 0b01110000){
             case (0<<4):  //Variant==0
                //Extended Country Code
                //Check if count has reached threshold
                if(_extendedCountryCode_count < RDS_THRESHOLD){
                   uint8_t ecc = response[Block_C_L];
                   //Check if datum changed
                   if(Dixom.Module.FmRadio.Rds.extendedCountryCode != ecc){
                      _extendedCountryCode_count=0;
                      new_info=true;
                   }
                   //Save new data
                   Dixom.Module.FmRadio.Rds.extendedCountryCode = ecc;
                   ++_extendedCountryCode_count;
                }
                break;
             case (3<<4):  //Variant==3
                //Language Code
                //Check if count has reached threshold
                if(_language_count < RDS_THRESHOLD){
                	uint8_t language = response[Block_C_L];
                   //Check if datum changed
                   if(Dixom.Module.FmRadio.Rds.language != language){
                      _language_count=0;
                      new_info=true;
                   }
                   //Save new data
                   Dixom.Module.FmRadio.Rds.language = language;
                   ++_language_count;
                }
                break;
             }
          }
          //Groups 2A & 2B - Radio Text
          else if(type==2){
             //Check A/B flag to see if Radio Text has changed
        	  uint8_t new_ab = (response[Block_B_L] & 0b00010000);
             if(new_ab != _abRadioText){
                //New message found - clear buffer
                _abRadioText=new_ab;
                for(uint8_t i=0; i<sizeof(Dixom.Module.FmRadio.Rds.radioText)-1; i++) Dixom.Module.FmRadio.Rds.radioText[i]=' ';
                Dixom.Module.FmRadio.Rds.radioTextLen=sizeof(Dixom.Module.FmRadio.Rds.radioText);  //Default to max length
             }
             //Get segment number
             segment = response[Block_B_L] & 0x0F;

             //Get Radio Text
             char *rt;  //Next position in rds.radioText[]
             uint8_t *block;  //Next char from segment
             uint8_t i;  //Loop counter
             //TODO maybe: convert RDS non ASCII chars to UTF-8 for terminal interface
             if(version==0){  // 2A
                rt = &Dixom.Module.FmRadio.Rds.radioText[segment*4];
                block = &response[Block_C_H];
                i=4;
             }
             else{  // 2B
                rt = &Dixom.Module.FmRadio.Rds.radioText[segment*2];
                block = &response[Block_D_H];
                i=2;
             }
             //Copy chars
             do{
                //Get next char from segment
                char ch = *block++;
                //Check for end of message marker
                if(ch=='\r'){
                   //Save new message length
                	Dixom.Module.FmRadio.Rds.radioTextLen = rt-Dixom.Module.FmRadio.Rds.radioText;
                }
                //Put next char in rds.radioText[]
                *rt++ = make_printable(ch);
             }while(--i);
             new_info=true;
          }
          //Group 4A - Clock-time and date
          else if(type==4 && version==0){
             //Only use if received perfectly.
             /* Note: Error Correcting Codes (ECC) are not perfect.  It is possible
              * for a block to be damaged enough that the ECC thinks the data is OK
              * when it's damaged or that it can recover when it cannot.  Because
              * date and time are useless unless accurate, we require that the date
              * and time be received perfectly to increase the odds of accurate data.
              */
             if( (response[12] & (FIELD_RDS_STATUS_RESP12_BLOCK_B |
              FIELD_RDS_STATUS_RESP12_BLOCK_C | FIELD_RDS_STATUS_RESP12_BLOCK_D)) ==
              (RDS_STATUS_RESP12_BLOCK_B_NO_ERRORS | RDS_STATUS_RESP12_BLOCK_C_NO_ERRORS |
              RDS_STATUS_RESP12_BLOCK_D_NO_ERRORS) ){
                //Get Modified Julian Date (MJD)
            	 Dixom.Module.FmRadio.Rds.MJD = (response[Block_B_L] & 0b00000011)<<15UL | response[Block_C_H]<<7U | response[Block_C_L]>>1U;

                //Get hour and minute
                Dixom.Module.FmRadio.Rds.hour = (response[Block_C_L] & 0b00000001)<<4U | response[Block_D_H]>>4U;
                Dixom.Module.FmRadio.Rds.minute = (response[Block_D_H] & 0x0F)<<2U | response[Block_D_L]>>6U;

                //Check if date and time sent (not 0)
                if(Dixom.Module.FmRadio.Rds.MJD || Dixom.Module.FmRadio.Rds.hour || Dixom.Module.FmRadio.Rds.minute || response[Block_D_L]){
                   //Get offset to convert UTC to local time
                	Dixom.Module.FmRadio.Rds.offset = response[Block_D_L]&0x1F;
                   //Check if offset should be negative
                   if(response[Block_D_L] & 0b00100000){
                      Dixom.Module.FmRadio.Rds.offset = -Dixom.Module.FmRadio.Rds.offset;  //Make it negative
                   }
                   new_info=true;
                }
             }
          }
          //Group 10A - Program Type Name
          else if(type==10 && version==0){
             //Check A/B flag to see if Program Type Name has changed
             uint8_t new_ab =  (response[Block_B_L] & 0b00010000);
             if(new_ab != _abProgramTypeName){
                //New name found - clear buffer
                _abProgramTypeName=new_ab;
                for(uint8_t i=0; i<sizeof(Dixom.Module.FmRadio.Rds.programTypeName)-1; i++) Dixom.Module.FmRadio.Rds.programTypeName[i]=' ';
             }
             //Get segment number
             segment = response[Block_B_L] & 0x01;

             //Get Program Type Name
             char *name = &Dixom.Module.FmRadio.Rds.programTypeName[segment*4];
             *name++ = make_printable(response[Block_C_H]);
             *name++ = make_printable(response[Block_C_L]);
             *name++ = make_printable(response[Block_D_H]);
             *name   = make_printable(response[Block_D_L]);
             new_info=true;
          }
       }

       sprintf((char *) Dixom.Buff,"RDS PS %s", (char *)&Dixom.Module.FmRadio.Rds.programService );
       Transmit_Data_Out(Dixom.Buff, AutoLenght, 0, FreeRTOS, YES);

       //return new_info;
}
/*
void SI47xx_SET_PRESET(uint8_t *Recive_USB){
	short    preset = 0;
	sscanf((char *)Recive_USB, "%hu", &preset );

	if( preset<0 || preset>NUM_RADIO_PRESET ){

		Transmit_Data_Out((uint8_t *)GeneralNotifi_CommandError, AutoLenght, 0, FreeRTOS, NO);
	}else{
		char data[30];

		if(Dixom.Module.FmRadio.FmPreset[preset].Freq >100){
			Dixom.Module.FmRadio.Settings.SelectedPreset = preset;
			Dixom.Module.FmRadio.FmPreset[0].Freq = Dixom.Module.FmRadio.FmPreset[preset].Freq;
			Dixom.Module.FmRadio.FmPreset[0].editing = YES;

			SI47xx_setFreq(Dixom.Module.FmRadio.FmPreset[0].Freq);
			sprintf( data,"RADIO_CURENT_PRESET %hhu %.2f", Dixom.Module.FmRadio.Settings.SelectedPreset, Dixom.Module.FmRadio.FmPreset[0].Freq*FConv);

			Transmit_Data_Out((uint8_t *)data, AutoLenght, 0, FreeRTOS, NO);
		}
	}
}*/


void SI47xx_GET_RADIO_FREQ(void){
	SI47xx_GetFreq();
}

void SI47xx_RADIO_AUTO_SEARCH(uint8_t *Recive_USB){
	uint16_t    seek = 0;
	sscanf((char *)Recive_USB, "%hu", &seek );

	if( seek<0 || seek>1 ){
		//USB_CDC_TransmitRN((uint8_t *)CommanError, LenCommanError, 1);
		Transmit_Data_Out((uint8_t *)GeneralNotifi_CommandError, AutoLenght, 0, FreeRTOS, YES);
	}else{
		SI47xx_AutoSearch(seek);
		Dixom.Module.FmRadio.Settings.SelectedPreset = 0;
	}
}

void SI47xx_RADIO_MANUAL_SEARCH(uint8_t *Recive_USB){
	uint16_t    seek = 0;
	sscanf((char *)Recive_USB, "%hu", &seek );

	if( seek<0 || seek>1 ){
		//USB_CDC_TransmitRN((uint8_t *)CommanError, LenCommanError, 1);
		Transmit_Data_Out((uint8_t *)GeneralNotifi_CommandError, AutoLenght, 0, FreeRTOS, YES);
	}else{
		SI47xx_ManualSearch(seek);
		Dixom.Module.FmRadio.Settings.SelectedPreset = 0;
	}
}


void  GET_SI47xx_InitInfo(void){

	 sprintf( (char*)Dixom.Buff,"RADIO_MODULE %hu", Dixom.Module.FmRadio.RadioModule );
	 Transmit_Data_Out(Dixom.Buff, AutoLenght, 20, FreeRTOS, YES);

}




void SI47xx_GetFreq(void){
    uint8_t byte[8];
    uint8_t send[2]={0x22, 0x00};

    ExchangeStatus(Transmit_I2C1(DEVICE_ADDR_RADIO_SI4735<<1, NOT_USED, NOT_USED, &send[0], 2, 200, ExchangeMainSteam),RADIO_WRITE);  Delay(5, FreeRTOS); //ExchangeMainSteamDMA
    ExchangeStatus(Receiver_I2C1(DEVICE_ADDR_RADIO_SI4735<<1, NOT_USED, NOT_USED, &byte[0], 8, 200, ExchangeMainSteam),RADIO_READ);      //Delay(, FreeRTOS);
    Dixom.Module.FmRadio.FmPreset[0].Freq =  byte[2] << 8 |byte[3];;
}




void SI47xx_GetStatus(void){

	uint8_t byte=20;
	uint8_t addr=20;

	ExchangeStatus(Receiver_I2C1(DEVICE_ADDR_RADIO_SI4735<<1, addr, 1, &byte, 1, 200, ExchangeMemMainSteam),RADIO_READ);    //  Delay(500, FreeRTOS);

}



void SI47xx_GetAgc(void){
    uint8_t byte[3];
    uint8_t send[2]={0x27, 0x00};
    //uint8_t lowByte = freq & 0x00FF;
    ExchangeStatus(Transmit_I2C1(DEVICE_ADDR_RADIO_SI4735<<1, NOT_USED, NOT_USED, &send[0], 2, 200, ExchangeMainSteam),RADIO_WRITE);      Delay(5, FreeRTOS);
    ExchangeStatus(Receiver_I2C1(DEVICE_ADDR_RADIO_SI4735<<1, NOT_USED, NOT_USED, &byte[0], 3, 200, ExchangeMainSteam),RADIO_READ);    //  Delay(500, FreeRTOS);


    sprintf((char *) Dixom.Buff,"AGC STATUS %d RFAGCDIS %d LNA_GAIN %d", byte[0], byte[1], byte[2]);

    Transmit_Data_Out(Dixom.Buff, AutoLenght, 0, FreeRTOS, YES);
}












