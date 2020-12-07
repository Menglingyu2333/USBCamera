/**
  ******************************************************************************
  * @file    I2C/EEPROM/main.c 
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
#include "i2c_eeprom.h"
#include "stdio.h"	
#include "string.h"	
#include "at32_board.h"

/** @addtogroup AT32F403A_StdPeriph_Examples
  * @{
  */

/** @addtogroup I2C_EEPROM
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
u8 ucBuf[TEST_EEPROM_SIZE]={0};

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  u16 i=0;
  u16 LEDCount=0;
  u16 WriteFlag=0;

  AT32_Board_Init();
  UART_Print_Init(115200);

  printf("I2C EEPROM demo.\r\n");		
  I2C_EE_Init();

  while(1)
  {
    if(AT32_BUTTON_Press() == BUTTON_WAKEUP)
    {
        WriteFlag++;
        printf("write %u data\r\n",TEST_EEPROM_SIZE);
        if(WriteFlag%2==0)
        {
          for(i=0;i<TEST_EEPROM_SIZE;i++)
            ucBuf[i]=i;
        }
        else
        {
          for(i=0;i<TEST_EEPROM_SIZE;i++)
            ucBuf[i]=255-i;
        }
        I2C_EE_WriteBuffer(ucBuf,TEST_EEPROM_ADDR,TEST_EEPROM_SIZE);
        printf("write done!\r\n");

        printf("read %u data:\r\n",TEST_EEPROM_SIZE);
        memset(ucBuf,0,TEST_EEPROM_SIZE);
        I2C_EE_ReadBuffer(ucBuf,TEST_EEPROM_ADDR,TEST_EEPROM_SIZE);
        for(i=0;i<TEST_EEPROM_SIZE;i++)
            printf("%x ",ucBuf[i]);
        printf("\r\n");
    }
    Delay_ms(1);
    if((LEDCount++)>=200)
    {
      AT32_LEDn_Toggle(LED4);
      LEDCount=0;
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
