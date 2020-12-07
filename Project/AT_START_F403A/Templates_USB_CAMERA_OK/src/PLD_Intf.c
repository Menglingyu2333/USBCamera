/*============================================================================
//  File Name       : Pld_Intf.c
//  Author          : HECC. DuckWeed Tech.
//                  @HuiZhou, 2010 
//  email:  hcc21cn@163.com	
//  Version         :
//  Description     : Main program body
//
==============================================================================*/
/* Includes ------------------------------------------------------------------*/
#include "at32f4xx.h"
#include "Pld_Intf.h"

/* Public variables ---------------------------------------------------------*/

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/

/* Public variables ---------------------------------------------------------*/
    
volatile Bool g_Img_CapEnable = False;      //开启/关闭CPLD自动保存摄像头图像    
volatile Bool LCD_VideoEnable = False;    //开启/关闭Scaler向LCD的图像显示

    //SDRAM分成2页，每页存储一帧图像(Page<->Frame),FSMC读写的帧与PLD正在保存当前帧不同
    //(FSMC读写是前一帧),两者同时进行; PLD在Page-0写完一帧，正常情况下应到Page-1写下一
    //帧，即页交换; Page-1保存着前一帧图像，这会导致此帧被破坏。g_Img_FrameLock设置为1，
    //则不进行页交换，还是在Page-0写，最后一帧图像被破坏，但MCU读写的Page-1内图像保持完整。
volatile Bool g_Img_FrameLock = False;
    
vu16 pld_RegFsmcPage = 0;                   //CPLD内页选择寄存器(Reg-1)影像

/* Private variables ---------------------------------------------------------*/
vu16 pld_RegCtrl = 0;                       //CPLD控制寄存器(Reg-0)影像
vu16 pld_regUpd_req = 0;                   //请求在VSYN中断函数中改写Reg-0，单稳态

    //当前XBUF读写用的全局环境变量
u16 pld_xbuf_rowbase = 0;                   //XBUF行起点
u16 pld_xbuf_colbase = 1024;                //XBUF列起点

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/*******************************************************************************
*******************************************************************************/

/*******************************************************************************
@
@   Part-1:    基础硬件设置
@
*******************************************************************************/

/*******************************************************************************
* Function Name  : 
* Description    : 
* Input          : None 
* Output         : None
* Return         : None
*******************************************************************************/
void CAMERA_Ctrl_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure; 
    EXTI_InitTypeDef EXTI_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
      
#ifdef  VECT_TAB_RAM  
    // Set the Vector Table base location at 0x20000000 
    NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0); 
#else  /* VECT_TAB_FLASH  */
    //Set the Vector Table base location at 0x08000000 
    NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);   
#endif

    //Camera Sync GPIO
    GPIO_InitStructure.GPIO_Pin =  PIN_CAM_HREF | PIN_CAM_VSYN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(PORT_CAMCTRL, &GPIO_InitStructure); 

    //Camera PowerOn Reset        
    GPIO_ResetBits(PORT_CAMCTRL, PIN_CAM_NRST);         //Reset Enable
    GPIO_InitStructure.GPIO_Pin =  PIN_CAM_NRST;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(PORT_CAMCTRL, &GPIO_InitStructure); 
    GPIO_ResetBits(PORT_CAMCTRL, PIN_CAM_NRST);         //Reset Enable

    //EXTI 
        //VSYNC中断：选择PortA的GPIO_Pinx连接到EXTIx中断输入
    GPIO_EXTILineConfig(GPIO_PORTSOURCE_VSYNC, GPIO_PINSOURCE_VSYNC);
    EXTI_InitStructure.EXTI_Line = EXTI_LINE_VSYNC;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;     //下降沿中断, VSYN负极性，场结束
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);
        //HREF中断
    GPIO_EXTILineConfig(GPIO_PORTSOURCE_HREF, GPIO_PINSOURCE_HREF);
    EXTI_InitStructure.EXTI_Line = EXTI_LINE_HREF;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;     //下降沿中断, 行结束
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);
    //NVIC
        //VSYN
    NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQChannel;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
        //HREF
    NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQChannel;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

        //  Enable TIM2 interrupt         
    NVIC_InitStructure.NVIC_IRQChannel=TIM2_IRQChannel;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
#if (DRAM_REFRESH_ADDONS != 0)
    NVIC_Init(&NVIC_InitStructure);
#endif

    //TIM for SDRAM refresh
    TIM_TimeBaseStructure.TIM_Prescaler = 72-1;          //计数时钟1MHz
    TIM_TimeBaseStructure.TIM_Period = 100-1;              //Update周期: 100us
    TIM_TimeBaseStructure.TIM_ClockDivision = 0x00;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

    TIM_ARRPreloadConfig(TIM2,DISABLE);
    TIM_UpdateRequestConfig(TIM2,TIM_UpdateSource_Regular);

    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);

    // TIMx enable counter 
    TIM_ClearFlag(TIM2, TIM_FLAG_Update);
#if (DRAM_REFRESH_ADDONS != 0)
    TIM_Cmd(TIM2, ENABLE);
#endif

//    delay_ms(50);
    GPIO_SetBits(PORT_CAMCTRL, PIN_CAM_NRST);       //PowerOn Reset Disable       
}

/*******************************************************************************
* Function Name  : FSMC_Mem_Init
* Description    : Configures the FSMC and GPIOs to interface with the SRAM memory.
*                  This function must be called before any write/read operation
*                  on the SRAM.
* Input          : None 
* Output         : None
* Return         : None
*******************************************************************************/
void FSMC_Mem_Init(void)
{
    FSMC_NORSRAMInitTypeDef  FSMC_NORSRAMInitStructure;
    FSMC_NORSRAMTimingInitTypeDef  p;
    GPIO_InitTypeDef GPIO_InitStructure; 

    /* Enable the FSMC Clock */
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC, ENABLE);

    /*-- GPIO Configuration ------------------------------------------------------*/
    /* SRAM Data lines configuration */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1           //FSCM_DA2,3
                      | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10       //FSCM_DA13,14,15
                      | GPIO_Pin_14 | GPIO_Pin_15;                  //FSMC_DA0,1 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOD, &GPIO_InitStructure); 
  
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 |
                                GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | 
                                GPIO_Pin_15;      //FSMC_DA4 ~ DA12 
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOE, &GPIO_InitStructure);


    // NOE, NWE, NE1 configuration 
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5   //PD.4 = OE,PD.5 = WE
                    | GPIO_Pin_7;                   //PD.7 = NE1
    GPIO_Init(GPIOD, &GPIO_InitStructure);
  
    //NADV
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;     //PB.7=NADV
    GPIO_Init(GPIOB, &GPIO_InitStructure); 

    //NWAIT                
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;     //PD.6=NWAIT
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOD, &GPIO_InitStructure);
 

    /*-- FSMC Configuration ---------------------------------------------------*/
    //时序
    p.FSMC_AddressSetupTime = 0;        //地址信号提前_NE有效（下降沿）的时间
    //NADV Fall Edge与NE信号的Fall Edge同时产生，经过(_AddressSetupTime+1)时钟后
    //Rise, 复用方式，地址信号在NE信号的Fall Edge之前1Clock有效，在_AddressHoldTime
    //个Clock之后无效。如_AddressHoldTime是0，则在NADV的Rise Edge变为无效。要用
    //NADV的Rise Edge采样地址信号，必须置_AddressHoldTime延时1Clock以上.
        //AdrressHoldTime也是NADV上升沿到RD信号的启动时间，NADV上升后经过(ADDHOLD+1)
        //启动RD，RD信号经过(DataSetupTime+3)无效；WR信号则不受ADDHOLD影响，与NADV
        //同步跳变，但BUS信号经过ADDHOLD时间后才变化，即影响的是数据输出时间。
    p.FSMC_AddressHoldTime = 1;         //地址信号在NADV上升沿后保持的时间。>0    
    p.FSMC_DataSetupTime = 7;           //FSMC_RD脉冲6T。
    p.FSMC_BusTurnAroundDuration = 4;   //总线恢复延迟，两次Read之间最小间隔时间，<5时不影响时序
    p.FSMC_CLKDivision = 0;
    p.FSMC_DataLatency = 0;
    //.FSMC_AccessMode可选的4种模式，与.FSMC_DataAddressMux是平行的，设定.FSMC_DataAddressMux
    //复用模式，且.FSMC_ExtendedMode为Disable时，.FSMC_AccessMode设置不起作用。
    p.FSMC_AccessMode = FSMC_AccessMode_A;    

    FSMC_NORSRAMInitStructure.FSMC_Bank = FSMC_Bank1_NORSRAM1;
    FSMC_NORSRAMInitStructure.FSMC_DataAddressMux = FSMC_DataAddressMux_Enable;
    //.FSMC_MemoryType设置为NOR，才会置BCRx寄存器的FACCEN (bit6)，使NADV信号有输出
    FSMC_NORSRAMInitStructure.FSMC_MemoryType = FSMC_MemoryType_NOR;
    FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_16b;
    //.FSMC_BurstAccessMode Enable会锁死，原因不明。
    FSMC_NORSRAMInitStructure.FSMC_BurstAccessMode = FSMC_BurstAccessMode_Disable;

    FSMC_NORSRAMInitStructure.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low;
    FSMC_NORSRAMInitStructure.FSMC_WrapMode = FSMC_WrapMode_Disable;
    FSMC_NORSRAMInitStructure.FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState;
    FSMC_NORSRAMInitStructure.FSMC_WaitSignal = FSMC_WaitSignal_Disable;
    FSMC_NORSRAMInitStructure.FSMC_WriteBurst = FSMC_WriteBurst_Disable;

    FSMC_NORSRAMInitStructure.FSMC_WriteOperation = FSMC_WriteOperation_Enable;
    //.FSMC_ExtendedMode按文档是使读、写时序不同，实际不仅如此, _Enable会使NADV也没输出
    FSMC_NORSRAMInitStructure.FSMC_ExtendedMode = FSMC_ExtendedMode_Disable;
    //
    FSMC_NORSRAMInitStructure.FSMC_AsyncWait = FSMC_AsyncWait_Enable; 
    FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct = &p;
    FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct = &p;

    FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure); 

    // Enable FSMC Bank1_SRAM Bank 
    FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM1, ENABLE);  

    //--------------------------------
    //CPLD内部寄存器清0
    PLD_WriteCtrlReg(0xffff, 0x00);
}

/*******************************************************************************
* Function Name  : [Public]
* Description    : CPLD内部虚拟寄存器初始化
* Input          : None 
* Output         : None
* Return         : None
*******************************************************************************/
void PLD_Init(Bool cap_enable)
{
    u16 tmp1;

    //启动图像存储
    tmp1 = PLD_LCDSCALE;
    if (cap_enable)
    {
        tmp1 |= PLD_REGBIT_CAPEN;
    }

#if ((CAM_IMG_FORMAT == _IMGFMT_QVGA_RGB)||(CAM_IMG_FORMAT == _IMGFMT_VGA_RGB)  \
    ||(CAM_IMG_FORMAT == _IMGFMT_QuadVGA_RGB)||(CAM_IMG_FORMAT == _IMGFMT_SXVGA_RGB))
    tmp1 |= PLD_REGBIT_RGB565;    
#endif 

#if (SENSOR_CHIP == _SENSOR_MT9M111)
    tmp1 |= PLD_REGBIT_XCLK20M;
#endif
#if (SENSOR_CHIP == _SENSOR_OV7670)
    tmp1 &= (~PLD_REGBIT_XCLK20M);
#endif

    PLD_WriteCtrlReg(0xffff, tmp1); 

    //清其他寄存器
    Pld_LCDPort_OutDis();

}

/*******************************************************************************
@
@   Part-2:    图像捕捉和LCD显示控制
@
*******************************************************************************/

/*******************************************************************************
* Function Name  : [IRQ] 改写图像Capture控制寄存器，帧结束(VSYN)中断服务程序
* Description    : 每帧结束时，控制图像Capture的On/Off, 设置自动写入的SDRAM页
* 
* Input          : None 
* Output         : None
* Return         : None
*******************************************************************************/
void PLD_CapFrameServe_Irq(void)
{
    u16 tmp;

    if (pld_regUpd_req | (g_Img_CapEnable && (!g_Img_FrameLock)))
    {
        tmp = pld_RegCtrl;
        if (g_Img_CapEnable)        //_CAPEN会等到VSYN下降沿才起作用
            tmp |= PLD_REGBIT_CAPEN;
        else
            tmp &= (~PLD_REGBIT_CAPEN);         
        if (!g_Img_FrameLock)
        {
            tmp ^= PLD_REGBIT_CAPWRPAGE;        //切换Capture写入SDRAM页
        }

        pld_RegCtrl = tmp;
        Pld_UpdateCtrlReg();        //写CPLD内寄存器
        pld_regUpd_req = False;
    }
}

/*******************************************************************************
* Function Name  : [Public]
* Description    : 直接写PLD控制寄存器
* Input          : None 
* Output         : None
* Return         : None
*******************************************************************************/
void PLD_WriteCtrlReg(u16 mask, u16 value)
{
    u8 tmp_st;

    tmp_st = pld_regUpd_req;
    pld_regUpd_req = 0;         //制止中断函数改写pld_RegCtrl

    if (mask == 0xffff)
        pld_RegCtrl = value;
    else
    {
        pld_RegCtrl &= (~mask);
        if (value !=0)
            pld_RegCtrl |= mask;
    }
    g_Img_CapEnable = (Bool)((pld_RegCtrl & PLD_REGBIT_CAPEN)!=0);
    LCD_VideoEnable = (Bool)((pld_RegCtrl & PLD_REGBIT_LCDVIDEOEN)!=0);

    //16Bit地址0x8000左移1Bit
    Pld_UpdateCtrlReg();        //写CPLD内寄存器
    pld_regUpd_req = tmp_st;    //恢复
}

/*******************************************************************************
* Function Name  : 
* Description    : 控制图像Capture On/Off，不立即起作用，等VSYN中断才实际写寄存器
* Input          : None 
* Output         : None
* Return         : None
*******************************************************************************/
void PLD_CaptureRun(Bool on)
{    
    g_Img_CapEnable = on;
    pld_regUpd_req = True;
}

/*******************************************************************************
* Function Name  : [Public]
* Description    : Video to LCD显示控制，不立即起作用，等VSYN中断才实际写寄存器
* Input          : None 
* Output         : None
* Return         : None
*******************************************************************************/
void PLD_SetVideo2LCD(Bool on, u16 lcd_scale)
{
    while(GPIO_ReadOutputDataBit(PORT_CAMCTRL, PIN_CAM_VSYN) !=0)
    {
        CLRWDT();
    }                       

    LCD_VideoEnable = on;    
    if (LCD_VideoEnable)
        pld_RegCtrl |= PLD_REGBIT_LCDVIDEOEN;
    else
        pld_RegCtrl &= (~PLD_REGBIT_LCDVIDEOEN);
    if (lcd_scale !=0xffff)
    {    
        pld_RegCtrl &= (~PLD_REGBIT_LCDSCALE);
        pld_RegCtrl |= lcd_scale;
    }
    Pld_UpdateCtrlReg();        //写CPLD内寄存器
        
    if (LCD_VideoEnable)
    {
        Pld_LCDPort_OutEn();
    }
}

/*******************************************************************************
* Function Name  : [Pulic]
* Description    : 取得保存有最后图像帧的SDRAM页，给FSMC读写用，同时设置/解除锁定
*       页模式，锁定页模式下不自动交换Capture存储页，同时进行图像捕捉写到另一页，
*       不会影响FSMC读写图像或SDRAM数据，但会覆盖此前一帧图像.
* Input          : on - 锁定页,  off - 解除锁定
* Return         : 当前FSMC读写可用的页号(0/1)，此页号给Pld_SetFsmcPage()宏函数配套使用。
*******************************************************************************/
u16 PLD_LockImageFrame(Bool on)
{
    u32 frame;
    u16 fsmcpage;
          
    if (on)
    {
        if (g_Img_FrameLock && g_Img_CapEnable )
        {
            //不是自动交换页存储，等当前帧存储完成，取这一帧给FSMC读
            frame = cam_FrameNo;            
            while((cam_FrameNo == frame)||(pld_regUpd_req))     //pld_regUpd_req避免与中断冲突
            {
                CLRWDT();
            }            

            pld_RegCtrl ^= PLD_REGBIT_CAPWRPAGE;        //强制交换，后续写到另一页
            Pld_UpdateCtrlReg();            //写Capture控制寄存器，设置Cap页
        }
            //自动交换模式，取当前正在写入页的另一页，即前一帧，有<=1帧的延时。无需改写
            //Capture控制寄存器
        while(pld_regUpd_req)
        {
            CLRWDT();           //VSYN中断函数可能改写pld_RegCtrl，避免。
        }
        fsmcpage = ((~pld_RegCtrl) & PLD_REGBIT_FSMCPAGE);    //取当前Cap不写入的页
            //进入保护状态，后续Capture写入页固定，不破坏FSMC页。
        g_Img_FrameLock = True;                 
        Pld_SetFsmcPage(fsmcpage);      //设置FSMC页寄存器
    }
    else
    {
        fsmcpage = pld_RegFsmcPage;
        g_Img_FrameLock = False;    //
    }
    return(fsmcpage);
}

/*******************************************************************************
* Function Name  : [IRQ] 
* Description    : 
* Input          : None 
* Output         : None
* Return         : None
*******************************************************************************/
void PLD_DRAM_Refresh(void)
{
    u8 tmp_st;

    tmp_st = pld_regUpd_req;
    pld_regUpd_req = 0;         //制止中断函数改写pld_RegCtrl

    pld_RegCtrl |= PLD_REGBIT_REFTRIG;
    Pld_UpdateCtrlReg();        //写CPLD内寄存器
    pld_RegCtrl &= (~PLD_REGBIT_REFTRIG);
    Pld_UpdateCtrlReg();        

    //16Bit地址0x8000左移1Bit
    pld_regUpd_req = tmp_st;    //恢复
}

/*******************************************************************************
@
@   Part-3:    SDRAM作为数据存储区XBUF使用
@
*  
    XBUF使用说明：
    1. PLD扩展4Mx16 SRAM，分成2Mx16的两页，一页自动写入来自摄像头的图像数据，另一页保存
    有最后存储的图像帧，用于读出、处理，两页会自动交换，在PLD_CapFrameServe_Irq()，PLD_LockImageFrame()
    进行页管理。每页即使存储一帧1280x1024图像，任意有(2048-1280)x1024x2Byte的存储区可供
    MCU作为数据存储区使用，这部分存储区称为XBUF。
    2. 图像在SDRAM中按Row/Col式存储，每页分成1024行，每行2048WORD，每个WORD(UInt16)存储
    一个Pixel，存储在低的Row/COL地址空间。XBuf设定应避开图像区，图像宽度在1024以内时可以将
    Col >1024区块作为XBuf; 高度小于512的图像，可以将Col >512的区块作为XBuf。
    3. XBUF作为普通RAM使用要进行地址转换，使用UInt32来传递XBuf RAM地址，高16Bit作为Row地址；
    低16Bit作为Col地址，与图像Pixel对应，但可达到Byte级寻址，高15Bit与Pixel位置对应，最低
    Bit对应UInt16中的Byte选择，使用小端模式。
        用PLD_SetupXBuf设定XBuf在内存页中的区块，之后PLD_WriteToXBuf()/PLD_ReadFromXBuf()
    自动管理地址的转换，大块数据读写时，可以将上次读写返回的XBuf地址值，传递给下一次读写，
    实现连续数据读写。但最好避免用奇数长度(Bytes)的数据读写，减少Byte转换时间。
    4. 每次使用XBUF，还要用Pld_SetFsmcPage()设定读写的页，对于中断函数中使用XBUF，还要
    做特别的处理：由于主程序可能正在进行SDRAM的读写，中断程序中要保护SDRAM读写环境
    (全局变量，包括CPLD内部寄存器的值); 退出中断前再恢复这个读写环境。这个功能由
    PLD_GetXBufContext()、PLD_SetXBufContext()完成。
        PLD_SetupXBuf()得到的复合地址指针，可以认为是一个简化的SDRAM读写环境。还有一些约束：
    只允许一级中断; 不能在一个PLD_SetupXBuf()后同时读写两个XBUF块。
*
*******************************************************************************/
/*******************************************************************************
* Function Name  : [Public] for IRQ
* Description    : 保存SDRAM读写关联的环境变量。由中断函数调用。
* Input          : 
* Output         : None
* Return         : 
*******************************************************************************/
PldXBufContext PLD_GetXBufContext(void)
{
    PldXBufContext contx;
    
    contx.fsmcpage = pld_RegFsmcPage;
    contx.fsmcrow = Pld_ReadImgRowAddr();
    contx.colbase = pld_xbuf_colbase;
    contx.rowbase = pld_xbuf_rowbase;
    return(contx);
}

/*******************************************************************************
* Function Name  : [Public] for IRQ
* Description    : 恢复SDRAM读写环境变量。由中断函数调用。
* Input          : 
* Output         : None
* Return         : 
*******************************************************************************/
void PLD_SetXBufContext(PldXBufContext contx)
{
    Pld_SetFsmcPage(contx.fsmcpage);
    Pld_SelectImgRow(contx.fsmcrow);
    pld_xbuf_colbase = contx.colbase;
    pld_xbuf_rowbase = contx.rowbase;
}

/*******************************************************************************
* Function Name  : [Public]
* Description    : 设定XBUF扩展RAM区的物理地址
* Input          : 起点的Row、Col地址，Col为Pixel(Uint16)地址 
* Output         : None
* Return         : 返回32bit复合地址值，给XBUF读写级联传递用
*******************************************************************************/
u32 PLD_SetupXBuf(u16 rowbase, u16 colbase)
{
    pld_xbuf_rowbase = rowbase;
    pld_xbuf_colbase = (colbase<<1);        //u16->u8地址

    return(((u32)pld_xbuf_rowbase<<16) | pld_xbuf_colbase);
}


/*******************************************************************************
* Function Name  : [Public]
* Description    : 向XBUF扩展RAM区写数据
* Input          : XBufptr传递写入起点FSMC复合地址
* Output         : None
* Return         : 返回结束点的FSMC XBUF复合地址
*******************************************************************************/
u32 PLD_WriteToXBuf(u8 *srcbuf, s32 byteNum, u32 XBufPtr)
{
    u16 xbuf_rowptr, xbuf_colptr;
    u16 *ptr, *srcWordPtr, tmp1;
    u32 rowlimit;
    s32 i;

    //解开复合地址        
    rowlimit = (u32)Pld_PixelPtr(2048);
    xbuf_colptr = (u16)XBufPtr;
    xbuf_rowptr = (u16)(XBufPtr>>16); 
    Pld_SelectImgRow(xbuf_rowptr);
    ptr = (u16 *)(FSMC_Bank1_SRAM1_BASE + (u32)(xbuf_colptr & 0xfffe)); 

    //地址Byte偏移处理
    if (xbuf_colptr %2)
    {
        tmp1 = *ptr;
        tmp1 = ((u16)(srcbuf[0])<<8)|(tmp1 & 0x00FF);
        *ptr++ = tmp1;              //1, 高Byte进位
        if ((u32)ptr >= rowlimit)
        {
            xbuf_rowptr++;                          
            if (xbuf_rowptr >= (pld_xbuf_rowbase + PLD_XBUF_ROWLIM))
            {
                xbuf_rowptr = pld_xbuf_rowbase;     //简化的溢出保护：超限循环
            }
            Pld_SelectImgRow(xbuf_rowptr);
            ptr = (u16 *)(FSMC_Bank1_SRAM1_BASE + (u32)pld_xbuf_colbase); 
        }
        srcbuf++;
        byteNum--;
    }
    //WORD序列写
    srcWordPtr = (u16 *)srcbuf;
    for(i=byteNum/2; i>0; --i)
    {
        *ptr++ = *srcWordPtr++;
        if ((u32)ptr >= rowlimit)
        {            
            xbuf_rowptr++;                          
            if (xbuf_rowptr >= (pld_xbuf_rowbase + PLD_XBUF_ROWLIM))
            {
                xbuf_rowptr = pld_xbuf_rowbase;     //简化的溢出保护：超限循环
            }

            Pld_SelectImgRow(xbuf_rowptr);
            ptr = (u16 *)(FSMC_Bank1_SRAM1_BASE + (u32)pld_xbuf_colbase); 
        }
    }
    xbuf_colptr = (u32)ptr - (u32)Pld_MemPtrBase();
    //冗余Byte处理
    if (byteNum %2)
    {
        tmp1 = *ptr;
        tmp1 = (tmp1 & 0xFF00) | srcbuf[byteNum-1];
        *ptr = tmp1;        //0，低Byte不进位
        xbuf_colptr++;
    }
    //
    return(((u32)xbuf_rowptr<<16) | xbuf_colptr);
}

/*******************************************************************************
* Function Name  : [Public]
* Description    : 从XBUF扩展RAM区读出数据段
* Input          : XBufptr传递写入起点FSMC复合地址
* Output         : None
* Return         : 返回32bit复合地址值，给XBUF读写级联传递用
*******************************************************************************/
u32 PLD_ReadFromXBuf(u8 *dstbuf, s32 byteNum, u32 XBufPtr)
{
    u16 xbuf_rowptr, xbuf_colptr;
    u16 *ptr, *dstWordPtr, tmp1;
    u32 rowlimit;
    s32 i;
    
    rowlimit = (u32)Pld_PixelPtr(2048);
    xbuf_colptr = (u16)XBufPtr;
    xbuf_rowptr = (u16)(XBufPtr>>16); 
    Pld_SelectImgRow(xbuf_rowptr);
    ptr = (u16 *)(FSMC_Bank1_SRAM1_BASE + (u32)(xbuf_colptr & 0xfffe)); 

    //地址Byte偏移处理
    if (xbuf_colptr %2)
    {
        tmp1 = *ptr++;
        *dstbuf++ = (u8)(tmp1>>8);
        byteNum--;
        if ((u32)ptr >= rowlimit)
        {            
            xbuf_rowptr++;
            if (xbuf_rowptr >= (pld_xbuf_rowbase + PLD_XBUF_ROWLIM))
            {
                xbuf_rowptr = pld_xbuf_rowbase;     //简化的溢出保护：超限循环
            }   
            Pld_SelectImgRow(xbuf_rowptr);
            ptr = (u16 *)Pld_MemPtrBase() + pld_xbuf_colbase; 
        }
    }
    dstWordPtr = (u16 *)dstbuf;
    //读出
    for(i=byteNum/2; i>0; --i)
    {
        *dstWordPtr++ = *ptr++;
        if ((u32)ptr >= rowlimit)
        {            
            xbuf_rowptr++;
            if (xbuf_rowptr >= (pld_xbuf_rowbase + PLD_XBUF_ROWLIM))
            {
                xbuf_rowptr = pld_xbuf_rowbase;     //简化的溢出保护：超限循环
            }   
            Pld_SelectImgRow(xbuf_rowptr);
            ptr = (u16 *)(FSMC_Bank1_SRAM1_BASE + (u32)pld_xbuf_colbase); 
        }
    }
    xbuf_colptr = (u32)ptr - (u32)Pld_MemPtrBase();
    //冗余Byte处理
    if (byteNum %2)
    {
        tmp1 = *ptr;
        dstbuf = (u8 *)dstWordPtr;
        *dstbuf = (u8)tmp1;
        xbuf_colptr++;
    }
    //
    return(((u32)xbuf_rowptr<<16) | xbuf_colptr);
}

/*******************************************************************************
@
@   Part-4:    其他
@
*******************************************************************************/
//---------------------------------------------------------------------------------
//  DRAM读出测试
//  #自动捕捉图像模式打开时，在自动存储1Frame的时间内，只测试有限行。捕捉完
//  #一帧图像DRAM要换页，MCU读写页也被换，测试模式写的页和读的页可能不同，要
//  #避开换页时间。    
//  #640x480 RGB模式，存储1Frame时间约45ms
s32 PLD_TestDRam(u16 pagesel, s32 yaddr)
{
    s32 y, x, uplim;    //, rows_frm, frame;
    vu16 v1, v2;
    vu16 *ptr;
    s32 errcnt = 0;
    u16 page;   //, pageloop;

    //锁定SDRM内存块
    page = PLD_LockImageFrame(True);
    if (page != pagesel)
        page = PLD_LockImageFrame(True);        
    Pld_SetFsmcPage(page);
        
    //按参数，设定读写检测的行地址
    if (yaddr <0)
    {
        uplim = 1024;       //8Mx16 ->2048
        y = 0;
    }
    else
    {
        y = yaddr;
        uplim = yaddr+1;
    }
    //行读写检测
    while(y<uplim)
    {
        Pld_SelectImgRow(y);
        ptr = Pld_MemPtrBase();
        //写入
        v2 = yaddr;
        for(x=0; x<2048; x++)
        {
            *ptr = v2;
            v2++;
            ptr++;         
        }   

        //读出   
        v2 = yaddr;
        ptr = Pld_MemPtrBase();
        for(x=0; x<2048; x++)
        {
            v1 = (u16)(*ptr);
            if (v1 != v2)           //错误提示            
            if (v1 != v2)
            {
                LED1_L();
                errcnt++;
                LED1_H();
            }        
            ptr++;    
            v2++;
        }       
        y++;
        //
    }
    
    //释放SDRAM内存块
    PLD_LockImageFrame(False);          
    return(errcnt);
}
