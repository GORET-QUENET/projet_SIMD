/* ---------------- */
/* --- morpho.h --- */
/* ---------------- */


#ifndef __MORPHO_H__
#define __MORPHO_H__

/* -------------------------- */
/* -- matrice & value copy -- */
/* -------------------------- */

void CopyMatrice(long nrl, long nrh, long ncl, long nch, uint8 **dest, uint8 **src);
void CopyValue(long nrl, long nrh, long ncl, long nch, uint8 **dest, int val);


void inverser_matrice(uint8 **m, long nrl, long nrh, long ncl, long nch);
void afficher_matrice(uint8 **m, long nrl, long nrh, long ncl, long nch);


/* -------------------------------- */
/* -- morpho de la matrice image -- */
/* -------------------------------- */

void Erosion3(long nrl, long nrh, long ncl, long nch, uint8 **m, uint8 **tmp);
void Erosion5(long nrl, long nrh, long ncl, long nch, uint8 **m, uint8 **tmp);
void Dilatation3(long nrl, long nrh, long ncl, long nch, uint8 **m, uint8 **tmp);
void Dilatation5(long nrl, long nrh, long ncl, long nch, uint8 **m, uint8 **tmp);
void Fermeture3(long nrl, long nrh, long ncl, long nch, uint8 **m, uint8 **tmp);
void Fermeture5(long nrl, long nrh, long ncl, long nch, uint8 **m, uint8 **tmp);
void Ouverture3(long nrl, long nrh, long ncl, long nch, uint8 **m, uint8 **tmp);
void Ouverture5(long nrl, long nrh, long ncl, long nch, uint8 **m, uint8 **tmp);

#endif // __MORPHO_H__


