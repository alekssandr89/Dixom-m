/*
 * Spi_flash.h
 *
 *  Created on: 24 нояб. 2021 г.
 *      Author: 79825
 */

#ifndef EXCHANGE_INC_MEMORY_SPI_H_
#define EXCHANGE_INC_MEMORY_SPI_H_

#include "stdint.h"

typedef struct {
    uint8_t busy : 1;
    uint8_t write_enable : 1;
    uint8_t block_protect : 3;
    uint8_t top_bot_ptotect : 1;
    uint8_t sector_protect : 1;
    uint8_t status_reg_protect0 : 1;
} STATUS_REG1_STRUCT_t;


void       FlashMemoryBusyStatus(void);                                                            //готовность памяти к приёму инструкций
void       FlashMemoryWriteEnable(void);                                                           //Включить возможность записи на флешку
void       FlashSectorErase(short sector);                                                         //Стирает секторы по 4кБ с 0 по 256 секторы
void       SpiFlashErase(void);
void       FlashMemoryErase(void);                                                                 //Стирает память целиком
void       FlashMemoryWriteDisable(void);                                                          //Оключить возможность записи на флешку
void       FlashMemoryWrite(short page, uint8_t* data, short len);                                 //Запись во флеш память по 256кБ, страницы с 0 по 4096
void       FlashMemoryRead(short page, uint8_t* readData, short readLen);                          //Чапись из флеш памяти по 256кБ, страницы с 0 по 4096
void       FlashRegistrWrite(uint8_t* data, short len);                                            //Чтение регистра флеш памяти
void       FlashRegistrRead(uint8_t* sendData, short sendLen, uint8_t* readData, short readLen);   //Запись регистра флеш памяти
void       Spi_Flash_Write_CheckSumm(uint32_t *flash_checkSum, uint32_t page_address, uint32_t* data_address, uint16_t length);
void       Spi_Flash_Read_CheckSumm(uint32_t *read_checkSum, uint32_t page_address, uint32_t* data_address, uint16_t length);

#endif /* EXCHANGE_INC_MEMORY_SPI_H_ */
