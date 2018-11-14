/* ---------------------------- */
/* --- test_mouvement_SEE.h --- */
/* ---------------------------- */


#ifndef __NR_TEST_MOUVEMENT_SSE_H__
#define __NR_TEST_MOUVEMENT_SSE_H__

#include "mouvement_SSE.h"
#include "morpho_SSE.h"
#define NBFRAME 300

void test_FD_SSE(uint8 **m, long nrl, long nrh, long ncl, long nch);
void test_SD_SSE(uint8 **m, long nrl, long nrh, long ncl, long nch);

#endif // __NR_TEST_MOUVEMENT_SSE_H__
