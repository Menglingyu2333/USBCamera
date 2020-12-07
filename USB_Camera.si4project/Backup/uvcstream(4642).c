/*============================================================================
//
//	UVCStream.c
//		by: HECC. @HuiZhou, 2011	
//
//   email: hcc21cn@163.com	
//		
**==============================================================================*/
#include "uvcstream.h"
#include "AppPlatForm.h"
//#include "Pld_Intf.h"
#include "ejpeg.h"

#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdio.h>

#include "usb_lib.h"
#include "usb_desc.h"
#include "usb_mem.h"

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/
#define DEF_JPGBUF_SIZE         2048

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
u8 jpg_codebuf[DEF_JPGBUF_SIZE];    //JPEG������bit stream buffer����JPEG Encoder����
vu32 xbuf_WrFramePtr;               //д��XBUF�õĸ��ϵ�ַ
    
    //����2��XBUF��P0/P1������Ӧ�洢1֡JPEG�����ͼ��һ������JPEG Encoderд�룬
    //һ������USB�������ͣ���̬����������XBUF���Ķ�д�����Ĳ������������У���д����
    //����xbuf_WrFrame/xbuf_RdFrame��Ϊ����ID����õ�ǰ��дXBUF����
    //  ����XBUF�����ڵ�ҳ�Ƕ�̬��, ��ͼ������ҳ��ͬ������ڲ�ͬ��ROW�ϣ���������
    //ҳ�Ƿ���ͬ���������ͻ��ÿһ����Ϊ512�У�ÿ��512Word, ��512KBytes���Դ洢һ��
    //JPEGͼ���ˡ�
volatile PldXBufContext  xbuf_context[2];   //XBUF���Ķ�д�����Ĳ���
vs32 jpg_codesize[2] = {0, 0};  //�ѱ������ݳ��ȡ��жϿ�����UVC_streamStart֮ǰִ�У�Ӧ��ʼ��
vu8 xbuf_WrFrame;               //Encoder writing frame ID
vu8 xbuf_RdFrame = 0;           //USB reading frame ID��Ӧ��ʼ��!

    //UVC payload head
#define CAMERA_SIZ_STREAMHD     2
u8 uvc_StreamHead[CAMERA_SIZ_STREAMHD] = {0x02, 0x01};
//#define CAMERA_SIZ_STREAMHD     12
//#u8 uvc_StreamHead[CAMERA_SIZ_STREAMHD] = {0x0C, 0x01, 0,0,0,0,0, 0,0,0,0,0};
    
    //XBUF����USB����Buffer
u8 uvc_rdbuf[PACKET_SIZE];          //USB TX Buffer
vu32 xbuf_RdFramePtr;               //����XBUF�õĸ��ϵ�ַ
vs32 uvc_rdsize = 0;                //��ǰFrame�ѷ���Byte Number

s32 m_frameNo = 0;
/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/
/*******************************************************************************
* Function Name  : [private] CallBack
* Description    :  ��JPEG Encoder������ݵ�Ŀ��File Stream��Ӧ�ô����� 
*       JPEG Encoder��bit streamд��һ���Թ����Buffer����Buffer���󣬵���jpgStreamManage()
*   ����������д�����յ����Stream��ˢ�����bit stream buffer�������ڶ��ı�׼����ģʽ��
* Input          :  JPEG��������ָ��(bufptr)�����ݳ���
* Output         : 
* Return         :  ˢ�º��bit stream bufferָ��
*******************************************************************************/
u8* jpgStreamManage(UInt8 *bufptr, Int32 byteNum)
{
    Pld_SetFsmcPage(xbuf_context[xbuf_WrFrame].fsmcpage);   //���ж϶�д��ֻ����ҳ�Ϳ�����
    xbuf_WrFramePtr = PLD_WriteToXBuf(bufptr, byteNum, xbuf_WrFramePtr);
    return(jpg_codebuf);
}

/*******************************************************************************
* Function Name  : [Public]
* Description    :  ��ʼ��USB Video Class Stream
* Input          : 
* Output         : 
* Return         : 
*******************************************************************************/
void UVC_streamStart(void)
{
//#if 0
    s32 datalen;   
//#endif

    //��ʼ��JPEG������
    JPG_initImgFormat(IMG_WIDTH, IMG_HEIGHT, IMG_VIDEO_SCALE, JPG_IMGFMT_YUV420, 
                2, IMG_WIDTH, 0, 0);
    JPG_initOutStream(jpgStreamManage, jpg_codebuf, DEF_JPGBUF_SIZE);

    //��ʼ��UVC payload head
    uvc_rdbuf[0] = 0x02;
    uvc_rdbuf[1] = 0x01;        //0x81Ҳ��
    
    uvc_rdsize = 0;             //��ʼ����֡���ݴ������

    //Ԥ�ȱ�����֡ͼ�񣬴���XBUF��ͬʱ��ʼ������XBUF��(P0/P1)
    xbuf_WrFrame = 0;
    PLD_LockImageFrame(True);                                   //����ͼ��֡P0
    xbuf_WrFramePtr = PLD_SetupXBuf(0, PLD_XBUF_STARTCOL);      //�趨P0֡JPEG���ݴ洢��
    xbuf_context[0] = PLD_GetXBufContext();         //����XBUF�����Ķ�д����
    JPG_WriteHeader(0);                                         //����JPEG Encoder
    jpg_codesize[0] = JPG_Encode();                             //��֡�������
        //
    xbuf_WrFrame = 1;
    PLD_LockImageFrame(True);                                   //����ͼ��֡P1
    xbuf_WrFramePtr = PLD_SetupXBuf(512, PLD_XBUF_STARTCOL);    //�趨P1֡JPEG���ݴ洢��
    xbuf_context[1] = PLD_GetXBufContext();         //����XBUF�����Ķ�д����
    JPG_WriteHeader(0);
    jpg_codesize[1] = JPG_Encode();
    
    //
    PLD_LockImageFrame(False);      //���֡�洢������SDRAMͼ���Զ�ˢ��
    xbuf_RdFrame = 0;               //��ʼ��XBUF����ָ�룺д��ָ��P1������ָ��P0��Ӧ��                                        
                                    //�˳���֮ǰҲ����ʼ������ֹ�ж���ǰִ��
//#if 0        
    //��SDRAM�ж���һ��Packet��Ԥд��USB TX Buffer��û�����Ҳ���Թ�������PC�յ���
    //��һ�����ݰ�����������.
        //read fram xbuf-p0
    Pld_SetFsmcPage(xbuf_context[0].fsmcpage);
    xbuf_RdFramePtr = PLD_SetupXBuf(0, PLD_XBUF_STARTCOL);
        //ISO Double Buffer, send
    datalen = PACKET_SIZE - CAMERA_SIZ_STREAMHD;
    xbuf_RdFramePtr = PLD_ReadFromXBuf(uvc_rdbuf+CAMERA_SIZ_STREAMHD, datalen, xbuf_RdFramePtr);                
    UserToPMABufferCopy(uvc_rdbuf, ENDP1_BUF0Addr, PACKET_SIZE);
    _SetEPDblBuf0Count(ENDP1, EP_DBUF_IN, PACKET_SIZE);
    uvc_rdsize += datalen;
//#endif

    //����׼����ɣ�������EP_TX_VALID�������Ҫ! ����EP_TX_VALID��һ���յ�IN���ƣ�
    //USB������PACKET_SIZE���ȵ����ݰ�������Buf0Count/Buf1Count������Ρ���ƾBufxCount=0
    //���ÿ��Ʋ�ס���͡�
    _SetEPTxStatus(ENDP1, EP_TX_VALID);
}

/*******************************************************************************
* Function Name  : [Public/INT]
* Description    :  USB���ݰ����͡���USB EndPoint�жϵ��á�
*       ��XBUF�ж������ݲ����ͣ�ÿ��ΪPACKET_SIZE���ȵ����ݰ�������payload��Ϣ��
*   �Զ��л�XBUF�鲢�ƶ�JPEG Encoder������һ֡��
* Input          : 
* Output         : 
* Return         : 
*******************************************************************************/
void UVC_SendPack_Irq(void)
{
    s32 datalen;    
    u8 *payload;
    PldXBufContext contx;

    if (jpg_codesize[xbuf_RdFrame] ==0)     
    {
        //��ǰ֡û��JPEG���ݣ���������ʵ�ʷ�������հ�
        return;
    }

    //��SDRAM�ж���
    payload = uvc_rdbuf + CAMERA_SIZ_STREAMHD;
    if (uvc_rdsize==0)
    {
        //ÿ֡ͼ�����ʼ��
        uvc_rdbuf[1] &= 0x01;
        uvc_rdbuf[1] ^= 0x01;

        //��XBUF�ж������ݰ����͸�USB TX Buffer������ÿ֡ͼ��Ŀ�ʼ��Ҫȡ��XBUF��ָ��
        datalen = PACKET_SIZE - CAMERA_SIZ_STREAMHD;
        contx = PLD_GetXBufContext();           //push �����д����
        PLD_SetXBufContext(xbuf_context[xbuf_RdFrame]);     //���ö�д�������ж��б��������!        
            //ȡ��XBUF������ĸ��ϵ�ַ����������ڱ���Context֮����.
        if (xbuf_RdFrame ==0)
            xbuf_RdFramePtr = PLD_SetupXBuf(0, PLD_XBUF_STARTCOL);            
        else
            xbuf_RdFramePtr = PLD_SetupXBuf(512, PLD_XBUF_STARTCOL);
            //����
        xbuf_RdFramePtr = PLD_ReadFromXBuf(payload, datalen, xbuf_RdFramePtr);                
        PLD_SetXBufContext(contx);              //pop �ָ����̵߳Ķ�д����

        //��TX Buffer�͵�USB PMA Buffer
        UserToPMABufferCopy(uvc_rdbuf, ENDP1_BUF1Addr, PACKET_SIZE);
        uvc_rdsize = datalen;
        datalen += CAMERA_SIZ_STREAMHD;
    }
    else
    {
        //��֡ͼ��ĺ�����
        datalen = PACKET_SIZE - CAMERA_SIZ_STREAMHD;
            //�ж��Ƿ����һ��                
        if (uvc_rdsize + datalen >= jpg_codesize[xbuf_RdFrame])
        {
            datalen = jpg_codesize[xbuf_RdFrame] - uvc_rdsize;
            uvc_rdbuf[1] |= 0x02;       //�ӽ��������
        }
        contx = PLD_GetXBufContext();           //push
        PLD_SetXBufContext(xbuf_context[xbuf_RdFrame]);     //setup context
        xbuf_RdFramePtr = PLD_ReadFromXBuf(payload, datalen, xbuf_RdFramePtr);
        PLD_SetXBufContext(contx);              //pop

        uvc_rdsize += datalen;    
        datalen += CAMERA_SIZ_STREAMHD;

#if 0
            //����������JPEG�涨�������ֽ�0xFF��䡣�ⲻ�Ǳ���ģ�������͵����ݰ�
            //<PACKET_SIZE�����Զ�������Byte��ʵ��Ҳ��PACKET_SIZE����
        if (datalen < PACKET_SIZE)         
        {
            datalen--;
            while(datalen<PACKET_SIZE-1)
            {
                uvc_rdbuf[datalen++] = 0xff;
            }
            uvc_rdbuf[datalen++] = 0xD9;        //�¹���JPEG������ 
        }
#endif
    }

    //USB˫����ģʽ���ݰ�����
    if(_GetENDPOINT(ENDP1) & EP_DTOG_RX)    //EP_DTOG_RX ->ʹ�õ���BUF1
    {
        // User use buffer0
        UserToPMABufferCopy(uvc_rdbuf, ENDP1_BUF0Addr, datalen);
        SetEPDblBuf0Count(ENDP1, EP_DBUF_IN, datalen);
    }
    else
    {
        // User use buffer1
        UserToPMABufferCopy(uvc_rdbuf, ENDP1_BUF1Addr, datalen);
        SetEPDblBuf1Count(ENDP1, EP_DBUF_IN, datalen);
    }
    _ToggleDTOG_RX(ENDP1);    

    //�жϱ�֡ͼ���Ƿ������, һ֡������ɣ��л�����һ֡
    if (uvc_rdsize >= jpg_codesize[xbuf_RdFrame])
    {
        uvc_rdsize = 0;         // 
        if ((xbuf_RdFrame != xbuf_WrFrame)&&(jpg_codesize[xbuf_WrFrame]>0)) 
        {
            xbuf_RdFrame = xbuf_WrFrame;    //��һ֡��д�ã�����ָ��ǰд��飬�ƶ�֡�л���
        }
    }
}

/*******************************************************************************
* Function Name  : [Public]
* Description    :  ����ͼ��JPEG���롣����ѭ���е��ô˺�����ʵ��UVC������Ƶ����
*       Ԥ������2֡ͼ��д��/����ָ��ͬXBUF��; ����������һ֡��ɣ���鵽д��ָ��
*   ��XBUF��������(���ѱ������)���л������˿��; �������鵽����ָ���л�����Ӧ��
*   �л�д��ָ�룬���ѷ��͵�XBUF�飬������ͼ��֡�ı��룬�Դ�ѭ��ʵ��������Ƶ���������
*       ���ڵ�֡ͼ��׽�����롢USB����ʱ�䲻ͬ������Ҫ��֡��׽ͬ�����˺���Ҫ��VSYN
*   �жϺ�ִ�У���ִ�к����һ��VSYN�ż������������ֱ��볬ǰ�������������Ƶ��ʱ��
*   ���С�����Ҫ��˺����ڲ����֡ͬ�������ⲿ��ѭ������֡ͬ�������UVC_MovieEncoder��
*   �������ٴ�ͬ����
*
* Input          : �Ƿ�Ҫ���ڴ˺����ڲ�����֡ͬ��
* Output         : 
* Return         : �ɹ�����һ֡������Byte�������򷵻�0
*******************************************************************************/
s32 UVC_MovieEncoder(bool reqSync)
{
    u16 page;
    
    if (reqSync)
    {        
        //֡ͬ��
        if (cam_FrameNo == m_frameNo)
            return(0);        
        m_frameNo = cam_FrameNo;
    }
    if (xbuf_RdFrame == xbuf_WrFrame)   //����֡�Ѹ������ˣ��л�XBUF����ʼ��֡����
    {
        xbuf_WrFrame++;                     //XBUF���д����
        xbuf_WrFrame &= 0x01;
        jpg_codesize[xbuf_WrFrame] = 0;     //��ǰд���������������ζ���

        //����һ֡ͼ��
        page = PLD_LockImageFrame(True);    //����ͼ�񣬶�̬���XBUF������ҳ��
                //@ֹͣͼ��Captureʹ��ǰ֡��ɺ���һ֡���Զ��洢��SDRAM����ʱ��SDRAM
                //�Ķ�д�ٶ�Ҫ��һЩ��JPEG�����ٶ���Ӧ��һЩ������ǰ֡�����д�꣬����
                //JPEG����ʱ�䳬��һ֡ͼ��ʱ�����Ч�������������Ҳ���ԡ�
                //Capture Stop������PLD_LockImageFrame()֮�����У��������_LockImageFrame.
        PLD_CaptureRun(False);  

        if (xbuf_WrFrame ==0)               //����XBUF����SDRAM�е�λ��
            xbuf_WrFramePtr = PLD_SetupXBuf(0, PLD_XBUF_STARTCOL);
        else
            xbuf_WrFramePtr = PLD_SetupXBuf(512, PLD_XBUF_STARTCOL);
        xbuf_context[xbuf_WrFrame].fsmcpage = page;     //��̬����XBUF������ҳ        
            //JPEG����
        JPG_WriteHeader(0);
        jpg_codesize[xbuf_WrFrame] = JPG_Encode();
        
        PLD_LockImageFrame(False);          //�ͷ�֡�����������Զ���׽
        PLD_CaptureRun(True);               //�ָ�ͼ���Զ���׽

#if 0
        //Test
        if (xbuf_WrFrame ==0)
            GPIO_ResetBits(PORT_LED,PIN_LED1);
        else
            GPIO_SetBits(PORT_LED,PIN_LED1); 
#endif
        return(jpg_codesize[xbuf_WrFrame]);
    }    
    else
        return(0);
}
