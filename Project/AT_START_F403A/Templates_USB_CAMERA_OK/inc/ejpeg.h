/*============================================================================
//  File Name       : ejpeg.h
//  Author          : HECC. DuckWeed Tech.
//                  @HuiZhou, 2011
//  email:  hcc21cn@163.com	
//  Version         : V2.0
//  Description     : JPEG������
//  ʹ��˵����
//      1).��JPG_initImgFormat()�趨Դͼ��ߴ�
//         ��JPG_initOutStream()�趨�������������������������CallBack����������
//      ��������С��1K SRAM��
//         ��JPG_WriteAppDat()�趨�������ݣ����ǿ�ѡ���ܡ�
//      2).����JPG_Encode()ִ��ͼ����롣���ؽ������ByteNum���м��Զ�����
//      CallBack_OutStreamд�����մ洢���ʡ�
==============================================================================*/
#ifndef __ejpeg_H_
#define __ejpeg_H_
//
//#include "stm32f10x_lib.h"
//#include "stm32f10x_type.h"

typedef signed long  Int32;
typedef signed short Int16;
typedef signed char  Int8;

typedef unsigned long  UInt32;
typedef unsigned short UInt16;
typedef unsigned char  UInt8;

//---------------------------------------------------------------------------
//  �������
#define JPG_USING_APP0

//--------------------------------------------------------------------------
#define	JPG_IMGFMT_MONO		    0
#define JPG_IMGFMT_YUV422       1
#define JPG_IMGFMT_YUV420       2
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
typedef UInt8* (*CallBack_OutStream)(UInt8 *bufptr, Int32 byteNum);

//---------------------------------------------------------------------------

    //��������quality_factor��Χ[1,8]��1��ã�8��� 
void JPG_initImgFormat(UInt16 width, UInt16 height, UInt8 scale, UInt8 img_format, 
                UInt8 quality, UInt16 pitch, UInt16 xbeg, UInt16 ybeg);
void JPG_initOutStream(CallBack_OutStream streamFunc, UInt8 *outbuf, Int32 bufsize);
UInt8 JPG_WriteAppDat(UInt8 *in_dat, UInt16 size);
UInt8 *JPG_WriteHeader(UInt16 jpghd_offset);
Int32 JPG_Encode(void);


#endif
