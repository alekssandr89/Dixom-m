/*
 * sRCC.h
 *
 *  Created on: 8 ���. 2021 �.
 *      Author: 79825
 */

#ifndef STM32_F405_UART1_REGISTER_H_
#define STM32_F405_UART1_REGISTER_H_

#include "stdint.h"

#define rUART1                ((sUART1_TypeDef *) USART1_BASE)

typedef struct{
	volatile uint32_t  PE        :1;  //0
	volatile uint32_t  FE        :1;  //1
	volatile uint32_t  NF        :1;  //2
	volatile uint32_t  ORE       :1;  //3
	volatile uint32_t  IDLE      :1;  //4
	volatile uint32_t  RXNE      :1;  //5
	volatile uint32_t  TC        :1;  //6
	volatile uint32_t  TXE       :1;  //7
	volatile uint32_t  LBD       :1;  //8
	volatile uint32_t  CTS       :1;  //9
	volatile uint32_t  Reserved  :22; //10-31
} sUSART_SR;

typedef struct{
	volatile uint32_t  DR        :9;  //0-8
	volatile uint32_t  Reserved  :23; //9-31
} sUSART_DR;

typedef struct{
	volatile uint32_t  DIV_Fraction  :4;  //0-3
	volatile uint32_t  DIV_Mantissa  :12; //4-15
	volatile uint32_t  Reserved3     :16; //16-31
} sUSART_BRR;

typedef struct{
	volatile uint32_t  SBK       :1;  //0
	volatile uint32_t  RWU       :1;  //1
	volatile uint32_t  RE        :1;  //2
	volatile uint32_t  TE        :1;  //3
	volatile uint32_t  IDLEIE    :1;  //4
	volatile uint32_t  RXNEIE    :1;  //5
	volatile uint32_t  TCIE      :1;  //6
	volatile uint32_t  TXEIE     :1;  //7
	volatile uint32_t  PEIE      :1;  //8
	volatile uint32_t  PS        :1;  //9
	volatile uint32_t  PCE       :1;  //10
	volatile uint32_t  WAKE      :1;  //11
	volatile uint32_t  M         :1;  //12
	volatile uint32_t  UE        :1;  //13
	volatile uint32_t  Reserved1 :1;  //14
	volatile uint32_t  OVER8     :1;  //15
	volatile uint32_t  Reserved2 :16; //16-31
} sUSART_CR1;

typedef struct{
	volatile uint32_t  ADD       :4;  //0-3
	volatile uint32_t  Reserved1 :1;  //4
	volatile uint32_t  LBDL      :1;  //5
	volatile uint32_t  LBDIE     :1;  //6
	volatile uint32_t  Reserved2 :1;  //7
	volatile uint32_t  LBCL      :1;  //8
	volatile uint32_t  CPHA      :1;  //9
	volatile uint32_t  CPOL      :1;  //10
	volatile uint32_t  CLKEN     :1;  //11
	volatile uint32_t  STOP      :2;  //12-13
	volatile uint32_t  LINEN     :1;  //14
	volatile uint32_t  Reserved3 :17; //15-31
} sUSART_CR2;

typedef struct{
	volatile uint32_t  EIE       :1;  //0
	volatile uint32_t  IREN      :1;  //1
	volatile uint32_t  IRLP      :1;  //2
	volatile uint32_t  HDSEL     :1;  //3
	volatile uint32_t  NACK      :1;  //4
	volatile uint32_t  SCEN      :1;  //5
	volatile uint32_t  DMAR      :1;  //6
	volatile uint32_t  DMAT      :1;  //7
	volatile uint32_t  RTSE      :1;  //8
	volatile uint32_t  CTSE      :1;  //9
	volatile uint32_t  CTSIE     :1;  //10
	volatile uint32_t  ONEBIT    :1;  //11
	volatile uint32_t  Reserved  :20; //12-31
} sUSART_CR3;

typedef struct{
	volatile uint32_t  PSC       :8;  //0-7
	volatile uint32_t  GT        :8;  //8-15
	volatile uint32_t  Reserved  :16; //16-31
} sUSART_GTPR;


typedef struct
{
  volatile sUSART_SR   SR;    /* Status register,                   Address offset: 0x00 */
  volatile sUSART_DR   DR;    /* Data register,                     Address offset: 0x04 */
  volatile sUSART_BRR  BRR;   /* Baud rate register,                Address offset: 0x08 */
  volatile sUSART_CR1  CR1;   /* Control register 1,                Address offset: 0x0C */
  volatile sUSART_CR2  CR2;   /* Control register 2,                Address offset: 0x10 */
  volatile sUSART_CR3  CR3;   /* Control register 3,                Address offset: 0x14 */
  volatile sUSART_GTPR GTPR;  /* Guard time and prescaler register, Address offset: 0x18 */

} sUART1_TypeDef;



#endif /* STM32_F405_UART1_REGISTER_H_ */
