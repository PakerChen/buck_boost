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
<<<<<<< HEAD
#include "config.h"
#include <stdio.h>
#include <sys/types.h>
=======
#include "usart.h"
#include "stdio.h"
>>>>>>> e6fe05ce2e12f979570d41f9e36a3280f7a708d1
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
  .priority = (osPriority_t) osPriorityNormal,
  .stack_size = 128 * 4
};
/* Definitions for KEY_Task */
osThreadId_t KEY_TaskHandle;
const osThreadAttr_t KEY_Task_attributes = {
  .name = "KEY_Task",
  .priority = (osPriority_t) osPriorityLow,
  .stack_size = 128 * 4
};
/* Definitions for UART_TX_TASK */
osThreadId_t UART_TX_TASKHandle;
const osThreadAttr_t UART_TX_TASK_attributes = {
  .name = "UART_TX_TASK",
  .priority = (osPriority_t) osPriorityLow,
  .stack_size = 128 * 4
};
/* Definitions for UART_RX_TASK */
osThreadId_t UART_RX_TASKHandle;
const osThreadAttr_t UART_RX_TASK_attributes = {
  .name = "UART_RX_TASK",
  .priority = (osPriority_t) osPriorityLow,
  .stack_size = 128 * 4
};
/* Definitions for myQueue01 */
osMessageQueueId_t myQueue01Handle;
const osMessageQueueAttr_t myQueue01_attributes = {
  .name = "myQueue01"
};
/* Definitions for myQueue02 */
osMessageQueueId_t myQueue02Handle;
const osMessageQueueAttr_t myQueue02_attributes = {
  .name = "myQueue02"
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
	xQueueHandle  DataQueue;
	
<<<<<<< HEAD
	int _write(int fd, char *ptr, int len)
  { 
    HAL_UART_Transmit(&huart2, (uint8_t*)ptr, len, 0xFFFF);
    return len;
  }

=======
	int fputc(int ch,FILE *f)
	{
		HAL_UART_Transmit(&huart2,(uint8_t *)&ch,1,1);
		return ch;
	}
>>>>>>> e6fe05ce2e12f979570d41f9e36a3280f7a708d1
/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);
void StartTask02(void *argument);
void StartTask03(void *argument);
void StartTask04(void *argument);

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

  /* Create the queue(s) */
  /* creation of myQueue01 */
  myQueue01Handle = osMessageQueueNew (16, sizeof(uint16_t), &myQueue01_attributes);

  /* creation of myQueue02 */
  myQueue02Handle = osMessageQueueNew (16, sizeof(uint16_t), &myQueue02_attributes);

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
	
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of Sys_LED_Task */
  Sys_LED_TaskHandle = osThreadNew(StartDefaultTask, NULL, &Sys_LED_Task_attributes);

  /* creation of KEY_Task */
  KEY_TaskHandle = osThreadNew(StartTask02, NULL, &KEY_Task_attributes);

  /* creation of UART_TX_TASK */
  UART_TX_TASKHandle = osThreadNew(StartTask03, NULL, &UART_TX_TASK_attributes);

  /* creation of UART_RX_TASK */
  UART_RX_TASKHandle = osThreadNew(StartTask04, NULL, &UART_RX_TASK_attributes);

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
<<<<<<< HEAD
		
=======
		HAL_GPIO_TogglePin(user_led_GPIO_Port,user_led_Pin);
>>>>>>> e6fe05ce2e12f979570d41f9e36a3280f7a708d1
    osDelay(200);//delay 200ms
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
<<<<<<< HEAD
  for(;;)
  {
		key_scanTask();
    printf("key 1 cnt = %d",key[1].cnt);
    printf(" key 1 flag = %d\r\n",key[1].flag);
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
    osDelay(100);
=======
	static uint16_t duty_val = 2000;
	static uint8_t cnt =0;
  for(;;)
  {
		if(HAL_GPIO_ReadPin(KEY2_GPIO_Port,KEY2_Pin)==1)
		{
				HAL_GPIO_WritePin(LED_G_GPIO_Port,LED_G_Pin,GPIO_PIN_RESET);
				HAL_GPIO_WritePin(LED_R_GPIO_Port,LED_R_Pin,GPIO_PIN_RESET);
				//HAL_GPIO_WritePin(buzzer_GPIO_Port,buzzer_Pin,GPIO_PIN_RESET);
				//osThreadResume(Sys_LED_TaskHandle);this function is aim to make some task to suspend state
		}
		else if (HAL_GPIO_ReadPin(KEY2_GPIO_Port,KEY2_Pin)==0)
		{
				HAL_GPIO_WritePin(LED_G_GPIO_Port,LED_G_Pin,GPIO_PIN_SET);
				HAL_GPIO_WritePin(LED_R_GPIO_Port,LED_R_Pin,GPIO_PIN_SET);
			
				//HAL_GPIO_WritePin(buzzer_GPIO_Port,buzzer_Pin,GPIO_PIN_SET);
				//osThreadSuspend(Sys_LED_TaskHandle);//this function is aim to make some task to blocked state
			
				//this  function is aim to change duty of pwm
				cnt++;
			if(cnt>=2)
			{
				duty_val+=200;
				
				__HAL_HRTIM_SETCOMPARE(&hhrtim1, HRTIM_TIMERINDEX_TIMER_D, HRTIM_COMPAREUNIT_1, duty_val); 
			}
		}
		
    osDelay(1);
>>>>>>> e6fe05ce2e12f979570d41f9e36a3280f7a708d1
  }
  /* USER CODE END StartTask02 */
}

/* USER CODE BEGIN Header_StartTask03 */
/**
* @brief Function implementing the UART_TX_TASK thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTask03 */
void StartTask03(void *argument)
{
  /* USER CODE BEGIN StartTask03 */
		uint8_t buf[] = "this is test";
  /* Infinite loop */
  for(;;)
  {
		//TX_Function
		xQueueSend(DataQueue,buf,10);
    osDelay(50);
  }
  /* USER CODE END StartTask03 */
}

/* USER CODE BEGIN Header_StartTask04 */
/**
* @brief Function implementing the UART_RX_TASK thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTask04 */
void StartTask04(void *argument)
{
  /* USER CODE BEGIN StartTask04 */
	uint8_t rece_buf[9];
  /* Infinite loop */
  for(;;)
  {
		if(pdPASS == xQueueReceive(DataQueue,&rece_buf,10))
		{	
			for(int i = 0;i<9;i++)
				printf("%c", rece_buf[i]);
		}
		else 
			printf("advent error");
		printf("\r\n");
    osDelay(50);
  }
  /* USER CODE END StartTask04 */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

