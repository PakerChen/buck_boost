#include "config.h"
#include "main.h"
#include <sys/types.h>
#include "function.h"
KEY key[3] = {0};
void page_task(uint8_t select_flag)
{
	if(select_flag == 0)//页面标志位
	{
		OLED_ShowString(0,0,"Wait start...",OLED_8X16);
	}
	else if (select_flag == 1)
	{
		//进入第一个页面
		OLED_ShowString(0, 0, "Set page",OLED_8X16);
	}
	OLED_Update(); //每次改完页面都要刷新一下OLED
}

void key_scanTask()
{
	key[0].status = HAL_GPIO_ReadPin(KEY1_GPIO_Port,KEY1_Pin);
	key[1].status = HAL_GPIO_ReadPin(KEY2_GPIO_Port,KEY2_Pin);
	
	for (int i =0; i<2;i++)
	{
		if (key[i].status == 0)
		{
			key[i].cnt++;
			if (key[i].cnt >=2)//两次消抖即可
			{
				key[i].cnt =2;
				key[i].flag =1;
			}
		}
		else
		{
			key[i].cnt =0;
		}
	}
	key[2].status = HAL_GPIO_ReadPin(Encoder_Key_GPIO_Port,Encoder_Key_Pin);
	if( key[2].status == 0)
	{
		key[2].cnt++;
		if (key[2].cnt >=2)
		{
			key[2].cnt =2;
			key[2].flag =1;
		}
	}
	else
	{
		key[2].cnt =0;
	}
}

