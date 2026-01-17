#ifndef __FRIMWAREUPDATE__
#define __FRIMWAREUPDATE__

#include "stdint.h"

typedef enum
{
	Ok_Frimware     = 10,
	New_Frimware    = 20,
	Bad_Frimware    = 100
}tUpdateState;

typedef enum
{
	STABLE     = 0,
	EXPERIMENTAL = 1,
}tExperementalFirmware;

void UPDATE_CANCEL();
void SYSTEM(uint8_t ControlByte, uint8_t Function, uint8_t *Received_data);
void UPDATE_SYSTEM_UNLOCK(uint8_t *Received_data);
void CHECK_FIRMWARE();

void COMPLETE_UPDATE();
void PREPARING_UPDATE(uint8_t *Received_data);
void FIRMWARE_DOWNLOAD(uint8_t *Received_data);

void ConnectionSetup(uint8_t ControlByte, uint8_t *Received_data);
void ProtectionBootUpdate(void);
void ProtectionSpiFlashErase(void);
void ProtectionSetKey(uint8_t *Received_data);
void ProtectionSetPcb(uint8_t *Received_data);

#define          SpiFlashAddrUpdateInfo   4080
#define          FlashAddrUpdateFlag      4081
#define          FlashAddrFrimwareLen     4075

int16_t          getCurrentBootVersion();
void             BootUpdate(void);
void             Only_BootUpdate(void);

void             UpdateWrite(uint8_t *Recive_USB);
void             UpdateRead(uint8_t *Recive_USB);

void             Firmware_Update_Loop();


/*
#define FrimwareUpdateSetup        48
#define FrimwareUpdateWrite        49
#define FrimwareUpdateEnd          50
#define FrimwareUpdateAppStart     51
#define FrimwareUpdateBlock        52
#define FrimwareUpdateUnlock       53
#define FrimwareVersion            48
#define FrimwareUpdateStatus       49*/


#endif
