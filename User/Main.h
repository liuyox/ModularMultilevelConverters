/* =================================================================================
 Description:   Head file for Main.c                                     
=====================================================================================
 History:
-------------------------------------------------------------------------------------
 Created by LiuYongxia, 2018.05.24, Zhejiang University                                                  
------------------------------------------------------------------------------*/

#ifndef MAIN_H
#define MAIN_H
//=================== Global Definition=============================
#define Close_loop 		1
#define ENCCSC  		1   	// Enable the circulating current suppression controller
#define FPGA_MODULMOD  	1   	// Modulation Mode: 0-NLM,1-DP
#define FPGA_PERIOD  	3750 	// Ts = 3750*2/37.5us = 200us
#define Debug	  		0 		//
#define EN_OverModu		0		//Enable over-modulation
//==================================================================
enum CMDISR   {   CmdIsrNone  = (0x1L<<0),
                  CmdIsrReset = (0x1L<<1),
                  CmdIsrPrep  = (0x1L<<2),
                  CmdIsrCharge= (0x1L<<3),
                  CmdIsrStart = (0x1L<<4),
                  CmdIsrStop  = (0x1L<<5),    //Common Stop
                  CmdIsrTrip  = (0x1L<<6)};   //Emergency Stop

enum STATISR  {   StatIsrNone          = (0x1L<<0),
                  StatIsrReset         = (0x1L<<1),  //清零
                  StatIsrADCInit       = (0x1L<<2),  //ADC初始化
                  StatIsrSeqJudge      = (0x1L<<3),  //相序判断
                  StatIsrThetaEst      = (0x1L<<4),  //坐标变换
                  StatIsrBusVoltWait   = (0x1L<<5),  //检查直流母线
                  StatIsrPrep		   = (0x1L<<6),  //等待开机
                  StatIsrCharge        = (0x1L<<7),	 //预充电
                  StatIsrStandby       = (0x1L<<8),  //等待开始运行
                  StatIsrRun           = (0x1L<<9),
                  StatIsrSoftOn        = (0x1L<<10),
                  StatIsrSoftOff       = (0x1L<<11),
                  StatIsrError         = (0x1L<<12)};

enum CMDMOD   {   CmdModNone  = (0x1L<<0),
                  CmdModReset = (0x1L<<1),
                  CmdModPrep  = (0x1L<<2),
                  CmdModStart = (0x1L<<3),
                  CmdModStop  = (0x1L<<4),
                  CmdModTrip  = (0x1L<<5),
                  CmdMod1     = (0x1L<<6),
                  CmdMod2     = (0x1L<<7),
                  CmdMod3     = (0x1L<<8),
                  CmdMod4     = (0x1L<<9)};

enum STATMOD  {   StatModNone    = (0x1L<<0),
                  StatModReset   = (0x1L<<1),
                  StatModPrep    = (0x1L<<2),     //Prep, preparation for operation
                  StatModRise    = (0x1L<<3),     //Rise, soft start and so on
                  StatModFall    = (0x1L<<4),     //Fall, soft stop and so on
                  StatModNorm    = (0x1L<<5),     //Norm, normal operation
                  StatModAbNomm  = (0x1L<<6),     //Abnorm, abnormal operation
                  StatModStop    = (0x1L<<7),     //Stop, standby
                  StatModError   = (0x1L<<8),
                  StatMod1       = (0x1L<<9),
                  StatMod2       = (0x1L<<10),
                  StatMod3       = (0x1L<<11),
                  StatMod4       = (0x1L<<12)};

enum STATERR  {   StatErrNone,
                  //桥臂电流保护
				  StatErrOverCurPU,
                  StatErrOverCurUN,
                  StatErrOverCurPV,
                  StatErrOverCurVN,
                  StatErrOverCurPW,
                  StatErrOverCurWN,
                  //桥臂电压保护
                  StatErrOverVolPU,
                  StatErrOverVolUN,
                  StatErrOverVolPV,
                  StatErrOverVolVN,
                  StatErrOverVolPW,
                  StatErrOverVolWN,
                  //直流侧电流电压保护
                  StatErrOverCurDC,
                  StatErrOverVolDC,
                  //电网电压故障状态保护
                  StatErrACOverFreq,
                  StatErrACOverVol,
                  StatErrACUnderFreq,
                  StatErrACUnderVol,
                  StatErrSeqJudge,
                  StatErrThetaEst,
                  StatErrCurCtrl,
                  //子模块电容电压保护
                  StatErrSMVol,
                  //子模块状态错误保护
                  StatErrSMInquire,
				  StatErrSampAInquire,
				  StatErrSampBInquire};


#endif    // end of Main.h
