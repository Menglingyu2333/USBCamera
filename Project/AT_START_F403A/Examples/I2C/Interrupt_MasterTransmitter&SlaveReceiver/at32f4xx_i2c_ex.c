/**
  ******************************************************************************
  * @file    I2C/Interrupt_MasterTransmitter&SlaveReceiver/at32f4xx_i2c_ex.c 
  * @author  Artery Technology 
  * @version V1.0.0
  * @date    2020-03-06
  * @brief   The driver of the I2C communication example based on interrupt mode.
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
#include "at32_board.h"
#include "at32f4xx_i2c_ex.h"

/** @addtogroup AT32F403A_StdPeriph_Examples
  * @{
  */

/** @addtogroup Interrupt - Master Transmitter & Slave Receiver
  * @brief  The communication is performed in the interrupt mode.The status of 
  *         all data processing is returned by the same function after 
  *         finishing transfer.
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

/** 
  * @brief  I2C Timeout definition  
  */  
#define I2C_TIMEOUT_BUSY_FLAG     ((uint32_t)10000)  /* Timeout 10 s  */
/** 
  * @brief  I2C Event check flag definition  
  */  
#define I2C_EVT_CHECK_NONE                   ((uint32_t)0x00000000)
#define I2C_EVT_CHECK_ACKFAIL                ((uint32_t)0x00000001)
#define I2C_EVT_CHECK_STOP                   ((uint32_t)0x00000002)

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint16_t Address;

uint16_t TxCount;
uint16_t RxCount;
uint8_t *TxBuffer;
uint8_t *RxBuffer;

static uint16_t I2C_Status;
/* Private function prototypes -----------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

/**
  * @brief  us delay used by the I2C.
  * @param  us: delay x us.
  * @retval None
  */
void I2C_Delay(uint32_t us)
{
  Delay_us(us);
}

/**
  * @brief  Wait for the communication to start.
  * @param  Timeout Timeout duration.
  * @retval I2C status.
  */
static I2C_StatusType I2C_WaitStart(uint32_t Timeout)
{
  while((I2C_Status & I2C_START_BIT) == 0)
  {
    /* 1 ms delay */
    I2C_Delay(1000);
    
    /* Check for the Timeout */
    if((Timeout--) == 0)
    {
      return I2C_TIMEOUT;
    }
  }

  return I2C_OK;
}

/**
  * @brief  Wait for the communication to end.
  * @param  Timeout Timeout duration.
  * @retval I2C status.
  */
static I2C_StatusType I2C_WaitEnd(uint32_t Timeout)
{

  while((I2C_Status & I2C_END_BIT) == 0)
  {
    /* 1 ms delay */
    I2C_Delay(1000);
    
    /* Check for the Timeout */
    if((Timeout--) == 0)
    {
      return I2C_TIMEOUT;
    }
  }

  return I2C_OK;
}

/** @brief  Clears the I2C ADDR pending flag.
  * @param  I2Cx: where x can be 1, 2 or 3 to select the I2C peripheral.
  * @retval None.
  */
void I2C_ClearADDRFlag(I2C_Type* I2Cx)
{
  __IO uint32_t tmpreg;  
  
  tmpreg = I2Cx->STS1; 
  
  tmpreg = I2Cx->STS2; 
}  

/** @brief  Clears the I2C STOPF pending flag.
  * @param  I2Cx: where x can be 1, 2 or 3 to select the I2C peripheral.
  * @retval None.
  */
void I2C_ClearSTOPFlag(I2C_Type* I2Cx)
{
  __IO uint32_t tmpreg;  
                          
  tmpreg = I2Cx->STS1;
  
  tmpreg = I2Cx->CTRL1 |= I2C_CTRL1_PEN; 
}  

/**
  * @brief  This function handles I2C Communication Timeout.
  * @param  I2Cx: where x can be 1, 2 or 3 to select the I2C peripheral.
  * @param  Flag: specifies the I2C flag to check.
  * @param  Status: The new Flag status (SET or RESET).
  * @param  EventCheck: Check flags at the same time.
  * @param  Timeout Timeout duration.
  * @retval I2C status.
  */
static I2C_StatusType I2C_WaitOnFlagUntilTimeout(I2C_Type* I2Cx, uint32_t Flag, FlagStatus Status, uint32_t EventCheck, uint32_t Timeout)
{
  /* delay 10 us = ms * 100 */
  Timeout *= 100;
  
  while(I2C_GetFlagStatus(I2Cx, Flag) == Status)
  {
    /* Check ACKFAIL Flag */
    if(EventCheck & I2C_EVT_CHECK_ACKFAIL)
    {
      if(I2C_GetFlagStatus(I2Cx, I2C_FLAG_ACKFAIL) == SET)
      {
        /* Send STOP Condition */
        I2C_GenerateSTOP(I2Cx, ENABLE);

        /* Clear AF Flag */
        I2C_ClearFlag(I2Cx, I2C_FLAG_ACKFAIL);
        
        return I2C_ERROR;
      }    
    }

    /* Check STOP Flag */
    if(EventCheck & I2C_EVT_CHECK_STOP)
    {
      if(I2C_GetFlagStatus(I2Cx, I2C_FLAG_STOPF) == SET)
      {
        /* Clear STOP Flag */
        I2C_ClearSTOPFlag(I2Cx);

        return I2C_ERROR;
      }    
    }    

    /* 10 us delay */
    I2C_Delay(10);
    
    /* Check for the Timeout */
    if((Timeout--) == 0)
    {
      return I2C_TIMEOUT;
    }
  }
  
  return I2C_OK;
}

/**
  * @brief  Initializes peripherals used by the I2C.
  * @param  None.
  * @retval None.
  */
void LowLevel_Init(void)
{
  GPIO_InitType GPIO_InitStructure;
  NVIC_InitType NVIC_InitStructure; 
  
  /* Config NVIC Group */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
  
  /* I2C_SCL_GPIO_CLK and I2C_SDA_GPIO_CLK Periph clock enable */
  RCC_APB2PeriphClockCmd(I2C_SCL_GPIO_CLK | I2C_SDA_GPIO_CLK, ENABLE);

  /* I2C Periph clock enable */
  RCC_APB1PeriphClockCmd(I2C_CLK, ENABLE);
    
  /* GPIO configuration */  
  /* Configure I2C pins: SCL */
  GPIO_InitStructure.GPIO_Pins = I2C_SCL_PIN;
  GPIO_InitStructure.GPIO_MaxSpeed = GPIO_MaxSpeed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
  GPIO_Init(I2C_SCL_GPIO_PORT, &GPIO_InitStructure);

  /* Configure I2C pins: SDA */
  GPIO_InitStructure.GPIO_Pins = I2C_SDA_PIN;
  GPIO_Init(I2C_SDA_GPIO_PORT, &GPIO_InitStructure); 
  
  NVIC_InitStructure.NVIC_IRQChannel = I2Cx_EV_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 5;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);  

  NVIC_InitStructure.NVIC_IRQChannel = I2Cx_ER_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 5;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

/**
  * @brief  Initializes peripherals used by the I2C EEPROM driver.
  * @param  None.
  * @retval None.
  */
void I2Cx_Init(I2C_Type* I2Cx)
{ 
  I2C_InitType  I2C_InitStructure;
  
  LowLevel_Init();
  
  /* I2C configuration */
  I2C_InitStructure.I2C_Mode = I2C_Mode_I2CDevice;
  I2C_InitStructure.I2C_FmDutyCycle = I2C_FmDutyCycle_2_1;
  I2C_InitStructure.I2C_OwnAddr1 = I2C_SLAVE_ADDRESS7;
  I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
  I2C_InitStructure.I2C_AddrMode = I2C_AddrMode_7bit;
  I2C_InitStructure.I2C_BitRate = I2C_SPEED;
  
  /* I2C Peripheral Enable */
  I2C_Cmd(I2Cx, ENABLE);
  
  /* Apply I2C configuration after enabling it */
  I2C_Init(I2Cx, &I2C_InitStructure);
}

/**
  * @brief  Transmit in master mode an amount of data in non-blocking mode with Interrupt
  * @param  I2Cx: where x can be 1, 2 or 3 to select the I2C peripheral.
  * @param  DevAddress Target device address.
  * @param  pData Pointer to data buffer.
  * @param  Size Amount of data to be sent.
  * @retval I2C status.
  */
I2C_StatusType I2C_Master_Transmit_INT(I2C_Type* I2Cx, uint16_t DevAddress, uint8_t *pData, uint16_t Size, uint32_t Timeout)
{
  /* Update the number of send data*/
  TxCount = Size;  
  
  /* Update the cache of send data*/ 
  TxBuffer = pData;

  /* Update the address*/   
  Address = DevAddress;

  /* Update the I2C Status*/  
  I2C_Status = 0; 
  
  /* Wait until BUSY flag is reset */
  if(I2C_WaitOnFlagUntilTimeout(I2Cx, I2C_FLAG_BUSYF, SET, I2C_EVT_CHECK_NONE, Timeout) != I2C_OK)
  {
    return I2C_ERROR_STEP_1;
  }

  /* Disable Pos */
  I2C_NACKPositionConfig(I2Cx, I2C_NACKPosition_Current);
  
  /* Send START condition */
  I2C_GenerateSTART(I2Cx, ENABLE);

  /* Enable EVT, BUF and ERR interrupt */
  I2C_INTConfig(I2Cx, I2C_INT_EVT | I2C_INT_BUF | I2C_INT_ERR, ENABLE);

  /* Wait for the communication to end */  
  if(I2C_WaitEnd(Timeout) != I2C_OK)
  {
    /* Disable EVT, BUF and ERR interrupt */
    I2C_INTConfig(I2Cx, I2C_INT_EVT | I2C_INT_BUF | I2C_INT_ERR, DISABLE);  
    
    /* Send STOP Condition */
    I2C_GenerateSTOP(I2Cx, ENABLE);
    
    return I2C_ERROR_STEP_4;  
  }
  
  return I2C_OK;
}

/**
  * @brief  Receive in slave mode an amount of data in non-blocking mode with Interrupt.
  * @param  I2Cx: where x can be 1, 2 or 3 to select the I2C peripheral.
  * @param  pData Pointer to data buffer.
  * @param  Size Amount of data to be sent.
  * @retval I2C status.
  */
I2C_StatusType I2C_Slave_Receive_INT(I2C_Type* I2Cx, uint8_t *pData, uint16_t Size, uint32_t Timeout)
{
  /* Update the number of receive data*/
  RxCount = Size;
  
  /* Update the cache of receive data*/ 
  RxBuffer = pData;  
  
  /* Update the I2C Status*/  
  I2C_Status = 0;   

  /* Wait until BUSY flag is reset */
  if(I2C_WaitOnFlagUntilTimeout(I2Cx, I2C_FLAG_BUSYF, SET, I2C_EVT_CHECK_NONE, Timeout) != I2C_OK)
  {
    return I2C_ERROR_STEP_1;
  }

  /* Disable Pos */
  I2C_NACKPositionConfig(I2Cx, I2C_NACKPosition_Current);
  
  /* Enable Address Acknowledge */
  I2C_AcknowledgeConfig(I2Cx, ENABLE); 

  /* Enable EVT, BUF and ERR interrupt */
  I2C_INTConfig(I2Cx, I2C_INT_EVT | I2C_INT_BUF | I2C_INT_ERR, ENABLE);

  /* Wait for the communication to satrt. */  
  /* The state is updated after receiving the address in the interrupt. */ 
  if(I2C_WaitStart(Timeout) != I2C_OK)
  {
    /* Disable EVT, BUF and ERR interrupt */
    I2C_INTConfig(I2Cx, I2C_INT_EVT | I2C_INT_BUF | I2C_INT_ERR, DISABLE);  
    
    return I2C_ERROR_STEP_2;  
  }
  
  /* Communication start */    
  /* Wait for the communication to end */  
  if(I2C_WaitEnd(Timeout) != I2C_OK)
  {
    /* Disable EVT, BUF and ERR interrupt */
    I2C_INTConfig(I2Cx, I2C_INT_EVT | I2C_INT_BUF | I2C_INT_ERR, DISABLE);  
    
    return I2C_ERROR_STEP_3;  
  }
  
  return I2C_OK;
}

/**
  * @brief  This function handles I2C event interrupt request.
  * @param  I2Cx: where x can be 1, 2 or 3 to select the I2C peripheral.
  * @retval None.
  */

#ifdef MASTER_BOARD

void I2C_EV_IRQHandler(I2C_Type* I2Cx)
{
  /* I2C in mode Master Transmitter */
  /* Step 1: Successull generated START conditon */
  if(I2C_GetFlagStatus(I2Cx, I2C_FLAG_STARTF) == SET)
  {
    /* Send slave address for write */
    I2C_Send7bitAddress(I2Cx, Address, I2C_Direction_Transmit);    
  }

  /* Step 2: Successull send slave address */
  else if(I2C_GetFlagStatus(I2Cx, I2C_FLAG_ADDRF) == SET)
  {
    /*Clear ADDRF flag*/  
    I2C_ClearADDRFlag(I2Cx);
  }
  
  /* Step 3: Transfer data */
  else if(I2C_GetFlagStatus(I2Cx, I2C_FLAG_TDE) == SET)
  {
    if(TxCount == 0)
    {
      /* End */
      I2C_Status |= I2C_END_BIT;
      
      /* Disable BUF interrupt */
      I2C_INTConfig(I2Cx, I2C_INT_EVT | I2C_INT_BUF | I2C_INT_ERR, DISABLE);         
      
      /* Data transmission completed. Generate Stop conditon */
      I2C_GenerateSTOP(I2Cx, ENABLE);
    }
    else
    {
      /* Write data to DR */
      I2C_SendData(I2Cx, *TxBuffer++);
      TxCount--;
    }
  }    
}

#else

void I2C_EV_IRQHandler(I2C_Type* I2Cx)
{
  /* I2C in mode Slave Receiver */
  /* Step 1: Receive address */
  if(I2C_GetFlagStatus(I2Cx, I2C_FLAG_ADDRF) == SET)
  {
    /* Communication start */   
    I2C_Status |= I2C_START_BIT;
    
    /* Clear ADDR flag */
    I2C_ClearADDRFlag(I2Cx);  
  }
  
  /* Step 2: Receive data */  
  else if(I2C_GetFlagStatus(I2Cx, I2C_FLAG_RDNE) == SET)
  {
    if(RxCount)
    {
      /* Read data from DR */
      (*RxBuffer++) = I2C_ReceiveData(I2Cx);
      
      RxCount--;     
    }
  }
  
  /* Step 3: received STOP conditon */  
  else if(I2C_GetFlagStatus(I2Cx, I2C_FLAG_STOPF) == SET)
  {
    /* End */
    I2C_Status |= I2C_END_BIT;
    
    /* Disable BUF interrupt */
    I2C_INTConfig(I2Cx, I2C_INT_EVT | I2C_INT_BUF | I2C_INT_ERR, DISABLE);   
  }
}

#endif

/**
  * @brief  This function handles I2C error interrupt request.
  * @param  I2Cx: where x can be 1, 2 or 3 to select the I2C peripheral.
  * @retval None.
  */
void I2C_ER_IRQHandler(I2C_Type* I2Cx)
{
  /* I2C Bus error interrupt occurred */
  if(I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSERR) == SET)
  {
    I2C_ClearFlag(I2Cx, I2C_FLAG_BUSERR);
  }    
  
  /* I2C Arbitration Loss error interrupt occurred */
  if(I2C_GetFlagStatus(I2Cx, I2C_FLAG_ARLOST) == SET)
  {
    I2C_ClearFlag(I2Cx, I2C_FLAG_ARLOST);
  }  

  /* I2C Over-Run/Under-Run interrupt occurred */
  if(I2C_GetFlagStatus(I2Cx, I2C_FLAG_OVRUN) == SET)
  {
    I2C_ClearFlag(I2Cx, I2C_FLAG_OVRUN);
  }  

  /* I2C acknowledge fail occurred */
  if(I2C_GetFlagStatus(I2Cx, I2C_FLAG_ACKFAIL) == SET)
  {
    I2C_ClearFlag(I2Cx, I2C_FLAG_ACKFAIL);
  }
  
  /* I2C SMBus timeout occurred */
  if(I2C_GetFlagStatus(I2Cx, I2C_FLAG_TIMOUT) == SET)
  {
    I2C_ClearFlag(I2Cx, I2C_FLAG_TIMOUT);
  }  

  /* I2C PEC error occurred */
  if(I2C_GetFlagStatus(I2Cx, I2C_FLAG_PECERR) == SET)
  {
    I2C_ClearFlag(I2Cx, I2C_FLAG_PECERR);
  }  
  
  /* Disable the error interrupt */
  I2C_INTConfig(I2Cx, I2C_INT_EVT | I2C_INT_BUF | I2C_INT_ERR, DISABLE);     
}

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT Artery Technology *****END OF FILE****/
