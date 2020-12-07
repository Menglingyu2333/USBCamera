//#include "sys.h"
#include "ov2640.h"
#include "ov2640cfg.h"
#include "delay.h"
//#include "timer.h"
//#include "delay.h"
//#include "usart.h"
//#include "sccb.h"


//////////////////////////////////////////////////////////////////////////////////
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F103������
//OV2640 ��������
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2015/4/16
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved
//////////////////////////////////////////////////////////////////////////////////
//void JTAG_Set(uint8_t mode)
//{
//	uint32_t temp;
//	temp=mode;
//	temp<<=25;
//	RCC->APB2ENR|=1<<0;     //��������ʱ��
//	AFIO->MAPR&=0XF8FFFFFF; //���MAPR��[26:24]
//	AFIO->MAPR|=temp;       //����jtagģʽ
//}
#define SWD_ENABLE         0X01

//u8 ImageBufffer[ImageWidth * ImageHeight];
//u8 *ImageBuf = &ImageBufffer[0];;
u16 ImagePtr_W, ImagePtr_H;
u8 YSYNC_FLAG;
u8 HREF_FLAG;
u8 PCLK_FLAG;
u8 ImageReadyFlag = 0;
uint32_t jpeglen = 0;
uint32_t jpegValidLen = 0;

//��ʼ��OV2640
//�������Ժ�,Ĭ�������1600*1200�ߴ��ͼƬ!!
//����ֵ:0,�ɹ�
//    ����,�������
uint8_t OV2640_Init(void)
{
	uint16_t i=0;
	uint16_t reg;
	GPIO_InitType  GPIO_InitStructure;
	//����IO
	RCC_APB2PeriphClockCmd(RCC_APB2PERIPH_GPIOB|RCC_APB2PERIPH_GPIOC|RCC_APB2PERIPH_GPIOD|RCC_APB2PERIPH_AFIO, ENABLE);	 //ʹ����ض˿�ʱ��

	GPIO_InitStructure.GPIO_Pins  = GPIO_Pins_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_PU;//PD6����
 	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_SetBits(GPIOA,GPIO_Pins_1);

  GPIO_InitStructure.GPIO_Pins  = GPIO_Pins_2; //PC8����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_PU;
 	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_SetBits(GPIOA,GPIO_Pins_2);

	GPIO_InitStructure.GPIO_Pins  = GPIO_Pins_3; 	//PB4 ���� ����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_PU;
	GPIO_InitStructure.GPIO_MaxSpeed = GPIO_MaxSpeed_50MHz;
 	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_SetBits(GPIOA,GPIO_Pins_3);

	GPIO_InitStructure.GPIO_Pins  = GPIO_Pins_7; 	//PB3 ���
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT_PP;
 	GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_SetBits(GPIOC,GPIO_Pins_7);


	GPIO_InitStructure.GPIO_Pins  = 0xff; //PC0~7 ���� ����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_PU;
	GPIO_InitStructure.GPIO_MaxSpeed = GPIO_MaxSpeed_50MHz;
 	GPIO_Init(GPIOD, &GPIO_InitStructure);






	GPIO_InitStructure.GPIO_Pins  = GPIO_Pins_15; //PB15���
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT_PP;
 	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_SetBits(GPIOB,GPIO_Pins_15);


//    GPIO_PinRemapConfi(GPIO_Remap_SWJ_JTAGDisable,ENABLE);

 	OV2640_PWDN=0;				//POWER ON
//	Delay_ms(20);
	OV2640_RST=0;				//��λOV2640
	Delay_ms(20);
	OV2640_RST=1;				//������λ
  	SCCB_Init();        		//��ʼ��SCCB ��IO��
	SCCB_WR_Reg(OV2640_DSP_RA_DLMT, 0x01);	//����sensor�Ĵ���
 	SCCB_WR_Reg(OV2640_SENSOR_COM7, 0x80);	//��λOV2640
	Delay_ms(100);
	reg=SCCB_RD_Reg(OV2640_SENSOR_MIDH);	//��ȡ����ID �߰�λ
	reg<<=8;
	reg|=SCCB_RD_Reg(OV2640_SENSOR_MIDL);	//��ȡ����ID �Ͱ�λ
	if(reg!=OV2640_MID)
	{
		//rt_kprintf("MID:%d\r\n",reg);
		return 1;
	}
	reg=SCCB_RD_Reg(OV2640_SENSOR_PIDH);	//��ȡ����ID �߰�λ
	reg<<=8;
	reg|=SCCB_RD_Reg(OV2640_SENSOR_PIDL);	//��ȡ����ID �Ͱ�λ
	if(reg!=OV2640_PID)
	{
//		rt_kprintf("HID:%d\r\n",reg);
		return 2;
	}
 	//��ʼ�� OV2640,����SXGA�ֱ���(1600*1200)
	for(i=0;i<sizeof(ov2640_uxga_init_reg_tbl)/2;i++)
	{
	   	SCCB_WR_Reg(ov2640_uxga_init_reg_tbl[i][0],ov2640_uxga_init_reg_tbl[i][1]);
 	}

  Delay_ms(500);
//  OV2640_YUV422_Mode();
//  OV2640_RGB565_Mode();

//  OV2640_Window_Set(0,0,240,160);
//  OV2640_ImageSize_Set(240,160);
//  OV2640_ImageWin_Set((OV2640_TOTAL_WIDTH-ImageWidth)/2,
//                      (OV2640_TOTAL_HEIGHT-ImageHeight)/2,
//                       ImageWidth,ImageHeight);
  OV2640_JPEG_Mode();
  OV2640_OutSize_Set(ImageWidth,ImageHeight);
  ov2640_speed_ctrl();

//	for(i = 0; i < 10; i++) //����10֡���ȴ�OV2640�Զ����ںã��ع��ƽ��֮��ģ�
//	{
//		while(OV2640_VSYNC == 1);
//		while(OV2640_VSYNC == 0);
//	}

	return 0x00; 	//ok
}
//OV2640�л�ΪJPEGģʽ
void OV2640_JPEG_Mode(void)
{
	uint16_t i=0;
	//����:YUV422��ʽ
	for(i=0;i<(sizeof(ov2640_yuv422_reg_tbl)/2);i++)
	{
		SCCB_WR_Reg(ov2640_yuv422_reg_tbl[i][0],ov2640_yuv422_reg_tbl[i][1]);
	}
	//����:���JPEG����
	for(i=0;i<(sizeof(ov2640_jpeg_reg_tbl)/2);i++)
	{
		SCCB_WR_Reg(ov2640_jpeg_reg_tbl[i][0],ov2640_jpeg_reg_tbl[i][1]);
	}
}
//OV2640�л�ΪRGB565ģʽ
void OV2640_RGB565_Mode(void)
{
	uint16_t i=0;
	//����:RGB565���
	for(i=0;i<(sizeof(ov2640_rgb565_reg_tbl)/2);i++)
	{
		SCCB_WR_Reg(ov2640_rgb565_reg_tbl[i][0],ov2640_rgb565_reg_tbl[i][1]);
	}
}
void OV2640_YUV422_Mode(void)
{
	SCCB_WR_Reg(0xFF, 0x00);
	SCCB_WR_Reg(0xDA, 0x01);
}
//�Զ��ع����ò�����,֧��5���ȼ�
const static uint8_t OV2640_AUTOEXPOSURE_LEVEL[5][8]=
{
	{
		0xFF,0x01,
		0x24,0x20,
		0x25,0x18,
		0x26,0x60,
	},
	{
		0xFF,0x01,
		0x24,0x34,
		0x25,0x1c,
		0x26,0x00,
	},
	{
		0xFF,0x01,
		0x24,0x3e,
		0x25,0x38,
		0x26,0x81,
	},
	{
		0xFF,0x01,
		0x24,0x48,
		0x25,0x40,
		0x26,0x81,
	},
	{
		0xFF,0x01,
		0x24,0x58,
		0x25,0x50,
		0x26,0x92,
	},
};
//OV2640�Զ��ع�ȼ�����
//level:0~4
void OV2640_Auto_Exposure(uint8_t level)
{
	uint8_t i;
	uint8_t *p=(uint8_t*)OV2640_AUTOEXPOSURE_LEVEL[level];
	for(i=0;i<4;i++)
	{
		SCCB_WR_Reg(p[i*2],p[i*2+1]);
	}
}
//��ƽ������
//0:�Զ�
//1:̫��sunny
//2,����cloudy
//3,�칫��office
//4,����home
void OV2640_Light_Mode(uint8_t mode)
{
	uint8_t regccval=0X5E;//Sunny
	uint8_t regcdval=0X41;
	uint8_t regceval=0X54;
	switch(mode)
	{
		case 0://auto
			SCCB_WR_Reg(0XFF,0X00);
			SCCB_WR_Reg(0XC7,0X00);//AWB ON
			return;
		case 2://cloudy
			regccval=0X65;
			regcdval=0X41;
			regceval=0X4F;
			break;
		case 3://office
			regccval=0X52;
			regcdval=0X41;
			regceval=0X66;
			break;
		case 4://home
			regccval=0X42;
			regcdval=0X3F;
			regceval=0X71;
			break;
	}
	SCCB_WR_Reg(0XFF,0X00);
	SCCB_WR_Reg(0XC7,0X40);	//AWB OFF
	SCCB_WR_Reg(0XCC,regccval);
	SCCB_WR_Reg(0XCD,regcdval);
	SCCB_WR_Reg(0XCE,regceval);
}
//ɫ������
//0:-2
//1:-1
//2,0
//3,+1
//4,+2
void OV2640_Color_Saturation(uint8_t sat)
{
	uint8_t reg7dval=((sat+2)<<4)|0X08;
	SCCB_WR_Reg(0XFF,0X00);
	SCCB_WR_Reg(0X7C,0X00);
	SCCB_WR_Reg(0X7D,0X02);
	SCCB_WR_Reg(0X7C,0X03);
	SCCB_WR_Reg(0X7D,reg7dval);
	SCCB_WR_Reg(0X7D,reg7dval);
}
//��������
//0:(0X00)-2
//1:(0X10)-1
//2,(0X20) 0
//3,(0X30)+1
//4,(0X40)+2
void OV2640_Brightness(uint8_t bright)
{
  SCCB_WR_Reg(0xff, 0x00);
  SCCB_WR_Reg(0x7c, 0x00);
  SCCB_WR_Reg(0x7d, 0x04);
  SCCB_WR_Reg(0x7c, 0x09);
  SCCB_WR_Reg(0x7d, bright<<4);
  SCCB_WR_Reg(0x7d, 0x00);
}
//�Աȶ�����
//0:-2
//1:-1
//2,0
//3,+1
//4,+2
void OV2640_Contrast(uint8_t contrast)
{
	uint8_t reg7d0val=0X20;//Ĭ��Ϊ��ͨģʽ
	uint8_t reg7d1val=0X20;
  	switch(contrast)
	{
		case 0://-2
			reg7d0val=0X18;
			reg7d1val=0X34;
			break;
		case 1://-1
			reg7d0val=0X1C;
			reg7d1val=0X2A;
			break;
		case 3://1
			reg7d0val=0X24;
			reg7d1val=0X16;
			break;
		case 4://2
			reg7d0val=0X28;
			reg7d1val=0X0C;
			break;
	}
	SCCB_WR_Reg(0xff,0x00);
	SCCB_WR_Reg(0x7c,0x00);
	SCCB_WR_Reg(0x7d,0x04);
	SCCB_WR_Reg(0x7c,0x07);
	SCCB_WR_Reg(0x7d,0x20);
	SCCB_WR_Reg(0x7d,reg7d0val);
	SCCB_WR_Reg(0x7d,reg7d1val);
	SCCB_WR_Reg(0x7d,0x06);
}
//��Ч����
//0:��ͨģʽ
//1,��Ƭ
//2,�ڰ�
//3,ƫ��ɫ
//4,ƫ��ɫ
//5,ƫ��ɫ
//6,����
void OV2640_Special_Effects(uint8_t eft)
{
	uint8_t reg7d0val=0X00;//Ĭ��Ϊ��ͨģʽ
	uint8_t reg7d1val=0X80;
	uint8_t reg7d2val=0X80;
	switch(eft)
	{
		case 1://��Ƭ
			reg7d0val=0X40;
			break;
		case 2://�ڰ�
			reg7d0val=0X18;
			break;
		case 3://ƫ��ɫ
			reg7d0val=0X18;
			reg7d1val=0X40;
			reg7d2val=0XC0;
			break;
		case 4://ƫ��ɫ
			reg7d0val=0X18;
			reg7d1val=0X40;
			reg7d2val=0X40;
			break;
		case 5://ƫ��ɫ
			reg7d0val=0X18;
			reg7d1val=0XA0;
			reg7d2val=0X40;
			break;
		case 6://����
			reg7d0val=0X18;
			reg7d1val=0X40;
			reg7d2val=0XA6;
			break;
	}
	SCCB_WR_Reg(0xff,0x00);
	SCCB_WR_Reg(0x7c,0x00);
	SCCB_WR_Reg(0x7d,reg7d0val);
	SCCB_WR_Reg(0x7c,0x05);
	SCCB_WR_Reg(0x7d,reg7d1val);
	SCCB_WR_Reg(0x7d,reg7d2val);
}
//��������
//sw:0,�رղ���
//   1,��������(ע��OV2640�Ĳ����ǵ�����ͼ�������)
void OV2640_Color_Bar(uint8_t sw)
{
	uint8_t reg;
	SCCB_WR_Reg(0XFF,0X01);
	reg=SCCB_RD_Reg(0X12);
	reg&=~(1<<1);
	if(sw)reg|=1<<1;
	SCCB_WR_Reg(0X12,reg);
}
//���ô������������
//sx,sy,��ʼ��ַ
//width,height:���(��Ӧ:horizontal)�͸߶�(��Ӧ:vertical)
void OV2640_Window_Set(uint16_t sx,uint16_t sy,uint16_t width,uint16_t height)
{
	uint16_t endx;
	uint16_t endy;
	uint8_t temp;
	endx=sx+width/2;	//V*2
 	endy=sy+height/2;

	SCCB_WR_Reg(0XFF,0X01);
	temp=SCCB_RD_Reg(0X03);				//��ȡVref֮ǰ��ֵ
	temp&=0XF0;
	temp|=((endy&0X03)<<2)|(sy&0X03);
	SCCB_WR_Reg(0X03,temp);				//����Vref��start��end�����2λ
	SCCB_WR_Reg(0X19,sy>>2);			//����Vref��start��8λ
	SCCB_WR_Reg(0X1A,endy>>2);			//����Vref��end�ĸ�8λ

	temp=SCCB_RD_Reg(0X32);				//��ȡHref֮ǰ��ֵ
	temp&=0XC0;
	temp|=((endx&0X07)<<3)|(sx&0X07);
	SCCB_WR_Reg(0X32,temp);				//����Href��start��end�����3λ
	SCCB_WR_Reg(0X17,sx>>3);			//����Href��start��8λ
	SCCB_WR_Reg(0X18,endx>>3);			//����Href��end�ĸ�8λ
}
//����ͼ�������С
//OV2640���ͼ��Ĵ�С(�ֱ���),��ȫ�ɸú���ȷ��
//width,height:���(��Ӧ:horizontal)�͸߶�(��Ӧ:vertical),width��height������4�ı���
//����ֵ:0,���óɹ�
//    ����,����ʧ��
uint8_t OV2640_OutSize_Set(uint16_t width,uint16_t height)
{
	uint16_t outh;
	uint16_t outw;
	uint8_t temp;
	if(width%4)return 1;
	if(height%4)return 2;
	outw=width/4;
	outh=height/4;
	SCCB_WR_Reg(0XFF,0X00);
	SCCB_WR_Reg(0XE0,0X04);
	SCCB_WR_Reg(0X50,0X89);
	SCCB_WR_Reg(0X5A,outw&0XFF);		//����OUTW�ĵͰ�λ
	SCCB_WR_Reg(0X5B,outh&0XFF);		//����OUTH�ĵͰ�λ
	temp=(outw>>8)&0X03;
	temp|=(outh>>6)&0X04;
	SCCB_WR_Reg(0X5C,temp);				//����OUTH/OUTW�ĸ�λ
	SCCB_WR_Reg(0XE0,0X00);
	return 0;
}
//����ͼ�񿪴���С
//��:OV2640_ImageSize_Setȷ������������ֱ��ʴӴ�С.
//�ú������������Χ������п���,����OV2640_OutSize_Set�����
//ע��:�������Ŀ�Ⱥ͸߶�,������ڵ���OV2640_OutSize_Set�����Ŀ�Ⱥ͸߶�
//     OV2640_OutSize_Set���õĿ�Ⱥ͸߶�,���ݱ��������õĿ�Ⱥ͸߶�,��DSP
//     �Զ��������ű���,������ⲿ�豸.
//width,height:���(��Ӧ:horizontal)�͸߶�(��Ӧ:vertical),width��height������4�ı���
//����ֵ:0,���óɹ�
//    ����,����ʧ��
uint8_t OV2640_ImageWin_Set(uint16_t offx,uint16_t offy,uint16_t width,uint16_t height)
{
	uint16_t hsize;
	uint16_t vsize;
	uint8_t temp;
	if(width%4)return 1;
	if(height%4)return 2;
	hsize=width/4;
	vsize=height/4;
	SCCB_WR_Reg(0XFF,0X00);
	SCCB_WR_Reg(0XE0,0X04);
	SCCB_WR_Reg(0X51,hsize&0XFF);		//����H_SIZE�ĵͰ�λ
	SCCB_WR_Reg(0X52,vsize&0XFF);		//����V_SIZE�ĵͰ�λ
	SCCB_WR_Reg(0X53,offx&0XFF);		//����offx�ĵͰ�λ
	SCCB_WR_Reg(0X54,offy&0XFF);		//����offy�ĵͰ�λ
	temp=(vsize>>1)&0X80;
	temp|=(offy>>4)&0X70;
	temp|=(hsize>>5)&0X08;
	temp|=(offx>>8)&0X07;
	SCCB_WR_Reg(0X55,temp);				//����H_SIZE/V_SIZE/OFFX,OFFY�ĸ�λ
	SCCB_WR_Reg(0X57,(hsize>>2)&0X80);	//����H_SIZE/V_SIZE/OFFX,OFFY�ĸ�λ
	SCCB_WR_Reg(0XE0,0X00);
	return 0;
}
//�ú�������ͼ��ߴ��С,Ҳ������ѡ��ʽ������ֱ���
//UXGA:1600*1200,SVGA:800*600,CIF:352*288
//width,height:ͼ���Ⱥ�ͼ��߶�
//����ֵ:0,���óɹ�
//    ����,����ʧ��
uint8_t OV2640_ImageSize_Set(uint16_t width,uint16_t height)
{
	uint8_t temp;
	SCCB_WR_Reg(0XFF,0X00);
	SCCB_WR_Reg(0XE0,0X04);
	SCCB_WR_Reg(0XC0,(width)>>3&0XFF);		//����HSIZE��10:3λ
	SCCB_WR_Reg(0XC1,(height)>>3&0XFF);		//����VSIZE��10:3λ
	temp=(width&0X07)<<3;
	temp|=height&0X07;
	temp|=(width>>4)&0X80;
	SCCB_WR_Reg(0X8C,temp);
	SCCB_WR_Reg(0XE0,0X00);
	return 0;
}


uint8_t ov2640_framebuf1[20*1024];				//֡����
uint8_t* ov2640_framebuf1_ptr = &ov2640_framebuf1[0];
uint8_t ov2640_framebuf2[20*1024];				//֡����
uint8_t* ov2640_framebuf2_ptr = &ov2640_framebuf2[0];
uint8_t ov2640_framebuf3[20*1024];				//֡����
uint8_t* ov2640_framebuf3_ptr = &ov2640_framebuf3[0];

//extern uint8_t ov_frame;					//��timer.c���涨��
//OV2640�ٶȿ���
//����LCD�ֱ��ʵĲ�ͬ�����ò�ͬ�Ĳ���
void ov2640_speed_ctrl(void)
{
//	uint8_t clkdiv,pclkdiv;			//ʱ�ӷ�Ƶϵ����PCLK��Ƶϵ��
//	clkdiv=3;
//	pclkdiv=15;
//	SCCB_WR_Reg(0XFF,0X00);
//	SCCB_WR_Reg(0XD3,18);	//����PCLK��Ƶ
//	SCCB_WR_Reg(0XFF,0X01);
//	SCCB_WR_Reg(0X11,4);	//����CLK��Ƶ

	SCCB_WR_Reg(0XFF,0X00);
	SCCB_WR_Reg(0XD3,30);
	SCCB_WR_Reg(0XFF,0X01);
	SCCB_WR_Reg(0X11,0X1);

//	SCCB_WR_Reg(0XFF,0X00);
//	SCCB_WR_Reg(0XD3,18);	//����PCLK��Ƶ
//	SCCB_WR_Reg(0XFF,0X01);
//	SCCB_WR_Reg(0X11,4);	//����CLK��Ƶ

//	SCCB_WR_Reg(0XFF,0X00);
//	SCCB_WR_Reg(0XD3,15);	//����PCLK��Ƶ
//	SCCB_WR_Reg(0XFF,0X01);
//	SCCB_WR_Reg(0X11,3);	//����CLK��Ƶ
}
//OV2640����jpgͼƬ
//����ֵ:0,�ɹ�
//    ����,�������


uint8_t ov2640_jpg_photo()
{
	uint8_t res=0;
	uint32_t i=0,t=0,j=0,c;
	uint8_t* pbuf;
	uint32_t Len=0;
	uint8_t s[4];

	OV2640_JPEG_Mode();							//�л�ΪJPEGģʽ
//    OV2640_RGB565_Mode();

  OV2640_OutSize_Set(ImageWidth,ImageHeight);
	SCCB_WR_Reg(0XFF,0X00);
	SCCB_WR_Reg(0XD3,26);
	SCCB_WR_Reg(0XFF,0X01);
	SCCB_WR_Reg(0X11,0X1);
	for(i=0;i<10;i++)		//����10֡���ȴ�OV2640�Զ����ںã��ع��ƽ��֮��ģ�
	{
		while(OV2640_VSYNC==1);
		while(OV2640_VSYNC==0);
	}
//  while(1)
//  {
    jpeglen=0;
    while(OV2640_VSYNC==1)	//��ʼ�ɼ�jpeg����
    {
    	while(OV2640_HREF)
    	{
    		while(OV2640_PCLK==0);
    		ov2640_framebuf1[jpeglen]=OV2640_DATA;
    		while(OV2640_PCLK==1);
    		jpeglen++;
    	}
    }
    	pbuf=(uint8_t*)ov2640_framebuf1;
    	for(i=0;i<jpeglen;i++)//����0XFF,0XD8
    	{
    		if((pbuf[i]==0XFF)&&(pbuf[i+1]==0XD8))//��¼֡ͷλ��
    		{
    			t=i;
    		}
    		if((pbuf[i]==0XFF)&&(pbuf[i+1]==0XD9))//��¼֡βλ��
    		{
    			j=i+1;
    			break;
    		}
    	}
    	Len=j-t+1;
    	if(i==jpeglen)
    	{
    		res=1;//û�ҵ�0XFF,0XD8

    	}
    	else		//�ҵ���
    	{
        jpeglen = Len;
    		pbuf+=t;//ƫ�Ƶ�0XFF,0XD8��
    		s[0]=(uint8_t)(((Len)&0xff000000)>>24);
    		s[1]=(uint8_t)(((Len)&0xff0000)>>16);
    		s[2]=(uint8_t)(((Len)&0xff00)>>8);
    		s[3]=(uint8_t)((Len)&0xff);
    		for(c=0;c<4;c++)      //���ͱ��ĳ���
    		{
    			USART1->DT=s[c];
    			while((USART1->STS&0X40)==0);
    		}
    		SendRAMDate(Len,pbuf);
    	}
//  }
	return res;
}


void SendAChar(uint8_t date)
{
    USART1->DT=date;
	while((USART1->STS&0X40)==0);
}
/*******************************************************************************
*������������SRAM�ж�ȡ���ݲ���������
*������ڣ�*date:ָ�����ݵ�ָ��
*��������:
*����˵��:	,uint32_t Len
*******************************************************************************/
void SendRAMDate(uint32_t Len,unsigned char *strp)
{
    while(Len--)
	{
	SendAChar(*strp++);
	}

}















