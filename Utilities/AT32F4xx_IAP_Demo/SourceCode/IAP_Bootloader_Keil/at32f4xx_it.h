/**
 **************************************************************************
 * File Name    : at32f4xx_it.h
 * Description  : Header file of ISR.
 * Date         : 2018-03-15
 * Version      : V1.0.4
 **************************************************************************
 */


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __AT32F4xx_IT_H
#define __AT32F4xx_IT_H

/* Includes ------------------------------------------------------------------*/
#include "at32f4xx.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

void NMI_Handler(void);
void HardFault_Handler(void);
void MemManage_Handler(void);
void BusFault_Handler(void);
void UsageFault_Handler(void);
void SVC_Handler(void);
void DebugMon_Handler(void);
void PendSV_Handler(void);
void SysTick_Handler(void);
void TMR3_GLOBAL_IRQHandler(void);

#endif /* __AT32F4xx_IT_H */


