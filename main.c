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
#include "ROC.h"

#define NBFRAME 300

//Commandes SSE : https://software.intel.com/sites/landingpage/IntrinsicsGuide/

/**************************************************/
/*     	     TESTS UNITAIRES DES MORPHOS	  */
/**************************************************/

void test_visage()
{
	//struct timespec tv1, tv2;
	long nrl; 
	long nrh;
	long ncl;
	long nch;
	uint8 **m; // image courante 
	uint8 **tmp;
	uint8 i;
	char *filename = malloc( 100 * sizeof(char));
	sprintf(filename,"visage/test.pgm");
	m = LoadPGM_ui8matrix(filename, &nrl, &nrh, &ncl, &nch);
	tmp = ui8matrix(nrl - 2, nrh + 2, ncl - 2, nch + 2);
	for (i = 0; i < 24; i++){
		sprintf(filename,"visage/test.pgm");
		m = LoadPGM_ui8matrix(filename, &nrl, &nrh, &ncl, &nch);
		
		inverser_matrice(m, nrl, nrh, ncl, nch);
		//afficher_matrice(m, nrl, nrh, ncl, nch);
		switch(i){
		/********************************/
		/* Test des morphos séquentiels */
		/********************************/
		case 0: 
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
                        break;
		/********************************/
		/* Test des morphos avec Thread */
		/********************************/
		case 8:
                        Erosion3_parallel(nrl, nrh, ncl, nch, m, tmp);
                        sprintf(filename,"visage/test_erosion3_THREAD.pgm");
                        break;
		case 9:
			Erosion5_parallel(nrl, nrh, ncl, nch, m, tmp);
                        sprintf(filename,"visage/test_erosion5_THREAD.pgm");
                        break;
		case 10:
                        Dilatation3_parallel(nrl, nrh, ncl, nch, m, tmp);
                        sprintf(filename,"visage/test_dilatation3_THREAD.pgm");
                        break;
                case 11:
                        Dilatation5_parallel(nrl, nrh, ncl, nch, m, tmp);
                        sprintf(filename,"visage/test_dilatation5_THREAD.pgm");
                        break;
		case 12:
                        Fermeture3_parallel(nrl, nrh, ncl, nch, m, tmp);
                        sprintf(filename,"visage/test_fermeture3_THREAD.pgm");
                        break;
                case 13:
                        Fermeture5_parallel(nrl, nrh, ncl, nch, m, tmp);
                        sprintf(filename,"visage/test_fermeture5_THREAD.pgm");
                        break;
		case 14:
                        Ouverture3_parallel(nrl, nrh, ncl, nch, m, tmp);
                        sprintf(filename,"visage/test_ouverture3_THREAD.pgm");
                        break;
                case 15:
                        Ouverture5_parallel(nrl, nrh, ncl, nch, m, tmp);
                        sprintf(filename,"visage/test_ouverture5_THREAD.pgm");
                        break;
		/********************************/
		/* Test des morphos avec Thread */
		/********************************/
		case 16:
                        Erosion3_SSE(nrl, nrh, ncl, nch, m, tmp);
                        sprintf(filename,"visage/test_erosion3_SSE2.pgm");
                        break;
		case 17:
			Erosion5_SSE(nrl, nrh, ncl, nch, m, tmp);
                        sprintf(filename,"visage/test_erosion5_SSE2.pgm");
                        break;
		case 18:
                        Dilatation3_SSE(nrl, nrh, ncl, nch, m, tmp);
                        sprintf(filename,"visage/test_dilatation3_SSE2.pgm");
                        break;
                case 19:
                        Dilatation5_SSE(nrl, nrh, ncl, nch, m, tmp);
                        sprintf(filename,"visage/test_dilatation5_SSE2.pgm");
                        break;
		case 20:
                        Fermeture3_SSE(nrl, nrh, ncl, nch, m, tmp);
                        sprintf(filename,"visage/test_fermeture3_SSE2.pgm");
                        break;
                case 21:
                        Fermeture5_SSE(nrl, nrh, ncl, nch, m, tmp);
                        sprintf(filename,"visage/test_fermeture5_SSE2.pgm");
                        break;
		case 22:
                        Ouverture3_SSE(nrl, nrh, ncl, nch, m, tmp);
                        sprintf(filename,"visage/test_ouverture3_SSE2.pgm");
                        break;
                case 23:
                        Ouverture5_SSE(nrl, nrh, ncl, nch, m, tmp);
                        sprintf(filename,"visage/test_ouverture5_SSE2.pgm");
                        break;

		default:
			return;
		}	
		afficher_matrice(m, nrl, nrh, ncl, nch);
        	inverser_matrice(m, nrl, nrh, ncl, nch);	
		SavePGM_ui8matrix(m, nrl, nrh, ncl, nch, filename);
	}
}


/**************************************************/
/*             FIN DES TESTS UNITAIRES            */
/**************************************************/


int main(int argc, char **argv)
{
	if (argc == 1)
	{ 
		/**********************************/
		/*   Generation de la matrice m   */
		/**********************************/
		char *filename = malloc( 100 * sizeof(char));
		uint8 ***tm;
		uint8 ***tmSD;
		uint8 ***tmFD;
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

		tm = malloc(NBFRAME * sizeof(uint8**));
		tmSD = malloc(NBFRAME * sizeof(uint8**));
		tmFD = malloc(NBFRAME * sizeof(uint8**));
		for(int i = 0; i < NBFRAME; i++)
		{
			tm[i] = ui8matrix(nrl - 2, nrh + 2, ncl - 2, nch + 2);
			sprintf(filename,"hall/hall%06d.pgm", i);
			MLoadPGM_ui8matrix(filename, nrl, nrh, ncl, nch, tm[i]);
		}
		for(int i = 0; i < NBFRAME; i++)
		{
			tmSD[i] = ui8matrix(nrl - 2, nrh + 2, ncl - 2, nch + 2);
			if(i)
			{
				sprintf(filename,"SD/hall%06d.pgm", i);
				MLoadPGM_ui8matrix(filename, nrl, nrh, ncl, nch, tmSD[i]);
			}
		}
		for(int i = 0; i < NBFRAME; i++)
		{
			tmFD[i] = ui8matrix(nrl - 2, nrh + 2, ncl - 2, nch + 2);
			if(i)
			{
				sprintf(filename,"FD/hall%06d.pgm", i);
				MLoadPGM_ui8matrix(filename, nrl, nrh, ncl, nch, tmFD[i]);
			}
		}
		/**********************************/
		/* Partie des tests des fonctions */
		/**********************************/

		test_FD_SEQ(tm, nrl, nrh, ncl, nch);
		test_SD_SEQ(tm, nrl, nrh, ncl, nch);
		test_morpho_FD_SEQ(tmFD, nrl, nrh, ncl, nch);
		test_morpho_SD_SEQ(tmSD, nrl, nrh, ncl, nch);
		test_FD_SSE(tm, nrl, nrh, ncl, nch);
		test_SD_SSE(tm, nrl, nrh, ncl, nch);
		test_morpho_FD_SSE(tmFD, nrl, nrh, ncl, nch);
		test_morpho_SD_SSE(tmSD, nrl, nrh, ncl, nch);
		test_morpho_FD_THREAD(tmFD, nrl, nrh, ncl, nch);
		test_morpho_SD_THREAD(tmSD, nrl, nrh, ncl, nch);

	}
	else
	{ // argc > 1, alors MODE TEST SUR VISAGE
		test_visage();
		printf("end\n");
	}
	

	return 0;
}
