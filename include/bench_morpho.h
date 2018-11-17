/* ---------------------- */
/* --- bench_morpho.h --- */
/* ---------------------- */


#ifndef __NR_BENCH_MORPHO_H__
#define __NR_BENCH_MORPHO_H__


#include "morpho.h"
#define NBFRAME 300

void bench_morpho_SD_SEQ(uint8 ***m, long nrl, long nrh, long ncl, long nch, int NBTESTMAX);
void bench_morpho_FD_SEQ(uint8 ***m, long nrl, long nrh, long ncl, long nch, int NBTESTMAX);

#endif // __NR_BENCH_MORPHO_H__
