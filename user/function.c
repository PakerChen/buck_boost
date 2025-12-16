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
	key[2].status = HAL_GPIO_ReadPin(EC11_Button_GPIO_Port,EC11_Button_Pin);
	for (int i =0; i<3;i++)
	{
		if (key[i].status == 0)
		{
			key[i].cnt++;
			if (key[i].cnt >=2)
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
}

