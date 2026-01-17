#ifndef __LICENS_H__
#define __LICENS_H__

#include "stdint.h"


void             DIXOM_KEY(uint8_t ControlByte,  uint8_t* data);
void             Software_CRC32_Initialization();
void             Activation_Key_Initialization();
void             getMD5( uint8_t* text, uint32_t len,uint8_t* outText );
void             SET_PRODUCT_key( uint8_t* data, int len);
void             GET_PRODUCT_key_string( uint8_t key[] );
int              strncmp2(char *str1, char *str2, int size);
int16_t          protection(char *str1, char *str2, uint8_t size);

void             checkKey(void);

#endif
