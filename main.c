#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "H264ParseSPS.h"
#include "log.h"

int main()
{
	const BYTE data[] = {0x67, 0x42, 0x00, 0x28, 0xab, 0x40, 0x22, 0x01, 0xe3, 0xcb, 0xcd, 0xc0, 0x80, 0x80, 0xa9, 0x02};
	UINT dataSize = sizeof(data);
	sps_info_struct *info = (sps_info_struct *)malloc(sizeof(sps_info_struct));
	memset(info, 0, sizeof(sps_info_struct));
	h264_parse_sps(data, dataSize, info);
	printf("width %u height %u fps %u\n", info->width, info->height, info->fps);
	LOGE("%02x\n", data[0]);
	LOGW("LOGW %02x\n", data[0]);
	LOGI("LOGI %02x\n", data[0]);
	return 0;	
 } 
