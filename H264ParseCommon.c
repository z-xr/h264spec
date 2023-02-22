#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "H264ParseCommon.h"

void del_emulation_prevention(BYTE *data, UINT *dataSize)
{
    UINT dataSizeTemp = *dataSize;
    for (UINT i=0, j=0; i<(dataSizeTemp-2); i++) {
        int val = (data[i]^0x0) + (data[i+1]^0x0) + (data[i+2]^0x3);    //����Ƿ��Ǿ�����
        if (val == 0) {
            for (j=i+2; j<dataSizeTemp-1; j++) {    //�Ƴ�������
                data[j] = data[j+1];
            }
            
            (*dataSize)--;      //data size ��1
        }
    }
}

void sps_bs_init(sps_bit_stream *bs, const BYTE *data, UINT size)
{
    if (bs) {
        bs->data = data;
        bs->size = size;
        bs->index = 0;
    }
}
 
INT eof(sps_bit_stream *bs)
{
    return (bs->index >= bs->size * 8);    //λƫ���Ѿ���������
}
 
UINT u(sps_bit_stream *bs, BYTE bitCount)
{
    UINT val = 0;
    for (BYTE i=0; i<bitCount; i++) {
        val <<= 1;
        if (eof(bs)) {
            val = 0;
            break;
        } else if (bs->data[bs->index / 8] & (0x80 >> (bs->index % 8))) {     //����index���ڵ�λ�Ƿ�Ϊ1
            val |= 1;
        }
        bs->index++;  //������ǰ��ʼλ(��ʾ��λ�Ѿ������㣬�ں���ļ�������в���Ҫ�ٴ�ȥ�������ڵ���ʼλ������ȱ�㣺����ÿ��bitλ����Ҫȥλ��)
    }
    
    return val;
}
 
UINT ue(sps_bit_stream *bs)
{
    UINT zeroNum = 0;
    while (u(bs, 1) == 0 && !eof(bs) && zeroNum < 32) {
        zeroNum ++;
    }
    
    return (UINT)((1 << zeroNum) - 1 + u(bs, zeroNum));
}
 
INT se(sps_bit_stream *bs)
{
    INT ueVal = (INT)ue(bs);
    double k = ueVal;
    
    INT seVal = (INT)ceil(k / 2);     //ceil:���ش��ڻ��ߵ���ָ������ʽ����С����
    if (ueVal % 2 == 0) {       //ż��ȡ������(-1)^(k+1)
        seVal = -seVal;
    }
    
    return seVal;
}

