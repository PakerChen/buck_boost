#include "OLED.h"
#include "adc.h"
#include "config.h"
#include "main.h"
#include "stm32g474xx.h"
#include "stm32g4xx_hal_hrtim.h"
#include <stdint.h>
#include <sys/types.h>
#include "function.h"
KEY key[3] = {0};
uint8_t buck_state = 0 ;//0为关，1为开
uint8_t boost_state = 0 ;//0为关，1为开
uint8_t select_flag =0;//页面选择标志位
uint8_t ECC11_Statues = 1;//编码器状态标志位
uint32_t duty_value = 10000;//占空比初始值
uint16_t adc1_buf[1]={0};
U1 u1={0};
uint8_t command_TaskFlag = 0;
char text_u1[1024]={0};
float Vin_Value = 0.0f;
void page_task(void)
{
	if(select_flag == 0)//页面标志位
	{
		OLED_Clear();                       // 清除OLED屏显示缓冲区
		OLED_ShowString(0,0,"Wait start...",OLED_8X16);//开机页面
		OLED_ShowString(0,16,"BUCK:",OLED_8X16);//开机页面
		OLED_ShowString(0,32,"BOOST:",OLED_8X16);//开机页面
		if(ECC11_Statues == 1)
		{
			OLED_DrawCircle(100, 25, 4, OLED_FILLED);
			OLED_DrawCircle(100, 40, 4, OLED_UNFILLED);
		}
		else if (ECC11_Statues == 2) 
		{
			OLED_DrawCircle(100, 40, 4, OLED_FILLED);
			OLED_DrawCircle(100, 25, 4, OLED_UNFILLED);
		}
		if(buck_state == 0)
		{
			OLED_ShowString(64,16,"OFF",OLED_8X16);
		}
		else
		{
			OLED_ShowString(64,16,"ON ",OLED_8X16);
		}
		if(boost_state == 0)
		{
			OLED_ShowString(64,32	,"OFF",OLED_8X16);
		}
		else
		{
			OLED_ShowString(64,32,"ON ",OLED_8X16);
		}		
		 OLED_ShowNum(0,50,duty_value,5,OLED_8X16);
	}
	else if (select_flag == 1)
	{	
		OLED_Clear();                       // 清除OLED屏显示缓冲区
		OLED_ShowString(0, 0, "Run page",OLED_8X16);//实际运行页面
		//OLED_ShowFloatNum(0, 25, Get_ADC_Value(&hadc1), 4, 3, OLED_8X16);//显示输入电压
	}
	else if (select_flag == 2)
	{	
		//输入参数查看界面
		OLED_ShowChinese(0, 0, "电压输入"); // 显示中文字
		OLED_ShowChinese(0, 16, "输入电流");
		OLED_ShowChinese(0, 32, "输出电压");
		OLED_ShowChinese(0, 48, "输出电流");
		OLED_ShowChar(64, 0, ':', OLED_8X16);                                                   // 显示冒号
		OLED_ShowChar(64, 16, ':', OLED_8X16);                                                  // 显示冒号
		OLED_ShowChar(64, 32, ':', OLED_8X16);                                                  // 显示冒号
		OLED_ShowChar(64, 48, ':', OLED_8X16);                                                  // 显示冒号
		OLED_ShowChar(72 + 8 * 5, 0, 'V', OLED_8X16);                                           // 显示单位符号
		OLED_ShowChar(72 + 8 * 5, 16, 'A', OLED_8X16);                                          // 显示单位符号
		OLED_ShowChar(72 + 8 * 5, 32, 'V', OLED_8X16);                                          // 显示单位符号
		OLED_ShowChar(72 + 8 * 5, 48, 'A', OLED_8X16);                                          // 显示单位符号
		OLED_ShowFloatNum(65, 0, Vin_Value, 2, 2, OLED_8X16);
		//* REF_3V3 / ADC_MAX_VALUE / 62.0F / 0.005F
	}
	else if (select_flag == 3)
	{
		OLED_Clear(); // 清除OLED屏显示缓冲区
		OLED_ShowChinese(0, 0, "过温保护");
		OLED_ShowChinese(0, 16, "过流保护");
		OLED_ShowChinese(0, 32, "过压保护");
		OLED_ShowChar(64, 0, ':', OLED_8X16);                                                               // 显示冒号
		OLED_ShowChar(64, 16, ':', OLED_8X16);                                                              // 显示冒号
		OLED_ShowChar(64, 32, ':', OLED_8X16);                                                              
	}
	else if (select_flag == 4)
	{
		OLED_Clear();
		//温度显示界面
		OLED_ShowString(0,0,"CPUTEM ",OLED_8X16);
		OLED_ShowString(0,16,"DevTEM ",OLED_8X16);
		OLED_ShowChar(64, 0, ':', OLED_8X16);                                                   // 显示冒号
		OLED_ShowChar(64, 16, ':', OLED_8X16);  
		OLED_ShowFloatNum(65, 0, Get_Cpu_temp(&hadc5), 2, 2, OLED_8X16);                                                // 显示冒号
	}
	OLED_Update(); //每次改完页面都要刷新一下OLED
}

void key_scanTask()
{
	key[0].status = HAL_GPIO_ReadPin(KEY1_GPIO_Port,KEY1_Pin);
	key[1].status = HAL_GPIO_ReadPin(KEY2_GPIO_Port,KEY2_Pin);
	for (int i =0; i<2;i++)
	{
		if (key[i].cnt >=2&&key[i].status == 1)//两次消抖即可
		{
				key[i].flag =1;
		}
		if (key[i].status == 0)
		{
			key[i].cnt++;
			
		}
		else
		{
			key[i].cnt =0;
		}
	}
	key[2].status = HAL_GPIO_ReadPin(Encoder_Key_GPIO_Port,Encoder_Key_Pin);
	if (key[2].cnt >=2&&key[2].status == 1)//两次消抖即可
	{
			key[2].flag =1;
	}
	if (key[2].status == 0)
	{
		key[2].cnt++;
		
	}
	else
	{
		key[2].cnt =0;
	}
}
void key_Task()
{
	//红色按键定义为页面切换
	//EC11定义为开关控制
	key_scanTask();
	if(select_flag ==0)
	{
		//开关控制页面
		if(key[2].flag ==1&&ECC11_Statues ==1)
		{

			buck_state = !buck_state;
			if(buck_state ==1)
			{
				//开启buck
				HAL_HRTIM_WaveformCountStop(&hhrtim1, HRTIM_TIMERID_TIMER_F); //pwm波输出
  				HAL_HRTIM_WaveformOutputStop(&hhrtim1, HRTIM_OUTPUT_TF1 | HRTIM_OUTPUT_TF2); // to generate pwm wave .
			}
			else
			{
				//关闭buck
				HAL_HRTIM_WaveformCountStart(&hhrtim1, HRTIM_TIMERID_TIMER_F); //pwm波输出
				HAL_HRTIM_WaveformOutputStart(&hhrtim1, HRTIM_OUTPUT_TF1 | HRTIM_OUTPUT_TF2); // to generate pwm wave .
			}
			key[2].flag =0;
			page_task();
		}
		else if (key[2].flag ==1&&ECC11_Statues ==2)
		{
			boost_state = !boost_state;
			if(buck_state ==1)
			{
				//开启buck
				HAL_HRTIM_WaveformCountStart(&hhrtim1, HRTIM_TIMERID_TIMER_D); //pwm波输出
				HAL_HRTIM_WaveformOutputStart(&hhrtim1, HRTIM_OUTPUT_TD1 | HRTIM_OUTPUT_TD2); // to generate pwm wave .
			}
			else
			{
				//关闭buck
				HAL_HRTIM_WaveformCountStop(&hhrtim1, HRTIM_TIMERID_TIMER_D); //pwm波输出
  				HAL_HRTIM_WaveformOutputStop(&hhrtim1, HRTIM_OUTPUT_TD1 | HRTIM_OUTPUT_TD2); // to generate pwm wave .
			}
			key[2].flag =0;
			page_task();
		
		}
	}
}
uint8_t rxData;
void Uart2_start()
{
	HAL_UARTEx_ReceiveToIdle_DMA(&huart2,&rxData,1);
}
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
	if(huart->Instance == USART2)
	{
		command_TaskFlag = 1;
		HAL_UARTEx_ReceiveToIdle_DMA(&huart2,(uint8_t*)u1.BUF,sizeof(u1.BUF));
	}
}
void command_Task()
{
	if(command_TaskFlag == 1)
	{
		//HAL_UART_Transmit_DMA(&huart2, u1.BUF, sizeof(u1.BUF));
		sprintf(text_u1,"%s",u1.BUF);
		HAL_UART_Transmit_DMA(&huart2,(uint8_t *)text_u1,strlen(text_u1));
		memset(u1.BUF,0,sizeof(u1.BUF));
		command_TaskFlag = 0;
	}
}
/*
** ===================================================================
**     Funtion Name :void adc_sample(void)
**     Description :处理ADC的数据
**     Parameters  : none
**     Returns     : none
** ===================================================================
*/
void adc_sample(void)
{
	float vin_temp_all = 0.0f;
	for(int i =0;i<10;i++)
	{
		//计算平均值，取平均值
		Vin_Value = Get_ADC_Value(&hadc2) /(4.7F / 75.0F);
		vin_temp_all += Vin_Value;
	}
	Vin_Value = vin_temp_all / 5.0f;
}
/*
** ===================================================================
**     Funtion Name :void VinSwUVP(void)
**     Description :输入软件欠压保护，低压输入保护,可恢复
**     Parameters  : none
**     Returns     : none
** ===================================================================
*/
void VinSwUVP(void)
{
	if(Vin_Value < 4.5F)//输入电压小于某个值就关闭输出
	{
		HAL_HRTIM_WaveformCountStop(&hhrtim1, HRTIM_TIMERID_TIMER_D); //pwm波输出
  		HAL_HRTIM_WaveformOutputStop(&hhrtim1, HRTIM_OUTPUT_TD1 | HRTIM_OUTPUT_TD2); // to generate pwm wave .
		HAL_GPIO_WritePin(buzzer_GPIO_Port,buzzer_Pin,GPIO_PIN_SET);
	}
	else 
	{
		HAL_HRTIM_WaveformCountStart(&hhrtim1, HRTIM_TIMERID_TIMER_D); //pwm波输出
		HAL_HRTIM_WaveformOutputStart(&hhrtim1, HRTIM_OUTPUT_TD1 | HRTIM_OUTPUT_TD2); // to generate pwm wave .
		HAL_GPIO_WritePin(buzzer_GPIO_Port,buzzer_Pin,GPIO_PIN_RESET);
	}
}