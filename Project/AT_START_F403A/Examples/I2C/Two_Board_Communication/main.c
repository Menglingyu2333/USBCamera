/**
  ******************************************************************************
  * @file    I2C/Two_Board_Communication/main.c 
  * @author  Artery Technology 
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
#include "at32f4xx.h"
#include "i2c.h"
#include "stdio.h"	
#include "string.h"	
#include "at32_board.h"

/** @addtogroup AT32F403A_StdPeriph_Examples
  * @{
  */

/** @addtogroup Two_Board_Communication
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

/*
 * 0   Slave Receive
 * 1   Slave Transmit
 * 2   Master Receive
 * 3   Master Transmit
 */
#define IIC_MODE   0

/* Private macro -------------------------------------------------------------*/
#define TX_SIZE      8
#define RX_SIZE      TX_SIZE

/* Private variables ---------------------------------------------------------*/
u8 tx_buf[TX_SIZE] = {0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8};
u8 rx_buf[RX_SIZE] = {0};

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Error handler program
  * @param  None
  * @retval None
  */
void Error_Handler(void)
{
  printf("Proc error!!!\r\n");
  /* Block communication and all processes */
  while (1)
  {   
  }
}

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
#if (IIC_MODE == 0) || (IIC_MODE == 2)
  int i = 0;
#endif

  AT32_Board_Init();
  UART_Print_Init(115200);

  AT32_I2C_Init();
  printf("I2C Two Board Communication demo.\r\n");		

  while(1)
  {
    if(AT32_BUTTON_Press() == BUTTON_WAKEUP)
    {
#if   (IIC_MODE == 0)
      if(I2C_Slave_Receive_INT(rx_buf, RX_SIZE) != PROC_OK)
      {
        Error_Handler();
      }
#elif (IIC_MODE == 1)
      if(I2C_Slave_Transmit_INT(tx_buf, TX_SIZE) != PROC_OK)
      {
        Error_Handler();
      }
#elif (IIC_MODE == 2)
      if(I2C_Master_Receive_INT(I2C_ADDRESS, rx_buf, RX_SIZE) != PROC_OK)
      {
        Error_Handler();
      }
#elif (IIC_MODE == 3)
      if(I2C_Master_Transmit_INT(I2C_ADDRESS, tx_buf, TX_SIZE) != PROC_OK)
      {
        Error_Handler();
      }
#endif

#if (IIC_MODE == 0) || (IIC_MODE == 2)
      printf("Receive OK!\r\n");
      printf("Data:");
      for(i=0; i<RX_SIZE; i++)
      {
        printf("%x ", rx_buf[i]);
      }
      printf("\r\n");
#else
      printf("Transmit OK!\r\n");
#endif
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
