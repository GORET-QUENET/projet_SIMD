/* ---------------------------- */
/* --- test_morpho_THREAD.h --- */
/* ---------------------------- */


#ifndef __NR_TEST_MORPHO_THREAD_H__
#define __NR_TEST_MORPHO_THREAD_H__


#include "morpho_THREAD.h"
#define NBFRAME 300

void test_morpho_SD_THREAD(uint8 **m, long nrl, long nrh, long ncl, long nch);
void test_morpho_FD_THREAD(uint8 **m, long nrl, long nrh, long ncl, long nch);

#endif // __NR_TEST_MORPHO_THREAD_H__
