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
	
  if(FLASH_Read_Upgrade_Flag() == RESET)					// 檢查IAP_UPGRADE_FLAG是否置起, 若未置起則跳轉到用戶APP
	{
		if(((*(vu32*)(APP_START_ADDR+4))&0xFF000000)==0x08000000)		// 判断APP起始地址是否为0x08xxxxxx.
			IAP_Load_APP(APP_START_ADDR);			// 执行APP代码
	}
	
	UART_Init(115200);				        // 初始化傳口, 用于更新 APP 内容
	if(FLASH_Read_Upgrade_Flag() != RESET)
		Back_Ok();							// 向上位机返回OK

	LED_Init();								// 用于LED2周期翻转
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
