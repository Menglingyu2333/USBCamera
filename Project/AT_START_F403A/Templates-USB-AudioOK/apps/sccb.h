#ifndef __SCCB_H
#define __SCCB_H
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////
//������ο�������guanfu_wang���롣
//ALIENTEK STM32F103������
//SCCB ��������
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2015/4/16
//�汾��V1.0
//////////////////////////////////////////////////////////////////////////////////


#define SCCB_SDA_IN()  {GPIOB->CTRLH&=0XFFF0FFFF;GPIOB->CTRLH|=0X00080000;}
#define SCCB_SDA_OUT() {GPIOB->CTRLH&=0XFFF0FFFF;GPIOB->CTRLH|=0X00030000;}

//IO��������
#define SCCB_SCL    		PBout(9)	 	//SCL
#define SCCB_SDA    		PBout(12) 		//SDA

#define SCCB_READ_SDA    	PBin(12)  		//����SDA
#define SCCB_ID   			0X60  			//OV2640��ID

///////////////////////////////////////////
void SCCB_Init(void);
void SCCB_Start(void);
void SCCB_Stop(void);
void SCCB_No_Ack(void);
uint8_t SCCB_WR_Byte(uint8_t dat);
uint8_t SCCB_RD_Byte(void);
uint8_t SCCB_WR_Reg(uint8_t reg,uint8_t data);
uint8_t SCCB_RD_Reg(uint8_t reg);
#endif













