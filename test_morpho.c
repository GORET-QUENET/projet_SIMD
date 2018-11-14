#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <ctype.h> 
#include <string.h>
#include <time.h>

#include "nrdef.h"
#include "nrutil.h"
#include "test_morpho.h"

void test_morpho_SD_SEQ(uint8 **m, long nrl, long nrh, long ncl, long nch)
{
	char *filename = malloc( 100 * sizeof(char));
	uint8 **tmp;
	
	tmp = ui8matrix(nrl - 2, nrh + 2, ncl - 2, nch + 2);
	double temps;
	struct timespec t1, t2,tfile;
	clock_gettime(CLOCK_MONOTONIC, &t1);

	for(int step = 0; step < NBFRAME; step++)
	{
		if(step)
		{
			sprintf(filename,"SD/hall%06d.pgm", step);
			MLoadPGM_ui8matrix(filename, nrl, nrh, ncl, nch, m);

			Erosion3(nrl, nrh, ncl, nch, m, tmp);
			Fermeture3(nrl, nrh, ncl, nch, m, tmp);
			Ouverture3(nrl, nrh, ncl, nch, m, tmp);
			Fermeture5(nrl, nrh, ncl, nch, m, tmp);
			Ouverture5(nrl, nrh, ncl, nch, m, tmp);
			
			
			sprintf(filename,"SD+morpho/hall%06d.pgm", step);
			SavePGM_ui8matrix(m, nrl, nrh, ncl, nch, filename);
		}
	}

	clock_gettime(CLOCK_MONOTONIC, &t2);
	for(int step = 0; step < NBFRAME; step++)
	{
		if(step)
		{
			sprintf(filename,"SD/hall%06d.pgm", step);
			MLoadPGM_ui8matrix(filename, nrl, nrh, ncl, nch, m);			
			
			sprintf(filename,"SD+morpho/hall%06d.pgm", step);
			SavePGM_ui8matrix(m, nrl, nrh, ncl, nch, filename);
		}
	}
	clock_gettime(CLOCK_MONOTONIC, &tfile);
	temps = (double) (t2.tv_sec - t1.tv_sec - (tfile.tv_sec - t2.tv_sec));
	temps += (double) (t2.tv_nsec - t1.tv_nsec - (tfile.tv_nsec - t2.tv_nsec))/ 1000000000.0;
	printf("|Pour morpho_SD_SEQ:\t\t|\n|t = %.3f \t\t\t|\n",temps);
}

void test_morpho_FD_SEQ(uint8 **m, long nrl, long nrh, long ncl, long nch)
{
	char *filename = malloc( 100 * sizeof(char));
	uint8 **tmp;
	
	tmp = ui8matrix(nrl - 2, nrh + 2, ncl - 2, nch + 2);
	double temps;
	struct timespec t1, t2,tfile;
	clock_gettime(CLOCK_MONOTONIC, &t1);

	for(int step = 0; step < NBFRAME; step++)
	{
		if(step)
		{
			sprintf(filename,"FD/hall%06d.pgm", step);
			MLoadPGM_ui8matrix(filename, nrl, nrh, ncl, nch, m);

			Fermeture3(nrl, nrh, ncl, nch, m, tmp);
			Ouverture3(nrl, nrh, ncl, nch, m, tmp);
			Ouverture5(nrl, nrh, ncl, nch, m, tmp);
			Fermeture5(nrl, nrh, ncl, nch, m, tmp);
		
			
			sprintf(filename,"FD+morpho/hall%06d.pgm", step);
			SavePGM_ui8matrix(m, nrl, nrh, ncl, nch, filename);
		}
	}

	clock_gettime(CLOCK_MONOTONIC, &t2);
	for(int step = 0; step < NBFRAME; step++)
	{
		if(step)
		{
			sprintf(filename,"SD/hall%06d.pgm", step);
			MLoadPGM_ui8matrix(filename, nrl, nrh, ncl, nch, m);			
			
			sprintf(filename,"SD+morpho/hall%06d.pgm", step);
			SavePGM_ui8matrix(m, nrl, nrh, ncl, nch, filename);
		}
	}
	clock_gettime(CLOCK_MONOTONIC, &tfile);
	temps = (double) (t2.tv_sec - t1.tv_sec - (tfile.tv_sec - t2.tv_sec));
	temps += (double) (t2.tv_nsec - t1.tv_nsec - (tfile.tv_nsec - t2.tv_nsec))/ 1000000000.0;
	printf("|Pour morpho_FD_SEQ:\t\t|\n|t = %.3f \t\t\t|\n",temps);
}
