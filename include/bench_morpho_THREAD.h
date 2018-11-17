/* ----------------------------- */
/* --- bench_morpho_THREAD.h --- */
/* ----------------------------- */


#ifndef __NR_BENCH_MORPHO_THREAD_H__
#define __NR_BENCH_MORPHO_THREAD_H__


#include "morpho_THREAD.h"
#define NBFRAME 300

void bench_morpho_SD_THREAD(uint8 ***m, long nrl, long nrh, long ncl, long nch, int NBTESTMAX);
void bench_morpho_FD_THREAD(uint8 ***m, long nrl, long nrh, long ncl, long nch, int NBTESTMAX);

#endif // __NR_BENCH_MORPHO_THREAD_H__
