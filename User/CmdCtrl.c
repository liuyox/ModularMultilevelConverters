/*
 * CmdCtrl.c
 *
 *  Created on: 2016-7-21
 *      Author: Administrator
 */

#include "DSP2833x_Device.h"
#include "DSP2833x_Examples.h"
#include "CmdCtrl.h"
#include "FPGA.h"
#include "ParaCalc.h"
#include "main.h"

#define ARMSTAT         1

Uint16 UPSMSTAT_gusv = 0;
Uint16 UNSMSTAT_gusv = 0;
Uint16 VPSMSTAT_gusv = 0;
Uint16 VNSMSTAT_gusv = 0;
Uint16 WPSMSTAT_gusv = 0;
Uint16 WNSMSTAT_gusv = 0;

void SMCmdSet(void)
{
	if(SMTYPE_gulv == TopologyT3L)
	{
		UPSortRegs.DRVCMD = SMCMD_3L;
		UNSortRegs.DRVCMD = SMCMD_3L;
		VPSortRegs.DRVCMD = SMCMD_3L;
		VNSortRegs.DRVCMD = SMCMD_3L;
		WPSortRegs.DRVCMD = SMCMD_3L;
		WNSortRegs.DRVCMD = SMCMD_3L;
	}
	else if(SMTYPE_gulv == TopologyF2L)
	{
		UPSortRegs.DRVCMD = SMCMD_2L;
		UNSortRegs.DRVCMD = SMCMD_2L;
		VPSortRegs.DRVCMD = SMCMD_2L;
		VNSortRegs.DRVCMD = SMCMD_2L;
		WPSortRegs.DRVCMD = SMCMD_2L;
		WNSortRegs.DRVCMD = SMCMD_2L;
	}

}
void SMCmdRun(void)
{
	UPSortRegs.DRVCMD = SMCMD_RUN;
	UNSortRegs.DRVCMD = SMCMD_RUN;
	VPSortRegs.DRVCMD = SMCMD_RUN;
	VNSortRegs.DRVCMD = SMCMD_RUN;
	WPSortRegs.DRVCMD = SMCMD_RUN;
	WNSortRegs.DRVCMD = SMCMD_RUN;
}
void SMCmdStop(void)
{
	UPSortRegs.DRVCMD = SMCMD_STOP;
	UNSortRegs.DRVCMD = SMCMD_STOP;
	VPSortRegs.DRVCMD = SMCMD_STOP;
	VNSortRegs.DRVCMD = SMCMD_STOP;
	WPSortRegs.DRVCMD = SMCMD_STOP;
	WNSortRegs.DRVCMD = SMCMD_STOP;
}
void SMCmdReset(void)
{
	UPSortRegs.DRVCMD = SMCMD_RESET;
	UNSortRegs.DRVCMD = SMCMD_RESET;
	VPSortRegs.DRVCMD = SMCMD_RESET;
	VNSortRegs.DRVCMD = SMCMD_RESET;
	WPSortRegs.DRVCMD = SMCMD_RESET;
	WNSortRegs.DRVCMD = SMCMD_RESET;
}
void SMCmdUpdate(void)
{
	//...
}
void SMCmdFanOn(void)
{
	UPSortRegs.DRVCMD = SMCMD_FANON;
	UNSortRegs.DRVCMD = SMCMD_FANON;
	VPSortRegs.DRVCMD = SMCMD_FANON;
	VNSortRegs.DRVCMD = SMCMD_FANON;
	WPSortRegs.DRVCMD = SMCMD_FANON;
	WNSortRegs.DRVCMD = SMCMD_FANON;
}
void SMCmdFanOff(void)
{
	UPSortRegs.DRVCMD = SMCMD_FANOFF;
	UNSortRegs.DRVCMD = SMCMD_FANOFF;
	VPSortRegs.DRVCMD = SMCMD_FANOFF;
	VNSortRegs.DRVCMD = SMCMD_FANOFF;
	WPSortRegs.DRVCMD = SMCMD_FANOFF;
	WNSortRegs.DRVCMD = SMCMD_FANOFF;
}

void SortStart(void)
{
	UPSortRegs.MCR.bit.ENSORT = 1;
	UNSortRegs.MCR.bit.ENSORT = 1;
	VPSortRegs.MCR.bit.ENSORT = 1;
	VNSortRegs.MCR.bit.ENSORT = 1;
	WPSortRegs.MCR.bit.ENSORT = 1;
	WNSortRegs.MCR.bit.ENSORT = 1;
}
void SortStop(void)
{
	UPSortRegs.MCR.bit.ENSORT = 0;
	UNSortRegs.MCR.bit.ENSORT = 0;
	VPSortRegs.MCR.bit.ENSORT = 0;
	VNSortRegs.MCR.bit.ENSORT = 0;
	WPSortRegs.MCR.bit.ENSORT = 0;
	WNSortRegs.MCR.bit.ENSORT = 0;
}

void SMVolSet(Uint16 VmodMax, Uint16 VmodMin)
{
	UPARMSTRegs.VMAXR = VmodMax;
	UPARMSTRegs.VMINR = VmodMin;
	UNARMSTRegs.VMAXR = VmodMax;
	UNARMSTRegs.VMINR = VmodMin;
	VPARMSTRegs.VMAXR = VmodMax;
	VPARMSTRegs.VMINR = VmodMin;
	VNARMSTRegs.VMAXR = VmodMax;
	VNARMSTRegs.VMINR = VmodMin;
	WPARMSTRegs.VMAXR = VmodMax;
	WPARMSTRegs.VMINR = VmodMin;
	WNARMSTRegs.VMAXR = VmodMax;
	WNARMSTRegs.VMINR = VmodMin;
}

//Command for SampleA DSP
void SampACmdSet(void)
{
	SampARegs.THR = SAMPCMD_CHK;
}
void SampACmdRun(void)
{
	SampARegs.THR = SAMPCMD_RUN;
}
void SampACmdStop(void)
{
	SampARegs.THR = SAMPCMD_STOP;
}
void SampACmdReset(void)
{
	SampARegs.THR = SAMPCMD_RESET;
}
void SampACmdACSOn(void)		//AC charge relay
{
	SampARegs.THR = SAMPCMD_ACSON;
}
void SampACmdACSOff(void)
{
	SampARegs.THR = SAMPCMD_ACSOFF;
}
void SampACmdACOn(void)		//AC main relay
{
	SampARegs.THR = SAMPCMD_ACON;
}
void SampACmdACOff(void)
{
	SampARegs.THR = SAMPCMD_ACOFF;
}
void SampACmdDCSOn(void)		//DC charge relay
{
	SampARegs.THR = SAMPCMD_DCSON;
}
void SampACmdDCSOff(void)
{
	SampARegs.THR = SAMPCMD_DCSOFF;
}
void SampACmdDCOn(void)		//DC main relay
{
	SampARegs.THR = SAMPCMD_DCON;
}
void SampACmdDCOff(void)
{
	SampARegs.THR = SAMPCMD_DCSOFF;
}
void SampACmdDCCBOn(void)
{
	SampARegs.THR = SAMPCMD_DCCBON; //DC circuit breaker ON
}
void SampACmdDCCBOff(void)
{
	SampARegs.THR = SAMPCMD_DCCBOFF; //DC circuit breaker OFF
}

//Command for SampleB DSP
void SampBCmdSet(void)
{
	SampBRegs.THR = SAMPCMD_CHK;
}
void SampBCmdRun(void)
{
	SampBRegs.THR = SAMPCMD_RUN;
}
void SampBCmdStop(void)
{
	SampBRegs.THR = SAMPCMD_STOP;
}
void SampBCmdReset(void)
{
	SampBRegs.THR = SAMPCMD_RESET;
}
void SampBCmdACSOn(void)		//AC charge relay
{
	SampBRegs.THR = SAMPCMD_ACSON;
}
void SampBCmdACSOff(void)
{
	SampBRegs.THR = SAMPCMD_ACSOFF;
}
void SampBCmdACOn(void)		//AC main relay
{
	SampBRegs.THR = SAMPCMD_ACON;
}
void SampBCmdACOff(void)
{
	SampBRegs.THR = SAMPCMD_ACOFF;
}
void SampBCmdDCSOn(void)		//DC charge relay
{
	SampBRegs.THR = SAMPCMD_DCSON;
}
void SampBCmdDCSOff(void)
{
	SampBRegs.THR = SAMPCMD_DCSOFF;
}
void SampBCmdDCOn(void)		//DC main relay
{
	SampBRegs.THR = SAMPCMD_DCON;
}
void SampBCmdDCOff(void)
{
	SampBRegs.THR = SAMPCMD_DCSOFF;
}
void SampBCmdDCCBOn(void)		//DC circuit breaker
{
	SampBRegs.THR = SAMPCMD_DCCBON;
}
void SampBCmdDCCBOff(void)
{
	SampBRegs.THR = SAMPCMD_DCCBOFF;
}

//DSP State of SM Check

int16 SMVolCheck(void)
{
	if(UPARMSTRegs.VASTR  & UNARMSTRegs.VASTR & VPARMSTRegs.VASTR &\
	   VNARMSTRegs.VASTR  & WPARMSTRegs.VASTR & WNARMSTRegs.VASTR &\
	   UPARMSTRegs.VBSTR  & UNARMSTRegs.VBSTR & VPARMSTRegs.VBSTR &\
	   VNARMSTRegs.VBSTR  & WPARMSTRegs.VBSTR & WNARMSTRegs.VBSTR == 0xFFF)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

int16 SMSetCheck(void)
{
#if ARMSTAT
	if(UPARMSTRegs.STBSTR  & UNARMSTRegs.STBSTR & VPARMSTRegs.STBSTR & \
	   VNARMSTRegs.STBSTR & WPARMSTRegs.STBSTR & WNARMSTRegs.STBSTR == StaSMChk_gusv)
		return 1;
	else
		return 0;
#else
	Uint16 i;
	UPSMSTAT_gusv = 0;
	UNSMSTAT_gusv = 0;
	VPSMSTAT_gusv = 0;
	VNSMSTAT_gusv = 0;
	WPSMSTAT_gusv = 0;
	WNSMSTAT_gusv = 0;

	for(i=0; i<12; i++)
	{
		if(*(UPSMSTRAddr_gusv + 8*i) == 0x0003)
		{
			UPSMSTAT_gusv |= (0x0001 << i);
		}
		if(*(UNSMSTRAddr_gusv + 8*i) == 0x0003)
		{
			UNSMSTAT_gusv |= (0x0001 << i);
		}
		if(*(VPSMSTRAddr_gusv + 8*i) == 0x0003)
		{
			VPSMSTAT_gusv |= (0x0001 << i);
		}
		if(*(VNSMSTRAddr_gusv + 8*i) == 0x0003)
		{
			VNSMSTAT_gusv |= (0x0001 << i);
		}
		if(*(WPSMSTRAddr_gusv + 8*i) == 0x0003)
		{
			WPSMSTAT_gusv |= (0x0001 << i);
		}
		if(*(WNSMSTRAddr_gusv + 8*i) == 0x0003)
		{
			WNSMSTAT_gusv |= (0x0001 << i);
		}
	}

	if(UPSMSTAT_gusv & UNSMSTAT_gusv & VPSMSTAT_gusv & VNSMSTAT_gusv\
    & WPSMSTAT_gusv & WNSMSTAT_gusv == 0x0fff)
	{
		return 1;
	}
	else
	{
		return 0;
	}
#endif
}
int16 SMRunCheck(void)
{
#if ARMSTAT
	if(UPARMSTRegs.RUNSTR & UNARMSTRegs.RUNSTR & VPARMSTRegs.RUNSTR \
	& VNARMSTRegs.RUNSTR & WPARMSTRegs.RUNSTR & WNARMSTRegs.RUNSTR == StaSMChk_gusv)
		return 1;
	else
		return 0;
#else
	Uint16 i;
	UPSMSTAT_gusv = 0;
	UNSMSTAT_gusv = 0;
	VPSMSTAT_gusv = 0;
	VNSMSTAT_gusv = 0;
	WPSMSTAT_gusv = 0;
	WNSMSTAT_gusv = 0;

	for(i=0; i<12; i++)
	{
		if(*(UPSMSTRAddr_gusv + 8*i) == 0x0004)
		{
			UPSMSTAT_gusv |= (0x0001 << i);
		}
		if(*(UNSMSTRAddr_gusv + 8*i) == 0x0004)
		{
			UNSMSTAT_gusv |= (0x0001 << i);
		}
		if(*(VPSMSTRAddr_gusv + 8*i) == 0x0004)
		{
			VPSMSTAT_gusv |= (0x0001 << i);
		}
		if(*(VNSMSTRAddr_gusv + 8*i) == 0x0004)
		{
			VNSMSTAT_gusv |= (0x0001 << i);
		}
		if(*(WPSMSTRAddr_gusv + 8*i) == 0x0004)
		{
			WPSMSTAT_gusv |= (0x0001 << i);
		}
		if(*(WNSMSTRAddr_gusv + 8*i) == 0x0004)
		{
			WNSMSTAT_gusv |= (0x0001 << i);
		}
	}

	if(UPSMSTAT_gusv & UNSMSTAT_gusv & VPSMSTAT_gusv & VNSMSTAT_gusv\
    & WPSMSTAT_gusv & WNSMSTAT_gusv == 0x0fff)
	{
		return 1;
	}
	else
	{
		return 0;
	}
#endif
}
int16 SMStopCheck(void)
{
#if ARMSTAT
	if(UPARMSTRegs.STOPSTR & UNARMSTRegs.STOPSTR & VPARMSTRegs.STOPSTR \
	& VNARMSTRegs.STOPSTR & WPARMSTRegs.STOPSTR & WNARMSTRegs.STOPSTR == StaSMChk_gusv)
		return 1;
	else
		return 0;
#else
	Uint16 i;
	UPSMSTAT_gusv = 0;
	UNSMSTAT_gusv = 0;
	VPSMSTAT_gusv = 0;
	VNSMSTAT_gusv = 0;
	WPSMSTAT_gusv = 0;
	WNSMSTAT_gusv = 0;

	for(i=0; i<12; i++)
	{
		if(*(UPSMSTRAddr_gusv + 8*i) == 0x0005)
		{
			UPSMSTAT_gusv |= (0x0001 << i);
		}
		if(*(UNSMSTRAddr_gusv + 8*i) == 0x0005)
		{
			UNSMSTAT_gusv |= (0x0001 << i);
		}
		if(*(VPSMSTRAddr_gusv + 8*i) == 0x0005)
		{
			VPSMSTAT_gusv |= (0x0001 << i);
		}
		if(*(VNSMSTRAddr_gusv + 8*i) == 0x0005)
		{
			VNSMSTAT_gusv |= (0x0001 << i);
		}
		if(*(WPSMSTRAddr_gusv + 8*i) == 0x0005)
		{
			WPSMSTAT_gusv |= (0x0001 << i);
		}
		if(*(WNSMSTRAddr_gusv + 8*i) == 0x0005)
		{
			WNSMSTAT_gusv |= (0x0001 << i);
		}
	}

	if(UPSMSTAT_gusv & UNSMSTAT_gusv & VPSMSTAT_gusv & VNSMSTAT_gusv\
    & WPSMSTAT_gusv & WNSMSTAT_gusv == 0x0fff)
	{
		return 1;
	}
	else
	{
		return 0;
	}
#endif
}
int16 SMResetCheck(void)
{
#if ARMSTAT
	if(UPARMSTRegs.PREPSTR & UNARMSTRegs.PREPSTR & VPARMSTRegs.PREPSTR \
	& VNARMSTRegs.PREPSTR & WPARMSTRegs.PREPSTR & WNARMSTRegs.PREPSTR == StaSMChk_gusv)
		return 1;
	else
		return 0;
#else
	Uint16 i;
	UPSMSTAT_gusv = 0;
	UNSMSTAT_gusv = 0;
	VPSMSTAT_gusv = 0;
	VNSMSTAT_gusv = 0;
	WPSMSTAT_gusv = 0;
	WNSMSTAT_gusv = 0;

	for(i=0; i<12; i++)
	{
		if(*(UPSMSTRAddr_gusv + 8*i) == 0x0002)
		{
			UPSMSTAT_gusv |= (0x0001 << i);
		}
		if(*(UNSMSTRAddr_gusv + 8*i) == 0x0002)
		{
			UNSMSTAT_gusv |= (0x0001 << i);
		}
		if(*(VPSMSTRAddr_gusv + 8*i) == 0x0002)
		{
			VPSMSTAT_gusv |= (0x0001 << i);
		}
		if(*(VNSMSTRAddr_gusv + 8*i) == 0x0002)
		{
			VNSMSTAT_gusv |= (0x0001 << i);
		}
		if(*(WPSMSTRAddr_gusv + 8*i) == 0x0002)
		{
			WPSMSTAT_gusv |= (0x0001 << i);
		}
		if(*(WNSMSTRAddr_gusv + 8*i) == 0x0002)
		{
			WNSMSTAT_gusv |= (0x0001 << i);
		}
	}

	if(UPSMSTAT_gusv & UNSMSTAT_gusv & VPSMSTAT_gusv & VNSMSTAT_gusv\
    & WPSMSTAT_gusv & WNSMSTAT_gusv == 0x0fff)
	{
		return 1;
	}
	else
	{
		return 0;
	}
#endif
}

int16 SampBSetCheck(void)
{
	if(SampBRegs.DRSTR.bit.STATE == 0x3)
		return 1;
	else
		return 0;
}
int16 SampBRunCheck(void)
{
	if(SampBRegs.DRSTR.bit.STATE == 0x4)
		return 1;
	else
		return 0;
}
int16 SampBStopCheck(void)
{
	if(SampBRegs.DRSTR.bit.STATE == 0x5)
		return 1;
	else
		return 0;
}
int16 SampBResetCheck(void)
{
	if(SampBRegs.DRSTR.bit.STATE == 0x2)
		return 1;
	else
		return 0;
}

int16 SampASetCheck(void)
{
	if(SampARegs.DRSTR.bit.STATE == 0x3)
		return 1;
	else
		return 0;
}
int16 SampARunCheck(void)
{
	if(SampARegs.DRSTR.bit.STATE == 0x4)
		return 1;
	else
		return 0;
}
int16 SampAStopCheck(void)
{
	if(SampARegs.DRSTR.bit.STATE == 0x5)
		return 1;
	else
		return 0;
}
int16 SampAResetCheck(void)
{
	if(SampARegs.DRSTR.bit.STATE == 0x2)
		return 1;
	else
		return 0;
}


