#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "H264ParseCommon.h"

void del_emulation_prevention(BYTE *data, UINT *dataSize)
{
    UINT dataSizeTemp = *dataSize;
    for (UINT i=0, j=0; i<(dataSizeTemp-2); i++) {
        int val = (data[i]^0x0) + (data[i+1]^0x0) + (data[i+2]^0x3);    
        if (val == 0) {
            for (j=i+2; j<dataSizeTemp-1; j++) {    
                data[j] = data[j+1];
            }
            
            (*dataSize)--;      
        }
    }
}

void sps_bs_init(bit_stream *bs, const BYTE *data, UINT size)
{
    if (bs) {
        bs->data = data;
        bs->size = size;
        bs->index = 0;
    }
}
 
INT eof(bit_stream *bs)
{
    return (bs->index >= bs->size * 8);    
}
 
UINT u(bit_stream *bs, BYTE bitCount)
{
    UINT val = 0;
    for (BYTE i=0; i<bitCount; i++) {
        val <<= 1;
        if (eof(bs)) {
            val = 0;
            break;
        } else if (bs->data[bs->index / 8] & (0x80 >> (bs->index % 8))) {    
			val |= 1;
        }
        bs->index++; 
    }
    return val;	
}
 
UINT ue(bit_stream *bs)
{
    UINT zeroNum = 0;
    while (u(bs, 1) == 0 && !eof(bs) && zeroNum < 32) {
        zeroNum ++;
    }
    
    return (UINT)((1 << zeroNum) - 1 + u(bs, zeroNum));
}
 
INT se(bit_stream *bs)
{
    INT ueVal = (INT)ue(bs);
    double k = ueVal;
    
    INT seVal = (INT)ceil(k / 2);     
    if (ueVal % 2 == 0) {       
        seVal = -seVal;
    }
    
    return seVal;
}

