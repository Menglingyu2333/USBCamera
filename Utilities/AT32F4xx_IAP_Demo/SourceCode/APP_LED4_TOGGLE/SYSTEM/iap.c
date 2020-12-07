#include "at32f4xx.h"
#include "usart.h"
#include "iap.h"
#include "led.h"

void IAP_Command_Handle(void)
{
	if(IAP_Flag==IAP_REV_FLAG_DONE)
	{
		FLASH_Unlock();
		FLASH_ErasePage(IAP_UPGRADE_FLAG_ADDR);
		FLASH_ProgramWord(IAP_UPGRADE_FLAG_ADDR, IAP_UPGRADE_FLAG);
		FLASH_Lock();
		//Back_Ok();
		NVIC_SystemReset();
	}
}

void IAP_Init(void)
{
	SystemCoreClockUpdate();
	//SCB->VTOR = FLASH_BASE | 0x00004000;
	
	if((*(u32*)IAP_UPGRADE_FLAG_ADDR)==IAP_UPGRADE_FLAG)
	{
		FLASH_Unlock();	
		FLASH_ErasePage(IAP_UPGRADE_FLAG_ADDR);
		FLASH_Lock();
	}
}

