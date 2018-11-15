#include <omp.h>
#include <stdio.h>

#include "nrdef.h"
#include "vnrdef.h"
#include "morpho_THREAD.h"

#define CHUNK 16

/*----------------------------------------------------------------------------------------*/
void CopyMatrice_parallel(long nrl, long nrh, long ncl, long nch, uint8 **dest, uint8 **src)
/*----------------------------------------------------------------------------------------*/
{
	#pragma omp parallel for schedule(static, CHUNK)
	for(int i = nrl; i <= nrh; i++)
	{
		for(int j = ncl; j <= nch; j++)
		{
			dest[i][j] = src[i][j];
		}
	}
}


/*----------------------------------------------------------------------------------*/
void Erosion3_parallel(long nrl, long nrh, long ncl, long nch, uint8 **m, uint8 **tmp)
/*----------------------------------------------------------------------------------*/
{
	#pragma omp parallel for schedule(static, CHUNK)
        for(int i = nrl; i <= nrh; i++)
        {
                for(int j = ncl; j <= nch; j++)
                {
                        tmp[i][j] = m[i+1][j] & m[i][j] & m[i-1][j];
                }
        }
        CopyMatrice_parallel(nrl, nrh, ncl, nch, m, tmp);
	#pragma omp parallel for schedule(static, CHUNK)
        for(int i = nrl; i <= nrh; i++)
        {
                for(int j = ncl; j <= nch; j++)
                {
                        tmp[i][j] = m[i][j+1] & m[i][j] & m[i][j-1];
                }
        }
        CopyMatrice_parallel(nrl, nrh, ncl, nch, m, tmp);
}

/*----------------------------------------------------------------------------------*/
void Erosion5_parallel(long nrl, long nrh, long ncl, long nch, uint8 **m, uint8 **tmp)
/*----------------------------------------------------------------------------------*/
{
	Erosion3_parallel(nrl, nrh, ncl, nch, m, tmp);
	Erosion3_parallel(nrl, nrh, ncl, nch, m, tmp);
}

/*-------------------------------------------------------------------------------------*/
void Dilatation3_parallel(long nrl, long nrh, long ncl, long nch, uint8 **m, uint8 **tmp)
/*-------------------------------------------------------------------------------------*/
{
	#pragma omp parallel for schedule(static, CHUNK)
        for(int i = nrl; i <= nrh; i++)
        {
                for(int j = ncl; j <= nch; j++)
                {
                        tmp[i][j] = m[i+1][j] | m[i][j] | m[i-1][j];
                }
        }
        CopyMatrice_parallel(nrl, nrh, ncl, nch, m, tmp);
	#pragma omp parallel for schedule(static, CHUNK)
        for(int i = nrl; i <= nrh; i++)
        {
                for(int j = ncl; j <= nch; j++)
                {
                        tmp[i][j] = m[i][j+1] | m[i][j] | m[i][j-1];
                }
        }
        CopyMatrice_parallel(nrl, nrh, ncl, nch, m, tmp);
}

/*-------------------------------------------------------------------------------------*/
void Dilatation5_parallel(long nrl, long nrh, long ncl, long nch, uint8 **m, uint8 **tmp)
/*-------------------------------------------------------------------------------------*/
{
	Dilatation3_parallel(nrl, nrh, ncl, nch, m, tmp);
	Dilatation3_parallel(nrl, nrh, ncl, nch, m, tmp);
}

/*------------------------------------------------------------------------------------*/
void Fermeture3_parallel(long nrl, long nrh, long ncl, long nch, uint8 **m, uint8 **tmp)
/*------------------------------------------------------------------------------------*/
{
        Dilatation3_parallel(nrl, nrh, ncl, nch, m, tmp);
        Erosion3_parallel(nrl, nrh, ncl, nch, m, tmp);
}

/*------------------------------------------------------------------------------------*/
void Fermeture5_parallel(long nrl, long nrh, long ncl, long nch, uint8 **m, uint8 **tmp)
/*------------------------------------------------------------------------------------*/
{
        Dilatation5_parallel(nrl, nrh, ncl, nch, m, tmp);
        Erosion5_parallel(nrl, nrh, ncl, nch, m, tmp);
}

/*------------------------------------------------------------------------------------*/
void Ouverture3_parallel(long nrl, long nrh, long ncl, long nch, uint8 **m, uint8 **tmp)
/*------------------------------------------------------------------------------------*/
{
        Erosion3_parallel(nrl, nrh, ncl, nch, m, tmp);
        Dilatation3_parallel(nrl, nrh, ncl, nch, m, tmp);
}

/*------------------------------------------------------------------------------------*/
void Ouverture5_parallel(long nrl, long nrh, long ncl, long nch, uint8 **m, uint8 **tmp)
/*------------------------------------------------------------------------------------*/
{
        Erosion5_parallel(nrl, nrh, ncl, nch, m, tmp);
        Dilatation5_parallel(nrl, nrh, ncl, nch, m, tmp);
}

