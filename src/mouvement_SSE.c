#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <ctype.h> 
#include <string.h>

#include "nrdef.h"
#include "vnrdef.h"
#include "mouvement_SSE.h"



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
void FD_SSE(long nrl, long nrh, long ncl, long nch, uint8 **ma, uint8 **m, uint8 **mFD)
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


/*---------------------------------------------------------------------------------------------------------------------------------------*/
void SD_SSE(long nrl, long nrh, long ncl, long nch, uint8 **m, uint8 **mSD, uint8 **MSD, uint8 **MSDa, uint8 **OSD, uint8 **VSD, uint8 **VSDa)
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


