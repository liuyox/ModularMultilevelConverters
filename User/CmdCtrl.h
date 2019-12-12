/*
 * CmdCtrl.h
 *
 *  Created on: 2016-7-21
 *      Author: Administrator
 */

#ifndef CMDCTRL_H_
#define CMDCTRL_H_

#include "main.h"

//SM Cabinet Command
#define SMCMD_2L		0x04
#define SMCMD_3L		0x05
#define SMCMD_RUN		0x08
#define SMCMD_STOP		0x0C
#define SMCMD_RESET		0x10
#define SMCMD_UPDATE	0x14
#define SMCMD_FANOFF	0x18
#define SMCMD_FANON		0x19

// SM Topology Control Bit
//#define Topology_T3L   	0  // T-type Three Level
//#define Topology_F2L  	1  // Full-bridge Two Level

//Sample Cabinet Command
#define SAMPCMD_CHK			0x10
#define SAMPCMD_RUN			0x20
#define SAMPCMD_STOP		0x30
#define SAMPCMD_RESET		0x40
#define SAMPCMD_UPDATE		0x50
#define SAMPCMD_ACSOFF		0x70
#define SAMPCMD_ACSON  		0x74
#define SAMPCMD_ACOFF		0x80
#define SAMPCMD_ACON		0x84
#define SAMPCMD_DCSOFF		0x90
#define SAMPCMD_DCSON		0x94
#define SAMPCMD_DCOFF		0xA0
#define SAMPCMD_DCON		0xA4
#define SAMPCMD_DCCBOFF		0xB0
#define SAMPCMD_DCCBON		0xB4

//Command for SM DSP
extern void SMCmdSet(void);
extern void SMCmdRun(void);
extern void SMCmdStop(void);
extern void SMCmdReset(void);
extern void SMCmdUpdate(void);
extern void SMCmdFanOn(void);
extern void SMCmdFanOff(void);

extern void SortStart(void);
extern void SortStop(void);
extern void SMVolSet(Uint16 VmodMax, Uint16 VmodMin);
extern int16 SMVolCheck(void);

//Command for SampleA DSP
extern void SampACmdSet(void);
extern void SampACmdRun(void);
extern void SampACmdStop(void);
extern void SampACmdReset(void);
extern void SampACmdACSOn(void);		//AC charge relay
extern void SampACmdACSOff(void);
extern void SampACmdACOn(void);		//AC main relay
extern void SampACmdACOff(void);
extern void SampACmdDCSOn(void);		//DC charge relay
extern void SampACmdDCSOff(void);
extern void SampACmdDCOn(void);		//DC main relay
extern void SampACmdDCOff(void);
extern void SampACmdDCCBOn(void);		//DC circuit breaker
extern void SampACmdDCCBOff(void);

//Command for SampleB DSP
extern void SampBCmdSet(void);
extern void SampBCmdRun(void);
extern void SampBCmdStop(void);
extern void SampBCmdReset(void);
extern void SampBCmdACSOn(void);		//AC charge relay
extern void SampBCmdACSOff(void);
extern void SampBCmdACOn(void);		//AC main relay
extern void SampBCmdACOff(void);
extern void SampBCmdDCSOn(void);		//DC charge relay
extern void SampBCmdDCSOff(void);
extern void SampBCmdDCOn(void);		//DC main relay
extern void SampBCmdDCOff(void);
extern void SampBCmdDCCBOn(void);		//DC circuit breaker
extern void SampBCmdDCCBOff(void);

//DSP State of SM Check
extern int16 SMSetCheck(void);
extern int16 SMRunCheck(void);
extern int16 SMStopCheck(void);
extern int16 SMResetCheck(void);
extern int16 SMUpdateCheck(void);

//DSP State of SampleB Check
extern int16 SampBSetCheck(void);
extern int16 SampBRunCheck(void);
extern int16 SampBStopCheck(void);
extern int16 SampBResetCheck(void);
extern int16 SampBUpdateCheck(void);

//DSP State of SampleA Check
extern int16 SampASetCheck(void);
extern int16 SampARunCheck(void);
extern int16 SampAStopCheck(void);
extern int16 SampAResetCheck(void);
extern int16 SampAUpdateCheck(void);

//子模块正常状态参考值
extern Uint16 StaSMChk_gusv;

extern Uint16 UPSMSTAT_gusv;
extern Uint16 UNSMSTAT_gusv;
extern Uint16 VPSMSTAT_gusv;
extern Uint16 VNSMSTAT_gusv;
extern Uint16 WPSMSTAT_gusv;
extern Uint16 WNSMSTAT_gusv;



#endif /* CMDCTRL_H_ */
