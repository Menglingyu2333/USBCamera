 /**
  ******************************************************************************
  * @file    TMR/Gated_mode/main.c 
  * @author  ArteryTek Technology
  * @version V1.0.4
  * @date    2019-01-04
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


/** @addtogroup AT32F403A_StdPeriph_Examples
  * @{
  */

/** @addtogroup TMR_Gated_Mode
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
TMR_TimerBaseInitType  TMR_TimeBaseStructure = {0};
TMR_ICInitType  TMR_ICInitStructure = {0};
TMR_OCInitType  TMR_OCInitStructure = {0};

/* Private function prototypes -----------------------------------------------*/
void RCC_Configuration(void);
void GPIO_Configuration(void);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief   Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  /* System Clocks Configuration */
  RCC_Configuration();

  /* Configure the GPIO ports */
  GPIO_Configuration();

  /* Gated mode test -----
    1/TMR1 is configured as Master Timer:
     - Toggle Mode is used

    2/TMR1 is configured as Slave Timer for an external Trigger connected
     to TMR1 TI2 pin (TMR1 CH2 configured as input pin):
     - The TMR1 TI2FP2 is used as Trigger Input
     - Rising edge is used to start and stop the TMR1: Gated Mode.

    * The TIMxCLK is fixed to 240 MHZ, the Prescaler is equal to 2 so the TMRx clock 
      counter is equal to 80 MHz.
      The Three Timers are running at: 
      TMRx frequency = TMRx clock counter/ 2*(TIMx_Period + 1) = 500 KHz. 
  */
  /* Time base configuration */
  TMR_TimeBaseStructInit(&TMR_TimeBaseStructure);
  TMR_TimeBaseStructure.TMR_Period = 79;
  TMR_TimeBaseStructure.TMR_DIV = 2;
  TMR_TimeBaseStructure.TMR_ClockDivision = 0;
  TMR_TimeBaseStructure.TMR_CounterMode = TMR_CounterDIR_Up;

  TMR_TimeBaseInit(TMR1, &TMR_TimeBaseStructure);

  /* Master Configuration in Toggle Mode */
  TMR_OCStructInit(&TMR_OCInitStructure);
  TMR_OCInitStructure.TMR_OCMode = TMR_OCMode_Toggle;
  TMR_OCInitStructure.TMR_OutputState = TMR_OutputState_Enable;
  TMR_OCInitStructure.TMR_Pulse = 40;
  TMR_OCInitStructure.TMR_OCPolarity = TMR_OCPolarity_High;

  TMR_OC1Init(TMR1, &TMR_OCInitStructure);

  /* TMR1 Input Capture Configuration */
  TMR_ICStructInit(&TMR_ICInitStructure);
  TMR_ICInitStructure.TMR_Channel = TMR_Channel_2;
  TMR_ICInitStructure.TMR_ICPolarity = TMR_ICPolarity_Rising;
  TMR_ICInitStructure.TMR_ICSelection = TMR_ICSelection_DirectTI;
  TMR_ICInitStructure.TMR_ICDIV = TMR_ICDIV_DIV1;
  TMR_ICInitStructure.TMR_ICFilter = 0;

  TMR_ICInit(TMR1, &TMR_ICInitStructure);

  /* TMR1 Input trigger configuration: External Trigger connected to TI2 */
  TMR_SelectInputTrigger(TMR1, TMR_TRGSEL_TI2FP2);
  TMR_SelectSlaveMode(TMR1, TMR_SlaveMode_Gate);

  /* TMR1 Main Output Enable */
  TMR_CtrlPWMOutputs(TMR1, ENABLE);

  /* TMR enable counter */
  TMR_Cmd(TMR1, ENABLE);

  while (1)
  {}
}

/**
  * @brief  Configures the different system clocks.
  * @param  None
  * @retval None
  */
void RCC_Configuration(void)
{

  /* TMR1, GPIOA, GPIOE, GPIOC and GPIOB clocks enable */
  RCC_APB2PeriphClockCmd(RCC_APB2PERIPH_TMR1 | RCC_APB2PERIPH_GPIOA | RCC_APB2PERIPH_GPIOB |
                         RCC_APB2PERIPH_GPIOE | RCC_APB2PERIPH_GPIOC | RCC_APB2PERIPH_AFIO, ENABLE);
}

/**
  * @brief  Configure the GPIO Pins.
  * @param  None
  * @retval None
  */
void GPIO_Configuration(void)
{
  GPIO_InitType GPIO_InitStructure = {0};

  /* GPIOA Configuration: PA.08(TMR1 CH1) as alternate function push-pull */
  GPIO_StructInit(&GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Pins = GPIO_Pins_8 ;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_MaxSpeed = GPIO_MaxSpeed_50MHz;

  GPIO_Init(GPIOA, &GPIO_InitStructure);

  /* GPIOA Configuration: PA.09(TMR1 CH2) */
  GPIO_InitStructure.GPIO_Pins = GPIO_Pins_9;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;

  GPIO_Init(GPIOA, &GPIO_InitStructure);

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

  while (1)
  {}
}

#endif

/**
  * @}
  */ 

/**
  * @}
  */ 
  
 /******************* (C) COPYRIGHT 2018 ArteryTek *****END OF FILE****/
  
