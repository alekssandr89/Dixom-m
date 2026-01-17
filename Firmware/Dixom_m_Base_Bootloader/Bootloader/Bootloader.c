#include <Math.h>
#include "aes.h"
#include "usb_device.h"
#include "Bootloader.h"
#include "stdint.h"
#include "usbd_customhid.h"

//1024*1024 = 1044480
//4081*256  = 1044736
//4075*256  = 1043200

#define FlashLenBlock              4096                        //↓ SPI FLASH Размер одной страницы в Байтах
#define FlashLenList               256                         //↓ SPI FLASH Размер одного блока в странице 256 Байт
#define FlashLenSector             16                          //↓ SPI FLASH Количество блоков в одной странице 16 блоков по 256 Байт
#define FlashLenBlockCount         256                         //↓ SPI FLASH количество страниц в микросхеме 256 страниц по 4096 байт

#define DEVICE_INFO_ADDRESS        0x08008000                   // end 0x0800BFFF 16kB // Storage sector for static objects

#define SpiFlashAddrUpdateInfo     4080                        //↓ Адрес во флешь для получении информации об обновлении

#define FrimwareUpdateSetup        48                          //↓ Запись  48     0    Предварительная настройка перед обновлением         SOUND WRITE 0
#define FrimwareUpdateWrite        49                          //↓ Запись  49     1    Загрузка обновления в микроконтроллер               SOUND WRITE 1(данные с прошивкой)
#define FrimwareUpdateEnd          50                          //↓ Запись  50     2    Окончательная проверка и завершение обновления      SOUND WRITE 2
#define FrimwareUpdateAppStart     51                          //↓ Запись  51     3    Запуск обновлённой программы                        SOUND WRITE 3
#define FrimwareUpdateBlock        52                          //↓ Запись  52     4    Блокировка системы обновления                       SOUND WRITE 4
#define FrimwareUpdateUnlock       53                          //↓ Запись  53     5    Разблокировка системы обновления (ключ 10286)       SOUND WRITE 510276
#define FrimwareVersion            48                          //↓ Чтение  48     0    Отобразит версию программного обеспечения           SOUND READ 0
#define FrimwareUpdateStatus       49                          //↓ Чтение  48     1    Отобразит статус системы обновления                 SOUND READ 1
                                                               //         BYTE  ASCII
sDevInfo DevInfo;
typedef  void (*pFunction)(void);                              //↓ Объявляем пользовательский тип
pFunction Jump_To_Application;                                 //↓ И создаём переменную этого типа
FLASH_EraseInitTypeDef EraseInitStruct;                        //↓ Структура с параметрами для стрирания FLSH STM32
#define UpdateInfo DevInfo.Firmware_UpdateInfo

typedef struct {                                               //↓
    uint8_t busy : 1;                                          //↓
    uint8_t write_enable : 1;                                  //↓
    uint8_t block_protect : 3;                                 //↓ Структура для тоступа к праметрам FLASH микросхемы
    uint8_t top_bot_ptotect : 1;                               //↓
    uint8_t sector_protect : 1;                                //↓
    uint8_t status_reg_protect0 : 1;                           //↓
} STATUS_REG1_STRUCT_t;                                        //↓

union {                                                        //↓
	uint8_t all;                                               //↓ Конвертация
    STATUS_REG1_STRUCT_t bit;                                  //↓
} FlashMemoryRegStatus;                                        //↓

extern SPI_HandleTypeDef hspi2;                                //↓
extern USBD_HandleTypeDef hUsbDeviceFS;                        //↓
extern IWDG_HandleTypeDef hiwdg;                               //↓

uint32_t  CriptFrimwareSize,                                   //↓ Размер прошивки в зашифрованной форме
          FirmwareSizeCounter,                                 //↓ Счётчик размера прошивки для обновления
          crc_table[256],                                      //↓ Буфер подсчёта CRC32
          CheckCRC,                                            //↓ CRC принятая по USB шифрованной прошивки
          CalculateCRC,                                        //↓ Контрольная сумма расчитанная бутлоадером

          jumpAddress,                                         //↓ пользовательский тип
          error,                                               //↓ Статусы ошибок стирания FLASH STM32
		  Compare,                                             //↓ Переменная для поиска ошибок
          FrimwareDataWrite[64],                               //↓ буфер для хранения одной страницы FLASH STM32 размер 2048 байт
          USB_CalculateCRC;                                    //↓ Переменная для хранения подсчитанных CRC отправленных данных

uint16_t  FlashMemoryTimeOut = 1000,                           //↓ Таймаут работы шины SPI
          counter,                                             //↓ Указатель буфера обновления
          FlashSector,                                         //↓ Указатель системы обновления
          VerBootloader = 600;                                 //↓ Версия прошивки загрузчика

uint8_t   UpdateFlag = 20,                                     //↓ Сигнал о готовности системы обновления к обновлению
          UpdateReady = 0,                                     //↓ Переменная для пердачи данных через основной цикл
          UpdateSetup = 0,                                     //↓ Переменная управляющая системой обновления
          versionStatus = 0,                                   //↓ Переменная для вызова версии загруженной прошивки через основной цикл
          UpdateOk = 10,                                       //↓ Загружаем основную прошивку
          NewFrimware =20,                                     //↓ Переходим в режим обновления прошивки
          BadFrimware =255,                                    //↓ Битая прошивка, переходим режим БООТЛОАДЕР с включением USB
          UpdateStatus=0,                                      //↓ Проверка на наличе новой прошивки, 2 есть новая прошивка, 0 нет
          FrimwareStatus = 0,                                  //↓ Статус проверки прошивки на ошибки, 1 ошибки есть, 0 ошибок нет
          FrimwareData [256],                                  //↓ буфер для хранения данных прошивки
          FrimwareDataEncrypt [256],                           //↓ буфер для хранения расшифрованны данных
		  FrimwareUpdateLock = 0,                              //↓ Статус системы обновления 1 разблокирован 0 заблокирован
          W25_READ_STATUS_1[1] = {0x05},                       //↓ буфер для хранения одной страницы с FLASH микросхемы размер 256 байт для проверки на ошибки
          W25_CHIP_ERASE[1]    = {0xC7},                       //↓ буфер для хранения одной страницы с FLASH микросхемы размер 256 байт для проверки на ошибки
          W25_WRITE_DISABLE[1] = {0x04},                       //↓ буфер для хранения одной страницы с FLASH микросхемы размер 256 байт для проверки на ошибки
          W25_WRITE_ENABLE[1]  = {0x06};                       //↓ буфер для хранения одной страницы с FLASH микросхемы размер 256 байт для проверки на ошибки

//=====================================================Ключ расшифровки прошивки=====================================================
const uint8_t  Key[32] = {0x2B, 0xDE, 0x48, 0xFF, 0xA4, 0xEB, 0x94, 0x79, 0xBC, 0x05, 0x39, 0x4C, 0x8A, 0x66, 0xC1, 0x8D,         //↓
                          0x65, 0x1B, 0x76, 0x20, 0x91, 0xC4, 0xAD, 0x14, 0x6A, 0xF4, 0x95, 0xE4, 0x3B, 0xEC, 0xF4, 0xF3 };       //↓
const uint8_t  Iv[16] =  {0xBA, 0x46, 0x9E, 0x25, 0x47, 0xCA, 0x48, 0xB4, 0xD1, 0xF8, 0xAE, 0x10, 0xF5, 0xDD, 0x31, 0x19 };       //↓
//===================================================================================================================================

uint8_t State_CRS = 60;
uint8_t DeliveredState      = 0;
uint16_t DeliveredTimer       = 0;
uint8_t DeliveredAttempts    = 0;
uint16_t DeliveredTimeOut     = 300;
uint8_t DeliveredMaxAttempts = 3;
uint8_t mesageNum = 0;

void Transmit_Data_Out_Delivered(uint8_t *Recive_USB){

	uint32_t DELIVERED_CRC =  Recive_USB[0]  |(Recive_USB[1] << 8) |(Recive_USB[2] << 16) |(Recive_USB[3] << 24);

	if(USB_CalculateCRC == DELIVERED_CRC){
		ResetCheckCRC();
	}
}

void ResetCheckCRC(void){
	DeliveredTimer = 0;
	DeliveredAttempts = 0;
	DeliveredState = 1;
}


void Transmit_Data_Out(uint8_t* data, uint8_t dataLen, uint16_t timeDelay, uint8_t crcCheck){
	sDataOut dataOut;
	memcpy(dataOut.data, data, dataLen);
	dataOut.auxiliary[1] = 2; //usb control byte
	/*If the message is not more than 64 bytes, then send it as is*/
	dataOut.auxiliary[2] = BYTE_WHOLE;
	dataOut.auxiliary[3] = dataLen;
	USBD_CUSTOM_HID_SendReport(&hUsbDeviceFS, (uint8_t *)&dataOut+17, 65);
	HAL_Delay(timeDelay);
}


void main_Menu(uint8_t *Received_String){//MENU
	uint8_t Bias = 3, Package = 0, Function = 0, ControlByte = 0;
	Package = strtoul((char *)Received_String, NULL, 10);
	Function = strtoul((char *)Received_String + 2, NULL, 10);
	if(Package>9)Bias++;
	if(Package>99)Bias++;
	if(Function>9)Bias++;
	if(Function>99)Bias++;
	ControlByte = Received_String[Bias];
	Bias++;
	if(Package == PACKED_SYSTEM){
		SYSTEM_PACKED(ControlByte, Function, Received_String+Bias);
    }else{
    	CommandError();
	}
}

void SYSTEM_PACKED(uint8_t ControlByte, uint8_t Function, uint8_t *Received_data){

	     if(Function == SYSTEM_FU_FIRMWARE_DOWNLOAD) {  FIRMWARE_DOWNLOAD (Received_data);      } //
    else if(Function == SYSTEM_FU_PREPARING_UPDATE)  {  PREPARING_UPDATE (Received_data);       } //
    else if(Function == SYSTEM_FU_CANCEL)            {  UPDATE_CANCEL();                        }
    else if(Function == SYSTEM_INFORMATION)          {  Version();                              } //
    else if(Function == SYSTEM_FU_COMPLETE_UPDATE)   {  COMPLETE_UPDATE();                      } //
    else if(Function == SYSTEM_FU_CHECK_FIRMWARE)    {  CHECK_FIRMWARE();                       } //
    else if(Function == SYSTEM_ACCESS_CONTROL)       {  UPDATE_SYSTEM_UNLOCK(Received_data);    } //
    else if(Function == SYSTEM_RESTART)              {  UpdateReady = 14  ;                     } //
    else if(Function == SYSTEM_SPI_FLASH_STATE)      {  FlashStatus();                          }
     else{
    	 CommandError();
     }
}

void  FIRMWARE_DOWNLOAD(uint8_t *Received_data){

	if(FrimwareUpdateLock == 1){                                                                                              //↓ Если система обновления разблокированна продолжаем
		                                                                                                                      //
		for (short i = 0; i<58;i++){                                                                                          //↓
			FrimwareData[counter]=Received_data[i];                                                                            //↓
			FirmwareSizeCounter++;                                                                                            //↓
			counter++;                                                                                                        //↓
			if (counter==256){                                                                                                //↓
		       FlashMemoryWrite(FlashSector, &(FrimwareData[0]), 256);                                                        //↓ Процедура сборки прошивки и запись во FLASH микросхему
			   counter=0;                                                                                                     //↓
			   FlashSector++;                                                                                                 //↓
			}                                                                                                                 //↓
		}
		UpdateReady = 10;
	}else{
		UpdateReady = 13;
	}
}


void  PREPARING_UPDATE(uint8_t *Received_data){
	if(FrimwareUpdateLock == 1){
		UpdateInfo.Firmware_Size   =  Received_data[0]  |(Received_data[1] << 8) |(Received_data[2] << 16) |(Received_data[3] << 24);     //↓ Получаем реальный размер прошивки
		CriptFrimwareSize          =  Received_data[4]  |(Received_data[5] << 8) |(Received_data[6] << 16) |(Received_data[7] << 24);     //↓ Получаемразмер прошивки в зашифрованной форме
		CheckCRC                   =  Received_data[8]  |(Received_data[9] << 8)|(Received_data[10] << 16)|(Received_data[11] << 24);    //↓ Получаем кнотрольную сумму расчитанную в VisualStudio для проверки
		UpdateInfo.Frimware_CRC    =  Received_data[12] |(Received_data[13] << 8)|(Received_data[14] << 16)|(Received_data[15] << 24);    //↓ Получаем контрольную сумму расшифрованной прошивки для бутлоадера//Получаем контрольную сумму расшифрованной прошивки для бутлоадера
		UpdateInfo.ApplicationAddr =  Received_data[16] |(Received_data[17] << 8)|(Received_data[18] << 16)|(Received_data[19] << 24);    //↓ Получаем адрес рассположения оснвоной прошивки
		UpdateInfo.Erase_Sector    =  Received_data[20];                                                                                  //↓ Получаем начальны сектор откуда начинаем стирать.
		UpdateInfo.Erase_Count     =  Received_data[21];                                                                                    //↓ Получаем данные сколько нужно стирать
		if(UpdateInfo.ApplicationAddr < DEVICE_INFO_ADDRESS || UpdateInfo.Erase_Sector <2 || UpdateInfo.Erase_Count>10){
			UpdateReady = 12;
		}else{
			FlashMemoryWriteEnable();                                                                                        //↓ Включаем запись во влешь память
			for(short i=0; i<255; i++){                                                                                      //↓
				FlashSectorErase(i);                                                                                           //↓ Полностью очищаем флешь микросхему
			}                                                                                                                //↓			                                                                                                                         //
			FirmwareSizeCounter=0;                                                                                           //↓ Очищаем счётчик прошивки
			counter=0;                                                                                                       //↓ Очищаем счётчик пачки прошивки
			FlashSector=0;                                                                                                   //↓ Очищаем счётчик секторов
			FlashMemoryWriteEnable();                                                                                        //↓ Включаем запись во влешь память
			UpdateReady = 2;                                                                                                   //↓ Отправляем сообщение в USB что готовы принимать прошивку
		}
	}
}

void  UPDATE_SYSTEM_UNLOCK(uint8_t *Received_data){
	if(Received_data[0] ==  '5' && Received_data[1] ==  '1' && Received_data[2] =='0' && Received_data[3] =='2' && Received_data[4] =='8' && Received_data[5] =='6'){
		FrimwareUpdateLock = 1;
		UpdateReady = 8;
	}else{
		FrimwareUpdateLock = 0;
		UpdateReady = 9;
	}
}

void CHECK_FIRMWARE(){
	if (FirmwareSizeCounter>=CriptFrimwareSize){
		FlashMemoryWriteDisable();
	    CalculateCRC = 0xFFFFFFFF;
		for  (uint32_t i=0;i<CriptFrimwareSize;i+=256){
			FlashMemoryRead( i/256, &(FrimwareData[0]), 256);
			for (uint16_t y= 0; y<256;y++){
				CalculateCRC = crc_table[(CalculateCRC ^ FrimwareData[y]) & 0xFF] ^ (CalculateCRC >> 8);
			}
 	    }
		CalculateCRC ^= 0xFFFFFFFF;
		if( CheckCRC == CalculateCRC){
			UpdateReady = 3;
		}else{
			FirmwareIsCorrupted();
		}
	} else {
		UpdateReady = 5;
	}
}

void COMPLETE_UPDATE(){
	if( CheckCRC == CalculateCRC){                                                                                        //↓ Сравнимаем CRC принятого по USB и расчитанного здесь
		DevInfo.Firmware_UpdateInfo.State = New_Frimware;                                                                 //↓ Записываем флаг статуса 20 (обновляемся)
		Write_UpdateInfo();                                                                                               //↓ Записываем CRC и размер прошивки во FLASH память
		UpdateReady = 14;                                                                                         //↓ Перезагружаем контроллер для обновления прошивки STM32
	}else{                                                                                                                //
		FirmwareIsCorrupted();                                                                                                //↓ Отправляем сообщение по USB ОШИБКА верификация не пройдена
	}                                                                                                                     //
}
void FirmwareIsCorrupted()   {UpdateReady = 4;                                                }
void UPDATE_CANCEL()         {UpdateReady = 7;                                                }
void FlashStatus()           {UpdateReady = 1;                                                }
void CommandError()          {UpdateReady = 6;                                                }
void Restart(void)           {NVIC_SystemReset();                                             }
void Version(void)           {versionStatus = 1;                                              }
void SPI_CS_FLASH_ON(void)   {HAL_GPIO_WritePin(GPIOB, SPI_CS_EEPROM_Pin, GPIO_PIN_SET);      }
void SPI_CS_FLASH_OFF(void)  {HAL_GPIO_WritePin(GPIOB, SPI_CS_EEPROM_Pin, GPIO_PIN_RESET);    }
void LED_ON(void)            {HAL_GPIO_WritePin(GPIOB, LED_Pin, GPIO_PIN_SET);                }
void LED_OFF(void)           {HAL_GPIO_WritePin(GPIOB, LED_Pin, GPIO_PIN_RESET);              }

void Flash_Unlock(){
	uint8_t attempt = 3;
	while (HAL_FLASH_Unlock() != HAL_OK || attempt !=0 ) {
		attempt--;
	}
}

void Flash_Lock(){
	uint8_t attempt = 3;
	while (HAL_FLASH_Lock() != HAL_OK || attempt !=0 ) {
		attempt--;
	}
}

void BOOTLOADER(void){
	//=======================================================Запуск BOOTLOADER======================================================
	                                                                                                                              //
	for (uint16_t i = 0; i < 256; i++){                                                                                           //↓
   	CalculateCRC = i;                                                                                                             //↓
	    for (uint8_t j = 0; j < 8; j++){                                                                                          //↓
	    	CalculateCRC = (CalculateCRC & 1) != 0  ? (CalculateCRC >> 1) ^ 0xEDB88320 : CalculateCRC >> 1;                       //↓
	    }                                                                                                                         //↓
	    crc_table[i] = CalculateCRC;                                                                                              //↓
	 };                                                                                                                           //↓ Инициализация таблиц подсчёта CRC
     CalculateCRC = 0xFFFFFFFF;

     HAL_StatusTypeDef	flash_ok = HAL_ERROR;                                                                                     //↓ Состояние FLASH STM 32

	 EraseInitStruct.TypeErase    = FLASH_TYPEERASE_SECTORS;                                                                      //↓ Указываем что стираем секторами
   	 EraseInitStruct.VoltageRange = FLASH_VOLTAGE_RANGE_3;                                                                        //↓ Указываем напряжение паралельного стирания

	 Launch_Checking_The_Firmware();

	 if(DevInfo.Firmware_UpdateInfo.StateIWDG == 1){
		 IWGNINIT();
	 }

	  if(DevInfo.Firmware_UpdateInfo.State == Ok_Frimware){                                                                                //↓ Проверяем и если надо прыгаем в основную программу
                                                                                                                                  //
		  Jump_To_Dixom_Frimware();                                                                                               //↓ Нет новой прошивки прыжок в основную программу
                                                                                                                                  //
	  }else if(DevInfo.Firmware_UpdateInfo.State == New_Frimware){                                                                                        //↓ Есть новая прошивка, обновляемся
		                                                                                                                          //
		 //
		  //==================================Запуск системы обновления обновления STM32============================================
		  HAL_GPIO_TogglePin(GPIOB, LED_Pin);                                                                                     //↓ Моргнём светодиодом

		  uint8_t checkCount = 0;
	      while (flash_ok != HAL_OK) {                                         //↓
			flash_ok = HAL_FLASH_Unlock();          //↓ Разблокируем FLASH STM32
			if (checkCount > 10) {
				NVIC_SystemReset();
			}
			checkCount++;
		  }                                                                                                                       //↓
		 			                                                                                                              //
		  short FrimwareSectorCount = ceil((float)DevInfo.Firmware_UpdateInfo.Firmware_Size/FlashLenList);                        //↓ Определяем сколько пачек для расшифровки в прошивке

		  UpdateInfo.State = Bad_Frimware;                                                                                        //↓ Записывает 255 в случае не успеха грузимся в BOOTLOADER
	      Write_UpdateInfo();

		  EraseInitStruct.Sector       = UpdateInfo.Erase_Sector;                                                                 //↓ Указываем адресс начального сектора для стирания
		  EraseInitStruct.NbSectors    = UpdateInfo.Erase_Count;

		  if(UpdateInfo.ApplicationAddr < DEVICE_INFO_ADDRESS || UpdateInfo.Erase_Sector <2 || UpdateInfo.Erase_Count>10){

			  UpdateReady = 5;

		  }else{

			  while (flash_ok != HAL_OK) {                                         													  //↓
			 	flash_ok = HAL_FLASH_Unlock();          																				  //↓ Заблокируем FLASH STM32/
			  }

			  HAL_FLASHEx_Erase(&EraseInitStruct, &error);                                                                           //↓ Стираем FLASH STM32 с параметрами EraseInitStruct

			  HAL_Delay(20);                                                                                                         //↓ Чуть подождём после стирания флешь FLAH STM32
			  uint8_t ledTime = 0;                                                                                                    //
	                                                                                                            //
			  //=======================================Цикл загрузки прошивки в STM32===================================================
			  for(short FlashSectorCount = 0; FlashSectorCount<FrimwareSectorCount; FlashSectorCount++ ){                             //↓
			 			                                                                                                           	  //
				  if(DevInfo.Firmware_UpdateInfo.StateIWDG == 1){		                                                       	      //↓ Сброс таймера сторожевой собаки
					  HAL_IWDG_Refresh(&hiwdg);
				  }
	        	                                                                                                                      //
				  if(ledTime == 0) HAL_GPIO_TogglePin(GPIOB, LED_Pin);                                                                //↓ Моргаем светодиодом
				  ledTime++;                                                                                                          //↓ Увеличиваем таймер мигания свтеодилода
				  if(ledTime>20) ledTime = 0;                                                                                         //↓ Сбрасываем таймер для мигания свтодиодом
			 				                                                                                                          //
				  FlashMemoryRead(FlashSectorCount, &(FrimwareData[0]), FlashLenList);                                                //↓ Чтение одной страницы с FLASH микросхемы
				                                                                                                                      //
				  AES_CBC_decrypt_buffer(FrimwareDataEncrypt, FrimwareData, FlashLenList, Key, Iv);                                   //↓ Расшифровка прошивки
				                                                                                                                      //
				  for(short byte = 0; byte < FlashLenList; byte++){                                                                   //↓ Цикл подсчёта CRC
					  CalculateCRC = crc_table[(CalculateCRC ^ FrimwareDataEncrypt[byte]) & 0xFF] ^ (CalculateCRC >> 8);              //↓
				  }                                                                                                                   //↓
			                                                                                                                          //
				  for(short byte=0; byte<64; byte++){                                                                                 //↓ Конвертация uint8 в uint32 для stm32
					  FrimwareDataWrite[byte] = FrimwareDataEncrypt[0+(byte*4)]                                                       //↓
			 							     | (FrimwareDataEncrypt[1+(byte*4)] << 8)                                                 //↓
			 								 | (FrimwareDataEncrypt[2+(byte*4)] << 16)                                                //↓
			 								 | (FrimwareDataEncrypt[3+(byte*4)] << 24);                                               //↓
				  }                                                                                                                   //↓
			                                                                                                                          //
				  for(short byte=0; byte<64; byte++){                                                                                 //↓ Цикл закрузки в STM32
					  HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, UpdateInfo.ApplicationAddr+
							                      ((byte*4)+(FlashSectorCount*FlashLenList)), FrimwareDataWrite[byte]);               //↓
				  }                                                                                                                   //↓
			  }                                                                                                                       //
			  //==================================Завершение процедуры обновления STM32================================================
			                                                                                                                          //
			  CalculateCRC ^= 0xFFFFFFFF;                                                                                             //↓
			                                                                                                                          //
			  flash_ok = HAL_ERROR;                                                                                                   //↓


			  while (flash_ok != HAL_OK) {                                         													  //↓
			 	flash_ok = HAL_FLASH_Lock();          																				  //↓ Заблокируем FLASH STM32/
			  }    																													  //
			  LED_OFF();                                                                                                              //↓ Отключаем свтодиод
			 			                                                                                                              //
			  if(DevInfo.Firmware_UpdateInfo.Frimware_CRC == CalculateCRC){	                                                                                      //↓ Сверяем контрольнные суммы CRC прошивки
				  Jump_To_Dixom_Frimware();                                                                                           //↓ Успешное обновления прыгаем в основную программу
			  }                                                                                                                       //
		  }



		  MX_USB_DEVICE_Init();  HAL_GPIO_WritePin(GPIOA, OTG_Pin, GPIO_PIN_SET);                                                 //↓ Неудачное обновления запускаем полный БУТЛОАДЕР с USB
	  }else{                                                                                                                      //↓ Битая прошивка
		  MX_USB_DEVICE_Init();  HAL_GPIO_WritePin(GPIOA, OTG_Pin, GPIO_PIN_SET);                                                 //↓ Битая прошивка запускаем полный БУТЛОАДЕР с USB
	  }                                                                                                                           //
	  //============================================================================================================================
}

void Jump_To_Dixom_Frimware(){
	//=============================================Прыжок в основную программу======================================================
	jumpAddress = *(__IO uint32_t*) (UpdateInfo.ApplicationAddr + 4);                                                                    //↓ Извлекаем адрес перехода из вектора Reset
	HAL_RCC_DeInit();                                                                                                             //↓ Убираем инициализацию RCC
//	HAL_DeInit();                                                                                                                 //↓ Убираем инициализацию HAL
	Jump_To_Application = (pFunction) jumpAddress;                                                                                //↓ Приводим его к пользовательскому типу
	__disable_irq();                                                                                                              //↓ Запрещаем прерывания
	__set_MSP(*(__IO uint32_t*) UpdateInfo.ApplicationAddr);                                                                             //↓ Устанавливаем SP приложения (вершину стека прошивки)
	Jump_To_Application();                                                                                                        //↓ Прыжок в основную прошивку Dxiom
}//=================================================================================================================================






void BOOTLOADER_WHILE(void){

	if(UpdateReady == 1){
		FlashRegistrWrite(W25_WRITE_ENABLE,  1);
		FlashRegistrRead(W25_READ_STATUS_1, 1, &FlashMemoryRegStatus.all, 1);
		if(FlashMemoryRegStatus.all == 2){
			Transmit_Data_Out((uint8_t *)MessageUpdate_2_1_5, 31, 0, 1);
		}else{
			Transmit_Data_Out((uint8_t *)MessageUpdate_2_1_6, 28, 0, 1);
		}
		FlashRegistrWrite(W25_WRITE_DISABLE,  1);
		UpdateReady = 0;
	}else if(UpdateReady == 2){
		Transmit_Data_Out((uint8_t *)MessageUpdate_2_1_0, 33, 0, 1);
		UpdateReady = 0;
	}else if(UpdateReady == 3){
		Transmit_Data_Out((uint8_t *)MessageUpdate_2_1_1, 38, 20, 1);
		UpdateReady = 0;
	}else if(UpdateReady == 4){
		Transmit_Data_Out((uint8_t *)MessageUpdate_2_1_2, 38, 0, 1);
		UpdateReady = 0;
	}else if(UpdateReady == 5){
		Transmit_Data_Out((uint8_t *)MessageUpdate_2_1_3, 31, 0, 1);
		UpdateReady = 0;
	}else if(UpdateReady == 7){
		Transmit_Data_Out((uint8_t *)MessageUpdate_2_1_8, 14, 0, 0);
		UpdateReady = 0;
	}else if(UpdateReady == 6){
		Transmit_Data_Out((uint8_t *)SysMessage_ERROR, 26, 0, 0);
		UpdateReady = 0;
	}else if(UpdateReady == 8){
		Transmit_Data_Out((uint8_t *)MessageUpdate_2_1_9, 30, 0, 0);
		UpdateReady = 0;
	}else if(UpdateReady == 9){
		Transmit_Data_Out((uint8_t *)MessageUpdate_2_1_10, 30, 0, 0);
		UpdateReady = 0;
	}else if(UpdateReady == 10){
		Transmit_Data_Out((uint8_t *)MessageUpdate_2_1_16, 28, 0, 0);
		UpdateReady = 0;
	}else if(UpdateReady == 11){
		Transmit_Data_Out((uint8_t *)SystemNotifi_SysCommandBlocked, 36, 0, 0);
		UpdateReady = 0;
	}else if(UpdateReady == 12){
		Transmit_Data_Out((uint8_t *)MessageUpdate_0_1_19, 37, 0, 0);
		UpdateReady = 0;
	}else if(UpdateReady == 13){
		Transmit_Data_Out((uint8_t *)MessageUpdate_0_1_20, 23, 0, 0);
		UpdateReady = 0;
	}else if(UpdateReady == 14){
	    Transmit_Data_Out((uint8_t *)MessageUpdate_2_1_7, 29, 20, 1);
	    UpdateReady = 0;
		HAL_Delay(1000);
		NVIC_SystemReset();
	}
	HAL_GPIO_TogglePin(GPIOB, LED_Pin);
	HAL_Delay(20);

	if(versionStatus == 1){
		Transmit_Data_Out((uint8_t *)"0 3 0 1 2 2 0.000 6.00 0.0 Platform Dixom-m Bootloader", 54, 20, 1);    //Palfthorma version information
		Transmit_Data_Out((uint8_t *)"0 3 1 0000-0000-0000-0000 000000000000", 38, 20, 1);  //Serial number and activation information
		Transmit_Data_Out((uint8_t *)"0 3 2 02.04.2024 0 0 0 0", 24, 20, 1);   //Information about the life of the platform
		Transmit_Data_Out((uint8_t *)"0 3 3 0.000 0.00 0 0", 20, 20, 1);    //Information about the USB sound card
		Transmit_Data_Out((uint8_t *)"0 5 0 0 2 1 0 0", 15, 20, 1);    //Connection interface information
		Transmit_Data_Out((uint8_t *)"0 3 5 6.00", 10, 20, 1);    //Connection interface information
		versionStatus = 0;
	}

	 if(DevInfo.Firmware_UpdateInfo.StateIWDG == 1){		                                                       	      //↓ Сброс таймера сторожевой собаки
		 HAL_IWDG_Refresh(&hiwdg);
	 }
}


void Launch_Checking_The_Firmware(){

	static uint8_t ReadingAttempts;
	uint32_t CheckSumRX;

	Spi_Flash_Read_CheckSumm(&CheckSumRX, SpiFlashAddrUpdateInfo, (uint32_t*)&DevInfo.Firmware_UpdateInfo, sizeof(DevInfo.Firmware_UpdateInfo)/4);

	if(CheckSumRX != DevInfo.Firmware_UpdateInfo.CheckSum){
		if(ReadingAttempts<5){
			ReadingAttempts++;
			HAL_Delay(50);
			Launch_Checking_The_Firmware();
		}
	}else{
		if(DevInfo.Firmware_UpdateInfo.Boot_VerFrimware!= VerBootloader){
			DevInfo.Firmware_UpdateInfo.StateIWDG = 1;
			DevInfo.Firmware_UpdateInfo.Boot_VerFrimware = VerBootloader;
			Write_UpdateInfo();
		}
	}
}


void Write_UpdateInfo(){
	FlashMemoryWriteEnable();                                                                                               //↓ Включить возможность записи на флешку
	FlashSectorErase(255);                                                                                                  //↓ Стирает сектор 4кБ ( 0 по 256 )
	FlashMemoryWriteEnable();
	Spi_Flash_Write_CheckSumm(&DevInfo.Firmware_UpdateInfo.CheckSum, SpiFlashAddrUpdateInfo, (uint32_t*)&DevInfo.Firmware_UpdateInfo, sizeof(DevInfo.Firmware_UpdateInfo)/4);
	FlashMemoryWriteDisable();
}




void FlashMemoryWrite(short page, uint8_t* data, short len){

	uint32_t addr =page*256;
	uint8_t WriteAddr[4]  = {0x02, (addr >> 16) & 0xFF, (addr >> 8) & 0xFF, addr & 0xFF};
	FlashMemoryBusyStatus();
	if(FlashMemoryRegStatus.bit.write_enable==0)  FlashMemoryWriteEnable();
	SPI_CS_FLASH_OFF();
	HAL_SPI_Transmit(&hspi2, WriteAddr, 4, FlashMemoryTimeOut);
	HAL_SPI_Transmit(&hspi2, data, len, FlashMemoryTimeOut);
	SPI_CS_FLASH_ON();
}

void FlashMemoryRead(short page, uint8_t* readData, short readLen){

	uint32_t addr =page*256;
	uint8_t ReadAddr[4]  = {0x03,(addr >> 16) & 0xFF, (addr >> 8) & 0xFF, addr & 0xFF};
	FlashMemoryBusyStatus();
	SPI_CS_FLASH_OFF();
	HAL_SPI_Transmit(&hspi2, ReadAddr, 4, FlashMemoryTimeOut);
	HAL_SPI_Receive (&hspi2, readData, readLen, FlashMemoryTimeOut);
	SPI_CS_FLASH_ON();
}

void FlashRegistrWrite(uint8_t* data, short len){

	SPI_CS_FLASH_OFF();
	HAL_SPI_Transmit(&hspi2, data, len, FlashMemoryTimeOut);
	SPI_CS_FLASH_ON();
}

void FlashRegistrRead(uint8_t* sendData, short sendLen, uint8_t* readData, short readLen){

	SPI_CS_FLASH_OFF();
	HAL_SPI_Transmit(&hspi2, sendData, sendLen, FlashMemoryTimeOut);
	HAL_SPI_Receive (&hspi2, readData, readLen, FlashMemoryTimeOut);
	SPI_CS_FLASH_ON();
}

void FlashSectorErase(short sector){

	uint32_t addr =sector*4096;
	uint8_t ReadAddr[4]  = {0x20, (addr >> 16) & 0xFF, (addr >> 8) & 0xFF, addr & 0xFF};
	FlashMemoryBusyStatus();
	if(FlashMemoryRegStatus.bit.write_enable==0)  FlashMemoryWriteEnable();
	SPI_CS_FLASH_OFF();
	HAL_SPI_Transmit(&hspi2, ReadAddr, 4, FlashMemoryTimeOut);
	SPI_CS_FLASH_ON();
}

void FlashMemoryErase(){

	FlashMemoryBusyStatus();
	if(FlashMemoryRegStatus.bit.write_enable==0)  FlashMemoryWriteEnable();
	FlashRegistrWrite(W25_CHIP_ERASE, 1);
}

void FlashMemoryWriteEnable(){

	do{
		FlashRegistrWrite(W25_WRITE_ENABLE,  1);
		FlashRegistrRead(W25_READ_STATUS_1, 1, &FlashMemoryRegStatus.all, 1);
	}while(FlashMemoryRegStatus.bit.write_enable==0);
}

void FlashMemoryWriteDisable(){

	do{
		FlashRegistrWrite(W25_WRITE_DISABLE, 1);
		FlashRegistrRead(W25_READ_STATUS_1, 1, &FlashMemoryRegStatus.all, 1);
	}while(FlashMemoryRegStatus.bit.write_enable==1);
}

void FlashMemoryBusyStatus(){

	do{
		FlashRegistrRead(W25_READ_STATUS_1, 1, &FlashMemoryRegStatus.all, 1);
	}while(FlashMemoryRegStatus.bit.busy==1);
}

void SpiFlashErase(){
	for(short i=0; i<256; i++){
	  FlashSectorErase(i);
    }
}

uint32_t Converter_8_to_u32(uint8_t* bytes) {
  uint32_t u32 = (bytes[0] << 24) + (bytes[1] << 16) + (bytes[2] << 8) + bytes[3];
  return u32;
}

void Converter_u32_to_u8 (uint32_t u32, uint8_t* u8) {
  u8[0] = (u32 & 0xff000000) >> 24;
  u8[1] = (u32 & 0x00ff0000) >> 16;
  u8[2] = (u32 & 0x0000ff00) >> 8;
  u8[3] = u32 & 0x000000ff;
}


void Spi_Flash_Write_CheckSumm(uint32_t *flash_checkSum, uint32_t page_address, uint32_t* data_address, uint16_t length){

	uint16_t PageLen = 256; //Flash page len
	uint8_t VarType = 4;    //4 - uint32_t, 2 - uint16_t, 1 - uint8_t
	uint8_t WriteData[256];
	uint16_t WriteDataLen;
	uint32_t data;
	uint8_t NumSector = length/(PageLen/VarType);
	uint16_t RemainLen = length;

	*flash_checkSum = 0xFFFFFFFF;

	for (uint16_t word = 1; word < length; word++) {
		*flash_checkSum = crc_table[(*flash_checkSum ^ *(data_address + word)) & 0xFF] ^ (*flash_checkSum >> 8);
	}

	*flash_checkSum ^= 0xFFFFFFFF;


	for(uint8_t page = 0; page <= NumSector; page++){

		WriteDataLen = 0;

		for(uint16_t byte = 0; byte < RemainLen; byte++){
			if((PageLen/VarType)>byte){
				data = *((data_address + byte)+page*(PageLen/VarType));
				Converter_u32_to_u8(data, &WriteData[byte*4]);
				WriteDataLen++;
			}else{
				byte = length;
				RemainLen = RemainLen-(PageLen/VarType);
			}
		}
		FlashMemoryWrite(page+page_address, &WriteData[0], PageLen);
	}
}


void Spi_Flash_Read_CheckSumm(uint32_t *read_checkSum, uint32_t page_address, uint32_t* data_address, uint16_t length){

	uint16_t PageLen = 256;   //Flash page len
	uint8_t VarType = 4;      //4 - uint32_t, 2 - uint16_t, 1 - uint8_t
	uint8_t ReadData[256];
	uint32_t data;
	uint8_t NumSector = length/(PageLen/VarType);
	uint16_t RemainLen = length;
	*read_checkSum = 0xFFFFFFFF;

	for(uint8_t page = 0; page <= NumSector; page++){

		FlashMemoryRead((page+page_address), &ReadData[0], PageLen);

		for(uint16_t byte = 0; byte < RemainLen; byte++){
			if((PageLen/VarType)>byte){

				data = Converter_8_to_u32(&ReadData[byte*4]);

				*((data_address + byte)+page*(PageLen/VarType)) = data;

				if(page!=0 || byte!=0){

					*read_checkSum = crc_table[(*read_checkSum ^ data) & 0xFF] ^ (*read_checkSum >> 8);
				}

			}else{
				RemainLen = RemainLen-(PageLen/VarType);
				byte = RemainLen;
			}
		}
	}
	*read_checkSum ^= 0xFFFFFFFF;
}













