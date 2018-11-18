#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <ctype.h> 
#include <string.h>
#include <time.h>

#include "nrdef.h"
#include "nrutil.h"
#include "test_morpho_SSE_THREAD.h"


/*-------------------------------------------------------------------------*/
void bench_morpho_SD_SSE_THREAD(uint8 ***m, long nrl, long nrh, long ncl, long nch, int NBTESTMAX)
/*-------------------------------------------------------------------------*/
{
	/***********************************/
	/*    Calcul des morphos sur SD    */
	/***********************************/
	uint8 **tmp;
	tmp = ui8matrix(nrl - 2, nrh + 2, ncl - 2, nch + 2);
	double temps;
	struct timespec t1, t2;
	clock_gettime(CLOCK_MONOTONIC, &t1);

	for(int nbtest = 0; nbtest < NBTESTMAX; nbtest++)
	{
		for(int step = 0; step < NBFRAME; step++)
		{
			if(step)
			{
				Fermeture5_SSE_THREAD(nrl, nrh, ncl, nch, m[step], tmp);	//Parcours 8 fois l'image
				Ouverture5_SSE_THREAD(nrl, nrh, ncl, nch, m[step], tmp);	//Parcours 8 fois l'image
			}
		}
	}

	/***********************************/
	/*   Calcul des temps de calculs   */
	/***********************************/
	clock_gettime(CLOCK_MONOTONIC, &t2);
	temps = (double) (t2.tv_sec - t1.tv_sec);
	temps += (double) (t2.tv_nsec - t1.tv_nsec)/ 1000000000.0;
	printf("|Pour morpho_SD_SSE_THREAD:\t\t|\n|t = %.3fs pour %d images \t\t|\n",temps, (NBFRAME * NBTESTMAX));
	long pixel = NBFRAME * NBTESTMAX *nrh * nch * 16;
	double debit = pixel / temps / 1000000.0;
	printf("|débit = %.3f Mpixel/s \t\t|\n",debit);
	free_ui8matrix(tmp, nrl - 2, nrh + 2, ncl - 2, nch + 2);
}


/*-------------------------------------------------------------------------*/
void bench_morpho_FD_SSE_THREAD(uint8 ***m, long nrl, long nrh, long ncl, long nch, int NBTESTMAX)
/*-------------------------------------------------------------------------*/
{
	/***********************************/
	/*    Calcul des morphos sur FD    */
	/***********************************/
	uint8 **tmp;
	tmp = ui8matrix(nrl - 2, nrh + 2, ncl - 2, nch + 2);
	double temps;
	struct timespec t1, t2;
	clock_gettime(CLOCK_MONOTONIC, &t1);

	for(int nbtest = 0; nbtest < NBTESTMAX; nbtest++)
	{
		for(int step = 0; step < NBFRAME; step++)
		{
			if(step)
			{
				Fermeture5_SSE_THREAD(nrl, nrh, ncl, nch, m[step], tmp);		//Parcours 8 fois l'image
				Erosion3_SSE_THREAD(nrl, nrh, ncl, nch, m[step], tmp);			//Parcours 2 fois l'image
				Ouverture5_SSE_THREAD(nrl, nrh, ncl, nch, m[step], tmp);		//Parcours 8 fois l'image
			}
		}
	}

	/***********************************/
	/*   Calcul des temps de calculs   */
	/***********************************/
	clock_gettime(CLOCK_MONOTONIC, &t2);
	temps = (double) (t2.tv_sec - t1.tv_sec);
	temps += (double) (t2.tv_nsec - t1.tv_nsec)/ 1000000000.0;
	printf("|Pour morpho_FD__SSE_THREAD:\t\t|\n|t = %.3fs pour %d images \t\t|\n",temps, (NBFRAME * NBTESTMAX));
	long pixel = NBFRAME * NBTESTMAX *nrh * nch * 18;
	double debit = pixel / temps / 1000000.0;
	printf("|débit = %.3f Mpixel/s \t\t|\n",debit);
	free_ui8matrix(tmp, nrl - 2, nrh + 2, ncl - 2, nch + 2);
}

