#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <ctype.h> 
#include <string.h>
#include <time.h>

#include "nrdef.h"
#include "nrutil.h"
#include "test_mouvement.h"
#include "ROC.h"


/*---------------------------------------------------------------*/
void test_FD_SEQ(uint8 **m, long nrl, long nrh, long ncl, long nch)
/*---------------------------------------------------------------*/
{
	/***********************************/
	/*         Calcul de la FD         */
	/***********************************/
	char *filename = malloc( 100 * sizeof(char));
	uint8 **ma; // matrice t-1
	uint8 **mFD; // matrice après FD
	int ROC[4] = {0}; //VP, FN, FP, VN
	ma = ui8matrix(nrl - 2, nrh + 2, ncl - 2, nch + 2);
	mFD = ui8matrix(nrl - 2, nrh + 2, ncl - 2, nch + 2);
	double temps;
	struct timespec t1, t2,tfile;
	clock_gettime(CLOCK_MONOTONIC, &t1);

	for(int step = 0; step < NBFRAME; step++)
	{
		sprintf(filename,"hall/hall%06d.pgm", step);
		CopyMatrice(nrl, nrh, ncl, nch, ma, m);
		MLoadPGM_ui8matrix(filename, nrl, nrh, ncl, nch, m);

		if(step)
		{
			SEQ_FD(nrl, nrh, ncl, nch, ma, m, mFD);
			sprintf(filename,"FD/hall%06d.pgm", step);
			SavePGM_ui8matrix(mFD, nrl, nrh, ncl, nch, filename);
		}
	}

	/***********************************/
	/*   Calcul des temps de calculs   */
	/***********************************/
	clock_gettime(CLOCK_MONOTONIC, &t2);
	for(int step = 0; step < NBFRAME; step++)
	{
		if(step)
		{
			sprintf(filename,"SD/hall%06d.pgm", step);
			MLoadPGM_ui8matrix(filename, nrl, nrh, ncl, nch, m);			
			
			sprintf(filename,"SD/hall%06d.pgm", step);
			SavePGM_ui8matrix(m, nrl, nrh, ncl, nch, filename);
		}
	}
	clock_gettime(CLOCK_MONOTONIC, &tfile);
	temps = (double) (t2.tv_sec - t1.tv_sec - (tfile.tv_sec - t2.tv_sec));
	temps += (double) (t2.tv_nsec - t1.tv_nsec - (tfile.tv_nsec - t2.tv_nsec))/ 1000000000.0;
	printf("|Pour FD_SEQ:\t\t\t\t|\n|t = %.3f \t\t\t\t|\n",temps);

	/***********************************/
	/*         Calcul de la ROC        */
	/***********************************/
	test_ROC(m, nrl, nrh, ncl, nch, ROC, "FD");
	Affiche_ROC(ROC);
}


/*---------------------------------------------------------------*/
void test_SD_SEQ(uint8 **m, long nrl, long nrh, long ncl, long nch)
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
	double temps;
	struct timespec t1, t2,tfile;
	clock_gettime(CLOCK_MONOTONIC, &t1);

	for(int step = 0; step < NBFRAME; step++)
	{
		sprintf(filename,"hall/hall%06d.pgm", step);
		CopyMatrice(nrl, nrh, ncl, nch, MSDa, MSD);
		CopyMatrice(nrl, nrh, ncl, nch, VSDa, VSD);
		MLoadPGM_ui8matrix(filename, nrl, nrh, ncl, nch, m);

		if(step == 0)
		{
			CopyMatrice(nrl, nrh, ncl, nch, MSD, m);
			CopyValue(nrl, nrh, ncl, nch, VSD, Vmin);
		}	
		else
		{
			SEQ_SD(nrl, nrh, ncl, nch, m, mSD, MSD, MSDa, OSD, VSD, VSDa);
			sprintf(filename,"SD/hall%06d.pgm", step);
			SavePGM_ui8matrix(mSD, nrl, nrh, ncl, nch, filename);
		}
	}

	/***********************************/
	/*   Calcul des temps de calculs   */
	/***********************************/
	clock_gettime(CLOCK_MONOTONIC, &t2);
	for(int step = 0; step < NBFRAME; step++)
	{
		if(step)
		{
			sprintf(filename,"SD/hall%06d.pgm", step);
			MLoadPGM_ui8matrix(filename, nrl, nrh, ncl, nch, m);			
			
			sprintf(filename,"SD/hall%06d.pgm", step);
			SavePGM_ui8matrix(m, nrl, nrh, ncl, nch, filename);
		}
	}
	clock_gettime(CLOCK_MONOTONIC, &tfile);
	temps = (double) (t2.tv_sec - t1.tv_sec - (tfile.tv_sec - t2.tv_sec));
	temps += (double) (t2.tv_nsec - t1.tv_nsec - (tfile.tv_nsec - t2.tv_nsec))/ 1000000000.0;
	printf("|Pour SD_SEQ:\t\t\t\t|\n|t = %.3f \t\t\t\t|\n",temps);

	/***********************************/
	/*         Calcul de la ROC        */
	/***********************************/
	test_ROC(m, nrl, nrh, ncl, nch, ROC, "SD");
	Affiche_ROC(ROC);
}

