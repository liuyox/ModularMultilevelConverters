/* =================================================================================
 Description:   Head File for Vector.c                                   
=====================================================================================
 History:
-------------------------------------------------------------------------------------
 Created by LiuYongxia, 2018.04.13, Zhejiang University. For IQ format.
 Modified by LiuYongxia, 2018.06.03, Zhejiang University. For float32.
------------------------------------------------------------------------------*/
#ifndef VECTOR_H
#define VECTOR_H

typedef struct{	float32 Re;
				float32 Im;
} VECTOR;

typedef struct{	VECTOR In;
				VECTOR Out;
				VECTOR A;
				VECTOR B;
				void  (*Calc)(Uint32);// Pointer to calculation function
} VECTOR_TF1;

extern void VectorAttrb(VECTOR *, VECTOR *);
extern void VectorAdd(VECTOR *, VECTOR *, VECTOR *); 
extern void VectorSub(VECTOR *, VECTOR *, VECTOR *);
extern void VectorMpy(VECTOR *, VECTOR *, VECTOR *);
extern void VectorMpyNum(VECTOR *, VECTOR *, float32 Num);
extern void VectorExp(VECTOR *, VECTOR *);
extern void VectorTF1Calc(VECTOR_TF1 *);

#define VECTOR_ZERO { 	0, \
                        0 }

#define VECTOR_TF1_DEFAULTS  { 	VECTOR_ZERO, \
                        		VECTOR_ZERO, \
                        		VECTOR_ZERO, \
                        		VECTOR_ZERO, \
              					(void (*)(Uint32))VectorTF1Calc }

#endif //VECTOR_H
 
