/* ----------------------- */
/* --- mouvement_SSE.h --- */
/* ----------------------- */


#ifndef __NR_MOUVEMENT_SSE_H__
#define __NR_MOUVEMENT_SSE_H__

#include "vnrdef.h"
#define THETA 16
#define N 3
#define Vmax 254
#define Vmin 16

vuint8 _mm_MAX_epi8(vuint8 a, vuint b);
vuint8 _mm_MIN_epi8(vuint a, vuint b);
vuint8 _mm_sub_abs_epi8(vuint8 a, vuint8 b);
vuint8 _mm_cmplt_epu8(vuint8 a, vuint8 b);
vuint8 _mm_cmpgt_epu8(vuint8 a, vuint8 b);

void FD_SSE(long nrl, long nrh, long ncl, long nch, uint8 **ma, uint8 **m, uint8 **mFD);
void SD_SSE(long nrl, long nrh, long ncl, long nch, uint8 **m, uint8 **mSD, uint8 **MSD, uint8 **MSDa, uint8 **OSD, uint8 **VSD, uint8 **VSDa);

#endif // __NR_MOUVEMENT_SSE_H__
