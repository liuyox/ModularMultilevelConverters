/*
 * DriveCtrl.c
 *
 *  Created on: 2016-7-22
 *      Author: Administrator
 */
#include "DSP2833x_Device.h"
#include "DSP2833x_Examples.h"
#include "DriveCtrl.h"
#include "ParaCalc.h"
#include "CurCtrl.h"
#include "Sample.h"
#include "FPGA.h"
#include "main.h"
#include "math.h"

volatile enum CMDMOD Cmd_PWMDrive_glv = CmdModNone;
volatile enum STATMOD Stat_PWMDrive_glv = StatModNone;
float32 ConvVolUDiffPU_gfv;
float32 ConvVolVDiffPU_gfv;
float32 ConvVolWDiffPU_gfv;
float32 ConvVolUComPU_gfv;
float32 ConvVolVComPU_gfv;
float32 ConvVolWComPU_gfv;

float32 ConvVolUPPU_gfv;
float32 ConvVolUNPU_gfv;
float32 ConvVolVPPU_gfv;
float32 ConvVolVNPU_gfv;
float32 ConvVolWPPU_gfv;
float32 ConvVolWNPU_gfv;

union SMNR_REG UPSMNR;
union SMNR_REG UNSMNR;
union SMNR_REG VPSMNR;
union SMNR_REG VNSMNR;
union SMNR_REG WPSMNR;
union SMNR_REG WNSMNR;

Uint32 PWMDutyUP_gulv;
Uint32 PWMDutyUN_gulv;
Uint32 PWMDutyVP_gulv;
Uint32 PWMDutyVN_gulv;
Uint32 PWMDutyWP_gulv;
Uint32 PWMDutyWN_gulv;

Uint32 NumUP_gulv;
# if Debug == 1
Uint16 ModuDIR = 0; // 0 - positive voltage, 1- negative voltage
Uint16 INSMNUM = 1;
#endif
void DriveCtrl(void)
{
	if(Cmd_PWMDrive_glv == CmdModReset)
	{
		ConvVolUDiffPU_gfv = 0.0;
		ConvVolVDiffPU_gfv = 0.0;
		ConvVolWDiffPU_gfv = 0.0;

		ConvVolUComPU_gfv = 0.0;
		ConvVolVComPU_gfv = 0.0;
		ConvVolWComPU_gfv = 0.0;
		Stat_PWMDrive_glv = StatModReset;
	}
	else
	{
		if(Cmd_PWMDrive_glv == CmdModStart)
		{
			if(Stat_SeqJudge_glv == StatModRise)
			{
				ConvVolUDiffPU_gfv = ConvVolADiffPU_gfv;
				ConvVolVDiffPU_gfv = ConvVolBDiffPU_gfv;
				ConvVolWDiffPU_gfv = ConvVolCDiffPU_gfv;

				ConvVolUComPU_gfv = ConvVolAComPU_gfv;
				ConvVolVComPU_gfv = ConvVolCComPU_gfv;
				ConvVolWComPU_gfv = ConvVolBComPU_gfv;
			}
			else if(Stat_SeqJudge_glv == StatModFall)
			{
				ConvVolUDiffPU_gfv = ConvVolADiffPU_gfv;
				ConvVolVDiffPU_gfv = ConvVolCDiffPU_gfv;
				ConvVolWDiffPU_gfv = ConvVolBDiffPU_gfv;

				ConvVolUComPU_gfv = ConvVolAComPU_gfv;
				ConvVolVComPU_gfv = ConvVolBComPU_gfv;
				ConvVolWComPU_gfv = ConvVolCComPU_gfv;
			}

			if(ConvVolUDiffPU_gfv > 0.5 * MaxModuIndex_gfv * VolDCNormPU_gfv)
			{
				ConvVolUDiffPU_gfv = 0.5 * MaxModuIndex_gfv * VolDCNormPU_gfv;
			}
			else if(ConvVolUDiffPU_gfv < -0.5 * MaxModuIndex_gfv * VolDCNormPU_gfv)
			{
				ConvVolUDiffPU_gfv = -0.5 * MaxModuIndex_gfv * VolDCNormPU_gfv;
			}

			if(ConvVolVDiffPU_gfv > 0.5 * MaxModuIndex_gfv * VolDCNormPU_gfv)
			{
				ConvVolVDiffPU_gfv = 0.5 * MaxModuIndex_gfv * VolDCNormPU_gfv;
			}
			else if(ConvVolVDiffPU_gfv < -0.5 * MaxModuIndex_gfv * VolDCNormPU_gfv)
			{
				ConvVolVDiffPU_gfv = -0.5 * MaxModuIndex_gfv * VolDCNormPU_gfv;
			}

			if(ConvVolWDiffPU_gfv > 0.5 * MaxModuIndex_gfv * VolDCNormPU_gfv)
			{
				ConvVolWDiffPU_gfv = 0.5 * MaxModuIndex_gfv * VolDCNormPU_gfv;
			}
			else if(ConvVolWDiffPU_gfv < -0.5 * MaxModuIndex_gfv * VolDCNormPU_gfv)
			{
				ConvVolWDiffPU_gfv = -0.5 * MaxModuIndex_gfv * VolDCNormPU_gfv;
			}
		}
		else if(Cmd_PWMDrive_glv == CmdModStop)
		{
			ConvVolUDiffPU_gfv = 0.0;
			ConvVolVDiffPU_gfv = 0.0;
			ConvVolWDiffPU_gfv = 0.0;
		}
		//生成桥臂电压
		ConvVolUPPU_gfv = -ConvVolUDiffPU_gfv + 0.5*ConvVolUComPU_gfv + 0.5*VolDCNormPU_gfv;
		ConvVolUNPU_gfv = ConvVolUDiffPU_gfv + 0.5*ConvVolUComPU_gfv + 0.5*VolDCNormPU_gfv;
		ConvVolVPPU_gfv = -ConvVolVDiffPU_gfv + 0.5*ConvVolVComPU_gfv + 0.5*VolDCNormPU_gfv;
		ConvVolVNPU_gfv = ConvVolVDiffPU_gfv + 0.5*ConvVolVComPU_gfv + 0.5*VolDCNormPU_gfv;
		ConvVolWPPU_gfv = -ConvVolWDiffPU_gfv + 0.5*ConvVolWComPU_gfv + 0.5*VolDCNormPU_gfv;
		ConvVolWNPU_gfv = ConvVolWDiffPU_gfv + 0.5*ConvVolWComPU_gfv + 0.5*VolDCNormPU_gfv;
		//生成桥臂的电流方向
		if(Stat_Isr_glv == StatIsrRun)
		{
			UPSMNR.bit.CURDIR = (IPA_gfv > 0);
			UNSMNR.bit.CURDIR = (IAN_gfv > 0);
			VPSMNR.bit.CURDIR = (IPB_gfv > 0);
			VNSMNR.bit.CURDIR = (IBN_gfv > 0);
			WPSMNR.bit.CURDIR = (IPC_gfv > 0);
			WNSMNR.bit.CURDIR = (ICN_gfv > 0);
		}
		else  //充电时，电流方向默认为正
		{
			UPSMNR.bit.CURDIR = 1;
			UNSMNR.bit.CURDIR = 1;
			VPSMNR.bit.CURDIR = 1;
			VNSMNR.bit.CURDIR = 1;
			WPSMNR.bit.CURDIR = 1;
			WNSMNR.bit.CURDIR = 1;
		}
		//写子模块轮换数
		UPSMNR.bit.CycleNUM = 0;
		UNSMNR.bit.CycleNUM = 0;
		VPSMNR.bit.CycleNUM = 0;
		VNSMNR.bit.CycleNUM = 0;
		WPSMNR.bit.CycleNUM = 0;
		WNSMNR.bit.CycleNUM = 0;
		//写调制波正负
#if Debug == 1
		UPSMNR.bit.MODUDIR = ModuDIR;
		UNSMNR.bit.MODUDIR = ModuDIR;
		VPSMNR.bit.MODUDIR = ModuDIR;
		VNSMNR.bit.MODUDIR = ModuDIR;
		WPSMNR.bit.MODUDIR = ModuDIR;
		WNSMNR.bit.MODUDIR = ModuDIR;
#else
		UPSMNR.bit.MODUDIR = (ConvVolUPPU_gfv < 0);
		UNSMNR.bit.MODUDIR = (ConvVolUNPU_gfv < 0);
		VPSMNR.bit.MODUDIR = (ConvVolVPPU_gfv < 0);
		VNSMNR.bit.MODUDIR = (ConvVolVNPU_gfv < 0);
		WPSMNR.bit.MODUDIR = (ConvVolWPPU_gfv < 0);
		WNSMNR.bit.MODUDIR = (ConvVolWNPU_gfv < 0);
#endif
#if FPGA_MODULMOD==0  //0-NLM, 1-DP-PWM
		//求子模块数
		UPSMNR.bit.SMINNUM = (Uint32) (fabs(ConvVolUPPU_gfv * INVVolSMNormPU_gfv + 0.5*(ConvVolUPPU_gfv > 0) - 0.5*(ConvVolUPPU_gfv < 0)));
		UNSMNR.bit.SMINNUM = (Uint32) (fabs(ConvVolUNPU_gfv * INVVolSMNormPU_gfv + 0.5*(ConvVolUNPU_gfv > 0) - 0.5*(ConvVolUNPU_gfv < 0)));
		VPSMNR.bit.SMINNUM = (Uint32) (fabs(ConvVolVPPU_gfv * INVVolSMNormPU_gfv + 0.5*(ConvVolVPPU_gfv > 0) - 0.5*(ConvVolVPPU_gfv < 0)));
		VNSMNR.bit.SMINNUM = (Uint32) (fabs(ConvVolVNPU_gfv * INVVolSMNormPU_gfv + 0.5*(ConvVolVNPU_gfv > 0) - 0.5*(ConvVolVNPU_gfv < 0)));
		WPSMNR.bit.SMINNUM = (Uint32) (fabs(ConvVolWPPU_gfv * INVVolSMNormPU_gfv + 0.5*(ConvVolWPPU_gfv > 0) - 0.5*(ConvVolWPPU_gfv < 0)));
		WNSMNR.bit.SMINNUM = (Uint32) (fabs(ConvVolWNPU_gfv * INVVolSMNormPU_gfv + 0.5*(ConvVolWNPU_gfv > 0) - 0.5*(ConvVolWNPU_gfv < 0)));
		NumUP_gulv = UPSMNR.bit.SMINNUM; // For debug use
#if Debug == 1
		UPSMNR.bit.SMINNUM = INSMNUM;
#endif

#else
		PWMDutyUP_gulv = (Uint32)((fabs(ConvVolUPPU_gfv)) * PWMDutyIndex_gulv);
		PWMDutyVP_gulv = (Uint32)((fabs(ConvVolVPPU_gfv)) * PWMDutyIndex_gulv);
		PWMDutyWP_gulv = (Uint32)((fabs(ConvVolWPPU_gfv)) * PWMDutyIndex_gulv);
		PWMDutyUN_gulv = (Uint32)((fabs(ConvVolUNPU_gfv)) * PWMDutyIndex_gulv);
		PWMDutyVN_gulv = (Uint32)((fabs(ConvVolVNPU_gfv)) * PWMDutyIndex_gulv);
		PWMDutyWN_gulv = (Uint32)((fabs(ConvVolWNPU_gfv)) * PWMDutyIndex_gulv);
		//写低16位比较寄存器
		UPSortRegs.CMPLR = (Uint16)PWMDutyUP_gulv;
		UNSortRegs.CMPLR = (Uint16)PWMDutyUN_gulv;
		VPSortRegs.CMPLR = (Uint16)PWMDutyVP_gulv;
		VNSortRegs.CMPLR = (Uint16)PWMDutyVN_gulv;
		WPSortRegs.CMPLR = (Uint16)PWMDutyWP_gulv;
		WNSortRegs.CMPLR = (Uint16)PWMDutyWN_gulv;
		//写高16位比较寄存器
		UPSortRegs.CMPHR = (Uint16)(PWMDutyUP_gulv >> 16);
		UNSortRegs.CMPHR = (Uint16)(PWMDutyUN_gulv >> 16);
		VPSortRegs.CMPHR = (Uint16)(PWMDutyVP_gulv >> 16);
		VNSortRegs.CMPHR = (Uint16)(PWMDutyVN_gulv >> 16);
		WPSortRegs.CMPHR = (Uint16)(PWMDutyWP_gulv >> 16);
		WNSortRegs.CMPHR = (Uint16)(PWMDutyWN_gulv >> 16);
#endif
		//写FPGA寄存器
		if(UPSortRegs.MSR.bit.SORTFIN & UNSortRegs.MSR.bit.SORTFIN & VPSortRegs.MSR.bit.SORTFIN &\
				VNSortRegs.MSR.bit.SORTFIN & WPSortRegs.MSR.bit.SORTFIN & WNSortRegs.MSR.bit.SORTFIN)
		{
			UPSortRegs.SMNR.all = UPSMNR.all;
			UNSortRegs.SMNR.all = UNSMNR.all;
			VPSortRegs.SMNR.all = VPSMNR.all;
			VNSortRegs.SMNR.all = VNSMNR.all;
			WPSortRegs.SMNR.all = WPSMNR.all;
			WNSortRegs.SMNR.all = WNSMNR.all;
		}
		else
		{
			Stat_PWMDrive_glv = StatModError;
		}
#if Debug == 1
		UPSortRegs.SMNR.all = UPSMNR.all;
#endif
	}
}

#pragma CODE_SECTION(DriveCtrl,"ramfuncs");
