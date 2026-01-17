/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define VDD_Remot_Pin GPIO_PIN_13
#define VDD_Remot_GPIO_Port GPIOC
#define Rear_Detected_Pin GPIO_PIN_14
#define Rear_Detected_GPIO_Port GPIOC
#define VDD_Camera_Pin GPIO_PIN_15
#define VDD_Camera_GPIO_Port GPIOC
#define ADC_CH1_Pin GPIO_PIN_0
#define ADC_CH1_GPIO_Port GPIOC
#define ADC_CH2_Pin GPIO_PIN_1
#define ADC_CH2_GPIO_Port GPIOC
#define GND_Triger_CH1_Pin GPIO_PIN_2
#define GND_Triger_CH1_GPIO_Port GPIOC
#define GND_Triger_CH2_Pin GPIO_PIN_3
#define GND_Triger_CH2_GPIO_Port GPIOC
#define WakeUP_Pin GPIO_PIN_0
#define WakeUP_GPIO_Port GPIOA
#define ADC_CH4_Pin GPIO_PIN_1
#define ADC_CH4_GPIO_Port GPIOA
#define ADC_CH3_Pin GPIO_PIN_4
#define ADC_CH3_GPIO_Port GPIOA
#define VDD_CINV_Pin GPIO_PIN_5
#define VDD_CINV_GPIO_Port GPIOA
#define ADC_LIGHT_Pin GPIO_PIN_6
#define ADC_LIGHT_GPIO_Port GPIOA
#define VDD_5V_USB1_Pin GPIO_PIN_7
#define VDD_5V_USB1_GPIO_Port GPIOA
#define ADC_ACC_Pin GPIO_PIN_4
#define ADC_ACC_GPIO_Port GPIOC
#define VDD_5V_USB2_Pin GPIO_PIN_5
#define VDD_5V_USB2_GPIO_Port GPIOC
#define VDD_5V_Vbus_Pin GPIO_PIN_0
#define VDD_5V_Vbus_GPIO_Port GPIOB
#define DC_DC_Power_Pin GPIO_PIN_1
#define DC_DC_Power_GPIO_Port GPIOB
#define LED_Pin GPIO_PIN_2
#define LED_GPIO_Port GPIOB
#define SPI_CS_EEPROM_Pin GPIO_PIN_12
#define SPI_CS_EEPROM_GPIO_Port GPIOB
#define ENCODER1_CHB_Pin GPIO_PIN_6
#define ENCODER1_CHB_GPIO_Port GPIOC
#define ENCODER1_CHA_Pin GPIO_PIN_7
#define ENCODER1_CHA_GPIO_Port GPIOC
#define SPI_CS_DSP_Pin GPIO_PIN_8
#define SPI_CS_DSP_GPIO_Port GPIOC
#define GND_Triger_CH3_Pin GPIO_PIN_9
#define GND_Triger_CH3_GPIO_Port GPIOC
#define Modules_Reset_Pin GPIO_PIN_8
#define Modules_Reset_GPIO_Port GPIOA
#define OTG_Pin GPIO_PIN_15
#define OTG_GPIO_Port GPIOA
#define DAC_RESET_Pin GPIO_PIN_3
#define DAC_RESET_GPIO_Port GPIOB
#define ENCODER2_CHB_Pin GPIO_PIN_4
#define ENCODER2_CHB_GPIO_Port GPIOB
#define ENCODER2_CHA_Pin GPIO_PIN_5
#define ENCODER2_CHA_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
