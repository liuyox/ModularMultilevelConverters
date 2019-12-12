/*
 * svgen.c
 *
 *  Created on: 2013-12-13
 *      Author: WangYuxiang
 *
 *  Modified by LiuYongxia  @2018-05-26  @ZhejiangUniversity
 *
 */

#include "dmctype.h"
#include "svgen.h"

void svgendq_calc(SVGENDQ *v)
{

	float32 Va,Vb,Vc,t1,t2;
	Uint32 Sector = 0;  // Sector is treated as Q0 - independently with global Q

// Inverse clarke transformation
    Va = v->Ubeta;
    Vb = -0.5 * v->Ubeta + 0.8660254 * v->Ualpha;  // 0.8660254 = sqrt(3)/2
    Vc = -0.5 * v->Ubeta - 0.8660254 * v->Ualpha;  // 0.8660254 = sqrt(3)/2

// 60 degree Sector determination
    if (Va>0)
       Sector = 1;
    if (Vb>0)
       Sector = Sector + 2;
    if (Vc>0)
       Sector = Sector + 4;

// X,Y,Z (Va,Vb,Vc) calculations
    Va = v->Ubeta;                                                       // X = Va
    Vb = 0.5 * v->Ubeta + 0.8660254 * v->Ualpha;   // Y = Vb
    Vc = 0.5 * v->Ubeta - 0.8660254 * v->Ualpha;   // Z = Vc

    if (Sector==0)  // Sector 0: this is special case for (Ualpha,Ubeta) = (0,0)
    {
       v->Ta = 0.5;
       v->Tb = 0.5;
       v->Tc = 0.5;
    }
    if (Sector==1)  // Sector 1: t1=Z and t2=Y (abc ---> Tb,Ta,Tc)
    {
       t1 = Vc;
       t2 = Vb;
       v->Tb = 0.5 * (1-t1-t2);      // tbon = (1-t1-t2)/2
       v->Ta = v->Tb+t1;                             // taon = tbon+t1
       v->Tc = v->Ta+t2;                             // tcon = taon+t2
    }
    else if (Sector==2)  // Sector 2: t1=Y and t2=-X (abc ---> Ta,Tc,Tb)
    {
       t1 = Vb;
       t2 = -Va;
       v->Ta = 0.5 * (1-t1-t2);      // taon = (1-t1-t2)/2
       v->Tc = v->Ta+t1;                             // tcon = taon+t1
       v->Tb = v->Tc+t2;                             // tbon = tcon+t2
    }
    else if (Sector==3)  // Sector 3: t1=-Z and t2=X (abc ---> Ta,Tb,Tc)
    {
       t1 = -Vc;
       t2 = Va;
       v->Ta = 0.5 * (1-t1-t2);      // taon = (1-t1-t2)/2
       v->Tb = v->Ta+t1;                             // tbon = taon+t1
       v->Tc = v->Tb+t2;                             // tcon = tbon+t2
    }
    else if (Sector==4)  // Sector 4: t1=-X and t2=Z (abc ---> Tc,Tb,Ta)
    {
       t1 = -Va;
       t2 = Vc;
       v->Tc = 0.5 * (1-t1-t2);      // tcon = (1-t1-t2)/2
       v->Tb = v->Tc+t1;                             // tbon = tcon+t1
       v->Ta = v->Tb+t2;                             // taon = tbon+t2
    }
    else if (Sector==5)  // Sector 5: t1=X and t2=-Y (abc ---> Tb,Tc,Ta)
    {
       t1 = Va;
       t2 = -Vb;
       v->Tb = 0.5 * (1-t1-t2);      // tbon = (1-t1-t2)/2
       v->Tc = v->Tb+t1;                             // tcon = tbon+t1
       v->Ta = v->Tc+t2;                             // taon = tcon+t2
    }
    else if (Sector==6)  // Sector 6: t1=-Y and t2=-Z (abc ---> Tc,Ta,Tb)
    {
       t1 = -Vb;
       t2 = -Vc;
       v->Tc = 0.5 * (1-t1-t2);      // tcon = (1-t1-t2)/2
       v->Ta = v->Tc+t1;                             // taon = tcon+t1
       v->Tb = v->Ta+t2;                             // tbon = taon+t2
    }

    //Output are ranged [0,1]
    //The transformation below is to be in accordance with the configuration in ePWM Module
    //The upper switch is On when the modulating wave is higher than the carrier
    //That is, tHe higher T(abc), the higher V(abc)
    v->Ta = 1.0 - v->Ta;
    v->Tb = 1.0 - v->Tb;
    v->Tc = 1.0 - v->Tc;
}



