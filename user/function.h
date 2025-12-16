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
#endif
