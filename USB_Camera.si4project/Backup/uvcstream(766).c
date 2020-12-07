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

#include "ov2640.h"

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
typedef struct  tagXBufContext
{
    u16 fsmcpage;
    u16 fsmcrow;
    u16 colbase;
    u16 rowbase;
}   PldXBufContext;
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
vs32 jpegSendLen = 0;                //��ǰFrame�ѷ���Byte Number
vu32 frameCnt = 0;               //����XBUF�õĸ��ϵ�ַ

s32 m_frameNo = 0;
/* Private function prototypes -----------------------------------------------*/

void UVC_SendPack_Irq(void)
{
    s32 datalen;
    u8 *payload;
    PldXBufContext contx;

    if (jpeglen ==0)
    {
        //��ǰ֡û��JPEG���ݣ���������ʵ�ʷ�������հ�
        return;
    }

    //��SDRAM�ж���
    payload = uvc_rdbuf + CAMERA_SIZ_STREAMHD;
    if (jpegSendLen==0)
    {
        //ÿ֡ͼ�����ʼ��
        //��ʼ��payload
        uvc_rdbuf[0] = 0x02;
        uvc_rdbuf[1] = 0x01;        //0x81Ҳ��

        uvc_rdbuf[1] &= 0x01;
        uvc_rdbuf[1] ^= 0x01;

        //��XBUF�ж������ݰ����͸�USB TX Buffer������ÿ֡ͼ��Ŀ�ʼ��Ҫȡ��XBUF��ָ��
        datalen = PACKET_SIZE - CAMERA_SIZ_STREAMHD;
        //
        bufCopy(ov2640_framebuf_ptr + jpegSendLen, payload, datalen);
        //��TX Buffer�͵�USB PMA Buffer
        UserToPMABufferCopy(uvc_rdbuf, ENDP1_BUF1Addr, PACKET_SIZE);
        jpegSendLen = datalen;
//        printf("222=%x,jpegSendLen=%d\n",ov2640_framebuf_ptr,jpegSendLen);
    }
    else
    {
        //��֡ͼ��ĺ�����
        datalen = PACKET_SIZE - CAMERA_SIZ_STREAMHD;
            //�ж��Ƿ����һ��
        if (jpegSendLen + datalen >= jpeglen)
        {
            datalen = jpeglen - jpegSendLen;
            uvc_rdbuf[1] |= 0x02;       //�ӽ��������
        }
        bufCopy(ov2640_framebuf_ptr + jpegSendLen, payload, datalen);
        jpegSendLen += datalen;
//        printf("111=%x,jpegSendLen=%d\n",ov2640_framebuf_ptr + jpegSendLen,jpegSendLen);
//        printf("jpegSendLen=%d,datalen=%d,frameCnt=%d\n",jpegSendLen,datalen,frameCnt);
//        datalen += CAMERA_SIZ_STREAMHD;

#if 0
            //����������JPEG�涨�������ֽ�0xFF��䡣�ⲻ�Ǳ���ģ�������͵����ݰ�
            //<PACKET_SIZE�����Զ�������Byte��ʵ��Ҳ��PACKET_SIZE����
        if (datalen < PACKET_SIZE-CAMERA_SIZ_STREAMHD)
        {
            printf("jpegSendLen=%d,datalen=%d,frameCnt=%d\n",jpegSendLen,datalen,frameCnt);
            datalen--;
            while(datalen<PACKET_SIZE-1-CAMERA_SIZ_STREAMHD)
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
        UserToPMABufferCopy(uvc_rdbuf, ENDP1_BUF0Addr, PACKET_SIZE);
        SetEPDblBuf0Count(ENDP1, EP_DBUF_IN, PACKET_SIZE);
    }
    else
    {
        // User use buffer1
        UserToPMABufferCopy(uvc_rdbuf, ENDP1_BUF1Addr, PACKET_SIZE);
        SetEPDblBuf1Count(ENDP1, EP_DBUF_IN, PACKET_SIZE);
    }
    _ToggleDTOG_RX(ENDP1);

    //�жϱ�֡ͼ���Ƿ������, һ֡������ɣ��л�����һ֡
    if (jpegSendLen >= jpeglen)
    {
      jpegSendLen = 0;
//        if ((xbuf_RdFrame != xbuf_WrFrame)&&(jpg_codesize[xbuf_WrFrame]>0))
//        {
//            xbuf_RdFrame = xbuf_WrFrame;    //��һ֡��д�ã�����ָ��ǰд��飬�ƶ�֡�л���
//        }
    }
}

void bufCopy(u8* srcPtr,u8* desPtr, u32 len)
{
  while(len)
  {
    *desPtr=*srcPtr;
    desPtr++;
    srcPtr++;
    len--;
  }
}
