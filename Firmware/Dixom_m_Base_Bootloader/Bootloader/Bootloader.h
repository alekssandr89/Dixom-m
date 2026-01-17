#ifndef __BOOTLOADER_H__
#define __BOOTLOADER__

typedef enum
{
	Ok_Frimware     = 10,
	New_Frimware    = 20,
	Bad_Frimware    = 100

}tUpdateState;

typedef struct
{
	uint32_t     CheckSum;
	uint32_t     ApplicationAddr;
	uint8_t      State :7;
	uint8_t      StateIWDG :1;
	uint8_t      Erase_Sector :4;
	uint8_t      Erase_Count  :4;
	uint16_t     Boot_VerFrimware;
	uint32_t     Firmware_Size;
	uint32_t     Frimware_CRC;
}sUpdateInfo;

typedef struct
{
	uint32_t     CheckSum;
	uint8_t 	 InMode;
	uint16_t     dRevPCB;
	uint8_t      dType;
    uint8_t      dModel;
	uint8_t      ActivationStatus;
	uint16_t     dVerFrimware;
	uint8_t      RevPCB[4];
	uint8_t      Type[16];
	uint8_t      Model[16];
	uint8_t      VerBootloader[6];
	uint8_t      VerFrimware[6];
	uint8_t      FirmwareName[12];
	uint8_t      SerialNumber[21];
	uint8_t      LicenseKey[14];
	uint8_t   	 MiniSetting[8];
	sUpdateInfo  Firmware_UpdateInfo;
} sDevInfo;  //size 32

typedef struct
{
	uint8_t     prefix[16];
	uint8_t     auxiliary[4];
	uint8_t     data[65];
}sDataOut;



typedef enum{
	BYTE_WHOLE        = 41, // ')',
}FrameEncoding;
typedef enum
{
	PACKED_SYSTEM            = 0,
}ePACKED;

typedef enum
{
	SYSTEM_FU_USB_SOUND_CARD      = 0,
	SYSTEM_NOTIFICATION           = 1,
	SYSTEM_SETTINGS               = 2,
	SYSTEM_INFORMATION            = 3,
	SYSTEM_ACCESS_CONTROL         = 4,
	SYSTEM_EXCHANGE_INTERFACE     = 5,
	SYSTEM_RESTART                = 6,
	SYSTEM_FU_FIRMWARE_DOWNLOAD   = 7,
	SYSTEM_FU_PREPARING_UPDATE    = 8,
	SYSTEM_FU_COMPLETE_UPDATE     = 9,
	SYSTEM_FU_CHECK_FIRMWARE      = 10,
	SYSTEM_FU_CANCEL              = 11,
	SYSTEM_FU_BOOTLOADER          = 12,
	SYSTEM_SPI_FLASH_STATE        = 13,
	SYSTEM_SPI_FLASH_ERASE        = 14,
	SYSTEM_LICENSE_KEY            = 15,
	SYSTEM_PCB_REVISION           = 16,
	SYSTEM_CONNECTION             = 17,
}ePACKED_SYSTEM;

#define MAX_HID_MESSAGE_SIZE  62

#define Basis_Notification_1_1_4           "1 1 3 'Not found'"
#define SysMessage_ERROR                   "1 1 1 'Command entry error'"
#define MessageUpdate_2_1_0                "0 1 0 'Update preparation done'"
#define MessageUpdate_2_1_1                "0 1 1 'Firmware successfully verified'"
#define MessageUpdate_2_1_2                "0 1 2 'Firmware is corrupted'"
#define MessageUpdate_2_1_3                "0 1 3 'Firmware uploaded error'"
#define MessageUpdate_2_1_4                "0 1 4 'Firmware uploaded successfully'"
#define MessageUpdate_2_1_5                "0 1 5 'Flash memory chip ready'"
#define MessageUpdate_2_1_6                "0 1 6 'Flash chip not found'"
#define MessageUpdate_2_1_7                "0 1 7 'Reboot'"
#define MessageUpdate_2_1_8                "0 1 8 'Cancel'"
#define MessageUpdate_2_1_9                "0 1 9 'Update system unlocked'"
#define MessageUpdate_2_1_10               "0 1 10 'Update system blocked'"
#define MessageUpdate_2_1_11               "0 1 11 'No bootloader update required'"
#define MessageUpdate_2_1_12               "0 1 12 'Bootloader updated successfully'"
#define MessageUpdate_2_1_13               "0 1 13 'Bootloader update fail!'"
#define MessageUpdate_2_1_14               "0 1 14 'Not ready for firmware update'"
#define SystemNotifi_SysCommandBlocked     "0 1 15 'System commands are blocked'"
#define MessageUpdate_2_1_16               "0 1 16 'Successful delivery'"
#define Basis_Notification_1_1_22_0        "0 1 17 'Ok cleared spi flash memory'"
#define Basis_Notification_1_1_11_1        "0 1 18 'Error cleared spi flash memory'"
#define MessageUpdate_0_1_19               "0 1 19 'Error while preparing update'"
#define MessageUpdate_0_1_20               "0 1 20 'Error delivery'"
//                                          | | |      |        |       |
//                                          | | |      +--------+-------+-------------- Data
//                                          | | +-------------------------------------- Detailed parameter
//                                          | +---------------------------------------- Parameter (Notification)
//                                          +------------------------------------------ Packed (Basis)
void main_Menu(uint8_t *Received_String);
void SYSTEM_PACKED(uint8_t ControlByte, uint8_t Function, uint8_t *Received_data);
void FIRMWARE_DOWNLOAD(uint8_t *Received_data);
void PREPARING_UPDATE(uint8_t *Received_data);
void UPDATE_SYSTEM_UNLOCK(uint8_t *Received_data);
void CHECK_FIRMWARE();
void COMPLETE_UPDATE();
void UPDATE_CANCEL();

void             Flash_Lock();
void             Flash_Unlock();
void             ResetCheckCRC(void);
void             Write_DeviceInfo();
void             Jump_To_Dixom_Frimware();
void             BOOTLOADER(void);
void             BOOTLOADER_WHILE(void);
void             UpdateRead();
void             FlashStatus();
void             CommandError();

void             UpdateWrite(uint8_t *Recive_USB);
void             Restart();
void             Version();
void             SetRead(uint8_t *Recive_USB);
void             FlashSectorErase(short sector);                                                         //������� ������� �� 4�� � 0 �� 256 �������
void             FlashMemoryBusyStatus();                                                                //���������� ������ � ����� ����������
void             FlashMemoryErase();                                                                     //������� ������ �������
void             FlashMemoryWriteEnable();                                                               //�������� ����������� ������ �� ������
void             FlashMemoryWriteDisable();                                                              //�������� ����������� ������ �� ������
void             FlashMemoryWrite(short page, uint8_t* data, short len);                                 //������ �� ���� ������ �� 256��, �������� � 0 �� 4096
void             FlashMemoryRead(short page, uint8_t* readData, short readLen);                          //������ �� ���� ������ �� 256��, �������� � 0 �� 4096
void             FlashRegistrWrite(uint8_t* data, short len);                                            //������ �������� ���� ������
void             FlashRegistrRead(uint8_t* sendData, short sendLen, uint8_t* readData, short readLen);   //������ �������� ���� ������
void             SPI_CS_FLASH_ON(void);
void             SPI_CS_FLASH_OFF(void);
void             LED_ON(void);
void             LED_OFF(void);
void             Write_UpdateInfo();
void             Launch_Checking_The_DevInfo();
void             Launch_Checking_The_Firmware();
void             SpiFlashErase();
void             Spi_Flash_Read_CheckSumm(uint32_t *read_checkSum, uint32_t page_address, uint32_t* data_address, uint16_t length);
void             Spi_Flash_Write_CheckSumm(uint32_t *flash_checkSum, uint32_t page_address, uint32_t* data_address, uint16_t length);
void             Converter_u32_to_u8 (uint32_t u32, uint8_t* u8);
uint32_t         Converter_8_to_u32(uint8_t* bytes);



#endif
