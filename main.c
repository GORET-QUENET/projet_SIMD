#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <ctype.h> 
#include <string.h>
#include <math.h>
#include <time.h>
#include <sys/time.h>

#include "nrdef.h"
#include "vnrdef.h"
#include "nrutil.h"
#include "vnrutil.h"
#include "morpho.h"

//Commande SSE : https://software.intel.com/sites/landingpage/IntrinsicsGuide/

#define NBFRAME 300
#define THETA 15
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

vuint8 _mm_cmplt_epu8(vuint8 a, vuint8 b)
{
	return _mm_cmplt_epi8(_mm_sub_epi8(a,_mm_set1_epi8(128)), _mm_sub_epi8(b,_mm_set1_epi8(128)));
}

vuint8 _mm_cmpgt_epu8(vuint8 a, vuint8 b)
{
	return _mm_cmpgt_epi8(_mm_sub_epi8(a,_mm_set1_epi8(128)), _mm_sub_epi8(b,_mm_set1_epi8(128)));
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
			vm = _mm_loadu_si128((__m128i*)&m[i][j]);
			vma = _mm_loadu_si128((__m128i*)&ma[i][j]);

			cmpl = _mm_cmplt_epu8(_mm_sub_abs_epi8(vm,vma), vTHETA);	//if( |vm - vma| < vTHETA)
			vmFD = _mm_or_si128(_mm_and_si128(cmpl,vnoir),			//if( |vm - vma| < vTHETA) -> vmFD = 0
			       _mm_andnot_si128(cmpl, vblanc));				//else -> vmFD = 255

			_mm_storeu_si128((__m128i*)&mFD[i][j], vmFD);
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
			vm = _mm_loadu_si128((__m128i*)&m[i][j]);
			vMSDa = _mm_loadu_si128((__m128i*)&MSDa[i][j]);
			vVSDa = _mm_loadu_si128((__m128i*)&VSDa[i][j]);


			cmpl = _mm_cmplt_epu8(vMSDa, vm);	//if(vMSDa < vm)
			cmpg = _mm_cmpgt_epu8(vMSDa, vm);	//if(vMSDa > vm)


			vMSD = _mm_or_si128(_mm_and_si128(cmpl,_mm_add_epi8(vMSDa,one)),				//if(vMSDa < vm ) -> vMSD = vMSDa + 1
			       _mm_andnot_si128(cmpl, _mm_or_si128(_mm_and_si128(cmpg, _mm_sub_epi8(vMSDa,one)),	//else if(vMSDa > vm ) -> vMSD = vMSDa - 1
			       _mm_andnot_si128(cmpg, vMSDa))));							//else -> vMSD = vMSDa

			vOSD = _mm_sub_abs_epi8(vMSD, vm);

			NvOSD = _mm_add_epi8(vOSD,_mm_add_epi8(vOSD,vOSD));	//NvOSD = N * vOSD = 3 * vOSD
			cmpl = _mm_cmplt_epu8(vVSDa, NvOSD);			//if(vVSDa < vOSD * 3)
			cmpg = _mm_cmpgt_epu8(vVSDa, NvOSD);			//if(vVSDa > vOSD * 3)

			vVSD = _mm_or_si128(_mm_and_si128(cmpl,_mm_add_epi8(vVSDa,one)),				//if(vVSDa < vOSD * vN) -> vVSD = vVSDa + 1
			       _mm_andnot_si128(cmpl, _mm_or_si128(_mm_and_si128(cmpg, _mm_sub_epi8(vVSDa,one)),	//else if(vVSDa > vOSD * vN) -> vVSD = vVSDa - 1
			       _mm_andnot_si128(cmpg, vVSDa))));							//else -> vVSD = vVSDa


			vVSD = _mm_MAX_epi8(_mm_MIN_epi8(vVSD,vVmax),vVmin);	


			cmpl = _mm_cmplt_epu8(vOSD, vVSD);		//if(vOSD < vVSD)
			vmSD = _mm_or_si128(_mm_and_si128(cmpl,vnoir),	//if(vOSD < vVSD) -> vmSD = 0
			       _mm_andnot_si128(cmpl, vblanc));		//else -> vmSD = 255

			_mm_storeu_si128((__m128i*)&mSD[i][j], vmSD);
			_mm_storeu_si128((__m128i*)&MSD[i][j], vMSD);
			_mm_storeu_si128((__m128i*)&VSD[i][j], vVSD);
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

void test_visage()
{
	struct timeval tv1, tv2;
	long nrl; 
	long nrh;
	long ncl;
	long nch;
	uint8 **m; // image courante 
	uint8 **tmp;
	uint8 i;
	char *filename = malloc( 100 * sizeof(char));

	for (i = 0; i < 20; i++){
		sprintf(filename,"visage/test.pgm");
		m = LoadPGM_ui8matrix(filename, &nrl, &nrh, &ncl, &nch);
		tmp = ui8matrix(nrl - 2, nrh + 2, ncl - 2, nch + 2);
		inverser_matrice(m, nrl, nrh, ncl, nch);
		//afficher_matrice(m, nrl, nrh, ncl, nch);
		switch(i){
		case 0: 
			gettimeofday(&tv1, NULL);
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
                        Ouverture5(nrl, nrh, ncl, nch, m, tmp);
                        sprintf(filename,"visage/test_ouverture5.pgm");
			gettimeofday(&tv2, NULL);
			printf ("Total sequential time = %f seconds\n", (double) (tv2.tv_usec - tv1.tv_usec) / 1000000 + (double) (tv2.tv_sec - tv1.tv_sec));
                        break;
		case 8:
			gettimeofday(&tv1, NULL);
                        Erosion3_parallel(nrl, nrh, ncl, nch, m, tmp);
                        sprintf(filename,"visage/test_erosion3_par.pgm");
                        break;
		case 9:
			Erosion5_parallel(nrl, nrh, ncl, nch, m, tmp);
                        sprintf(filename,"visage/test_erosion5_par.pgm");
                        break;
		case 10:
                        Dilatation3_parallel(nrl, nrh, ncl, nch, m, tmp);
                        sprintf(filename,"visage/test_dilatation3_par.pgm");
                        break;
                case 11:
                        Dilatation5_parallel(nrl, nrh, ncl, nch, m, tmp);
                        sprintf(filename,"visage/test_dilatation5_par.pgm");
                        break;
		case 12:
                        Fermeture3_parallel(nrl, nrh, ncl, nch, m, tmp);
                        sprintf(filename,"visage/test_fermeture3_par.pgm");
                        break;
                case 13:
                        Fermeture5_parallel(nrl, nrh, ncl, nch, m, tmp);
                        sprintf(filename,"visage/test_fermeture5_par.pgm");
                        break;
		case 14:
                        Ouverture3_parallel(nrl, nrh, ncl, nch, m, tmp);
                        sprintf(filename,"visage/test_ouverture3_par.pgm");
                        break;
                case 15:
                        Ouverture5_parallel(nrl, nrh, ncl, nch, m, tmp);
                        sprintf(filename,"visage/test_ouverture5_par.pgm");
			gettimeofday(&tv2, NULL);
			printf ("Total parallel time = %f seconds\n", (double) (tv2.tv_usec - tv1.tv_usec) / 1000000 + (double) (tv2.tv_sec - tv1.tv_sec));
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
			else
				printf("Error : grey value detected v=%d, m=%d, step=%d, i=%d, j=%d\n",v[i][j],m[i][j], step, i, j);
		}
	}
}

void test_FD_SEQ(uint8 **m, long nrl, long nrh, long ncl, long nch)
{
	char *filename = malloc( 100 * sizeof(char));
	uint8 **ma; // matrice t-1
	uint8 **mFD; // matrice après FD

	ma = ui8matrix(nrl - 2, nrh + 2, ncl - 2, nch + 2);
	mFD = ui8matrix(nrl - 2, nrh + 2, ncl - 2, nch + 2);

	for(int step = 0; step < NBFRAME; step++)
	{
		sprintf(filename,"hall/hall%06d.pgm", step);
		CopyMatrice(nrl, nrh, ncl, nch, ma, m);
		MLoadPGM_ui8matrix(filename, nrl, nrh, ncl, nch, m);

		if(step > 0)
		{
			SEQ_FD(nrl, nrh, ncl, nch, ma, m, mFD);
			sprintf(filename,"FD/hall%06d.pgm", step);
			SavePGM_ui8matrix(mFD, nrl, nrh, ncl, nch, filename);
		}
	}
}

void test_SD_SEQ(uint8 **m, long nrl, long nrh, long ncl, long nch)
{
	char *filename = malloc( 100 * sizeof(char));
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
}

void test_FD_SSE(uint8 **m, long nrl, long nrh, long ncl, long nch)
{
	char *filename = malloc( 100 * sizeof(char));
	uint8 **ma; // matrice t-1
	uint8 **mFD; // matrice après FD

	ma = ui8matrix(nrl - 2, nrh + 2, ncl - 2, nch + 2);
	mFD = ui8matrix(nrl - 2, nrh + 2, ncl - 2, nch + 2);

	for(int step = 0; step < NBFRAME; step++)
	{
		sprintf(filename,"hall/hall%06d.pgm", step);
		CopyMatrice(nrl, nrh, ncl, nch, ma, m);
		MLoadPGM_ui8matrix(filename, nrl, nrh, ncl, nch, m);

		if(step > 0)
		{
			FD(nrl, nrh, ncl, nch, ma, m, mFD);
			sprintf(filename,"FD/hall%06d.pgm", step);
			SavePGM_ui8matrix(mFD, nrl, nrh, ncl, nch, filename);
		}
	}
}

void test_SD_SSE(uint8 **m, long nrl, long nrh, long ncl, long nch)
{
	char *filename = malloc( 100 * sizeof(char));
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
			SD(nrl, nrh, ncl, nch, m, mSD, MSD, MSDa, OSD, VSD, VSDa);
			sprintf(filename,"SD/hall%06d.pgm", step);
			SavePGM_ui8matrix(mSD, nrl, nrh, ncl, nch, filename);
		}
	}
}

void test_morpho_SD_SEQ(uint8 **m, long nrl, long nrh, long ncl, long nch)
{
	char *filename = malloc( 100 * sizeof(char));
	uint8 **tmp;
	
	tmp = ui8matrix(nrl - 2, nrh + 2, ncl - 2, nch + 2);
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
}

void test_morpho_FD_SEQ(uint8 **m, long nrl, long nrh, long ncl, long nch)
{
	char *filename = malloc( 100 * sizeof(char));
	uint8 **tmp;
	
	tmp = ui8matrix(nrl - 2, nrh + 2, ncl - 2, nch + 2);
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
}

void test_morpho_SD_THREAD(uint8 **m, long nrl, long nrh, long ncl, long nch)
{
	char *filename = malloc( 100 * sizeof(char));
	uint8 **tmp;
	
	tmp = ui8matrix(nrl - 2, nrh + 2, ncl - 2, nch + 2);
	for(int step = 0; step < NBFRAME; step++)
	{
		if(step)
		{
			sprintf(filename,"SD/hall%06d.pgm", step);
			MLoadPGM_ui8matrix(filename, nrl, nrh, ncl, nch, m);

			Erosion3_parallel(nrl, nrh, ncl, nch, m, tmp);
			Fermeture3_parallel(nrl, nrh, ncl, nch, m, tmp);
			Ouverture3_parallel(nrl, nrh, ncl, nch, m, tmp);
			Fermeture5_parallel(nrl, nrh, ncl, nch, m, tmp);
			Ouverture5_parallel(nrl, nrh, ncl, nch, m, tmp);
			
			
			sprintf(filename,"SD+morpho/hall%06d.pgm", step);
			SavePGM_ui8matrix(m, nrl, nrh, ncl, nch, filename);
		}
	}
}

void test_morpho_FD_THREAD(uint8 **m, long nrl, long nrh, long ncl, long nch)
{
	char *filename = malloc( 100 * sizeof(char));
	uint8 **tmp;
	
	tmp = ui8matrix(nrl - 2, nrh + 2, ncl - 2, nch + 2);
	for(int step = 0; step < NBFRAME; step++)
	{
		if(step)
		{
			sprintf(filename,"FD/hall%06d.pgm", step);
			MLoadPGM_ui8matrix(filename, nrl, nrh, ncl, nch, m);

			Fermeture3_parallel(nrl, nrh, ncl, nch, m, tmp);
			Ouverture3_parallel(nrl, nrh, ncl, nch, m, tmp);
			Ouverture5_parallel(nrl, nrh, ncl, nch, m, tmp);
			Fermeture5_parallel(nrl, nrh, ncl, nch, m, tmp);
		
			
			sprintf(filename,"FD+morpho/hall%06d.pgm", step);
			SavePGM_ui8matrix(m, nrl, nrh, ncl, nch, filename);
		}
	}
}

void temps_fichier(uint8 **m, long nrl, long nrh, long ncl, long nch)
{
	char *filename = malloc( 100 * sizeof(char));
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
}

void test_code_SEQ(uint8 **m, long nrl, long nrh, long ncl, long nch)
{
	struct timeval tv1, tv2, tv3, tv4, tv5, tvfile;
	double temps_SD, temps_FD, temps_morpho_SD, temps_morpho_FD;
	int cycle_SD, cycle_FD, cycle_morpho_SD, cycle_morpho_FD;
	clock_t t1, t2, t3, t4, t5, tfile;

	t1 = clock();gettimeofday(&tv1, NULL);
	test_FD_SEQ(m, nrl, nrh, ncl, nch);
	t2 = clock();gettimeofday(&tv2, NULL);
	test_SD_SEQ(m, nrl, nrh, ncl, nch);
	t3 = clock();gettimeofday(&tv3, NULL);
	test_morpho_SD_SEQ(m, nrl, nrh, ncl, nch);
	t4 = clock();gettimeofday(&tv4, NULL);
	test_morpho_FD_SEQ(m, nrl, nrh, ncl, nch);
	t5 = clock();gettimeofday(&tv5, NULL);

	tfile = clock();
	temps_fichier(m, nrl, nrh, ncl, nch);
	tfile = clock() - tfile;gettimeofday(&tvfile, NULL);
	
	//Affichage des temps de calculs
	//Temps
	temps_FD = (double) (tv2.tv_usec - tv1.tv_usec - (tvfile.tv_usec - tv5.tv_usec))/ 1000000;
	temps_SD = (double) (tv3.tv_usec - tv2.tv_usec - (tvfile.tv_usec - tv5.tv_usec))/ 1000000;
	temps_morpho_SD = (double) (tv4.tv_usec - tv3.tv_usec - (tvfile.tv_usec - tv5.tv_usec))/ 1000000;
	temps_morpho_FD = (double) (tv5.tv_usec - tv4.tv_usec - (tvfile.tv_usec - tv5.tv_usec))/ 1000000;
	//Cycles
	cycle_FD = (t2 - t1 - tfile);
	cycle_SD = (t3 - t2 - tfile);
	cycle_morpho_SD = (t4 - t3 - tfile);
	cycle_morpho_FD = (t5 - t4 - tfile);
	//Affichage
	printf("|-------------------------------|\n");
	printf("|  Temps de calculs sequentiels |\n");
	printf("|_______________________________|\n");
	printf("|Pour FD:\t\t\t|\n|t = %.3f | cycle = %d\t|\n",temps_FD, cycle_FD);
	printf("|Pour SD:\t\t\t|\n|t = %.3f | cycle = %d\t|\n",temps_SD, cycle_SD);
	printf("|Pour les morphos sur SD:\t|\n|t = %.3f | cycle = %d\t|\n",temps_morpho_SD, cycle_morpho_SD);
	printf("|Pour les morphos sur FD:\t|\n|t = %.3f | cycle = %d\t|\n",temps_morpho_FD, cycle_morpho_FD);
	printf("|_______________________________|\n\n\n");
}

void test_code_OPTI(uint8 **m, long nrl, long nrh, long ncl, long nch)
{
	struct timeval tv1, tv2, tv3, tv4, tv5, tvfile;
	double temps_SD, temps_FD, temps_morpho_SD, temps_morpho_FD;
	int cycle_SD, cycle_FD, cycle_morpho_SD, cycle_morpho_FD;
	clock_t t1, t2, t3, t4, t5, tfile;

	t1 = clock();gettimeofday(&tv1, NULL);
	test_FD_SSE(m, nrl, nrh, ncl, nch);
	t2 = clock();gettimeofday(&tv2, NULL);
	test_SD_SSE(m, nrl, nrh, ncl, nch);
	t3 = clock();gettimeofday(&tv3, NULL);
	test_morpho_SD_THREAD(m, nrl, nrh, ncl, nch);
	t4 = clock();gettimeofday(&tv4, NULL);
	test_morpho_FD_THREAD(m, nrl, nrh, ncl, nch);
	t5 = clock();gettimeofday(&tv5, NULL);

	tfile = clock();
	temps_fichier(m, nrl, nrh, ncl, nch);
	tfile = clock() - tfile;gettimeofday(&tvfile, NULL);
	
	//Affichage des temps de calculs
	//Temps
	temps_FD = (double) (tv2.tv_usec - tv1.tv_usec - (tvfile.tv_usec - tv5.tv_usec) )/ 1000000;
	temps_SD = (double) (tv3.tv_usec - tv2.tv_usec - (tvfile.tv_usec - tv5.tv_usec) )/ 1000000;
	temps_morpho_SD = (double) (tv4.tv_usec - tv3.tv_usec - (tvfile.tv_usec - tv5.tv_usec) )/ 1000000;
	temps_morpho_FD = (double) (tv5.tv_usec - tv4.tv_usec - (tvfile.tv_usec - tv5.tv_usec) )/ 1000000;
	//Cycles
	cycle_FD = (t2 - t1 - tfile);
	cycle_SD = (t3 - t2 - tfile);
	cycle_morpho_SD = (t4 - t3 - tfile);
	cycle_morpho_FD = (t5 - t4 - tfile);
	//Affichage
	printf("|-------------------------------|\n");
	printf("|   Temps de calculs optimise   |\n");
	printf("|_______________________________|\n");
	printf("|Pour FD:\t\t\t|\n|t = %.3f | cycle = %d\t|\n",temps_FD, cycle_FD);
	printf("|Pour SD:\t\t\t|\n|t = %.3f | cycle = %d\t|\n",temps_SD, cycle_SD);
	printf("|Pour les morphos sur SD:\t|\n|t = %.3f | cycle = %d\t|\n",temps_morpho_SD, cycle_morpho_SD);
	printf("|Pour les morphos sur FD:\t|\n|t = %.3f | cycle = %d\t|\n",temps_morpho_FD, cycle_morpho_FD);
	printf("|_______________________________|\n\n\n");
}

void test_ROC(uint8 **m, long nrl, long nrh, long ncl, long nch, int ROC[], char* AlgoName)
{
	char *filename = malloc( 100 * sizeof(char));
	for(int step = 10; step < NBFRAME; step+=10)
	{
		sprintf(filename,"%s/hall%06d.pgm", AlgoName, step);
		MLoadPGM_ui8matrix(filename, nrl, nrh, ncl, nch, m);
		Generate_ROC(m, nrl, nrh, ncl, nch, ROC, step);
	}
}



/**************************************************/
/*             FIN DES TESTS UNITAIRES            */
/**************************************************/


int main(int argc, char **argv)
{
	if (argc == 1)
	{ // EXECUTION NORMALE DU PROGRAMME
		int ROC_FD[4] = {0}; //VP, FN, FP, VN
		int ROC_SD[4] = {0}; //VP, FN, FP, VN
		int ROC_FD_morpho[4] = {0}; //VP, FN, FP, VN
		int ROC_SD_morpho[4] = {0}; //VP, FN, FP, VN
		/*  GENERATION DE LA MATRICE M  */
		uint8 **m;
		long nrl; 
		long nrh;
		long ncl;
		long nch;
		long height, width;
		char *buffer;
		buffer = (char*) calloc(80, sizeof(char));
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
		
		/* ########################## */

		test_code_SEQ(m, nrl, nrh, ncl, nch);
		test_code_OPTI(m, nrl, nrh, ncl, nch);

		test_ROC(m, nrl, nrh, ncl, nch, ROC_FD, "FD");
		test_ROC(m, nrl, nrh, ncl, nch, ROC_SD, "SD");
		test_ROC(m, nrl, nrh, ncl, nch, ROC_FD_morpho, "FD+morpho");
		test_ROC(m, nrl, nrh, ncl, nch, ROC_SD_morpho, "SD+morpho");
		

		//Affichage de la matrice ROC
		printf("Pour SD :\nVP = %d, FN = %d, FP = %d, VN = %d\n",ROC_SD[0]/30,ROC_SD[1]/30,ROC_SD[2]/30,ROC_SD[3]/30);
		float p = (float)(ROC_SD[0] + ROC_SD[3]) / (float)(ROC_SD[0] + ROC_SD[1] + ROC_SD[2] + ROC_SD[3]) * 100.0;
		printf("%.2f poucent de pixels vrais\n", p);
		p = (float)(ROC_SD[0]) / (float)(ROC_SD[0] + ROC_SD[2]) * 100.0;
		printf("%.2f poucent de pixels en mouvement vrais\n\n", p);

		printf("Pour FD :\nVP = %d, FN = %d, FP = %d, VN = %d\n",ROC_FD[0]/30,ROC_FD[1]/30,ROC_FD[2]/30,ROC_FD[3]/30);
		p = (float)(ROC_FD[0] + ROC_FD[3]) / (float)(ROC_FD[0] + ROC_FD[1] + ROC_FD[2] + ROC_FD[3]) * 100.0;
		printf("%.2f poucent de pixels vrais\n", p);
		p = (float)(ROC_FD[0]) / (float)(ROC_FD[0] + ROC_FD[2]) * 100.0;
		printf("%.2f poucent de pixels en mouvement vrais\n\n", p);

		printf("Pour SD + morpho :\nVP = %d, FN = %d, FP = %d, VN = %d\n",ROC_SD_morpho[0]/30,ROC_SD_morpho[1]/30,ROC_SD_morpho[2]/30,ROC_SD_morpho[3]/30);
		p = (float)(ROC_SD_morpho[0] + ROC_SD_morpho[3]) / (float)(ROC_SD_morpho[0] + ROC_SD_morpho[1] + ROC_SD_morpho[2] + ROC_SD_morpho[3]) * 100.0;
		printf("%.2f poucent de pixels vrais\n", p);
		p = (float)(ROC_SD_morpho[0]) / (float)(ROC_SD_morpho[0] + ROC_SD_morpho[2]) * 100.0;
		printf("%.2f poucent de pixels en mouvement vrais\n\n", p);;

		printf("Pour FD + morpho:\nVP = %d, FN = %d, FP = %d, VN = %d\n",ROC_FD_morpho[0]/30,ROC_FD_morpho[1]/30,ROC_FD_morpho[2]/30,ROC_FD_morpho[3]/30);
		p = (float)(ROC_FD_morpho[0] + ROC_FD_morpho[3]) / (float)(ROC_FD_morpho[0] + ROC_FD_morpho[1] + ROC_FD_morpho[2] + ROC_FD_morpho[3]) * 100.0;
		printf("%.2f poucent de pixels vrais\n", p);
		p = (float)(ROC_FD_morpho[0]) / (float)(ROC_FD_morpho[0] + ROC_FD_morpho[2]) * 100.0;
		printf("%.2f poucent de pixels en mouvement vrais\n", p);

	}
	else
	{ // argc > 1, alors MODE TEST
		test_visage();
		printf("end\n");
	}
	

	return 0;
}
