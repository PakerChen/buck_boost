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
#include "OLED.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "hrtim.h"
#include "queue.h"
#include "config.h"
#include <stdio.h>
#include <sys/types.h>
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
	//	OLED_ShowFloatNum(80, 40, Get_ADC_Value(&hadc1)+0.1, 3, 2, OLED_8X16);
   // OLED_ShowNum(80, 40, Get_ADC_Value(&hadc1)+1, 3, OLED_8X16);
    printf("ADC:%d\r\n",(int)(Get_ADC_Value(&hadc1)+1));
    osDelay(20);//delay 200ms
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
  /* Infinite loop */
  for(;;)
  {
		key_scanTask();
    //printf("key 1 cnt = %d",key[1].cnt);
    //printf(" key 1 flag = %d\r\n",key[1].flag);
    HAL_GPIO_TogglePin(user_led_GPIO_Port,user_led_Pin);
    if(key[1].flag == 1)
    {
      page_task(1);
      key[1].flag =0;
    }
    else if (key[1].flag ==0)
    {
      page_task(0);
      key[1].flag =0;
    }
    OLED_ShowNum(80,0,key[0].cnt,3,OLED_8X16);
    OLED_ShowNum(80,20,key[1].cnt,3,OLED_8X16);
   // OLED_ShowNum(80,40,key[2].cnt,3,OLED_8X16);
   // printf("key 2 cnt = %d\r\n ",key[2].cnt);
    osDelay(10);
  }
  /* USER CODE END StartTask02 */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

