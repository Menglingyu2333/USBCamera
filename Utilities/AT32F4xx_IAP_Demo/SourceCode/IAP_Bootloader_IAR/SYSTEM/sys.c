#include "sys.h"

//THUMBָ�֧�ֻ������
//�������·���ʵ��ִ�л��ָ��WFI  
void WFI_SET(void)
{
	__ASM volatile("wfi");		  
}

//�ر������ж�
void INTX_DISABLE(void)
{		  
	__ASM volatile("cpsid i");
}

//���������ж�
void INTX_ENABLE(void)
{
	__ASM volatile("cpsie i");		  
}

//����ջ����ַ
//addr:ջ����ַ
void MSR_MSP(u32 addr) 
{
	asm("MSR MSP, r0"); 			//set Main Stack value
	asm("BX r14");
}
