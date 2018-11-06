#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <ctype.h> 
#include <string.h>
#include <math.h> /* fabs */

#include "nrdef.h"
#include "nrutil.h"
#include "morpho.h"

#define NBFRAME 300
#define THETA 30
#define N 3
#define Vmax 254
#define Vmin 1
#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

/*--------------------------------------------------------------------------------*/
void FD(long nrl, long nrh, long ncl, long nch, uint8 **ma, uint8 **m, uint8 **mFD)
/*--------------------------------------------------------------------------------*/
{
	for(int i = nrl; i < nrh; i++)
	{
		for(int j = ncl; j < nch; j++)
		{
			if(m[i][j] - ma[i][j] < THETA)
				mFD[i][j] = 0;
			else
				mFD[i][j] = 255;
		}
	}
}

/*---------------------------------------------------------------------------------------------------------------------------------------*/
void SD(long nrl, long nrh, long ncl, long nch, uint8 **m, uint8 **mSD, uint8 **MSD, uint8 **MSDa, uint8 **OSD, uint8 **VSD, uint8 **VSDa)
/*---------------------------------------------------------------------------------------------------------------------------------------*/
{
	for(int i = nrl; i < nrh; i++)
	{
		for(int j = ncl; j < nch; j++)
		{
			if(MSDa[i][j] < m[i][j] )
				MSD[i][j] = MSDa[i][j] + 1;
			else if(MSDa[i][j] > m[i][j] )
				MSD[i][j] = MSDa[i][j] - 1;
			else
				MSD[i][j] = MSDa[i][j];

			OSD[i][j] = MSD[i][j] - m[i][j];

			if(VSDa[i][j] < (N * OSD[i][j]))
				VSD[i][j] = VSDa[i][j] + 1;
			else if(VSDa[i][j] > (N * OSD[i][j]))
				VSD[i][j] = VSDa[i][j] - 1;
			else
				VSD[i][j] = VSDa[i][j];

			VSD[i][j] = MAX(MIN(VSD[i][j],Vmax),Vmin);

			if(OSD[i][j] < VSD[i][j])
				mSD[i][j] = 0;
			else
				mSD[i][j] = 255;
		}
	}
}



int main()
{
	char *filename = malloc( 100 * sizeof(char));

	/*  GENERATION DE LA MATRICE M  */
	long nrl; 
	long nrh;
	long ncl;
	long nch;
	long height, width;
	char *buffer;
	buffer = (char*) calloc(80, sizeof(char));
	uint8 **m; // image courante 
	uint8 **ma; // matrice t-1
	uint8 **mFD; // matrice après FD
	uint8 **mSD; // matrice après SD
	uint8 **MSD; // matrice Moyenne
	uint8 **MSDa; // matrice moyenne à t-1
	uint8 **OSD; // matrice différence
	uint8 **VSD; // matrice Variance V
	uint8 **VSDa; // matrice variance V à t-1
	uint8 **tmp;
	FILE *file;

	file = fopen("hall/hall000000.pgm","rb");
	
	if (file==NULL)
		nrerror("ouverture du fichier impossible\n");
		//nrerror("ouverture du fichier %s impossible\n", filename);

	/* lecture de l'entete du fichier pgm */
	readitem(file, buffer);
	/*fscanf(fichier, "%s", buffer);*/
	if(strcmp(buffer, "P5") != 0)
		nrerror("entete du fichier %s invalide\n");
		//nrerror("entete du fichier %s invalide\n", filename);
	
	width  = atoi(readitem(file, buffer));
  	height = atoi(readitem(file, buffer));

	nrl = 0;
	nrh = height - 1;
	ncl = 0;
	nch = width - 1;

	m = ui8matrix(nrl, nrh, ncl, nch);
	/* POUR CALCUL FD */
	ma = ui8matrix(nrl, nrh, ncl, nch);
	mFD = ui8matrix(nrl, nrh, ncl, nch);
	/* POUR CALCUL SD */
	mSD = ui8matrix(nrl, nrh, ncl, nch);
	MSD = ui8matrix(nrl, nrh, ncl, nch);
	MSDa = ui8matrix(nrl, nrh, ncl, nch);
	OSD = ui8matrix(nrl, nrh, ncl, nch);
	VSD = ui8matrix(nrl, nrh, ncl, nch);
	VSDa = ui8matrix(nrl, nrh, ncl, nch);
	tmp = ui8matrix(nrl, nrh, ncl, nch);

	/*printf("nrl : %ld\n",*nrl);
	printf("nrh : %ld\n",*nrh);
	printf("ncl : %ld\n",*ncl);
	printf("nch : %ld\n",*nch);*/
	/* ########################## */
	for(int step = 0; step < NBFRAME; step++)
	{
		sprintf(filename,"hall/hall%06d.pgm", step);
		printf("name : %s\n", filename);
                CopyMatrice(nrl, nrh, ncl, nch, ma, m);
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
			/*FD(*nrl, *nrh, *ncl, *nch, ma, m, mFD);
			sprintf(filename,"FD/hall%06d.pgm", i);
			SavePGM_ui8matrix(mFD, *nrl, *nrh, *ncl, *nch, filename);

			Fermeture3(*nrl, *nrh, *ncl, *nch, mFD, tmp);
			Fermeture5(*nrl, *nrh, *ncl, *nch, mFD, tmp);
			sprintf(filename,"FD+morpho/hall%06d.pgm", i);
			SavePGM_ui8matrix(mFD, *nrl, *nrh, *ncl, *nch, filename);*/

			SD(nrl, nrh, ncl, nch, m, mSD, MSD, MSDa, OSD, VSD, VSDa);
			sprintf(filename,"SD/hall%06d.pgm", step);
			SavePGM_ui8matrix(mSD, nrl, nrh, ncl, nch, filename);
			
			Ouverture3(nrl, nrh, ncl, nch, mSD, tmp);
			Fermeture3(nrl, nrh, ncl, nch, mSD, tmp);
			Ouverture5(nrl, nrh, ncl, nch, mSD, tmp);
			Fermeture5(nrl, nrh, ncl, nch, mSD, tmp);
			
			
			sprintf(filename,"SD+morpho/hall%06d.pgm", step);
			SavePGM_ui8matrix(mSD, nrl, nrh, ncl, nch, filename);
		}
	}

	printf("end\n");
	return 0;
}
