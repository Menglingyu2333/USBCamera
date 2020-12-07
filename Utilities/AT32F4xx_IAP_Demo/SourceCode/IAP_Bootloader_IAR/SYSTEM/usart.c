#include "usart.h"	 
#include "led.h"	

UARTSTRUCT UART1_struct;

//重定义fputc函数 
/*
int fputc(int ch, FILE *f)
{      
	while((USART1->STS&0x40)==0);//循环发送,直到发送完毕   
	USART1->DT = (u8)ch;      
	return ch;
}
 */
//串口1中断服务程序
//注意,读取USARTx->SR能避免莫名其妙的错误   	
u8 USART_RX_BUF[USART_REC_LEN] @(0x2001000);//接收缓冲,最大USART_REC_LEN个字节,起始地址为0X20001000.    
//接收状态
//bit15，	接收完成标志
//bit14，	接收到0x0d
//bit13~0，	接收到的有效字节数目
u16 USART_RX_STA=0;			//接收状态标记	  
u16 USART_RX_CNT=0;			//接收的字节数	   
  
void UART_Init(u32 bound)
{
	GPIO_InitType GPIO_InitStructure;
	USART_InitType USART_InitStructure;
	NVIC_InitType NVIC_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2PERIPH_USART1 | RCC_APB2PERIPH_GPIOA, ENABLE);//使能USART1，GPIOA时钟
	
	//USART1_TX   GPIOA.9 
	GPIO_InitStructure.GPIO_Pins = GPIO_Pins_9; //PA.9
	GPIO_InitStructure.GPIO_MaxSpeed = GPIO_MaxSpeed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
	GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.9

	//USART1_RX	  GPIOA.10初始化
	GPIO_InitStructure.GPIO_Pins = GPIO_Pins_10;//PA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.10  

	//Usart1 NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器

	//USART 初始化设置
	USART_InitStructure.USART_BaudRate = 115200;//串口波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式

	USART_Init(USART1, &USART_InitStructure); //初始化串口1
	USART_INTConfig(USART1, USART_INT_RDNE, ENABLE);//开启串口接受中断
	USART_Cmd(USART1, ENABLE);                    //使能串口1 
}

void USART1_IRQHandler(void)
{
	u16 Res;
	TIME2_IRA_CNT=0;
	
	if(USART_GetITStatus(USART1, USART_INT_RDNE) != RESET)  //接收中断
	{
		Res =USART_ReceiveData(USART1);	//读取接收到的数据    
		if(UART1_struct.count>(USART_REC_LEN-1))
		{
			UART1_struct.count=0;//接收数据错误,重新开始接收	 
			UART1_struct.head=0;
			UART1_struct.tail=0;
		}			
		else  
		{
			UART1_struct.count++;
			UART1_struct.buf[UART1_struct.head++]=Res;
			if(UART1_struct.head>(USART_REC_LEN-1))
			{
					UART1_struct.head=0;
			}
		}
	}
}
