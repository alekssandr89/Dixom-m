#include <Math.h>
#include "Bootloader.h"
#include "main.h"
#include "aes.h"

extern I2C_HandleTypeDef hi2c1;
//IWDG_HandleTypeDef hiwdg;

HAL_StatusTypeDef	flash_ok = HAL_ERROR;              //↓ Состояние FLASH STM 32
FLASH_EraseInitTypeDef EraseInitStruct;                //↓ Структура с параметрами для стрирания FLSH STM32

typedef  void (*pFunction)(void);                      //↓ Объявляем пользовательский тип
pFunction Jump_To_Application;                         //↓ И создаём переменную этого типа

const short DataLen = 42;                              //↓ Длина одного пакета передачи и приёма по I2C

#define   APPLICATION_ADDRESS      0x08004000          //↓ Адрес начала основной программы

#define TypeWrite                  48                  //↓         48     0    I2C КОМАНДА Запись данных в контроллер
#define TypeRead                   49                  //↓         49     1    I2C КОМАНДА Чтение данных из контроллера
                                                       //        BYTE  ASCII                 Описание                          Пример ввода команды
#define FrimwareUpdateSetup        48                  //↓ Запись  48     0    Предварительная настройка перед обновлением         SOUND WRITE 0
#define FrimwareUpdateWrite        49                  //↓ Запись  49     1    Загрузка обновления в микроконтроллер               SOUND WRITE 1(данные с прошивкой)
#define FrimwareUpdateEnd          50                  //↓ Запись  50     2    Окончательная проверка и завершение обновления      SOUND WRITE 2
#define FrimwareUpdateAppStart     51                  //↓ Запись  51     3    Запуск обновлённой программы                        SOUND WRITE 3
#define FrimwareUpdateBlock        52                  //↓ Запись  52     4    Блокировка системы обновления                       SOUND WRITE 4
#define FrimwareUpdateUnlock       53                  //↓ Запись  53     5    Разблокировка системы обновления (ключ 10286)       SOUND WRITE 510276
#define FrimwareVersion            48                  //↓ Чтение  48     0    Отобразит версию программного обеспечения           SOUND READ 0
#define FrimwareUpdateStatus       49                  //↓ Чтение  48     1    Отобразит статус системы обновления                 SOUND READ 1

uint32_t  RealFrimwareSize,                            //↓ Реальный размер прошивки
          CriptFrimwareSize,                           //↓ Размер прошивки в зашифрованной форме
          FirmwareSizeCounter,                         //↓ Счётчик размера прошивки для обновления
          CheckCRC,                                    //↓ CRC принятая по USB шифрованной прошивки
          CalculateCRC,                                //↓ Контрольная сумма расчитанная бутлоадером
		  counter,                                     //↓ Указатель буфера обновления
          FrimwareCRC,                                 //↓ Настоящая контрольная сумма прошивки
		  jumpAddress,                                 //↓ Пользовательский тип
		  error,                                       //↓ Статусы ошибок стирания FLASH STM32
		  FLASHAddrStm32 = APPLICATION_ADDRESS,        //↓ Адреса памяти для записи новой прошивки
		  FLASHSectorData  [64],                       //↓ Буфер для записи данных в FLASH STM32
          crc_table[256];                              //↓ Буфер подсчёта CRC32

uint8_t   UpdateStatus = 0,                            //↓ Флаг статуса сисетмы обновления
		  FrimwareUpdateLock = 0,                      //↓ Ключ раблокировки 10286
		  AppStartReady = 0,                           //↓ Флаг запуска основной программы
		  I2CReadData[42],                             //↓ Буфер для приёма и передачи данных по I2C
		  FrimwareDataRead [256],                      //↓ Буфер для не расшифрованных данных
		  FrimwareData     [256];                      //↓ Буфер для расшифрованных данных

//=====================================================Ключ расшифровки прошивки============================================================
const uint8_t  Key[32] = {0xB4, 0x35, 0x05, 0xC3, 0x6A, 0x14, 0x11, 0x43, 0xF7, 0xCA, 0x37, 0x78, 0xE1, 0xF0, 0x56, 0xB1,                //↓
                          0xE6, 0xD7, 0x1D, 0xFF, 0x4E, 0x5D, 0xF8, 0x17, 0xCF, 0xE0, 0xD7, 0x26, 0x2C, 0x9A, 0xAA, 0x8B };              //↓
const uint8_t  Iv[16] = { 0xB8, 0xF2, 0x15, 0xF5, 0x86, 0x29, 0x31, 0x11, 0x75, 0xEB, 0x5E, 0xB4, 0x7F, 0x04, 0x47, 0xEB  };             //↓
//==========================================================================================================================================

void UpdateMenu(){

	//=============================================Меню записи в контроллер из I2C==========================================================
	if(I2CReadData[0]==TypeWrite){                                                                                                        //↓ Команда на запись
		                                                                                                                                  //
		if(I2CReadData[1]==FrimwareUpdateSetup){                                                                                          //↓ Предварительная настройка перед обновление
			//==================================Предварительная настройка перед обновление==================================================
		                                                                                                                               	  //
			if (FrimwareUpdateLock == 1){                                                                                                 //↓ Если система обновления разблокированна продолжаем
                                                                                                                                          //
				UpdateStatus = 0;                                                                                                         //↓ Выставляем статус готовности к обновлению НЕ ГОТОВ
				RealFrimwareSize  =  I2CReadData[2]  |(I2CReadData[3] << 8) |(I2CReadData[4] << 16) |(I2CReadData[5] << 24);              //↓ Получаем реальный размер прошивки
				CriptFrimwareSize =  I2CReadData[6]  |(I2CReadData[7] << 8) |(I2CReadData[8] << 16) |(I2CReadData[9] << 24);              //↓ Получаемразмер прошивки в зашифрованной форме
				FrimwareCRC       =  I2CReadData[10] |(I2CReadData[11] << 8)|(I2CReadData[12] << 16)|(I2CReadData[13] << 24);             //↓ Получаем контрольную сумму расшифрованной прошивки для бутлоадера//Получаем контрольную сумму расшифрованной прошивки для бутлоадера
				                                                                                                                          //
				while(flash_ok != HAL_OK){                                                                                                //↓
					flash_ok = HAL_FLASH_Unlock();                                                                                        //↓ Разблокируем FLASH STM32
				}                                                                                                 			              //↓
		                                                                                                                                  //
				EraseInitStruct.TypeErase    = FLASH_TYPEERASE_SECTORS;                                                                   //↓ Указываем что стираем секторами
				EraseInitStruct.VoltageRange = FLASH_VOLTAGE_RANGE_3;                                                                     //↓ Указываем напряжение паралельного стирания
				EraseInitStruct.Sector       = 1;                                                                                         //↓ Указываем адресс начального сектора для стирания
				EraseInitStruct.NbSectors    = 7;                                                                                         //↓ Указываем сколько секторов нужно стереть относительно начального сектора
				HAL_FLASHEx_Erase(&EraseInitStruct, &error);                                                                              //↓ Стираем FLASH STM32 с параметрами EraseInitStruct
		                                                                                                                                  //
				FirmwareSizeCounter=0;                                                                                                    //↓ Очищаем счётчик прошивки
				counter=0;                                                                                                                //↓ Очищаем счётчик пачки прошивки
                                                                                                                                          //
			    for (uint16_t i = 0; i < 256; i++){                                                                                       //↓
			    	CalculateCRC = i;                                                                                                     //↓
				    for (uint8_t j = 0; j < 8; j++){                                                                                      //↓
				    	CalculateCRC = (CalculateCRC & 1) != 0  ? (CalculateCRC >> 1) ^ 0xEDB88320 : CalculateCRC >> 1;                   //↓ Настройка расчёта CRC контрольной суммы
				    }                                                                                                                     //↓
				    crc_table[i] = CalculateCRC;                                                                                          //↓
				 };                                                                                                                       //↓
			    CalculateCRC = 0xFFFFFFFF;                                                                                                //↓
                                                                                                                                          //
			    UpdateStatus = 1;                                                                                                         //↓ Выставляем статус готовности к обновлению ГОТОВ К ОБНОВЛЕНИЮ
			}                                                                                                                             //
		}else if(I2CReadData[1]==FrimwareUpdateWrite){                                                                                    //↓ Загрузка обновления в микроконтроллер
			//===================================Загрузка обновления в микроконтроллер======================================================
			if (FrimwareUpdateLock == 1){                                                                                                 //↓ Если система обновления разблокированна продолжаем
                                                                                                                                          //
				for (short i = 0; i<32;i++){                                                                                              //↓ Цикл сборки прошивки
					FrimwareDataRead[counter]=I2CReadData[i+2];                                                                           //↓ Заполнения будфера обновления					                                                                                          //↓
					counter++;                                                                                                            //↓ Инкрементация указателя буфера обновления
					if (counter==256){                                                                                                    //↓ Если указателя буфера обновления достиг 256
						                                                                                                                  //
				       AES_CBC_decrypt_buffer(FrimwareData, FrimwareDataRead, 256, Key, Iv);                                              //↓ Расшифровка прошивки пачками по 256 байт                                                                                               //
                                                                                                                                          //
				       for (uint16_t y= 0; y<256;y++){                                                                                    //↓
				    	   CalculateCRC = crc_table[(CalculateCRC ^ FrimwareData[y]) & 0xFF] ^ (CalculateCRC >> 8);                       //↓ Расчёт CRC контрольной суммы
				       }                                                                                                                  //↓
                                                                                                                                          //
				       for(short byte=0; byte<64; byte++){                                                                                //↓
				    	   FLASHSectorData[byte] = FrimwareData[0+(byte*4)]                                                               //↓
											    | (FrimwareData[1+(byte*4)] << 8)                                                         //↓ Цикл сборки пачки прошивки для FLASH STM по 32 байта
											    | (FrimwareData[2+(byte*4)] << 16)                                                        //↓
												| (FrimwareData[3+(byte*4)] << 24);                                                       //↓
				       }                                                                                                                  //↓
				                                                                                                                          //
				       for(short byte=0; byte<64; byte++){                                                                                //↓
				    	   HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD,FLASHAddrStm32+FirmwareSizeCounter*4,FLASHSectorData[byte]);          //↓ Цикл закрузки пачки прошивки во FLASH STM32                                                                    //↓
				    	   FirmwareSizeCounter++;                                                                                         //↓
				       }                                                                                                                  //↓
				                                                                                                                          //
				       counter=0;                                                                                                         //↓ Сброс указателя буфера обновления
					}                                                                                                                     //
				}                                                                                                                         //
				UpdateStatus = 0;                                                                                                         //↓ Выставляем статус готовности к обновлению НЕ ГОТОВ
			}                                                                                                                             //                                                                                               //
		}else if(I2CReadData[1]==FrimwareUpdateEnd){                                                                                      //↓ Окончательная проверка и завершение обновления
			//==================================Окончательная проверка и завершение обновления==============================================
			UpdateStatus = 0;                                                                                                             //↓ Выставляем статус готовности к обновлению НЕ ГОТОВ
			CalculateCRC ^= 0xFFFFFFFF;                                                                                                   //↓ Преобразование CRC
			                                                                                                                              //
        	if( FrimwareCRC == CalculateCRC){                                                                                             //↓ Сравнение CRC расчитанной в Dixom Update Maker и бутлоадеров здес
    	    	UpdateStatus = 1;                                                                                                         //↓ Выставляем статус готовности к обновлению ГОТОВ К ОБНОВЛЕНИЮ
        	}                                                                                                                             //
		}else if(I2CReadData[1]==FrimwareUpdateAppStart){                                                                                 //↓ Запуск обновлённой программы
			//=============================================Запуск обновлённой программы=====================================================
			AppStartReady = 1;                                                                                                            //↓ Запускаем основную программу звуковой карты
			UpdateStatus = 0;                                                                                                             //↓ Выставляем статус готовности к обновлению НЕ ГОТОВ
                                                                                                                                          //
		}else if(I2CReadData[1]==FrimwareUpdateBlock){                                                                                    //↓ Блокировка системы обновления
			//=============================================Блокировка системы обновления====================================================
			FrimwareUpdateLock = 0;                                                                                                       //↓ Блокируем систему обновления
	    	while(flash_ok != HAL_OK){                                                                                                    //↓
	    		flash_ok =  HAL_FLASH_Lock();                                                                                             //↓ Блокируем запись во FLASH STM32
	    	}                                                                                                                             //↓
		}else if(I2CReadData[1]==FrimwareUpdateUnlock){                                                                                   //↓ Разблокировка системы обновления
			//===========================================Разблокировка системы обновления===================================================
			if(I2CReadData[2] ==  '1' && I2CReadData[3] =='0' && I2CReadData[4] =='2' && I2CReadData[5] =='8' && I2CReadData[6] =='6'){   //↓ Если ключ разблокировки верен (10286)
				                                                                                                                          //
				FrimwareUpdateLock = 1;                                                                                                   //↓ То разблокируем систему обновления
				                                                                                                                          //
			}	                                                                                                                          //
		}	                                                                                                                              //
	//=============================================Меню чтения из контроллер в I2C==========================================================
	}else if(I2CReadData[0]==TypeRead){                                                                                                   //↓ Команда на чтение
                                                                                                                                          //
		if(I2CReadData[1]==FrimwareVersion){                                                                                              //↓ Отобразит версию программного обеспечения
			//==================================== Отобразит версию программного обеспечения================================================
			GetFrimwareVersion();                                                                                                         //↓ Заполнение буфера версией ПО
			HAL_I2C_Slave_Transmit(&hi2c1, &I2CReadData[0],DataLen, 200);                                                                 //↓ Отправка данных на шину I2C                                                                                         //↓
                                                                                                                                          //
	    }else if(I2CReadData[1] == FrimwareUpdateStatus){                                                                                 //↓ Отобразит статус системы обновления
	    	//========================================Отобразит статус системы обновления===================================================
	    	GetUpdateReadyStatus();                                                                                                       //↓ Заполнение буфера статусом системы обновления
	    	HAL_I2C_Slave_Transmit(&hi2c1, &I2CReadData[0],DataLen, 200);                                                                 //↓ Отправка данных на шину I2C
                                                                                                                                          //
	    }else{                                                                                                                            //
	    	CommandReadError();                                                                                                           //↓ Не верная команда чтения
	    }                                                                                                                                 //
	}
}

void Jump_To_Dixom_Frimware(){                                                                                                            //↓ Прыжок в основную программу
	//==============================================Прыжок в основную программу=============================================================
	jumpAddress = *(__IO uint32_t*) (APPLICATION_ADDRESS + 4);                                                                            //↓ Извлекаем адрес перехода из вектора Reset
	HAL_RCC_DeInit();                                                                                                                     //↓ Убираем инициализацию RCC
	HAL_DeInit();                                                                                                                         //↓ Убираем инициализацию HAL
	Jump_To_Application = (pFunction) jumpAddress;                                                                                        //↓ Приводим его к пользовательскому типу
	__disable_irq();                                                                                                                      //↓ Запрещаем прерывания
	__set_MSP(*(__IO uint32_t*) APPLICATION_ADDRESS);                                                                                     //↓ Устанавливаем SP приложения (вершину стека прошивки)
	Jump_To_Application();                                                                                                                //↓ Прыжок в основную прошивку Dxiom
}

void BOOTLOADER(void){                                                                                                                    //↓ Настройка работы Bootloader
	//==============================================Настройка работы Bootloader=============================================================
	HAL_I2C_Slave_Receive_IT(&hi2c1, &I2CReadData[0], DataLen);                                                                           //↓ Выставляем прерывание для приёма данных по I2C1
	for(uint8_t i = 0; i<42; i++){                                                                                                        //↓
		I2CReadData[i] = 0;                                                                                                               //↓ Инициализируем буфер приёма данных по I2C
	}
}

void BOOTLOADER_WHILE(void){                                                                                                              //↓ Основной цикл Bootloader
	//===============================================Основной цикл Bootloader===============================================================
	//HAL_IWDG_Refresh(&hiwdg);                                                                                                           //↓ Сбрасываем флаг сторожевого таймера
	if(AppStartReady==1){                                                                                                                 //↓ Если  AppStartReady = 1 прыгаем в основную программу
		Jump_To_Dixom_Frimware();                                                                                                         //↓ Прыгаем в основную программу
	}
}

void BREAKFLAG(){
	HAL_I2C_Slave_Receive_IT(&hi2c1, &I2CReadData[0], DataLen);                                                                           //↓ Инициализация прерывания приёма данных по I2C
}

void HAL_I2C_SlaveRxCpltCallback(I2C_HandleTypeDef *hi2c){                                                                                //↓ Приём данных по шине I2C
	//===============================================Приём данных по шине I2C===============================================================
	if(hi2c->Instance ==hi2c1.Instance){                                                                                                  //↓ Если шина I2C №1 то продолжаем работу
		UpdateMenu();                                                                                                                     //↓ Отправляем данные в основное меню
		HAL_I2C_Slave_Receive_IT(&hi2c1, &I2CReadData[0], DataLen);                                                                       //↓ Приём данных по шине I2C
	}
}


void GetFrimwareVersion(){

//	I2CReadData[3] =  {'<','<'	    };
	//<VERSION SOUND:N/D >\r\n
	I2CReadData[0] =  '<';                                                                                //
	I2CReadData[1] =  'V';                                                                                //
	I2CReadData[2] =  'E';                                                                               //
	I2CReadData[3] =  'R';                                                                               //
	I2CReadData[4] =  'S';                                                                               //
	I2CReadData[5] =  'I';                                                                               //Собираем строку для отправки
	I2CReadData[6] =  'O';                                                                               //
	I2CReadData[7] =  'N';                                                                               //
	I2CReadData[8] =  '_';                                                                               //
	I2CReadData[9] =  'S';                                                                               //
	I2CReadData[10] = 'O';                                                                               //
	I2CReadData[11] = 'U';                                                                               //Собираем строку для отправки
	I2CReadData[12] = 'N';                                                                               //
	I2CReadData[13] = 'D';                                                                               //
	I2CReadData[14] = ' ';                                                                               //
	I2CReadData[15] = 'B';                                                                               //
	I2CReadData[16] = 'O';                                                                               //
	I2CReadData[17] = 'O';                                                                               //
	I2CReadData[18] = 'T';                                                                               //
	I2CReadData[19] = 'L';                                                                               //
	I2CReadData[20] = 'O';                                                                               //Собираем строку для отправки
	I2CReadData[21] = 'A';                                                                               //
	I2CReadData[22] = 'D';                                                                               //
	I2CReadData[23] = 'E';                                                                               //
	I2CReadData[24] = 'R';                                                                               //
	I2CReadData[25] = '_';                                                                               //
	I2CReadData[26] = 'V';                                                                               //
	I2CReadData[27] = '1';                                                                               //
	I2CReadData[28] = '.';                                                                               //
	I2CReadData[29] = '0';                                                                               //
	I2CReadData[30] = '0';                                                                               //
	I2CReadData[31] = ' ';                                                                               //
	I2CReadData[32] = '>';                                                                               //
	I2CReadData[33] = '\r';                                                                              //
	I2CReadData[34] = '\n';                                                                              //
}

void GetUpdateReadyStatus(){
	//<READY_TO_GET_FRIMWARE"
	if(UpdateStatus == 1){
		I2CReadData[0] =  '<';                                                                               //
		I2CReadData[1] =  'F';                                                                               //
		I2CReadData[2] =  'R';                                                                               //
		I2CReadData[3] =  'I';                                                                               //
		I2CReadData[4] =  'M';                                                                               //
		I2CReadData[5] =  'W';                                                                               //
		I2CReadData[6] =  'A';                                                                               //
		I2CReadData[7] =  'R';                                                                               //
		I2CReadData[8] =  'E';                                                                               //
		I2CReadData[9] =  '_';                                                                               //
		I2CReadData[10] = 'U';                                                                               //
		I2CReadData[11] = 'P';                                                                               //
		I2CReadData[12] = 'D';                                                                               //
		I2CReadData[13] = 'A';                                                                               //
		I2CReadData[14] = 'T';                                                                               //
		I2CReadData[15] = 'E';                                                                               //
		I2CReadData[16] = '_';                                                                               //
		I2CReadData[17] = 'R';                                                                               //
		I2CReadData[18] = 'E';                                                                               //
		I2CReadData[19] = 'A';                                                                               //
		I2CReadData[20] = 'D';                                                                               //
		I2CReadData[21] = 'Y';                                                                               //
		I2CReadData[22] = ' ';                                                                               //
		I2CReadData[23] = '>';                                                                               //
		I2CReadData[24] = '\r';                                                                              //
		I2CReadData[25] = '\n';                                                                              //
	}else{
		I2CReadData[0] =  '<';                                                                               //
		I2CReadData[1] =  'F';                                                                               //
		I2CReadData[2] =  'R';                                                                               //
		I2CReadData[3] =  'I';                                                                               //
		I2CReadData[4] =  'M';                                                                               //
		I2CReadData[5] =  'W';                                                                               //
		I2CReadData[6] =  'A';                                                                               //
		I2CReadData[7] =  'R';                                                                               //
		I2CReadData[8] =  'E';                                                                               //
		I2CReadData[9] =  '_';                                                                               //
		I2CReadData[10] = 'U';                                                                               //
		I2CReadData[11] = 'P';                                                                               //
		I2CReadData[12] = 'D';                                                                               //
		I2CReadData[13] = 'A';                                                                               //
		I2CReadData[14] = 'T';                                                                               //
		I2CReadData[15] = 'E';                                                                               //
		I2CReadData[16] = '_';                                                                               //
		I2CReadData[17] = 'N';                                                                               //
		I2CReadData[18] = 'O';                                                                               //
		I2CReadData[19] = 'T';                                                                               //
		I2CReadData[20] = '_';                                                                               //
		I2CReadData[21] = 'R';                                                                               //
		I2CReadData[22] = 'E';                                                                               //
		I2CReadData[23] = 'A';                                                                               //
		I2CReadData[24] = 'D';                                                                               //
		I2CReadData[25] = 'Y';                                                                               //
		I2CReadData[26] = ' ';                                                                               //
		I2CReadData[27] = '>';                                                                               //
		I2CReadData[28] = '\r';                                                                              //
		I2CReadData[29] = '\n';                                                                              //
	}
}








void CommandReadError(){

	I2CReadData[0] =  '<';                                                                                //
	I2CReadData[1] =  'S';                                                                                //
	I2CReadData[2] =  'O';                                                                                //
	I2CReadData[3] =  'U';                                                                                //
	I2CReadData[4] =  'N';                                                                                //
	I2CReadData[5] =  'D';                                                                                //
	I2CReadData[6] =  '_';                                                                                //
	I2CReadData[7] =  'C';                                                                                //
	I2CReadData[8] =  'A';                                                                                //
	I2CReadData[9] =  'R';                                                                                //
	I2CReadData[10] = 'D';                                                                                //
	I2CReadData[11] = '_';                                                                                //Собираем строку для отправки
	I2CReadData[12] = 'R';                                                                                //
	I2CReadData[13] = 'E';                                                                                //
	I2CReadData[14] = 'A';                                                                                //
	I2CReadData[15] = 'D';                                                                                //
	I2CReadData[16] = '_';                                                                                //Собираем строку для отправки
	I2CReadData[17] = 'C';                                                                                //
	I2CReadData[18] = 'O';                                                                                //
	I2CReadData[19] = 'M';                                                                                //
	I2CReadData[20] = 'M';                                                                                //
	I2CReadData[21] = 'A';                                                                                //
	I2CReadData[22] = 'N';                                                                                //Собираем строку для отправки
	I2CReadData[23] = 'D';                                                                                //
	I2CReadData[24] = '_';                                                                                //
	I2CReadData[25] = 'E';                                                                                //
	I2CReadData[26] = 'R';                                                                                //
	I2CReadData[27] = 'R';                                                                                //
	I2CReadData[28] = 'O';                                                                                //
	I2CReadData[29] = 'R';                                                                                //
	I2CReadData[30] = ' ';                                                                               //
	I2CReadData[31] = '>';                                                                               //
	I2CReadData[32] = '\r';                                                                               //
	I2CReadData[33] = '\n';                                                                               //
	HAL_I2C_Slave_Transmit(&hi2c1, &I2CReadData[0],DataLen, 200);                                            //Отправляем строку
}


























