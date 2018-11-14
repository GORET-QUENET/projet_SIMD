/* ------------------------ */
/* --- test_mouvement.h --- */
/* ------------------------ */


#ifndef __NR_TEST_MOUVEMENT_H__
#define __NR_TEST_MOUVEMENT_H__

#include "mouvement.h"
#include "morpho.h"
#define NBFRAME 300

void test_FD_SEQ(uint8 **m, long nrl, long nrh, long ncl, long nch);
void test_SD_SEQ(uint8 **m, long nrl, long nrh, long ncl, long nch);

#endif // __NR_TEST_MOUVEMENT_H__
