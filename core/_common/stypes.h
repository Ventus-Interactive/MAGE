
#ifndef COMMON_STYPES
#define COMMON_STYPES

#include "./sdefs.h"

DT(byte, unsigned char)
DT(u32, unsigned int)
DT(i32, signed int)
DT(l64, long int)

DT(byte_arr_t, byte*)

DT(f32, float)

DS(size_pair_t,
    u32 size_k; 
    u32 size_v;
)

#endif
