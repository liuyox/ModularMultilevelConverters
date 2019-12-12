/*
 * Sample.h
 *
 *  Created on: 2016-7-19
 *      Author: Administrator
 */

#ifndef SAMPLE_H_
#define SAMPLE_H_

extern void AdcInqr(void);
extern void SMDataUpdate(void);
extern void RealInqr(void);
extern void PUInqr(void);

//==========SCI Data Inquire==========
// Sample Cabinet A
extern int16 IPAADC_glv;
extern int16 IPBADC_glv;
extern int16 IPCADC_glv;
extern int16 INAADC_glv;
extern int16 INBADC_glv;
extern int16 INCADC_glv;
extern int16 VUVADC_glv;
extern int16 VVWADC_glv;
extern int16 VWUADC_glv;
extern int16 IVDCADC_glv;
extern int16 VDCRADC_glv;
// Sample Cabinet B
extern int16 VUVGADC_glv;
extern int16 VVWGADC_glv;
extern int16 VWUGADC_glv;
extern int16 IVDCADC_glv;
extern int16 VPAADC_glv;
extern int16 VPBADC_glv;
extern int16 VPCADC_glv;
extern int16 VANADC_glv;
extern int16 VBNADC_glv;
extern int16 VCNADC_glv;
extern int16 VDCADC_glv;
extern float32 VolU_gfv;  //Transformer voltage at inverter side
extern float32 VolV_gfv;
extern float32 VolW_gfv;
extern float32 CurUDiff_gfv;  //differential current
extern float32 CurVDiff_gfv;
extern float32 CurWDiff_gfv;
extern float32 CurUCom_gfv;  //common mode current
extern float32 CurVCom_gfv;
extern float32 CurWCom_gfv;
//Voltage of SMs
extern int16 UPVSMADC_glv[24];
extern int16 UNVSMADC_glv[24];
extern int16 VPVSMADC_glv[24];
extern int16 VNVSMADC_glv[24];
extern int16 WPVSMADC_glv[24];
extern int16 WNVSMADC_glv[24];
//Temperature of SMs
extern int16 TemSMAP_glv[24];
extern int16 TemSMAN_glv[24];
extern int16 TemSMBP_glv[24];
extern int16 TemSMBN_glv[24];
extern int16 TemSMCP_glv[24];
extern int16 TemSMCN_glv[24];
//Err State of SMs
extern int16 ErrSMAP_glv[12];
extern int16 ErrSMAN_glv[12];
extern int16 ErrSMBP_glv[12];
extern int16 ErrSMBN_glv[12];
extern int16 ErrSMCP_glv[12];
extern int16 ErrSMCN_glv[12];
//states of SMs
extern int16 VSMRDY_glv;
extern int16 VSMSCIERR_glv;
extern int16 VSMSTAERR_glv;
extern Uint16 SciErrSampACnt_gusv;
extern Uint16 SciErrSampBCnt_gusv;
extern Uint16 SciErrSMCnt_gusv;
extern Uint16 StaErrSampACnt_gusv;
extern Uint16 StaErrSampBCnt_gusv;
extern Uint16 StaErrSMCnt_gusv;
extern int16 ADCOffs_glv;

extern volatile union DRSTR_REG  DRSTRARegsShadow;
extern volatile union DRSTR_REG  DRSTRBRegsShadow;
//==========End of Data Inquire==========

//==========Real Sample Value Calculate==========
//Real Sample Value of Cabinet A
extern float32 IPA_gfv;
extern float32 IPB_gfv;
extern float32 IPC_gfv;
extern float32 IAN_gfv;
extern float32 IBN_gfv;
extern float32 ICN_gfv;
extern float32 VUV_gfv;
extern float32 VVW_gfv;
extern float32 VWU_gfv;
extern float32 IVDC_gfv;
extern float32 VDCR_gfv;
//Real Sample Value of Cabinet B
extern float32 VUVG_gfv;
extern float32 VVWG_gfv;
extern float32 VWUG_gfv;
extern float32 VPA_gfv;
extern float32 VPB_gfv;
extern float32 VPC_gfv;
extern float32 VAN_gfv;
extern float32 VBN_gfv;
extern float32 VCN_gfv;
extern float32 VDC_gfv;
//Real Sample Value of SM
extern float32 UPVSM_gfv[24];
extern float32 UNVSM_gfv[24];
extern float32 VPVSM_gfv[24];
extern float32 VNVSM_gfv[24];
extern float32 WPVSM_gfv[24];
extern float32 WNVSM_gfv[24];

//Temperature of SMs
extern Uint32 UPTSM_glv[24];
extern Uint32 UNTSM_glv[24];
extern Uint32 VPTSM_glv[24];
extern Uint32 VNTSM_glv[24];
extern Uint32 WPTSM_glv[24];
extern Uint32 WNTSM_glv[24];

//extern int16 TemSMAP_glv[24];
//extern int16 TemSMAN_glv[24];
//extern int16 TemSMBP_glv[24];
//extern int16 TemSMBN_glv[24];
//extern int16 TemSMCP_glv[24];
//extern int16 TemSMCN_glv[24];

//Real Coefficient of Sample Cabinet A
extern float32 IPACoef_gfv;
extern float32 IPBCoef_gfv;
extern float32 IPCCoef_gfv;
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

extern float32 VSMCoef_gfv;
//==========End of Real Sample Value Calculate==========

//==========Perunit Sample Value Calculate==========
//Pu Sample Value of Cabinet A
extern float32 IAPPU_gfv;
extern float32 IBPPU_gfv;
extern float32 ICPPU_gfv;
extern float32 IANPU_gfv;
extern float32 IBNPU_gfv;
extern float32 ICNPU_gfv;
extern float32 VUVPU_gfv;
extern float32 VVWPU_gfv;
extern float32 VWUPU_gfv;
extern float32 IVDCPU_gfv;
extern float32 VDCRPU_gfv;
//Pu Sample Value of Cabinet B
extern float32 VUVGPU_gfv;
extern float32 VVWGPU_gfv;
extern float32 VWUGPU_gfv;
extern float32 IVDCPU_gfv;
extern float32 VPAPU_gfv;
extern float32 VPBPU_gfv;
extern float32 VPCPU_gfv;
extern float32 VANPU_gfv;
extern float32 VBNPU_gfv;
extern float32 VCNPU_gfv;
extern float32 VDCPU_gfv;

//extern float32 VBaseCoef_gfv;
//extern float32 CBaseCoef_gfv;
//==========End of Perunit Sample Value Calculate==========

//Debug
extern Uint16 	SCIErrUPCnt;
extern Uint16 	SCIErrUNCnt;
extern Uint16 	SCIErrVPCnt;
extern Uint16 	SCIErrVNCnt;
extern Uint16 	SCIErrWPCnt;
extern Uint16 	SCIErrWNCnt;

extern float32 VUV_gfvDe[200];
extern float32 VVW_gfvDe[200];
extern float32 VWU_gfvDe[200];
extern float32 VUVG_gfvDe[200];
extern float32 VVWG_gfvDe[200];
extern float32 VWUG_gfvDe[200];
extern int16 debugi;
#endif /* SAMPLE_H_ */
