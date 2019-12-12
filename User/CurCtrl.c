/*
 * CurCtrl.c
 *
 *  Created on: 2016-7-19
 *      Author: Administrator
 */
#include "DSP2833x_Device.h"
#include "DSP2833x_Examples.h"
#include "Algorithms.h"
#include "CurCtrl.h"
#include "CmdCtrl.h"
#include "Sample.h"
#include "ParaCalc.h"
#include "math.h"
#include "FPGA.h"

//=========UI Transformation================
float32 VolUPU_gfv;  //Transformer voltage at inverter side
float32 VolVPU_gfv;
float32 VolWPU_gfv;
float32 CurUDiffPU_gfv;  //differential current
float32 CurVDiffPU_gfv;
float32 CurWDiffPU_gfv;
float32 CurUComPU_gfv;  //common mode current
float32 CurVComPU_gfv;
float32 CurWComPU_gfv;
float32 CurDCPU_gfv;
//=======End UI Transformation==============

//============Sequence Judge================
volatile enum CMDMOD Cmd_SeqJudge_glv = CmdModNone;
volatile enum STATMOD Stat_SeqJudge_glv = StatModNone;
int32   Cnt_SJDir_glv;
float32 Sum_SJPrdt_gfv;
float32 SJVolPrevBePU_gfv;
CLARKE  SJVol_CLARKE = CLARKE_DEFAULTS;
//==========End of Sequence Judge============

//==========Sequence Transformation==========
//UVW is from hardware, ABC is in positive sequence
//ABC is used for phase lock or dq-trans
volatile enum CMDMOD  Cmd_SeqTrans_glv = CmdModNone;
float32 VolAPU_gfv;  //grid voltage(primary or secondary side of transformer)
float32 VolBPU_gfv;
float32 VolCPU_gfv;
float32 CurADiffPU_gfv;  //AC(differential) current
float32 CurBDiffPU_gfv;
float32 CurCDiffPU_gfv;
float32 Cur0DiffPU_gfv;
float32 CurAComPU_gfv;  //common mode current
float32 CurBComPU_gfv;
float32 CurCComPU_gfv;
float32 Cur0ComPU_gfv;
//==========End of Sequence Transformation==========

//==========Theta Estimate==========
volatile enum CMDMOD  Cmd_ThetaEst_glv = CmdModNone;
volatile enum STATMOD Stat_ThetaEst_glv = StatModNone;
int32   Cnt_ThetaEstOK_glv;     //State
float32 OmegaEstPU_gfv;			//Angular frequency of grid
float32 DeltaEstPU_gfv;			//Output
float32 DeltaFilPU_gfv;			//Output,State
float32 ThetaEstPU_gfv;			//1st phase angle
float32 ThetaEst2rdPU_gfv;		//2rd phase angle
float32 VolAlPU_gfv;		    //Output
float32 VolBePU_gfv;		    //Output
float32 VolDPU_gfv;		        //Output
float32 VolQPU_gfv;		        //Output
float32 CurAlDiffPU_gfv;	    //Output
float32 CurBeDiffPU_gfv;	    //Output
float32 CurDDiffPU_gfv;	        //Output
float32 CurQDiffPU_gfv;         //Output
float32 CurAlComPU_gfv;		    //Output
float32 CurBeComPU_gfv;		    //Output
float32 CurDComPU_gfv;	        //Output
float32 CurQComPU_gfv;	        //Output
float32 VolAmpPU_gfv;           //Output
float32 VolAmpFilPU_gfv;        //Output
PIDREG3 ThetaEst_PID = PIDREG3_DEFAULTS;         //PID for PLL
RAMPGEN ThetaEst_ROL = RAMPGEN_DEFAULTS_ROL;     //Omega to 1st Theta
RAMPGEN ThetaEst2rd_ROL = RAMPGEN_DEFAULTS_ROL;  //Omega to 2rd Theta
CLARKE  Vol_CLARKE = CLARKE_DEFAULTS;
CLARKE  Cur_CLARKE = CLARKE_DEFAULTS;
CLARKE  Cur2rd_CLARKE = CLARKE_DEFAULTS;
PARK    Vol_PARK = PARK_DEFAULTS;
PARK    Cur_PARK = PARK_DEFAULTS;
PARK    Cur2rd_PARK = PARK_DEFAULTS;
//==========End of Theta Estimate==========

//==========Current & Voltage Control==========

volatile enum CMDMOD Cmd_CurCtrl_glv = CmdModNone;
volatile enum STATMOD Stat_CurCtrl_glv = StatModNone;
int32   Flag_ACVolFwd_glv = 0;
float32 CurQMaxCmdPU_gfv = 0.1;
float32 CurQMaxPU_gfv;
float32 CurDCmdPU_gfv;
float32 CurQCmdPU_gfv;
float32 CurDComCmdPU_gfv;
float32 CurQComCmdPU_gfv;
float32 CurDComFilPU_gfv;
float32 CurQComFilPU_gfv;
float32 ConvVolDDiffPU_gfv;
float32 ConvVolQDiffPU_gfv;
float32 ConvVolAlDiffPU_gfv;
float32 ConvVolBeDiffPU_gfv;
float32 ConvVolADiffPU_gfv;
float32 ConvVolBDiffPU_gfv;
float32 ConvVolCDiffPU_gfv;
float32 ConvVolDComPU_gfv;
float32 ConvVolQComPU_gfv;
float32 ConvVolAlComPU_gfv;
float32 ConvVolBeComPU_gfv;
float32 ConvVolAComPU_gfv;
float32 ConvVolBComPU_gfv;
float32 ConvVolCComPU_gfv;
float32 DutyA_gfv;
float32 DutyB_gfv;
float32 DutyC_gfv;
float32 PWMPrd_gfv;
float32 PWMPrdPU_gfv;
float32 VolDC1FilPU_gfv;
float32 CurDCFilPU_gfv;
Uint32  OffDelayCnt_glv;
TFORDER2  ResoD6_TF = TFORDER2_DEFAULTS;
TFORDER2  ResoQ6_TF = TFORDER2_DEFAULTS;
IPARK   ConvVol_IPARK = IPARK_DEFAULTS;
ICLARKE ConvVol_ICLARKE = ICLARKE_DEFAULTS;
IPARK   ConvVolCom_IPARK = IPARK_DEFAULTS;
ICLARKE ConvVolCom_ICLARKE = ICLARKE_DEFAULTS;
PIDREG3 CurD_PID = PIDREG3_DEFAULTS;
PIDREG3 CurQ_PID = PIDREG3_DEFAULTS;
PIDREG3 CurDCom_PID = PIDREG3_DEFAULTS;
PIDREG3 CurQCom_PID = PIDREG3_DEFAULTS;
PIDREG3 VolDC_PID = PIDREG3_DEFAULTS;
//==========End of Current & Voltage Control==========

//Debug Buffer
int32 i=0;
float32 VolDBuf[200];
float32 VolQBuf[200];
float32 VolCDiffPUBuf_gfv[200];
float32 VolAComPUBuf_gfv[200];
float32 VolBComPUBuf_gfv[200];
float32 VolCComPUBuf_gfv[200];
float32 CurMaxPU_gfv;
int32 ENCCSCtl=0;

Uint16 FanOn_=0;

void UITrans(void)
{
	VolUPU_gfv = 0.33333 * (VUVPU_gfv - VWUPU_gfv);
	VolVPU_gfv = 0.33333 * (VVWPU_gfv - VUVPU_gfv);
	VolWPU_gfv = 0.33333 * (VWUPU_gfv - VVWPU_gfv);
	CurUDiffPU_gfv = IAPPU_gfv - IANPU_gfv;
	CurVDiffPU_gfv = IBPPU_gfv - IBNPU_gfv;
	CurWDiffPU_gfv = ICPPU_gfv - ICNPU_gfv;
	CurUComPU_gfv = -0.5 * (IAPPU_gfv + IANPU_gfv);
	CurVComPU_gfv = -0.5 * (IBPPU_gfv + IBNPU_gfv);
	CurWComPU_gfv = -0.5 * (ICPPU_gfv + ICNPU_gfv);
	CurDCPU_gfv =  0.3333 * (CurUComPU_gfv + CurVComPU_gfv + CurWComPU_gfv);

}


void SeqJudge(void)
{
	float32 VAmp_lfv;

	if(Cmd_SeqJudge_glv == CmdModReset)
	{
		Sum_SJPrdt_gfv = 0;
		Cnt_SJDir_glv = 0;
		Stat_SeqJudge_glv = StatModReset;
	}
	else if(Cmd_SeqJudge_glv == CmdModStart)
	{
		if(Stat_SeqJudge_glv & (StatModReset|StatModPrep))
		{
			SJVol_CLARKE.As = VolUPU_gfv;
			SJVol_CLARKE.Bs = VolVPU_gfv;
			SJVol_CLARKE.calc(&SJVol_CLARKE);

			if(Stat_SeqJudge_glv == StatModReset)
			{
				SJVolPrevBePU_gfv = SJVol_CLARKE.Beta;
				Stat_SeqJudge_glv = StatModPrep;
			}
			else if(Stat_SeqJudge_glv == StatModPrep)
			{
				//求电网电压幅值
				VAmp_lfv = SJVol_CLARKE.Alpha * SJVol_CLARKE.Alpha + SJVol_CLARKE.Beta * SJVol_CLARKE.Beta;
				//如果电网电压大于0.01PU，则执行相序判断
				if( VAmp_lfv >= 0.01)
				{
					Sum_SJPrdt_gfv += (SJVol_CLARKE.Beta - SJVolPrevBePU_gfv) * SJVol_CLARKE.Alpha;

					if(Sum_SJPrdt_gfv > PI)
					{
						Sum_SJPrdt_gfv = PI;
					}
					else if(Sum_SJPrdt_gfv < -PI)
					{
						Sum_SJPrdt_gfv = -PI;
					}

					if(Sum_SJPrdt_gfv >1.0)
					{
						Cnt_SJDir_glv ++;
					}
					else if(Sum_SJPrdt_gfv < -1.0)
					{
						Cnt_SJDir_glv --;
					}

					if(fabs(Cnt_SJDir_glv)>=1000)
					{
						if(Cnt_SJDir_glv > 0)
						{
							Stat_SeqJudge_glv = StatModRise;
						}
						else
						{
							Stat_SeqJudge_glv = StatModFall;
						}
					}
				}

				SJVolPrevBePU_gfv = SJVol_CLARKE.Beta;

			}

		}
	}
}

void SeqTrans(void)
{
	if(Cmd_SeqTrans_glv == CmdModReset)
	{
		VolAPU_gfv = 0.0;
		VolBPU_gfv = 0.0;
		VolCPU_gfv = 0.0;

		Cur0DiffPU_gfv = 0.0;
		CurADiffPU_gfv = 0.0;
		CurBDiffPU_gfv = 0.0;
		CurCDiffPU_gfv = 0.0;

		Cur0ComPU_gfv = 0.0;
		CurAComPU_gfv = 0.0;
		CurBComPU_gfv = 0.0;
		CurCComPU_gfv = 0.0;
	}
	else if(Cmd_SeqTrans_glv == CmdModStart)
	{
		if(Stat_SeqJudge_glv == StatModRise)
		{
			//Transformer voltage at Inverter side
			VolAPU_gfv = VolUPU_gfv;
			VolBPU_gfv = VolVPU_gfv;
			VolCPU_gfv = VolWPU_gfv;
			//differential current
			CurADiffPU_gfv = CurUDiffPU_gfv;
			CurBDiffPU_gfv = CurVDiffPU_gfv;
			CurCDiffPU_gfv = CurWDiffPU_gfv;
			//common mode current
			CurAComPU_gfv = CurUComPU_gfv;
			CurBComPU_gfv = CurWComPU_gfv;
			CurCComPU_gfv = CurVComPU_gfv;

		}
		else if(Stat_SeqJudge_glv == StatModFall)
		{
			VolAPU_gfv = VolUPU_gfv;
			VolBPU_gfv = VolWPU_gfv;
			VolCPU_gfv = VolVPU_gfv;

			CurADiffPU_gfv = CurUDiffPU_gfv;
			CurBDiffPU_gfv = CurWDiffPU_gfv;
			CurCDiffPU_gfv = CurVDiffPU_gfv;

			CurAComPU_gfv = CurUComPU_gfv;
			CurBComPU_gfv = CurVComPU_gfv;
			CurCComPU_gfv = CurWComPU_gfv;
		}
		//AC side current
		Cur0DiffPU_gfv = (CurADiffPU_gfv + CurBDiffPU_gfv + CurCDiffPU_gfv)/3.0;
		CurADiffPU_gfv = CurADiffPU_gfv - Cur0DiffPU_gfv;
		CurBDiffPU_gfv = CurBDiffPU_gfv - Cur0DiffPU_gfv;
		CurCDiffPU_gfv = CurCDiffPU_gfv - Cur0DiffPU_gfv;
		//Circulating current
		Cur0ComPU_gfv = (CurAComPU_gfv + CurBComPU_gfv + CurCComPU_gfv)/3.0;
		CurAComPU_gfv = CurAComPU_gfv - Cur0ComPU_gfv;
		CurBComPU_gfv = CurBComPU_gfv - Cur0ComPU_gfv;
		CurCComPU_gfv = CurCComPU_gfv - Cur0ComPU_gfv;
	}
}

void ThetaEst(void)
{
	if(Cmd_ThetaEst_glv == CmdModReset)
	{
		//Reset here, initialize PLL PI
		ThetaEst_PID.Kp = OmegaEstKp_gfv;
		ThetaEst_PID.Ki = OmegaEstKi_gfv;
		ThetaEst_PID.OutMax = 1.2;
		ThetaEst_PID.OutMin = -0.2;
		ThetaEst_PID.Ui   = 1.0;
		ThetaEst_PID.Err1 = 0.0;

		//Initialize Omega to Theta
		ThetaEst_ROL.StepAngleMax = ThetaEstKi_gfv;
		ThetaEst_ROL.Angle = 0.0;
		ThetaEst_ROL.Gain = 1.0;
		ThetaEst2rd_ROL.StepAngleMax = 2 * ThetaEstKi_gfv;
		ThetaEst2rd_ROL.Angle = 0.0;
		ThetaEst2rd_ROL.Gain = 1.0;

		//Initialize PLL flag & intermediate variable of PI
		DeltaFilPU_gfv = 0;
		Cnt_ThetaEstOK_glv = 0;

		Stat_ThetaEst_glv = StatModReset;
	}
	else if(Cmd_ThetaEst_glv == CmdModStart)
	{
		if(Stat_ThetaEst_glv == StatModReset)
		{
			Vol_CLARKE.As = VolAPU_gfv;
		    Vol_CLARKE.Bs = VolBPU_gfv;
		    Vol_CLARKE.calc(Vol_CLARKE);

			VolAlPU_gfv = Vol_CLARKE.Alpha;
			VolBePU_gfv = Vol_CLARKE.Beta;
			//A相Sin函数相位超前空间矢量90度，与此同时，由于此处锁相在q轴，故旋转空间矢量超前锁相得到的相位90度
			//因此A相sin函数相位超前锁相输出角度ThetaEstPU_gfv 180度
			ThetaEstPU_gfv = -atan2(VolAlPU_gfv, VolBePU_gfv) * INVPI2;
			ThetaEst_ROL.Angle = ThetaEstPU_gfv;
			ThetaEst2rd_ROL.Angle = ThetaEstPU_gfv;

			VolAmpFilPU_gfv = VolAmpPU_gfv = sqrt(VolAlPU_gfv * VolAlPU_gfv + VolBePU_gfv * VolBePU_gfv);
			Stat_ThetaEst_glv = StatModPrep;
		}
		else if(Stat_ThetaEst_glv & (StatModPrep|StatModNorm|StatModError))
		{
			//Clark Trans of Voltage
			Vol_CLARKE.As = VolAPU_gfv;
		    Vol_CLARKE.Bs = VolBPU_gfv;
		    Vol_CLARKE.calc(&Vol_CLARKE);
		    //Park Trans
			Vol_PARK.Alpha = Vol_CLARKE.Alpha;
		    Vol_PARK.Beta= Vol_CLARKE.Beta;
			Vol_PARK.Angle = ThetaEstPU_gfv;
		    Vol_PARK.calc(&Vol_PARK);
		    //Acquire Alpha, Beta, Ds, Qs
			VolAlPU_gfv = Vol_CLARKE.Alpha;
			VolBePU_gfv = Vol_CLARKE.Beta;
			VolDPU_gfv = Vol_PARK.Ds;
			VolQPU_gfv = Vol_PARK.Qs;

		    //Clark Trans of Differential Current
			Cur_CLARKE.As = CurADiffPU_gfv;
		    Cur_CLARKE.Bs = CurBDiffPU_gfv;
		    Cur_CLARKE.calc(&Cur_CLARKE);
 		    //Park Trans
			Cur_PARK.Alpha = Cur_CLARKE.Alpha;
		    Cur_PARK.Beta= Cur_CLARKE.Beta;
			Cur_PARK.Angle = ThetaEstPU_gfv;
		    Cur_PARK.calc(&Cur_PARK);
		    //Acquire Alpha, Beta, Ds, Qs
			CurAlDiffPU_gfv = Cur_CLARKE.Alpha;
			CurBeDiffPU_gfv = Cur_CLARKE.Beta;
			CurDDiffPU_gfv = Cur_PARK.Ds;
			CurQDiffPU_gfv = Cur_PARK.Qs;

		    //Clark Trans of Common mode Current
			Cur2rd_CLARKE.As = CurAComPU_gfv;
		    Cur2rd_CLARKE.Bs = CurBComPU_gfv;
		    Cur2rd_CLARKE.calc(&Cur2rd_CLARKE);
		    //Park Trans
			Cur2rd_PARK.Alpha = Cur2rd_CLARKE.Alpha;
		    Cur2rd_PARK.Beta= Cur2rd_CLARKE.Beta;
			Cur2rd_PARK.Angle = ThetaEst2rdPU_gfv;
		    Cur2rd_PARK.calc(&Cur2rd_PARK);
		    //Acquire Alpha, Beta, Ds, Qs
			CurAlComPU_gfv = Cur2rd_CLARKE.Alpha;
			CurBeComPU_gfv = Cur2rd_CLARKE.Beta;
			CurDComPU_gfv = Cur2rd_PARK.Ds;
			CurQComPU_gfv = Cur2rd_PARK.Qs;
			//filtering
			CurDComFilPU_gfv += (CurDComPU_gfv - CurDComFilPU_gfv) * LF100Gain_gfv;
			CurQComFilPU_gfv += (CurQComPU_gfv - CurQComFilPU_gfv) * LF100Gain_gfv;
			CurDComPU_gfv = CurDComFilPU_gfv;
			CurQComPU_gfv = CurQComFilPU_gfv;

			//phase lock calculation
			DeltaEstPU_gfv = -atan2(VolDPU_gfv, VolQPU_gfv) * INVPI2; //此处电压矢量锁相在q轴
			DeltaFilPU_gfv += (DeltaEstPU_gfv - DeltaFilPU_gfv) * LF100Gain_gfv;

			ThetaEst_PID.Err = DeltaFilPU_gfv;
			ThetaEst_PID.calc((Uint32)&ThetaEst_PID);
			OmegaEstPU_gfv = ThetaEst_PID.Out;

			//acquire the 1st order voltage phase angle
			#if Close_loop
			ThetaEst_ROL.Freq = OmegaEstPU_gfv;
			#else
			ThetaEst_ROL.Freq = 1;  // 开环控制，频率固定
			#endif
			ThetaEst_ROL.calc(&ThetaEst_ROL);
			ThetaEstPU_gfv = ThetaEst_ROL.Out;

			//acquire the 2rd order voltage phase angle
//			ThetaEst2rd_ROL.Freq = OmegaEstPU_gfv;
//			ThetaEst2rd_ROL.calc(&ThetaEst2rd_ROL);
//			ThetaEst2rdPU_gfv = ThetaEst2rd_ROL.Out;
			ThetaEst2rdPU_gfv = 2 * ThetaEstPU_gfv;

			//phase voltage amplitude
			VolAmpPU_gfv = sqrt(VolAlPU_gfv * VolAlPU_gfv + VolBePU_gfv * VolBePU_gfv);
			//low pass filter(10/6.28 Hz)
			VolAmpFilPU_gfv += (VolAmpPU_gfv - VolAmpFilPU_gfv) * LF10Gain_gfv;
			//phase lock state judge
			if(Stat_ThetaEst_glv == StatModPrep)
			{
				if(fabs(DeltaFilPU_gfv) < 0.05)
				{
					Cnt_ThetaEstOK_glv ++;
					if(Cnt_ThetaEstOK_glv >= 1000)
					{
						Cnt_ThetaEstOK_glv = 1000;
						Stat_ThetaEst_glv = StatModNorm;
					}
				}
				else
				{
					Cnt_ThetaEstOK_glv -= 50;
					if(Cnt_ThetaEstOK_glv < 0)
					{
						Cnt_ThetaEstOK_glv = 0;
					}
				}

			}
			else if(Stat_ThetaEst_glv == StatModNorm)
			{
				#if Close_loop
				if(fabs(DeltaFilPU_gfv) > 0.1)
				{
					Cnt_ThetaEstOK_glv -= 200.0 * fabs(DeltaFilPU_gfv);
				}
				else if(fabs(DeltaFilPU_gfv) < 0.05)
				{
					Cnt_ThetaEstOK_glv ++;
				}
				if(Cnt_ThetaEstOK_glv > 1000)
				{
					Cnt_ThetaEstOK_glv = 1000;
				}
				else if(Cnt_ThetaEstOK_glv <= 0) // 锁相失败
				{
					Cnt_ThetaEstOK_glv = 0;
					Stat_ThetaEst_glv = StatModError;
				}
				#endif
			}
		}
	}
}

void CurCtrl(void)
{
	float32 Temp_lfv;
	if(Cmd_CurCtrl_glv == CmdModReset)
	{
		CurD_PID.Kp = CurDKp_gfv;
		CurD_PID.Ki = CurDKi_gfv;
		CurD_PID.OutMax = 0.5 * MaxModuIndex_gfv * VolDCNormPU_gfv;
		CurD_PID.OutMin = -0.5 * MaxModuIndex_gfv * VolDCNormPU_gfv;
		CurD_PID.Err1 = 0.0;
		CurD_PID.Ui   = 0.0;

		CurQ_PID.Kp = CurQKp_gfv;
		CurQ_PID.Ki = CurQKi_gfv;
		CurQ_PID.OutMax = 0.5 * MaxModuIndex_gfv * VolDCNormPU_gfv;
		CurQ_PID.OutMin = -0.5 * MaxModuIndex_gfv * VolDCNormPU_gfv;
		CurQ_PID.Err1 = 0.0;
		CurQ_PID.Ui   = 0.0;

		CurDCom_PID.Kp = 4*CurDKp_gfv;
		CurDCom_PID.Ki = CurDKi_gfv;
		CurDCom_PID.OutMax = 0.5*VolDCNormPU_gfv;
		CurDCom_PID.OutMin = -0.5*VolDCNormPU_gfv;
		CurDCom_PID.Err1 = 0.0;
		CurDCom_PID.Ui   = 0.0;

		CurQCom_PID.Kp = 4*CurQKp_gfv;
		CurQCom_PID.Ki = CurQKi_gfv;
		CurQCom_PID.OutMax = 0.5*VolDCNormPU_gfv;
		CurQCom_PID.OutMin = -0.5*VolDCNormPU_gfv;
		CurQCom_PID.Err1 = 0.0;
		CurQCom_PID.Ui   = 0.0;

		VolDC_PID.Kp = VolDCKp_gfv;
		VolDC_PID.Ki = VolDCKi_gfv;
		VolDC_PID.OutMax = 1.5;
		VolDC_PID.OutMin = -1.5;
		VolDC_PID.Err1 = 0.0;
		VolDC_PID.Ui   = 0.0;

		CurQMaxPU_gfv = 0.0;
		PWMPrd_gfv = PWMPrdNorm_gfv;


		ConvVolAComPU_gfv = 0.0;
		ConvVolBComPU_gfv = 0.0;
		ConvVolCComPU_gfv = 0.0;
		ConvVolADiffPU_gfv = 0.0;
		ConvVolBDiffPU_gfv = 0.0;
		ConvVolCDiffPU_gfv = 0.0;

		CurQCmdPU_gfv = 0.0;
		CurDCmdPU_gfv = 0.0;
		CurMaxPU_gfv = 0.5;
		CurDComCmdPU_gfv = 0.0;
		CurQComCmdPU_gfv = 0.0;

		Stat_CurCtrl_glv = StatModReset;
	}
	else
	{
		if(Stat_CurCtrl_glv == StatModReset)
		{
			if(Cmd_CurCtrl_glv == CmdModStart)
			{
				if(!Flag_ACVolFwd_glv) // No AC Voltage Feed-Forward
				{
					ConvVolDDiffPU_gfv = CurD_PID.Ui = VolDPU_gfv; //初始化电流环的输出和PI积分值
					ConvVolQDiffPU_gfv = CurQ_PID.Ui = VolQPU_gfv;
				}
				else  // AC Voltage Feed-Forward
				{
					ConvVolDDiffPU_gfv = CurD_PID.Ui = 0;
					ConvVolQDiffPU_gfv = CurQ_PID.Ui = 0;
				}

				CurDCFilPU_gfv = IVDCPU_gfv;
				VolDC1FilPU_gfv = VDCPU_gfv;
				OffDelayCnt_glv = 0;
				Stat_CurCtrl_glv = StatModPrep;
			}
		}
		else if(Stat_CurCtrl_glv & (StatModPrep|StatModNorm))
		{
			if(Stat_CurCtrl_glv == StatModPrep)
			{
				if(Cmd_CurCtrl_glv == CmdModStart)
				{
					Stat_CurCtrl_glv = StatModNorm;
				}
				else if(Cmd_CurCtrl_glv == CmdModStop)
				{
					Stat_CurCtrl_glv = StatModStop;
				}
				else
				{
					Stat_CurCtrl_glv = StatModError;
				}
			}

			if(Stat_CurCtrl_glv == StatModNorm)
			{
				if(Cmd_CurCtrl_glv == CmdModStart)
				{
					SampACmdACOn();
					SMCmdFanOn();
//					if(FanOn_ == 1)
//					{
//						FanOn_ = SampBRegs.VPA - 15;
//						FanOn_ = 1;
//					}

//					FanOn_ ++;
//					if(FanOn_ == 10)
//					{
//						SMCmdFanOn();
//
//					}

					CurQMaxPU_gfv += (1.0/2.0) * SampTime_gfv; // 缓启动
					if(CurQMaxPU_gfv > 1.0)
					{
						CurQMaxPU_gfv = 1.0;
					}

					if(CurMaxPU_gfv > CurQMaxPU_gfv)
					{
						CurQCmdPU_gfv = CurQMaxPU_gfv;
					}
					else
					{
						CurQCmdPU_gfv = CurMaxPU_gfv;
					}
				}
				else if(Cmd_CurCtrl_glv == CmdModStop)
				{
					//软关断
//					CurQCmdPU_gfv -= (1.0/2.0) * SampTime_gfv;
//					if(CurQCmdPU_gfv <= 0.0)
//					{
//						CurQCmdPU_gfv = 0.0;
//						//CurQMaxPU_gfv = 0.0;
//						SMCmdFanOff();
//						SampACmdACOff();
//						Stat_CurCtrl_glv = StatModStop;
//					}
					CurQMaxPU_gfv -= (1.0/2.0) * SampTime_gfv;
					if(CurQMaxPU_gfv <= 0.0)
					{
						CurQMaxPU_gfv = 0.0;
						SMCmdFanOff();
						SampACmdACOff();
						Stat_CurCtrl_glv = StatModStop;
					}
					if(CurQCmdPU_gfv > CurQMaxPU_gfv)
					{
						CurQCmdPU_gfv = CurQMaxPU_gfv;
					}

				}
				else if(Cmd_CurCtrl_glv == CmdModTrip)
				{
					Stat_CurCtrl_glv = StatModError;
				}

                /********电压外环控制(整流)**********/
				//1st low bypass filter
				VolDC1FilPU_gfv += (VDCPU_gfv - VolDC1FilPU_gfv) * LF100Gain_gfv;
				CurDCFilPU_gfv += (IVDCPU_gfv - CurDCFilPU_gfv) * LF100Gain_gfv;

				Temp_lfv = VolAmpFilPU_gfv * 3.0;
				if(Temp_lfv > VolDCCmdPU_gfv * 0.9) //Debug!
					Temp_lfv = VolDCCmdPU_gfv * 0.9;
				VolDC_PID.Err = Temp_lfv - VDCPU_gfv;
				VolDC_PID.OutMax = CurQMaxPU_gfv;
				VolDC_PID.OutMin = -CurQMaxPU_gfv;
				VolDC_PID.calc((Uint32)&VolDC_PID);
//				CurQCmdPU_gfv = -VolDC_PID.Out;
//				CurDCmdPU_gfv = 0;
				/*******************************/

				//Differential CurLoop PID
				CurD_PID.Err = CurDCmdPU_gfv - CurDDiffPU_gfv;
				CurD_PID.calc((Uint32)&CurD_PID);
				CurQ_PID.Err = CurQCmdPU_gfv - CurQDiffPU_gfv;
				CurQ_PID.calc((Uint32)&CurQ_PID);

				#if Close_loop
				ConvVolDDiffPU_gfv = CurD_PID.Out;
				ConvVolQDiffPU_gfv = CurQ_PID.Out;
				// Feed Forward Decoupling(解耦算式由dq坐标变换公式和电路正方向决定，与电压定向在哪个轴无关)
				ConvVolDDiffPU_gfv = ConvVolDDiffPU_gfv - XqPU_gfv * CurQDiffPU_gfv * OmegaEstPU_gfv;
				ConvVolQDiffPU_gfv = ConvVolQDiffPU_gfv + XdPU_gfv * CurDDiffPU_gfv * OmegaEstPU_gfv;
				#else
				//ConvVolDDiffPU_gfv = CurQMaxPU_gfv;
				ConvVolQDiffPU_gfv = 0;
				if (ModuIndex_gfv < MaxModuIndex_gfv )
					ConvVolDDiffPU_gfv = ModuIndex_gfv * 0.5 * VolDCNormPU_gfv;
				else
					ConvVolDDiffPU_gfv = MaxModuIndex_gfv * 0.5 * VolDCNormPU_gfv;
				#endif


				// Line Side Voltage Feed Forward
				if(Flag_ACVolFwd_glv)
				{
					ConvVolDDiffPU_gfv = ConvVolDDiffPU_gfv + VolDPU_gfv;
					ConvVolQDiffPU_gfv = ConvVolQDiffPU_gfv + VolQPU_gfv;
				}

				#if Close_loop
				#if ENCCSC
				if (ENCCSCtl==1)
				{
					//Common mode CurLoop PID
					CurDCom_PID.Err = CurDComCmdPU_gfv - CurDComPU_gfv;
					CurDCom_PID.calc((Uint32)&CurDCom_PID);
					CurQCom_PID.Err = CurQComCmdPU_gfv - CurQComPU_gfv;
					CurQCom_PID.calc((Uint32)&CurQCom_PID);
					//circulating current suppression
					ConvVolDComPU_gfv = CurDCom_PID.Out;
					ConvVolQComPU_gfv = CurQCom_PID.Out;
				}
				else
				{
					ConvVolDComPU_gfv = 0;//CurDCom_PID.Out;
					ConvVolQComPU_gfv = 0;//CurQCom_PID.Out;
				}
				#else
				//no circulating current suppression
				ConvVolDComPU_gfv = 0;//CurDCom_PID.Out;
				ConvVolQComPU_gfv = 0;//CurQCom_PID.Out;
				#endif

				#else
				ConvVolDComPU_gfv = 0;
				ConvVolQComPU_gfv = 0;
				#endif
				// Feed Forward Decoupling
//				ConvVolDComPU_gfv = ConvVolDComPU_gfv - XqPU_gfv * CurQComPU_gfv * OmegaEstPU_gfv * 2.0;
//				ConvVolQComPU_gfv = ConvVolQComPU_gfv + XdPU_gfv * CurDComPU_gfv * OmegaEstPU_gfv * 2.0;

			}
		}
		else if(Stat_CurCtrl_glv & StatModStop)
		{
			if(OffDelayCnt_glv < 100)
			{
				OffDelayCnt_glv++;
			}
			else
			{
				ConvVolDDiffPU_gfv = 0.0;  //交流输出为0，保持子模块电容电压均衡
				ConvVolQDiffPU_gfv = 0.0;
			}
			if(Cmd_CurCtrl_glv == CmdModStart)
			{
				Stat_CurCtrl_glv = StatModReset;
			}
		}
		else if(Stat_CurCtrl_glv & StatModError)
		{
			;
		}

		if(Stat_CurCtrl_glv & (StatModPrep|StatModNorm|StatModStop))
		{
			//Differential Output Voltage
			ConvVol_IPARK.Ds = ConvVolDDiffPU_gfv;
			ConvVol_IPARK.Qs = ConvVolQDiffPU_gfv;
			ConvVol_IPARK.Angle = ThetaEstPU_gfv;
			ConvVol_IPARK.calc(&ConvVol_IPARK);
			ConvVolAlDiffPU_gfv = ConvVol_IPARK.Alpha;
			ConvVolBeDiffPU_gfv = ConvVol_IPARK.Beta;

			ConvVol_ICLARKE.Alpha = ConvVolAlDiffPU_gfv;
			ConvVol_ICLARKE.Beta = ConvVolBeDiffPU_gfv;
			ConvVol_ICLARKE.calc(&ConvVol_ICLARKE);
			ConvVolADiffPU_gfv = ConvVol_ICLARKE.As;
			ConvVolBDiffPU_gfv = ConvVol_ICLARKE.Bs;
			ConvVolCDiffPU_gfv = ConvVol_ICLARKE.Cs;

			//Common mode Output Voltage
			ConvVolCom_IPARK.Ds = ConvVolDComPU_gfv; //for close-loop
			ConvVolCom_IPARK.Qs = ConvVolQComPU_gfv; //for close-loop
			ConvVolCom_IPARK.Angle = ThetaEst2rdPU_gfv;
			ConvVolCom_IPARK.calc(&ConvVolCom_IPARK);
			ConvVolAlComPU_gfv = ConvVolCom_IPARK.Alpha;
			ConvVolBeComPU_gfv = ConvVolCom_IPARK.Beta;

			ConvVolCom_ICLARKE.Alpha = ConvVolAlComPU_gfv;
			ConvVolCom_ICLARKE.Beta = ConvVolBeComPU_gfv;
			ConvVolCom_ICLARKE.calc(&ConvVolCom_ICLARKE);
			ConvVolAComPU_gfv = ConvVolCom_ICLARKE.As;
			ConvVolBComPU_gfv = ConvVolCom_ICLARKE.Bs;
			ConvVolCComPU_gfv = ConvVolCom_ICLARKE.Cs;

		}
	}
}

#pragma CODE_SECTION(SeqJudge,"ramfuncs");
#pragma CODE_SECTION(SeqTrans,"ramfuncs");
#pragma CODE_SECTION(ThetaEst,"ramfuncs");
#pragma CODE_SECTION(CurCtrl,"ramfuncs");
