#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <ctype.h> 
#include <string.h>
#include <math.h>
#include <time.h>

#include "nrdef.h"
#include "vnrdef.h"
#include "nrutil.h"
#include "vnrutil.h"
#include "morpho.h"

//Commande SSE : https://software.intel.com/sites/landingpage/IntrinsicsGuide/

#define NBFRAME 300
#define THETA 30
#define N 3
#define Vmax 150
#define Vmin 50
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
	vuint8 vm, vmSD, vMSD, vMSDa, vOSD, vVSD, vVSDa, cmpl, cmpg;
	vuint8 one, vVmax, vVmin, vblanc, vnoir, NvOSD;

	one = init_vuint8(1);
	vVmax = init_vuint8(Vmax);
	vVmin = init_vuint8(Vmin);
	vblanc = init_vuint8(255);
	vnoir = init_vuint8(0);

	vuint8 vmSD16, vMSD16, vVSD16;

	for(int i = nrl; i < nrh; i++)
	{
		for(int j = ncl; j < nch; j+=16)
		{
			vm = init_vuint8_all(m[i][j+0],m[i][j+1],m[i][j+2],m[i][j+3],
					     m[i][j+4],m[i][j+5],m[i][j+6],m[i][j+7],
					     m[i][j+8],m[i][j+9],m[i][j+10],m[i][j+11],
					     m[i][j+12],m[i][j+13],m[i][j+14],m[i][j+15]);
			vMSDa = init_vuint8_all(MSDa[i][j+0],MSDa[i][j+1],MSDa[i][j+2],MSDa[i][j+3],
						MSDa[i][j+4],MSDa[i][j+5],MSDa[i][j+6],MSDa[i][j+7],
						MSDa[i][j+8],MSDa[i][j+9],MSDa[i][j+10],MSDa[i][j+11],
						MSDa[i][j+12],MSDa[i][j+13],MSDa[i][j+14],MSDa[i][j+15]);
			vVSDa = init_vuint8_all(VSDa[i][j+0],VSDa[i][j+1],VSDa[i][j+2],VSDa[i][j+3],
						VSDa[i][j+4],VSDa[i][j+5],VSDa[i][j+6],VSDa[i][j+7],
						VSDa[i][j+8],VSDa[i][j+9],VSDa[i][j+10],VSDa[i][j+11],
						VSDa[i][j+12],VSDa[i][j+13],VSDa[i][j+14],VSDa[i][j+15]);

			cmpl = _mm_cmplt_epi8(vMSDa, vm);	//if(vMSDa < vm)
			cmpg = _mm_cmpgt_epi8(vMSDa, vm);	//if(vMSDa > vm)


			vMSD = _mm_or_si128(_mm_and_si128(cmpl,_mm_add_epi8(vMSDa,one)),				//if(vMSDa < vm ) -> vMSD = vMSDa + 1
			       _mm_andnot_si128(cmpl, _mm_or_si128(_mm_and_si128(cmpg, _mm_sub_epi8(vMSDa,one)),	//else if(vMSDa > vm ) -> vMSD = vMSDa - 1
			       _mm_andnot_si128(cmpg, vMSDa))));							//else -> vMSD = vMSDa

			vOSD = _mm_sub_epi8(vMSD, vm);

			NvOSD = _mm_add_epi8(vOSD,_mm_add_epi8(vOSD,vOSD));
			cmpl = _mm_cmplt_epi8(vVSDa, NvOSD);	//if(vVSDa < vOSD * 3)
			cmpg = _mm_cmpgt_epi8(vVSDa, NvOSD);	//if(vVSDa > vOSD * 3)

			vVSD = _mm_or_si128(_mm_and_si128(cmpl,_mm_add_epi8(vVSDa,one)),				//if(vVSDa < vOSD * vN) -> vVSD = vVSDa + 1
			       _mm_andnot_si128(cmpl, _mm_or_si128(_mm_and_si128(cmpg, _mm_sub_epi8(vVSDa,one)),	//else if(vVSDa > vOSD * vN) -> vVSD = vVSDa - 1
			       _mm_andnot_si128(cmpg, vVSDa))));							//else -> vVSD = vVSDa

			cmpl = _mm_cmplt_epi8(vVSD, vVmax);		//if(vVSD < vVmax)
			vVSD = _mm_or_si128(_mm_and_si128(cmpl,vVSD),	//if(vVSD < vVmax) -> vVSD = vVSD
			       _mm_andnot_si128(cmpl, vVmax));		//else -> vVSD = vVmax
			cmpl = _mm_cmplt_epi8(vVSD, vVmin);		//if(vVSD < vVmin)
			vVSD = _mm_or_si128(_mm_and_si128(cmpl,vVmin),	//if(vVSD < vVmin) -> vVSD = vVmin
			       _mm_andnot_si128(cmpl, vVSD));		//else -> vVSD = vVSD


			cmpl = _mm_cmplt_epi8(vOSD, vVSD);		//if(vOSD < vVSD)
			vmSD = _mm_or_si128(_mm_and_si128(cmpl,vnoir),	//if(vOSD < vVSD) -> vmSD = 0
			       _mm_andnot_si128(cmpl, vblanc));		//else -> vmSD = 255

			_mm_store_si128( &vmSD16, vmSD);
			_mm_store_si128( &vMSD16, vMSD);
			_mm_store_si128( &vVSD16, vVSD);

			//printf("%d, %d, %d\n", sizeof(vmSD16[0]), sizeof(vuint8), sizeof(unsigned char));

			mSD[i][j+0] = (vmSD16[0] >> 56) & 255;
			mSD[i][j+1] = (vmSD16[0] >> 48) & 255;
			mSD[i][j+2] = (vmSD16[0] >> 40) & 255;
			mSD[i][j+3] = (vmSD16[0] >> 32) & 255;
			mSD[i][j+4] = (vmSD16[0] >> 24) & 255;
			mSD[i][j+5] = (vmSD16[0] >> 16) & 255;
			mSD[i][j+6] = (vmSD16[0] >> 8) & 255;
			mSD[i][j+7] = (vmSD16[0] >> 0) & 255;
			mSD[i][j+8] = (vmSD16[1] >> 56) & 255;
			mSD[i][j+9] = (vmSD16[1] >> 48) & 255;
			mSD[i][j+10] = (vmSD16[1] >> 40) & 255;
			mSD[i][j+11] = (vmSD16[1] >> 32) & 255;
			mSD[i][j+12] = (vmSD16[1] >> 24) & 255;
			mSD[i][j+13] = (vmSD16[1] >> 16) & 255;
			mSD[i][j+14] = (vmSD16[1] >> 8) & 255;
			mSD[i][j+15] = (vmSD16[1] >> 0) & 255;


			MSD[i][j+0] = (vMSD16[0] >> 56) & 255;
			MSD[i][j+1] = (vMSD16[0] >> 48) & 255;
			MSD[i][j+2] = (vMSD16[0] >> 40) & 255;
			MSD[i][j+3] = (vMSD16[0] >> 32) & 255;
			MSD[i][j+4] = (vMSD16[0] >> 24) & 255;
			MSD[i][j+5] = (vMSD16[0] >> 16) & 255;
			MSD[i][j+6] = (vMSD16[0] >> 8) & 255;
			MSD[i][j+7] = (vMSD16[0] >> 0) & 255;
			MSD[i][j+8] = (vMSD16[1] >> 56) & 255;
			MSD[i][j+9] = (vMSD16[1] >> 48) & 255;
			MSD[i][j+10] = (vMSD16[1] >> 40) & 255;
			MSD[i][j+11] = (vMSD16[1] >> 32) & 255;
			MSD[i][j+12] = (vMSD16[1] >> 24) & 255;
			MSD[i][j+13] = (vMSD16[1] >> 16) & 255;
			MSD[i][j+14] = (vMSD16[1] >> 8) & 255;
			MSD[i][j+15] = (vMSD16[1] >> 0) & 255;

			VSD[i][j+0] = (vVSD16[0] >> 56) & 255;
			VSD[i][j+1] = (vVSD16[0] >> 48) & 255;
			VSD[i][j+2] = (vVSD16[0] >> 40) & 255;
			VSD[i][j+3] = (vVSD16[0] >> 36) & 255;
			VSD[i][j+4] = (vVSD16[0] >> 24) & 255;
			VSD[i][j+5] = (vVSD16[0] >> 16) & 255;
			VSD[i][j+6] = (vVSD16[0] >> 8) & 255;
			VSD[i][j+7] = (vVSD16[0] >> 0) & 255;
			VSD[i][j+8] = (vVSD16[1] >> 56) & 255;
			VSD[i][j+9] = (vVSD16[1] >> 48) & 255;
			VSD[i][j+10] = (vVSD16[1] >> 40) & 255;
			VSD[i][j+11] = (vVSD16[1] >> 36) & 255;
			VSD[i][j+12] = (vVSD16[1] >> 24) & 255;
			VSD[i][j+13] = (vVSD16[1] >> 16) & 255;
			VSD[i][j+14] = (vVSD16[1] >> 8) & 255;
			VSD[i][j+15] = (vVSD16[1] >> 0) & 255;
		}
	}
}

/*---------------------------------------------------------------------------------------------------------------------------------------*/
void SEQ_SD(long nrl, long nrh, long ncl, long nch, uint8 **m, uint8 **mSD, uint8 **MSD, uint8 **MSDa, uint8 **OSD, uint8 **VSD, uint8 **VSDa)
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

/**************************************************/
/*     		  TESTS UNITAIRES		  */
/**************************************************/


void inverser_matrice(uint8 **m, long nrl, long nrh, long ncl, long nch)
{
	for (uint8 i = nrl; i <= nrh; i++)
		for (uint8 j = ncl; j <= nch; j++)
		{
			if (m[i][j] == 255)
				m[i][j] = 0;
			else if (m[i][j] == 0)
				m[i][j] = 255;
			else
				printf("Error : grey value detected\n");
		}
}
			
void afficher_matrice(uint8 **m, long nrl, long nrh, long ncl, long nch)
{
        for (uint8 i = nrl; i <= nrh; i++)
	{
                for (uint8 j = ncl; j <= nch; j++)
                                printf("%d\t", m[i][j]);
		printf("\n");
	}
}


void test_visage()
{
	long nrl; 
	long nrh;
	long ncl;
	long nch;
	uint8 **m; // image courante 
	uint8 **tmp;

	char *filename = malloc( 100 * sizeof(char));
	sprintf(filename,"test2.pgm");
	m = LoadPGM_ui8matrix(filename, &nrl, &nrh, &ncl, &nch);
	
	tmp = ui8matrix(nrl - 2, nrh + 2, ncl - 2, nch + 2);

	inverser_matrice(m, nrl, nrh, ncl, nch);
	afficher_matrice(m, nrl, nrh, ncl, nch);	
	Erosion3(nrl, nrh, ncl, nch, m, tmp);
	inverser_matrice(m, nrl, nrh, ncl, nch);
	afficher_matrice(m, nrl, nrh, ncl, nch);
	sprintf(filename,"test_erosion.pgm");
	SavePGM_ui8matrix(m, nrl, nrh, ncl, nch, filename);
}

/**************************************************/
/*             FIN DES TESTS UNITAIRES            */
/**************************************************/


int main(int argc, char **argv)
{
	if (argc == 1)
	{ // EXECUTION NORMALE DU PROGRAMME
		char *filename = malloc( 100 * sizeof(char));
		float t;
		clock_t t1, t2;

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

		m = ui8matrix(nrl - 2, nrh + 2, ncl - 2, nch + 2);
		/* POUR CALCUL FD */
		ma = ui8matrix(nrl - 2, nrh + 2, ncl - 2, nch + 2);
		mFD = ui8matrix(nrl - 2, nrh + 2, ncl - 2, nch + 2);
		/* POUR CALCUL SD */
		mSD = ui8matrix(nrl - 2, nrh + 2, ncl - 2, nch + 2);
		MSD = ui8matrix(nrl - 2, nrh + 2, ncl - 2, nch + 2);
		MSDa = ui8matrix(nrl - 2, nrh + 2, ncl - 2, nch + 2);
		OSD = ui8matrix(nrl - 2, nrh + 2, ncl - 2, nch + 2);
		VSD = ui8matrix(nrl - 2, nrh + 2, ncl - 2, nch + 2);
		VSDa = ui8matrix(nrl - 2, nrh + 2, ncl - 2, nch + 2);
		tmp = ui8matrix(nrl - 2, nrh + 2, ncl - 2, nch + 2);

		/*printf("nrl : %ld\n",*nrl);
		printf("nrh : %ld\n",*nrh);
		printf("ncl : %ld\n",*ncl);
		printf("nch : %ld\n",*nch);*/
		/* ########################## */
		t1 = clock();
		for(int step = 0; step < NBFRAME; step++)
		{
			sprintf(filename,"hall/hall%06d.pgm", step);
			//printf("name : %s\n", filename);
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
			
				/*Ouverture3(nrl, nrh, ncl, nch, mSD, tmp);
				Fermeture3(nrl, nrh, ncl, nch, mSD, tmp);
				Ouverture5(nrl, nrh, ncl, nch, mSD, tmp);
				Fermeture5(nrl, nrh, ncl, nch, mSD, tmp);
			
			
				sprintf(filename,"SD+morpho/hall%06d.pgm", step);
				SavePGM_ui8matrix(mSD, nrl, nrh, ncl, nch, filename);*/
			}
		}
		t2 = clock();
		//test_visage();
		t = (float)(t2 - t1) / CLOCKS_PER_SEC;
		printf("end after %fs\n", t);

	}
	else
	{ // argc > 1, alors MODE TEST
		test_visage();
		printf("end\n");
	}
	

	return 0;
}
