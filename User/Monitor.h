/* =================================================================================
 Description:   Head file for Monitor.h                                     
=====================================================================================
 Version:		MonitorV3.B1
 History:		Created by LiuYongxia, 2018.04.08, Zhejiang University                                                 
------------------------------------------------------------------------------*/

#ifndef MONITOR_H
#define MONITOR_H

#define MON_SCOPE -1
#define ECANA 0
#define ECANB 1
#define MON_ECAN ECANB  //Please define the ECAN used for monitor, ECANA or ECANB
#define BAUD_RATE 1000 	//Please define the Baud Rate 1000,500,250,50
						//Please Change the GPIO Configuration in .c code

extern Uint32 *ParaVarPt[30];
extern Uint32 *WaveVarPt[30];
extern Uint32 MonitorPara[30];

extern int16 MonitorMain(volatile struct ECAN_REGS *pECanRegs,volatile struct ECAN_MBOXES *pECanMboxes);
extern int16 MonitorIsr(void);
extern int16 TrigManage(Uint32);

//extern void InitMonECan(void);
extern void InitMonECan(volatile struct ECAN_REGS *pECanRegs,volatile struct ECAN_MBOXES *pECanMboxes,volatile struct LAM_REGS *pECanLAMRegs);//modified
extern void ConfigMonECanBox(volatile struct ECAN_REGS *pECanRegs,volatile struct LAM_REGS *pECanLAMRegs,volatile struct ECAN_MBOXES *pECanMboxes);
//extern void InitMonECanBox(void);
extern void InitMonECanBox(volatile struct ECAN_REGS *pECanRegs,volatile struct ECAN_MBOXES *pECanMboxes);//modified
extern void InitMonECanAGpio(void);
extern void InitMonECanBGpio(void);
extern int16 MonitorTransmit(Uint32, Uint32,volatile struct ECAN_REGS *pECanRegs,volatile struct ECAN_MBOXES *pECanMboxes);
extern int16 MonitorReceive(volatile struct ECAN_REGS *pECanRegs,volatile struct ECAN_MBOXES *pECanMboxes);

#endif    // end of Monitor.h

