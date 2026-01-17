#ifndef __BLUETOOTH_H__
#define __BLUETOOTH_H__

#include "stdint.h"
#include "Constants.h"


/*
typedef struct  {
	uint8_t  ModuleName[17];
	uint8_t	 ModulePass[17];
	uint8_t	 ModuleAddr[18];

	uint8_t	 PairName[18];
	uint8_t	 PairAddr[18];
	uint8_t	 PairCount;
	uint8_t	 PairListAddr[15][18];
	uint8_t	 PairListName[15][18];
} tBT_module;

tBT_module BT_module;*/

#define BtModuleInfoLen      18
#define BtMaxPairCount       8
#define BtPairInfoNameLen    24
#define BtPairInfoAddrLen    13
#define NUM_ELEM_BT_SETTINGS 10

void BLUETOOTH_SETTINGS(uint8_t ControlByte, uint8_t *Received_String);
void Bluetooth_Settings(uint8_t ControlByte, uint8_t param, uint8_t data);
uint8_t Bluetooth_Settings_Default();
uint8_t Bluetooth_EEPROM_Read();
uint8_t Bluetooth_EEPROM_Write();

typedef struct
{
	uint8_t      Name[60];
	uint8_t      MacAddr[BtModuleInfoLen];
	uint8_t      NetworkName[BtModuleInfoLen];
	uint8_t      LastPhoneNumber[BtModuleInfoLen];
	uint8_t      PairListAddr[BtMaxPairCount][BtPairInfoAddrLen];
	uint8_t      PairListName[BtMaxPairCount][BtPairInfoNameLen];
	uint8_t      PairCount;
	uint8_t      NetworkLevl;
	uint8_t      A2dpDecoder;
	uint8_t      MediaState;
	uint8_t      SpeakerphoneState;
	uint32_t     TimeTotal;
	uint32_t     TimeProgress;
} sBtConDevInf;


typedef enum
{
	Baud9600       = 0,
	Baud19200      = 1,
	Baud38400      = 2,
	Baud57600      = 3,
	Baud115200     = 4,
	Baud230400     = 5,
	Baud460800     = 6,
	Baud921600     = 7, //default
} TDE_BaudRate;

typedef enum
{
	BtBaudrate            = 0,
	BtControlDirectMode   = 1,
	BtThroughputMode      = 2,
	BtDeliveryGuarantee   = 3,
}TDE_BtSettings;


static const uint8_t  DEFAULT_BT_SETTINGS[10] = {
	Baud115200,
    OFF,
    OFF,
	ON,
	0,
	0,
	0,
	0,
	0,
	0,
};


typedef struct{
	uint8_t Data;
} sBtSettings;

typedef struct
{
	uint32_t      CheckSum;
	sBtSettings   Settings[NUM_ELEM_BT_SETTINGS];
	uint16_t      DataSaveSize;
	uint8_t       NeedToSave;
	uint8_t       InMode;
	uint8_t       Connection;
	uint8_t       BT_Control_Mode;
	uint8_t       SppThroughputMode;
	uint8_t       bBuff[65];
	uint8_t       Model[BtModuleInfoLen];
	uint8_t       MacAddr[BtModuleInfoLen];
	uint8_t       Name[BtModuleInfoLen];
	uint8_t       Pass[BtModuleInfoLen];
	uint8_t       VerFrimware[15];
	uint8_t       dModel;
	uint16_t      dVerFrimware;
	sBtConDevInf  ConnDevInfo;
} sBluetooth;



/************Volume***************/
#define MaxVolume               60

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

uint32_t         GetTotal();
uint32_t         GetProgress();
uint8_t          GetNetvorkSignal();

void             UpdteMediaProgress(uint32_t total, uint32_t progress);
void             BLUETOOTH_Settigns_Update(uint8_t point, uint8_t data);
void             Initialization_Bluetooth(void);
void             InitBluetooth();
void             BLUETOOTH_Menu(uint8_t ControlByte, uint8_t Function, uint8_t *Recive_data);
void             BLUETOOTH_Load_All(void);
void             BLUETOOTH_Data_Receiver(uint8_t *data, uint8_t from);
void             Bluetooth_Loop();
void             BLUETOOTH_Data_out(uint8_t* data, uint8_t dataLen, uint16_t timeDelay, uint8_t from, uint8_t param);


void BT_module_param_clear();

void BLUETOOTH_Transmit(char *Recive_data);

void BLUETOOTH_UART_Resive();

void Bluetooth_Track_Next(void);

void Bluetooth_Track_Prev(void);

void Bluetooth_Forward(void);

void Bluetooth_Rewind(void);

void Phone_IncomingCall(void);
void Phone_OutgoingCall(void);
void Phone_ActiveCall(void);
void Phone_CompletedCall(void);

void                   FSC_Bluetooth_Transmit(uint8_t *data, uint8_t dataLen, uint16_t timeDelay, uint8_t from);
void                   FSC_Bluetooth_TransmitSPP(uint8_t *data, uint8_t dataLen, uint16_t timeDelay, uint8_t from);




#define MediaVolUp         "VOL UP"
#define MediaVolDown       "VOL DOWN"
#define MediaMute          "MUTE"
#define MediaStop          "STOP"
#define MediaPlay          "PLAY"
#define MediaPause         "PAUSE"
#define MediaPlayPause     "PLAYPAUSE"
#define MediaFast_Forward  "FAST_FORWARD"
#define MediaRewind        "REWIND"
#define MediaForward       "FORWARD"
#define MediaBackward      "BACKWARD"

#define PhoneReply         "REPLY"
#define PhoneCancel        "CANCEL"
#define PhoneRedial        "REDIAL"



#endif
