#ifndef SOUNDMENU_H_
#define SOUNDMENU_H_

#ifdef __cplusplus
 extern "C" {
#endif
//------------------------------------------------
#include "stm32f4xx_hal.h"
//------------------------------------------------

void           Init_Sound_Param();
void           SoundMenu();
void           I2C_1_Slave_Rx_Cmplt(uint8_t Direction, uint8_t DataLen);
void           I2C_1_EV_IRQHandler();
void           I2C_1_ER_IRQHandler();
void           I2C_1_START_IRQHandler();


#define ADDR_REG_RESTART           100
#define ADDR_REG_USB_AUDIO         125
#define ADDR_REG_USB_AUDIO_STATE   150
#define ADDR_REG_USB_AUDIO_DEVICE  175

typedef enum
{
	Initialization              = 0,  // Initialization of parameters.
	ReadyForWork                = 1,  // Ready for normal operation
	SafeMode                    = 2,  // Errors occurred during initialization, some structures are in standard configuration
	DataSave                    = 3,  // Saving data to energy-independent memory
	WaitingForShutdown          = 4,  // Waiting for platform shutdown
	FirmwareUpdate              = 5,  // Platform firmware update
	DefaultStruct               = 6,  // Structure in standard mode
	PowerDown                   = 7,  // The platform is preparing for a power outage
	BootLoader                  = 8,  // Bootloader mode
	Connected                   = 9,  // There are connections
	Connecting                  = 10, // Waiting for connection
	OutgoingCall                = 11, // Outgoing bluetooth call
	IncomingCall                = 12, // Incoming bluetooth call
	ActiveCall                  = 13, // Bluetooth active call
	Stopped                     = 14, // In stop mode
	Playing                     = 15, // In playback mode
	Paused                      = 16, // Pause mode
	FastForwarding              = 17, // Fast forward playback
	FastRewinding               = 18, // Fast backward playback
	NotFound                    = 19, // Nothing found
	NotConnected                = 20, // No connection
}StatusTDE;

typedef enum
{
	Nothing_Yet         = 0,
	Dixom_HiRes_Audio   = 1
}tUSBAudioModel;

typedef enum
{
	Integrated         = 0,
	External           = 1
}tUSBAudioPcbType;


typedef enum
{
	Read          = 0,
	Write         = 1
}tI2C_Direction;

 typedef enum
 {
	BIT_8           = 0,
	BIT_16          = 1,
 	BIT_24          = 2,
	BIT_32          = 3
 }tBitRate;

 typedef enum
 {
	SFreq_22000    = 0,
	SFreq_44100    = 1,
	SFreq_48000    = 2,
	SFreq_88200    = 3,
	SFreq_96000    = 4,
	SFreq_192000   = 5
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
	 sUsbAudioInfo  State;
	 sDeviceInfo    Device;
 } sUsbAudio;

//------------------------------------------------
#ifdef __cplusplus
}
#endif
#endif /* SOUNDMENU_H_ */
