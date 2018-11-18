/* --------------------------- */
/* --- morpho_SSE_THREAD.h --- */
/* --------------------------- */


#ifndef __MORPHO_SSE_THREAD_H__
#define __MORPHO_SSE_THREAD_H__

void CopyMatrice_SSE_THREAD(long nrl, long nrh, long ncl, long nch, uint8 **dest, uint8 **src);

void Erosion3_SSE_THREAD(long nrl, long nrh, long ncl, long nch, uint8 **m, uint8 **tmp);
void Erosion5_SSE_THREAD(long nrl, long nrh, long ncl, long nch, uint8 **m, uint8 **tmp);
void Dilatation3_SSE_THREAD(long nrl, long nrh, long ncl, long nch, uint8 **m, uint8 **tmp);
void Dilatation5_SSE_THREAD(long nrl, long nrh, long ncl, long nch, uint8 **m, uint8 **tmp);
void Fermeture3_SSE_THREAD(long nrl, long nrh, long ncl, long nch, uint8 **m, uint8 **tmp);
void Overture3_SSE_THREAD(long nrl, long nrh, long ncl, long nch, uint8 **m, uint8 **tmp);
void Fermeture5_SSE_THREAD(long nrl, long nrh, long ncl, long nch, uint8 **m, uint8 **tmp);
void Ouverture3_SSE_THREAD(long nrl, long nrh, long ncl, long nch, uint8 **m, uint8 **tmp);
void Ouverture5_SSE_THREAD(long nrl, long nrh, long ncl, long nch, uint8 **m, uint8 **tmp);

#endif // __MORPHO_SSE_THREAD_H__
