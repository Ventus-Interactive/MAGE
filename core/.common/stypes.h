
#ifndef COMMON_STYPES
#define COMMON_STYPES

#include "./sdefs.h"

DT(byte, unsigned char)
DT(u32, unsigned int)
DT(i32, signed int)
DT(l64, long int)

DT(byte_arr, byte*)

DT(f32, float)

DS(Float2,
    f32 x; f32 y;
)

DS(size_pair,
    u32 size_k; 
    u32 size_v;
)

#endif
