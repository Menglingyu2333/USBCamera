/*============================================================================
//  File Name       : uvcStream.h
//  Author          : HECC. DuckWeed Tech.
//                  @HuiZhou, 2011 
//  email:  hcc21cn@163.com	
//  Version         :   
//      
//  Description     : USB Video Class 摄像头流控制
//
==============================================================================*/
#ifndef 	_UVCSTREAM_H_
#define		_UVCSTREAM_H_
#include "at32f4xx.h"

//----------------------------------------------------------------------------
/* Public typedef -----------------------------------------------------------*/
/* Public define ------------------------------------------------------------*/
/* Public macro -------------------------------------------------------------*/

/* Public variables ---------------------------------------------------------*/
//  共用变量
/* Public function prototypes -----------------------------------------------*/
/* Publice functions ---------------------------------------------------------*/
//  公用函数
void UVC_streamStart(void);
void UVC_SendPack_Irq(void);
s32 UVC_MovieEncoder(u8 reqSync);


#endif
