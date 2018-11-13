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
#define THETA 20
#define N 3
#define Vmax 200
#define Vmin 10
#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

vuint8 _mm_MAX_epi8(vuint8 a, vuint b)
{	
	return _mm_or_si128(_mm_and_si128(_mm_cmplt_epi8(a, b),b),_mm_andnot_si128(_mm_cmplt_epi8(a, b), a));
}

vuint8 _mm_MIN_epi8(vuint a, vuint b)
{
	return _mm_or_si128(_mm_and_si128(_mm_cmplt_epi8(a, b),a),_mm_andnot_si128(_mm_cmplt_epi8(a, b), b));
}


vuint8 _mm_sub_abs_epi8(vuint8 a, vuint8 b)
{
	return _mm_MAX_epi8(_mm_sub_epi8(a,b), _mm_sub_epi8(b,a));
}

/*--------------------------------------------------------------------------------*/
void FD(long nrl, long nrh, long ncl, long nch, uint8 **ma, uint8 **m, uint8 **mFD)
/*--------------------------------------------------------------------------------*/
{
	vuint8 vm, vma, vmFD, cmpl;
	vuint8 vblanc, vnoir, vTHETA;

	vblanc = _mm_set1_epi8(255);
	vnoir = _mm_set1_epi8(0);
	vTHETA = _mm_set1_epi8(THETA);

	for(int i = nrl; i <= nrh; i++)
	{
		for(int j = ncl; j <= nch; j+=16)
		{
			vm = _mm_set_epi8(m[i][j+0 ],m[i][j+1 ],m[i][j+2 ],m[i][j+3 ],
				          m[i][j+4 ],m[i][j+5 ],m[i][j+6 ],m[i][j+7 ],
				          m[i][j+8 ],m[i][j+9 ],m[i][j+10],m[i][j+11],
				          m[i][j+12],m[i][j+13],m[i][j+14],m[i][j+15]);
			
			vma = _mm_set_epi8(ma[i][j+0 ],ma[i][j+1 ],ma[i][j+2 ],ma[i][j+3 ],
				           ma[i][j+4 ],ma[i][j+5 ],ma[i][j+6 ],ma[i][j+7 ],
				           ma[i][j+8 ],ma[i][j+9 ],ma[i][j+10],ma[i][j+11],
				           ma[i][j+12],ma[i][j+13],ma[i][j+14],ma[i][j+15]);

			cmpl = _mm_cmplt_epi8(_mm_sub_abs_epi8(vm,vma), vTHETA);	//if( |vm - vma| < vTHETA)
			vmFD = _mm_or_si128(_mm_and_si128(cmpl,vnoir),			//if( |vm - vma| < vTHETA) -> vmFD = 0
			       _mm_andnot_si128(cmpl, vblanc));				//else -> vmFD = 255

			mFD[i][j+15] = (vmFD[0] >> 0 ) & 255;
			mFD[i][j+14] = (vmFD[0] >> 8 ) & 255;
			mFD[i][j+13] = (vmFD[0] >> 16) & 255;
			mFD[i][j+12] = (vmFD[0] >> 24) & 255;
			mFD[i][j+11] = (vmFD[0] >> 32) & 255;
			mFD[i][j+10] = (vmFD[0] >> 40) & 255;
			mFD[i][j+9 ] = (vmFD[0] >> 48) & 255;
			mFD[i][j+8 ] = (vmFD[0] >> 56) & 255;
			mFD[i][j+7 ] = (vmFD[1] >> 0 ) & 255;
			mFD[i][j+6 ] = (vmFD[1] >> 8 ) & 255;
			mFD[i][j+5 ] = (vmFD[1] >> 16) & 255;
			mFD[i][j+4 ] = (vmFD[1] >> 24) & 255;
			mFD[i][j+3 ] = (vmFD[1] >> 32) & 255;
			mFD[i][j+2 ] = (vmFD[1] >> 40) & 255;
			mFD[i][j+1 ] = (vmFD[1] >> 48) & 255;
			mFD[i][j+0 ] = (vmFD[1] >> 56) & 255;
		}
	}
}


/*--------------------------------------------------------------------------------*/
void SEQ_FD(long nrl, long nrh, long ncl, long nch, uint8 **ma, uint8 **m, uint8 **mFD)
/*--------------------------------------------------------------------------------*/
{
	for(int i = nrl; i <= nrh; i++)
	{
		for(int j = ncl; j <= nch; j++)
		{
			if(abs(m[i][j] - ma[i][j]) < THETA)
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

	one = _mm_set1_epi8(1);
	vVmax = _mm_set1_epi8(Vmax);
	vVmin = _mm_set1_epi8(Vmin);
	vblanc = _mm_set1_epi8(255);
	vnoir = _mm_set1_epi8(0);

	for(int i = nrl; i <= nrh; i++)
	{
		for(int j = ncl; j <= nch; j+=16)
		{
			vm = _mm_set_epi8(m[i][j+0 ],m[i][j+1 ],m[i][j+2 ],m[i][j+3 ],
				          m[i][j+4 ],m[i][j+5 ],m[i][j+6 ],m[i][j+7 ],
				          m[i][j+8 ],m[i][j+9 ],m[i][j+10],m[i][j+11],
				          m[i][j+12],m[i][j+13],m[i][j+14],m[i][j+15]);

			vMSDa = _mm_set_epi8(MSDa[i][j+0 ],MSDa[i][j+1 ],MSDa[i][j+2 ],MSDa[i][j+3 ],
					     MSDa[i][j+4 ],MSDa[i][j+5 ],MSDa[i][j+6 ],MSDa[i][j+7 ],
					     MSDa[i][j+8 ],MSDa[i][j+9 ],MSDa[i][j+10],MSDa[i][j+11],
					     MSDa[i][j+12],MSDa[i][j+13],MSDa[i][j+14],MSDa[i][j+15]);

			vVSDa = _mm_set_epi8(VSDa[i][j+0 ],VSDa[i][j+1 ],VSDa[i][j+2 ],VSDa[i][j+3 ],
					     VSDa[i][j+4 ],VSDa[i][j+5 ],VSDa[i][j+6 ],VSDa[i][j+7 ],
					     VSDa[i][j+8 ],VSDa[i][j+9 ],VSDa[i][j+10],VSDa[i][j+11],
					     VSDa[i][j+12],VSDa[i][j+13],VSDa[i][j+14],VSDa[i][j+15]);

			cmpl = _mm_cmplt_epi8(vMSDa, vm);	//if(vMSDa < vm)
			cmpg = _mm_cmpgt_epi8(vMSDa, vm);	//if(vMSDa > vm)


			vMSD = _mm_or_si128(_mm_and_si128(cmpl,_mm_add_epi8(vMSDa,one)),				//if(vMSDa < vm ) -> vMSD = vMSDa + 1
			       _mm_andnot_si128(cmpl, _mm_or_si128(_mm_and_si128(cmpg, _mm_sub_epi8(vMSDa,one)),	//else if(vMSDa > vm ) -> vMSD = vMSDa - 1
			       _mm_andnot_si128(cmpg, vMSDa))));							//else -> vMSD = vMSDa

			vOSD = _mm_sub_abs_epi8(vMSD, vm);

			NvOSD = _mm_add_epi8(vOSD,_mm_add_epi8(vOSD,vOSD));	//NvOSD = N * vOSD = 3 * vOSD
			cmpl = _mm_cmplt_epi8(vVSDa, NvOSD);			//if(vVSDa < vOSD * 3)
			cmpg = _mm_cmpgt_epi8(vVSDa, NvOSD);			//if(vVSDa > vOSD * 3)

			vVSD = _mm_or_si128(_mm_and_si128(cmpl,_mm_add_epi8(vVSDa,one)),				//if(vVSDa < vOSD * vN) -> vVSD = vVSDa + 1
			       _mm_andnot_si128(cmpl, _mm_or_si128(_mm_and_si128(cmpg, _mm_sub_epi8(vVSDa,one)),	//else if(vVSDa > vOSD * vN) -> vVSD = vVSDa - 1
			       _mm_andnot_si128(cmpg, vVSDa))));							//else -> vVSD = vVSDa


			vVSD = _mm_MAX_epi8(_mm_MIN_epi8(vVSD,vVmax),vVmin);	


			cmpl = _mm_cmplt_epi8(vOSD, vVSD);		//if(vOSD < vVSD)
			vmSD = _mm_or_si128(_mm_and_si128(cmpl,vnoir),	//if(vOSD < vVSD) -> vmSD = 0
			       _mm_andnot_si128(cmpl, vblanc));		//else -> vmSD = 255


			mSD[i][j+15] = (vmSD[0] >> 0 ) & 255;
			mSD[i][j+14] = (vmSD[0] >> 8 ) & 255;
			mSD[i][j+13] = (vmSD[0] >> 16) & 255;
			mSD[i][j+12] = (vmSD[0] >> 24) & 255;
			mSD[i][j+11] = (vmSD[0] >> 32) & 255;
			mSD[i][j+10] = (vmSD[0] >> 40) & 255;
			mSD[i][j+9 ] = (vmSD[0] >> 48) & 255;
			mSD[i][j+8 ] = (vmSD[0] >> 56) & 255;
			mSD[i][j+7 ] = (vmSD[1] >> 0 ) & 255;
			mSD[i][j+6 ] = (vmSD[1] >> 8 ) & 255;
			mSD[i][j+5 ] = (vmSD[1] >> 16) & 255;
			mSD[i][j+4 ] = (vmSD[1] >> 24) & 255;
			mSD[i][j+3 ] = (vmSD[1] >> 32) & 255;
			mSD[i][j+2 ] = (vmSD[1] >> 40) & 255;
			mSD[i][j+1 ] = (vmSD[1] >> 48) & 255;
			mSD[i][j+0 ] = (vmSD[1] >> 56) & 255;


			MSD[i][j+15] = (vMSD[0] >> 0 ) & 255;
			MSD[i][j+14] = (vMSD[0] >> 8 ) & 255;
			MSD[i][j+13] = (vMSD[0] >> 16) & 255;
			MSD[i][j+12] = (vMSD[0] >> 24) & 255;
			MSD[i][j+11] = (vMSD[0] >> 32) & 255;
			MSD[i][j+10] = (vMSD[0] >> 40) & 255;
			MSD[i][j+9 ] = (vMSD[0] >> 48) & 255;
			MSD[i][j+8 ] = (vMSD[0] >> 56) & 255;
			MSD[i][j+7 ] = (vMSD[1] >> 0 ) & 255;
			MSD[i][j+6 ] = (vMSD[1] >> 8 ) & 255;
			MSD[i][j+5 ] = (vMSD[1] >> 16) & 255;
			MSD[i][j+4 ] = (vMSD[1] >> 24) & 255;
			MSD[i][j+3 ] = (vMSD[1] >> 32) & 255;
			MSD[i][j+2 ] = (vMSD[1] >> 40) & 255;
			MSD[i][j+1 ] = (vMSD[1] >> 48) & 255;
			MSD[i][j+0 ] = (vMSD[1] >> 56) & 255;

			VSD[i][j+15] = (vVSD[0] >> 0 ) & 255;
			VSD[i][j+14] = (vVSD[0] >> 8 ) & 255;
			VSD[i][j+13] = (vVSD[0] >> 16) & 255;
			VSD[i][j+12] = (vVSD[0] >> 24) & 255;
			VSD[i][j+11] = (vVSD[0] >> 32) & 255;
			VSD[i][j+10] = (vVSD[0] >> 40) & 255;
			VSD[i][j+9 ] = (vVSD[0] >> 48) & 255;
			VSD[i][j+8 ] = (vVSD[0] >> 56) & 255;
			VSD[i][j+7 ] = (vVSD[1] >> 0 ) & 255;
			VSD[i][j+6 ] = (vVSD[1] >> 8 ) & 255;
			VSD[i][j+5 ] = (vVSD[1] >> 16) & 255;
			VSD[i][j+4 ] = (vVSD[1] >> 24) & 255;
			VSD[i][j+3 ] = (vVSD[1] >> 32) & 255;
			VSD[i][j+2 ] = (vVSD[1] >> 40) & 255;
			VSD[i][j+1 ] = (vVSD[1] >> 48) & 255;
			VSD[i][j+0 ] = (vVSD[1] >> 56) & 255;
		}
	}
}

/*---------------------------------------------------------------------------------------------------------------------------------------*/
void SEQ_SD(long nrl, long nrh, long ncl, long nch, uint8 **m, uint8 **mSD, uint8 **MSD, uint8 **MSDa, uint8 **OSD, uint8 **VSD, uint8 **VSDa)
/*---------------------------------------------------------------------------------------------------------------------------------------*/
{
	for(int i = nrl; i <= nrh; i++)
	{
		for(int j = ncl; j <= nch; j++)
		{
			if(MSDa[i][j] < m[i][j] )
				MSD[i][j] = MSDa[i][j] + 1;
			else if(MSDa[i][j] > m[i][j] )
				MSD[i][j] = MSDa[i][j] - 1;
			else
				MSD[i][j] = MSDa[i][j];

			OSD[i][j] = abs(MSD[i][j] - m[i][j]);

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

int64_t clocktime() {
   struct timeval full_time;

   gettimeofday(&full_time, NULL);
   return (int64_t) ((full_time.tv_usec + full_time.tv_sec * 1000000) / 1000);
}

void test_visage()
{
	long nrl; 
	long nrh;
	long ncl;
	long nch;
	//int64_t end;
	//int64_t start;
	uint8 **m; // image courante 
	uint8 **tmp;
	uint8 i;
	char *filename = malloc( 100 * sizeof(char));

	for (i = 0; i < 8; i++){
		sprintf(filename,"visage/test.pgm");
		m = LoadPGM_ui8matrix(filename, &nrl, &nrh, &ncl, &nch);
		tmp = ui8matrix(nrl - 2, nrh + 2, ncl - 2, nch + 2);
		inverser_matrice(m, nrl, nrh, ncl, nch);
		//afficher_matrice(m, nrl, nrh, ncl, nch);
		switch(i){
		case 0: 
			//start = clocktime();	
			Erosion3(nrl, nrh, ncl, nch, m, tmp);
			sprintf(filename,"visage/test_erosion3.pgm");
			break;
		case 1:
			Dilatation3(nrl, nrh, ncl, nch, m, tmp);
                	sprintf(filename,"visage/test_dilatation3.pgm");
                	break;
		case 2:
			Erosion5(nrl, nrh, ncl, nch, m, tmp);
                	sprintf(filename,"visage/test_erosion5.pgm");
               		break;
		case 3:
			Dilatation5(nrl, nrh, ncl, nch, m, tmp);
                        sprintf(filename,"visage/test_dilatation5.pgm");
                        break;
		case 4:
                        Fermeture3(nrl, nrh, ncl, nch, m, tmp);
                        sprintf(filename,"visage/test_fermeture3.pgm");
                        break;
		case 5:
                        Fermeture5(nrl, nrh, ncl, nch, m, tmp);
                        sprintf(filename,"visage/test_fermeture5.pgm");
                        break;
		case 6:
                        Ouverture3(nrl, nrh, ncl, nch, m, tmp);
                        sprintf(filename,"visage/test_ouverture3.pgm");
                        break;
		case 7:
			//start = clocktime();
                        Ouverture5(nrl, nrh, ncl, nch, m, tmp);
                        sprintf(filename,"visage/test_ouverture5.pgm");
			//end = clocktime();
                        //printf("Ouverture5_seq completed   %8lld ms\n", end - start);
                        break;
		default:
			return;
		}	
		//afficher_matrice(m, nrl, nrh, ncl, nch);
        	inverser_matrice(m, nrl, nrh, ncl, nch);	
		SavePGM_ui8matrix(m, nrl, nrh, ncl, nch, filename);
	}
}


void Generate_ROC(uint8 **m, long nrl, long nrh, long ncl, long nch, int ROC[], int step)
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
			//else
				//printf("Error : grey value detected v=%d, m=%d, step=%d, i=%d, j=%d\n",v[i][j],m[i][j], step, i, j);
		}
	}
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
		int ROC_FD[4] = {0}; //VP, FN, FP, VN
		int ROC_SD[4] = {0}; //VP, FN, FP, VN
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
				FD(nrl, nrh, ncl, nch, ma, m, mFD);
				sprintf(filename,"FD/hall%06d.pgm", step);
				SavePGM_ui8matrix(mFD, nrl, nrh, ncl, nch, filename);

				/*Fermeture3(*nrl, *nrh, *ncl, *nch, mFD, tmp);
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

				if(step%10 == 0)
				{
					Generate_ROC(mSD, nrl, nrh, ncl, nch, ROC_SD, step);
					Generate_ROC(mFD, nrl, nrh, ncl, nch, ROC_FD, step);
				}
			}
		}
		t2 = clock();
		//test_visage();
		t = (float)(t2 - t1) / CLOCKS_PER_SEC;
		printf("end after %fs\n", t);
		printf("Pour SD :\nVP = %d, FN = %d, FP = %d, VN = %d\n",ROC_SD[0]/30,ROC_SD[1]/30,ROC_SD[2]/30,ROC_SD[3]/30);
		float p = (float)(ROC_SD[0] + ROC_SD[3]) / (float)(ROC_SD[0] + ROC_SD[1] + ROC_SD[2] + ROC_SD[3]) * 100.0;
		printf("%.2f poucent de pixels vraie\n\n", p);
		printf("Pour FD :\nVP = %d, FN = %d, FP = %d, VN = %d\n",ROC_FD[0]/30,ROC_FD[1]/30,ROC_FD[2]/30,ROC_FD[3]/30);
		p = (float)(ROC_FD[0] + ROC_FD[3]) / (float)(ROC_FD[0] + ROC_FD[1] + ROC_FD[2] + ROC_FD[3]) * 100.0;
		printf("%.2f poucent de pixels vraie\n", p);

	}
	else
	{ // argc > 1, alors MODE TEST
		test_visage();
		printf("end\n");
	}
	

	return 0;
}
