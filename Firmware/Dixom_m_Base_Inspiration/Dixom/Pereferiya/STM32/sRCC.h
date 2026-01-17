/*
 * sRCC.h
 *
 *  Created on: 8 дек. 2021 г.
 *      Author: 79825
 */

#ifndef STM32_SRCC_H_
#define STM32_SRCC_H_

#include "stdio.h"
  #define sPWR                 ((sPWR_TypeDef *) 0x40007000)
  #define sRCC                 ((sRCC_TypeDef *) 0x40023800)
  #define sFLASH               ((sFLASH_TypeDef *) 0x40023800)


  typedef struct{
	volatile uint32_t  HSION     :1; //0  Internal high-speed clock enable
	volatile uint32_t  HSIRDY    :1; //1 Internal high-speed clock ready flag
	volatile uint32_t  Reserved1 :1; //2
	volatile uint32_t  HSITRIM   :4; //3-7 Internal high-speed clock trimming
	volatile uint32_t  HSICAL    :7; //8-15 Internal high-speed clock calibration
	volatile uint32_t  HSEON     :1; //16 HSE clock enable
	volatile uint32_t  HSERDY    :1; //17 HSE clock ready flag
	volatile uint32_t  HSEBYP    :1; //18 HSE clock bypass
	volatile uint32_t  CSSON     :1; //19 Clock security system enable
	volatile uint32_t  Reserved2 :4; //20-23
	volatile uint32_t  PLLON     :1; //24 Main PLL (PLL) enable
	volatile uint32_t  PLLRDY    :1; //25 Main PLL (PLL) clock ready flag
	volatile uint32_t  PLLI2SON  :1; //26 PLLI2S enable
	volatile uint32_t  PLLI2SRDY :1; //27 TPLLI2S clock ready flag
	volatile uint32_t  Reserved3 :4; //28-31
  } sRCC_CR;


  typedef struct{
	uint32_t  PLLM      :6; //0-5 Division factor for the main PLL
	uint32_t  PLLN      :9; //6-14 Main PLL (PLL) multiplication factor for VCO
	uint32_t  PLLP      :2; //16-17 Main PLL (PLL) division factor for main system clock
	uint32_t  Reserved1 :4; //18-21
	uint32_t  PLLSRC    :1; //22 Main PLL(PLL) and audio PLL (PLLI2S) entry clock source
	uint32_t  Reserved2 :1; //23
	uint32_t  PLLQ      :4; //24-27 Main PLL (PLL) division factor for USB OTG FS, SDIO and random number generator	clocks
	uint32_t  Reserved3 :4; //23
  } sPLLCFGR;

  typedef struct{
	uint32_t  LSEON     :1; //1 External low-speed oscillator enable
	uint32_t  LSERDY    :1; //2 External low-speed oscillator ready
	uint32_t  LSEBYP    :1; //3 External low-speed oscillator bypass
	uint32_t  Reserved1 :5; //4-7
	uint32_t  RTCSEL    :1; //8-9 RTC clock source selection
	uint32_t  Reserved2 :5; //10-14
	uint32_t  RTCEN     :1; //15 : RTC clock enable
	uint32_t  BDRST     :1; //16 Backup domain software reset
	uint32_t  Reserved3 :15; //10-14
  } sBDCR;

  typedef struct{
	uint32_t  TIM2EN    :1; //0 TIM2 clock enable
	uint32_t  TIM3EN    :1; //1 TIM3 clock enable
	uint32_t  TIM4EN    :1; //2 TIM4 clock enable
	uint32_t  TIM5EN    :1; //3 TIM5 clock enable
	uint32_t  TIM6EN    :1; //4 TIM6 clock enable
	uint32_t  TIM7EN    :1; //5 TIM7 clock enable
	uint32_t  TIM12EN   :1; //6 TIM12 clock enable
	uint32_t  TIM13EN   :1; //7 TIM13 clock enable
	uint32_t  TIM14EN   :1; //8 TIM14 clock enable
	uint32_t  Reserved6 :2; //9-10
	uint32_t  WWDGEN    :1; //11 Window watchdog clock enable
	uint32_t  Reserved5 :2; //12-13
	uint32_t  SPI2EN    :1; //14 SPI2 clock enable
	uint32_t  SPI3EN    :1; //15 SPI3 clock enable
	uint32_t  Reserved4 :1; //16
	uint32_t  USART2EN  :1; //17 USART2 clock enable
	uint32_t  USART3EN  :1; //18 USART3 clock enable
	uint32_t  UART4EN   :1; //19 UART4 clock enable
	uint32_t  UART5EN   :1; //20 UART5 clock enable
	uint32_t  I2C1EN    :1; //21 I2C1 clock enable
	uint32_t  I2C2EN    :1; //22 I2C2 clock enable
	uint32_t  I2C3EN    :1; //23 I2C3 clock enable
	uint32_t  Reserved3 :1; //24
	uint32_t  CAN1EN    :1; //25 CAN 1 clock enable
	uint32_t  CAN2EN    :1; //26 CAN 2 clock enable
	uint32_t  Reserved2 :1; //27
	uint32_t  PWREN     :1; //28 Power interface clock enable
	uint32_t  DACEN     :1; //29 DAC interface clock enable
	uint32_t  Reserved1 :2; //30-31
  } sRCC_APB1ENR;

  typedef struct{
	uint32_t  LSIRDYF    :1; //0 LSI ready interrupt flag
	uint32_t  LSERDYF    :1; //1  LSE ready interrupt flag
	uint32_t  HSIRDYF    :1; //2 HSI ready interrupt flag
	uint32_t  HSERDYF    :1; //3 HSE ready interrupt flag
	uint32_t  PLLRDYF    :1; //4 Main PLL (PLL) ready interrupt flag
	uint32_t  PLLI2SRDYF :1; //5 PLLI2S ready interrupt flag
	uint32_t  Reserved1  :1; //6
	uint32_t  CSSF       :1; //7 Clock security system interrupt flag
	uint32_t  LSIRDYIE   :1; //8 LSI ready interrupt enable
	uint32_t  LSERDYIE   :1; //9 LSE ready interrupt enable
	uint32_t  HSIRDYIE   :1; //10 HSI ready interrupt enable
	uint32_t  HSERDYIE   :1; //11 HSE ready interrupt enable
	uint32_t  PLLRDYIE   :1; //12 Main PLL (PLL) ready interrupt enable
	uint32_t  PLLI2SRDYIE :1; //13 PLLI2S ready interrupt enable
	uint32_t  Reserved2  :2; //14-15
	uint32_t  LSIRDYC    :1; //16 LSI ready interrupt clear
	uint32_t  LSERDYC    :1; //17  LSE ready interrupt clear
	uint32_t  HSIRDYC    :1; //18 HSI ready interrupt clear
	uint32_t  HSERDYC    :1; //19 HSE ready interrupt clear
	uint32_t  PLLRDYC    :1; //20 Main PLL(PLL) ready interrupt clear
	uint32_t  PLLI2SRDYC :1; //21 PLLI2S ready interrupt clear
	uint32_t  Reserved3  :1; //22
	uint32_t  CSSC       :1; //23 Clock security system interrupt clear
	uint32_t  Reserved4  :8; //22
  } sRCC_CIR;

  typedef struct{
	uint32_t  LSION      :1; //0 Internal low-speed oscillator enable
	uint32_t  LSIRDY     :1; //1 Internal low-speed oscillator ready
	uint32_t  Reserved1  :22;//2-23
	uint32_t  RMVF       :1; //24 : Remove reset flag
	uint32_t  BORRSTF    :1; //25 BOR reset flag
	uint32_t  PINRSTF    :1; //26 PIN reset flag
	uint32_t  PORRSTF    :1; //27 POR/PDR reset flag
	uint32_t  SFTRSTF    :1; //28 Software reset flag
	uint32_t  IWDGRSTF   :1; //29 Independent watchdog reset flag
	uint32_t  WWDGRSTF   :1; //30 Window watchdog reset flag
	uint32_t  LPWRRSTF   :1; //31 Low-power reset flag
  } sRCC_CSR;

  typedef struct{
	uint32_t  SW         :2; //0-1 System clock switch
	uint32_t  SWS        :2; //2-3 System clock switch status
	uint32_t  HPRE       :4; //4-7 AHB prescaler
	uint32_t  Reserved1  :2; //8-9
	uint32_t  PPRE1      :3; //10-12 APB Low speed prescaler (APB1) 100 divided 2   101  4
	uint32_t  PPRE2      :3; //13-15 APB high-speed prescaler (APB2)
	uint32_t  RTCPRE     :5; //16-20 HSE division factor for RTC clock
	uint32_t  MCO1       :2; //21-22 Microcontroller clock output 1
	uint32_t  I2SSRC     :1; //23 I2S clock selection
	uint32_t  MCO1PRE    :3; //24-26 MCO1 prescaler
	uint32_t  MCO2PRE    :3; //27-29 MCO2 prescaler
	uint32_t  MCO2       :2; //30-31 Microcontroller clock output 2
  } sRCC_CFGR;




  typedef struct
  {
    volatile sRCC_CR  CR;            /*!< RCC clock control register,                                  Address offset: 0x00 */
    volatile sPLLCFGR PLLCFGR;       /*!< RCC PLL configuration register,                              Address offset: 0x04 */
    volatile sRCC_CFGR CFGR;          /*!< RCC clock configuration register,                            Address offset: 0x08 */
    volatile sRCC_CIR CIR;           /*!< RCC clock interrupt register,                                Address offset: 0x0C */
    volatile uint32_t AHB1RSTR;      /*!< RCC AHB1 peripheral reset register,                          Address offset: 0x10 */
    volatile uint32_t AHB2RSTR;      /*!< RCC AHB2 peripheral reset register,                          Address offset: 0x14 */
    volatile uint32_t AHB3RSTR;      /*!< RCC AHB3 peripheral reset register,                          Address offset: 0x18 */
    uint32_t          RESERVED0;     /*!< Reserved, 0x1C                                                                    */
    volatile uint32_t APB1RSTR;      /*!< RCC APB1 peripheral reset register,                          Address offset: 0x20 */
    volatile uint32_t APB2RSTR;      /*!< RCC APB2 peripheral reset register,                          Address offset: 0x24 */
    uint32_t          RESERVED1[2];  /*!< Reserved, 0x28-0x2C                                                               */
    volatile uint32_t AHB1ENR;       /*!< RCC AHB1 peripheral clock register,                          Address offset: 0x30 */
    volatile uint32_t AHB2ENR;       /*!< RCC AHB2 peripheral clock register,                          Address offset: 0x34 */
    volatile uint32_t AHB3ENR;       /*!< RCC AHB3 peripheral clock register,                          Address offset: 0x38 */
    uint32_t          RESERVED2;     /*!< Reserved, 0x3C                                                                    */
    volatile sRCC_APB1ENR APB1ENR;       /*!< RCC APB1 peripheral clock enable register,                   Address offset: 0x40 */
    volatile uint32_t APB2ENR;       /*!< RCC APB2 peripheral clock enable register,                   Address offset: 0x44 */
    uint32_t          RESERVED3[2];  /*!< Reserved, 0x48-0x4C                                                               */
    volatile uint32_t AHB1LPENR;     /*!< RCC AHB1 peripheral clock enable in low power mode register, Address offset: 0x50 */
    volatile uint32_t AHB2LPENR;     /*!< RCC AHB2 peripheral clock enable in low power mode register, Address offset: 0x54 */
    volatile uint32_t AHB3LPENR;     /*!< RCC AHB3 peripheral clock enable in low power mode register, Address offset: 0x58 */
    uint32_t          RESERVED4;     /*!< Reserved, 0x5C                                                                    */
    volatile uint32_t APB1LPENR;     /*!< RCC APB1 peripheral clock enable in low power mode register, Address offset: 0x60 */
    volatile uint32_t APB2LPENR;     /*!< RCC APB2 peripheral clock enable in low power mode register, Address offset: 0x64 */
    uint32_t          RESERVED5[2];  /*!< Reserved, 0x68-0x6C                                                               */
    volatile sBDCR    BDCR;          /*!< RCC Backup domain control register,                          Address offset: 0x70 */
    volatile sRCC_CSR CSR;           /*!< RCC clock control & status register,                         Address offset: 0x74 */
    uint32_t          RESERVED6[2];  /*!< Reserved, 0x78-0x7C                                                               */
    volatile uint32_t SSCGR;         /*!< RCC spread spectrum clock generation register,               Address offset: 0x80 */
    volatile uint32_t PLLI2SCFGR;    /*!< RCC PLLI2S configuration register,                           Address offset: 0x84 */
  } sRCC_TypeDef;











  typedef struct{
	uint32_t  LPDS      :1; //0 Low-power deepsleep
	uint32_t  PDDS      :1; //1 Power-down deepsleep
	uint32_t  CWUF      :1; //2 Clear wakeup flag
	uint32_t  CSBF      :1; //3 Clear standby flag
	uint32_t  PVDE      :1; //4 Power voltage detector enable
	uint32_t  PLS       :3; //5-7 PVD level selection
	uint32_t  DBP       :1; //8 Disable backup domain write protection
	uint32_t  FPDS      :1; //9 Flash power-down in Stop mode
	uint32_t  Reserved1 :4; //1-13
	uint32_t  VOS       :1; //14 Low-power regulator in deepsleep under-drive mode
	uint32_t  Reserved2 :17;//15-31
  } sPWR_CR;

  typedef struct
  {
	  volatile sPWR_CR CR;   /*!< PWR power control register,        Address offset: 0x00 */
    __IO uint32_t CSR;  /*!< PWR power control/status register, Address offset: 0x04 */
  } sPWR_TypeDef;




  typedef struct{
	uint32_t  LATENCY    :3; //0-2 Latency
	uint32_t  Reserved1  :5; //3-7
	uint32_t  PRFTEN     :1; //8 Prefetch enable
	uint32_t  ICEN       :1; //9 Instruction cache enable
	uint32_t  DCEN       :1; //10 Data cache enable
	uint32_t  ICRST      :1; //11 Instruction cache reset
	uint32_t  DCRST      :1; //12 Data cache reset
	uint32_t  Reserved2  :19; //13-31
  } sFLASH_ACR;



  typedef struct
  {
	volatile sFLASH_ACR ACR;      /*!< FLASH access control register,   Address offset: 0x00 */
    volatile uint32_t KEYR;     /*!< FLASH key register,              Address offset: 0x04 */
    volatile uint32_t OPTKEYR;  /*!< FLASH option key register,       Address offset: 0x08 */
    volatile uint32_t SR;       /*!< FLASH status register,           Address offset: 0x0C */
    volatile uint32_t CR;       /*!< FLASH control register,          Address offset: 0x10 */
    volatile uint32_t OPTCR;    /*!< FLASH option control register ,  Address offset: 0x14 */
    volatile uint32_t OPTCR1;   /*!< FLASH option control register 1, Address offset: 0x18 */
  } sFLASH_TypeDef;


#endif /* STM32_SRCC_H_ */
