#ifndef __function_h_
#define __function_h_

//task
#include <stdint.h>
#include <sys/types.h>
void page_task(void);

 typedef struct
 {
    uint8_t cnt;
    uint8_t status;
    uint8_t flag;
 }KEY;
typedef struct
{	
	char BUF[1024];
	char DATA;
	uint32_t i;
}U1;
extern U1 u1;

extern KEY key[3];
extern uint8_t select_flag;
void key_Task();
extern uint8_t ECC11_Statues ;
extern uint32_t duty_value;
extern uint16_t adc1_buf[1];
void key_scanTask();
void command_Task();
#endif
