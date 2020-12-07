/**
  ******************************************************************************
  * @file    main.c 
  * @author  Artery Technology 
  * @version V1.2.0
  * @date    2019-7-3
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, ARTERYTEK SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2018 ArteryTek</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "at32f4xx.h"
#include "led.h"
#include "usart.h"
#include "flash.h"
#include "iap.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
	SystemCoreClockUpdate();
	
  if(FLASH_Read_Upgrade_Flag() == RESET)					// �z��IAP_UPGRADE_FLAG�Ƿ�����, ��δ�����t���D���Ñ�APP
	{
		if(((*(vu32*)(APP_START_ADDR+4))&0xFF000000)==0x08000000)		// �ж�APP��ʼ��ַ�Ƿ�Ϊ0x08xxxxxx.
			IAP_Load_APP(APP_START_ADDR);			// ִ��APP����
	}
	
	UART_Init(115200);				        // ��ʼ������, ���ڸ��� APP ����
	if(FLASH_Read_Upgrade_Flag() != RESET)
		Back_Ok();							// ����λ������OK

	LED_Init();								// ����LED2���ڷ�ת
  while(1)
	{
		IAP_Upgrade_APP_Handle();
	}
}

/**
  * @}
  */

/**
  * @}
  */

/******************* (C) COPYRIGHT 2018 ArteryTek *****END OF FILE****/ 
