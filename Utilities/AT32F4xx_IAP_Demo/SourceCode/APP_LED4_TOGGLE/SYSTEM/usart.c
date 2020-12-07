#include "at32f4xx.h"
#include "usart.h"	 

u8 IAP_Flag=IAP_REV_FLAG_NO;

void Uart_Init(u32 bound)
{
	//GPIO端口设置
	GPIO_InitType GPIO_InitStructure;
	USART_InitType USART_InitStructure;
	NVIC_InitType NVIC_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2PERIPH_USART1 | RCC_APB2PERIPH_GPIOA, ENABLE);		//使能USART1，GPIOA时钟
	//USART1_TX   GPIOA.9 
	GPIO_InitStructure.GPIO_Pins = GPIO_Pins_9;			//PA.9
	GPIO_InitStructure.GPIO_MaxSpeed = GPIO_MaxSpeed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
	GPIO_Init(GPIOA, &GPIO_InitStructure);					//初始化GPIOA.9

	//USART1_RX	  GPIOA.10初始化
	GPIO_InitStructure.GPIO_Pins = GPIO_Pins_10;					//PA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	//浮空输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);								//初始化GPIOA.10  

	//Usart1 NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;	//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;			//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;					//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);			//根据指定的参数初始化VIC寄存器

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
	
	if(USART_GetITStatus(USART1, USART_INT_RDNE) != RESET)  //接收中断
	{
		Res=USART_ReceiveData(USART1);	//读取接收到的数据
		
		if((Res==0x5A)&&(IAP_Flag==IAP_REV_FLAG_NO))
			IAP_Flag=IAP_REV_FLAG_5A;
		else if((Res==0xA5)&&(IAP_Flag==IAP_REV_FLAG_5A))
			IAP_Flag=IAP_REV_FLAG_DONE;
		else 
			IAP_Flag=IAP_REV_FLAG_NO;
	}
}

void Back_Ok(void)
{
	USART1->DT = 0xCC;    
	while((USART1->STS&0x40)==0); 
	USART1->DT = 0xDD;    
	while((USART1->STS&0x40)==0); 
}

void Back_Err(void)
{
	USART1->DT = 0xEE;    
	while((USART1->STS&0x40)==0); 
	USART1->DT = 0xFF;  
	while((USART1->STS&0x40)==0);
}
