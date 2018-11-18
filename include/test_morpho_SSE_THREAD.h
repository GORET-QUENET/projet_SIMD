/* -------------------------------- */
/* --- test_morpho_SSE_THREAD.h --- */
/* -------------------------------- */


#ifndef __NR_TEST_MORPHO_SSE_THREAD_H__
#define __NR_TEST_MORPHO_SSE_THREAD_H__


#include "morpho_SSE_THREAD.h"
#define NBFRAME 300

void test_morpho_SD_SSE_THREAD(uint8 **m, long nrl, long nrh, long ncl, long nch);
void test_morpho_FD_SSE_THREAD(uint8 **m, long nrl, long nrh, long ncl, long nch);

#endif // __NR_TEST_MORPHO_SSE_THREAD_H__
