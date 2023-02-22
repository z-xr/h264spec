#ifndef H264ParseCommon_H
#define H264ParseCommon_H
#include <stdio.h>

typedef unsigned char BYTE;
typedef int INT;
typedef unsigned int UINT;

typedef struct
{
    const BYTE *data;   //spsÊý¾Ý
    UINT size;          
    UINT index;         
} bit_stream;
 
void del_emulation_prevention(BYTE *data, UINT *dataSize);
void sps_bs_init(bit_stream *bs, const BYTE *data, UINT size);
INT eof(bit_stream *bs);
UINT u(bit_stream *bs, BYTE bitCount);
UINT ue(bit_stream *bs);
INT se(bit_stream *bs);


#endif
