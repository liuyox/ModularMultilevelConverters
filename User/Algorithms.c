/* =================================================================================
 Description:   Common algorithms                                   
=====================================================================================
 History:
-------------------------------------------------------------------------------------
 Created by LiuYongxia, 2018.04.30, Zhejiang University                                                  
------------------------------------------------------------------------------*/
#include "DSP28x_Project.h"

Uint32 	BitConvF2Uint(float32 Var)
{
	return(*((Uint32 *)(&Var)));
}

float32 BitConvUint2F(Uint32 Var)
{
	return(*((float32 *)(&Var)));
}

