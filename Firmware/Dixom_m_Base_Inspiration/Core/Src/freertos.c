/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
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

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "usb_device.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
osThreadId Main_TaskHandle;
osThreadId UART_RESIVEHandle;
osThreadId LED_STROBHandle;
osThreadId SendDataUsbHandle;
osThreadId DisplayHandle;
osThreadId InteractionHandle;
osMessageQId SendUsbHandle;
osMessageQId NewMessageFromUsbHandle;
osSemaphoreId BinarySem_ExchangeI2C2Handle;
osSemaphoreId BinarySem_ExchangeI2C1Handle;
osSemaphoreId BinarySem_ExchangeSPI2Handle;
osSemaphoreId BinarySem_TxUSB_HIDHandle;
osSemaphoreId BinarySem_TxUART1Handle;
osSemaphoreId BinarySem_TxUSB_CDCHandle;
osSemaphoreId BinarySem_DataOutHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void Start_Control_Task(void const * argument);
void Start_UART_RESIVE(void const * argument);
void Start_LED_STROB(void const * argument);
void Start_SendDataUsb(void const * argument);
void Start_Display(void const * argument);
void Start_Interaction(void const * argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* GetIdleTaskMemory prototype (linked to static allocation support) */
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize );

/* USER CODE BEGIN GET_IDLE_TASK_MEMORY */
static StaticTask_t xIdleTaskTCBBuffer;
static StackType_t xIdleStack[configMINIMAL_STACK_SIZE];

void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize )
{
  *ppxIdleTaskTCBBuffer = &xIdleTaskTCBBuffer;
  *ppxIdleTaskStackBuffer = &xIdleStack[0];
  *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
  /* place for user code */
}
/* USER CODE END GET_IDLE_TASK_MEMORY */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* Create the semaphores(s) */
  /* definition and creation of BinarySem_ExchangeI2C2 */
  osSemaphoreDef(BinarySem_ExchangeI2C2);
  BinarySem_ExchangeI2C2Handle = osSemaphoreCreate(osSemaphore(BinarySem_ExchangeI2C2), 1);

  /* definition and creation of BinarySem_ExchangeI2C1 */
  osSemaphoreDef(BinarySem_ExchangeI2C1);
  BinarySem_ExchangeI2C1Handle = osSemaphoreCreate(osSemaphore(BinarySem_ExchangeI2C1), 1);

  /* definition and creation of BinarySem_ExchangeSPI2 */
  osSemaphoreDef(BinarySem_ExchangeSPI2);
  BinarySem_ExchangeSPI2Handle = osSemaphoreCreate(osSemaphore(BinarySem_ExchangeSPI2), 1);

  /* definition and creation of BinarySem_TxUSB_HID */
  osSemaphoreDef(BinarySem_TxUSB_HID);
  BinarySem_TxUSB_HIDHandle = osSemaphoreCreate(osSemaphore(BinarySem_TxUSB_HID), 1);

  /* definition and creation of BinarySem_TxUART1 */
  osSemaphoreDef(BinarySem_TxUART1);
  BinarySem_TxUART1Handle = osSemaphoreCreate(osSemaphore(BinarySem_TxUART1), 1);

  /* definition and creation of BinarySem_TxUSB_CDC */
  osSemaphoreDef(BinarySem_TxUSB_CDC);
  BinarySem_TxUSB_CDCHandle = osSemaphoreCreate(osSemaphore(BinarySem_TxUSB_CDC), 1);

  /* definition and creation of BinarySem_DataOut */
  osSemaphoreDef(BinarySem_DataOut);
  BinarySem_DataOutHandle = osSemaphoreCreate(osSemaphore(BinarySem_DataOut), 1);

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* Create the queue(s) */
  /* definition and creation of SendUsb */
  osMessageQDef(SendUsb, 16, uint16_t);
  SendUsbHandle = osMessageCreate(osMessageQ(SendUsb), NULL);

  /* definition and creation of NewMessageFromUsb */
  osMessageQDef(NewMessageFromUsb, 16, uint16_t);
  NewMessageFromUsbHandle = osMessageCreate(osMessageQ(NewMessageFromUsb), NULL);

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of Main_Task */
  osThreadDef(Main_Task, Start_Control_Task, osPriorityNormal, 0, 640);
  Main_TaskHandle = osThreadCreate(osThread(Main_Task), NULL);

  /* definition and creation of UART_RESIVE */
  osThreadDef(UART_RESIVE, Start_UART_RESIVE, osPriorityIdle, 0, 512);
  UART_RESIVEHandle = osThreadCreate(osThread(UART_RESIVE), NULL);

  /* definition and creation of LED_STROB */
  osThreadDef(LED_STROB, Start_LED_STROB, osPriorityIdle, 0, 512);
  LED_STROBHandle = osThreadCreate(osThread(LED_STROB), NULL);

  /* definition and creation of SendDataUsb */
  osThreadDef(SendDataUsb, Start_SendDataUsb, osPriorityIdle, 0, 640);
  SendDataUsbHandle = osThreadCreate(osThread(SendDataUsb), NULL);

  /* definition and creation of Display */
  osThreadDef(Display, Start_Display, osPriorityIdle, 0, 512);
  DisplayHandle = osThreadCreate(osThread(Display), NULL);

  /* definition and creation of Interaction */
  osThreadDef(Interaction, Start_Interaction, osPriorityIdle, 0, 640);
  InteractionHandle = osThreadCreate(osThread(Interaction), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_Start_Control_Task */
/**
  * @brief  Function implementing the Main_Task thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_Start_Control_Task */
void Start_Control_Task(void const * argument)
{
  /* USER CODE BEGIN Start_Control_Task */
	Task_Main_Setup();
	MX_USB_DEVICE_Init();
  /* Infinite loop */
  for(;;)
  {
	Task_Main_Infinite_Loop();
    osDelay(1);
  }
  /* USER CODE END Start_Control_Task */
}

/* USER CODE BEGIN Header_Start_UART_RESIVE */
/**
* @brief Function implementing the UART_RESIVE thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Start_UART_RESIVE */
void Start_UART_RESIVE(void const * argument)
{
  /* USER CODE BEGIN Start_UART_RESIVE */
  /* Infinite loop */
  for(;;)
  {
    Task_Execution_With_Time_Interval();
    osDelay(1);
  }
  /* USER CODE END Start_UART_RESIVE */
}

/* USER CODE BEGIN Header_Start_LED_STROB */
/**
* @brief Function implementing the LED_STROB thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Start_LED_STROB */
void Start_LED_STROB(void const * argument)
{
  /* USER CODE BEGIN Start_LED_STROB */
  /* Infinite loop */
  for(;;)
  {
	Task_Flashing_LED_Indicator_Infinite_Loop();
    osDelay(1);
  }
  /* USER CODE END Start_LED_STROB */
}

/* USER CODE BEGIN Header_Start_SendDataUsb */
/**
* @brief Function implementing the SendDataUsb thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Start_SendDataUsb */
void Start_SendDataUsb(void const * argument)
{
  /* USER CODE BEGIN Start_SendDataUsb */

  /* Infinite loop */
  for(;;)
  {
	Task_Perifer_Service_Infinite_Loop();
    osDelay(1);
  }
  /* USER CODE END Start_SendDataUsb */
}

/* USER CODE BEGIN Header_Start_Display */
/**
* @brief Function implementing the Display thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Start_Display */
void Start_Display(void const * argument)
{
  /* USER CODE BEGIN Start_Display */
  /* Infinite loop */
  for(;;)
  {
	Task_Display_Infinite_Loop();
    osDelay(1);
  }
  /* USER CODE END Start_Display */
}

/* USER CODE BEGIN Header_Start_Interaction */
/**
* @brief Function implementing the Interaction thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Start_Interaction */
void Start_Interaction(void const * argument)
{
  /* USER CODE BEGIN Start_Interaction */
  /* Infinite loop */
  for(;;)
  {
	Task_Interaction_Infinite_Loop();
    osDelay(1);
  }
  /* USER CODE END Start_Interaction */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */
