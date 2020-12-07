#ifndef __USART_H
#define __USART_H
#include "at32f4xx.h"

#define USART_REC_LEN  			2

#define IAP_REV_FLAG_NO     0
#define IAP_REV_FLAG_5A     1 
#define IAP_REV_FLAG_DONE   2 
#define IAP_REV_FLAG_ERROR  3

extern u8 IAP_Flag;

void Uart_Init(u32 bound);

void Back_Ok(void);
void Back_Err(void);
		
#endif


