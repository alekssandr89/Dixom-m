#ifndef __DSP_CONSTANT_H__
#define __DSP_CONSTANT_H__

#include <Constants.h>








#define EQ_                         "EQ "
#define STATUS_EQ_                  "STATUS EQ "
#define CROSS_                      "CROSS "
#define PRESET_EQ_                  "PRESET EQ "
#define PRESET_CROSS_               "PRESET CROSS "
#define VOL_                        "VOL "
#define DELAY_                      "DELAY "
#define SOURCE_                     "SOURCE "
#define INVERT_                     "INVERT "
#define BTN_                        "BTN "
#define SETTINGS_                   "SETTINGS "
#define TIMER_                      "TIMER "
#define PHATSTEREO_                 "PHATSTEREO "
#define FILTR                       "FILTR"
#define ALL                         "ALL"

#define DATA_LENGTH_20            20
#define DATA_LENGTH_12            12
#define DATA_LENGTH_4             4
/*

*/

#define CONV_FLOAT                0.01
#define CONV_INT                  100


#define DacVolum                  0
#define MasterVolum               10


#define COUNT_USER_PRESET         4  //3 user preset + 1 main
/************EQ Preset************/
#define PresetOff                 0
#define PresetUser1               1
#define PresetUser2               2
#define PresetUser3               3
#define PresetUser4               4
#define PresetUser5               5
#define PresetFlat                6
#define PresetBass                7
#define PresetBassExtreme         8
#define PresetBassAndHigh         9
#define PresetHigh                10
#define PresetClassic             11
#define PresetDancing             12
#define PresetRock                13
#define PresetTehno               14
#define PresetSpeaker             15
#define PresetLive                16
#define PresetMedium              17
#define PresetSoft                18
#define PresetSoftBass            19
#define PresetSoftHigh            20


#define GAIN_MIN                  -20
#define GAIN_MAX                  10



#define UNIQ_ELEM_CH_QE            15

typedef enum
{
	SoursUsb             =0,
	SoursBluetoooth      =1,
	SoursAux             =2,
	SoursRadio           =3,
	SoursSpdif           =4,
	SoursMic             =5
}TDE_SoundzSource;




typedef enum
{
	General_EQ =0,
	CH1        =1,
	CH2        =2,
	CH3        =3,
	CH4        =4,
	CH5        =5,
	CH6        =6,
	CH7        =7,
	CH8        =8,
	CH9        =9,
	CH10       =10,
	CH11       =11,
	CH12       =12,
	SPDIF_L    =13,
	SPDIF_R    =14
}TDE_AudioChanel;


typedef enum
{
	Band1 =0,
	Band2 =1,
	Band3 =2,
	Band4 =3,
	Band5 =4,
	Band6 =5,
	Band7 =6,
	Band8 =7,
	Band9 =8,
	Band10 =9,
	Band11 =10,
	Band12 =11,
	Band13 =12,
	Band14 =13,
	Band15 =14,
	Band16 =15,
	Band17 =16,
	Band18 =17,
	Band19 =18,
	Band20 =19,
	Band21 =20,
	Band22 =21,
	Band23 =22,
	Band24 =23,
	Band25 =24,
	Band26 =25,
	Band27 =26,
	Band28 =27,
	Band29 =28,
	Band30 =29
}TDE_EqBandNum;


typedef enum
{
	FILTER_OFF           =0,//GOOD
	PEAK                 =1,//GOOD
	LOWPASS              =2,//GOOD
	HIGHPASS             =3,//GOOD
	BUTTERWORTH_LP       =4,//GOOD
	BUTTERWORTH_HP       =5,//GOOD
	BESSEL_LP            =6,//GOOD
	BESSEL_HP            =7,//GOOD
	LOWPASS_FIRST        =8,//GOOD
	HIGHPASS_FIRST       =9,//GOOD
	LOW_SHELF            =10,//GOOD
	HIGH_SHELF           =11,//GOOD
	LINKWITZ_RILEY_HP    =12,//NO GOOD
	LINKWITZ_RILEY_LP    =13,//NO GOOD
	BANDPASS             =14,//NO GOOD
	BANDSTOP             =15//NO GOOD
}TDE_FilterType;


typedef enum
{
	High_Pass =0,
	Low_Pass  =1
}TDE_CrossoverFilterType;


typedef enum
{
	Main_Preset   =0,
	User_preset1  =1,
	User_preset2  =2,
	User_preset3  =3
}TDE_Preset;


typedef enum
{
	VolumControlOnDAC            =0,
	VolumControlOnDSP            =1,
	VolumControlOnLoudness       =2
}TDE_VolumeControlMode;


/***********Filter Types**********/
/*#define FILTER_OFF                0  //GOOD
#define PEAK                      1  //GOOD
#define LOWPASS                   2  //GOOD
#define HIGHPASS                  3  //GOOD
#define BUTTERWORTH_LP            4  //GOOD
#define BUTTERWORTH_HP            5  //GOOD
#define BESSEL_LP                 6  //GOOD
#define BESSEL_HP                 7  //GOOD
#define LOWPASS_FIRST             8  //GOOD
#define HIGHPASS_FIRST            9  //GOOD
#define LOW_SHELF                 10 //GOOD
#define HIGH_SHELF                11 //GOOD
#define LINKWITZ_RILEY_HP         12 //NO GOOD
#define LINKWITZ_RILEY_LP         13 //NO GOOD
#define BANDPASS                  12 //NO GOOD
#define BANDSTOP                  13 //NO GOOD
*/

#define SAMPLE_RATE_22050         22050
#define SAMPLE_RATE_32000         32000
#define SAMPLE_RATE_44100         44100
#define SAMPLE_RATE_48000         48000
#define SAMPLE_RATE_88200         88200
#define SAMPLE_RATE_96000         96000
#define SAMPLE_RATE_192000        192000
#define SAMPLE_RATE_348000        348000
#define SAMPLE_RATE_768000        768000

#define GAIN_CONST                0
#define CrossoverCount            14
#define EqualizerCount            15

#define DELAY_SAMPLE              1440  //���������� ������ �������� ������� � DSP ��� ��������

/***********Filter Types**********/
/*#define FILTER_OFF                0  //GOOD
#define PEAK                      1  //GOOD
#define LOWPASS                   2  //GOOD
#define HIGHPASS                  3  //GOOD
#define BUTTERWORTH_LP            4  //GOOD
#define BUTTERWORTH_HP            5  //GOOD
#define BESSEL_LP                 6  //GOOD
#define BESSEL_HP                 7  //GOOD
#define LOWPASS_FIRST             8  //GOOD
#define HIGHPASS_FIRST            9  //GOOD
#define LOW_SHELF                 10 //GOOD
#define HIGH_SHELF                11 //GOOD
#define LINKWITZ_RILEY_HP         12 //NO GOOD
#define LINKWITZ_RILEY_LP         13 //NO GOOD
#define BANDPASS                  12 //NO GOOD
#define BANDSTOP                  13 //NO GOOD*/




/*****Filter Default Values******/
#define FILTER_EQ_SOURCE_Q_DEF    71    //*0,01
#define FILTER_CROSS_Q_DEF        71    //*0,01
#define FILTER_EQ_CH_Q_DEF        250   //*0,01
#define FILTER_TYPE_DEF           1
#define FILTER_FREQ_DEF           20
#define FILTER_BOOST_DEF          0      //*0,01
#define FILTER_ORDER_DEF          2
#define FILTER_EQ_ONOFF_DEF       ON
#define FILTER_CROSS_ONOFF_DEF    OFF
#define FILTER_POINT_DEF          0
#define DEFAULT_INVERT            0
#define DEFAULT_DELAY             0     //*0,01
#define DEFAULT_GAIN              0

/*****Unical element struct******/
#define NUM_ELEM_EQ_GENERAL      30
#define NUM_ELEM_DELAY           14
#define NUM_ELEM_INVERT          26
#define NUM_ELEM_PHATSTEREO      6
#define NUM_ELEM_WIDESTEREO      6
#define NUM_ELEM_SUPPERBASS      6
#define NUM_ELEM_VOL             248
#define NUM_ELEM_FILTR           270
#define NUM_ELEM_EQ_CENTR        30
#define NUM_ELEM_CROSSOVER       14
#define NUM_ELEM_CROSSOVER_TYPE  2
#define NUM_ELEM_EQ_CH           14
#define NUM_ELEM_GROUP           8
#define NUM_PRESET               3
#define NUM_ELEM_EQ_CH_BAND      15
#define NUM_ELEM_EQ_SOURCE_BAND  5
#define NUM_ELEM_DAC             13
#define NUM_ELEM_LOUDNESS        6
#define NUM_ELEM_LIMITER         14
#define NUM_ELEM_GAIN            14
#define NUM_ELEM_GROUP_NAME      7
#define NUM_ELEM_GROUP_DELAY     7
#define NUM_ELEM_LVL_DETECTED    64
#define NUM_ELEM_GRAPHIC_EQ      32
#define NUM_ELEM_DSP_SETTINGS    64
#define NUM_ELEM_SOURCE          6
#define NUM_ELEM_VOL_EEPROM      50

/*******Parametr MAX MIN ************/
#define INVERT_MIN                0
#define INVERT_MAX                1
#define DELAY_MAX                 1000
#define DELAY_MIN                 0
#define VOL_MAX                   60
#define VOL_MIN                   0
#define FREQ_MAX                  22000
#define FREQ_MIN                  20
#define Q_MAX                     3000     //*0.01 = 30.00
#define Q_MIN                     5        //*0.01 = 00.05
#define BOOST_MAX                 2000     //*0.01 = 20.00
#define BOOST_MIN                 -2000    //*0.01 =-20.00
#define NAME_MAX_LEN              48
#define SBASSFREQ_MIN             20
#define SBASSFREQ_MAX             500
#define SBASSLVL_MIN              1
#define SBASSLVL_MAX              30
#define SBASSINTESIV_MIN          1
#define SBASSINTESIV_MAX          30
#define NO_GROUP                  255

#define ORDER_MAX                 10
#define ORDER_MIN                 1
#define SOUND_DELAY_MAX           3000
#define SOUND_DELAY_MIN           0
#define WIDESTEREO_MIN            0
#define WIDESTEREO_MAX            20
#define DEF_SINGEN_FREQ           1500
#define DEF_SINGEN_PHASE          0

/*********Point Strict **************/
#define EQ_GENERAL                0
#define FILTR_CH1                 1
#define FILTR_CH2                 2
#define FILTR_CH3                 3
#define FILTR_CH4                 4
#define FILTR_CH5                 5
#define FILTR_CH6                 6
#define FILTR_CH7                 7
#define FILTR_CH8                 8
#define FILTR_CH9                 9
#define FILTR_CH10                10
#define FILTR_CH11                11
#define FILTR_CH12                12
#define FILTR_SPDIFL              13
#define FILTR_SPDIFR              14

#define CrossCH1                  15
#define CrossCH2                  16
#define CrossCH3                  17
#define CrossCH4                  18
#define CrossCH5                  19
#define CrossCH6                  20
#define CrossCH7                  21
#define CrossCH8                  22
#define CrossCH9                  23
#define CrossCH10                 24
#define CrossCH11                 25
#define CrossCH12                 26
#define CrossSPDIFL               27
#define CrossSPDIFR               28

/*****Crossover order******/
#define CROSS_ORDER_1             1
#define CROSS_ORDER_2             2
#define CROSS_ORDER_3             3
#define CROSS_ORDER_4             4
#define CROSS_ORDER_5             5
#define CROSS_ORDER_6             6
#define CROSS_ORDER_7             7
#define CROSS_ORDER_8             8
#define CROSS_ORDER_9             9
#define CROSS_ORDER_10            10

/************Volume***************/
#define MaxVolumUsb               0
#define MaxVolumBluetooth         1
#define MaxVolumAux               2
#define MaxVolumRadio             3
#define MinVolumMic               4
#define MinVolumUsb               5
#define MinVolumBluetooth         6
#define MinVolumAux               7
#define MinVolumRadio             8
#define MinVolumSpdif             9
#define VolumUsb                  11
#define VolumBluetooth            14
#define VolumAux                  17
#define VolumRadio                20
#define VolumSpdif                23
#define VolumSoursMic             126

#define VolumGroup1               29
#define VolumGroup2               32
#define VolumGroup3               35
#define VolumGroup4               38
#define VolumGroup5               41
#define VolumGroup6               44
#define VolumGroupSPDIF           47
/*
#define DspNoiseSwitchAddr        MOD_GENNOISE_ENABLENOISE_ADDR
#define DspNoiseGainAddr          MOD_GENNOISE_ENABLENOISE_ADDR
#define DspSinGenSwitchAddr       MOD_GENSINUS_ALG0_SINEPHASEGAINALGS3001GAIN00_ADDR
#define DspSinGenFreqAddr         MOD_GENSINUS_ALG0_INCREMENT_ADDR*/

#endif
