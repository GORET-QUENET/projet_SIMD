#include <stdio.h>

#include "nrdef.h"
#include "vnrdef.h"
#include "morpho.h"


/*--------------------------------------------------------------------------------*/
void CopyMatrice(long nrl, long nrh, long ncl, long nch, uint8 **dest, uint8 **src)
/*--------------------------------------------------------------------------------*/
{
	for(int i = nrl; i <= nrh; i++)
	{
		for(int j = ncl; j <= nch; j++)
		{
			dest[i][j] = src[i][j];
		}
	}
}

/*--------------------------------------------------------------------------*/
void CopyValue(long nrl, long nrh, long ncl, long nch, uint8 **dest, int val)
/*--------------------------------------------------------------------------*/
{
	for(int i = nrl; i <= nrh; i++)
	{
		for(int j = ncl; j <= nch; j++)
		{
			dest[i][j] = val;
		}
	}
}

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
				printf("Error : grey value detected during invert\n");
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
	printf("\n\n\n");
}


/*--------------------------------------------------------------------------*/
void Erosion3(long nrl, long nrh, long ncl, long nch, uint8 **m, uint8 **tmp)
/*--------------------------------------------------------------------------*/
{
	for(int i = nrl; i <= nrh; i++)
	{
		for(int j = ncl; j <= nch; j++)
		{
			tmp[i][j] = m[i-1][j-1] & m[i-1][j+0] & m[i-1][j+1];
			tmp[i][j]&= m[i+0][j-1] & m[i+0][j+0] & m[i+0][j+1];
			tmp[i][j]&= m[i+1][j-1] & m[i+1][j+0] & m[i+1][j+1];
		}
	}
	CopyMatrice(nrl, nrh, ncl, nch, m, tmp);
}

/*--------------------------------------------------------------------------*/
void Erosion5(long nrl, long nrh, long ncl, long nch, uint8 **m, uint8 **tmp)
/*--------------------------------------------------------------------------*/
{
	for(int i = nrl; i <= nrh; i++)
	{
		for(int j = ncl; j <= nch; j++)
		{
			tmp[i][j] = m[i-2][j-2] & m[i-2][j-1] & m[i-2][j+0] & m[i-2][j+1] & m[i-2][j+2];
			tmp[i][j]&= m[i-1][j-2] & m[i-1][j-1] & m[i-1][j+0] & m[i-1][j+1] & m[i-1][j+2];
			tmp[i][j]&= m[i+0][j-2] & m[i+0][j-1] & m[i+0][j+0] & m[i+0][j+1] & m[i+0][j+2];
			tmp[i][j]&= m[i+1][j-2] & m[i+1][j-1] & m[i+1][j+0] & m[i+1][j+1] & m[i+1][j+2];
			tmp[i][j]&= m[i+2][j-2] & m[i+2][j-1] & m[i+2][j+0] & m[i+2][j+1] & m[i+2][j+2];
		}
	}
	CopyMatrice(nrl, nrh, ncl, nch, m, tmp);
}

/*-----------------------------------------------------------------------------*/
void Dilatation3(long nrl, long nrh, long ncl, long nch, uint8 **m, uint8 **tmp)
/*-----------------------------------------------------------------------------*/
{
	for(int i = nrl; i <= nrh; i++)
	{
		for(int j = ncl; j <= nch; j++)
		{
			tmp[i][j] = m[i-1][j-1] | m[i-1][j+0] | m[i-1][j+1];
			tmp[i][j]|= m[i+0][j-1] | m[i+0][j+0] | m[i+0][j+1];
			tmp[i][j]|= m[i+1][j-1] | m[i+1][j+0] | m[i+1][j+1];
		}
	}
	CopyMatrice(nrl, nrh, ncl, nch, m, tmp);
}

/*-----------------------------------------------------------------------------*/
void Dilatation5(long nrl, long nrh, long ncl, long nch, uint8 **m, uint8 **tmp)
/*-----------------------------------------------------------------------------*/
{
	for(int i = nrl; i <= nrh; i++)
	{
		for(int j = ncl; j <= nch; j++)
		{
			tmp[i][j] = m[i-2][j-2] | m[i-2][j-1] | m[i-2][j+0] | m[i-2][j+1] | m[i-2][j+2];
			tmp[i][j]|= m[i-1][j-2] | m[i-1][j-1] | m[i-1][j+0] | m[i-1][j+1] | m[i-1][j+2];
			tmp[i][j]|= m[i+0][j-2] | m[i+0][j-1] | m[i+0][j+0] | m[i+0][j+1] | m[i+0][j+2];
			tmp[i][j]|= m[i+1][j-2] | m[i+1][j-1] | m[i+1][j+0] | m[i+1][j+1] | m[i+1][j+2];
			tmp[i][j]|= m[i+2][j-2] | m[i+2][j-1] | m[i+2][j+0] | m[i+2][j+1] | m[i+2][j+2];
		}
	}
	CopyMatrice(nrl, nrh, ncl, nch, m, tmp);
}

/*-------------------------------------------------------------------------------*/
void Fermeture3(long nrl, long nrh, long ncl, long nch, uint8 **m, uint8 **tmp)
/*-------------------------------------------------------------------------------*/
{
	Dilatation3(nrl, nrh, ncl, nch, m, tmp);
	Erosion3(nrl, nrh, ncl, nch, m, tmp);
}

/*-------------------------------------------------------------------------------*/
void Fermeture5(long nrl, long nrh, long ncl, long nch, uint8 **m, uint8 **tmp)
/*-------------------------------------------------------------------------------*/
{
	Dilatation5(nrl, nrh, ncl, nch, m, tmp);
	Erosion5(nrl, nrh, ncl, nch, m, tmp);
}

/*-------------------------------------------------------------------------------*/
void Ouverture3(long nrl, long nrh, long ncl, long nch, uint8 **m, uint8 **tmp)
/*-------------------------------------------------------------------------------*/
{
	Erosion3(nrl, nrh, ncl, nch, m, tmp);
	Dilatation3(nrl, nrh, ncl, nch, m, tmp);
}

/*-------------------------------------------------------------------------------*/
void Ouverture5(long nrl, long nrh, long ncl, long nch, uint8 **m, uint8 **tmp)
/*-------------------------------------------------------------------------------*/
{
	Erosion5(nrl, nrh, ncl, nch, m, tmp);
	Dilatation5(nrl, nrh, ncl, nch, m, tmp);
}



