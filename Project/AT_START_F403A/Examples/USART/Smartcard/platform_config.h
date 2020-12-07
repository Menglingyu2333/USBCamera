/**
  ******************************************************************************
  * @file    ADC/ADC1_DMA/platform_config.h 
  * @author  Artery Technology
  * @version V1.1.2
  * @date    2019-01-04
  * @brief   specific configuration file.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __PLATFORM_CONFIG_H
#define __PLATFORM_CONFIG_H

/* Includes ------------------------------------------------------------------*/

/* Smartcard Inteface USART pins */
#define SC_USART                 USART3
#define SC_USART_GPIO            GPIOB
#define SC_USART_CLK             RCC_APB1PERIPH_USART3
#define SC_USART_GPIO_CLK        RCC_APB2PERIPH_GPIOB
#define SC_USART_TxPin           GPIO_Pins_10
#define SC_USART_ClkPin          GPIO_Pins_12
#define SC_USART_IRQn            USART3_IRQn 
#define SC_USART_IRQHandler      USART3_IRQHandler    
/* Smartcard Inteface GPIO pins */
#define SC_3_5V                  GPIO_Pins_0  /* GPIOB Pin 0 */
#define SC_RESET                 GPIO_Pins_11 /* GPIOB Pin 11 */
#define SC_CMDVCC                GPIO_Pins_6  /* GPIOC Pin 6  */
#define SC_OFF                   GPIO_Pins_7  /* GPIOC Pin 7 */ 
#define GPIO_3_5V                GPIOB
#define GPIO_RESET               GPIOB
#define GPIO_CMDVCC              GPIOC
#define GPIO_OFF                 GPIOC
#define RCC_APB2Periph_3_5V      RCC_APB2PERIPH_GPIOB
#define RCC_APB2Periph_RESET     RCC_APB2PERIPH_GPIOB
#define RCC_APB2Periph_CMDVCC    RCC_APB2PERIPH_GPIOC
#define RCC_APB2Periph_OFF       RCC_APB2PERIPH_GPIOC
#define SC_EXTI                  EXTI_Line7
#define SC_PortSource            GPIO_PortSourceGPIOC
#define SC_PinSource             GPIO_PinsSource7
#define SC_EXTI_IRQ              EXTI9_5_IRQn


/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

#endif /* __PLATFORM_CONFIG_H */

/******************* (C) COPYRIGHT 2018 ArteryTek *****END OF FILE****/

