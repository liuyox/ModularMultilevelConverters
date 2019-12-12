#ifndef __TFORDER2_H__
#define __TFORDER2_H__

typedef struct {  float32	Yn;
				  float32	Yn_1;
				  float32	Yn_2;
				  float32	Xn;
				  float32	Xn_1;
				  float32	Xn_2;

				  float32	B0;
				  float32	B1;
				  float32	A0;
				  float32	A1;
				  float32	A2;

				  float32	YMax;
				  float32	YMin;
				  void  (*calc)(Uint32);
				 } TFORDER2;	            

typedef TFORDER2 *TFORDER2_handle;


#define TFORDER2_DEFAULTS		{ 0, \
								  0, \
								  0, \
								  0, \
								  0, \
								  0, \
								  0, \
								  0, \
								  0, \
								  0, \
								  0, \
								  0, \
								  0, \
              					  (void (*)(Uint32))tf_order2_calc }


void tf_order2_calc(TFORDER2_handle);

#endif
