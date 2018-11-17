/* ----------------------------- */
/* --- bench_mouvement_SEE.h --- */
/* ----------------------------- */


#ifndef __NR_BENCH_MOUVEMENT_SSE_H__
#define __NR_BENCH_MOUVEMENT_SSE_H__

#include "mouvement_SSE.h"
#include "morpho_SSE.h"
#define NBFRAME 300

void bench_FD_SSE(uint8 ***m, long nrl, long nrh, long ncl, long nch, int NBTESTMAX);
void bench_SD_SSE(uint8 ***m, long nrl, long nrh, long ncl, long nch, int NBTESTMAX);

#endif // __NR_BENCH_MOUVEMENT_SSE_H__
