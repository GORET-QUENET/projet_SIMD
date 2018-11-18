/* --------------------------------- */
/* --- bench_morpho_SSE_THREAD.h --- */
/* --------------------------------- */


#ifndef __NR_BENCH_MORPHO_SSE_THREAD_H__
#define __NR_BENCH_MORPHO_SSE_THREAD_H__


#include "morpho_SSE_THREAD.h"
#define NBFRAME 300

void bench_morpho_SD_SSE_THREAD(uint8 ***m, long nrl, long nrh, long ncl, long nch, int NBTESTMAX);
void bench_morpho_FD_SSE_THREAD(uint8 ***m, long nrl, long nrh, long ncl, long nch, int NBTESTMAX);

#endif // __NR_BENCH_MORPHO_SSE_THREAD_H__
