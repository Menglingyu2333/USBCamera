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
extern u8  USART_RX_BUF[USART_REC_LEN]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern u16 USART_RX_STA;         		//����״̬���	
extern u16 USART_RX_CNT;				//���յ��ֽ���	  
//����봮���жϽ��գ��벻Ҫע�����º궨��
void UART_Init(u32 bound);

#endif


