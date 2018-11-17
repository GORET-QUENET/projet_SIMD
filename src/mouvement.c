#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <ctype.h> 
#include <string.h>
#include <math.h>

#include "nrdef.h"
#include "mouvement.h"



/*-----------------------------------------------------------------------------------*/
void SEQ_FD(long nrl, long nrh, long ncl, long nch, uint8 **ma, uint8 **m, uint8 **mFD)
/*-----------------------------------------------------------------------------------*/
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

/*------------------------------------------------------------------------------------------------------------------------------------------*/
void SEQ_SD(long nrl, long nrh, long ncl, long nch, uint8 **m, uint8 **mSD, uint8 **MSD, uint8 **MSDa, uint8 **OSD, uint8 **VSD, uint8 **VSDa)
/*------------------------------------------------------------------------------------------------------------------------------------------*/
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

