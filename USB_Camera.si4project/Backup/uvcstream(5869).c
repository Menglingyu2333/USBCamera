
#include "uvcstream.h"
#include "usb_lib.h"
#include "usb_desc.h"
#include "usb_mem.h"

#include "ov2640.h"


#define CAMERA_SIZ_STREAMHD     2

u8 UVC_TxBuf[PACKET_SIZE];          //USB TX Buffer
vs32 FrameSentLen = 0;               //��ǰFrame�ѷ���Byte Number

/* Private function prototypes -----------------------------------------------*/

void UVC_SendPack_Irq(void)
{
    uint32_t datalen;
    uint8_t *payload;

    payload = UVC_TxBuf + CAMERA_SIZ_STREAMHD;

    if (FrameSentLen >= FrameLen)
    {
        FrameSentLen = 0;
        //�������֡����Buf�ĸ����ڽ�����ɵ�״̬
        if(FrameBuf_1_Ready)
        {
          Frame_SendPtr = FrameBuf_1_Addr;
          FrameLen = FrameBuf_1_Len;
        }
        else
        {
          Frame_SendPtr = FrameBuf_2_Addr;
          FrameLen = FrameBuf_2_Len;
        }
        //ÿ֡ͼ�����ʼ������ʼ��payload
        UVC_TxBuf[0] = 0x02;
        UVC_TxBuf[1] &= 0x01;
        UVC_TxBuf[1] ^= 0x01;

        datalen = PACKET_SIZE - CAMERA_SIZ_STREAMHD;
        bufCopy(Frame_SendPtr + FrameSentLen, payload, datalen);
        //��TX Buffer�͵�USB PMA Buffer
        UserToPMABufferCopy(UVC_TxBuf, ENDP1_BUF1Addr, PACKET_SIZE);
        FrameSentLen = datalen;

    }
    else
    {
        //��֡ͼ��ĺ�����
        datalen = PACKET_SIZE - CAMERA_SIZ_STREAMHD;
            //�ж��Ƿ����һ��
        if (FrameSentLen + datalen >= FrameLen)
        {
            datalen = FrameLen - FrameSentLen;
            UVC_TxBuf[1] |= 0x02;       //�ӽ��������
        }
        bufCopy(Frame_SendPtr + FrameSentLen, payload, datalen);
        FrameSentLen += datalen;
    }

    //USB˫����ģʽ���ݰ�����
    if(_GetENDPOINT(ENDP1) & EP_DTOG_RX)    //EP_DTOG_RX ->ʹ�õ���BUF1
    {
        // User use buffer0
        UserToPMABufferCopy(UVC_TxBuf, ENDP1_BUF0Addr, PACKET_SIZE);
        SetEPDblBuf0Count(ENDP1, EP_DBUF_IN, PACKET_SIZE);
    }
    else
    {
        // User use buffer1
        UserToPMABufferCopy(UVC_TxBuf, ENDP1_BUF1Addr, PACKET_SIZE);
        SetEPDblBuf1Count(ENDP1, EP_DBUF_IN, PACKET_SIZE);
    }
    _ToggleDTOG_RX(ENDP1);

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
