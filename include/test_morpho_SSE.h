/* ------------------------- */
/* --- test_morpho_SSE.h --- */
/* ------------------------- */


#ifndef __NR_TEST_MORPHO_SSE_H__
#define __NR_TEST_MORPHO_SSE_H__


#include "morpho_SSE.h"
#define NBFRAME 300

void test_morpho_SD_SSE(uint8 **m, long nrl, long nrh, long ncl, long nch);
void test_morpho_FD_SSE(uint8 **m, long nrl, long nrh, long ncl, long nch);

#endif // __NR_TEST_MORPHO_SSE_H__
