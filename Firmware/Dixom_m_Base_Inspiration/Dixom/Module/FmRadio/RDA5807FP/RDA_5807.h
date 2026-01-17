#ifndef __FM_RADIO_H__
#define __FM_RADIO_H__
#include "stdint.h"

#define Reg02Addr               0x02
#define Reg03Addr               0x03
#define Reg04Addr               0x04
#define Reg05Addr               0x05
#define Reg06Addr               0x06
#define Reg07Addr               0x07
#define Reg0AAddr               0x0A
#define Reg0BAddr               0x0B
#define Reg0AAddr               0x0A
#define Reg0CAddr               0x0C
#define Reg0DAddr               0x0D
#define Reg0EAddr               0x0E
#define Reg0FAddr               0x0F


#define MaxFreqEur              108.0  // US/Europe max
#define MinFreqEur              87.0   // US/Europe min
#define MaxFreqJap              91.0   // Japan max
#define MinFreqJap              76.0   // Japan min
#define MaxFreqWw               108.0  // World wide max
#define MinFreqWw               76.0   // World wide min
#define MaxFreqEEur             76.0   // World wide East Europe
#define MinFreqEEur             65.0   // World wide East Europe

/*********Radio Spacing***********/
#define RadioSpacing100         0.100     // 100kHz
#define RadioSpacing200         0.200     // 200kHz
#define RadioSpacing50          0.050     // 50kHz
#define RadioSpacing25          0.025     // 25kHz

typedef struct{
	float Data;
	uint8_t  editing;
} RadioStruct;

#define UNIQ_ELEM_RADIO         20

#define RadioFreq               0





//static void rda5807_process_one_char(uint8_t chr, uint8_t *textbuf, uint8_t lcd_row);
void            rda5807_process_rds_data();


void                   Transmit_RADIO(uint16_t MemAddress, uint16_t *pData);
void                   Receiver_RADIO(uint16_t MemAddress, uint16_t *pData, uint8_t RegNum );
void                   Receiver_RADIO_bytesChange(uint8_t *pData, uint8_t Count);

void             StructRadioInit_rda5807(void);
//void             StructRadioDefault(void);                   //EXAMPLE: SAVE RADIO                      ���������� �������� �����
//void             StructRadioWrite(void);                     //EXAMPLE: DEFAULT RADIO                   ����� �������� ����� �� �����������

void             GET_RADIO_ALL();
//void             SET_RADIO_SPACING( uint8_t *Recive_USB );   //EXAMPLE: RADIO SET SPACING 0             ����� �������� ���������� ������� 0 = 0.1, 1 = 0.2, 2 = 0.5, 3 = 0.025
//void             SET_RADIO_BAND( uint8_t *Recive_USB );      //EXAMPLE: RADIO SET BAND 0                ����� �������
void             SET_RADIO_FREQ( uint8_t *Recive_USB );      //EXAMPLE: RADIO SET FREQ 88.2             ������ ���� �������
void             GET_RADIO_FREQ(void);                       //EXAMPLE: RADIO GET FREQ                  ���������� ������� ������� �����
void             GET_SIGNAL_LEVEL( uint8_t *Recive_USB );    //EXAMPLE: RADIO GET RSSI 1                ���������� ������ ������� �������
//void             RADIO_AUTO_SEARCH( uint8_t *Recive_USB );   //EXAMPLE: RADIO AUTO SEARCH 1             �������������� ����� �������: 1 ����� 0 ����
//void             RADIO_MANUAL_SEARCH( uint8_t *Recive_USB ); //EXAMPLE: RADIO MANUAL SEARCH 1           ������ ����� �������: 1 ����� 0 ����

//void             SET_PRESET_RADIO(uint8_t *Recive_USB);      //EXAMPLE: RADIO SET PRESET 1              ����� �������������� �������
void             GET_PRESET_RADIO(void);                     //EXAMPLE: RADIO GET PRESET                ���������� ������� ������
//void             AUTO_SAVE_PRESET_RADIO(void);               //EXAMPLE: RADIO AUTO SAVE PRESET          ��������� ������ � ��������� ������
//void             SAVE_PRESET_RADIO(uint8_t *Recive_USB);     //EXAMPLE: RADIO SAVE PRESET 1             ��������� ������ � ��������� ������
//void             DELETE_PRESET_RADIO(uint8_t *Recive_USB);   //EXAMPLE: RADIO DELETE_PRESET 1           ��������� ������ � ��������� ������
void             GET_ALL_FREQ_PRESET(void);                  //EXAMPLE: RADIO GET ALL PRESET            ���������� ��� �������
float            GetRadioFreq();

void             RadioAutoSearchTimer(void);
void             RadioAutoSearch(short direction);
void             RadioReadRDS( uint8_t *Recive_USB );
void             RadioReadSignalLevel( uint8_t *Recive_USB );
void             RadioReadFreq( uint8_t *Recive_USB );
void             RadioManualSearch(short direction);
void             SetRadioPreset(short preset);
void             RadioGetFreq(void);
void             RadioPresetRouting(uint8_t action, short direction, uint8_t pressed);       // ���������� ������� ���������� 1 ����� 0 ����

void             updateRadioSpacing(void);
void             updateRadioBand(void);

void             RDA5807_autoSearch(short direction);
void             RDA5807_setFreq(void);
void             RDA5807_setBand(void);
void             RDA5807_setSpacing(void);



typedef struct 
{
	uint16_t bENABLE            :1;    // 00 Power Up Enable (0 = Disabled; 1 = Enabled)
	uint16_t bSOFT_RESET        :1;    // 01 Soft reset (0 = not reset; 1 = reset)
	uint16_t bNEW_METHOD        :1;    // 02 New Demodulate Method Enable, can improve the receive sensitivity about 1dB.
	uint16_t bRDS_EN            :1;    // 03 RDS/RBDS enable (1 = rds/rbds enable)
	uint16_t bCLK_MODE          :3;    // 4-6 Xtal freq 000=32.768kHz
	uint16_t bSKMODE            :1;    // 07 Seek Mode (0 = wrap at the upper or lower band limit and continue seeking; 1 = stop seeking at the upper or lower band limit)
	uint16_t bSEEK              :1;    // 08 Seek (0 = Disable stop seek; 1 = Enable)
	uint16_t bSEEKUP            :1;    // 09 Seek Up (0 = Seek down; 1 = Seek up)
	uint16_t bRCLK_DirectInput  :1;    // 10 RCLK DIRECT INPUT MODE (1 = RCLK clock use the directly input mode)
	uint16_t bRCLK_NonCalibMode :1;    // 11 RCLK NON-CALIBRATE MODE
	uint16_t bBASS              :1;    // 12 Bass Boost (0 = Disabled; 1 = Bass boost enabled)
	uint16_t bMONO              :1;    // 13 Mono Select (0 = Stereo; 1 = Force mono)
	uint16_t bDMUTE             :1;    // 14 Mute Disable (0 = Mute; 1 = Normal operation)
	uint16_t bDHIZ              :1;    // 15 Audio Output High-Z Disable (0 = High impedance; 1 = Normal operation)
} tReg02h;

typedef struct
{
	uint16_t bSPACE             :2;    // 0-1 Channel Spacing.
	uint16_t bBAND              :2;    // 2-3 Band Select
	uint16_t bTUNE              :1;    // 04 Tune (0 = Disable; 1 = Enable)
	uint16_t bDIRECT_MODE       :1;    // 05 Directly Control Mode, Only used when test.
	uint16_t bCHAN              :10;   // 6-15 Channel Select
} tReg03h;

typedef struct 
{
	uint16_t bGPIO1             :2;    // 0-1 GPIO1
	uint16_t bGPIO2             :2;    // 2-3 GPIO2
	uint16_t bGPIO3             :2;    // 4-5 GPIO2
	uint16_t bI2S_ENABLED       :1;    // 06 I2S_ENABLED
	uint16_t bRSVD1             :1;    // 07 Reserved
	uint16_t bAFCD              :1;    // 08 AFC disable (0 = afc work; 1 = afc disabled)
	uint16_t bSOFTMUTE_EN       :1;    // 09 1 = softmute enable
	uint16_t bRSVD2             :1;    // 10 Reserved
	uint16_t bDE                :1;    // 11 De-emphasis (0 = 75 µs; 1 = 50 µs)
	uint16_t bRSVD3             :2;    // 12-13 Reserved
	uint16_t bSTCIEN            :1;    // 14 Seek/Tune Complete Interrupt
	uint16_t bRSVD4             :1;    // 15 Reserved
} tReg04h;

typedef struct 
{
	uint16_t bVOLUME            :4;    // 0-3 Volume - DAC Gain Control Bits (0000=min; 1111=max)
	uint16_t bRSVD1             :2;    // 4-5 Reserved
	uint16_t bLNAPORTSEL        :2;    // 6-7 LNA input port selection bit:
	uint16_t bSEEKTH            :4;    // 8-11 Seek SNR threshold value
	uint16_t bRSVD2             :3;    // 12-14 Reserved
	uint16_t bINT_MODE          :1;    // 15 INT MODE (0 = generate 5ms interrupt; 1 = interrupt last until read reg0CH action occurs)
} tReg05h;

typedef struct 
{
	uint16_t bRDELY             :1;    // 00 R channel data delay 1T
	uint16_t bLDELY             :1;    // 01 L channel data delay 1T
	uint16_t bSCLKOEDGE         :1;    // 02 Invert sclk output when as master
	uint16_t bSWOEDGE           :1;    // 03 Invert ws output when as master
	uint16_t bI2SSWCNT          :4;    // 4-7 I2S FS 0000: WS_STEP=8kbps
	uint16_t bWSIEDGE           :1;    // 08 Ws internally
	uint16_t bDATASIGNED        :1;    // 09 I2S output16-bit audio data
	uint16_t bSCLKIEDGE         :1;    // 10 When I2S enable
	uint16_t bSWLR              :1;    // 11 Ws relation to l/r channel
    uint16_t bI2SMODE           :1;    // 12 I2S Mode
    uint16_t bOPEN_MODE         :2;    // 13-14 Open reserved register mode (11 = open behind registers writing function others: only open behind registers reading function
    uint16_t bRSVD1             :1;    // 15 Reserved
} tReg06h;

typedef struct 
{
	uint16_t bFREQ_MODE         :1;     // 00 If 1, then freq setting changed (Freq = 76000(or 87000) kHz + freq_direct (08H) kHz)
	uint16_t bSOFTBLEND_EN      :1;     // 01 Softblend enable
	uint16_t bSEEK_TH_OLD       :6;     // 2-7 Seek threshold for old seek mode, Valid when Seek_Mode=001
	uint16_t bRSVD1             :1;     // 08 Resvered
	uint16_t b65M_50M_MODE      :1;     // 09 Valid when band[1:0] = 2’b11 (0x03H_bit<3:2>)
	uint16_t bTH_SOFRBLEND      :5;     // 10-14 Threshold for noise soft blend setting, unit 2dB
	uint16_t bRSVD2             :1;     // 15 Reserved
} tReg07h;

typedef struct 
{
	uint16_t bREADCHAN          :10;    // 0-9 Read Channel.
	uint16_t bST                :1;     // 10 Stereo Indicator (0 = Mono; 1 = Stereo)
	uint16_t bBLK_E             :1;     // 11 When RDS enable (1 = Block E has been found; 0 = no Block E has been found)
	uint16_t bRDSS              :1;     // 12 RDS Synchronization (0 = RDS decoder not synchronized(default); 1 = RDS decoder synchronized)
	uint16_t bSF                :1;     // 13 Seek Fail (0 = Seek successful; 1 = Seek failure)
	uint16_t bSTC               :1;     // 14 Seek/Tune Complete (0 = Not complete; 1 = Complete)
	uint16_t bRDSR              :1;     // 15 RDS ready (0 = No RDS/RBDS group ready(default); 1 = New RDS/RBDS group ready)
} tReg0Ah;

typedef struct 
{
	uint16_t bBLERB             :2;     // 0-1 Block Errors Level of RDS_DATA_1, and is always read as Errors Level of RDS BLOCK B (in RDS mode ) or E (in RBDS mode when ABCD_E flag is 1)
	uint16_t bBLERA             :2;     // 2-3 Block Errors Level of RDS_DATA_0, and is always read as Errors Level of RDS BLOCK A (in RDS mode) or BLOCK E (in RBDS mode when ABCD_E flag is 1)
	uint16_t bABCD_E            :1;     // 04 (1 = the block id of register 0cH,0dH,0eH,0fH is E; 0 = the block id of register 0cH, 0dH, 0eH,0fH is A, B, C, D)
	uint16_t bRSVD1             :2;     // 5-6 Resvered
	uint16_t bFM_READY          :1;     // 07 1 = ready; 0 = not ready
	uint16_t bFM_TRUE           :1;     // 08 1 = the current channel is a station; 0 = the current channel is not a station
	uint16_t bRSSI              :7;     // 9-15 RSSI (000000 = min; 111111 = max) RSSI scale is logarithmic
} tReg0Bh;




#endif
