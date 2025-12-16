#ifndef __function_h_
#define __function_h_

<<<<<<< HEAD
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
=======
#include "gpio.h"





#include "stdio.h"
#include "stdlib.h"

#include "oled.h"


//task
void page_task(uint8_t select_flag);



>>>>>>> e6fe05ce2e12f979570d41f9e36a3280f7a708d1
#endif
