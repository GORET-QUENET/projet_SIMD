/* ------------- */
/* --- ROC.h --- */
/* ------------- */


#ifndef __ROC_H__
#define __ROC_H__

#include "morpho.h"
#include "morpho_SSE.h"
#include "morpho_THREAD.h"
#include "mouvement.h"
#include "mouvement_SSE.h"
#include "vnrdef.h"
#define NBFRAME 300

void Fill_ROC(uint8 **m, long nrl, long nrh, long ncl, long nch, int ROC[], int step);
void test_ROC(uint8 **m, long nrl, long nrh, long ncl, long nch, int ROC[], char* AlgoName);
void Affiche_ROC(int ROC[]);

void test_ROC_morpho_SD_SEQ(uint8 **m, long nrl, long nrh, long ncl, long nch);
void test_ROC_morpho_FD_SEQ(uint8 **m, long nrl, long nrh, long ncl, long nch);
void test_ROC_morpho_SD_SSE(uint8 **m, long nrl, long nrh, long ncl, long nch);
void test_ROC_morpho_FD_SSE(uint8 **m, long nrl, long nrh, long ncl, long nch);
void test_ROC_morpho_SD_THREAD(uint8 **m, long nrl, long nrh, long ncl, long nch);
void test_ROC_morpho_FD_THREAD(uint8 **m, long nrl, long nrh, long ncl, long nch);
void test_ROC_FD_SEQ(uint8 **m, long nrl, long nrh, long ncl, long nch);
void test_ROC_SD_SEQ(uint8 **m, long nrl, long nrh, long ncl, long nch);
void test_ROC_FD_SSE(uint8 **m, long nrl, long nrh, long ncl, long nch);
void test_ROC_SD_SSE(uint8 **m, long nrl, long nrh, long ncl, long nch);


#endif // __ROC_H__
