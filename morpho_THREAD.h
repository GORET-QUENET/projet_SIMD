/* ----------------------- */
/* --- morpho_THREAD.h --- */
/* ----------------------- */


#ifndef __MORPHO_THREAD_H__
#define __MORPHO_THREAD_H__

void CopyMatrice_parallel(long nrl, long nrh, long ncl, long nch, uint8 **dest, uint8 **src);

void Erosion3_parallel(long nrl, long nrh, long ncl, long nch, uint8 **m, uint8 **tmp);
void Erosion5_parallel(long nrl, long nrh, long ncl, long nch, uint8 **m, uint8 **tmp);
void Dilatation3_parallel(long nrl, long nrh, long ncl, long nch, uint8 **m, uint8 **tmp);
void Dilatation5_parallel(long nrl, long nrh, long ncl, long nch, uint8 **m, uint8 **tmp);
void Fermeture3_parallel(long nrl, long nrh, long ncl, long nch, uint8 **m, uint8 **tmp);
void Overture3_parallel(long nrl, long nrh, long ncl, long nch, uint8 **m, uint8 **tmp);
void Fermeture5_parallel(long nrl, long nrh, long ncl, long nch, uint8 **m, uint8 **tmp);
void Ouverture3_parallel(long nrl, long nrh, long ncl, long nch, uint8 **m, uint8 **tmp);
void Ouverture5_parallel(long nrl, long nrh, long ncl, long nch, uint8 **m, uint8 **tmp);

#endif // __MORPHO_THREAD_H__
