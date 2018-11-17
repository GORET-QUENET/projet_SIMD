/* ------------------- */
/* --- mouvement.h --- */
/* ------------------- */


#ifndef __NR_MOUVEMENT_H__
#define __NR_MOUVEMENT_H__

#define THETA 16
#define N 3
#define Vmax 254
#define Vmin 16
#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))


void SEQ_FD(long nrl, long nrh, long ncl, long nch, uint8 **ma, uint8 **m, uint8 **mFD);
void SEQ_SD(long nrl, long nrh, long ncl, long nch, uint8 **m, uint8 **mSD, uint8 **MSD, uint8 **MSDa, uint8 **OSD, uint8 **VSD, uint8 **VSDa);

#endif // __NR_MOUVEMENT_H__
