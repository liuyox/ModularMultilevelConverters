/*
 * ParaCalc.h
 *
 *  Created on: 2016-7-19
 *      Author: Administrator
 */

#ifndef PARACALC_H_
#define PARACALC_H_

//SM Number Config
#define SMNUM_HB25L		24
#define SMNUM_HB17L		16
#define SMNUM_HB9L		8
#define SMNUM_FB13L		12
#define SMNUM_FB9L		8
#define SMNUM_FB5L		4



//MMC SM Number Configure
enum MMC_SMNUM {   	HBSM25L         = 24,
					HBSM17L         = 16,
					HBSM9L         	= 8,
					FBSM13L        	= 12,
					FBSM9L        	= 8,
					FBSM5L         	= 4};

//SM Topology Set
enum SMTOPOLOGY { 	TopologyT3L     = 0,
                  	TopologyF2L    	= 1};
extern void ParaCalc(void);
extern void BaseCalc(void);
extern void SampCalc(void);
extern void CtrlCalc(void);
extern void InitMonitorVar(void);
extern Uint32  NumTotal_gulv;
extern Uint32  NumNorm_gulv;     //SM number
extern Uint32  NumSort_gulv;      //Activated SM
extern Uint32  SMTYPE_gulv;      //SMTYPE=0, all SMs are HBSMs, otherwise all the SMs are FBSMs
extern Uint32  NumHBSM_gulv;     // Only effective when SMTYPE=1

extern float32 VolSMNorm_gfv;   //SM Voltage
extern float32 PowerNorm_gfv;
extern float32 VolDCNorm_gfv;   //bi-pole
extern float32 VolACNorm_gfv;   //line-line RMS
extern float32 CurDCNorm_gfv;   //single-pole
extern float32 CurACNorm_gfv;   //phase RMS
extern float32 MaxModuIndex_gfv;
extern float32 ModuIndex_gfv;

extern float32 PowerBase_gfv;
extern float32 VolBase_gfv;
extern float32 CurBase_gfv;
extern float32 ImpBase_gfv;
extern float32 AdmBase_gfv;
extern float32 FreqBase_gfv;
extern float32 OmegaBase_gfv;
extern float32 VBaseCoef_gfv;
extern float32 CBaseCoef_gfv;
//extern Uint32 VolSMNorm_gulv;
extern float32 VolDCNormPU_gfv; //bi-pole
extern float32 CurDCNormPU_gfv; //single-pole
extern float32 VolSMNormPU_gfv;   //SM Voltage
extern float32 INVVolSMNormPU_gfv;
extern Uint32 PWMDutyIndex_gulv;
//////////  End of Base and Norm Parameter

//////////  Sampling Parameter
//Real Coefficient of Sample Cabinet A
extern float32 IAPCoef_gfv;
extern float32 IBPCoef_gfv;
extern float32 ICPCoef_gfv;
extern float32 IANCoef_gfv;
extern float32 IBNCoef_gfv;
extern float32 ICNCoef_gfv;
extern float32 VUVCoef_gfv;
extern float32 VVWCoef_gfv;
extern float32 VWUCoef_gfv;
extern float32 IVDCCoef_gfv;
extern float32 VDCRCoef_gfv;

//Real Coefficient of Sample Cabinet B
extern float32 VUVGCoef_gfv;
extern float32 VVWGCoef_gfv;
extern float32 VWUGCoef_gfv;
extern float32 IVDCCoef_gfv;
extern float32 VPACoef_gfv;
extern float32 VPBCoef_gfv;
extern float32 VPCCoef_gfv;
extern float32 VANCoef_gfv;
extern float32 VBNCoef_gfv;
extern float32 VCNCoef_gfv;
extern float32 VDCCoef_gfv;
//Real Coefficient of SM Cabinet
extern float32 VSMCoef_gfv;
extern float32 ISMCoef_gfv;
//////////  End of Sampling Parameter

//////////  Control Parameter
extern float32 SwitFreq_gfv;
extern float32 SwitTime_gfv;
extern float32 SampFreq_gfv;
extern float32 SampTime_gfv;
extern float32 PWMPrdNorm_gfv;

extern float32 LAC_gfv;
extern float32 CDC_gfv;
extern float32 LACPU_gfv;
extern float32 CDCPU_gfv;
extern float32 XdPU_gfv;
extern float32 XqPU_gfv;

extern float32 VolDCCmdPU_gfv;
extern float32 ModIndex_gfv;

extern float32 LF100Gain_gfv;
extern float32 LF10Gain_gfv;
extern float32 LF1Gain_gfv;
extern float32 OmegaEstKp_gfv;
extern float32 OmegaEstKi_gfv;
extern float32 ThetaEstKi_gfv;
extern float32 CurDKp_gfv;
extern float32 CurDKi_gfv;
extern float32 CurQKp_gfv;
extern float32 CurQKi_gfv;
extern float32 VolDCKp_gfv;
extern float32 VolDCKi_gfv;

extern Uint16 VolSMMax_gusv;
extern Uint16 VolSMMin_gusv;
extern Uint16 StaSMChk_gusv;
extern Uint16 SciErrSampACnt_gusv;
extern Uint16 SciErrSampBCnt_gusv;
extern Uint16 SciErrSMCnt_gusv;
extern Uint16 StaErrSampACnt_gusv;
extern Uint16 StaErrSampBCnt_gusv;
extern Uint16 StaErrSMCnt_gusv;

#endif /* PARACALC_H_ */
