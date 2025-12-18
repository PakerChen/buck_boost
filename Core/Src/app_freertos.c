/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : app_freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
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
#include "hrtim.h"
#include "queue.h"
#include "config.h"
#include <stdint.h>
#include <stdio.h>
#include <sys/types.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
uint32_t encoder_flag = 0;
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
/* Definitions for Sys_LED_Task */
osThreadId_t Sys_LED_TaskHandle;
const osThreadAttr_t Sys_LED_Task_attributes = {
  .name = "Sys_LED_Task",
  .priority = (osPriority_t) osPriorityLow,
  .stack_size = 128 * 4
};
/* Definitions for KEY_Task */
osThreadId_t KEY_TaskHandle;
const osThreadAttr_t KEY_Task_attributes = {
  .name = "KEY_Task",
  .priority = (osPriority_t) osPriorityHigh,
  .stack_size = 128 * 4
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
	xQueueHandle  DataQueue;
	
	int _write(int fd, char *ptr, int len)
  { 
    HAL_UART_Transmit(&huart2, (uint8_t*)ptr, len, 0xFFFF);
    return len;
  }

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);
void StartTask02(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */
	DataQueue = xQueueCreate(2,9);
  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */

  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
	
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of Sys_LED_Task */
  Sys_LED_TaskHandle = osThreadNew(StartDefaultTask, NULL, &Sys_LED_Task_attributes);

  /* creation of KEY_Task */
  KEY_TaskHandle = osThreadNew(StartTask02, NULL, &KEY_Task_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */
  for(;;)
  {
		//System LED ,When you run this code ,the light will sprinkle.
     HAL_GPIO_TogglePin(user_led_GPIO_Port,user_led_Pin);
    osDelay(100);//delay 200ms
  }
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_StartTask02 */
/**
* @brief Function implementing the OLED_Task thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTask02 */
void StartTask02(void *argument)
{
  /* USER CODE BEGIN StartTask02 */
  static uint8_t page_select_flag =0;
  /* Infinite loop */
  for(;;)
  {
		key_scanTask();
    if(key[1].flag == 1)
    {
      page_select_flag+=1;
      if (page_select_flag >2)
      {
        page_select_flag =0;
      }
      page_task(page_select_flag);
      key[1].flag =0;
    }
    if(encoder_flag == 1)
    {
      osDelay(1);
       if (HAL_GPIO_ReadPin(Encoder_A_GPIO_Port, Encoder_A_Pin) == 0)
       {
         if (HAL_GPIO_ReadPin(Encoder_B_GPIO_Port, Encoder_B_Pin) == 1)
         {
          printf("this is right\n");
         }
         else if (HAL_GPIO_ReadPin(Encoder_B_GPIO_Port, Encoder_B_Pin) == 0)
         {
          printf("this is left\n");
         }
       }
      encoder_flag = 0;
    }
    osDelay(50);
  }
  /* USER CODE END StartTask02 */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  if(GPIO_Pin == Encoder_A_Pin)
  {
     encoder_flag = 1;
  }
}
/* USER CODE END Application */

