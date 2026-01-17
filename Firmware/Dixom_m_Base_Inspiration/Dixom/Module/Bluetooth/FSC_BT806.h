#ifndef __BT_FSC_BT806_H__
#define __BT_FSC_BT806_H__
#include "Bluetooth.h"


typedef enum
{                      //  ASCII
	R            = 13, // /r
	N            = 10, // /n
	Zy           = 44, // ,
	Text         = 31,
	Stx          = 2,  // (Start of Text) (Start transmission)
	Eot          = 4,  // (End of Transmission)
	Ack          = 6,  // Acknowledge
	Nak          = 21, // Negative Acknowledge
	Empty        = 8,
	End_Mesage   = 0  //ERROR ASII
}BtReadState;



/***********BLUETOOTH HFPSTATE************/
#define HFP_State_Unsupported           '0'
#define HFP_State_Standby               '1'
#define HFP_State_Connecting            '2'
#define HFP_State_Connected             '3'
#define HFP_State_OutgoingCall          '4'
#define HFP_State_IncomingCall          '5'
#define HFP_State_ActiveCall            '6'

/**********BLUETOOTH PLAYSTATE************/
#define PLAY_State_MediaStopped         '0'
#define PLAY_State_MediaPlaying         '1'
#define PLAY_State_MediaPaused          '2'
#define PLAY_State_MediaFastForwarding  '3'
#define PLAY_State_MediaFastRewinding   '4'


typedef struct
{
	uint8_t  Power               :1;
	uint8_t  BR_EDR_Discoverable :1;
	uint8_t  BLE_Advertising     :1;
	uint8_t  BR_EDR_Scaning      :1;
	uint8_t  BLE_Scaning         :1;
} tDEVSTAT;








#define Mode_MediaStopped         11
#define Mode_MediaPlaying         12
#define Mode_MediaPaused          13
#define Mode_MediaFastForwarding  14
#define Mode_MediaFastRewinding   15

int             FscGetSppData(uint8_t* data, uint16_t len);

void             FscSpeakerphoneState(uint8_t state);
void             FSC_Bluetooth_Receiver_buff(uint8_t* data, uint8_t from);
void             FSC_Bluetooth_Receiver(uint8_t data);
//void             FSC_Bluetooth_Transmit(uint8_t *data, uint8_t dataLen, uint16_t timeDelay, uint8_t from);
uint8_t          BluetoothMenu(  uint8_t* Recive_data, uint8_t len);
uint16_t         incCircleBufferVariable(uint16_t var);
void             UpWritePointer(void);
void             DownWritePointer();
void             ScanBtDeviceDisconnecting();
int              LoopBT(void);
void             MEDIA_FSC(uint8_t *Recive_data);
void             PHONE_FSC(uint8_t *Recive_data);
void             SPP_FSC(uint8_t *Recive_data);
void             DOWNLOAD_CALLS_FSC(uint8_t *Recive_data);
void             DOWNLOAD_PBOOK_FSC(uint8_t *Recive_data);
void             Transmit_Long_Data_Out(uint8_t* StartSuf, uint8_t StartSufLen, uint8_t* data, uint8_t delay);

void FscSppData(uint8_t* data, short len);
void FscSppState(uint8_t* data, short len);


#endif
