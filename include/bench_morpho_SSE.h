/* ------------------------- */
/* --- bench_morpho_SSE.h --- */
/* ------------------------- */


#ifndef __NR_BENCH_MORPHO_SSE_H__
#define __NR_BENCH_MORPHO_SSE_H__


#include "morpho_SSE.h"
#define NBFRAME 300

void bench_morpho_SD_SSE(uint8 ***m, long nrl, long nrh, long ncl, long nch, int NBTESTMAX);
void bench_morpho_FD_SSE(uint8 ***m, long nrl, long nrh, long ncl, long nch, int NBTESTMAX);

#endif // __NR_BENCH_MORPHO_SSE_H__
