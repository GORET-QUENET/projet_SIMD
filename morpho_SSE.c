
#include <stdio.h>

#include "nrdef.h"
#include "vnrdef.h"
#include "morpho_SSE.h"

/*--------------------------------------------------------------------------------*/
void CopyMatrice_SSE(long nrl, long nrh, long ncl, long nch, uint8 **dest, uint8 **src)
/*--------------------------------------------------------------------------------*/
{
	vuint8 vdest, vsrc;
	for(int i = nrl; i <= nrh; i++)
	{
		for(int j = ncl; j <= nch; j+=16)
		{
			vsrc = _mm_loadu_si128((__m128i*)&src[i][j]);
			vdest = vsrc;
			_mm_storeu_si128((__m128i*)&dest[i][j], vdest);
		}
	}
}

/*--------------------------------------------------------------------------*/
void Erosion3_SSE(long nrl, long nrh, long ncl, long nch, uint8 **m, uint8 **tmp)
/*--------------------------------------------------------------------------*/
{
	vuint8 vm1,vm2,vm3, vtmp;
	
        for(int i = nrl; i <= nrh; i++)
        {
                for(int j = ncl; j <= nch; j+=16)
                {
			vm1 = _mm_loadu_si128((__m128i*)&m[i][j]);
			vm2 = _mm_loadu_si128((__m128i*)&m[i-1][j]);
			vm3 = _mm_loadu_si128((__m128i*)&m[i+1][j]);
                        vtmp = _mm_and_si128(_mm_and_si128(vm1, vm2), vm3);
			_mm_storeu_si128((__m128i*)&tmp[i][j], vtmp);
                }
        }
        CopyMatrice_SSE(nrl, nrh, ncl, nch, m, tmp);
	
        for(int i = nrl; i <= nrh; i++)
        {
                for(int j = ncl; j <= nch; j+=16)
                {
                        vm1 = _mm_loadu_si128((__m128i*)&m[i][j]);
			vm2 = _mm_loadu_si128((__m128i*)&m[i][j-1]);
			vm3 = _mm_loadu_si128((__m128i*)&m[i][j+1]);
                        vtmp = _mm_and_si128(_mm_and_si128(vm1, vm2), vm3);
			_mm_storeu_si128((__m128i*)&tmp[i][j], vtmp);
                }
        }
        CopyMatrice_SSE(nrl, nrh, ncl, nch, m, tmp);
}

/*--------------------------------------------------------------------------*/
void Erosion5_SSE(long nrl, long nrh, long ncl, long nch, uint8 **m, uint8 **tmp)
/*--------------------------------------------------------------------------*/
{
	Erosion3_SSE(nrl, nrh, ncl, nch, m, tmp);
	Erosion3_SSE(nrl, nrh, ncl, nch, m, tmp);
}

/*--------------------------------------------------------------------------*/
void Dilatation3_SSE(long nrl, long nrh, long ncl, long nch, uint8 **m, uint8 **tmp)
/*--------------------------------------------------------------------------*/
{
	vuint8 vm1,vm2,vm3, vtmp;
        for(int i = nrl; i <= nrh; i++)
        {
                for(int j = ncl; j <= nch; j+=16)
                {
			vm1 = _mm_loadu_si128((__m128i*)&m[i][j]);
			vm2 = _mm_loadu_si128((__m128i*)&m[i-1][j]);
			vm3 = _mm_loadu_si128((__m128i*)&m[i+1][j]);
                        vtmp = _mm_or_si128(_mm_or_si128(vm1, vm2), vm3);
			_mm_storeu_si128((__m128i*)&tmp[i][j], vtmp);
                }
        }
        CopyMatrice_SSE(nrl, nrh, ncl, nch, m, tmp);
	
        for(int i = nrl; i <= nrh; i++)
        {
                for(int j = ncl; j <= nch; j+=16)
                {
                        vm1 = _mm_loadu_si128((__m128i*)&m[i][j]);
			vm2 = _mm_loadu_si128((__m128i*)&m[i][j-1]);
			vm3 = _mm_loadu_si128((__m128i*)&m[i][j+1]);
                        vtmp = _mm_or_si128(_mm_or_si128(vm1, vm2), vm3);
			_mm_storeu_si128((__m128i*)&tmp[i][j], vtmp);
                }
        }
        CopyMatrice_SSE(nrl, nrh, ncl, nch, m, tmp);
}

/*-----------------------------------------------------------------------------*/
void Dilatation5_SSE(long nrl, long nrh, long ncl, long nch, uint8 **m, uint8 **tmp)
/*-----------------------------------------------------------------------------*/
{
	Dilatation3_SSE(nrl, nrh, ncl, nch, m, tmp);
	Dilatation3_SSE(nrl, nrh, ncl, nch, m, tmp);
}

/*-------------------------------------------------------------------------------*/
void Fermeture3_SSE(long nrl, long nrh, long ncl, long nch, uint8 **m, uint8 **tmp)
/*-------------------------------------------------------------------------------*/
{
        Dilatation3_SSE(nrl, nrh, ncl, nch, m, tmp);
        Erosion3_SSE(nrl, nrh, ncl, nch, m, tmp);
}

/*-------------------------------------------------------------------------------*/
void Fermeture5_SSE(long nrl, long nrh, long ncl, long nch, uint8 **m, uint8 **tmp)
/*-------------------------------------------------------------------------------*/
{
        Dilatation5_SSE(nrl, nrh, ncl, nch, m, tmp);
        Erosion5_SSE(nrl, nrh, ncl, nch, m, tmp);
}

/*-------------------------------------------------------------------------------*/
void Ouverture3_SSE(long nrl, long nrh, long ncl, long nch, uint8 **m, uint8 **tmp)
/*-------------------------------------------------------------------------------*/
{
        Erosion3_SSE(nrl, nrh, ncl, nch, m, tmp);
        Dilatation3_SSE(nrl, nrh, ncl, nch, m, tmp);
}

/*-------------------------------------------------------------------------------*/
void Ouverture5_SSE(long nrl, long nrh, long ncl, long nch, uint8 **m, uint8 **tmp)
/*-------------------------------------------------------------------------------*/
{
        Erosion5_SSE(nrl, nrh, ncl, nch, m, tmp);
        Dilatation5_SSE(nrl, nrh, ncl, nch, m, tmp);
}

