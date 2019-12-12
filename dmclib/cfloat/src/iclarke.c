/*=====================================================================================
 File name:        CLARKE.C

 Originator:	Digital Control Systems Group
			Texas Instruments

 Description:  The Clarke Transformation

=====================================================================================
 History:
-------------------------------------------------------------------------------------
 04-15-2005	Version 3.20
-------------------------------------------------------------------------------------*/

#include "dmctype.h"
#include "iclarke.h"

void iclarke_calc(ICLARKE *v)
{
   v->As = v->Alpha;

   v->Bs = -0.5*v->Alpha + 0.866*v->Beta;

   v->Cs = -0.5*v->Alpha - 0.866*v->Beta;
}

#pragma CODE_SECTION(iclarke_calc,"ramfuncs");
