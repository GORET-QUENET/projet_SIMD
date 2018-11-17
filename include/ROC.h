/* ------------- */
/* --- ROC.h --- */
/* ------------- */


#ifndef __ROC_H__
#define __ROC_H__

#include "vnrdef.h"
#define NBFRAME 300

void Fill_ROC(uint8 **m, long nrl, long nrh, long ncl, long nch, int ROC[], int step);
void test_ROC(uint8 **m, long nrl, long nrh, long ncl, long nch, int ROC[], char* AlgoName);
void Affiche_ROC(int ROC[]);

#endif // __ROC_H__
