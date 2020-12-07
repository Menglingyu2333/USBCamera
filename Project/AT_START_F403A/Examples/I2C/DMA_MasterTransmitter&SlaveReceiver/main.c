/**
  ******************************************************************************
  * @file    I2C/DMA_MasterTransmitter&SlaveReceiver/main.c 
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

/** @addtogroup DMA_MasterTransmitter&SlaveReceiver
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
#define BUF_SIZE     8

/* Private variables ---------------------------------------------------------*/
u8 tx_buf[BUF_SIZE] = {0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8};
u8 rx_buf[BUF_SIZE] = {0};

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Error handler program
  * @param  I2C_Status
  * @retval None
  */
void Error_Handler(I2C_StatusType I2C_Status)
{
  printf("Proc error I2C_ERROR_STEP_%d!!!\r\n", I2C_Status);
}

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  int i = 0;
  
  I2C_StatusType I2C_Status;
  
  AT32_Board_Init();
  
  UART_Print_Init(115200);

  I2Cx_Init(I2C_PORT);
  
  printf("DMA - Master Transmitter & Slave Receiver.\r\n");		  

  while(1)
  {
    
#ifdef MASTER_BOARD   
  
    /* Wait for Key BUTTON_WAKEUP press before starting the Communication */
    /* Timeout is set to 10S  */
    while(AT32_BUTTON_Press() != BUTTON_WAKEUP)
    {
    }
    
    /* Start the transmission process */  
    if((I2C_Status = I2C_Master_Transmit_DMA(I2C_PORT, I2C_SLAVE_ADDRESS7, tx_buf, BUF_SIZE, 10000)) != I2C_OK)
    {
      /* Error_Handler() function is called when error occurs */
      Error_Handler(I2C_Status);
    }       
    else
    {
      printf("Transmit End!\r\n");  
    }
    
#else
    
    /* Start the reception process */  
    /* Timeout is set to 10S  */
    if((I2C_Status = I2C_Slave_Receive_DMA(I2C_PORT, rx_buf, BUF_SIZE, 10000)) != I2C_OK)
    {
      /* Error_Handler() function is called when error occurs */ 
      Error_Handler(I2C_Status);
    }
    else
    {
      printf("Receive OK!\r\n");
      printf("Data:");
      for(i = 0; i < BUF_SIZE; i++)
      {
        printf("%x ", rx_buf[i]);
      }
      printf("\r\n");    
    }
    
#endif
    
  }   
}

/**
  * @}
  */ 

/**
  * @}
  */ 
  
/******************* (C) COPYRIGHT 2018 ArteryTek *****END OF FILE****/ 
