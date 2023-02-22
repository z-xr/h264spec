#ifndef H264ParsePPS_H
#define H264ParsePPS_H

#include "H264ParseCommon.h"

typedef struct
{
    unsigned int profile_idc;
    unsigned int level_idc;
    
    unsigned int width;
    unsigned int height;
    unsigned int fps;       //SPS�п��ܲ�����FPS��Ϣ
} pps_info_struct;

int h264_parse_pps(const unsigned char *data, unsigned int dataSize, pps_info_struct *info);
#endif