/*
 **************************************************************************
 * File Name    : random.h
 * Description  : Header file for AT-START board
 *              : 1. Set of firmware functions to manage Leds, push-button and COM ports.
 *                2. initialize Delay Function and USB
 * Date         : 2018-11-28
 * Version      : V1.0.2
 */
/****************** (C) COPYRIGHT 2019 ArteryTek *********END OF FILE*********/

#ifndef __RANDOM_H
#define __RANDOM_H	 
#include <at32f4xx.h>

#define EnableRTCAsSeed     (1)  //This define  seed for random number gen,RTC for AT32F403/AT32F413,ERTC for AT32F415

void RandNum_Test ( void);


#endif
