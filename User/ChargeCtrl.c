/*
 * ChargeCtrl.c
 *
 *  Created on: 2016-7-21
 *      Author: Administrator
 */

#include "DSP2833x_Device.h"
#include "DSP2833x_Examples.h"
#include "ChargeCtrl.h"
#include "ParaCalc.h"
#include "Sample.h"
#include "CmdCtrl.h"
#include "FPGA.h"
#include "math.h"

volatile enum CMDMOD   Cmd_Charge_glv = CmdModNone;
volatile enum STATMOD  Stat_Charge_glv = StatModNone;



void ChargeCtrl(void)
{
	if(Cmd_Charge_glv == CmdModReset)
	{
		Stat_Charge_glv = StatModReset;
	}
	else
	{
		if(Stat_Charge_glv == StatModReset)
		{
			if(Cmd_Charge_glv == CmdModStart)
			{
				if((SampARunCheck() == 1) && (SMRunCheck() == 1))
				{
					//Set SM Voltage reference
					SMVolSet(VolSMMax_gusv, VolSMMin_gusv);
					Stat_Charge_glv = StatModPrep;
				}
				else
				{
					//SampleA & SM Run(Drive enabled)
					SampACmdRun();
					SMCmdRun();
				}
			}
		}
		else if(Stat_Charge_glv == StatModPrep)
		{
			//���Ԥ������
			if(SMVolCheck() == 1)
			{
				//��������ֱ��ĸ����ֱ����ѹ
				if(abs(VDCR_gfv-VDC_gfv) < 15)
				{
					Stat_Charge_glv = StatMod1;
					SampACmdDCOn();
				}

			}
			else
			{
				SampACmdDCSOn();
			}
		}
		else if(Stat_Charge_glv == StatMod1)
		{
			//�ض�ֱ��Ԥ���̵���
			SampACmdDCSOff();
			Stat_Charge_glv = StatModNorm;
		}
		//����������ʼ�󣬷�����ģ���ѹԽ��
		else if(Stat_Charge_glv == StatModNorm)
		{
			if(SMVolCheck() != 1)
			{;}
				//Stat_Charge_glv = StatModError;
		}
	}
}
