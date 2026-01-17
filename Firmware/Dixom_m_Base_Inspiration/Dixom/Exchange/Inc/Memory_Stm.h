#ifndef __STM_FLASH_H__
#define __STM_FLASH_H__
#include <Constants.h>
#include <Structures.h>

#define FLASH_ADDR_DEVICE_INFO 0x08008000 // end 0x0800BFFF 16kB // Storage sector for static objects
#define Sector2                0x08008000 // end 0x0800BFFF 16kB // Storage sector for static objects
#define Sector3                0x0800C000 // end 0x0800FFFF 16kB // Sector for copying data if there is not enough RAM.
#define Sector4                0x08010000 // end 0x0801FFFF 64kB // Sector for storing variable settings

#define NumAttempt 3;                     // number of attempts in cycles

WorkRes    Init_StmFlash();
WorkRes    Flash_Unlock();
WorkRes    Flash_Lock();
WorkRes    Flash_Erase(uint32_t sector, uint8_t numLis);
WorkRes    FlashWrite(uint32_t flash_address, uint32_t* structure_address, uint16_t length);
WorkRes    FlashRead(uint32_t flash_address, uint32_t* data_address, uint16_t length);
WorkRes    FlashWriteCRC_Check(uint32_t *flash_checkSum, uint32_t flash_address, uint32_t* data_address, uint16_t length);
void       FlashReadCRC_Check(uint32_t *read_checkSum, uint32_t flash_address, uint32_t* data_address, uint16_t length);
void       Write_DeviceInfo();

/*
 *
Block Name Block base addresses Size
Main memory

Sector 0  0x0800 0000 - 0x0800 3FFF 16  Kilobytes BOOT
Sector 1  0x0800 4000 - 0x0800 7FFF 16  Kilobytes BOOT
Sector 2  0x0800 8000 - 0x0800 BFFF 16  Kilobytes free memory
Sector 3  0x0800 C000 - 0x0800 FFFF 16  Kilobytes free memory
Sector 4  0x0801 0000 - 0x0801 FFFF 64  Kilobytes data structure
Sector 5  0x0802 0000 - 0x0803 FFFF 128 Kilobytes DIXOM Firmware
Sector 6  0x0804 0000 - 0x0805 FFFF 128 Kilobytes DIXOM Firmware
Sector 7  0x0804 0000 - 0x0805 FFFF 128 Kilobytes DIXOM Firmware
Sector 8  0x0804 0000 - 0x0805 FFFF 128 Kilobytes DIXOM Firmware
Sector 9  0x0804 0000 - 0x0805 FFFF 128 Kilobytes DIXOM Firmware
Sector 10 0x0804 0000 - 0x0805 FFFF 128 Kilobytes DIXOM Firmware
Sector 11 0x080E 0000 - 0x080F FFFF 128 Kilobytes DIXOM Firmware

System memory 0x1FFF 0000 - 0x1FFF 77FF 30  Kilobytes
OTP area      0x1FFF 7800 - 0x1FFF 7A0F 528 bytes
Option bytes  0x1FFF C000 - 0x1FFF C00F 16  bytes

*/































#endif
