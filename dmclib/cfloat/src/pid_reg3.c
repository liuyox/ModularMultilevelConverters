/*=====================================================================================
 File name:        PID_REG3.C                   
                    
 Originator:	Digital Control Systems Group
			Texas Instruments

 Description:  PID controller with anti-windup          

=====================================================================================
 History:
-------------------------------------------------------------------------------------
 04-15-2005	Version 3.20
 Modified by LiuYongxia @ZhejiangUniversity @2018.04
-------------------------------------------------------------------------------------*/

#include "dmctype.h"
#include "pid_reg3.h"

void pid_reg3_calc(PIDREG3 *v)
{
    // Compute the proportional output
    v->Up = v->Kp*v->Err;

    // Compute the integral output
    v->Ui = v->Ui + v->Ki*v->Err;
    
    // Compute the derivative output
    v->Ud = v->Kd*(v->Err - v->Err1);

    // Saturate the integrator
    if (v->Ui > v->OutMax)
      v->Ui =  v->OutMax;
    else if (v->Ui < v->OutMin)
      v->Ui =  v->OutMin;

    // Compute the pre-saturated output
    v->OutPreSat = v->Up + v->Ui + v->Ud;     
    
    // Saturate the output
    if (v->OutPreSat > v->OutMax)                   
      v->Out =  v->OutMax;
    else if (v->OutPreSat < v->OutMin)
      v->Out =  v->OutMin;  
    else
      v->Out = v->OutPreSat;
    
    // Update the previous proportional output 
    v->Err1 = v->Err;
}

#pragma CODE_SECTION(pid_reg3_calc,"ramfuncs");
