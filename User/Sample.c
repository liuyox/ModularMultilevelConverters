/*
 * Sample.c
 *
 *  Created on: 2016-7-19
 *      Author: Administrator
 */


#include "DSP2833x_Device.h"
#include "DSP2833x_Examples.h"
#include "ParaCalc.h"
#include "Sample.h"
#include "FPGA.h"

//==========SCI Data Inquire==========
// Sample Cabinet A
int16 IPAADC_glv;
int16 IPBADC_glv;
int16 IPCADC_glv;
int16 INAADC_glv;
int16 INBADC_glv;
int16 INCADC_glv;
int16 VUVADC_glv;
int16 VVWADC_glv;
int16 VWUADC_glv;
int16 IVDCADC_glv;
int16 VDCRADC_glv;
// Sample Cabinet B
int16 VUVGADC_glv;
int16 VVWGADC_glv;
int16 VWUGADC_glv;
int16 IVDCADC_glv;
int16 VPAADC_glv;
int16 VPBADC_glv;
int16 VPCADC_glv;
int16 VANADC_glv;
int16 VBNADC_glv;
int16 VCNADC_glv;
int16 VDCADC_glv;
// Voltage of SMs
int16 UPVSMADC_glv[24];
int16 UNVSMADC_glv[24];
int16 VPVSMADC_glv[24];
int16 VNVSMADC_glv[24];
int16 WPVSMADC_glv[24];
int16 WNVSMADC_glv[24];
//Temperature of SMs
int16 TemSMAP_glv[24];
int16 TemSMAN_glv[24];
int16 TemSMBP_glv[24];
int16 TemSMBN_glv[24];
int16 TemSMCP_glv[24];
int16 TemSMCN_glv[24];
//Err State of SMs
int16 ErrSMAP_glv[12];
int16 ErrSMAN_glv[12];
int16 ErrSMBP_glv[12];
int16 ErrSMBN_glv[12];
int16 ErrSMCP_glv[12];
int16 ErrSMCN_glv[12];

Uint16 SMUpdateFlag_gusv;

//states of SMs
int16 VSMRDY_glv;
int16 VSMSCIERR_glv;
int16 VSMSTAERR_glv;


volatile union DRSTR_REG  DRSTRARegsShadow;
volatile union DRSTR_REG  DRSTRBRegsShadow;


int16 ADCOffs_glv = 2048;

//==========End of Conversion Inquire==========

//==========Real Sample Value Calculate==========
//Real Sample Value of Cabinet A
float32 IPA_gfv;
float32 IPB_gfv;
float32 IPC_gfv;
float32 IAN_gfv;
float32 IBN_gfv;
float32 ICN_gfv;
float32 VUV_gfv;
float32 VVW_gfv;
float32 VWU_gfv;
float32 IVDC_gfv;
float32 VDCR_gfv;
//Real Sample Value of Cabinet B
float32 VUVG_gfv;
float32 VVWG_gfv;
float32 VWUG_gfv;
float32 VPA_gfv;
float32 VPB_gfv;
float32 VPC_gfv;
float32 VAN_gfv;
float32 VBN_gfv;
float32 VCN_gfv;
float32 VDC_gfv;
//calculation
float32 VolU_gfv;  //Transformer voltage at inverter side
float32 VolV_gfv;
float32 VolW_gfv;
float32 CurUDiff_gfv;  //differential current
float32 CurVDiff_gfv;
float32 CurWDiff_gfv;
float32 CurUCom_gfv;  //common mode current
float32 CurVCom_gfv;
float32 CurWCom_gfv;
//Real Sample Value of SM
float32 UPVSM_gfv[24];
float32 UNVSM_gfv[24];
float32 VPVSM_gfv[24];
float32 VNVSM_gfv[24];
float32 WPVSM_gfv[24];
float32 WNVSM_gfv[24];

Uint32 UPTSM_glv[24];
Uint32 UNTSM_glv[24];
Uint32 VPTSM_glv[24];
Uint32 VNTSM_glv[24];
Uint32 WPTSM_glv[24];
Uint32 WNTSM_glv[24];
//==========End of Real Sample Value Calculate==========

//==========Perunit Sample Value Calculate==========
//Pu Sample Value of Cabinet A
float32 IAPPU_gfv;
float32 IBPPU_gfv;
float32 ICPPU_gfv;
float32 IANPU_gfv;
float32 IBNPU_gfv;
float32 ICNPU_gfv;
float32 VUVPU_gfv;
float32 VVWPU_gfv;
float32 VWUPU_gfv;
float32 IVDCPU_gfv;
float32 VDCRPU_gfv;
//Pu Sample Value of Cabinet B
float32 VUVGPU_gfv;
float32 VVWGPU_gfv;
float32 VWUGPU_gfv;
float32 IVDCPU_gfv;
float32 VPAPU_gfv;
float32 VPBPU_gfv;
float32 VPCPU_gfv;
float32 VANPU_gfv;
float32 VBNPU_gfv;
float32 VCNPU_gfv;
float32 VDCPU_gfv;
//===============End Perunit Sample=================

//For Debug use
Uint16 	SCIErrUPCnt =0;
Uint16 	SCIErrUNCnt =0;
Uint16 	SCIErrVPCnt =0;
Uint16 	SCIErrVNCnt =0;
Uint16 	SCIErrWPCnt =0;
Uint16 	SCIErrWNCnt =0;

//float32 VUV_gfvDe[200];
//float32 VVW_gfvDe[200];
//float32 VWU_gfvDe[200];
//float32 VUVG_gfvDe[200];
//float32 VVWG_gfvDe[200];
//float32 VWUG_gfvDe[200];
//int16 debugi = 0;

void AdcInqr(void)
{
	//Sample A Error Check
	DRSTRARegsShadow.all = SampARegs.DRSTR.all;
	//Sample A 数据收齐 & 串行通信检查无误 & 子模块状态无误时采样数据有效，任意一条不满足则关闭换流器
	if((DRSTRARegsShadow.bit.DR == 1) && (DRSTRARegsShadow.bit.DERR == 0))
	{
		IPAADC_glv = SampARegs.IPA - ADCOffs_glv - 25;		//bias for 25L station
		IPBADC_glv = SampARegs.IPB - ADCOffs_glv + 4;
		IPCADC_glv = SampARegs.IPC - ADCOffs_glv - 6;
		INAADC_glv = SampARegs.INA - ADCOffs_glv - 5;
		INBADC_glv = SampARegs.INB - ADCOffs_glv - 7;
		INCADC_glv = SampARegs.INC - ADCOffs_glv - 3;
		VUVADC_glv = SampARegs.VUV - ADCOffs_glv - 31;
		VVWADC_glv = SampARegs.VVW - ADCOffs_glv - 0;
		VWUADC_glv = SampARegs.VWU - ADCOffs_glv + 2;
		//IVDCADC_glv = SampARegs.IVDC - ADCOffs_glv;
		VDCRADC_glv = SampARegs.VDCR - 22;
		//采样箱自身产生错误
		if(DRSTRARegsShadow.bit.STATE == 6)
		{
			StaErrSampACnt_gusv++;
		}
	}
	else
	{
		//串行通信产生错误
		SciErrSampACnt_gusv++;
		//UPSortRegs.MCR.bit.SCIReset=1;
	}
	//Sample B Error Check
	DRSTRBRegsShadow.all = SampBRegs.DRSTR.all;
	//Sample B 数据收齐 & 串行通信检查无误 & 子模块状态无误时采样数据有效，任意一条不满足则关闭换流器
	if((DRSTRBRegsShadow.bit.DR == 1) && (DRSTRBRegsShadow.bit.DERR == 0))
	{
		VUVGADC_glv = SampBRegs.VUVG - ADCOffs_glv + 12;		//bias for 25L station
		VVWGADC_glv = SampBRegs.VVWG - ADCOffs_glv - 14;
		VWUGADC_glv = SampBRegs.VWUG - ADCOffs_glv + 9;
		IVDCADC_glv = SampBRegs.IVDC - 10; //- ADCOffs_glv + 27; 		//not same with other station
		VPAADC_glv = SampBRegs.VPA - 15;
		VPBADC_glv = SampBRegs.VPB - 40;
		VPCADC_glv = SampBRegs.VPC - 28;
		VANADC_glv = SampBRegs.VAN - 35;
		VBNADC_glv = SampBRegs.VBN - 18;
		VCNADC_glv = SampBRegs.VCN - 1;
		VDCADC_glv = SampBRegs.VDC - 15;
		//采样箱自身发生错误
		if(DRSTRARegsShadow.bit.STATE == 6)
		{
			StaErrSampBCnt_gusv++;
		}

	}
	else
	{
		//串行通信产生错误
		SciErrSampBCnt_gusv++;
		//UPSortRegs.MCR.bit.SCIReset=1;
	}
	//==================For Debug Use===================
	UPARMSTRegsShadow.DRSR = UPARMSTRegs.DRSR;
	if (UPARMSTRegsShadow.DRSR != StaSMChk_gusv)
		SCIErrUPCnt++;
	UNARMSTRegsShadow.DRSR = UNARMSTRegs.DRSR;
	if (UNARMSTRegsShadow.DRSR != StaSMChk_gusv)
		SCIErrUNCnt++;
	VPARMSTRegsShadow.DRSR = VPARMSTRegs.DRSR;
	if (VPARMSTRegsShadow.DRSR != StaSMChk_gusv)
		SCIErrVPCnt++;
	VNARMSTRegsShadow.DRSR = VNARMSTRegs.DRSR;
	if (VNARMSTRegsShadow.DRSR != StaSMChk_gusv)
		SCIErrVNCnt++;
	WPARMSTRegsShadow.DRSR = WPARMSTRegs.DRSR;
	if (WPARMSTRegsShadow.DRSR != StaSMChk_gusv)
		SCIErrWPCnt++;
	WNARMSTRegsShadow.DRSR = WNARMSTRegs.DRSR;
	if (WNARMSTRegsShadow.DRSR != StaSMChk_gusv)
		SCIErrWNCnt++;
	VSMRDY_glv = UPARMSTRegsShadow.DRSR & UNARMSTRegsShadow.DRSR & VPARMSTRegsShadow.DRSR &\
			VNARMSTRegsShadow.DRSR & WPARMSTRegsShadow.DRSR & WNARMSTRegsShadow.DRSR;
	//==================================================
	//Note: Register DRSR can only be read once in each cycle.
//	VSMRDY_glv = UPARMSTRegs.DRSR & UNARMSTRegs.DRSR & VPARMSTRegs.DRSR &\
//	             VNARMSTRegs.DRSR & WPARMSTRegs.DRSR & WNARMSTRegs.DRSR;
	VSMSCIERR_glv = UPARMSTRegs.DRER | UNARMSTRegs.DRER | VPARMSTRegs.DRER |\
            	    VNARMSTRegs.DRER | WPARMSTRegs.DRER | WNARMSTRegs.DRER;
	VSMSTAERR_glv = UPARMSTRegs.SMSR | UNARMSTRegs.SMSR | VPARMSTRegs.SMSR |\
    	    		VNARMSTRegs.SMSR | WPARMSTRegs.SMSR | WNARMSTRegs.SMSR;
	//三相六桥臂SM 数据收齐 & 串行通信检查无误 & 子模块不在故障状态时采样数据有效，任意一条不满足则关闭换流器
	if((VSMRDY_glv == StaSMChk_gusv) && (VSMSCIERR_glv == 0x00))
	{
		//增加子模块状态判断及子模块数据上传（盛景）
		//...
		//子模块数据更新
		SMDataUpdate();

		SMUpdateFlag_gusv = 1;
		if(VSMSTAERR_glv != 0x00)
		{
			StaErrSMCnt_gusv++;
		}
	}
	else
	{
		//或者直接关掉换流器
		SciErrSMCnt_gusv++;
	}

}

void RealInqr(void)
{
	//Sample Cabinet A
	IPA_gfv = IPACoef_gfv * IPAADC_glv;
	IPB_gfv = IPBCoef_gfv * IPBADC_glv;
	IPC_gfv = IPCCoef_gfv * IPCADC_glv;
	IAN_gfv = -IANCoef_gfv * INAADC_glv;
	IBN_gfv = -IBNCoef_gfv * INBADC_glv;
	ICN_gfv = -ICNCoef_gfv * INCADC_glv;
	VUV_gfv = VUVCoef_gfv * VUVADC_glv;
	VVW_gfv = VVWCoef_gfv * VVWADC_glv;
	VWU_gfv = VWUCoef_gfv * VWUADC_glv;
	//IVDC_gfv = IVDCCoef_gfv * IVDCADC_glv;
	VDCR_gfv = VDCRCoef_gfv * VDCRADC_glv;
	//Sample Cabinet B
	VUVG_gfv = VUVGCoef_gfv * VUVGADC_glv;
	VVWG_gfv = VVWGCoef_gfv * VVWGADC_glv;
	VWUG_gfv = VWUGCoef_gfv * VWUGADC_glv;
	IVDC_gfv = IVDCCoef_gfv * IVDCADC_glv;
	VPA_gfv = VPACoef_gfv * VPAADC_glv;
	VPB_gfv = VPBCoef_gfv * VPBADC_glv;
	VPC_gfv = VPCCoef_gfv * VPCADC_glv;
	VAN_gfv = VANCoef_gfv * VANADC_glv;
	VBN_gfv = VBNCoef_gfv * VBNADC_glv;
	VCN_gfv = VCNCoef_gfv * VCNADC_glv;
	VDC_gfv = VDCCoef_gfv * VDCADC_glv;

}

void PUInqr(void)
{
	IAPPU_gfv = IPA_gfv * CBaseCoef_gfv;
	IBPPU_gfv = IPB_gfv * CBaseCoef_gfv;
	ICPPU_gfv = IPC_gfv * CBaseCoef_gfv;
	IANPU_gfv = IAN_gfv * CBaseCoef_gfv;
	IBNPU_gfv = IBN_gfv * CBaseCoef_gfv;
	ICNPU_gfv = ICN_gfv * CBaseCoef_gfv;
	VUVPU_gfv = VUV_gfv * VBaseCoef_gfv;
	VVWPU_gfv = VVW_gfv * VBaseCoef_gfv;
	VWUPU_gfv = VWU_gfv * VBaseCoef_gfv;
	IVDCPU_gfv = IVDC_gfv * CBaseCoef_gfv;
	VDCRPU_gfv = VDCR_gfv * VBaseCoef_gfv;
	//Sample Cabinet B
	VUVGPU_gfv = VUVG_gfv * VBaseCoef_gfv;
	VVWGPU_gfv = VVWG_gfv * VBaseCoef_gfv;
	VWUGPU_gfv = VWUG_gfv * VBaseCoef_gfv;
//	IVDCPU_gfv = IVDC_gfv * CBaseCoef_gfv;
	VPAPU_gfv = VPA_gfv * VBaseCoef_gfv;
	VPBPU_gfv = VPB_gfv * VBaseCoef_gfv;
	VPCPU_gfv = VPC_gfv * VBaseCoef_gfv;
	VANPU_gfv = VAN_gfv * VBaseCoef_gfv;
	VBNPU_gfv = VBN_gfv * VBaseCoef_gfv;
	VCNPU_gfv = VCN_gfv * VBaseCoef_gfv;
	VDCPU_gfv = VDC_gfv * VBaseCoef_gfv;
}


void SMDataUpdate(void)
{
	int16 i;
	if(SMUpdateFlag_gusv)
	{
		SMUpdateFlag_gusv = 0;
		for(i=0; i< 12; i++)
		{
			//子模块电压
			UPVSMADC_glv[2*i] = *(Addr_offset + UPSMVPRAddrStart + 8*i);
			UPVSMADC_glv[2*i+1] = *(Addr_offset + UPSMVNRAddrStart + 8*i);
			UNVSMADC_glv[2*i] = *(Addr_offset + UNSMVPRAddrStart + 8*i);
			UNVSMADC_glv[2*i+1] = *(Addr_offset + UNSMVNRAddrStart + 8*i);

			VPVSMADC_glv[2*i] = *(Addr_offset + VPSMVPRAddrStart + 8*i);
			VPVSMADC_glv[2*i+1] = *(Addr_offset + VPSMVNRAddrStart + 8*i);
			VNVSMADC_glv[2*i] = *(Addr_offset + VNSMVPRAddrStart + 8*i);
			VNVSMADC_glv[2*i+1] = *(Addr_offset + VNSMVNRAddrStart + 8*i);

			WPVSMADC_glv[2*i] = *(Addr_offset + WPSMVPRAddrStart + 8*i);
			WPVSMADC_glv[2*i+1] = *(Addr_offset + WPSMVNRAddrStart + 8*i);
			WNVSMADC_glv[2*i] = *(Addr_offset + WNSMVPRAddrStart + 8*i);
			WNVSMADC_glv[2*i+1] = *(Addr_offset + WNSMVNRAddrStart + 8*i);

			//子模块温度
//			UPTSM_glv[2*i] = *(Addr_offset + UPSMTMRAddrStart + 8*i);UPSMCURAddrStart
//			UPTSM_glv[2*i + 1] = (UPTSM_glv[2*i] >> 8);
//			UPTSM_glv[2*i] = UPTSM_glv[2*i] & 0X00FF;
//			UNTSM_glv[2*i] = *(Addr_offset + UNSMTMRAddrStart + 8*i);
//			UNTSM_glv[2*i + 1] = (UNTSM_glv[2*i] >> 8);
//			UNTSM_glv[2*i] = UNTSM_glv[2*i] & 0X00FF;
//			UPTSM_glv[i] = *(Addr_offset + UPSMSTRAddrStart + 8*i);
//			UNTSM_glv[i] = *(Addr_offset + UNSMSTRAddrStart + 8*i);
//			VPTSM_glv[i] = *(Addr_offset + VPSMSTRAddrStart + 8*i);
//			VNTSM_glv[i] = *(Addr_offset + VNSMSTRAddrStart + 8*i);
//			WPTSM_glv[i] = *(Addr_offset + WPSMSTRAddrStart + 8*i);
//			WNTSM_glv[i] = *(Addr_offset + WNSMSTRAddrStart + 8*i);

			//子模块错误状态
//			ErrSMAP_glv[i] = *(Addr_offset + UPSMSTRAddrStart + 8*i);
//			ErrSMAN_glv[i] = *(Addr_offset + UNSMSTRAddrStart + 8*i);
//
//			ErrSMBP_glv[i] = *(Addr_offset + VPSMSTRAddrStart + 8*i);
//			ErrSMBN_glv[i] = *(Addr_offset + VNSMSTRAddrStart + 8*i);
//
//			ErrSMCP_glv[i] = *(Addr_offset + WPSMSTRAddrStart + 8*i);
//			ErrSMCN_glv[i] = *(Addr_offset + WNSMSTRAddrStart + 8*i);
		}
		for(i=0; i< 24; i++)
		{
			UPVSM_gfv[i] = VSMCoef_gfv * UPVSMADC_glv[i];
			UNVSM_gfv[i] = VSMCoef_gfv * UNVSMADC_glv[i];
			VPVSM_gfv[i] = VSMCoef_gfv * VPVSMADC_glv[i];
			VNVSM_gfv[i] = VSMCoef_gfv * VNVSMADC_glv[i];
			WPVSM_gfv[i] = VSMCoef_gfv * WPVSMADC_glv[i];
			WNVSM_gfv[i] = VSMCoef_gfv * WNVSMADC_glv[i];
		}
		VNTSM_glv[0] = (*(Addr_offset + VNSMTMRAddrStart + 8*0)) & (0X00FF);		//BN1子模块开路状态标志位
		WNTSM_glv[0] = (*(Addr_offset + WNSMTMRAddrStart + 8*0)) & (0X00FF);		//CN1子模块开路状态标志位
//		WNTSM_glv[4] = (*(Addr_offset + WNSMTMRAddrStart + 8*2)) & (0X00FF);		//CN3子模块开路状态标志位
	}
}

#pragma CODE_SECTION(AdcInqr,"ramfuncs");
#pragma CODE_SECTION(RealInqr,"ramfuncs");
#pragma CODE_SECTION(PUInqr,"ramfuncs");
#pragma CODE_SECTION(SMDataUpdate,"ramfuncs");
//==========End of Perunit Sample Value Calculate==========
