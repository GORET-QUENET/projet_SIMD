#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <ctype.h> 
#include <string.h>
#include <time.h>

#include "nrdef.h"
#include "vnrdef.h"
#include "nrutil.h"
#include "test_mouvement.h"
#include "test_mouvement_SSE.h"
#include "test_morpho.h"
#include "test_morpho_THREAD.h"
#include "test_morpho_SSE.h"

//Commande SSE : https://software.intel.com/sites/landingpage/IntrinsicsGuide/

#define NBFRAME 300

/**************************************************/
/*     		  TESTS UNITAIRES		  */
/**************************************************/

void test_visage()
{
	struct timespec tv1, tv2;
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
			clock_gettime(CLOCK_MONOTONIC, &tv1);
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
			clock_gettime(CLOCK_MONOTONIC, &tv2);
			double temps = (double) (tv2.tv_sec - tv1.tv_sec);
			temps += (double) (tv2.tv_nsec - tv1.tv_nsec)/ 1000000000.0;
			printf ("Total sequential time = %f seconds\n", temps);
                        break;
		case 8:
			clock_gettime(CLOCK_MONOTONIC, &tv1);
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
			clock_gettime(CLOCK_MONOTONIC, &tv2);
			temps = (double) (tv2.tv_sec - tv1.tv_sec);
			temps += (double) (tv2.tv_nsec - tv1.tv_nsec)/ 1000000000.0;
			printf ("Total parallel time = %f seconds\n", temps);
                        break;

		default:
			return;
		}	
		//afficher_matrice(m, nrl, nrh, ncl, nch);
        	inverser_matrice(m, nrl, nrh, ncl, nch);	
		SavePGM_ui8matrix(m, nrl, nrh, ncl, nch, filename);
	}
}


void Fill_ROC(uint8 **m, long nrl, long nrh, long ncl, long nch, int ROC[], int step)
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
	double temps_SD, temps_FD, temps_morpho_SD, temps_morpho_FD;
	struct timespec t1, t2, t3, t4, t5, tfile;

	clock_gettime(CLOCK_MONOTONIC, &t1);
	test_FD_SEQ(m, nrl, nrh, ncl, nch);
	clock_gettime(CLOCK_MONOTONIC, &t2);
	test_SD_SEQ(m, nrl, nrh, ncl, nch);
	clock_gettime(CLOCK_MONOTONIC, &t3);
	test_morpho_SD_SEQ(m, nrl, nrh, ncl, nch);
	clock_gettime(CLOCK_MONOTONIC, &t4);
	test_morpho_FD_SEQ(m, nrl, nrh, ncl, nch);
	clock_gettime(CLOCK_MONOTONIC, &t5);

	temps_fichier(m, nrl, nrh, ncl, nch);
	clock_gettime(CLOCK_MONOTONIC, &tfile);
	
	//Affichage des temps de calculs
	//Temps
	temps_FD = (double) (t2.tv_sec - t1.tv_sec - (tfile.tv_sec - t5.tv_sec));
	temps_FD += (double) (t2.tv_nsec - t1.tv_nsec - (tfile.tv_nsec - t5.tv_nsec))/ 1000000000.0;

	temps_SD = (double) (t3.tv_sec - t2.tv_sec - (tfile.tv_sec - t5.tv_sec));
	temps_SD += (double) (t3.tv_nsec - t2.tv_nsec - (tfile.tv_nsec - t5.tv_nsec))/ 1000000000.0;

	temps_morpho_SD = (double) (t4.tv_sec - t3.tv_sec - (tfile.tv_sec - t5.tv_sec));
	temps_morpho_SD += (double) (t4.tv_nsec - t3.tv_nsec - (tfile.tv_nsec - t5.tv_nsec))/ 1000000000.0;

	temps_morpho_FD = (double) (t5.tv_sec - t4.tv_sec - (tfile.tv_sec - t5.tv_sec));
	temps_morpho_FD += (double) (t5.tv_nsec - t4.tv_nsec - (tfile.tv_nsec - t5.tv_nsec))/ 1000000000.0;
	//Affichage
	printf("|-------------------------------|\n");
	printf("|  Temps de calculs sequentiels |\n");
	printf("|_______________________________|\n");
	printf("|Pour FD:\t\t\t|\n|t = %.3f \t\t\t|\n",temps_FD);
	printf("|Pour SD:\t\t\t|\n|t = %.3f \t\t\t|\n",temps_SD);
	printf("|Pour les morphos sur SD:\t|\n|t = %.3f \t\t\t|\n",temps_morpho_SD);
	printf("|Pour les morphos sur FD:\t|\n|t = %.3f \t\t\t|\n",temps_morpho_FD);
	printf("|_______________________________|\n\n\n");
}

void test_code_OPTI(uint8 **m, long nrl, long nrh, long ncl, long nch)
{
	double temps_SD, temps_FD, temps_morpho_SD, temps_morpho_FD;
	struct timespec t1, t2, t3, t4, t5, tfile;

	clock_gettime(CLOCK_MONOTONIC, &t1);
	test_FD_SSE(m, nrl, nrh, ncl, nch);
	clock_gettime(CLOCK_MONOTONIC, &t2);
	test_SD_SSE(m, nrl, nrh, ncl, nch);
	clock_gettime(CLOCK_MONOTONIC, &t3);
	test_morpho_SD_SSE(m, nrl, nrh, ncl, nch);
	clock_gettime(CLOCK_MONOTONIC, &t4);
	test_morpho_FD_SSE(m, nrl, nrh, ncl, nch);
	clock_gettime(CLOCK_MONOTONIC, &t5);

	temps_fichier(m, nrl, nrh, ncl, nch);
	clock_gettime(CLOCK_MONOTONIC, &tfile);
	
	//Affichage des temps de calculs
	//Temps
	temps_FD = (double) (t2.tv_sec - t1.tv_sec - (tfile.tv_sec - t5.tv_sec));
	temps_FD += (double) (t2.tv_nsec - t1.tv_nsec - (tfile.tv_nsec - t5.tv_nsec))/ 1000000000.0;

	temps_SD = (double) (t3.tv_sec - t2.tv_sec - (tfile.tv_sec - t5.tv_sec));
	temps_SD += (double) (t3.tv_nsec - t2.tv_nsec - (tfile.tv_nsec - t5.tv_nsec))/ 1000000000.0;

	temps_morpho_SD = (double) (t4.tv_sec - t3.tv_sec - (tfile.tv_sec - t5.tv_sec));
	temps_morpho_SD += (double) (t4.tv_nsec - t3.tv_nsec - (tfile.tv_nsec - t5.tv_nsec))/ 1000000000.0;

	temps_morpho_FD = (double) (t5.tv_sec - t4.tv_sec - (tfile.tv_sec - t5.tv_sec));
	temps_morpho_FD += (double) (t5.tv_nsec - t4.tv_nsec - (tfile.tv_nsec - t5.tv_nsec))/ 1000000000.0;
	//Affichage
	printf("|-------------------------------|\n");
	printf("|   Temps de calculs optimise   |\n");
	printf("|_______________________________|\n");
	printf("|Pour FD:\t\t\t|\n|t = %.3f \t\t\t|\n",temps_FD);
	printf("|Pour SD:\t\t\t|\n|t = %.3f \t\t\t|\n",temps_SD);
	printf("|Pour les morphos sur SD:\t|\n|t = %.3f \t\t\t|\n",temps_morpho_SD);
	printf("|Pour les morphos sur FD:\t|\n|t = %.3f \t\t\t|\n",temps_morpho_FD);
	printf("|_______________________________|\n\n\n");
}

void test_ROC(uint8 **m, long nrl, long nrh, long ncl, long nch, int ROC[], char* AlgoName)
{
	char *filename = malloc( 100 * sizeof(char));
	for(int step = 10; step < NBFRAME; step+=10)
	{
		sprintf(filename,"%s/hall%06d.pgm", AlgoName, step);
		MLoadPGM_ui8matrix(filename, nrl, nrh, ncl, nch, m);
		Fill_ROC(m, nrl, nrh, ncl, nch, ROC, step);
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

		//test_code_SEQ(m, nrl, nrh, ncl, nch);
		//test_code_OPTI(m, nrl, nrh, ncl, nch);

		test_FD_SEQ(m, nrl, nrh, ncl, nch);
		test_SD_SEQ(m, nrl, nrh, ncl, nch);
		test_morpho_SD_SEQ(m, nrl, nrh, ncl, nch);
		test_morpho_FD_SEQ(m, nrl, nrh, ncl, nch);
		test_FD_SSE(m, nrl, nrh, ncl, nch);
		test_SD_SSE(m, nrl, nrh, ncl, nch);
		test_morpho_SD_SSE(m, nrl, nrh, ncl, nch);
		test_morpho_FD_SSE(m, nrl, nrh, ncl, nch);
		test_morpho_SD_THREAD(m, nrl, nrh, ncl, nch);
		test_morpho_FD_THREAD(m, nrl, nrh, ncl, nch);
		
		test_ROC(m, nrl, nrh, ncl, nch, ROC_FD, "FD");
		test_ROC(m, nrl, nrh, ncl, nch, ROC_SD, "SD");
		test_ROC(m, nrl, nrh, ncl, nch, ROC_FD_morpho, "FD+morpho");
		test_ROC(m, nrl, nrh, ncl, nch, ROC_SD_morpho, "SD+morpho");
		

		//Affichage des matrices ROC
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
