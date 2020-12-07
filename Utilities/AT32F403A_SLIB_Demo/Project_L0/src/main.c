/**
  ******************************************************************************
  * @file    Project_L0/src/main.c 
  * @author  ArteryTek SW Team
  * @version V1.0.0
  * @date    2019-02-25
  * @brief   This example provides a description of how to use SLIB protection
  *          feature on AT32F413.
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

/** @addtogroup AT32F413_StdPeriph_Examples
  * @{
  */

/** @addtogroup SLIB_EXAMPLE
  * @{
  */
/* Private define ------------------------------------------------------------*/

//#define USE_SLIB_FUNCTION       /* Define it when user want to activate SLIB in this project */

#if defined (USE_SLIB_FUNCTION)
 #define FLASH_START_ADDR       0x08000000
  
 #define TEST_SLIB_PSW          0x55665566
 #define SLIB_START_ADDR        0x08001000
 #define SLIB_DATA_ADDR         0x08002000
 #define SLIB_END_ADDR          0x08002800 

 #define  FLASH_PAGE_SIZE  2048
 
 #define PAGE_NUM(dwAddr)        ((dwAddr & (FLASH_START_ADDR - 1)) / FLASH_PAGE_SIZE)
 #define PAGE_WORD_CNT   (FLASH_PAGE_SIZE>>2)    // words per page

#endif

#define SNR_THRESHOLD_F32    140.0f
#define TEST_LENGTH_SAMPLES  320


/* Private variables ---------------------------------------------------------*/
#if defined (USE_SLIB_FUNCTION)
 
uint32_t dwStartPage, dwDataStartPage, dwEndPage;
uint32_t TempBuf[512];       // 2KB buffer
uint16_t count;
#endif

extern float32_t testInput_f32_1kHz_15kHz[TEST_LENGTH_SAMPLES];
extern float32_t refOutput[TEST_LENGTH_SAMPLES];
static float32_t testOutput[TEST_LENGTH_SAMPLES];

/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/
#if defined (USE_SLIB_FUNCTION)
FLASH_Status SLIB_Enable(void);
#endif

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

    /* Turn Off LED3 */  
  AT32_LEDn_OFF(LED3);
  
  /* Wait for KEY button to be pushed */ 
  while(AT32_BUTTON_State(BUTTON_WAKEUP) == Bit_RESET);
  
  /* Call the FIR Low Pass Filter function */
  FIR_lowpass_filter(inputF32, outputF32, TEST_LENGTH_SAMPLES);

  /* Compare the generated output against the reference output computed in MATLAB. */
  snr = arm_snr_f32(&refOutput[0], &testOutput[0], TEST_LENGTH_SAMPLES);
  if (snr < SNR_THRESHOLD_F32)
  {
     status = ARM_MATH_TEST_FAILURE;
  }
  else
  {
     status = ARM_MATH_SUCCESS;
      
#if defined (USE_SLIB_FUNCTION)    

    /* Enable SLIB protection from page 2 to page 5 when FIR filter IP-Code is tested OK */ 
           
       if(!(FLASH->SLIB_CDR0 & 0x08))
      {   
         /* If SLIB has not been enabled, set SLIB configuration */        

         if(SLIB_Enable()==FLASH_PRC_DONE)
        {
           /* If SLIB configuration is correctly set */
           /* Tuen on Green LED4 */
           for(count=0; count<10; count++)
           {               
              AT32_LEDn_ON(LED4);
              Delay_ms(300); 
           }   
           /* Execute system reset to activate SLIB protection */
           NVIC_SystemReset();
        }else{
           
          /* Turn on RED LED2 if SLIB activation failed */ 
          AT32_LEDn_ON(LED2);
          while(1);
        }      
      }
      
#endif          

  }	     
    
  /* Infinite loop */
  while (1)
  {
    if (status == ARM_MATH_TEST_FAILURE)
    {	
       /* Toggle RED LED2 if FIR function failed */
       AT32_LEDn_Toggle(LED2);
       Delay_ms(500);
    }
    else
    {
       /* Toggle Green LED4 if FIR function success */
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


#if defined (USE_SLIB_FUNCTION)

FLASH_Status SLIB_Enable(void)
{
uint32_t *Ptr;
uint32_t i, j;  
FLASH_Status Status = FLASH_PRC_DONE;
    
   Ptr = (uint32_t *)SLIB_START_ADDR; 
   dwStartPage = PAGE_NUM(SLIB_START_ADDR);           // SLIB start page
   dwDataStartPage = PAGE_NUM(SLIB_DATA_ADDR);        // SLIB data page
   dwEndPage = PAGE_NUM(SLIB_END_ADDR);                // SLIB end page
   FLASH_Unlock();   
   Status = FLASH_SlibMainEnable(TEST_SLIB_PSW, dwStartPage, dwDataStartPage, dwEndPage);
   if(Status != FLASH_PRC_DONE) return (Status);
    
   for(i=0; i<=(dwEndPage-dwStartPage); i++)
   {  
      memcpy(TempBuf, Ptr, FLASH_PAGE_SIZE);
      Status = FLASH_ErasePage(SLIB_START_ADDR+i*FLASH_PAGE_SIZE);
      if(Status != FLASH_PRC_DONE) return (Status); 
      for(j=0; j<(FLASH_PAGE_SIZE>>2); j++)
      {
        Status = FLASH_ProgramWord(SLIB_START_ADDR+i*FLASH_PAGE_SIZE+(j<<2), TempBuf[j]); 
        if(Status != FLASH_PRC_DONE) return (Status);   
      }
      Ptr+=PAGE_WORD_CNT;            
   }
      
   return (FLASH_PRC_DONE);  
}
#endif

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



