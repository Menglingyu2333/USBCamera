#include "at32f4xx.h"
#include "usart.h"	 

u8 IAP_Flag=IAP_REV_FLAG_NO;

void Uart_Init(u32 bound)
{
	//GPIO�˿�����
	GPIO_InitType GPIO_InitStructure;
	USART_InitType USART_InitStructure;
	NVIC_InitType NVIC_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2PERIPH_USART1 | RCC_APB2PERIPH_GPIOA, ENABLE);		//ʹ��USART1��GPIOAʱ��
	//USART1_TX   GPIOA.9 
	GPIO_InitStructure.GPIO_Pins = GPIO_Pins_9;			//PA.9
	GPIO_InitStructure.GPIO_MaxSpeed = GPIO_MaxSpeed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
	GPIO_Init(GPIOA, &GPIO_InitStructure);					//��ʼ��GPIOA.9

	//USART1_RX	  GPIOA.10��ʼ��
	GPIO_InitStructure.GPIO_Pins = GPIO_Pins_10;					//PA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	//��������
	GPIO_Init(GPIOA, &GPIO_InitStructure);								//��ʼ��GPIOA.10  

	//Usart1 NVIC ����
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;	//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;			//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;					//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);			//����ָ���Ĳ�����ʼ��VIC�Ĵ���

	//USART ��ʼ������
	USART_InitStructure.USART_BaudRate = 115200;//���ڲ�����
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
	USART_Init(USART1, &USART_InitStructure); //��ʼ������1
	USART_INTConfig(USART1, USART_INT_RDNE, ENABLE);//�������ڽ����ж�
	USART_Cmd(USART1, ENABLE);                    //ʹ�ܴ���1 
}

void USART1_IRQHandler(void)
{
	u16 Res;
	
	if(USART_GetITStatus(USART1, USART_INT_RDNE) != RESET)  //�����ж�
	{
		Res=USART_ReceiveData(USART1);	//��ȡ���յ�������
		
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
