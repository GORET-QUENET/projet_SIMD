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

/*---------------------------------------------------------------*/
void test_FD_SSE(uint8 **m, long nrl, long nrh, long ncl, long nch)
/*---------------------------------------------------------------*/
{
	/***********************************/
	/*         Calcul de la SD         */
	/***********************************/
	char *filename = malloc( 100 * sizeof(char));
	uint8 **ma; // matrice t-1
	uint8 **mFD; // matrice après FD
	int ROC[4] = {0}; //VP, FN, FP, VN
	ma = ui8matrix(nrl - 2, nrh + 2, ncl - 2, nch + 2);
	mFD = ui8matrix(nrl - 2, nrh + 2, ncl - 2, nch + 2);

	for(int step = 0; step < NBFRAME; step++)
	{
		sprintf(filename,"hall/hall%06d.pgm", step);
		CopyMatrice_SSE(nrl, nrh, ncl, nch, ma, m);
		MLoadPGM_ui8matrix(filename, nrl, nrh, ncl, nch, m);

		if(step > 0)
		{
			FD_SSE(nrl, nrh, ncl, nch, ma, m, mFD);
			sprintf(filename,"FD/hall%06d.pgm", step);
			SavePGM_ui8matrix(mFD, nrl, nrh, ncl, nch, filename);
		}
	}
	free_ui8matrix(ma, nrl - 2, nrh + 2, ncl - 2, nch + 2);
	free_ui8matrix(mFD, nrl - 2, nrh + 2, ncl - 2, nch + 2);
	free(filename);
	/***********************************/
	/*         Calcul de la ROC        */
	/***********************************/
	test_ROC(m, nrl, nrh, ncl, nch, ROC, "FD");
	Affiche_ROC(ROC);
}


/*---------------------------------------------------------------*/
void test_SD_SSE(uint8 **m, long nrl, long nrh, long ncl, long nch)
/*---------------------------------------------------------------*/
{
	/***********************************/
	/*         Calcul de la SD         */
	/***********************************/
	char *filename = malloc( 100 * sizeof(char));
	uint8 **mSD; // matrice après SD
	uint8 **MSD; // matrice Moyenne
	uint8 **MSDa; // matrice moyenne à t-1
	uint8 **OSD; // matrice différence
	uint8 **VSD; // matrice Variance V
	uint8 **VSDa; // matrice variance V à t-1
	int ROC[4] = {0}; //VP, FN, FP, VN
	mSD = ui8matrix(nrl - 2, nrh + 2, ncl - 2, nch + 2);
	MSD = ui8matrix(nrl - 2, nrh + 2, ncl - 2, nch + 2);
	MSDa = ui8matrix(nrl - 2, nrh + 2, ncl - 2, nch + 2);
	OSD = ui8matrix(nrl - 2, nrh + 2, ncl - 2, nch + 2);
	VSD = ui8matrix(nrl - 2, nrh + 2, ncl - 2, nch + 2);
	VSDa = ui8matrix(nrl - 2, nrh + 2, ncl - 2, nch + 2);

	for(int step = 0; step < NBFRAME; step++)
	{
		sprintf(filename,"hall/hall%06d.pgm", step);
		CopyMatrice_SSE(nrl, nrh, ncl, nch, MSDa, MSD);
		CopyMatrice_SSE(nrl, nrh, ncl, nch, VSDa, VSD);
		MLoadPGM_ui8matrix(filename, nrl, nrh, ncl, nch, m);

		if(step == 0)
		{
			CopyMatrice_SSE(nrl, nrh, ncl, nch, MSD, m);
			CopyValue_SSE(nrl, nrh, ncl, nch, VSD, Vmin);
		}	
		else
		{
			SD_SSE(nrl, nrh, ncl, nch, m, mSD, MSD, MSDa, OSD, VSD, VSDa);
			sprintf(filename,"SD/hall%06d.pgm", step);
			SavePGM_ui8matrix(mSD, nrl, nrh, ncl, nch, filename);
		}
	}
	free_ui8matrix(mSD, nrl - 2, nrh + 2, ncl - 2, nch + 2);
	free_ui8matrix(MSD, nrl - 2, nrh + 2, ncl - 2, nch + 2);
	free_ui8matrix(MSDa, nrl - 2, nrh + 2, ncl - 2, nch + 2);
	free_ui8matrix(OSD, nrl - 2, nrh + 2, ncl - 2, nch + 2);
	free_ui8matrix(VSD, nrl - 2, nrh + 2, ncl - 2, nch + 2);
	free_ui8matrix(VSDa, nrl - 2, nrh + 2, ncl - 2, nch + 2);
	free(filename);
	/***********************************/
	/*         Calcul de la ROC        */
	/***********************************/
	test_ROC(m, nrl, nrh, ncl, nch, ROC, "SD");
	Affiche_ROC(ROC);
}

