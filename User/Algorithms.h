/* =================================================================================
 Description:   Head file for Algorithms.c                                     
=====================================================================================
 History:
-------------------------------------------------------------------------------------
 Created by LiuYongxia, 2018.04.30, Zhejiang University                                                  
------------------------------------------------------------------------------*/

#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#define PI			3.1415926535897932384626433832795f
#define PI2			6.283185307179586476925286766559f
#define SQRT2		1.4142135623730950488016887242097f
#define SQRT3		1.7320508075688772935274463415059f
#define SQRT3D2		0.86602540378443864676372317075294f

#define INVPI		(1.0f/PI)
#define INVPI2		(1.0f/PI2)
#define INVSQRT2	(1.0f/SQRT2)
#define INVSQRT3	(1.0f/SQRT3)

#define FALSE 0
#define TRUE  1

#define OFF   0
#define ON    1

extern Uint32  BitConvF2Uint(float32 );
extern float32 BitConvUint2F(Uint32 );
 
#endif    // end of Algorithms.h



