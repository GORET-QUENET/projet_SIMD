#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <ctype.h> 
#include <string.h>

#include "nrdef.h"
#include "nrutil.h"

#include "ROC.h"

/*---------------------------------------------------------------------------------*/
void Fill_ROC(uint8 **m, long nrl, long nrh, long ncl, long nch, int ROC[], int step)
/*---------------------------------------------------------------------------------*/
{
	uint8** v;
	v = ui8matrix(nrl - 2, nrh + 2, ncl - 2, nch + 2);
	char *filename = malloc( 100 * sizeof(char));
	sprintf(filename,"../verite/hall%06d.pgm", step);
	MLoadPGM_ui8matrix(filename, nrl, nrh, ncl, nch, v);

	for(int i = nrl; i <= nrh; i++)
	{
		for(int j = ncl; j <= nch; j++)
		{
			if(v[i][j] == 255 && m[i][j] == 255)
				ROC[0]++;	//VP
			else if(v[i][j] == 255 && m[i][j] == 0)
				ROC[1]++;	//FN
			else if(v[i][j] == 0 && m[i][j] == 255)
				ROC[2]++;	//FP
			else if(v[i][j] == 0 && m[i][j] == 0)
				ROC[3]++;	//VN
			else
				printf("Error : grey value detected v=%d, m=%d, step=%d, i=%d, j=%d\n",v[i][j],m[i][j], step, i, j);
		}
	}
	free_ui8matrix(v, nrl - 2, nrh + 2, ncl - 2, nch + 2);
	free(filename);
}

/*---------------------------------------------------------------------------------------*/
void test_ROC(uint8 **m, long nrl, long nrh, long ncl, long nch, int ROC[], char* AlgoName)
/*---------------------------------------------------------------------------------------*/
{
	char *filename = malloc( 100 * sizeof(char));
	for(int step = 10; step < NBFRAME; step+=10)
	{
		sprintf(filename,"../%s/hall%06d.pgm", AlgoName, step);
		MLoadPGM_ui8matrix(filename, nrl, nrh, ncl, nch, m);
		Fill_ROC(m, nrl, nrh, ncl, nch, ROC, step);
	}
	free(filename);
}


/*-----------------------*/
void Affiche_ROC(int ROC[])
/*-----------------------*/
{
	printf("|                                       |\n");
	printf("|  ROC moyennée pour une image sur 30   |\n");
	printf("|VP = %d\t   |    FN = %d\t|\n|FP = %d\t   |    VN = %d\t|\n",ROC[0]/29,ROC[1]/29,ROC[2]/29,ROC[3]/29);
	float p = (float)(ROC[0]) / (float)(ROC[0] + ROC[1] + ROC[2]) * 100.0;
	printf("|%.2f%% de pixels en mouvement vrais    |\n", p);
	printf("|_______________________________________|\n");
	printf("|                                       |\n");
}










