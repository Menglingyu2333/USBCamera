#include "lcd.h"
#include "FONT.h"
#include "delay.h"
//#include "stdlib.h"
//#include "usart.h"
//#include "delay.h"
//////////////////////////////////////////////////////////////////////////////////
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//����Ӳ������Ƭ��STM32F103RBT6,��Ƶ72M  ��Ƭ��������ѹ3.3V
//QDtech-TFTҺ������ for STM32 IOģ��
//xiao��@ShenZhen QDtech co.,LTD
//��˾��վ:www.qdtech.net
//�Ա���վ��http://qdtech.taobao.com
//��˾�ṩ����֧�֣��κμ������⻶ӭ��ʱ����ѧϰ
//�̻�(����) :+86 0755-23594567
//�ֻ�:15989313508���빤��
//����:QDtech2008@gmail.com
//Skype:QDtech2008
//��������QQȺ:324828016
//��������:2013/5/13
//�汾��V1.1
//��Ȩ���У�����ؾ���
//Copyright(C) ������ȫ�����Ӽ������޹�˾ 2009-2019
//All rights reserved
//////////////////////////////////////////////////////////////////////////////////
/****************************************************************************************************
//=======================================Һ���������߽���==========================================//
STM32 PB���Һ����DB0~DB16,��������ΪDB0��PB0,..DB15��PB15.
//=======================================Һ���������߽���==========================================//
//LCD_CS	��PC9	//Ƭѡ�ź�
//LCD_RS	��PC8	//�Ĵ���/����ѡ���ź�
//LCD_WR	��PC7	//д�ź�
//LCD_RD	��PC6	//���ź�
//LCD_RST	��PC5	//��λ�ź�
//LCD_LED	��PC10	//��������ź�(�ߵ�ƽ����)
//=========================================������������=========================================//
//��ʹ�ô�������ģ�鱾������������ɲ�����
//MO(MISO)	��PC2	//SPI�������
//MI(MOSI)	��PC3	//SPI��������
//PEN		��PC1	//�������ж��ź�
//TCS		��PC13	//����ICƬѡ
//CLK		��PC0	//SPI����ʱ��
**************************************************************************************************/

//����LCD��Ҫ����
//Ĭ��Ϊ����
_lcd_dev lcddev;

//������ɫ,������ɫ
u16 POINT_COLOR = 0x0000,BACK_COLOR = 0xFFFF;
u16 DeviceCode;



//******************************************************************
//��������  LCD_WR_REG
//���ߣ�    xiao��@ȫ������
//���ڣ�    2013-02-22
//���ܣ�    ��Һ��������д��д16λָ��
//���������Reg:��д���ָ��ֵ
//����ֵ��  ��
//�޸ļ�¼����
//******************************************************************
void LCD_WR_REG(u8 data)
{
#if LCD_USE8BIT_MODEL==1//ʹ��8λ������������ģʽ
	LCD_RS_CLR;
	LCD_CS_CLR;
	DATAOUT(data<<8);
	LCD_WR_CLR;
	LCD_WR_SET;
	LCD_CS_SET;

#else//ʹ��16λ������������ģʽ
	LCD_RS_CLR;
	LCD_CS_CLR;
	DATAOUT(data);
	LCD_WR_CLR;
	LCD_WR_SET;
	LCD_CS_SET;

#endif
}

//******************************************************************
//��������  LCD_WR_DATA
//���ߣ�    xiao��@ȫ������
//���ڣ�    2013-02-22
//���ܣ�    ��Һ��������д��д16λ����
//���������Data:��д�������
//����ֵ��  ��
//�޸ļ�¼����
//******************************************************************
void LCD_WR_DATA(u16 data)
{


#if LCD_USE8BIT_MODEL==1//ʹ��8λ������������ģʽ
	LCD_RS_SET;
	LCD_CS_CLR;
	DATAOUT(data<<8);
	LCD_WR_CLR;
	LCD_WR_SET;
	LCD_CS_SET;

#else//ʹ��16λ������������ģʽ
	LCD_RS_SET;
	LCD_CS_CLR;
	DATAOUT(data);
	LCD_WR_CLR;
	LCD_WR_SET;
	LCD_CS_SET;

#endif

}
//******************************************************************
//��������  LCD_DrawPoint_16Bit
//���ߣ�    xiao��@ȫ������
//���ڣ�    2013-02-22
//���ܣ�    8λ���������д��һ��16λ����
//���������(x,y):�������
//����ֵ��  ��
//�޸ļ�¼����
//******************************************************************
void LCD_DrawPoint_16Bit(u16 color)
{
#if LCD_USE8BIT_MODEL==1
	LCD_CS_CLR;
	LCD_RD_SET;
	LCD_RS_SET;//д��ַ
	DATAOUT(color);
	LCD_WR_CLR;
	LCD_WR_SET;
	DATAOUT(color<<8);
	LCD_WR_CLR;
	LCD_WR_SET;
	LCD_CS_SET;
#else
	LCD_WR_DATA(color);
#endif


}

//******************************************************************
//��������  LCD_WriteReg
//���ߣ�    xiao��@ȫ������
//���ڣ�    2013-02-22
//���ܣ�    д�Ĵ�������
//���������LCD_Reg:�Ĵ�����ַ
//			LCD_RegValue:Ҫд�������
//����ֵ��  ��
//�޸ļ�¼����
//******************************************************************
void LCD_WriteReg(u8 LCD_Reg, u16 LCD_RegValue)
{
	LCD_WR_REG(LCD_Reg);
	LCD_WR_DATA(LCD_RegValue);
}

//******************************************************************
//��������  LCD_WriteRAM_Prepare
//���ߣ�    xiao��@ȫ������
//���ڣ�    2013-02-22
//���ܣ�    ��ʼдGRAM
//			�ڸ�Һ��������RGB����ǰ��Ӧ�÷���дGRAMָ��
//�����������
//����ֵ��  ��
//�޸ļ�¼����
//******************************************************************
void LCD_WriteRAM_Prepare(void)
{
	LCD_WR_REG(lcddev.wramcmd);
}

//******************************************************************
//��������  LCD_DrawLine
//���ߣ�    xiao��@ȫ������
//���ڣ�    2013-02-22
//���ܣ�    GUI����
//���������x1,y1:�������
//        	x2,y2:�յ�����
//����ֵ��  ��
//�޸ļ�¼����
//******************************************************************
void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2)
{
	u16 t;
	int xerr=0,yerr=0,delta_x,delta_y,distance;
	int incx,incy,uRow,uCol;

	delta_x=x2-x1; //������������
	delta_y=y2-y1;
	uRow=x1;
	uCol=y1;
	if(delta_x>0)incx=1; //���õ�������
	else if(delta_x==0)incx=0;//��ֱ��
	else {incx=-1;delta_x=-delta_x;}
	if(delta_y>0)incy=1;
	else if(delta_y==0)incy=0;//ˮƽ��
	else{incy=-1;delta_y=-delta_y;}
	if( delta_x>delta_y)distance=delta_x; //ѡȡ��������������
	else distance=delta_y;
	for(t=0;t<=distance+1;t++ )//�������
	{
		LCD_DrawPoint(uRow,uCol);//����
		xerr+=delta_x ;
		yerr+=delta_y ;
		if(xerr>distance)
		{
			xerr-=distance;
			uRow+=incx;
		}
		if(yerr>distance)
		{
			yerr-=distance;
			uCol+=incy;
		}
	}
}

//******************************************************************
//��������  LCD_DrawPoint
//���ߣ�    xiao��@ȫ������
//���ڣ�    2013-02-22
//���ܣ�    ��ָ��λ��д��һ�����ص�����
//���������(x,y):�������
//����ֵ��  ��
//�޸ļ�¼����
//******************************************************************
void LCD_DrawPoint(u16 x,u16 y)
{
	LCD_SetCursor(x,y);//���ù��λ��
#if LCD_USE8BIT_MODEL==1
	LCD_CS_CLR;
	LCD_RD_SET;
	LCD_RS_SET;//д��ַ
	DATAOUT(POINT_COLOR);
	LCD_WR_CLR;
	LCD_WR_SET;
	DATAOUT(POINT_COLOR<<8);
	LCD_WR_CLR;
	LCD_WR_SET;
	LCD_CS_SET;
#else
	LCD_WR_DATA(POINT_COLOR);
#endif


}

//******************************************************************
//��������  LCD_Clear
//���ߣ�    xiao��@ȫ������
//���ڣ�    2013-02-22
//���ܣ�    LCDȫ�������������
//���������Color:Ҫ���������ɫ
//����ֵ��  ��
//�޸ļ�¼����
//******************************************************************
void LCD_Clear(u16 Color)
{
	u32 index=0;
	LCD_SetWindows(0,0,lcddev.width-1,lcddev.height-1);
	/*
#if LCD_USE8BIT_MODEL==1
	LCD_RS_SET;//д����
	LCD_CS_CLR;
	for(index=0;index<lcddev.width*lcddev.height;index++)
	{
		DATAOUT(Color);
		LCD_WR_CLR;
		LCD_WR_SET;

		DATAOUT(Color<<8);
		LCD_WR_CLR;
		LCD_WR_SET;
	}
	LCD_CS_SET;
#else //16λģʽ
	for(index=0;index<lcddev.width*lcddev.height;index++)
	{
		LCD_WR_DATA(Color);
	}
#endif
	*/
		LCD_RS_SET;//д����
	LCD_CS_CLR;
	for(index=0;index<lcddev.width*lcddev.height;index++)
	{
		DATAOUT(Color);
		LCD_WR_CLR;
		LCD_WR_SET;

		DATAOUT(Color<<8);
		LCD_WR_CLR;
		LCD_WR_SET;
	}
	LCD_CS_SET;

}

//******************************************************************
//��������  LCD_GPIOInit
//���ߣ�    xiao��@ȫ������
//���ڣ�    2013-02-22
//���ܣ�    Һ����IO��ʼ����Һ����ʼ��ǰҪ���ô˺���
//�����������
//����ֵ��  ��
//�޸ļ�¼����
//******************************************************************
void LCD_GPIOInit(void)
{
	GPIO_InitType GPIO_InitStructure;

 	RCC_APB2PeriphClockCmd(RCC_APB2PERIPH_GPIOC|RCC_APB2PERIPH_GPIOE|RCC_APB2PERIPH_AFIO, ENABLE);
	GPIO_PinsRemapConfig(GPIO_Remap_SWJ_JTAGDisable , ENABLE);

	GPIO_InitStructure.GPIO_Pins = GPIO_Pins_10|GPIO_Pins_0|GPIO_Pins_1|GPIO_Pins_2|GPIO_Pins_3|GPIO_Pins_5;	   //GPIO_Pin_10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT_PP;  //�������
	GPIO_InitStructure.GPIO_MaxSpeed = GPIO_MaxSpeed_10MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure); //GPIOC
	GPIO_SetBits(GPIOC,GPIO_Pins_10|GPIO_Pins_0|GPIO_Pins_1|GPIO_Pins_2|GPIO_Pins_3|GPIO_Pins_5);


	GPIO_InitStructure.GPIO_Pins = GPIO_Pins_All;	//
	GPIO_Init(GPIOE, &GPIO_InitStructure); //GPIOB
	GPIO_SetBits(GPIOE,GPIO_Pins_All);
}

//******************************************************************
//��������  LCD_Reset
//���ߣ�    xiao��@ȫ������
//���ڣ�    2013-02-22
//���ܣ�    LCD��λ������Һ����ʼ��ǰҪ���ô˺���
//�����������
//����ֵ��  ��
//�޸ļ�¼����
//******************************************************************
void LCD_RESET(void)
{
	LCD_RST_CLR;
	delay_ms(100);
	LCD_RST_SET;
	delay_ms(50);
}
u16 LCD_RD_DATA(void)
{
	u16 t;
 	GPIOE->CTRLL=0X88888888; //PB0-7  ��������
	GPIOE->CTRLH=0X88888888; //PB8-15 ��������
	GPIOE->OPTDT=0X0000;     //ȫ�����0

	LCD_RS_SET;
	LCD_CS_CLR;
	//��ȡ����(���Ĵ���ʱ,������Ҫ��2��)
	LCD_RD_CLR;
	delay_us(2);//FOR 8989,��ʱ2us
	t=DATAIN;
	LCD_RD_SET;
	LCD_CS_SET;

	GPIOE->CTRLL=0X33333333; //PB0-7  �������
	GPIOE->CTRLH=0X33333333; //PB8-15 �������
	GPIOE->OPTDT=0XFFFF;    //ȫ�������
	return t;
}

//******************************************************************
//��������  LCD_Init
//���ߣ�    xiao��@ȫ������
//���ڣ�    2013-02-22
//���ܣ�    LCD��ʼ��
//�����������
//����ֵ��  ��
//�޸ļ�¼����
//******************************************************************
void LCD_Init(void)
{

	LCD_GPIOInit();
 	LCD_RESET();

    LCD_WR_REG(0XD3);
    LCD_RD_DATA();              //dummy read
    LCD_RD_DATA();              //����0X00
    lcddev.id=LCD_RD_DATA();    //��ȡ93
    lcddev.id<<=8;
    lcddev.id|=LCD_RD_DATA();   //��ȡ41

	//************* Start Initial Sequence **********//
	LCD_WR_REG(0xCF);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0xC1);
	LCD_WR_DATA(0X30);
	LCD_WR_REG(0xED);
	LCD_WR_DATA(0x64);
	LCD_WR_DATA(0x03);
	LCD_WR_DATA(0X12);
	LCD_WR_DATA(0X81);
	LCD_WR_REG(0xE8);
	LCD_WR_DATA(0x85);
	LCD_WR_DATA(0x10);
	LCD_WR_DATA(0x7A);
	LCD_WR_REG(0xCB);
	LCD_WR_DATA(0x39);
	LCD_WR_DATA(0x2C);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x34);
	LCD_WR_DATA(0x02);
	LCD_WR_REG(0xF7);
	LCD_WR_DATA(0x20);
	LCD_WR_REG(0xEA);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_REG(0xC0);    //Power control
	LCD_WR_DATA(0x1B);   //VRH[5:0]
	LCD_WR_REG(0xC1);    //Power control
	LCD_WR_DATA(0x01);   //SAP[2:0];BT[3:0]
	LCD_WR_REG(0xC5);    //VCM control
	LCD_WR_DATA(0x30); 	 //3F
	LCD_WR_DATA(0x30); 	 //3C
	LCD_WR_REG(0xC7);    //VCM control2
	LCD_WR_DATA(0XB7);
	LCD_WR_REG(0x36);    // Memory Access Control
	LCD_WR_DATA(0x48);
	LCD_WR_REG(0x3A);
	LCD_WR_DATA(0x55);
	LCD_WR_REG(0xB1);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x1A);
	LCD_WR_REG(0xB6);    // Display Function Control
	LCD_WR_DATA(0x0A);
	LCD_WR_DATA(0xA2);
	LCD_WR_REG(0xF2);    // 3Gamma Function Disable
	LCD_WR_DATA(0x00);
	LCD_WR_REG(0x26);    //Gamma curve selected
	LCD_WR_DATA(0x01);
	LCD_WR_REG(0xE0);    //Set Gamma
	LCD_WR_DATA(0x0F);
	LCD_WR_DATA(0x2A);
	LCD_WR_DATA(0x28);
	LCD_WR_DATA(0x08);
	LCD_WR_DATA(0x0E);
	LCD_WR_DATA(0x08);
	LCD_WR_DATA(0x54);
	LCD_WR_DATA(0XA9);
	LCD_WR_DATA(0x43);
	LCD_WR_DATA(0x0A);
	LCD_WR_DATA(0x0F);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_REG(0XE1);    //Set Gamma
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x15);
	LCD_WR_DATA(0x17);
	LCD_WR_DATA(0x07);
	LCD_WR_DATA(0x11);
	LCD_WR_DATA(0x06);
	LCD_WR_DATA(0x2B);
	LCD_WR_DATA(0x56);
	LCD_WR_DATA(0x3C);
	LCD_WR_DATA(0x05);
	LCD_WR_DATA(0x10);
	LCD_WR_DATA(0x0F);
	LCD_WR_DATA(0x3F);
	LCD_WR_DATA(0x3F);
	LCD_WR_DATA(0x0F);
	LCD_WR_REG(0x2B);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x01);
	LCD_WR_DATA(0x3f);
	LCD_WR_REG(0x2A);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0xef);
	LCD_WR_REG(0x11); //Exit Sleep
	delay_ms(120);
	LCD_WR_REG(0x29); //display on


	LCD_SetParam();//����LCD����
	LCD_LED=1;//��������
	//LCD_Clear(WHITE);
}

/*************************************************
��������LCD_SetWindows
���ܣ�����lcd��ʾ���ڣ��ڴ�����д�������Զ�����
��ڲ�����xy�����յ�
����ֵ����
*************************************************/
void LCD_SetWindows(u16 xStar, u16 yStar,u16 xEnd,u16 yEnd)
{
	LCD_WR_REG(lcddev.setxcmd);
	LCD_WR_DATA(xStar>>8);
	LCD_WR_DATA(0x00FF&xStar);
	LCD_WR_DATA(xEnd>>8);
	LCD_WR_DATA(0x00FF&xEnd);

	LCD_WR_REG(lcddev.setycmd);
	LCD_WR_DATA(yStar>>8);
	LCD_WR_DATA(0x00FF&yStar);
	LCD_WR_DATA(yEnd>>8);
	LCD_WR_DATA(0x00FF&yEnd);

	LCD_WriteRAM_Prepare();	//��ʼд��GRAM
}

/*************************************************
��������LCD_SetCursor
���ܣ����ù��λ��
��ڲ�����xy����
����ֵ����
*************************************************/
void LCD_SetCursor(u16 Xpos, u16 Ypos)
{
	LCD_WR_REG(lcddev.setxcmd);
	LCD_WR_DATA(Xpos>>8);
	LCD_WR_DATA(0x00FF&Xpos);


	LCD_WR_REG(lcddev.setycmd);
	LCD_WR_DATA(Ypos>>8);
	LCD_WR_DATA(0x00FF&Ypos);

	LCD_WriteRAM_Prepare();	//��ʼд��GRAM
}

//����LCD����
//������к�����ģʽ�л�
void LCD_SetParam(void)
{
	lcddev.wramcmd=0x2C;
#if USE_HORIZONTAL==1	//ʹ�ú���
	lcddev.dir=1;//����
	lcddev.width=320;
	lcddev.height=240;
	lcddev.setxcmd=0x2A;
	lcddev.setycmd=0x2B;
	LCD_WriteReg(0x36,0x6C);

#else//����
	lcddev.dir=0;//����
	lcddev.width=240;
	lcddev.height=320;
	lcddev.setxcmd=0x2A;
	lcddev.setycmd=0x2B;
	LCD_WriteReg(0x36,0xC9);
#endif
}



void LCD_Set_Window(u16 sx,u16 sy,u16 width,u16 height)
{

	LCD_WR_REG(0x2A);
	LCD_WR_DATA(sx>>8);
	LCD_WR_DATA(0x00FF&sx);
	LCD_WR_DATA((sx+width)>>8);
	LCD_WR_DATA(0x00FF&(sx+width));

	LCD_WR_REG(0x2B);
	LCD_WR_DATA(sy>>8);
	LCD_WR_DATA(0x00FF&sy);
	LCD_WR_DATA((sy+height)>>8);
	LCD_WR_DATA(0x00FF&(sy+height));

	LCD_WR_REG(0x2C);	//��ʼд��GRAM
}

void LCD_Drawbmp16(u16 x,u16 y,u16 width,u16 length,const unsigned char *p)
{
   int i;
   unsigned char picH,picL;
   LCD_SetWindows(x,y,x+width-1,y+length-1);//��������
     for(i=0;i<width*length;i++)
   {
     picL=*(p+i*2);  //���ݵ�λ��ǰ
     picH=*(p+i*2+1);
     LCD_DrawPoint_16Bit(picH<<8|picL);
   }
   LCD_SetWindows(0,0,lcddev.width-1,lcddev.height-1);//�ָ���ʾ����Ϊȫ��

 }
void LCD_DrawBinImg(u16 x,u16 y,u16 width,u16 length,u8 threshold,const unsigned char *p)
{
  int i;
  LCD_SetWindows(x,y,x+width-1,y+length-1);//��������
  for(i=0;i<width*length;i++)
  {
//    LCD_DrawPoint_16Bit(YUV_to_Bin(*(p+i),threshold));ColorCvt
    LCD_DrawPoint_16Bit(ColorCvt(*(p+i)));
  }
}

void LCD_DrawBinImg_8bit(u16 x,u16 y,u16 width,u16 length,const unsigned char *p)
{
  int i;
  u16 temp;
  LCD_SetWindows(x,y,x+width-1,y+length-1);//��������
  for(i=0;i<width*length;i++)
  {
    temp = 0 ;
    temp=*(p+i);
    LCD_DrawPoint_16Bit((temp<<8 | temp));
  }
}

void LCD_DrawYVU8Img(u16 x,u16 y,u16 width,u16 length,const unsigned char *p)
{
   int i;
//    unsigned char picH,picL;
   LCD_SetWindows(x,y,x+width-1,y+length-1);//��������
   for(i=0;i<width*length;i++)
   {
//      picL=*(p+i*2);  //���ݵ�λ��ǰ
//      picH=*(p+i*2+1);
     LCD_DrawPoint_16Bit(yuv_8_to_RGBGray(*(p+i)));
   }
//    LCD_SetWindows(0,0,lcddev.width-1,lcddev.height-1);//�ָ���ʾ����Ϊȫ��

 }


 //******************************************************************
 //��������  gui_circle
 //���ߣ�    xiao��@ȫ������
 //���ڣ�    2013-02-22
 //���ܣ�    ��ָ��λ�û�һ��ָ����С��Բ(���)
 //���������(xc,yc) :Բ��������
 //          c:������ɫ
 //      r:Բ�뾶
 //      fill:����жϱ�־��1-��䣬0-�����
 //����ֵ��  ��
 //�޸ļ�¼����
 //******************************************************************
 void gui_circle(int xc, int yc,u16 c,int r, int fill)
 {
   int x = 0, y = r, yi, d;

   d = 3 - 2 * r;


   if (fill)
   {
     // �����䣨��ʵ��Բ��
     while (x <= y) {
       for (yi = x; yi <= y; yi++)
         _draw_circle_8(xc, yc, x, yi, c);

       if (d < 0) {
         d = d + 4 * x + 6;
       } else {
         d = d + 4 * (x - y) + 10;
         y--;
       }
       x++;
     }
   } else
   {
     // �������䣨������Բ��
     while (x <= y) {
       _draw_circle_8(xc, yc, x, y, c);
       if (d < 0) {
         d = d + 4 * x + 6;
       } else {
         d = d + 4 * (x - y) + 10;
         y--;
       }
       x++;
     }
   }
 }

 //******************************************************************
 //��������  LCD_ShowString
 //���ߣ�    xiao��@ȫ������
 //���ڣ�    2013-02-22
 //���ܣ�    ��ʾӢ���ַ���
 //���������x,y :�������
 //      size:�����С
 //      *p:�ַ�����ʼ��ַ
 //      mode:ģʽ 0,���ģʽ;1,����ģʽ
 //����ֵ��  ��
 //�޸ļ�¼����
 //******************************************************************
 void LCD_ShowString(u16 x,u16 y,u8 size,u8 *p,u8 mode)
 {
     while((*p<='~')&&(*p>=' '))//�ж��ǲ��ǷǷ��ַ�!
     {
     if(x>(lcddev.width-1)||y>(lcddev.height-1))
     return;
         LCD_ShowChar(x,y,POINT_COLOR,BACK_COLOR,*p,size,mode);
         x+=size/2;
         p++;
     }
 }

 //******************************************************************
 //��������  LCD_ShowNum
 //���ߣ�    xiao��@ȫ������
 //���ڣ�    2013-02-22
 //���ܣ�    ��ʾ�������ֱ���ֵ
 //���������x,y :�������
 //      len :ָ����ʾ���ֵ�λ��
 //      size:�����С(12,16)
 //      color:��ɫ
 //      num:��ֵ(0~4294967295)
 //����ֵ��  ��
 //�޸ļ�¼����
 //******************************************************************
 void LCD_ShowNum(u16 x,u16 y,u32 num,u8 len,u8 size)
 {
   u8 t,temp;
   u8 enshow=0;
   for(t=0;t<len;t++)
   {
     temp=(num/mypow(10,len-t-1))%10;
     if(enshow==0&&t<(len-1))
     {
       if(temp==0)
       {
         LCD_ShowChar(x+(size/2)*t,y,POINT_COLOR,BACK_COLOR,' ',size,0);
         continue;
       }else enshow=1;

     }
     LCD_ShowChar(x+(size/2)*t,y,POINT_COLOR,BACK_COLOR,temp+'0',size,0);
   }
 }

 //******************************************************************
 //��������  LCD_ShowChar
 //���ߣ�    xiao��@ȫ������
 //���ڣ�    2013-02-22
 //���ܣ�    ��ʾ����Ӣ���ַ�
 //���������(x,y):�ַ���ʾλ����ʼ����
 //          fc:ǰ�û�����ɫ
 //      bc:������ɫ
 //      num:��ֵ��0-94��
 //      size:�����С
 //      mode:ģʽ  0,���ģʽ;1,����ģʽ
 //����ֵ��  ��
 //�޸ļ�¼����
 //******************************************************************
 void LCD_ShowChar(u16 x,u16 y,u16 fc, u16 bc, u8 num,u8 size,u8 mode)
 {
     u8 temp;
     u8 pos,t;
   u16 colortemp=POINT_COLOR;

   num=num-' ';//�õ�ƫ�ƺ��ֵ
   LCD_SetWindows(x,y,x+size/2-1,y+size-1);//���õ���������ʾ����
   if(!mode) //�ǵ��ӷ�ʽ
   {

     for(pos=0;pos<size;pos++)
     {
       if(size==12)temp=asc2_1206[num][pos];//����1206����
       else temp=asc2_1608[num][pos];     //����1608����
       for(t=0;t<size/2;t++)
         {
             if(temp&0x01)LCD_DrawPoint_16Bit(fc);
         else LCD_DrawPoint_16Bit(bc);
         temp>>=1;

         }

     }
   }else//���ӷ�ʽ
   {
     for(pos=0;pos<size;pos++)
     {
       if(size==12)temp=asc2_1206[num][pos];//����1206����
       else temp=asc2_1608[num][pos];     //����1608����
       for(t=0;t<size/2;t++)
         {
         POINT_COLOR=fc;
             if(temp&0x01)LCD_DrawPoint(x+t,y+pos);//��һ����
             temp>>=1;
         }
     }
   }
   POINT_COLOR=colortemp;
   LCD_SetWindows(0,0,lcddev.width-1,lcddev.height-1);//�ָ�����Ϊȫ��
 }

 //******************************************************************
 //��������  mypow
 //���ߣ�    xiao��@ȫ������
 //���ڣ�    2013-02-22
 //���ܣ�    ��m��n�η�(gui�ڲ�����)
 //���������m:����
 //          n:��
 //����ֵ��  m��n�η�
 //�޸ļ�¼����
 //******************************************************************
 u32 mypow(u8 m,u8 n)
 {
   u32 result=1;
   while(n--)result*=m;
   return result;
 }

 //******************************************************************
 //��������  _draw_circle_8
 //���ߣ�    xiao��@ȫ������
 //���ڣ�    2013-02-22
 //���ܣ�    8�Գ��Ի�Բ�㷨(�ڲ�����)
 //���������(xc,yc) :Բ��������
 //      (x,y):��������Բ�ĵ�����
 //          c:������ɫ
 //����ֵ��  ��
 //�޸ļ�¼����
 //******************************************************************
 void _draw_circle_8(int xc, int yc, int x, int y, u16 c)
 {
   GUI_DrawPoint(xc + x, yc + y, c);

   GUI_DrawPoint(xc - x, yc + y, c);

   GUI_DrawPoint(xc + x, yc - y, c);

   GUI_DrawPoint(xc - x, yc - y, c);

   GUI_DrawPoint(xc + y, yc + x, c);

   GUI_DrawPoint(xc - y, yc + x, c);

   GUI_DrawPoint(xc + y, yc - x, c);

   GUI_DrawPoint(xc - y, yc - x, c);
 }
 //////////////////////////////////////////////////////////////////////////////////
 //������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
 //����Ӳ������Ƭ��STM32F103RBT6,��Ƶ72M  ��Ƭ��������ѹ3.3V
 //QDtech-TFTҺ������ for STM32 IOģ��
 //xiao��@ShenZhen QDtech co.,LTD
 //��˾��վ:www.qdtech.net
 //�Ա���վ��http://qdtech.taobao.com
 //��˾�ṩ����֧�֣��κμ������⻶ӭ��ʱ����ѧϰ
 //�̻�(����) :+86 0755-23594567
 //�ֻ�:15989313508���빤��
 //����:QDtech2008@gmail.com
 //Skype:QDtech2008
 //��������QQȺ:324828016
 //��������:2013/5/13
 //�汾��V1.1
 //��Ȩ���У�����ؾ���
 //Copyright(C) ������ȫ�����Ӽ������޹�˾ 2009-2019
 //All rights reserved
 //////////////////////////////////////////////////////////////////////////////////
 //******************************************************************
 //��������  GUI_DrawPoint
 //���ߣ�    xiao��@ȫ������
 //���ڣ�    2013-02-22
 //���ܣ�    GUI���һ����
 //���������x:���λ��x����
 //          y:���λ��y����
 //      color:Ҫ������ɫ
 //����ֵ��  ��
 //�޸ļ�¼����
 //******************************************************************
 void GUI_DrawPoint(u16 x,u16 y,u16 color)
 {
   LCD_SetCursor(x,y);//���ù��λ��
   LCD_DrawPoint_16Bit(color);
 }

 u16 yuv_8_to_RGBGray(u8 yuv422)
 {
   u16 Gray;
   Gray =(((yuv422>>3)<<11)|((yuv422>>2)<<5)|((yuv422>>3)<<0));
   return Gray;
 }

