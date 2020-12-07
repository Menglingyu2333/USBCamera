#ifndef __LED_H
#define __LED_H	 
#include "at32f4xx.h"

extern u8 TIME2_IRA_CNT;
extern u8 GetDaraFromUSART_Flag;

void LED_Init(void);
void TMR2_GLOBAL_IRQHandler(void);
		 				    
#endif
