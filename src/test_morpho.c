#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <ctype.h> 
#include <string.h>
#include <time.h>

#include "nrdef.h"
#include "nrutil.h"
#include "test_morpho.h"
#include "ROC.h"

#define NBTESTMAX 10

/*----------------------------------------------------------------------*/
void test_morpho_SD_SEQ(uint8 **m, long nrl, long nrh, long ncl, long nch)
/*----------------------------------------------------------------------*/
{
	/***********************************/
	/*    Calcul des morphos sur SD    */
	/***********************************/
	char *filename = malloc( 100 * sizeof(char));
	uint8 **tmp;
	int ROC[4] = {0}; //VP, FN, FP, VN
	tmp = ui8matrix(nrl - 2, nrh + 2, ncl - 2, nch + 2);

	for(int step = 0; step < NBFRAME; step++)
	{
		if(step)
		{
			sprintf(filename,"SD/hall%06d.pgm", step);
			MLoadPGM_ui8matrix(filename, nrl, nrh, ncl, nch, m);


			Fermeture5(nrl, nrh, ncl, nch, m, tmp);
			Ouverture5(nrl, nrh, ncl, nch, m, tmp);
			

			sprintf(filename,"SD+morpho/hall%06d.pgm", step);
			SavePGM_ui8matrix(m, nrl, nrh, ncl, nch, filename);
		}
	}
	free_ui8matrix(tmp, nrl - 2, nrh + 2, ncl - 2, nch + 2);
	free(filename);
	/***********************************/
	/*         Calcul de la ROC        */
	/***********************************/
	test_ROC(m, nrl, nrh, ncl, nch, ROC, "SD+morpho");
	Affiche_ROC(ROC);
}



/*----------------------------------------------------------------------*/
void test_morpho_FD_SEQ(uint8 **m, long nrl, long nrh, long ncl, long nch)
/*----------------------------------------------------------------------*/
{
	/***********************************/
	/*    Calcul des morphos sur FD    */
	/***********************************/
	char *filename = malloc( 100 * sizeof(char));
	uint8 **tmp;
	int ROC[4] = {0}; //VP, FN, FP, VN
	tmp = ui8matrix(nrl - 2, nrh + 2, ncl - 2, nch + 2);

	for(int step = 0; step < NBFRAME; step++)
	{
		if(step)
		{
			sprintf(filename,"FD/hall%06d.pgm", step);
			MLoadPGM_ui8matrix(filename, nrl, nrh, ncl, nch, m);


			Fermeture5(nrl, nrh, ncl, nch, m, tmp);
			Erosion3(nrl, nrh, ncl, nch, m, tmp);
			Ouverture5(nrl, nrh, ncl, nch, m, tmp);
		
			
			sprintf(filename,"FD+morpho/hall%06d.pgm", step);
			SavePGM_ui8matrix(m, nrl, nrh, ncl, nch, filename);
		}
	}
	free_ui8matrix(tmp, nrl - 2, nrh + 2, ncl - 2, nch + 2);
	free(filename);
	/***********************************/
	/*         Calcul de la ROC        */
	/***********************************/
	test_ROC(m, nrl, nrh, ncl, nch, ROC, "FD+morpho");
	Affiche_ROC(ROC);
}
