#ifndef __USART_H
#define __USART_H
#include "at32f4xx.h"

#define USART_REC_LEN  		1024 
typedef struct{
uint16_t buf[USART_REC_LEN];
uint16_t head;
uint16_t tail;
uint16_t count;	
}UARTSTRUCT;

extern UARTSTRUCT UART1_struct;	  	  	
extern u8  USART_RX_BUF[USART_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
extern u16 USART_RX_STA;         		//接收状态标记	
extern u16 USART_RX_CNT;				//接收的字节数	  
//如果想串口中断接收，请不要注释以下宏定义
void UART_Init(u32 bound);

#endif


