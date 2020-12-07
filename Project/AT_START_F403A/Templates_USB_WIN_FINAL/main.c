/**
 **************************************************************************
 * @file      : EXTI/EXTI_Config/main.c
 * @author    : Artery Technology
 * @version   : V1.0.4
 * @date      : 2018-12-26
 * @brief     : Main program body
 **************************************************************************
 * @brief     : Initializes and turn on LED.Configure external interrupt lines.
 *              Then immediately generation an interrupt on external line 0 by software,it will cause a Toggle on LED3.
 *              And then if monitor a falling trigger on PB9,an interrupt on external line 9 will happen,and it will cause a Toggle on LED4
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
 ***************************************************************************
 */

 /* Includes ------------------------------------------------------------------*/
#include "at32f4xx.h"

#include "sys.h"

#include "ov2640.h"

#include "usb_lib.h"
#include "hw_config.h"
#include "usb_pwr.h"

/** @addtogroup AT32F403A_StdPeriph_Examples
  * @{
  */

/** @addtogroup EXTI_Config
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
GPIO_InitType GPIO_InitStructure;
EXTI_InitType EXTI_InitStructure;
NVIC_InitType NVIC_InitStructure = {0};

/* Private function prototypes -----------------------------------------------*/
void EXTI1_Config(void);
void EXTI2_Config(void);
void EXTI3_Config(void);


/* Private functions ---------------------------------------------------------*/

/**
 *	@brief  main function
 *	@param  None
 *	@retval None
 */
int main(void)
{
	GPIO_StructInit(&GPIO_InitStructure);
	EXTI_StructInit(&EXTI_InitStructure);
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	AT32_Board_Init();   ///<Initialize LED and KEY

  UART_Print_Init(460800);


	AT32_USB_GPIO_init();
  /*Enable USB Interrut*/
  USB_Interrupts_Config();
  /*Set USB Clock, USB Clock must 48MHz and clock source is HSE or HSI*/
  Set_USBClock(USBCLK_FROM_HSE);


  /*if use USB SRAM_Size = 768 Byte, default is 512 Byte*/
//  Set_USB768ByteMode();
  /* USB protocol and register initialize*/
  USB_Init();

  EXTI1_Config();
  EXTI2_Config();
  EXTI3_Config();

  OV2640_Init();

  Frame_RcvPtr = FrameBuf_1_Addr;
  EXTI_Enable(EXTI1_IRQn);

  while(FrameBuf_1_Ready == 0);
  //Ê¹ÄÜUSB´«Êä
  _SetEPTxStatus(ENDP1, EP_TX_VALID);

	while (1)
	{
    AT32_LEDn_Toggle(LED4);
    Delay_ms(500);
	}
}

/**
  * @brief  EXTI0 Config.Configure PA0 in interrupt mode
  * @param  None
  * @retval None
  */
void EXTI1_Config(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2PERIPH_GPIOA, ENABLE);        ///<Enable GPIOA clock
	GPIO_InitStructure.GPIO_Pins = GPIO_Pins_1;
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_PU;
 	GPIO_InitStructure.GPIO_MaxSpeed = GPIO_MaxSpeed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);                       ///<Configure PA.00 pin as input floating

	RCC_APB2PeriphClockCmd(RCC_APB2PERIPH_AFIO, ENABLE);         ///<Enable AFIO clock
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinsSource1); ///<Connect EXTI0 Line to PA0 pin

	EXTI_InitStructure.EXTI_Line = EXTI_Line1;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
	EXTI_InitStructure.EXTI_LineEnable = ENABLE;
	EXTI_Init(&EXTI_InitStructure);                              ///<Configure EXTI0 line

	NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;
	NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;
	NVIC_Init(&NVIC_InitStructure);                              ///<Enable and set EXTI0 Interrupt to the lowest priority
}

/**
  * @brief  EXTI0 Config.Configure PA0 in interrupt mode
  * @param  None
  * @retval None
  */
void EXTI2_Config(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2PERIPH_GPIOA, ENABLE);        ///<Enable GPIOA clock
	GPIO_InitStructure.GPIO_Pins = GPIO_Pins_2;
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_PU;
 	GPIO_InitStructure.GPIO_MaxSpeed = GPIO_MaxSpeed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);                       ///<Configure PA.00 pin as input floating

	RCC_APB2PeriphClockCmd(RCC_APB2PERIPH_AFIO, ENABLE);         ///<Enable AFIO clock
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinsSource2); ///<Connect EXTI0 Line to PA0 pin

	EXTI_InitStructure.EXTI_Line = EXTI_Line2;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
	EXTI_InitStructure.EXTI_LineEnable = ENABLE;
	EXTI_Init(&EXTI_InitStructure);                              ///<Configure EXTI0 line

	NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;
	NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;
	NVIC_Init(&NVIC_InitStructure);                              ///<Enable and set EXTI0 Interrupt to the lowest priority
}

/**
  * @brief  EXTI0 Config.Configure PA0 in interrupt mode
  * @param  None
  * @retval None
  */
void EXTI3_Config(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2PERIPH_GPIOA, ENABLE);        ///<Enable GPIOA clock
	GPIO_InitStructure.GPIO_Pins = GPIO_Pins_3;
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_PU;
 	GPIO_InitStructure.GPIO_MaxSpeed = GPIO_MaxSpeed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);                       ///<Configure PA.00 pin as input floating

	RCC_APB2PeriphClockCmd(RCC_APB2PERIPH_AFIO, ENABLE);         ///<Enable AFIO clock
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinsSource3); ///<Connect EXTI0 Line to PA0 pin

	EXTI_InitStructure.EXTI_Line = EXTI_Line3;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_InitStructure.EXTI_LineEnable = ENABLE;
	EXTI_Init(&EXTI_InitStructure);                              ///<Configure EXTI0 line

	NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;
	NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;
	NVIC_Init(&NVIC_InitStructure);                              ///<Enable and set EXTI0 Interrupt to the lowest priority
}


#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}

#endif

/**
  * @}
  */

/**
  * @}
  */

/******************* (C) COPYRIGHT 2018 ArteryTek *****END OF FILE****/
