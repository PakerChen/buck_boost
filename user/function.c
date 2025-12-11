#include "function.h"



void page_task(uint8_t select_flag)
{
	if(select_flag == 0)//开机默认页面
	{
		OLED_ShowString(0,0,"Wait start...",OLED_8X16);
	}
	
	OLED_Update(); // 刷新屏幕显示
	
	
}