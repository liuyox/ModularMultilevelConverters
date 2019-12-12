/* =================================================================================
 Description:   Vector Algorisms                                   
=====================================================================================
 History:
-------------------------------------------------------------------------------------
 Created by LiuYongxia, 2018.04.13, Zhejiang University. For IQ format.
 Modified by LiuYongxia, 2018.06.03, Zhejiang University. For float32.
------------------------------------------------------------------------------*/

#include "dmctype.h"
#include "Vector.h"
#include "math.h"

void VectorAttrb(VECTOR *VectorA, VECTOR *VectorB)
{
	VectorB->Re = VectorA->Re;
	VectorB->Im = VectorA->Im;
}

void VectorAdd(VECTOR *VectorA, VECTOR *VectorB, VECTOR *VectorC)
{
	VectorC->Re = VectorA->Re + VectorB->Re;
	VectorC->Im = VectorA->Im + VectorB->Im;
}

void VectorSub(VECTOR *VectorA, VECTOR *VectorB, VECTOR *VectorC)
{
	VectorC->Re = VectorA->Re - VectorB->Re;
	VectorC->Im = VectorA->Im - VectorB->Im;
}

void VectorMpy(VECTOR *VectorA, VECTOR *VectorB, VECTOR *VectorC)
{
	VectorC->Re = (VectorA->Re) * (VectorB->Re) - (VectorA->Im) * (VectorB->Im);
	VectorC->Im = (VectorA->Re) * (VectorB->Im) + (VectorA->Im) * (VectorB->Re);
}

void VectorMpyNum(VECTOR *VectorA, VECTOR *VectorB, float32 Num)
{
	VectorB->Re = (VectorA->Re) * Num;
	VectorB->Im = (VectorA->Im) * Num;
}

void VectorExp(VECTOR *VectorA, VECTOR *VectorB)
{
	float32	Temp;
	Temp = exp(VectorA->Re);
	VectorB->Re = Temp * cos(VectorA->Im);
	VectorB->Im = Temp * sin(VectorA->Im);
}

//Difference Forward
/*void VectorTF1Calc(VECTOR_TF1 *VectorTF1)
{
	VECTOR Temp1_VECTOR;
	VECTOR Temp2_VECTOR;
	VECTOR Temp3_VECTOR;
	
	VectorMpy(&(VectorTF1->B),&(VectorTF1->In),&Temp1_VECTOR);
	VectorMpy(&(VectorTF1->A),&(VectorTF1->Out),&Temp2_VECTOR);
	VectorSub(&Temp1_VECTOR,&Temp2_VECTOR,&Temp3_VECTOR);
	
	VectorAdd(&Temp3_VECTOR , &(VectorTF1->Out), &Temp2_VECTOR);
	VectorAttrb(&Temp2_VECTOR ,&(VectorTF1->Out));
}*/

//Zero Pole
void VectorTF1Calc(VECTOR_TF1 *VectorTF1)
{
	VECTOR Temp1_VECTOR;
	VECTOR Temp2_VECTOR;
	VECTOR Temp3_VECTOR;
	
	VectorMpy(&(VectorTF1->B),&(VectorTF1->In),&Temp1_VECTOR);
	VectorAdd(&Temp1_VECTOR, &(VectorTF1->Out), &Temp2_VECTOR);
	Temp3_VECTOR.Re = -VectorTF1->A.Re;
	Temp3_VECTOR.Im = -VectorTF1->A.Im;
	VectorExp(&Temp3_VECTOR, &Temp1_VECTOR);
	VectorMpy(&Temp1_VECTOR, &Temp2_VECTOR, &Temp3_VECTOR);
	VectorAttrb(&Temp3_VECTOR ,&(VectorTF1->Out));
}




