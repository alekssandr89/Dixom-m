/*
 * sRCC.h
 *
 *  Created on: 8 пїЅпїЅпїЅ. 2021 пїЅ.
 *      Author: 79825
 */

#ifndef STM32_F405_DMA_REGISTER_H_
#define STM32_F405_DMA_REGISTER_H_

#include "stdint.h"

#define rDMA1 ((sDMA_TypeDef *) DMA1_BASE)
#define rDMA2 ((sDMA_TypeDef *) DMA2_BASE)

typedef volatile struct{
	uint32_t  FEIF0       :1;  //0
	uint32_t  Reserved1   :1;  //1
	uint32_t  DMEIF0      :1;  //2
	uint32_t  TEIF0       :1;  //3
	uint32_t  HTIF0       :1;  //4
	uint32_t  TCIF0       :1;  //5
	uint32_t  FEIF1       :1;  //6
	uint32_t  Reserved2   :1;  //7
	uint32_t  DMEIF1      :1;  //8
	uint32_t  TEIF1       :1;  //9
	uint32_t  HTIF1       :1;  //10
	uint32_t  TCIF1       :1;  //11
	uint32_t  Reserved3   :4;  //12-15
	uint32_t  FEIF2       :1;  //16
	uint32_t  Reserved4   :1;  //17
	uint32_t  DMEIF2      :1;  //18
	uint32_t  TEIF2       :1;  //19
	uint32_t  HTIF2       :1;  //20
	uint32_t  TCIF2       :1;  //21
	uint32_t  FEIF3       :1;  //22
	uint32_t  Reserved5   :1;  //23
	uint32_t  DMEIF3      :1;  //24
	uint32_t  TEIF3       :1;  //25
	uint32_t  HTIF3       :1;  //26
	uint32_t  TCIF3       :1;  //27
	uint32_t  Reserved6   :4;  //28-31
} sDMA_LISR;

typedef volatile struct{
	uint32_t  FEIF4       :1;  //0
	uint32_t  Reserved1   :1;  //1
	uint32_t  DMEIF04     :1;  //2
	uint32_t  TEIF4       :1;  //3
	uint32_t  HTIF4       :1;  //4
	uint32_t  TCIF4       :1;  //5
	uint32_t  FEIF5       :1;  //6
	uint32_t  Reserved2   :1;  //7
	uint32_t  DMEIF5      :1;  //8
	uint32_t  TEIF5       :1;  //9
	uint32_t  HTIF5       :1;  //10
	uint32_t  TCIF5       :1;  //11
	uint32_t  Reserved3   :4;  //12-15
	uint32_t  FEIF6       :1;  //16
	uint32_t  Reserved4   :1;  //17
	uint32_t  DMEIF6      :1;  //18
	uint32_t  TEIF6       :1;  //19
	uint32_t  HTIF6       :1;  //20
	uint32_t  TCIF6       :1;  //21
	uint32_t  FEIF7       :1;  //22
	uint32_t  Reserved5   :1;  //23
	uint32_t  DMEIF7      :1;  //24
	uint32_t  TEIF7       :1;  //25
	uint32_t  HTIF7       :1;  //26
	uint32_t  TCIF7       :1;  //27
	uint32_t  Reserved6   :4;  //28-31
} sDMA_HISR;

typedef volatile struct{
	uint32_t  CFEIF0      :1;  //0
	uint32_t  Reserved1   :1;  //1
	uint32_t  CDMEIF0     :1;  //2
	uint32_t  CTEIF0      :1;  //3
	uint32_t  CHTIF0      :1;  //4
	uint32_t  CTCIF0      :1;  //5
	uint32_t  CFEIF1      :1;  //6
	uint32_t  Reserved2   :1;  //7
	uint32_t  CDMEIF1     :1;  //8
	uint32_t  CTEIF1      :1;  //9
	uint32_t  CHTIF1      :1;  //10
	uint32_t  CTCIF1      :1;  //11
	uint32_t  Reserved3   :4;  //12-15
	uint32_t  CFEIF2      :1;  //16
	uint32_t  Reserved4   :1;  //17
	uint32_t  CDMEIF2     :1;  //18
	uint32_t  CTEIF2      :1;  //19
	uint32_t  CHTIF2      :1;  //20
	uint32_t  CTCIF2      :1;  //21
	uint32_t  CFEIF3      :1;  //22
	uint32_t  Reserved5   :1;  //23
	uint32_t  CDMEIF3     :1;  //24
	uint32_t  CTEIF3      :1;  //25
	uint32_t  CHTIF3      :1;  //26
	uint32_t  CTCIF3      :1;  //27
	uint32_t  Reserved6   :4;  //28-31
} sDMA_LIFCR;

typedef volatile struct{
	uint32_t  CFEIF4      :1;  //0
	uint32_t  Reserved1   :1;  //1
	uint32_t  CDMEIF4     :1;  //2
	uint32_t  CTEIF4      :1;  //3
	uint32_t  CHTIF4      :1;  //4
	uint32_t  CTCIF4      :1;  //5
	uint32_t  CFEIF5      :1;  //6
	uint32_t  Reserved2   :1;  //7
	uint32_t  CDMEIF5     :1;  //8
	uint32_t  CTEIF5      :1;  //9
	uint32_t  CHTIF5      :1;  //10
	uint32_t  CTCIF5      :1;  //11
	uint32_t  Reserved3   :4;  //12-15
	uint32_t  CFEIF6      :1;  //16
	uint32_t  Reserved4   :1;  //17
	uint32_t  CDMEIF6     :1;  //18
	uint32_t  CTEIF6      :1;  //19
	uint32_t  CHTIF6      :1;  //20
	uint32_t  CTCIF6      :1;  //21
	uint32_t  CFEIF7      :1;  //22
	uint32_t  Reserved5   :1;  //23
	uint32_t  CDMEIF7     :1;  //24
	uint32_t  CTEIF7      :1;  //25
	uint32_t  CHTIF7      :1;  //26
	uint32_t  CTCIF7      :1;  //27
	uint32_t  Reserved6   :4;  //28-31
} sDMA_HIFCR;

typedef volatile struct{
	uint32_t  EN          :1;  //0
	uint32_t  DMEIE       :1;  //1
	uint32_t  TEIE        :1;  //2
	uint32_t  HTIE        :1;  //3
	uint32_t  TCIE        :1;  //4
	uint32_t  PFCTRL      :1;  //5
	uint32_t  DIR         :2;  //6-7
	uint32_t  CIRC        :1;  //8
	uint32_t  PINC        :1;  //9
	uint32_t  MINC        :1;  //10
	uint32_t  PSIZE       :2;  //11-12
	uint32_t  MSIZE       :2;  //13-14
	uint32_t  PINCOS      :1;  //15
	uint32_t  PL          :2;  //16-17
	uint32_t  DBM         :1;  //18
	uint32_t  CT          :1;  //19
	uint32_t  Reserved1   :1;  //20
	uint32_t  PBURST      :2;  //21-22
	uint32_t  MBURST      :2;  //23-24
	uint32_t  CHSEL       :3;  //25-27
	uint32_t  Reserved2   :4;  //28-31
} sDMA_CR;

typedef volatile struct{
	uint32_t  NDT         :16;  //0-15
	uint32_t  Reserved1   :16;  //15-31
} sDMA_NDTR;

typedef volatile struct{
	uint32_t  FTH         :2;  //0-1
	uint32_t  DMDIS       :1;  //2
	uint32_t  FS          :3;  //3-5
	uint32_t  Reserved1   :1;  //6
	uint32_t  FEIE        :1;  //7
	uint32_t  Reserved2   :24; //8-31
} sDMA_FCR;

typedef volatile struct{
	sDMA_CR   CR;        /* DMA stream x configuration register,      Address offset: 0x10 + 0x18 × stream number */
	sDMA_NDTR NDTR;      /* DMA stream x number of data register,     Address offset: 0x14 + 0x18 × stream number */
	uint32_t  PAR;       /* DMA stream x peripheral address register, Address offset: 0x18 + 0x18 × stream number */
	uint32_t  M0AR;      /* DMA stream x memory 0 address register,   Address offset: 0x1C + 0x18 × stream number */
	uint32_t  M1AR;      /* DMA stream x memory 1 address register,   Address offset: 0x20 + 0x18 × stream number */
	sDMA_FCR  FCR;       /* DMA stream x FIFO control register,       Address offset: 0x24 + 0x18 × stream number */
} sDMA_Stream;

typedef volatile struct
{
  sDMA_LISR   LISR;        /* DMA low interrupt status register,        Address offset: 0x00 */
  sDMA_HISR   HISR;        /* DMA high interrupt status register,       Address offset: 0x04 */
  sDMA_LIFCR  LIFCR;       /* DMA low interrupt flag clear register,    Address offset: 0x08 */
  sDMA_HIFCR  HIFCR;       /* DMA high interrupt flag clear register,   Address offset: 0x0C */
  sDMA_Stream Stream0;     /* DMA stream number 1*/
  sDMA_Stream Stream1;     /* DMA stream number 1*/
  sDMA_Stream Stream2;     /* DMA stream number 1*/
  sDMA_Stream Stream3;     /* DMA stream number 1*/
  sDMA_Stream Stream4;     /* DMA stream number 1*/
  sDMA_Stream Stream5;     /* DMA stream number 1*/
  sDMA_Stream Stream6;     /* DMA stream number 1*/
  sDMA_Stream Stream7;     /* DMA stream number 1*/
} sDMA_TypeDef;



#endif /* STM32_F405_DMA_REGISTER_H_ */
