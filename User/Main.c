// This is the control code for three-phase voltage source converter with bipolar DC voltage
/*-- Code Revision History :
-- --------------------------------------------------------------------
-- Ver: :| Author       :| Mod. Date   :|Changes Made:
-- V0.0 :| GuYunjie     :| @2014.05.25 :| First Release
-- V1.0 :| DYF          :| @2016.07.19 :| for MMC
-- V1.1 :| DYF          :| @2016.12.26 :| for MMC open-loop
-- V1.2 :| DYF          :| @2017.01.03 :| for MMC close-loop
-- V1.4 :| TJS			:| @2017.04.09 :| add CCS
-- V1.5 :| TJS          :| @2017.05.26 :| Debug the 9L MMC Platform
-- V1.6 :| TJS          :| @2017.08.03 :| Modify the SYNC Pattern of DSP and FPGA,SYNC at PRD of EPWM
-- V1.7 :| TJS 			:| @2017.12.06 :| Enable full-bridge output of SMs and also over-modulation for MMC
-- V1.0 :| SJ           :| @2017.12.09 :| add Ethernet based on above version
-- V1.1 :| SJ           :| @2018.03.15 :| sigle C for Ethernet and  TX/RX success
-- V1.1 :| SJ           :| @2018.03.19 :| sample SM voltage and send to Ethernet
-- V1.1 :| SJ           :| @2018.03.22 :| FB13L 输出 做子模块开路实验
-- V1.1 :| SJ           :| @2018.03.26 :| 全桥输出  6/12子模块做子模块开路实验  上位机正常收取数据
-- V1.1 :| SJ           :| @2018.04.18 :| 在0326基础上收取子模块开路状态标志位并发送至上位机
-- V1.1 :| LiuYongxia   :| @2018.05.24 :| 在0418基础上功率指令改变并将标志位发送至上位机（逆变实验）

-- ------------------------------------------------------------------*/
#include "DSP28x_Project.h"     // Device Headerfile and Examples Include File
#include "Vector.h"			    // DMC head files
#include "math.h"				// C standard head file
#include "Monitor.h"			// User defined head files
#include "IOOperate.h"			// User defined head files
#include "Algorithms.h"			// User defined head files
#include "Main.h"               // User defined head files
#include "Sample.h"             // User defined head files
#include "ParaCalc.h"           // User defined head files
#include "CurCtrl.h"
#include "FPGA.h"
#include "ChargeCtrl.h"
#include "CmdCtrl.h"
#include "DriveCtrl.h"
//head files for Ethernet communication
#include "Ethernet.h"
//////////////////////////////////////////////////////////////////////////////////
//Variables and Functions Maintained by Interrupt Service Routine Below
//////////////////////////////////////////////////////////////////////////////////

//==========Interrupt Service Routine Shared==========
volatile enum CMDISR Cmd_Isr_glv = CmdIsrNone;
volatile enum STATISR Stat_Isr_glv = StatIsrNone;
int32 Flag_PWMOn_glv = 0;
int32 IsrTimeStampIn_glv = 0;
int32 IsrTimeStampOut_glv = 0;
int32 IsrTimeStampMid_glv = 0;
int32 IsrTick_gfv = 0;
interrupt void ISREPwm1_PRD(void);

//***********Debug************//
int32 IsrTimeStampADC_glv = 0;
int32 IsrTimeStampTheta_glv = 0;
int32 IsrTimeStampCharge_glv = 0;
int32 IsrTimeStampCur_glv = 0;
int32 IsrTimeStampDrive_glv = 0;
//==========End of Interrupt Service Routine Shared==========


//==========Error Protection==========
void ErrPrt(void);
volatile enum STATERR Stat_ErrPrt_glv = StatErrNone;
volatile enum CMDMOD Cmd_ErrPrt_glv = CmdModNone;
//==========End of Error Protection==========

//////////////////////////////////////////////////////////////////////////////////
//Variables and Functions Maintained by Main Below
//////////////////////////////////////////////////////////////////////////////////

//==========Main Timer==========
int32 Flag_Timer0Ring_glv = 0;
int32 Cnt_Timer0Ring_glv = 0;
//==========End of Main Timer==========



//=====Monitor=================================
//volatile struct ECAN_REGS *pECanRegsa;
//volatile struct ECAN_MBOXES *pECanMboxesa;
//volatile struct LAM_REGS *pECanLAMRegsa;
//volatile struct ECAN_REGS *pECanRegsb;
//volatile struct ECAN_MBOXES *pECanMboxesb;
//volatile struct LAM_REGS *pECanLAMRegsb;
//void InitMonitorVar(void);
//void SelectEcana(void);
//void SelectEcanb(void);

float IPADebug = 0;
float IANDebug = 0;
float IPBDebug = 0;
float IBNDebug = 0;
float IPCDebug = 0;
float ICNDebug = 0;

Uint16 ReadAfterWrite = 0;
Uint16 DlyOn = 0;
Uint16 CostTimeOn = 0;

main(void)
{
	InitSysCtrl();

	DINT;
	IER=0x0000;
	IFR=0x0000;
	InitPieCtrl();
	InitPieVectTable();

	EALLOW;
	PieVectTable.EPWM1_INT = &ISREPwm1_PRD; // EPwm1 counter underflow interrupt
    EDIS;

	MemCopy(&RamfuncsLoadStart,&RamfuncsLoadEnd,&RamfuncsRunStart);//ram中间的函数

	InitGpio();
	InitEPwmGpio();
	InitFlash();

	//System variable calculation
    ParaCalc();
    IniLAN = Lan_Initialize();

	//Initial CPU timers
	InitCpuTimers();
   	ConfigCpuTimer(&CpuTimer0, 150, 2500);		//Timer0 for main time base,2.5ms
	ConfigCpuTimer(&CpuTimer1, 150, 10000);		//Timer1 for ISR time stamp
	StartCpuTimer0();
	StartCpuTimer1();


	InitFPGA();


	ConfigEPwm();


	EnableInterrupts();

	//Fisrt Time Reset of ISR
	Cmd_Isr_glv = CmdIsrReset;
	while(Stat_Isr_glv != StatIsrReset)
	{
		;
	}
	Cmd_Isr_glv = CmdIsrPrep;

	while(1)
	{
		;
		//子模块数据更新
//		SMDataUpdate();

		/*if (Send_Watch_Flag && Cmd_Watch && IniLAN)	//!
		{
			Send_Watch_Flag = 0;	//清发送使能，防止重复发送
			//设计以5KHz频率上传数据，上位机一副图绘制工频两个周期波形
			Send_Watch_Frame(FramNum, SMNum);//只需2860

			FramNum++;
			if (FramNum == 200) //帧编号0-199
			{
				FramNum = 0;
			}

			SMNum++;
			if(NumTotal_gulv == 24)
			{
				if (SMNum == 72) //子模块电容编号0-71
				{
					SMNum = 0;
				}
			}
			else if(NumTotal_gulv == 16)
			{
				if (SMNum == 48) //子模块电容编号0-47
				{
					SMNum = 0;
				}
			}
			else if(NumTotal_gulv == 8)
			{
				if (SMNum == 24) //子模块电容编号0-23
				{
					SMNum = 0;
				}
			}
		}
		else if (Send_Debug_Cnt && Cmd_Ctrl && IniLAN)
		{
//			IsrTimeStampIn_glv = (ReadCpuTimer1Period() - ReadCpuTimer1Counter());
			Send_Debug_Frame();		//151个数据耗时7300
			Send_Debug_Cnt = 0;
//			IsrTimeStampOut_glv = (ReadCpuTimer1Period() - ReadCpuTimer1Counter());
		}*/
	}
	
}


interrupt void ISREPwm1_PRD(void)
{
	//Timer Reload
	ReloadCpuTimer1();
	IsrTimeStampIn_glv = (ReadCpuTimer1Period() - ReadCpuTimer1Counter());
	//13电平全桥时，9188接收数据放前面，上电运行 FPGA的通信error counter就有？？？？、、、、、放最后则没有
//	if(IniLAN)
//	{
//		LAN911xGetFrame(RxEthnetFrameBuffer); //空判断需要250，ARP需要2800
//	}

	//Sample Key Signals
	AdcInqr(); //2704
	RealInqr(); //3089
	PUInqr(); //3408
	UITrans(); //3665
	// Entrance Time Stamp
//	IsrTimeStampIn_glv = (ReadCpuTimer1Period() - ReadCpuTimer1Counter()); //3670
	//Isr State Machine
	if(Cmd_Isr_glv == CmdIsrReset)
	{
		Flag_PWMOn_glv = 0;
//		IsrTimeStampIn_glv = 0;
//		IsrTimeStampOut_glv = 0;
		IsrTimeStampMid_glv = 0;
		IsrTick_gfv = 0;
//		MonitorPara[1] = 0.5;

		Cmd_SeqJudge_glv = CmdModReset;
		Cmd_SeqTrans_glv = CmdModReset;
		Cmd_ThetaEst_glv = CmdModReset;
		Cmd_CurCtrl_glv = CmdModReset;
		Cmd_ErrPrt_glv = CmdModReset;

		Cmd_PWMDrive_glv = CmdModReset;
		Cmd_Charge_glv = CmdModReset;

		//复位FPGA
		//FPGAReset();
		//复位子模块
		//SampACmdReset();
		//SampBCmdReset();
		//SMCmdReset();

		Stat_Isr_glv = StatIsrReset;
	}
	else
	{
		IsrTick_gfv ++;
		if(IsrTick_gfv >= SampFreq_gfv)
		{
			IsrTick_gfv = 0;
		}

		if(Stat_Isr_glv != StatIsrError)
		{
			if(Stat_ErrPrt_glv != StatErrNone)
			{
				Stat_Isr_glv = StatIsrError;
			}
		}

		//检查桥臂fpga与子模块dsp的串行通信，并设置子模块拓扑
		if(Stat_Isr_glv == StatIsrReset)
		{
			if(Cmd_Isr_glv == CmdIsrPrep)
			{
				if(SMSetCheck() && SampASetCheck() && SampBSetCheck())
				{
					#if Close_loop
					//相序判断
					Cmd_SeqJudge_glv = CmdModStart;
					Stat_Isr_glv = StatIsrSeqJudge;
					#else
					Stat_SeqJudge_glv = StatModRise;
					Stat_ThetaEst_glv = StatModNorm;
					Cmd_SeqJudge_glv = CmdModStart;
					Cmd_PWMDrive_glv = CmdModStart;
					Cmd_SeqTrans_glv = CmdModStart;
					Cmd_ThetaEst_glv = CmdModStart;
					Stat_Isr_glv = StatIsrBusVoltWait;	//开环直接进入等待母线上电
					#endif

				}
				else
				{
					//检查子模块及采样机箱串行通信是否正常
					SampACmdSet();
					SampBCmdSet();
					SMCmdSet();
				}
			}
		}
		//检查串行通信无误后，开始相序判断
		else if(Stat_Isr_glv == StatIsrSeqJudge)
		{
			if(Cmd_Isr_glv == CmdIsrPrep)
			{
				if(Stat_SeqJudge_glv & (StatModRise|StatModFall))
				{
					Cmd_SeqTrans_glv = CmdModStart;
					Cmd_ThetaEst_glv = CmdModStart;
					Cmd_PWMDrive_glv = CmdModStart;
					Stat_Isr_glv = StatIsrThetaEst;
				}
			}
		}
		//相序判断结束后开始锁相
		else if(Stat_Isr_glv == StatIsrThetaEst)
		{
			if(Cmd_Isr_glv == CmdIsrPrep)
			{
				if(Stat_ThetaEst_glv == StatModNorm)
				{
					Stat_Isr_glv = StatIsrBusVoltWait;
				}
			}
		}
		//锁相成功后，检查直流母线是否欠压
		else if(Stat_Isr_glv == StatIsrBusVoltWait)
		{
			if(Cmd_Isr_glv == CmdIsrPrep)
			{
				//检测直流母线电压，若为开环则判断直流母线是否在范围内，如果闭环则判断直流母线能否满足交流输出要求
				#if Close_loop
				if(VDCPU_gfv > VolAmpFilPU_gfv * SQRT3 * 1.0)	//调至比受限
				#else
				if((VDCPU_gfv > 0.95* VolDCNormPU_gfv) && (VDCPU_gfv < 1.05* VolDCNormPU_gfv))
				#endif
				{
					Cmd_ErrPrt_glv = CmdModStart;
					Stat_Isr_glv = StatIsrPrep;

				}
			}
		}
		//直流母线不欠压，则等待开机预充电
		else if(Stat_Isr_glv == StatIsrPrep)
		{
			if(Cmd_Isr_glv == CmdIsrCharge)
			{
				Cmd_Charge_glv = CmdModStart;
				Stat_Isr_glv = StatIsrStandby;
			}
		}
		//预充电完成后，等待开始运行指令
		else if(Stat_Isr_glv == StatIsrStandby)
		{
			if((Stat_Charge_glv & Stat_ThetaEst_glv) == StatModNorm)
			{
				if(Cmd_Isr_glv == CmdIsrStart)
				{
					Cmd_CurCtrl_glv = CmdModStart;
					Cmd_PWMDrive_glv = CmdModStart;
					Stat_Isr_glv = StatIsrRun;
				}
			}
		}
		else if(Stat_Isr_glv == StatIsrRun)
		{
			if(Cmd_Isr_glv == CmdIsrStop)
			{
				/************for debug*************
				#if !Close_loop
				Cmd_PWMDrive_glv = CmdModStop;
				#endif
				***********end debug**************/
				Cmd_CurCtrl_glv = CmdModStop;
				Stat_Isr_glv = StatIsrSoftOff;
			}
			else if(Cmd_Isr_glv == CmdIsrTrip)
			{
				Cmd_PWMDrive_glv = CmdModStop;
				Cmd_CurCtrl_glv = CmdModTrip;
				Stat_Isr_glv = StatIsrError;
			}
		}
		else if(Stat_Isr_glv == StatIsrSoftOff)
		{
			if(Stat_CurCtrl_glv == StatModStop)
			{
				//Cmd_PWMDrive_glv = CmdModReset;
				//Cmd_CurCtrl_glv = CmdModReset;
				Stat_Isr_glv = StatIsrStandby;
			}
		}
		else if(Stat_Isr_glv == StatIsrError)
		{
			;
		}
	}

	// Key Control Algorithms
	if((Cmd_Isr_glv != CmdIsrNone) && (Stat_Isr_glv != StatIsrNone))
	{
		SeqJudge();
		SeqTrans();
		ThetaEst();
		ChargeCtrl();
		CurCtrl();
		DriveCtrl();
		ErrPrt();
	}
	ReadAfterWrite = SampBRegs.VPA;  //写FPGA之后要再随便读一次FPGA，才能接着正常读取LAN9118，否则LAN9118读取不正常，控制也异常

	if (Cmd_Watch && IniLAN)	//!
	{

		//设计以5KHz频率上传数据，上位机一副图绘制工频两个周期波形
		Send_Watch_Frame(FramNum, SMNum);//只需2860

		FramNum++;
		if (FramNum == 200) //帧编号0-199
		{
			FramNum = 0;
		}

		SMNum++;
		if (SMNum == 72) //子模块电容编号0-71
		{
			SMNum = 0;
		}

	}
	else if (Cmd_Ctrl && IniLAN)
	{
		Send_Debug_Frame();		//151个数据耗时7300
	}

	if(IniLAN)
	{
		LAN911xGetFrame(RxEthnetFrameBuffer); //空判断需要250，ARP需要2800
	}

//	IsrTimeStampOut_glv = (ReadCpuTimer1Period() - ReadCpuTimer1Counter());

	// Exit Time Stamp
	IsrTimeStampOut_glv = (ReadCpuTimer1Period() - ReadCpuTimer1Counter()); //8332
	// Interupt acknowledgement and return
	EPwm1Regs.ETCLR.bit.INT = 1;					// Clear PWM Int
	//AdcRegs.ADCTRL2.bit.RST_SEQ1 = 1;				// Reset ADC
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP3;			// ACK PIE
	return;

}

void ErrPrt(void)
{

	if(Cmd_ErrPrt_glv == CmdModReset)
	{
		Stat_ErrPrt_glv = StatErrNone;
	}
	else if(Cmd_ErrPrt_glv == CmdModStart)
	{
		//Software Protection
		if(Stat_ErrPrt_glv == StatErrNone)
		{
			//桥臂电流保护
			if(fabs(IPA_gfv) > 35.0)
			{
				Stat_ErrPrt_glv= StatErrOverCurPU;
				IPADebug = IPA_gfv;
			}
			else if(fabs(IAN_gfv) > 35.0)
			{
				Stat_ErrPrt_glv= StatErrOverCurUN;
				IANDebug = IAN_gfv;
			}
			else if(fabs(IPB_gfv) > 35.0)
			{
				Stat_ErrPrt_glv= StatErrOverCurPV;
				IPBDebug = IPB_gfv;
			}
			else if(fabs(IBN_gfv) > 35.0)
			{
				Stat_ErrPrt_glv= StatErrOverCurVN;
				IBNDebug = IBN_gfv;
			}
			else if(fabs(IPC_gfv) > 35.0)
			{
				Stat_ErrPrt_glv= StatErrOverCurPW;
				IPCDebug = IPC_gfv;
			}
			else if(fabs(ICN_gfv) > 35.0)
			{
				Stat_ErrPrt_glv= StatErrOverCurWN;
				ICNDebug = ICN_gfv;
			}
			//桥臂电压保护
			else if(VPA_gfv > 880.0)
				Stat_ErrPrt_glv= StatErrOverVolPU;
			else if(VAN_gfv > 880.0)
				Stat_ErrPrt_glv= StatErrOverVolUN;
			else if(VPB_gfv > 880.0)
				Stat_ErrPrt_glv= StatErrOverVolPV;
			else if(VBN_gfv > 880.0)
				Stat_ErrPrt_glv= StatErrOverVolVN;
			else if(VPC_gfv > 880.0)
				Stat_ErrPrt_glv= StatErrOverVolPW;
			else if(VCN_gfv > 880.0)
				Stat_ErrPrt_glv= StatErrOverVolWN;
			//子模块及采样箱接收数据保护
			else if((SciErrSMCnt_gusv > 3) || (StaErrSMCnt_gusv > 1))
				Stat_ErrPrt_glv= StatErrSMInquire;
			else if((SciErrSampACnt_gusv > 3) || (StaErrSampACnt_gusv > 1))
				Stat_ErrPrt_glv= StatErrSampAInquire;
			else if((SciErrSampBCnt_gusv > 3) || (StaErrSampBCnt_gusv > 1))
				Stat_ErrPrt_glv= StatErrSampBInquire;
			//控制保护
			else if(Stat_CurCtrl_glv == StatModError)
				Stat_ErrPrt_glv= StatErrCurCtrl;
			else if(Stat_ThetaEst_glv == StatModError)
				Stat_ErrPrt_glv= StatErrThetaEst;/**/
			//子模块电容电压保护
//			else if ((Stat_Isr_glv == StatIsrRun) && ( SMVolCheck() == 0))
//			{
//					Stat_ErrPrt_glv= StatErrSMVol;/**/
//			}
			//=======================================
			//短路子模块电压保护
			else if(VNVSM_gfv[0] + VNVSM_gfv[1] > 40)
			{
				Stat_ErrPrt_glv= StatErrSMVol;
			}
			//=======================================
		}

		if(Stat_ErrPrt_glv != StatErrNone)
		{
			SMCmdStop();
			SampACmdStop();
			//Flag_PWMOn_glv = 0;
		}
	}

}
#pragma CODE_SECTION(ErrPrt,"ramfuncs");

/*
void SelectEcana(void)
{
	pECanRegsa = &ECanaRegs;
	pECanMboxesa = &ECanaMboxes;
	pECanLAMRegsa = &ECanaLAMRegs;
}

void SelectEcanb(void)
{
	pECanRegsb = &ECanbRegs;
	pECanMboxesb = &ECanbMboxes;
	pECanLAMRegsb = &ECanbLAMRegs;
}

void InitMonitorVar(void)
{
	//Attribute the address of the monitoring variable to the list below
	WaveVarPt[MON_SCOPE+1]= (Uint32 *)&CurDDiffPU_gfv;
	WaveVarPt[MON_SCOPE+2]= (Uint32 *)&CurQDiffPU_gfv;
	WaveVarPt[MON_SCOPE+3]= (Uint32 *)&CurDComPU_gfv;
	WaveVarPt[MON_SCOPE+4]= (Uint32 *)&CurQComPU_gfv;
//	WaveVarPt[MON_SCOPE+1]= (Uint32 *)&UPSMNR;
//	WaveVarPt[MON_SCOPE+2]= (Uint32 *)&UNSMNR;

//
//	WaveVarPt[MON_SCOPE+5]= (Uint32 *)&UPARMSTRegsShadow.DRSR;
//	WaveVarPt[MON_SCOPE+6]= (Uint32 *)&VPARMSTRegsShadow.DRSR;
//	WaveVarPt[MON_SCOPE+7]= (Uint32 *)&WPARMSTRegsShadow.DRSR;

	WaveVarPt[MON_SCOPE+5]= (Uint32 *)&VolUPU_gfv;
	WaveVarPt[MON_SCOPE+6]= (Uint32 *)&VolWPU_gfv;
	WaveVarPt[MON_SCOPE+7]= (Uint32 *)&VolVPU_gfv;

	WaveVarPt[MON_SCOPE+8]= (Uint32 *)&ThetaEstPU_gfv;
	WaveVarPt[MON_SCOPE+9]= (Uint32 *)&ThetaEst2rdPU_gfv;
	WaveVarPt[MON_SCOPE+10]= (Uint32 *)&VVWG_gfv;
	WaveVarPt[MON_SCOPE+11]= (Uint32 *)&VWUG_gfv;
	WaveVarPt[MON_SCOPE+12]= (Uint32 *)&IVDC_gfv;

	WaveVarPt[MON_SCOPE+13]= (Uint32 *)&VPA_gfv;
	WaveVarPt[MON_SCOPE+14]= (Uint32 *)&VPB_gfv;
	WaveVarPt[MON_SCOPE+15]= (Uint32 *)&VPC_gfv;
	WaveVarPt[MON_SCOPE+16]= (Uint32 *)&VAN_gfv;
	WaveVarPt[MON_SCOPE+17]= (Uint32 *)&VBN_gfv;
	WaveVarPt[MON_SCOPE+18]= (Uint32 *)&VCN_gfv;

	WaveVarPt[MON_SCOPE+19]= (Uint32 *)&VDC_gfv;

//	WaveVarPt[MON_SCOPE+20]= (Uint32 *)&IPA_gfv;
//	WaveVarPt[MON_SCOPE+21]= (Uint32 *)&IPB_gfv;
//	WaveVarPt[MON_SCOPE+22]= (Uint32 *)&IPC_gfv;
//	WaveVarPt[MON_SCOPE+23]= (Uint32 *)&IAN_gfv;
//	WaveVarPt[MON_SCOPE+24]= (Uint32 *)&IBN_gfv;
//	WaveVarPt[MON_SCOPE+25]= (Uint32 *)&ICN_gfv;
	WaveVarPt[MON_SCOPE+20]= (Uint32 *)&PWMDutyUP_gulv;
	WaveVarPt[MON_SCOPE+21]= (Uint32 *)&PWMDutyUN_gulv;
	WaveVarPt[MON_SCOPE+22]= (Uint32 *)&PWMDutyVP_gulv;
	WaveVarPt[MON_SCOPE+23]= (Uint32 *)&PWMDutyVN_gulv;
	WaveVarPt[MON_SCOPE+24]= (Uint32 *)&PWMDutyWP_gulv;
	WaveVarPt[MON_SCOPE+25]= (Uint32 *)&PWMDutyWN_gulv;
	WaveVarPt[MON_SCOPE+26]= (Uint32 *)&ConvVolUPPU_gfv;
	WaveVarPt[MON_SCOPE+27]= (Uint32 *)&ConvVolUNPU_gfv;
	WaveVarPt[MON_SCOPE+28]= (Uint32 *)&ConvVolVPPU_gfv;
	WaveVarPt[MON_SCOPE+29]= (Uint32 *)&ConvVolVNPU_gfv;
	WaveVarPt[MON_SCOPE+30]= (Uint32 *)&ConvVolWPPU_gfv;
}
*/
