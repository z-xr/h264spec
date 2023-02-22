//
//  H264ParseSPS.h
//
//  Created by lzj<lizhijian_21@163.com> on 2018/7/6.
//  Copyright ? 2018�� LZJ. All rights reserved.
//

#ifndef H264ParseSPS_h
#define H264ParseSPS_h
#ifdef __cplusplus
extern "C"
{
#endif
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "H264ParseCommon.h"


typedef struct
{
    unsigned int profile_idc;
    unsigned int level_idc;
    
    unsigned int width;
    unsigned int height;
    unsigned int fps;       //SPS�п��ܲ�����FPS��Ϣ
} sps_info_struct;


void vui_para_parse(sps_bit_stream *bs, sps_info_struct *info);

/**
 ����SPS������Ϣ

 @param data SPS�������ݣ���ҪNal����Ϊ0x7���ݵĿ�ʼ(���磺67 42 00 28 ab 40 22 01 e3 cb cd c0 80 80 a9 02)
 @param dataSize SPS���ݵĳ���
 @param info SPS����֮�����Ϣ���ݽṹ��
 @return success:1��fail:0
 
 */
int h264_parse_sps(const unsigned char *data, unsigned int dataSize, sps_info_struct *info);

#ifdef __cplusplus
}
#endif
#endif /* H264ParseSPS_h */
