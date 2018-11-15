/* --------------------- */
/* --- test_morpho.h --- */
/* --------------------- */


#ifndef __NR_TEST_MORPHO_H__
#define __NR_TEST_MORPHO_H__


#include "morpho.h"
#define NBFRAME 300

void test_morpho_SD_SEQ(uint8 ***m, long nrl, long nrh, long ncl, long nch);
void test_morpho_FD_SEQ(uint8 ***m, long nrl, long nrh, long ncl, long nch);

#endif // __NR_TEST_MORPHO_H__
