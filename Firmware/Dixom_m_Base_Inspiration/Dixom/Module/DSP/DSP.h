#ifndef __DSP_H__
#define __DSP_H__

#include "DSP_Constant.h"
#include "DSP_InternalCalls.h"
#include "Settings.h"

WorkRes DSP_Module_Initialization_Final();
WorkRes DSP_Struct_Flash_Read();
void    DSP_Flash_NeedToSave();
void    DSP_EEPROM_NeedToSave();
WorkRes DSP_Struct_Flash_Write();
WorkRes DSP_Struct_EEPROM_Read();
WorkRes DSP_Struct_EEPROM_Write();
WorkRes DSP_Struct_Chip_Init();
void DSP_SHOW_PARAMETERS(uint8_t ShowType);
void DSP_Loop();

typedef struct
{
	uint8_t  data;
}sLvlDetec;

typedef struct
{
	uint8_t  data;
}sGraphicEQ;

typedef struct{
	uint16_t addrInDsp;
	float    DataGain;
} sGain;

typedef struct
{
	sFiltrParam  Band[30];                 //3*30 = 90
	sGain        Gain;                     //1
	uint8_t      Status;                   //1
	uint8_t      StandardPreset;
	uint16_t     Reserved1;
}sEqualizerGL; //TOTAL SIZE 92 word

typedef struct
{
	sFiltrParam  Band[15];                //3*15= 45
	sGain        Gain;                    //1
	uint8_t      Bypass;                  //1
//	uint8_t      Group;
	uint16_t     Reserved1;

}sEqualizerCH; //TOTAL SIZE 47 word

typedef struct
{
	sFiltrParam  Band[5];                 //3*5= 15
	uint16_t     Reserved1;
}sEqualizerSource; //TOTAL SIZE 47 word

typedef struct
{
	sFiltrParam  Filter[2];
}sCrossover;   //TOTAL SIZE 6 word

typedef struct{
	uint16_t startAddess;        //   2 byte
	uint16_t LevelHP;            //   2 byte
	uint16_t FreqHP;             //   2 byte
	uint16_t LevelLP;            //   2 byte
	uint16_t FreqLP;
	uint16_t reserved1 ;

} sLoudness; //TOTAL SIZE 3 word

typedef struct{
	uint16_t startAddess;        //   2 byte
	uint16_t ThresholdAddess;
	uint16_t RmsTCAddess;
	uint16_t DecayAddess;
	uint16_t Threshold;
	uint16_t RmsTC;
	uint16_t Decay;
	uint16_t  reserved;
} sLimiter; //TOTAL SIZE 4 word

typedef struct{
	uint16_t addrInDsp;
	int16_t  DataVol;
} sVolumControl; //TOTAL SIZE 1 word


typedef struct{
	uint16_t dspAddessFreq;
	uint16_t dspAddessGain;
	uint16_t dspAddessInte;
	uint16_t Freq;
	uint8_t  State;
	uint8_t  Intensity;
	int16_t  Gain;
} sSupBass; //TOTAL SIZE 3 word

typedef struct{
	uint16_t addrInDsp;
	uint8_t  DataInvert;
	uint8_t  reserved1;
} sInvert;  //TOTAL SIZE 1 word

typedef struct{
	uint16_t addrInDsp;
	int16_t  Gain;
} sWideStereo; //TOTAL SIZE 1 word

typedef struct{
	uint16_t addrInDsp;
	uint16_t DataDelay;
} sDelay;  //TOTAL SIZE 1 word

typedef struct{
	uint16_t addrInDsp;
	uint8_t  Gain;
	uint8_t  editing;
} sSWideStereo;

typedef struct{
	uint16_t      Size;                //| 1 word  DSP structure size
	uint16_t      UserPresetSize;      //|         The size of the sDSP structure preset.

	uint8_t       VersionDSP;          //| 1 word  DSP SigmaStudio project version
	uint8_t       Status:4;            //|         Displaying the state of the structure, the presence of errors, etc.
	uint8_t       NeedToSave:1;        //|         If 1(true) then we save the structure into stm32 memory. if 0(false) then no.
	uint8_t       reserved0:3;         //|
	uint8_t       reserved1;           //|
	uint8_t       reserved2;           //|

	uint32_t      reserved3;           //  1 word
	uint32_t      reserved4;           //  1 word
} sDspStrInfo;

typedef enum
{
	EqStandardPresets             =0,
	UserPresset                   =1,
	VolumeControlMode             =2,
	Selfbot                       =3,
	DspInitialization             =4,
	SoundSource                   =5,
	Sound_Routing_USB             =6,
	Sound_Routing_Bluetooth       =7,
	Sound_Routing_Aux             =8,
	Sound_Routing_Radio           =9,
	Sound_Routing_Spdif           =10,
	Sound_Routing_Mic             =11,
	Preset_Routing_User1          =12,
	Preset_Routing_User2          =13,
	Preset_Routing_User3          =14,
	Preset_Routing_Flat           =15,
	Preset_Routing_Bass           =16,
	Preset_Routing_BassExtreme    =17,
	Preset_Routing_BassAndHigh    =18,
	Preset_Routing_Classic        =19,
	Preset_Routing_Dancing        =20,
	Preset_Routing_Speaker        =21,
	Preset_Routing_Live           =22,
	Preset_Routing_Medium         =23,
	Preset_Routing_Soft           =24,
	Preset_Routing_SoftBass       =25,
	Preset_Routing_SoftHigh       =26,
	LoudnessUsb                   =27,
	LoudnessBluetooth             =28,
	LoudnessRadio                 =29,
	LoudnessSpdif                 =30,
	LoudnessMic                   =31,
	LEVEL_DETECTOR                =32,
	VolumLevelPoewrUp             =33,
	pTempHysteresis               =34,
	psTempOS                      =35,

}TDE_DspSettings;

typedef struct{
	uint8_t Data;
} sDspSettings;

typedef struct{
	uint16_t addrSwitchInDsp;
	uint16_t addrFreqInDsp;
	uint16_t Freq;
	uint8_t  State;
	uint8_t  editing;
} sSineGenerator;

typedef struct{
	uint16_t addrSwitchInDsp;
	uint16_t addrGainInDsp;
	uint16_t Gain;
	uint8_t  State;
    uint8_t  editing;
} sNoiseGenerator;

typedef struct{
	uint8_t StateMute         :2;
	uint8_t StateAtt          :2;
	uint8_t TouchedVolControl :4;
	uint8_t oldMuteVolum;
	uint8_t oldAttVolum;
	uint8_t oldAttRearVolum;

} sMuteATT;


typedef struct{
//	sEqualizerGL     EQ_General;                         // 92 word
	sEqualizerSource EQ_Source [NUM_ELEM_SOURCE];
	sEqualizerCH     EQ_CH[NUM_ELEM_EQ_CH];	            // 705 word
	sCrossover       Crossover[NUM_ELEM_CROSSOVER];      // 90 word
	sInvert          Invert[NUM_ELEM_INVERT];            // 27 word
	sDelay           SoundDelay[NUM_ELEM_DELAY];         // 15 word
}sPresetData; //TOTAL SIZE 929 word


typedef struct{
	sPresetData     Data[COUNT_USER_PRESET];           // 929
	uint32_t        Resreved0;                         //1
} sPreset;


typedef struct{
	uint8_t Preset[NUM_PRESET][NAME_MAX_LEN];
	uint8_t Channel[NUM_ELEM_EQ_CH][NAME_MAX_LEN];
	uint8_t Group[NUM_ELEM_GROUP_NAME][NAME_MAX_LEN];
	uint8_t Delay[NUM_ELEM_GROUP_DELAY][NAME_MAX_LEN];
} sSoundLabel;

typedef struct{
	uint8_t Channel[NUM_ELEM_EQ_CH];
	uint8_t Delay[NUM_ELEM_EQ_CH];
	uint8_t State[NUM_ELEM_GROUP];
} sSoundGroup;

typedef struct
{
	//Part of a preset structure                        //  ATTENTION!!! CheckSum must be at the top of the structure !!!
	uint32_t        CheckSum;                           //  Checksum of the structure, calculated before storing the structure in the flash-memory of STM32
	sDspStrInfo     StructInfo;                         //  4 word
	sPreset         Preset;                             //  930
	sLimiter        Limiter[NUM_ELEM_LIMITER];          //  56 word
	sVolumControl   Volume[NUM_ELEM_VOL];               //  248 word
	sSoundLabel     Lable;
	sSoundGroup     Group;
	sPhatStereo     PhatStereo[NUM_ELEM_PHATSTEREO];    //  12 word
	sSupBass        SupperBass[NUM_ELEM_SUPPERBASS];    //  18 word
	sWideStereo     WideStereo[NUM_ELEM_WIDESTEREO];    //  6 word
	sLoudness       Loudness[NUM_ELEM_LOUDNESS];        //  18 word
	sDspSettings    Settings[NUM_ELEM_DSP_SETTINGS];    //  64/4 = 16 word
	sLvlDetec       LvlDetec[NUM_ELEM_LVL_DETECTED];    // 1*64/4 =16 word
	sGraphicEQ      GraphicEQ[NUM_ELEM_GRAPHIC_EQ];     // 1*32/4 = 8 word
	sSineGenerator  SineGenerator;                      // 2 word
	sNoiseGenerator NoiseGenerator;                     // 2 word

	uint16_t        SAMPLE_RATE;
	sMuteATT        MuteATT;
} sDSP;



typedef struct
{
	uint32_t        CheckSum;
	uint8_t         Volume[NUM_ELEM_VOL_EEPROM];
    uint8_t         SoundSource;
    uint8_t         EqStandardPresets;
    uint8_t 	    UserPresset;
    uint8_t 	    Reserved0;
} sDSP_EEPROM;

typedef struct
{
	uint8_t    availability;
} sMiniDSP;

typedef struct
{

}sDSP_Aux;







#endif
