#ifndef H264ParseCommon_H
#define H264ParseCommon_H
#include <stdio.h>

typedef unsigned char BYTE;
typedef int INT;
typedef unsigned int UINT;

typedef struct
{
    const BYTE *data;   //sps����
    UINT size;          //sps���ݴ�С
    UINT index;         //��ǰ����λ���ڵ�λ�ñ��
} sps_bit_stream;
 
void del_emulation_prevention(BYTE *data, UINT *dataSize);
void sps_bs_init(sps_bit_stream *bs, const BYTE *data, UINT size);
INT eof(sps_bit_stream *bs);
UINT u(sps_bit_stream *bs, BYTE bitCount);
UINT ue(sps_bit_stream *bs);
INT se(sps_bit_stream *bs);


#endif
