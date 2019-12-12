/*
 * ParaCalc.c
 *
 *  Created on: 2016-7-19
 *      Author: Administrator
 */
#include "DSP2833x_Device.h"
#include "DSP2833x_Examples.h"
#include "ParaCalc.h"
#include "Algorithms.h"
#include "Sample.h"
#include "main.h"
#include "CmdCtrl.h"
//==========System Parameter Calculation==========

//////////  Base and Norm Parameter
Uint32  NumTotal_gulv;    //桥臂中子模块总数（e.g. for 9L-MMC, NumTotal_gulv = 8）
Uint32  NumSort_gulv;	  //桥臂中参与排序的子模块数
Uint32  NumNorm_gulv;     //桥臂中实际运行的子模块数
Uint32  SMTYPE_gulv;      //SMTYPE=0, MMC所有子模块均配置为半桥(T3L)
                          //SMTYPE=1, MMC所有子模块均配置为全桥(F2L)
Uint32  NumHBSM_gulv;     //定义hybrid MMC 中半桥子模块数目. Only effective when SMTYPE=1

float32 VolSMNorm_gfv;   //SM Voltage
float32 PowerNorm_gfv;
float32 VolDCNorm_gfv;   //bi-pole
float32 VolACNorm_gfv;   //line-line RMS
float32 CurDCNorm_gfv;   //single-pole
float32 CurACNorm_gfv;   //phase RMS
float32 MaxModuIndex_gfv; // max modulation index

float32 PowerBase_gfv;
float32 VolBase_gfv;
float32 CurBase_gfv;
float32 ImpBase_gfv;
float32 AdmBase_gfv;
float32 FreqBase_gfv;
float32 OmegaBase_gfv;

float32 VolDCNormPU_gfv; //bi-pole
float32 CurDCNormPU_gfv; //single-pole
float32 VolSMNormPU_gfv;   //SM Voltage
float32 INVVolSMNormPU_gfv;
Uint32 PWMDutyIndex_gulv;
//////////  End of Base and Norm Parameter

//////////  Sampling Parameter
//Real Coefficient of Sample Cabinet A
float32 IPACoef_gfv;
float32 IPBCoef_gfv;
float32 IPCCoef_gfv;
float32 IANCoef_gfv;
float32 IBNCoef_gfv;
float32 ICNCoef_gfv;
float32 VUVCoef_gfv;
float32 VVWCoef_gfv;
float32 VWUCoef_gfv;
float32 IVDCCoef_gfv;
float32 VDCRCoef_gfv;
//Real Coefficient of Sample Cabinet B
float32 VUVGCoef_gfv;
float32 VVWGCoef_gfv;
float32 VWUGCoef_gfv;
float32 IVDCCoef_gfv;
float32 VPACoef_gfv;
float32 VPBCoef_gfv;
float32 VPCCoef_gfv;
float32 VANCoef_gfv;
float32 VBNCoef_gfv;
float32 VCNCoef_gfv;
float32 VDCCoef_gfv;
//Real Coefficient of SM Cabinet
float32 VSMCoef_gfv;
//PU Coefficient
float32 VBaseCoef_gfv;
float32 CBaseCoef_gfv;
//////////  End of Sampling Parameter

//////////  Control Parameter
float32 SwitFreq_gfv;
float32 SwitTime_gfv;
float32 SampFreq_gfv;
float32 SampTime_gfv;
float32 PWMPrdNorm_gfv;

float32 LArm_gfv;
float32 LTran_gfv;
float32 LAC_gfv;
float32 CSM_gfv;
float32 LACPU_gfv;
float32 CSMPU_gfv;
float32 XdPU_gfv;
float32 XqPU_gfv;

float32 VolDCCmdPU_gfv;
float32 ModuIndex_gfv;
//float32 ModIndex_gfv;


float32 LF100Gain_gfv;
float32 LF10Gain_gfv;
float32 LF1Gain_gfv;
float32 OmegaEstKp_gfv;
float32 OmegaEstKi_gfv;
float32 ThetaEstKi_gfv;
float32 CurDKp_gfv;
float32 CurDKi_gfv;
float32 CurQKp_gfv;
float32 CurQKi_gfv;
float32 VolDCKp_gfv;
float32 VolDCKi_gfv;

Uint16 VolSMNorm_gulv;
Uint16 VolSMMax_gusv;
Uint16 VolSMMin_gusv;
Uint16 StaSMChk_gusv;

Uint16 SciErrSampACnt_gusv;
Uint16 SciErrSampBCnt_gusv;
Uint16 SciErrSMCnt_gusv;
Uint16 StaErrSampACnt_gusv;
Uint16 StaErrSampBCnt_gusv;
Uint16 StaErrSMCnt_gusv;
//////////  End of Control Parameter

void ParaCalc(void)
{
	BaseCalc();
	SampCalc();
	CtrlCalc();
//	InitMonitorVar();
}
void BaseCalc(void)
{
	//=====================================================================
	//Set System Parameters here:
	NumTotal_gulv= FBSM13L; //HBSM25L;//FBSM13L; 	//Total SM Number of MMC
	NumNorm_gulv = FBSM13L; //HBSM25L;//FBSM13L;
	NumSort_gulv = 12;   	 // 9 level
	NumHBSM_gulv = 12; 		 // Only effective in Hybrid MMC, Otherwise NumHBSM_gulv=0
	VolDCNorm_gfv = 400; 	 //400V
	VolACNorm_gfv = 113*1.732;  //100*1.732;//Set the AC voltage reference
	PowerNorm_gfv = 3200.0;
	CurDCNorm_gfv = PowerNorm_gfv/VolDCNorm_gfv;
	CurACNorm_gfv = PowerNorm_gfv/VolACNorm_gfv/SQRT3;
	//===================================================================
#if EN_OverModu
	VolSMNorm_gfv = 80; 	 //过调制时：子模块电容电压独立于直流电压和排序子模块数
	MaxModuIndex_gfv =1.4; 	 // define the max modulation index
#else
	VolSMNorm_gfv = VolDCNorm_gfv / NumSort_gulv;
	MaxModuIndex_gfv =1.0; 	 // define the max modulation index
#endif

#if Close_loop == 0
	ModuIndex_gfv = 0.8; 	// define modulation index in open loop control
#endif
	//Set SM Topology
	if ((NumTotal_gulv == FBSM13L) || (NumTotal_gulv == FBSM9L) || (NumTotal_gulv == FBSM5L))
		SMTYPE_gulv  = TopologyF2L; // SM Type Configure: T3L or F2L
	else
		SMTYPE_gulv  = TopologyT3L;
	//Set System References: voltage, power and current
	FreqBase_gfv = 50.0;
	OmegaBase_gfv = PI2 * FreqBase_gfv;
	PowerBase_gfv = PowerNorm_gfv;
	VolBase_gfv = VolACNorm_gfv / SQRT3 * SQRT2;
	CurBase_gfv = 2.0 / 3.0 * PowerBase_gfv / VolBase_gfv;
	ImpBase_gfv = VolBase_gfv/CurBase_gfv;
	AdmBase_gfv = 1.0 / ImpBase_gfv;
	//used for PU Trans
	VBaseCoef_gfv = 1.0 / VolBase_gfv;
	CBaseCoef_gfv = 1.0 / CurBase_gfv;
	//used for protection
	VolDCNormPU_gfv =VolDCNorm_gfv / VolBase_gfv;
	CurDCNormPU_gfv = CurDCNorm_gfv / CurBase_gfv;
	VolSMNormPU_gfv = VolSMNorm_gfv / VolBase_gfv;
	INVVolSMNormPU_gfv = 1.0 / VolSMNormPU_gfv;
	PWMDutyIndex_gulv = (Uint32)(VolBase_gfv / VolSMNorm_gfv * FPGA_PERIOD);
}
void SampCalc(void)
{
	//Sample Cabinet A
	IPACoef_gfv = 30.2/1.25 * 3.0/4096.0 ;
	IPBCoef_gfv = 30.2/1.25 * 3.0/4096.0;
	IPCCoef_gfv = 30.2/1.25 * 3.0/4096.0;
	IANCoef_gfv = 30.2/1.25 * 3.0/4096.0;
	IBNCoef_gfv = 30.2/1.25 * 3.0/4096.0;
	ICNCoef_gfv = 30.2/1.25 * 3.0/4096.0;
	VUVCoef_gfv = 1905/1.25 * 3.0/4096.0;
	VVWCoef_gfv = 1905/1.25 * 3.0/4096.0;
	VWUCoef_gfv = 1905/1.25 * 3.0/4096.0;
	IVDCCoef_gfv = 22.25/1.25 * 3.0/4096.0;
	VDCRCoef_gfv = 2200/2.73 * 3.0/4096.0;

	//Sample Cabinet B
	VUVGCoef_gfv = 591/1.25 * 3.0/4096.0;
	VVWGCoef_gfv = 591/1.25 * 3.0/4096.0;
	VWUGCoef_gfv = 591/1.25 * 3.0/4096.0;
	IVDCCoef_gfv = 22.25/1.25 * 3.0/4096.0;
	VPACoef_gfv = 2200/2.73 * 3.0/4096.0;
	VPBCoef_gfv = 2200/2.73 * 3.0/4096.0;
	VPCCoef_gfv = 2200/2.73 * 3.0/4096.0;
	VANCoef_gfv = 2200/2.73 * 3.0/4096.0;
	VBNCoef_gfv = 2200/2.73 * 3.0/4096.0;
	VCNCoef_gfv = 2200/2.73 * 3.0/4096.0;
	VDCCoef_gfv = 2200/2.73 * 3.0/4096.0;

	//SM Cabinet
	VSMCoef_gfv = 120.0/2.73 *3.0/4096.0;
}
void CtrlCalc(void)
{
	Uint16 i; //局部变量
	SwitFreq_gfv = 5000.0;
	SwitTime_gfv = 1.0/SwitFreq_gfv;
	SampFreq_gfv = SwitFreq_gfv;
	SampTime_gfv =  1.0/SampFreq_gfv;
	PWMPrdNorm_gfv = 150000000.0L / ((float64)SwitFreq_gfv) / 2.0L;

	LArm_gfv = 4e-3;
	LTran_gfv = 0.15e-3;
	LAC_gfv = LArm_gfv / 2 + LTran_gfv;
	CSM_gfv = 4.8e-3;
	LACPU_gfv = LAC_gfv * AdmBase_gfv;
	CSMPU_gfv = CSM_gfv * ImpBase_gfv;
	XdPU_gfv = LACPU_gfv * OmegaBase_gfv;
	XqPU_gfv = LACPU_gfv * OmegaBase_gfv;

	VolDCCmdPU_gfv = VolDCNormPU_gfv;
	//ModIndex_gfv = VolDCCmdPU_gfv / SQRT3;

	LF100Gain_gfv = 100.0 * SampTime_gfv;
	LF10Gain_gfv = 10.0 * SampTime_gfv;
	LF1Gain_gfv = 1.0 * SampTime_gfv;
	OmegaEstKp_gfv = 2.5 * PI2 / FreqBase_gfv;
	OmegaEstKi_gfv = SampTime_gfv * OmegaEstKp_gfv * 2.5 * PI2 * 0.25;
	ThetaEstKi_gfv = FreqBase_gfv * SampTime_gfv;
	CurQKp_gfv = CurDKp_gfv = 100.0 * PI2 * LACPU_gfv;
	CurQKi_gfv = CurDKi_gfv = SampTime_gfv * CurDKp_gfv * 50.0 * PI2 * 0.25;
	VolDCKp_gfv = 15.0 * PI2 * 0.5 * CSMPU_gfv * (2.0/3.0) * VolDCCmdPU_gfv;
	VolDCKi_gfv = SampTime_gfv * VolDCKp_gfv * 15.0 * PI2 * 0.5;

	//子模块预充电通过的最大最小电压
	VolSMMax_gusv = (Uint16)(1.3 * VolSMNorm_gfv / (SMTYPE_gulv + 1) / VSMCoef_gfv);
	VolSMMin_gusv = (Uint16)(0.7 * VolSMNorm_gfv / (SMTYPE_gulv + 1) / VSMCoef_gfv);
	//子模块正常状态参考值
	StaSMChk_gusv = 0;
	for(i=0; i<(Uint16)((SMTYPE_gulv + 1) * 0.5 * NumTotal_gulv); i++)
	{
		StaSMChk_gusv = (StaSMChk_gusv << 1) + 1;
	}
	//故障计数起始值为零
	SciErrSampACnt_gusv = 0;
	SciErrSampBCnt_gusv = 0;
	SciErrSMCnt_gusv = 0;
	StaErrSampACnt_gusv = 0;
	StaErrSampBCnt_gusv = 0;
	StaErrSMCnt_gusv = 0;

}
//==========End of System Parameter Calculation==========


