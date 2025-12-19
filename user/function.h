#ifndef __function_h_
#define __function_h_

//task
#include <stdint.h>
#include <sys/types.h>
void page_task(uint8_t select_flag);

 typedef struct
 {
    uint8_t cnt;
    uint8_t status;
    uint8_t flag;
 }KEY;

extern KEY key[3];

void key_scanTask();
uint8_t Encoder_Scanf(void);
//获取CLK引脚的状态
#define GET_CLK_STATE  HAL_GPIO_ReadPin(Encoder_A_GPIO_Port,Encoder_A_Pin)
//获取DT引脚的状态
#define GET_DT_STATE    HAL_GPIO_ReadPin(Encoder_B_GPIO_Port,Encoder_B_Pin)
#endif
