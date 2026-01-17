/*
 * sRCC.h
 *
 *  Created on: 8 ���. 2021 �.
 *      Author: 79825
 */

#ifndef STM32_I2C_REGISTER_H_
#define STM32_I2C_REGISTER_H_

#include "stdio.h"
  #define sI2C1                ((sI2C_TypeDef *) I2C1_BASE)

typedef struct{
	volatile uint32_t  PE        :1;  //0
	volatile uint32_t  SMBUS     :1;  //1
	volatile uint32_t  Reserved1 :1;  //2
	volatile uint32_t  SMBTYPE   :1;  //3
	volatile uint32_t  ENARP     :1;  //4
	volatile uint32_t  ENPEC     :1;  //5
	volatile uint32_t  ENGC      :1;  //6
	volatile uint32_t  NOSTRETCH :1;  //7
	volatile uint32_t  START     :1;  //8
	volatile uint32_t  STOP      :1;  //9
	volatile uint32_t  ACK       :1;  //10
	volatile uint32_t  POS       :1;  //11
	volatile uint32_t  PEC       :1;  //12
	volatile uint32_t  ALERT     :1;  //13
	volatile uint32_t  Reserved2 :1;  //14
	volatile uint32_t  SWRST     :1;  //15
	volatile uint32_t  Reserved3 :16; //16-31
} sI2C_CR1;

typedef struct{
	volatile uint32_t  FREQ      :6;   //0-5
	volatile uint32_t  Reserved1 :2;   //6-7
	volatile uint32_t  ITERREN   :1;   //8
	volatile uint32_t  ITEVTEN   :1;   //9
	volatile uint32_t  ITBUFEN   :1;   //10
	volatile uint32_t  DMAEN     :1;   //11
	volatile uint32_t  LAST      :1;   //12
	volatile uint32_t  Reserved2 :19;  //13-31
} sI2C_CR2;

typedef struct{
	volatile uint32_t  SB        :1;  //0
	volatile uint32_t  ADDR      :1;  //1
	volatile uint32_t  BTF       :1;  //2
	volatile uint32_t  ADD10     :1;  //3
	volatile uint32_t  STOPF     :1;  //4
	volatile uint32_t  Reserved1 :1;  //5
	volatile uint32_t  RxNE      :1;  //6
	volatile uint32_t  TxE       :1;  //7
	volatile uint32_t  BERR      :1;  //8
	volatile uint32_t  ARLO      :1;  //9
	volatile uint32_t  AF        :1;  //10
	volatile uint32_t  OVR       :1;  //11
	volatile uint32_t  PECERR    :1;  //12
	volatile uint32_t  Reserved2 :1;  //13
	volatile uint32_t  TIMEOUT   :1;  //14
	volatile uint32_t  SMBALERT  :1;  //15
	volatile uint32_t  Reserved3 :16; //16-31
} sI2C_SR1;

typedef struct{
	volatile uint32_t  MSL       :1;  //0
	volatile uint32_t  BUSY      :1;  //1
	volatile uint32_t  TRA       :1;  //2
	volatile uint32_t  Reserved1 :1;  //3
	volatile uint32_t  GENCALL   :1;  //4
	volatile uint32_t  SMBDEFAULT:1;  //5
	volatile uint32_t  SMBHOST   :1;  //6
	volatile uint32_t  DUALF     :1;  //7
	volatile uint32_t  PEC       :8;  //8-15
	volatile uint32_t  Reserved3 :16; //16-31
} sI2C_SR2;

typedef struct{
	volatile uint32_t  DR        :8;  //1-7
	volatile uint32_t  Reserved1 :24; //8-31
} sI2C_DR;


typedef struct
{
  volatile sI2C_CR1 CR1;        /*!< I2C Control register 1,     Address offset: 0x00 */
  volatile sI2C_CR2 CR2;        /*!< I2C Control register 2,     Address offset: 0x04 */
  volatile uint32_t OAR1;       /*!< I2C Own address register 1, Address offset: 0x08 */
  volatile uint32_t OAR2;       /*!< I2C Own address register 2, Address offset: 0x0C */
  volatile sI2C_DR  DR;         /*!< I2C Data register,          Address offset: 0x10 */
  volatile sI2C_SR1 SR1;        /*!< I2C Status register 1,      Address offset: 0x14 */
  volatile sI2C_SR2 SR2;        /*!< I2C Status register 2,      Address offset: 0x18 */
  volatile uint32_t CCR;        /*!< I2C Clock control register, Address offset: 0x1C */
  volatile uint32_t TRISE;      /*!< I2C TRISE register,         Address offset: 0x20 */
  volatile uint32_t FLTR;       /*!< I2C FLTR register,          Address offset: 0x24 */
} sI2C_TypeDef;



#endif /* STM32_I2C_REGISTER_H_ */
