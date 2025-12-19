#include "config.h"
#include "main.h"
#include <stdint.h>
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
	else if (select_flag == 2)
	{
		//进入第二个页面
		OLED_ShowString(0, 0, "Run page",OLED_8X16);
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
uint8_t Encoder_Scanf(void)
{
    static  uint8_t    EC11_CLK_Last= 0;    //EC11的LCK引脚上一次的状态 （A相）
    static  uint8_t    EC11_DT_Last = 0;     //EC11的DT引脚上一次的状态（B相）
   	static  uint8_t ScanResult = 0;
    //当A发生跳变时采集B当前的状态，并将B与上一次的状态进行对比。
    if(GET_CLK_STATE !=EC11_CLK_Last)
    {           //若A 0->1 时，B 1->0 正转；若A 1->0 时，B 0->1 正转；
                //若A 0->1 时，B 0->1 反转；若A 1->0 时，B 1->0 反转
        if(GET_CLK_STATE == 1)     //EC11_A和上一次状态相比，为上升沿
        {
            //EC11_B和上一次状态相比，为下降沿
            if((EC11_DT_Last == 1)&&(GET_DT_STATE == 0))
                ScanResult = 1;  //正转
             //EC11_B和上一次状态相比，为上升沿
            if((EC11_DT_Last == 0)&&(GET_DT_STATE == 1))
                ScanResult = 2; //反转

            //>>>>>>>>>>>>>>>>下面为正转一次再反转或反转一次再正转处理<<<<<<<<<<<<<<<<//
            //A上升沿时，采集的B不变且为0
            if((EC11_DT_Last == GET_DT_STATE)&&(GET_DT_STATE == 0))
                ScanResult = 1;                                 //正转
             //A上升沿时，采集的B不变且为1
            if((EC11_DT_Last == GET_DT_STATE)&&(GET_DT_STATE == 1))
                ScanResult = 2;                                //反转
        }
        else  //EC11_A和上一次状态相比，为下降沿
        {
            //EC11_B和上一次状态相比，为下降沿
            if((EC11_DT_Last == 1)&&(GET_DT_STATE == 0))
                ScanResult = 2;                        //反转
             //EC11_B和上一次状态相比，为上升沿
            if((EC11_DT_Last == 0)&&(GET_DT_STATE == 1))
                ScanResult = 1;                         //正转

            //>>>>>>>>>>>>>>>>下面为正转一次再反转或反转一次再正转处理<<<<<<<<<<<<<<<<//
            //A上升沿时，采集的B不变且为0
            if((EC11_DT_Last == GET_DT_STATE)&&(GET_DT_STATE == 0))
                ScanResult = 2;                                //反转
            //A上升沿时，采集的B不变且为1
            if((EC11_DT_Last == GET_DT_STATE)&&(GET_DT_STATE == 1))
                ScanResult = 1;                                 //正转
        }
        EC11_CLK_Last = GET_CLK_STATE;   //更新编码器上一个状态暂存变量
        EC11_DT_Last = GET_DT_STATE;     //更新编码器上一个状态暂存变量
        return ScanResult;               //返回值的取值：   0：无动作； 1：正转；  2：反转；
    }
    return 0;
}

