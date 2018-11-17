/* ------------------------- */
/* --- bench_mouvement.h --- */
/* ------------------------- */


#ifndef __NR_BENCH_MOUVEMENT_H__
#define __NR_BENCH_MOUVEMENT_H__

#include "mouvement.h"
#include "morpho.h"
#define NBFRAME 300

void bench_FD_SEQ(uint8 ***m, long nrl, long nrh, long ncl, long nch, int NBTESTMAX);
void bench_SD_SEQ(uint8 ***m, long nrl, long nrh, long ncl, long nch, int NBTESTMAX);

#endif // __NR_TEST_MOUVEMENT_H__
