/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __APP_PLATFORM_H
#define __APP_PLATFORM_H

/*****************************************************************************
*  Program Selector
******************************************************************************/
//----------------------------------------------------------------------------
//  编译控制
#define _SENSOR_OV7670       0
#define _SENSOR_MT9M111      1
#define SENSOR_CHIP          _SENSOR_OV7670
//#define SENSOR_CHIP          _SENSOR_MT9M111


    //源图像格式配置选择(Size, Pixel)
#define _IMGFMT_QVGA_RGB    0
#define _IMGFMT_VGA_RGB     1
#define _IMGFMT_VGA_YUV     2
#define _IMGFMT_SXVGA_RGB   3
#define _IMGFMT_SXVGA_YUV   4
#define _IMGFMT_QuadVGA_RGB     5
#define _IMGFMT_QuadVGA_YUV     6
//#define CAM_IMG_FORMAT      _IMGFMT_VGA_RGB
//#define CAM_IMG_FORMAT      _IMGFMT_VGA_YUV
#define CAM_IMG_FORMAT      _IMGFMT_QVGA_RGB

//#define CAM_IMG_FORMAT      _IMGFMT_QuadVGA_YUV
//#define CAM_IMG_FORMAT      _IMGFMT_QuadVGA_RGB
//#define CAM_IMG_FORMAT      _IMGFMT_SXVGA_RGB
//#define CAM_IMG_FORMAT      _IMGFMT_SXVGA_YUV

    //图像Size(Captured)
#if (CAM_IMG_FORMAT == _IMGFMT_QVGA_RGB)
    #define IMG_WIDTH       320
    #define IMG_HEIGHT      240
#elif ((CAM_IMG_FORMAT == _IMGFMT_VGA_RGB)||(CAM_IMG_FORMAT == _IMGFMT_VGA_YUV))
    #define IMG_WIDTH       640
    #define IMG_HEIGHT      480
#elif ((CAM_IMG_FORMAT == _IMGFMT_QuadVGA_RGB)||(CAM_IMG_FORMAT == _IMGFMT_QuadVGA_YUV))
    #define IMG_WIDTH       1280
    #define IMG_HEIGHT      960
#elif ((CAM_IMG_FORMAT == _IMGFMT_SXVGA_RGB)||(CAM_IMG_FORMAT == _IMGFMT_SXVGA_YUV))
    #define IMG_WIDTH       1280
    #define IMG_HEIGHT      1024
#endif

    //LCD预览显示窗口大小和缩放比例选择。
    //  Default LCD 320x240, 有2:1, 4:1, 5:1三种缩放比例可选择
#define _LCD_PREVIEW_SCALE  2
#define _PREVIEW_WIDTH      (IMG_WIDTH/_LCD_PREVIEW_SCALE)
#define _PREVIEW_HEIGHT     (IMG_HEIGHT/_LCD_PREVIEW_SCALE)

#if (_LCD_PREVIEW_SCALE == 5)
    #define PLD_LCDSCALE    PLDMSK_LCDSCALE5
#elif (_LCD_PREVIEW_SCALE == 4)
    #define PLD_LCDSCALE    PLDMSK_LCDSCALE4
#else
    #define PLD_LCDSCALE    PLDMSK_LCDSCALE2
#endif

    //uvc-MJPEG图像缩小比例，不同于LCD Preview Scale
#define IMG_VIDEO_SCALE         1           //MJPEG size=Img_size/scale
        //uvc-MJPEG帧率
#define IMG_MJPG_FRAMERATE      5           //预定义MJPEG视频帧率

//----------------------------------------------------------------------------
/* Public typedef -----------------------------------------------------------*/
typedef unsigned char    Bool;
#define False   FALSE
#define True    TRUE

/* Public define ------------------------------------------------------------*/
#define RXD_BUFFERSIZE      0x200
#define TXD_BUFFERSIZE      0x200

/* Public macro -------------------------------------------------------------*/
#define M_USART     USART1

    //Port-Pins
#define PORT_USB_EN     GPIOA
#define PIN_USB_EN      (u16)(GPIO_Pin_15)

    //LED
#define PORT_LED    GPIOE
#define PIN_LED0    (u16)(GPIO_Pin_2)
#define PIN_LED1    (u16)(GPIO_Pin_3)
#define PIN_LED2    (u16)(GPIO_Pin_4)
#define PIN_LED3    (u16)(GPIO_Pin_5)
#define LED0_H()    GPIOE->BSRR = (u16)(GPIO_Pin_2)
#define LED0_L()    GPIOE->BRR = (u16)(GPIO_Pin_2)
#define LED1_H()    GPIOE->BSRR = (u16)(GPIO_Pin_3)
#define LED1_L()    GPIOE->BRR = (u16)(GPIO_Pin_3)
#define LED2_H()    GPIOE->BSRR = (u16)(GPIO_Pin_4)
#define LED2_L()    GPIOE->BRR = (u16)(GPIO_Pin_4)
#define LED3_H()    GPIOE->BSRR = (u16)(GPIO_Pin_5)
#define LED3_L()    GPIOE->BRR = (u16)(GPIO_Pin_5)
//efine LED_TOGGLE(ledpin)      {   PORT_LED->ODR ^= (u16)(ledpin);   }
#define LED_TOGGLE(ledpin)        GPIO_WriteBit(PORT_LED, (ledpin),     \
    (BitAction)((1-GPIO_ReadOutputDataBit(PORT_LED, (ledpin)))));

    //Setup Reserved
#define PORT_SETUP0     GPIOE
#define PIN_SETUP0     (u16)(GPIO_Pin_6)
#define PORT_SETUP1     GPIOB
#define PIN_SETUP1     (u16)(GPIO_Pin_9)

    //  PortB: PB2~PB6 reserved
#define AUXPORT_PB     GPIOB
    //  PortD: PD11~PD13 reserved
#define AUXPORT_PD     GPIOD
    //  PortC: PC13~PC15 reserved
#define AUXPORT_PC     GPIOC

    //LCD Control
#define PORT_LCD_DAT    GPIOC               //0..7 8bit

#define PORT_LCD_CTRL   GPIOB
#define PIN_LCD_BL      GPIO_Pin_15
#define PIN_LCD_CS      GPIO_Pin_14
#define PIN_LCD_RS      GPIO_Pin_13
#define PIN_LCD_RD      GPIO_Pin_12
#define PIN_LCD_WRS     GPIO_Pin_8          //外部线与，双向切换或OD输出
#define PIN_LCD_RST      GPIO_Pin_2

#define LCD_BL_H()      GPIOB->BSRR = PIN_LCD_BL
#define LCD_BL_L()      GPIOB->BRR = PIN_LCD_BL
#define LCD_CS_H()      GPIOB->BSRR = PIN_LCD_CS
#define LCD_CS_L()      GPIOB->BRR = PIN_LCD_CS
#define LCD_RS_H()      GPIOB->BSRR = PIN_LCD_RS
#define LCD_RS_L()      GPIOB->BRR = PIN_LCD_RS
#define LCD_RD_H()      GPIOB->BSRR = PIN_LCD_RD
#define LCD_RD_L()      GPIOB->BRR = PIN_LCD_RD
#define LCD_RST_H()     GPIOB->BSRR = PIN_LCD_RST
#define LCD_RST_L()     GPIOB->BRR = PIN_LCD_RST

#define LCD_WR_H()      GPIOB->BSRR = PIN_LCD_WRS
#define LCD_WR_L()      GPIOB->BRR = PIN_LCD_WRS

    //LCD ADC reserved
#define PORT_LCD_ADC01   GPIOB
#define PIN_LCD_ADC0    GPIO_Pin_0
#define PIN_LCD_ADC1    GPIO_Pin_1

#define PORT_LCD_ADC23   GPIOA
#define PIN_LCD_ADC2    GPIO_Pin_2
#define PIN_LCD_ADC3    GPIO_Pin_3

    //Camera Port
#define PORT_CAMCTRL    GPIOA
#define PIN_CAM_VSYN    (u16)(GPIO_Pin_0)       //Input
#define PIN_CAM_HREF    (u16)(GPIO_Pin_1)
#define PIN_CAM_NRST    (u16)(GPIO_Pin_8)       //Output, 保留

#define EXTI_LINE_VSYNC             EXTI_Line0
#define GPIO_PORTSOURCE_VSYNC       GPIO_PortSourceGPIOA
#define GPIO_PINSOURCE_VSYNC        GPIO_PinSource0

#define EXTI_LINE_HREF              EXTI_Line1
#define GPIO_PORTSOURCE_HREF        GPIO_PortSourceGPIOA
#define GPIO_PINSOURCE_HREF         GPIO_PinSource1

    //I2C Bus (soft-I2C)
#define SCCB_GPIO_PORT  GPIOB
#define SCCB_SIC_BIT    GPIO_Pin_10
#define SCCB_SID_BIT    GPIO_Pin_11

#define SCCB_SIC_H()    GPIOB->BSRR = SCCB_SIC_BIT;
#define SCCB_SIC_L()    GPIOB->BRR =  SCCB_SIC_BIT;
#define SCCB_SID_H()    GPIOB->BSRR = SCCB_SID_BIT;
#define SCCB_SID_L()    GPIOB->BRR =  SCCB_SID_BIT;

#define SCCB_SID_IN     SCCB_SID_GPIO_INPUT();
#define SCCB_SID_OUT    SCCB_SID_GPIO_OUTPUT();
#define SCCB_SID_STATE	    (GPIOB->IDR & 0x0800)       //Input

//-------------------------------------
//	宏函数
#define delay_ms        Delay_ms
#define delay_us        Delay_us

#define countof(a)   (sizeof(a) / sizeof(*(a)))

#define CLRWDT()        __nop()

/* Public variables ---------------------------------------------------------*/
//  共用变量
extern vu16 SysTickTime;

extern vu8  Rxd_Buffer[];
extern vu8  Txd_Buffer[];
extern vu16 Rxd_RdPtr,Rxd_WrPtr;
extern vu16 Txd_RdPtr,Txd_WrPtr;

extern vu32 cam_FrameNo;

/* Public function prototypes -----------------------------------------------*/
/* Publice functions ---------------------------------------------------------*/
//Application 公用函数
//void IWDG_Config(void);

//void Delay_ms(u16 ms_cnt);
//void Delay_us(u16 us_cnt);


#endif /* __APP_PLATFORM_H */



