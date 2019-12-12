#include "dmctype.h"
#include "tf_order2.h"

/*-------------------------------
for a general transfer function in Z domain

Y(z)	A2Z^2 + A1Z^1 + A0 
---- = --------------------
X(z)	  Z^2 + B1Z^1 + B0

==>

Y(z)	A2 + A1Z^-1 + A0Z^-2 
---- = ----------------------
X(z)	 1 + B1Z^-1 + B0Z^-2

==>

y(n)+B1y(n-1)+B0y(n-2) = A2x(n) + A1x(n-1) + A0x(n-2)
y(n)= A2x(n) + A1x(n-1) + A0x(n-2) - B1y(n-1) - B0y(n-2) 
-------------------------------*/

void tf_order2_calc(TFORDER2 *v)
{	
	v->Yn = (v->A2)*(v->Xn) + (v->A1)*(v->Xn_1) + (v->A0)*(v->Xn_2) - (v->B1)*(v->Yn_1) - (v->B0)*(v->Yn_2);
	
	//Saturate the Y
	if(v->Yn > v->YMax)
	{
		v->Yn = v->YMax;
	}
	else if(v->Yn < v->YMin)
	{
		v->Yn = v->YMin;
	}

	v->Yn_2 = v->Yn_1;
	v->Yn_1 = v->Yn;
	v->Xn_2 = v->Xn_1;
	v->Xn_1 = v->Xn;
}

