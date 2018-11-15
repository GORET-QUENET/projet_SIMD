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
	sprintf(filename,"verite/hall%06d.pgm", step);
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
		sprintf(filename,"%s/hall%06d.pgm", AlgoName, step);
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



/*----------------------------------------------------------------------*/
void test_ROC_morpho_SD_SEQ(uint8 **m, long nrl, long nrh, long ncl, long nch)
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
void test_ROC_morpho_FD_SEQ(uint8 **m, long nrl, long nrh, long ncl, long nch)
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

/*----------------------------------------------------------------------*/
void test_ROC_morpho_SD_SSE(uint8 **m, long nrl, long nrh, long ncl, long nch)
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


			Fermeture5_SSE(nrl, nrh, ncl, nch, m, tmp);
			Ouverture5_SSE(nrl, nrh, ncl, nch, m, tmp);
			
			
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
void test_ROC_morpho_FD_SSE(uint8 **m, long nrl, long nrh, long ncl, long nch)
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

			Fermeture5_SSE(nrl, nrh, ncl, nch, m, tmp);
			Erosion3_SSE(nrl, nrh, ncl, nch, m, tmp);
			Ouverture5_SSE(nrl, nrh, ncl, nch, m, tmp);
		
			
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


/*-------------------------------------------------------------------------*/
void test_ROC_morpho_SD_THREAD(uint8 **m, long nrl, long nrh, long ncl, long nch)
/*-------------------------------------------------------------------------*/
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


			Fermeture5_parallel(nrl, nrh, ncl, nch, m, tmp);
			Ouverture5_parallel(nrl, nrh, ncl, nch, m, tmp);
			
			
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


/*-------------------------------------------------------------------------*/
void test_ROC_morpho_FD_THREAD(uint8 **m, long nrl, long nrh, long ncl, long nch)
/*-------------------------------------------------------------------------*/
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

			Fermeture5_parallel(nrl, nrh, ncl, nch, m, tmp);
			Erosion3_parallel(nrl, nrh, ncl, nch, m, tmp);
			Ouverture5_parallel(nrl, nrh, ncl, nch, m, tmp);
		
			
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


/*---------------------------------------------------------------*/
void test_ROC_FD_SEQ(uint8 **m, long nrl, long nrh, long ncl, long nch)
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
void test_ROC_SD_SEQ(uint8 **m, long nrl, long nrh, long ncl, long nch)
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

/*---------------------------------------------------------------*/
void test_ROC_FD_SSE(uint8 **m, long nrl, long nrh, long ncl, long nch)
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
void test_ROC_SD_SSE(uint8 **m, long nrl, long nrh, long ncl, long nch)
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



