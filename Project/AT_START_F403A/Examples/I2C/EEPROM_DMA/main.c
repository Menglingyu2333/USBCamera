/**
  ******************************************************************************
  * @file    I2C/EEPROM_DMA/main.c 
  * @author  Artery Technology 
  * @version V1.0.0
  * @date    2020-03-06
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
#include "at32f4xx_i2c_ex.h"
#include "stdio.h"	
#include "string.h"	
#include "at32_board.h"

/** @addtogroup AT32F403A_StdPeriph_Examples
  * @{
  */

/** @addtogroup EEPROM_DMA
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
#define BUF_SIZE      12


/* Private variables ---------------------------------------------------------*/
u8 write_buf1[BUF_SIZE] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x10, 0x11, 0x12};
u8 write_buf2[BUF_SIZE] = {0x10, 0x20, 0x30, 0x40, 0x50, 0x60, 0x70, 0x80, 0x90, 0xA0, 0xB0, 0xC0};
u8 read_buf[BUF_SIZE] = {0};

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/


/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  int i = 0, Flag = 0;

  AT32_Board_Init();
  
  UART_Print_Init(115200);

  I2Cx_Init(I2C_PORT);
  
  printf("I2C EEPROM demo(DMA).\r\n");		

  while(1)
  { 
    /* Wait for Key BUTTON_KEY1_DOWN press before starting write */
    if(AT32_BUTTON_Press() == BUTTON_WAKEUP)
    {
      switch(Flag)
      {
        /* Write buffer 1*/ 
        case 0:
        {
          I2C_EE_WriteBuffer_DMA(I2C_PORT, 0, write_buf1, BUF_SIZE, 1000);
          
          printf("Write buffer 1!\r\n");
        }break;
        
        /* Read buffer 1*/ 
        case 1:
        {
          I2C_EE_ReadBuffer_DMA(I2C_PORT, 0, read_buf, BUF_SIZE, 1000);
        
          for(i = 0; i < BUF_SIZE; i++)
          {      
            printf("%X ",read_buf[i]);
          }
          
          printf("\r\n");
        }break;
        
        /* Write buffer 2*/         
        case 2:
        {
          I2C_EE_WriteBuffer_DMA(I2C_PORT, 0, write_buf2, BUF_SIZE, 1000);
          
          printf("Write buffer 2!\r\n");
        }break;
        
        /* Read buffer 2*/ 
        case 3:
        {
          I2C_EE_ReadBuffer_DMA(I2C_PORT, 0, read_buf, BUF_SIZE, 1000);
        
          for(i = 0; i < BUF_SIZE; i++)
          {      
            printf("%X ",read_buf[i]);
          }
          
          printf("\r\n");
        }break;  
        
        default:
        {
          Flag = 0;
        }break;        
      }
      
      Flag++;
      
      if(Flag == 4)
      {
        Flag = 0;
      }
    }
  }   
}

/**
  * @}
  */ 

/**
  * @}
  */ 
  
/******************* (C) COPYRIGHT 2018 ArteryTek *****END OF FILE****/ 
