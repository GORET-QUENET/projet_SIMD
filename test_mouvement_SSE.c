#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <ctype.h> 
#include <string.h>
#include <time.h>

#include "nrdef.h"
#include "vnrdef.h"
#include "nrutil.h"
#include "test_mouvement_SSE.h"
#include "ROC.h"

#define NBTESTMAX 10

/*---------------------------------------------------------------*/
void test_FD_SSE(uint8 ***m, long nrl, long nrh, long ncl, long nch)
/*---------------------------------------------------------------*/
{
	/***********************************/
	/*         Calcul de la SD         */
	/***********************************/
	uint8 **ma; // matrice t-1
	uint8 **mFD; // matrice après FD
	ma = ui8matrix(nrl - 2, nrh + 2, ncl - 2, nch + 2);
	mFD = ui8matrix(nrl - 2, nrh + 2, ncl - 2, nch + 2);
	double temps;
	struct timespec t1, t2;
	clock_gettime(CLOCK_MONOTONIC, &t1);
	for(int nbtest = 0; nbtest < NBTESTMAX; nbtest++)
	{
		for(int step = 0; step < NBFRAME; step++)
		{
			if(step > 0)
			{
				CopyMatrice_SSE(nrl, nrh, ncl, nch, ma, m[step-1]);
				FD_SSE(nrl, nrh, ncl, nch, ma, m[step], mFD);
			}
		}
	}

	/***********************************/
	/*   Calcul des temps de calculs   */
	/***********************************/
	clock_gettime(CLOCK_MONOTONIC, &t2);
	temps = (double) (t2.tv_sec - t1.tv_sec);
	temps += (double) (t2.tv_nsec - t1.tv_nsec)/ 1000000000.0;
	printf("|Pour FD_SSE:\t\t\t\t|\n|t = %.3f \t\t\t\t|\n",temps);
	free_ui8matrix(ma, nrl - 2, nrh + 2, ncl - 2, nch + 2);
	free_ui8matrix(mFD, nrl - 2, nrh + 2, ncl - 2, nch + 2);
	/***********************************/
	/*         Calcul de la ROC        */
	/***********************************/
	test_ROC_FD_SSE(m[1], nrl, nrh, ncl, nch);
}


/*---------------------------------------------------------------*/
void test_SD_SSE(uint8 ***m, long nrl, long nrh, long ncl, long nch)
/*---------------------------------------------------------------*/
{
	/***********************************/
	/*         Calcul de la SD         */
	/***********************************/
	uint8 **mSD; // matrice après SD
	uint8 **MSD; // matrice Moyenne
	uint8 **MSDa; // matrice moyenne à t-1
	uint8 **OSD; // matrice différence
	uint8 **VSD; // matrice Variance V
	uint8 **VSDa; // matrice variance V à t-1
	mSD = ui8matrix(nrl - 2, nrh + 2, ncl - 2, nch + 2);
	MSD = ui8matrix(nrl - 2, nrh + 2, ncl - 2, nch + 2);
	MSDa = ui8matrix(nrl - 2, nrh + 2, ncl - 2, nch + 2);
	OSD = ui8matrix(nrl - 2, nrh + 2, ncl - 2, nch + 2);
	VSD = ui8matrix(nrl - 2, nrh + 2, ncl - 2, nch + 2);
	VSDa = ui8matrix(nrl - 2, nrh + 2, ncl - 2, nch + 2);
	double temps;
	struct timespec t1, t2;
	clock_gettime(CLOCK_MONOTONIC, &t1);

	for(int nbtest = 0; nbtest < NBTESTMAX; nbtest++)
	{
		for(int step = 0; step < NBFRAME; step++)
		{
			CopyMatrice_SSE(nrl, nrh, ncl, nch, MSDa, MSD);
			CopyMatrice_SSE(nrl, nrh, ncl, nch, VSDa, VSD);

			if(step == 0)
			{
				CopyMatrice_SSE(nrl, nrh, ncl, nch, MSD, m[0]);
				CopyValue_SSE(nrl, nrh, ncl, nch, VSD, Vmin);
			}	
			else
			{
				SD_SSE(nrl, nrh, ncl, nch, m[step], mSD, MSD, MSDa, OSD, VSD, VSDa);
			}
		}
	}

	/***********************************/
	/*   Calcul des temps de calculs   */
	/***********************************/
	clock_gettime(CLOCK_MONOTONIC, &t2);
	temps = (double) (t2.tv_sec - t1.tv_sec);
	temps += (double) (t2.tv_nsec - t1.tv_nsec)/ 1000000000.0;
	printf("|Pour SD_SSE:\t\t\t\t|\n|t = %.3f \t\t\t\t|\n",temps);
	free_ui8matrix(mSD, nrl - 2, nrh + 2, ncl - 2, nch + 2);
	free_ui8matrix(MSD, nrl - 2, nrh + 2, ncl - 2, nch + 2);
	free_ui8matrix(MSDa, nrl - 2, nrh + 2, ncl - 2, nch + 2);
	free_ui8matrix(OSD, nrl - 2, nrh + 2, ncl - 2, nch + 2);
	free_ui8matrix(VSD, nrl - 2, nrh + 2, ncl - 2, nch + 2);
	free_ui8matrix(VSDa, nrl - 2, nrh + 2, ncl - 2, nch + 2);
	/***********************************/
	/*         Calcul de la ROC        */
	/***********************************/
	test_ROC_SD_SSE(m[1], nrl, nrh, ncl, nch);
}
