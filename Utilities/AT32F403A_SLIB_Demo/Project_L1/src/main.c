/**
  ******************************************************************************
  * @file    Templates/main.c 
  * @author  ArteryTek SW Team
  * @version V1.1.2
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
#include <stdio.h>
#include "at32f4xx.h"
#include "at32_board.h"
#include "math_helper.h"
#include "fir_filter.h"

/** @addtogroup AT32F413_StdPeriph_Templates
  * @{
  */

/** @addtogroup GPIO_LED_Toggle
  * @{
  */
/* Private define ------------------------------------------------------------*/

#define SNR_THRESHOLD_F32    140.0f
#define TEST_LENGTH_SAMPLES  320

#define TEST_SLIB_PSW           0x55665566
/* Private variables ---------------------------------------------------------*/

extern float32_t testInput_f32_1kHz_15kHz[TEST_LENGTH_SAMPLES];
extern float32_t refOutput[TEST_LENGTH_SAMPLES];
static float32_t testOutput[TEST_LENGTH_SAMPLES];

/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/
void Enable_Flash_INT(void);


/**
  * @brief  Main Function.
  * @param  None
  * @retval None
  */
int main(void)
{
  arm_status status;
  float32_t  *inputF32, *outputF32, snr;
  /* Initialize input and output buffer pointers */
  inputF32 = &testInput_f32_1kHz_15kHz[0];
  outputF32 = &testOutput[0];    
 
 /* Initial LED and Button on AT-START-F413 board */      
  AT32_Board_Init();
 /* Configure Flash to generate an interrupt when a write protect error occur */
  Enable_Flash_INT();
    
 /* Wait for KEY button to be pushed */ 
  while(AT32_BUTTON_State(BUTTON_WAKEUP) == Bit_RESET)
  {
     AT32_LEDn_Toggle(LED3);
     Delay_ms(300); 
  }

  AT32_LEDn_OFF(LED3);

  /* Turn Off LED3 */  
  
  /* Call the SLIB-ed FIR Low Pass Filter function */
  FIR_lowpass_filter(inputF32, outputF32, TEST_LENGTH_SAMPLES);

  /* Compare the generated output against the reference output computed
  in MATLAB. */
  snr = arm_snr_f32(&refOutput[0], &testOutput[0], TEST_LENGTH_SAMPLES);
  if (snr < SNR_THRESHOLD_F32)
  {
     status = ARM_MATH_TEST_FAILURE;
  }
  else
  {
      status = ARM_MATH_SUCCESS;
  }	     
    

  while (1)
  {
    if (status == ARM_MATH_TEST_FAILURE)
    {	
       /* Toggle LED2 */
       AT32_LEDn_Toggle(LED2);
       Delay_ms(500);
    }
    else
    {
       /* Toggle LED4 */
       AT32_LEDn_Toggle(LED4);
       Delay_ms(500);
     }
  }   

}

void Enable_Flash_INT(void)
{
NVIC_InitType NVIC_InitStructure;
    
  FLASH_Unlock();  
  FLASH_INTConfig(FLASH_INT_BANK1_FLR, ENABLE); 
  FLASH_Lock();
    
  NVIC_InitStructure.NVIC_IRQChannel = FLASH_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3; 
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			
  NVIC_Init(&NVIC_InitStructure);	  
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
  {}
}
#endif

/**
  * @}
  */

/**
  * @}
  */



