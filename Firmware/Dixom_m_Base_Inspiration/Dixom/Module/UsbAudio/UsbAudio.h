/*
 * UsbAudio.h
 *
 *  Created on: 12 џэт. 2022 у.
 *      Author: 79825
 */

#ifndef MODULE_USBAUDIO_USBAUDIO_H_
#define MODULE_USBAUDIO_USBAUDIO_H_
#include <Constants.h>
#include <stdint.h>

#define DATA_LEN_DIXOM_HIRES_AUDIO_BOOT    42
#define DATA_LEN_DIXOM_HIRES_AUDIO_NORMAL  2

#define ADDR_REG_RESTART                   100
#define ADDR_REG_USB_AUDIO                 125
#define ADDR_REG_USB_AUDIO_STATE           150
#define ADDR_REG_USB_AUDIO_DEVICE          175

void                   USB_AUDIO_CARD_STRING_CONTROL(uint8_t *Received_String);
void                   Automatic_Detection_USB_Audio_Card_And_Initialization();
void                   InitDixomHiResAudio(void);
void                   Update_AudioCard_Information(uint8_t GetState);
void                   Get_AudioCard_Information();
WorkRes                Transmit_UsbSoundCard( uint8_t* pData, uint8_t type );
void                   Receiver_UsbSoundCard(  uint8_t* pDataSend, uint8_t DataSendLen, uint8_t *pDataRead);
void                   ReceiverIntrenal_UsbSoundCard(uint8_t *pDataRead);
void                   GET_Receiver_UsbSoundCard(uint8_t *pDataIN, uint8_t DataSendLen);

typedef enum
{
	Nothing_Yet         = 0,
	Dixom_HiRes_Audio   = 1
}tUSBAudioModel;

typedef enum
{
	Integrated          = 0,
	External            = 1
}tUSBAudioPcbType;

typedef enum
{
	BIT_8               = 0,
	BIT_16              = 1,
	BIT_24              = 2,
	BIT_32              = 3
}tBitRate;

typedef enum
{
	SFreq_22000        = 0,
	SFreq_44100        = 1,
	SFreq_48000        = 2,
	SFreq_88200        = 3,
	SFreq_96000        = 4,
	SFreq_192000       = 5
}tSamplingFreq;

typedef enum
{
	I2S           = 0,
	TDM           = 1,
}tButType;

typedef enum
{
	Stereo_2_0    = 0,
	Stereo_3_1    = 1,
	Quadro_4_0    = 2,
	Surround_7_1  = 3,
}tAudioConfig;

typedef struct
{
	uint8_t   Playback        :1;
	uint8_t   Mute            :1;
	uint8_t   SamplingFreq    :4;
	uint8_t   BusType         :2;

	uint8_t   BitRate         :3;
	uint8_t   USB             :5;

	uint8_t   InMode          :5;
	uint8_t   AudioConfig     :3;

	uint8_t   Volume;
} sUsbAudioInfo;

typedef struct
{
	uint16_t  Version_Main;
	uint8_t   Version_Bootloader ;
	uint8_t   Model :4;
	uint8_t   Pcb_Type :4;
	uint8_t   Circuit_Revision;
	uint8_t   External_PcbRevision;
} sDeviceInfo;

typedef struct
{
	sUsbAudioInfo State;
	uint8_t       StateSize;
	sDeviceInfo   DeviceInfo;
	uint8_t       VerFrimware[6];
	uint8_t       VerBootloader[5];
	uint8_t 	  InMode;
} sUsbAudio;

#endif /* MODULE_USBAUDIO_USBAUDIO_H_ */
