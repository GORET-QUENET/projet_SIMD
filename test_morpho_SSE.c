#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <ctype.h> 
#include <string.h>
#include <time.h>

#include "nrdef.h"
#include "nrutil.h"
#include "test_morpho_SSE.h"
#include "ROC.h"

#define NBTESTMAX 10

/*----------------------------------------------------------------------*/
void test_morpho_SD_SSE(uint8 ***m, long nrl, long nrh, long ncl, long nch)
/*----------------------------------------------------------------------*/
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
				Fermeture5_SSE(nrl, nrh, ncl, nch, m[step], tmp);
				Ouverture5_SSE(nrl, nrh, ncl, nch, m[step], tmp);
			}
		}
	}
	/***********************************/
	/*   Calcul des temps de calculs   */
	/***********************************/
	clock_gettime(CLOCK_MONOTONIC, &t2);
	temps = (double) (t2.tv_sec - t1.tv_sec );
	temps += (double) (t2.tv_nsec - t1.tv_nsec)/ 1000000000.0;
	printf("|Pour morpho_SD_SSE:\t\t\t|\n|t = %.3f \t\t\t\t|\n",temps);
	free_ui8matrix(tmp, nrl - 2, nrh + 2, ncl - 2, nch + 2);
	/***********************************/
	/*         Calcul de la ROC        */
	/***********************************/
	test_ROC_morpho_SD_SSE(m[1], nrl, nrh, ncl, nch);
}



/*----------------------------------------------------------------------*/
void test_morpho_FD_SSE(uint8 ***m, long nrl, long nrh, long ncl, long nch)
/*----------------------------------------------------------------------*/
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
				Fermeture5_SSE(nrl, nrh, ncl, nch, m[step], tmp);
				Erosion3_SSE(nrl, nrh, ncl, nch, m[step], tmp);
				Ouverture5_SSE(nrl, nrh, ncl, nch, m[step], tmp);
			}
		}
	}
	
	/***********************************/
	/*   Calcul des temps de calculs   */
	/***********************************/
	clock_gettime(CLOCK_MONOTONIC, &t2);
	temps = (double) (t2.tv_sec - t1.tv_sec);
	temps += (double) (t2.tv_nsec - t1.tv_nsec)/ 1000000000.0;
	printf("|Pour morpho_FD_SSE:\t\t\t|\n|t = %.3f \t\t\t\t|\n",temps);
	free_ui8matrix(tmp, nrl - 2, nrh + 2, ncl - 2, nch + 2);
	/***********************************/
	/*         Calcul de la ROC        */
	/***********************************/
	test_ROC_morpho_FD_SSE(m[1], nrl, nrh, ncl, nch);
}
