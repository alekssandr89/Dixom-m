/*
 * BootloaderUpdate.c
 *
 *  Created on: 12 ���. 2022 �.
 *      Author: 79825
 */
#include <Description.h>
#include <Memory_Spi.h>
#include "stdint.h"
#include "Constants.h"
#include "Structures.h"
#include "DIX_usb.h"
#include "stm32f4xx_hal_flash.h"
#include <UpdateBootloader.h>
#include <UpdateFirmware.h>

extern sDixom Dixom;

int16_t getCurrentBootVersion(){
	return (int16_t*)VER_BOOTLOADER;
}

void BootUpdate(void){

	uint8_t UpdateState = HAL_ERROR;
	uint8_t attempts = 10;
	uint32_t error = 0, FlashAddrBootloader = 0x08000000;
	HAL_StatusTypeDef flash_ok = HAL_ERROR;
	FLASH_EraseInitTypeDef EraseInitStruct;
	Dixom.DevInfo.InMode = FirmwareUpdate;

	if (Dixom.DevInfo.Firmware_UpdateInfo.Boot_VerFrimware != VER_BOOTLOADER) {
		while (attempts != 0 && UpdateState == HAL_ERROR) {
			EraseInitStruct.TypeErase = FLASH_TYPEERASE_SECTORS;
			EraseInitStruct.VoltageRange = FLASH_VOLTAGE_RANGE_3;
			EraseInitStruct.Sector = 0;
			EraseInitStruct.NbSectors = 2;
			attempts--;
			while (flash_ok != HAL_OK) {
				flash_ok = HAL_FLASH_Unlock();
			}

			Delay(200, FreeRTOS);
			HAL_FLASHEx_Erase(&EraseInitStruct, &error);
			// erase the bootloader memory area
			Delay(700, FreeRTOS);

			for (short byte = 0; byte < BootLoaderFrimwareSize; byte++) {
				//writing bootloader firmware to stm32 flash memory
				HAL_FLASH_Program(FLASH_TYPEPROGRAM_BYTE, FlashAddrBootloader + byte,
				BootLoaderFrimware[byte]);
			}
			Delay(200, FreeRTOS);
			for (short byte = 0; byte < BootLoaderFrimwareSize; byte++) {
				//checking the downloaded firmware
				if((*(__IO uint8_t*)(FlashAddrBootloader+byte)) != BootLoaderFrimware[byte]){
					UpdateState = HAL_ERROR;
				}else{
					UpdateState = HAL_OK;
				}
			}
			Delay(200, FreeRTOS);
			if(UpdateState == HAL_OK && flash_ok == HAL_OK){
				//successful download update
				FlashMemoryWriteEnable();
				FlashSectorErase(255);
				FlashMemoryWriteEnable();
				Dixom.DevInfo.Firmware_UpdateInfo.Boot_VerFrimware = VER_BOOTLOADER;
				Spi_Flash_Write_CheckSumm(&Dixom.DevInfo.Firmware_UpdateInfo.CheckSum, SpiFlashAddrUpdateInfo, (uint32_t*)&Dixom.DevInfo.Firmware_UpdateInfo, sizeof(Dixom.DevInfo.Firmware_UpdateInfo)/4);
				FlashMemoryWriteDisable();
				DIXOM_VERSION();
 		  		TransmitDataOutputs((uint8_t *)MessageUpdate_2_1_12, AutoLenght,  false,  true, allAvailable);

			}else{
				//error while updating the download
				DIXOM_VERSION();
 		  		TransmitDataOutputs((uint8_t *)MessageUpdate_2_1_11, AutoLenght,  false,  true, allAvailable);
 				Delay(1000, FreeRTOS);
			}
		}
		flash_ok = HAL_ERROR;
		while (flash_ok != HAL_OK) {
			flash_ok = HAL_FLASH_Lock();
		}
	}else{
		DIXOM_VERSION();
		TransmitDataOutputs((uint8_t *)MessageUpdate_2_1_11, AutoLenght,  false,  true, allAvailable);

	}
	Dixom.DevInfo.InMode = ReadyForWork;
}


void Only_BootUpdate(void){

	uint8_t UpdateState = HAL_ERROR;
	uint8_t attempts = 10;
	uint32_t error = 0, FlashAddrBootloader = 0x08000000;
	HAL_StatusTypeDef flash_ok = HAL_ERROR;
	FLASH_EraseInitTypeDef EraseInitStruct;

	if (Dixom.DevInfo.Firmware_UpdateInfo.Boot_VerFrimware != VER_BOOTLOADER) {
		while (attempts != 0 && UpdateState == HAL_ERROR) {
			EraseInitStruct.TypeErase = FLASH_TYPEERASE_SECTORS;
			EraseInitStruct.VoltageRange = FLASH_VOLTAGE_RANGE_3;
			EraseInitStruct.Sector = 0;
			EraseInitStruct.NbSectors = 2;
			attempts--;
			while (flash_ok != HAL_OK) {
				flash_ok = HAL_FLASH_Unlock();
			}
			Delay(200, FreeRTOS);
			HAL_FLASHEx_Erase(&EraseInitStruct, &error);
		    // erase the bootloader memory area
			Delay(700, FreeRTOS);

			for (short byte = 0; byte < BootLoaderFrimwareSize; byte++) {
				//writing bootloader firmware to stm32 flash memory
				HAL_FLASH_Program(FLASH_TYPEPROGRAM_BYTE, FlashAddrBootloader + byte,
				BootLoaderFrimware[byte]);
			}
			Delay(200, FreeRTOS);
			for (short byte = 0; byte < BootLoaderFrimwareSize; byte++) {
					//checking the downloaded firmware
				if((*(__IO uint8_t*)(FlashAddrBootloader+byte)) != BootLoaderFrimware[byte]){
					UpdateState = HAL_ERROR;
				}else{
					UpdateState = HAL_OK;
				}
			}
			Delay(200, FreeRTOS);
			if(UpdateState == HAL_OK && flash_ok == HAL_OK){
				//successful download update
				FlashMemoryWriteEnable();
				FlashSectorErase(255);
				FlashMemoryWriteEnable();
				Dixom.DevInfo.Firmware_UpdateInfo.Boot_VerFrimware = VER_BOOTLOADER;
				Dixom.DevInfo.Firmware_UpdateInfo.State = Bad_Frimware;
				Dixom.DevInfo.Firmware_UpdateInfo.StateIWDG = 1;
				Spi_Flash_Write_CheckSumm(&Dixom.DevInfo.Firmware_UpdateInfo.CheckSum, SpiFlashAddrUpdateInfo, (uint32_t*)&Dixom.DevInfo.Firmware_UpdateInfo, sizeof(Dixom.DevInfo.Firmware_UpdateInfo)/4);
				FlashMemoryWriteDisable();

				Delay(1000, FreeRTOS);
				NVIC_SystemReset();

			}else{
				//error while updating the download
				Delay(1000, FreeRTOS);
			}
		}
		flash_ok = HAL_ERROR;

		while (flash_ok != HAL_OK) {
			flash_ok = HAL_FLASH_Lock();
		}
	}else{
		FlashMemoryWriteEnable();
		FlashSectorErase(255);
		FlashMemoryWriteEnable();
		Dixom.DevInfo.Firmware_UpdateInfo.Boot_VerFrimware = VER_BOOTLOADER;
		Dixom.DevInfo.Firmware_UpdateInfo.State = Bad_Frimware;
		Spi_Flash_Write_CheckSumm(&Dixom.DevInfo.Firmware_UpdateInfo.CheckSum, SpiFlashAddrUpdateInfo, (uint32_t*)&Dixom.DevInfo.Firmware_UpdateInfo, sizeof(Dixom.DevInfo.Firmware_UpdateInfo)/4);
		FlashMemoryWriteDisable();

		Delay(1000, FreeRTOS);
		NVIC_SystemReset();
	}
}



