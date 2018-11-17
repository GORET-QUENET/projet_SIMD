/* -------------------- */
/* --- morpho_SSE.h --- */
/* -------------------- */


#ifndef __MORPHO_SSE_H__
#define __MORPHO_SSE_H__

void CopyMatrice_SSE(long nrl, long nrh, long ncl, long nch, uint8 **dest, uint8 **src);
void CopyValue_SSE(long nrl, long nrh, long ncl, long nch, uint8 **dest, int val);

void Erosion3_SSE(long nrl, long nrh, long ncl, long nch, uint8 **m, uint8 **tmp);
void Erosion5_SSE(long nrl, long nrh, long ncl, long nch, uint8 **m, uint8 **tmp);
void Dilatation3_SSE(long nrl, long nrh, long ncl, long nch, uint8 **m, uint8 **tmp);
void Dilatation5_SSE(long nrl, long nrh, long ncl, long nch, uint8 **m, uint8 **tmp);
void Fermeture3_SSE(long nrl, long nrh, long ncl, long nch, uint8 **m, uint8 **tmp);
void Overture3_SSE(long nrl, long nrh, long ncl, long nch, uint8 **m, uint8 **tmp);
void Fermeture5_SSE(long nrl, long nrh, long ncl, long nch, uint8 **m, uint8 **tmp);
void Ouverture3_SSE(long nrl, long nrh, long ncl, long nch, uint8 **m, uint8 **tmp);
void Ouverture5_SSE(long nrl, long nrh, long ncl, long nch, uint8 **m, uint8 **tmp);

#endif // __MORPHO_SSE_H__
