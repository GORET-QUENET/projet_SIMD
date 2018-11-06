
#include "nrdef.h"
#include "morpho.h"

/*--------------------------------------------------------------------------------*/
void CopyMatrice(long nrl, long nrh, long ncl, long nch, uint8 **dest, uint8 **src)
/*--------------------------------------------------------------------------------*/
{
	for(int i = nrl; i < nrh; i++)
	{
		for(int j = ncl; j < nch; j++)
		{
			dest[i][j] = src[i][j];
		}
	}
}

/*--------------------------------------------------------------------------*/
void CopyValue(long nrl, long nrh, long ncl, long nch, uint8 **dest, int val)
/*--------------------------------------------------------------------------*/
{
	for(int i = nrl; i < nrh; i++)
	{
		for(int j = ncl; j < nch; j++)
		{
			dest[i][j] = val;
		}
	}
}


/*--------------------------------------------------------------------------*/
void Erosion3(long nrl, long nrh, long ncl, long nch, uint8 **m, uint8 **tmp)
/*--------------------------------------------------------------------------*/
{
	for(int i = nrl + 1; i < nrh - 1; i++)
	{
		for(int j = ncl + 1; j < nch - 1; j++)
		{
			for(int k = -1; k <= 1; k++)
			{
				for(int w = -1; w <= 1; w++)
				{
					tmp[i+k][j+w] = m[i+k][j+w] & m[i][j];
				}
			}
		}
	}
	CopyMatrice(nrl, nrh, ncl, nch, m, tmp);
}

/*--------------------------------------------------------------------------*/
void Erosion5(long nrl, long nrh, long ncl, long nch, uint8 **m, uint8 **tmp)
/*--------------------------------------------------------------------------*/
{
	for(int i = nrl + 2; i < nrh - 2; i++)
	{
		for(int j = ncl + 2; j < nch - 2; j++)
		{
			for(int k = -2; k <= 2; k++)
			{
				for(int w = -2; w <= 2; w++)
				{
					tmp[i+k][j+w] = m[i+k][j+w] & m[i][j];
				}
			}
		}
	}
	CopyMatrice(nrl, nrh, ncl, nch, m, tmp);
}

/*-----------------------------------------------------------------------------*/
void Dilatation3(long nrl, long nrh, long ncl, long nch, uint8 **m, uint8 **tmp)
/*-----------------------------------------------------------------------------*/
{
	for(int i = nrl + 1; i < nrh - 1; i++)
	{
		for(int j = ncl + 1; j < nch - 1; j++)
		{
			for(int k = -1; k <= 1; k++)
			{
				for(int w = -1; w <= 1; w++)
				{
					tmp[i+k][j+w] = m[i+k][j+w] | m[i][j];
				}
			}
		}
	}
	CopyMatrice(nrl, nrh, ncl, nch, m, tmp);
}

/*-----------------------------------------------------------------------------*/
void Dilatation5(long nrl, long nrh, long ncl, long nch, uint8 **m, uint8 **tmp)
/*-----------------------------------------------------------------------------*/
{
	for(int i = nrl + 2; i < nrh - 2; i++)
	{
		for(int j = ncl + 2; j < nch - 2; j++)
		{
			for(int k = -2; k <= 2; k++)
			{
				for(int w = -2; w <= 2; w++)
				{
					tmp[i+k][j+w] = m[i+k][j+w] | m[i][j];
				}
			}
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
