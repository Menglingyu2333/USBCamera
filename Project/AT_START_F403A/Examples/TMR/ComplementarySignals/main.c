 /**
  ******************************************************************************
  * @file    TMR/ComplementarySignals/main.c 
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

/** @addtogroup TMR_ComplementarySignals
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
TMR_TimerBaseInitType  TMR_TimeBaseStructure = {0};
TMR_OCInitType  TMR_OCInitStructure = {0};
TMR_BRKDTInitType TMR_BDTRInitStructure = {0};
uint16_t TimerPeriod = 0;
uint16_t Channel1Pulse = 0, Channel2Pulse = 0, Channel3Pulse = 0;

/* Private function prototypes -----------------------------------------------*/
void RCC_Configuration(void);
void GPIO_Configuration(void);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  /* System Clocks Configuration */
  RCC_Configuration();

  /* GPIO Configuration */
  GPIO_Configuration();

  /* -----------------------------------------------------------------------
  TMR1 Configuration to:

  1/ Generate 3 complementary PWM signals with 3 different duty cycles:
    TMR1CLK is fixed to SystemCoreClock, the TMR1 Prescaler is equal to 0 so the 
    TMR1 counter clock used is SystemCoreClock.
    * SystemCoreClock is set to 192 MHz .

    The objective is to generate PWM signal at 17.57 KHz:
    - TMR1_Period = (SystemCoreClock / 17570) - 1

    The Three Duty cycles are computed as the following description: 

    The channel 1 duty cycle is set to 50% so channel 1N is set to 50%.
    The channel 2 duty cycle is set to 25% so channel 2N is set to 75%.
    The channel 3 duty cycle is set to 12.5% so channel 3N is set to 87.5%.
    The Timer pulse is calculated as follows:
      - ChannelxPulse = DutyCycle * (TMR1_Period - 1) / 100

  2/ Insert a dead time equal to 11/SystemCoreClock ns
  3/ Configure the break feature, active at High level, and using the automatic 
     output enable feature
  4/ Use the Locking parameters level1. 
  ----------------------------------------------------------------------- */

  /* Compute the value to be set in ARR register to generate signal frequency at 17.57 Khz */
  TimerPeriod = (SystemCoreClock / 17570) - 1;
  /* Compute CCR1 value to generate a duty cycle at 50% for channel 1 */
  Channel1Pulse = (uint16_t) (((uint32_t) 5 * (TimerPeriod - 1)) / 10);
  /* Compute CCR2 value to generate a duty cycle at 25%  for channel 2 */
  Channel2Pulse = (uint16_t) (((uint32_t) 25 * (TimerPeriod - 1)) / 100);
  /* Compute CCR3 value to generate a duty cycle at 12.5%  for channel 3 */
  Channel3Pulse = (uint16_t) (((uint32_t) 125 * (TimerPeriod - 1)) / 1000);

  /* Time Base configuration */
  TMR_TimeBaseStructInit(&TMR_TimeBaseStructure);
  TMR_TimeBaseStructure.TMR_DIV = 0;
  TMR_TimeBaseStructure.TMR_CounterMode = TMR_CounterDIR_Up;
  TMR_TimeBaseStructure.TMR_Period = TimerPeriod;
  TMR_TimeBaseStructure.TMR_ClockDivision = 0;
  TMR_TimeBaseStructure.TMR_RepetitionCounter = 0;

  TMR_TimeBaseInit(TMR1, &TMR_TimeBaseStructure);

  /* Channel 1, 2 and 3 Configuration in PWM mode */
  TMR_OCStructInit(&TMR_OCInitStructure);
  TMR_OCInitStructure.TMR_OCMode = TMR_OCMode_PWM2;
  TMR_OCInitStructure.TMR_OutputState = TMR_OutputState_Enable;
  TMR_OCInitStructure.TMR_OutputNState = TMR_OutputNState_Enable;
  TMR_OCInitStructure.TMR_Pulse = Channel1Pulse;
  TMR_OCInitStructure.TMR_OCPolarity = TMR_OCPolarity_Low;
  TMR_OCInitStructure.TMR_OCNPolarity = TMR_OCNPolarity_Low;
  TMR_OCInitStructure.TMR_OCIdleState = TMR_OCIdleState_Set;
  TMR_OCInitStructure.TMR_OCNIdleState = TMR_OCIdleState_Reset;

  TMR_OC1Init(TMR1, &TMR_OCInitStructure);

  TMR_OCInitStructure.TMR_Pulse = Channel2Pulse;
  TMR_OC2Init(TMR1, &TMR_OCInitStructure);

  TMR_OCInitStructure.TMR_Pulse = Channel3Pulse;
  TMR_OC3Init(TMR1, &TMR_OCInitStructure);

  /* Automatic Output enable, Break, dead time and lock configuration*/
  TMR_BRKDTStructInit(&TMR_BDTRInitStructure);
  TMR_BDTRInitStructure.TMR_OSIMRState = TMR_OSIMRState_Enable;
  TMR_BDTRInitStructure.TMR_OSIMIState = TMR_OSIMIState_Enable;
  TMR_BDTRInitStructure.TMR_LOCKgrade = TMR_LOCKgrade_1;
  TMR_BDTRInitStructure.TMR_DeadTime = 11;
  TMR_BDTRInitStructure.TMR_Break = TMR_Break_Enable;
  TMR_BDTRInitStructure.TMR_BreakPolarity = TMR_BreakPolarity_High;
  TMR_BDTRInitStructure.TMR_AutomaticOutput = TMR_AutomaticOutput_Enable;

  TMR_BRKDTConfig(TMR1, &TMR_BDTRInitStructure);

  /* TMR1 counter enable */
  TMR_Cmd(TMR1, ENABLE);

  /* Main Output Enable */
  TMR_CtrlPWMOutputs(TMR1, ENABLE);

  while (1)
  {
  }
}

/**
  * @brief  Configures the different system clocks.
  * @param  None
  * @retval None
  */
void RCC_Configuration(void)
{
  /* TMR1, GPIOA, GPIOB, GPIOE and AFIO clocks enable */
  RCC_APB2PeriphClockCmd(RCC_APB2PERIPH_TMR1 | RCC_APB2PERIPH_GPIOA | RCC_APB2PERIPH_GPIOE|
                         RCC_APB2PERIPH_GPIOB |RCC_APB2PERIPH_AFIO, ENABLE);
}

/**
  * @brief  Configure the TMR1 Pins.
  * @param  None
  * @retval None
  */
void GPIO_Configuration(void)
{
  GPIO_InitType GPIO_InitStructure = {0};

  /* GPIOA Configuration: Channel 1, 2 and 3 as alternate function push-pull */
  GPIO_StructInit(&GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Pins = GPIO_Pins_8 | GPIO_Pins_9 | GPIO_Pins_10;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_MaxSpeed = GPIO_MaxSpeed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  /* GPIOB Configuration: Channel 1N, 2N and 3N as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pins = GPIO_Pins_13 | GPIO_Pins_14 | GPIO_Pins_15;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  /* GPIOB Configuration: BKIN pin */
  GPIO_InitStructure.GPIO_Pins = GPIO_Pins_12;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOB, &GPIO_InitStructure);  
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
