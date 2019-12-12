/*=====================================================================================
 File name:        RAMPGEN.C                 
                    
 Originator:	Digital Control Systems Group
			Texas Instruments

 Description:  The Ramp Generation                   

=====================================================================================
 History:
-------------------------------------------------------------------------------------
 04-15-2005	Version 3.20
 Modified by LiuYongxia ZhejiangUniversity 2018.5.25
-------------------------------------------------------------------------------------*/

#include "dmctype.h"
#include "rampgen.h"

void rampgen_calc(RAMPGEN *v)
{	

        v->Angle += v->StepAngleMax*v->Freq;       
        
        if (v->Angle>1.0)
          v->Angle -= 1.0;
        else if (v->Angle<-1.0)
          v->Angle += 1.0;

       v->Out = v->Angle*v->Gain + v->Offset;

       if (v->Out>1.0)
          v->Out -= 1.0;
        else if (v->Out<-1.0)
          v->Out += 1.0;

}

void rampgen_calc_rol(RAMPGEN *v)
{

// Compute the angle rate
    v->Angle += v->StepAngleMax * v->Freq;

// Saturate the angle rate within (0,1)
    while (v->Angle >= 1.0)
	{
	    v->Angle -= 1.0;
	}
	while (v->Angle < 0.0)
	{
	    v->Angle += 1.0;
	}

// Compute the ramp output
    v->Out = v->Angle * v->Gain + v->Offset;

}

void rampgen_calc_sat(RAMPGEN *v)
{

// Compute the angle rate
    v->Angle += v->StepAngleMax * v->Freq;

// Saturate the angle rate within (-1,1)
    if (v->Angle>1.0)
        v->Angle = 1.0;
    else if (v->Angle<-1.0)
        v->Angle = -1.0;

// Compute the ramp output
    v->Out = v->Angle*v->Gain + v->Offset;

}

#pragma CODE_SECTION(rampgen_calc,"ramfuncs");
#pragma CODE_SECTION(rampgen_calc_rol,"ramfuncs");
#pragma CODE_SECTION(rampgen_calc_sat,"ramfuncs");
