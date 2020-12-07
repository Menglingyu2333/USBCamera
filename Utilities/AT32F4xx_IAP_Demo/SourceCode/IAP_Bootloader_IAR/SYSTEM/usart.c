#include "usart.h"	 
#include "led.h"	

UARTSTRUCT UART1_struct;

//�ض���fputc���� 
/*
int fputc(int ch, FILE *f)
{      
	while((USART1->STS&0x40)==0);//ѭ������,ֱ���������   
	USART1->DT = (u8)ch;      
	return ch;
}
 */
//����1�жϷ������
//ע��,��ȡUSARTx->SR�ܱ���Ī������Ĵ���   	
u8 USART_RX_BUF[USART_REC_LEN] @(0x2001000);//���ջ���,���USART_REC_LEN���ֽ�,��ʼ��ַΪ0X20001000.    
//����״̬
//bit15��	������ɱ�־
//bit14��	���յ�0x0d
//bit13~0��	���յ�����Ч�ֽ���Ŀ
u16 USART_RX_STA=0;			//����״̬���	  
u16 USART_RX_CNT=0;			//���յ��ֽ���	   
  
void UART_Init(u32 bound)
{
	GPIO_InitType GPIO_InitStructure;
	USART_InitType USART_InitStructure;
	NVIC_InitType NVIC_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2PERIPH_USART1 | RCC_APB2PERIPH_GPIOA, ENABLE);//ʹ��USART1��GPIOAʱ��
	
	//USART1_TX   GPIOA.9 
	GPIO_InitStructure.GPIO_Pins = GPIO_Pins_9; //PA.9
	GPIO_InitStructure.GPIO_MaxSpeed = GPIO_MaxSpeed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
	GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA.9

	//USART1_RX	  GPIOA.10��ʼ��
	GPIO_InitStructure.GPIO_Pins = GPIO_Pins_10;//PA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
	GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA.10  

	//Usart1 NVIC ����
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���

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
	TIME2_IRA_CNT=0;
	
	if(USART_GetITStatus(USART1, USART_INT_RDNE) != RESET)  //�����ж�
	{
		Res =USART_ReceiveData(USART1);	//��ȡ���յ�������    
		if(UART1_struct.count>(USART_REC_LEN-1))
		{
			UART1_struct.count=0;//�������ݴ���,���¿�ʼ����	 
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
