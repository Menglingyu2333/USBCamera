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
#include "at32_board.h"

#include "sys.h"
#include "lcd.h"
#include "touch.h"
#include "test.h"

#include "ov2640.h"
#include "delay.h"

#include "usb_lib.h"
#include "hw_config.h"
#include "usb_pwr.h"
#include "wm8988.h"
#include "at32_audio_conf.h"

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
void EXTI3_Open(void);


/* Private functions ---------------------------------------------------------*/

/**
 *	@brief  main function
 *	@param  None
 *	@retval None
 */
void Camera_thread_entry(void * parameter)
{

	LCD_SetWindows(0, 0, ImageWidth - 1, ImageHeight - 1); //LCD窗口设置

  while(1)
  {
  	ImagePtr_W			= 0;
  	ImagePtr_H			= 0;
  	LCD_SetCursor(0, 0);
  	while(OV2640_VSYNC == 1);//丢弃传输到一半的图像
  	while(OV2640_VSYNC == 0);
  	while(OV2640_VSYNC == 1) //开始采集jpeg数据
  	{
  		ImagePtr_W			= 0;

  		while(OV2640_HREF == 0 && OV2640_VSYNC == 1);

  		while(OV2640_HREF)
      {
  			while(OV2640_PCLK == 0);
  			while(OV2640_PCLK == 1);
  			while(OV2640_PCLK == 0);

        AT32_LEDn_Toggle(LED3);
//        AT32_LEDn_ON(LED3);

  			ImageData(ImagePtr_H, ImagePtr_W) = OV2640_DATA;

  			LCD_RS_SET;
  			LCD_CS_CLR;
  			DATAOUT(yuv_8_to_RGBGray(ImageData(ImagePtr_H, ImagePtr_W)));
  			LCD_WR_CLR;
  			LCD_WR_SET;
  			LCD_CS_SET;
//        AT32_LEDn_OFF(LED3);

  			while(OV2640_PCLK == 1);

  			++ImagePtr_W;
  		}
  		++ImagePtr_H;
  	}
  }
}
int main(void)
{
//  SystemInit ();
	GPIO_StructInit(&GPIO_InitStructure);
	EXTI_StructInit(&EXTI_InitStructure);
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	AT32_Board_Init();   ///<Initialize LED and KEY

  UART_Print_Init(115200);


	AT32_USB_GPIO_init();
  /*Enable USB Interrut*/
  USB_Interrupts_Config();
  /*Set USB Clock, USB Clock must 48MHz and clock source is HSE or HSI*/
  Set_USBClock(USBCLK_FROM_HSE);

  /*WM8988 initialize*/
  if ( Audio_Init(AT32_DEFAULT_AUDIO_FREQ, AT32_DEFAULT_AUDIO_BITW) != AUDIO_OK )
  {
      /*WM8988 init Failed*/
      AT32_LEDn_Toggle(LED2);
  }




  /*if use USB SRAM_Size = 768 Byte, default is 512 Byte*/
  Set_USB768ByteMode();
  /* USB protocol and register initialize*/
  USB_Init();
//	AT32_LEDn_ON(LED2);
//	AT32_LEDn_ON(LED3);
//	AT32_LEDn_ON(LED4);


  EXTI1_Config();
  EXTI2_Config();
  EXTI3_Config();

  LCD_Init();
  LCD_SetWindows(0, 0, ImageWidth - 1, ImageHeight - 1); //LCD窗口设置
  Delay_ms(100);
  OV2640_Init();
  //  ov2640_jpg_photo();

//	EXTI_GenerateSWInt(EXTI_Line0); ///<Generate software interrupt


  EXTI3_Open();

  //获取图像缓存
//	Camera_thread_entry(0);
	while (1)
	{
    AT32_LEDn_Toggle(LED4);
    Delay_ms(500);
//    printf("1");
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
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_InitStructure.EXTI_LineEnable = ENABLE;
	EXTI_Init(&EXTI_InitStructure);                              ///<Configure EXTI0 line

	NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
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
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_InitStructure.EXTI_LineEnable = ENABLE;
	EXTI_Init(&EXTI_InitStructure);                              ///<Configure EXTI0 line

	NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
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

void EXTI3_Open(void)
{
	NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);                              ///<Enable and set EXTI0 Interrupt to the lowest priority
}

//
///**
//  * @brief  EXTI9_5 Config.Configure PB9 in interrupt mode
//  * @param  None
//  * @retval None
//  */
//void EXTI9_5_Config(void)
//{
//	RCC_APB2PeriphClockCmd(RCC_APB2PERIPH_GPIOB, ENABLE);         ///<Enable GPIOB clock
//	GPIO_InitStructure.GPIO_Pins = GPIO_Pins_9;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_PU;
//	GPIO_Init(GPIOB, &GPIO_InitStructure);                        ///<Configure PB9 pin as input floating
//
//	RCC_APB2PeriphClockCmd(RCC_APB2PERIPH_AFIO, ENABLE);          ///<Enable AFIO clock
//	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinsSource9);  ///<Connect EXTI9 Line to PB9 pin
//
//	EXTI_InitStructure.EXTI_Line = EXTI_Line9;
//	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
//	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
//	EXTI_InitStructure.EXTI_LineEnable = ENABLE;
//	EXTI_Init(&EXTI_InitStructure);                               ///<Configure EXTI9 line
//
//	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//	NVIC_Init(&NVIC_InitStructure);                               ///<Enable and set EXTI9_5 Interrupt to the lowest priority
//}

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
