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
    
volatile Bool g_Img_CapEnable = False;      //����/�ر�CPLD�Զ���������ͷͼ��    
volatile Bool LCD_VideoEnable = False;    //����/�ر�Scaler��LCD��ͼ����ʾ

    //SDRAM�ֳ�2ҳ��ÿҳ�洢һ֡ͼ��(Page<->Frame),FSMC��д��֡��PLD���ڱ��浱ǰ֡��ͬ
    //(FSMC��д��ǰһ֡),����ͬʱ����; PLD��Page-0д��һ֡�����������Ӧ��Page-1д��һ
    //֡����ҳ����; Page-1������ǰһ֡ͼ����ᵼ�´�֡���ƻ���g_Img_FrameLock����Ϊ1��
    //�򲻽���ҳ������������Page-0д�����һ֡ͼ���ƻ�����MCU��д��Page-1��ͼ�񱣳�������
volatile Bool g_Img_FrameLock = False;
    
vu16 pld_RegFsmcPage = 0;                   //CPLD��ҳѡ��Ĵ���(Reg-1)Ӱ��

/* Private variables ---------------------------------------------------------*/
vu16 pld_RegCtrl = 0;                       //CPLD���ƼĴ���(Reg-0)Ӱ��
vu16 pld_regUpd_req = 0;                   //������VSYN�жϺ����и�дReg-0������̬

    //��ǰXBUF��д�õ�ȫ�ֻ�������
u16 pld_xbuf_rowbase = 0;                   //XBUF�����
u16 pld_xbuf_colbase = 1024;                //XBUF�����

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/*******************************************************************************
*******************************************************************************/

/*******************************************************************************
@
@   Part-1:    ����Ӳ������
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
        //VSYNC�жϣ�ѡ��PortA��GPIO_Pinx���ӵ�EXTIx�ж�����
    GPIO_EXTILineConfig(GPIO_PORTSOURCE_VSYNC, GPIO_PINSOURCE_VSYNC);
    EXTI_InitStructure.EXTI_Line = EXTI_LINE_VSYNC;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;     //�½����ж�, VSYN�����ԣ�������
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);
        //HREF�ж�
    GPIO_EXTILineConfig(GPIO_PORTSOURCE_HREF, GPIO_PINSOURCE_HREF);
    EXTI_InitStructure.EXTI_Line = EXTI_LINE_HREF;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;     //�½����ж�, �н���
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
    TIM_TimeBaseStructure.TIM_Prescaler = 72-1;          //����ʱ��1MHz
    TIM_TimeBaseStructure.TIM_Period = 100-1;              //Update����: 100us
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
    //ʱ��
    p.FSMC_AddressSetupTime = 0;        //��ַ�ź���ǰ_NE��Ч���½��أ���ʱ��
    //NADV Fall Edge��NE�źŵ�Fall Edgeͬʱ����������(_AddressSetupTime+1)ʱ�Ӻ�
    //Rise, ���÷�ʽ����ַ�ź���NE�źŵ�Fall Edge֮ǰ1Clock��Ч����_AddressHoldTime
    //��Clock֮����Ч����_AddressHoldTime��0������NADV��Rise Edge��Ϊ��Ч��Ҫ��
    //NADV��Rise Edge������ַ�źţ�������_AddressHoldTime��ʱ1Clock����.
        //AdrressHoldTimeҲ��NADV�����ص�RD�źŵ�����ʱ�䣬NADV�����󾭹�(ADDHOLD+1)
        //����RD��RD�źž���(DataSetupTime+3)��Ч��WR�ź�����ADDHOLDӰ�죬��NADV
        //ͬ�����䣬��BUS�źž���ADDHOLDʱ���ű仯����Ӱ������������ʱ�䡣
    p.FSMC_AddressHoldTime = 1;         //��ַ�ź���NADV�����غ󱣳ֵ�ʱ�䡣>0    
    p.FSMC_DataSetupTime = 7;           //FSMC_RD����6T��
    p.FSMC_BusTurnAroundDuration = 4;   //���߻ָ��ӳ٣�����Read֮����С���ʱ�䣬<5ʱ��Ӱ��ʱ��
    p.FSMC_CLKDivision = 0;
    p.FSMC_DataLatency = 0;
    //.FSMC_AccessMode��ѡ��4��ģʽ����.FSMC_DataAddressMux��ƽ�еģ��趨.FSMC_DataAddressMux
    //����ģʽ����.FSMC_ExtendedModeΪDisableʱ��.FSMC_AccessMode���ò������á�
    p.FSMC_AccessMode = FSMC_AccessMode_A;    

    FSMC_NORSRAMInitStructure.FSMC_Bank = FSMC_Bank1_NORSRAM1;
    FSMC_NORSRAMInitStructure.FSMC_DataAddressMux = FSMC_DataAddressMux_Enable;
    //.FSMC_MemoryType����ΪNOR���Ż���BCRx�Ĵ�����FACCEN (bit6)��ʹNADV�ź������
    FSMC_NORSRAMInitStructure.FSMC_MemoryType = FSMC_MemoryType_NOR;
    FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_16b;
    //.FSMC_BurstAccessMode Enable��������ԭ������
    FSMC_NORSRAMInitStructure.FSMC_BurstAccessMode = FSMC_BurstAccessMode_Disable;

    FSMC_NORSRAMInitStructure.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low;
    FSMC_NORSRAMInitStructure.FSMC_WrapMode = FSMC_WrapMode_Disable;
    FSMC_NORSRAMInitStructure.FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState;
    FSMC_NORSRAMInitStructure.FSMC_WaitSignal = FSMC_WaitSignal_Disable;
    FSMC_NORSRAMInitStructure.FSMC_WriteBurst = FSMC_WriteBurst_Disable;

    FSMC_NORSRAMInitStructure.FSMC_WriteOperation = FSMC_WriteOperation_Enable;
    //.FSMC_ExtendedMode���ĵ���ʹ����дʱ��ͬ��ʵ�ʲ������, _Enable��ʹNADVҲû���
    FSMC_NORSRAMInitStructure.FSMC_ExtendedMode = FSMC_ExtendedMode_Disable;
    //
    FSMC_NORSRAMInitStructure.FSMC_AsyncWait = FSMC_AsyncWait_Enable; 
    FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct = &p;
    FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct = &p;

    FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure); 

    // Enable FSMC Bank1_SRAM Bank 
    FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM1, ENABLE);  

    //--------------------------------
    //CPLD�ڲ��Ĵ�����0
    PLD_WriteCtrlReg(0xffff, 0x00);
}

/*******************************************************************************
* Function Name  : [Public]
* Description    : CPLD�ڲ�����Ĵ�����ʼ��
* Input          : None 
* Output         : None
* Return         : None
*******************************************************************************/
void PLD_Init(Bool cap_enable)
{
    u16 tmp1;

    //����ͼ��洢
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

    //�������Ĵ���
    Pld_LCDPort_OutDis();

}

/*******************************************************************************
@
@   Part-2:    ͼ��׽��LCD��ʾ����
@
*******************************************************************************/

/*******************************************************************************
* Function Name  : [IRQ] ��дͼ��Capture���ƼĴ�����֡����(VSYN)�жϷ������
* Description    : ÿ֡����ʱ������ͼ��Capture��On/Off, �����Զ�д���SDRAMҳ
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
        if (g_Img_CapEnable)        //_CAPEN��ȵ�VSYN�½��ز�������
            tmp |= PLD_REGBIT_CAPEN;
        else
            tmp &= (~PLD_REGBIT_CAPEN);         
        if (!g_Img_FrameLock)
        {
            tmp ^= PLD_REGBIT_CAPWRPAGE;        //�л�Captureд��SDRAMҳ
        }

        pld_RegCtrl = tmp;
        Pld_UpdateCtrlReg();        //дCPLD�ڼĴ���
        pld_regUpd_req = False;
    }
}

/*******************************************************************************
* Function Name  : [Public]
* Description    : ֱ��дPLD���ƼĴ���
* Input          : None 
* Output         : None
* Return         : None
*******************************************************************************/
void PLD_WriteCtrlReg(u16 mask, u16 value)
{
    u8 tmp_st;

    tmp_st = pld_regUpd_req;
    pld_regUpd_req = 0;         //��ֹ�жϺ�����дpld_RegCtrl

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

    //16Bit��ַ0x8000����1Bit
    Pld_UpdateCtrlReg();        //дCPLD�ڼĴ���
    pld_regUpd_req = tmp_st;    //�ָ�
}

/*******************************************************************************
* Function Name  : 
* Description    : ����ͼ��Capture On/Off�������������ã���VSYN�жϲ�ʵ��д�Ĵ���
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
* Description    : Video to LCD��ʾ���ƣ������������ã���VSYN�жϲ�ʵ��д�Ĵ���
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
    Pld_UpdateCtrlReg();        //дCPLD�ڼĴ���
        
    if (LCD_VideoEnable)
    {
        Pld_LCDPort_OutEn();
    }
}

/*******************************************************************************
* Function Name  : [Pulic]
* Description    : ȡ�ñ��������ͼ��֡��SDRAMҳ����FSMC��д�ã�ͬʱ����/�������
*       ҳģʽ������ҳģʽ�²��Զ�����Capture�洢ҳ��ͬʱ����ͼ��׽д����һҳ��
*       ����Ӱ��FSMC��дͼ���SDRAM���ݣ����Ḳ�Ǵ�ǰһ֡ͼ��.
* Input          : on - ����ҳ,  off - �������
* Return         : ��ǰFSMC��д���õ�ҳ��(0/1)����ҳ�Ÿ�Pld_SetFsmcPage()�꺯������ʹ�á�
*******************************************************************************/
u16 PLD_LockImageFrame(Bool on)
{
    u32 frame;
    u16 fsmcpage;
          
    if (on)
    {
        if (g_Img_FrameLock && g_Img_CapEnable )
        {
            //�����Զ�����ҳ�洢���ȵ�ǰ֡�洢��ɣ�ȡ��һ֡��FSMC��
            frame = cam_FrameNo;            
            while((cam_FrameNo == frame)||(pld_regUpd_req))     //pld_regUpd_req�������жϳ�ͻ
            {
                CLRWDT();
            }            

            pld_RegCtrl ^= PLD_REGBIT_CAPWRPAGE;        //ǿ�ƽ���������д����һҳ
            Pld_UpdateCtrlReg();            //дCapture���ƼĴ���������Capҳ
        }
            //�Զ�����ģʽ��ȡ��ǰ����д��ҳ����һҳ����ǰһ֡����<=1֡����ʱ�������д
            //Capture���ƼĴ���
        while(pld_regUpd_req)
        {
            CLRWDT();           //VSYN�жϺ������ܸ�дpld_RegCtrl�����⡣
        }
        fsmcpage = ((~pld_RegCtrl) & PLD_REGBIT_FSMCPAGE);    //ȡ��ǰCap��д���ҳ
            //���뱣��״̬������Captureд��ҳ�̶������ƻ�FSMCҳ��
        g_Img_FrameLock = True;                 
        Pld_SetFsmcPage(fsmcpage);      //����FSMCҳ�Ĵ���
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
    pld_regUpd_req = 0;         //��ֹ�жϺ�����дpld_RegCtrl

    pld_RegCtrl |= PLD_REGBIT_REFTRIG;
    Pld_UpdateCtrlReg();        //дCPLD�ڼĴ���
    pld_RegCtrl &= (~PLD_REGBIT_REFTRIG);
    Pld_UpdateCtrlReg();        

    //16Bit��ַ0x8000����1Bit
    pld_regUpd_req = tmp_st;    //�ָ�
}

/*******************************************************************************
@
@   Part-3:    SDRAM��Ϊ���ݴ洢��XBUFʹ��
@
*  
    XBUFʹ��˵����
    1. PLD��չ4Mx16 SRAM���ֳ�2Mx16����ҳ��һҳ�Զ�д����������ͷ��ͼ�����ݣ���һҳ����
    �����洢��ͼ��֡�����ڶ�����������ҳ���Զ���������PLD_CapFrameServe_Irq()��PLD_LockImageFrame()
    ����ҳ����ÿҳ��ʹ�洢һ֡1280x1024ͼ��������(2048-1280)x1024x2Byte�Ĵ洢���ɹ�
    MCU��Ϊ���ݴ洢��ʹ�ã��ⲿ�ִ洢����ΪXBUF��
    2. ͼ����SDRAM�а�Row/Colʽ�洢��ÿҳ�ֳ�1024�У�ÿ��2048WORD��ÿ��WORD(UInt16)�洢
    һ��Pixel���洢�ڵ͵�Row/COL��ַ�ռ䡣XBuf�趨Ӧ�ܿ�ͼ������ͼ������1024����ʱ���Խ�
    Col >1024������ΪXBuf; �߶�С��512��ͼ�񣬿��Խ�Col >512��������ΪXBuf��
    3. XBUF��Ϊ��ͨRAMʹ��Ҫ���е�ַת����ʹ��UInt32������XBuf RAM��ַ����16Bit��ΪRow��ַ��
    ��16Bit��ΪCol��ַ����ͼ��Pixel��Ӧ�����ɴﵽByte��Ѱַ����15Bit��Pixelλ�ö�Ӧ�����
    Bit��ӦUInt16�е�Byteѡ��ʹ��С��ģʽ��
        ��PLD_SetupXBuf�趨XBuf���ڴ�ҳ�е����飬֮��PLD_WriteToXBuf()/PLD_ReadFromXBuf()
    �Զ������ַ��ת����������ݶ�дʱ�����Խ��ϴζ�д���ص�XBuf��ֵַ�����ݸ���һ�ζ�д��
    ʵ���������ݶ�д������ñ�������������(Bytes)�����ݶ�д������Byteת��ʱ�䡣
    4. ÿ��ʹ��XBUF����Ҫ��Pld_SetFsmcPage()�趨��д��ҳ�������жϺ�����ʹ��XBUF����Ҫ
    ���ر�Ĵ�������������������ڽ���SDRAM�Ķ�д���жϳ�����Ҫ����SDRAM��д����
    (ȫ�ֱ���������CPLD�ڲ��Ĵ�����ֵ); �˳��ж�ǰ�ٻָ������д���������������
    PLD_GetXBufContext()��PLD_SetXBufContext()��ɡ�
        PLD_SetupXBuf()�õ��ĸ��ϵ�ַָ�룬������Ϊ��һ���򻯵�SDRAM��д����������һЩԼ����
    ֻ����һ���ж�; ������һ��PLD_SetupXBuf()��ͬʱ��д����XBUF�顣
*
*******************************************************************************/
/*******************************************************************************
* Function Name  : [Public] for IRQ
* Description    : ����SDRAM��д�����Ļ������������жϺ������á�
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
* Description    : �ָ�SDRAM��д�������������жϺ������á�
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
* Description    : �趨XBUF��չRAM���������ַ
* Input          : ����Row��Col��ַ��ColΪPixel(Uint16)��ַ 
* Output         : None
* Return         : ����32bit���ϵ�ֵַ����XBUF��д����������
*******************************************************************************/
u32 PLD_SetupXBuf(u16 rowbase, u16 colbase)
{
    pld_xbuf_rowbase = rowbase;
    pld_xbuf_colbase = (colbase<<1);        //u16->u8��ַ

    return(((u32)pld_xbuf_rowbase<<16) | pld_xbuf_colbase);
}


/*******************************************************************************
* Function Name  : [Public]
* Description    : ��XBUF��չRAM��д����
* Input          : XBufptr����д�����FSMC���ϵ�ַ
* Output         : None
* Return         : ���ؽ������FSMC XBUF���ϵ�ַ
*******************************************************************************/
u32 PLD_WriteToXBuf(u8 *srcbuf, s32 byteNum, u32 XBufPtr)
{
    u16 xbuf_rowptr, xbuf_colptr;
    u16 *ptr, *srcWordPtr, tmp1;
    u32 rowlimit;
    s32 i;

    //�⿪���ϵ�ַ        
    rowlimit = (u32)Pld_PixelPtr(2048);
    xbuf_colptr = (u16)XBufPtr;
    xbuf_rowptr = (u16)(XBufPtr>>16); 
    Pld_SelectImgRow(xbuf_rowptr);
    ptr = (u16 *)(FSMC_Bank1_SRAM1_BASE + (u32)(xbuf_colptr & 0xfffe)); 

    //��ַByteƫ�ƴ���
    if (xbuf_colptr %2)
    {
        tmp1 = *ptr;
        tmp1 = ((u16)(srcbuf[0])<<8)|(tmp1 & 0x00FF);
        *ptr++ = tmp1;              //1, ��Byte��λ
        if ((u32)ptr >= rowlimit)
        {
            xbuf_rowptr++;                          
            if (xbuf_rowptr >= (pld_xbuf_rowbase + PLD_XBUF_ROWLIM))
            {
                xbuf_rowptr = pld_xbuf_rowbase;     //�򻯵��������������ѭ��
            }
            Pld_SelectImgRow(xbuf_rowptr);
            ptr = (u16 *)(FSMC_Bank1_SRAM1_BASE + (u32)pld_xbuf_colbase); 
        }
        srcbuf++;
        byteNum--;
    }
    //WORD����д
    srcWordPtr = (u16 *)srcbuf;
    for(i=byteNum/2; i>0; --i)
    {
        *ptr++ = *srcWordPtr++;
        if ((u32)ptr >= rowlimit)
        {            
            xbuf_rowptr++;                          
            if (xbuf_rowptr >= (pld_xbuf_rowbase + PLD_XBUF_ROWLIM))
            {
                xbuf_rowptr = pld_xbuf_rowbase;     //�򻯵��������������ѭ��
            }

            Pld_SelectImgRow(xbuf_rowptr);
            ptr = (u16 *)(FSMC_Bank1_SRAM1_BASE + (u32)pld_xbuf_colbase); 
        }
    }
    xbuf_colptr = (u32)ptr - (u32)Pld_MemPtrBase();
    //����Byte����
    if (byteNum %2)
    {
        tmp1 = *ptr;
        tmp1 = (tmp1 & 0xFF00) | srcbuf[byteNum-1];
        *ptr = tmp1;        //0����Byte����λ
        xbuf_colptr++;
    }
    //
    return(((u32)xbuf_rowptr<<16) | xbuf_colptr);
}

/*******************************************************************************
* Function Name  : [Public]
* Description    : ��XBUF��չRAM���������ݶ�
* Input          : XBufptr����д�����FSMC���ϵ�ַ
* Output         : None
* Return         : ����32bit���ϵ�ֵַ����XBUF��д����������
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

    //��ַByteƫ�ƴ���
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
                xbuf_rowptr = pld_xbuf_rowbase;     //�򻯵��������������ѭ��
            }   
            Pld_SelectImgRow(xbuf_rowptr);
            ptr = (u16 *)Pld_MemPtrBase() + pld_xbuf_colbase; 
        }
    }
    dstWordPtr = (u16 *)dstbuf;
    //����
    for(i=byteNum/2; i>0; --i)
    {
        *dstWordPtr++ = *ptr++;
        if ((u32)ptr >= rowlimit)
        {            
            xbuf_rowptr++;
            if (xbuf_rowptr >= (pld_xbuf_rowbase + PLD_XBUF_ROWLIM))
            {
                xbuf_rowptr = pld_xbuf_rowbase;     //�򻯵��������������ѭ��
            }   
            Pld_SelectImgRow(xbuf_rowptr);
            ptr = (u16 *)(FSMC_Bank1_SRAM1_BASE + (u32)pld_xbuf_colbase); 
        }
    }
    xbuf_colptr = (u32)ptr - (u32)Pld_MemPtrBase();
    //����Byte����
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
@   Part-4:    ����
@
*******************************************************************************/
//---------------------------------------------------------------------------------
//  DRAM��������
//  #�Զ���׽ͼ��ģʽ��ʱ�����Զ��洢1Frame��ʱ���ڣ�ֻ���������С���׽��
//  #һ֡ͼ��DRAMҪ��ҳ��MCU��дҳҲ����������ģʽд��ҳ�Ͷ���ҳ���ܲ�ͬ��Ҫ
//  #�ܿ���ҳʱ�䡣    
//  #640x480 RGBģʽ���洢1Frameʱ��Լ45ms
s32 PLD_TestDRam(u16 pagesel, s32 yaddr)
{
    s32 y, x, uplim;    //, rows_frm, frame;
    vu16 v1, v2;
    vu16 *ptr;
    s32 errcnt = 0;
    u16 page;   //, pageloop;

    //����SDRM�ڴ��
    page = PLD_LockImageFrame(True);
    if (page != pagesel)
        page = PLD_LockImageFrame(True);        
    Pld_SetFsmcPage(page);
        
    //���������趨��д�����е�ַ
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
    //�ж�д���
    while(y<uplim)
    {
        Pld_SelectImgRow(y);
        ptr = Pld_MemPtrBase();
        //д��
        v2 = yaddr;
        for(x=0; x<2048; x++)
        {
            *ptr = v2;
            v2++;
            ptr++;         
        }   

        //����   
        v2 = yaddr;
        ptr = Pld_MemPtrBase();
        for(x=0; x<2048; x++)
        {
            v1 = (u16)(*ptr);
            if (v1 != v2)           //������ʾ            
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
    
    //�ͷ�SDRAM�ڴ��
    PLD_LockImageFrame(False);          
    return(errcnt);
}
