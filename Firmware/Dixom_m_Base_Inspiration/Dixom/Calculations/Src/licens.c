#include "md5.h"
#include "Exchange.h"
#include "licens.h"
#include <stdbool.h>
#include <string.h>
#include <stdint.h>
#include "main.h"
#include "Description.h"
#include <Structures.h>

extern sDixom Dixom;

#define STM32_UUID ((uint32_t *)0x1FFF7A10)

/*
 *STM32 F1 0x1FFFF7E8
 *STM32 F2
 *STM32 F3 0x1FFFF7AC
 *STM32 F4 0x1FFF7A10
 *
 */

uint8_t unique_ID[16],
        unique_ID_temp[12],
    //    productKey[12],
        productKeyToCheck[16],
        productKeyToCheckRes[12],
        resDataForKey[26],
        abcKey[16];

void DIXOM_KEY(uint8_t ControlByte,  uint8_t* data){



	if(ControlByte == DATA_SET){ //SET
		Transmit_EEPROM(EEPROM_ADDR_LICENS_KEY, data, 12, 10 );
		Delay(20, FreeRTOS);
		checkKey();
	}else{
		TransmitDataOutputs((uint8_t *)GeneralNotifi_CtlBitNotSupported, AutoLenght,  false,  true, allAvailable);

	}
}


void Software_CRC32_Initialization (){
	uint32_t CalculateCRC;

	CalculateCRC = 0xFFFFFFFF;

	for (uint16_t i = 0; i < 256; i++) {
		CalculateCRC = i;
		for (uint8_t j = 0; j < 8; j++) {
			CalculateCRC =	(CalculateCRC & 1) != 0 ?(CalculateCRC >> 1) ^ 0xEDB88320 :	CalculateCRC >> 1;
		}
		Dixom.Service.crc_table[i] = CalculateCRC;
	};
}

void Activation_Key_Initialization(){

	uint32_t idPart1 = STM32_UUID[0];
	uint32_t idPart2 = STM32_UUID[1];
	uint32_t idPart3 = STM32_UUID[2];

	unique_ID_temp[0]  = idPart1;
	unique_ID_temp[1]  = idPart1 >> 24;
	unique_ID_temp[2]  = idPart1 >> 16;
	unique_ID_temp[3]  = idPart1 >> 8;
	unique_ID_temp[4]  = idPart2;
	unique_ID_temp[5]  = idPart2 >> 16;
	unique_ID_temp[6]  = idPart2 >> 8;
	unique_ID_temp[7]  = idPart2 >> 24;
	unique_ID_temp[8]  = idPart3;
	unique_ID_temp[9]  = idPart3 >>  8;
	unique_ID_temp[10] = idPart3 >> 24;
	unique_ID_temp[11] = idPart3 >> 16;

	getMD5( &unique_ID_temp, 12, unique_ID );


	for( int i=0;i<=8;i++){
   		sprintf( resDataForKey+(i*2),"%02X", unique_ID[i]);
	}

	resDataForKey[16]='A';
	resDataForKey[17]='A';
	resDataForKey[18]='D';
	resDataForKey[19]='5';
	resDataForKey[20]='%';
	resDataForKey[21]='1';
	resDataForKey[22]='M';
	resDataForKey[23]='9';
	resDataForKey[24]='F';

	//AAD5%1M9F

	getMD5( resDataForKey, 25, productKeyToCheck );

	for( int i=0;i<=6;i++){
		sprintf( productKeyToCheckRes+(i*2),"%02X", productKeyToCheck[i]);
	}

	GET_PRODUCT_key_string(Dixom.DevInfo.SerialNumber);

	checkKey();
}

void checkKey(void){
	//Receiver_EEPROM( EEPROM_ADDR_LICENS_KEY,  productKey, 12, 15 );


	uint8_t cnt = 5;  //number of I2C read attempts
	do{
		cnt--;
		if(ExchangeStatus(	Receiver_I2C2(DEVICE_ADDR_EEPROM_24LC256 << 1, EEPROM_ADDR_LICENS_KEY, 2, Dixom.DevInfo.LicenseKey,	12, 200, ExchangeMemMainSteam), EEPROM_READ) == WR_OK){
			cnt = 0;
		}
	}while(cnt !=0);

	Dixom.DevInfo.ActivationStatus = YES;

	for (int i=0;i<sizeof(productKeyToCheckRes);i++){
		if (productKeyToCheckRes[i]!=Dixom.DevInfo.LicenseKey[i]){
			Dixom.DevInfo.ActivationStatus = NO;
			break;
		}
	 }
}

int	strncmp2(char *str1, char *str2, int size){
	if (Dixom.DevInfo.ActivationStatus == YES){
		return strncmp(str1,str2,size);

	} else return (-10);
}

int16_t protection(char *str1, char *str2, uint8_t size){
	if (Dixom.DevInfo.ActivationStatus == YES){
		return strncmp((char*)str1,(char*)str2,size);

	} else return (-10);
}

void GET_PRODUCT_key( ){

	uint8_t dataByte[2];

	for( int i=0;i<8;i++){
	     sprintf(dataByte,"%02X", unique_ID[i]);
	     USB_CDC_Transmit( &dataByte, 2 ,5);
		 if (((i+1)%2==0)&&(i!=7)){
			 USB_CDC_Transmit( "-", 1 ,5);
		 }
    }
    osDelay(10);
}

void  GET_PRODUCT_key_string( uint8_t key[] ){
    sprintf(key,"%02X%02X-%02X%02X-%02X%02X-%02X%02X", unique_ID[0],unique_ID[1],unique_ID[2],unique_ID[3],unique_ID[4],unique_ID[5],unique_ID[6],unique_ID[7]);
}




