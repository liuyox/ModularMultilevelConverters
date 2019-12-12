// TI File $Revision: /main/5 $
// Checkin $Date: October 23, 2007   13:34:09 $
//###########################################################################
//
// FILE:	DSP2833x_Adc.c
//
// TITLE:	DSP2833x ADC Initialization & Support Functions.
//
//###########################################################################
// $TI Release: DSP2833x/DSP2823x C/C++ Header Files V1.31 $
// $Release Date: August 4, 2009 $
//###########################################################################

#include "DSP2833x_Device.h"     // DSP2833x Headerfile Include File
#include "DSP2833x_Examples.h"   // DSP2833x Examples Include File

#define ADC_usDELAY  5000L

//---------------------------------------------------------------------------
// InitAdc:
//---------------------------------------------------------------------------
// This function initializes ADC to a known state.
//
void InitAdc(void)
{
    extern void DSP28x_usDelay(Uint32 Count);


    // *IMPORTANT*
	// The ADC_cal function, which  copies the ADC calibration values from TI reserved
	// OTP into the ADCREFSEL and ADCOFFTRIM registers, occurs automatically in the
	// Boot ROM. If the boot ROM code is bypassed during the debug process, the
	// following function MUST be called for the ADC to function according
	// to specification. The clocks to the ADC MUST be enabled before calling this
	// function.
	// See the device data manual and/or the ADC Reference
	// Manual for more information.

	    EALLOW;
		SysCtrlRegs.PCLKCR0.bit.ADCENCLK = 1;
		ADC_cal();
		EDIS;




    // To powerup the ADC the ADCENCLK bit should be set first to enable
    // clocks, followed by powering up the bandgap, reference circuitry, and ADC core.
    // Before the first conversion is performed a 5ms delay must be observed
	// after power up to give all analog circuits time to power up and settle

    // Please note that for the delay function below to operate correctly the
	// CPU_RATE define statement in the DSP2833x_Examples.h file must
	// contain the correct CPU clock period in nanoseconds.

    AdcRegs.ADCTRL3.all = 0x00E0;  // Power up bandgap/reference/ADC circuits
    DELAY_US(ADC_usDELAY);         // Delay before converting ADC channels
}

//###########################################################################
//Adc Configuration Codes
//By WangYuxiang
//2013.11 Zhejiang University
//###########################################################################

// ADCTRL1 �Ĵ���
#define ADC_SHCLK  			 0x1   // ��������ʱ��S/H width in ADC module periods = 1 ADC clocks
#define ADC_CPS 			 0x0   // �ں�ʱ��Ԥ��Ƶ��ADC module clock�����һ���ڲ�����Ƶ
#define ADC_CONTRUN_RUNSTOP  0x0   // ��ͣģʽ
#define ADC_CONTRUN_CONTRUN  0x1   // ����ת��ģʽ
#define ADC_SEQCASC_DUALSEQ  0x0   // ˫����ģʽ
#define ADC_SEQCASC_SEQCNCT  0x1   // ����ģʽ

// ADCTRL2�Ĵ���
#define ADC_INTSEQ_ENABLE 	 0x1   // �����ж�����
#define ADC_INTSEQ_DISABLE   0x0   // �����ж�����
#define ADC_PWMSOC_ENABLE    0x1   // ����PWM��������ADת��
#define ADC_PWMSOC_DISABLE   0x0   // ����PWM��������ADת��
#define ADC_INTMOD_1ST       0x0   // �ж�ģʽ: ÿ��SEQ��������INT_SEQ
#define ADC_INTMOD_2ND       0x1   // �ж�ģʽ:ÿ��һ��SEQ��������INT_SEQ

// ADCTRL3�Ĵ���
#define ADC_CKPS   			 0x3   // ADC module clock�ڲ���Ƶ = HSPCLK/(2*ADC_CKPS)=75/6=12.5MHz
#define ADC_SMODESEL_SEQ     0x0   // ˳�����ģʽ
#define ADC_SMODESEL_SYN     0x1   // ͬ������ģʽ

// ADCMAXCONV�Ĵ��� (���ת��ͨ��)
#define ADC_MAX_CONV 0x000F

void ConfigAdc(void)
{
// adc clock 15M
	AdcRegs.ADCTRL3.bit.ADCCLKPS = ADC_CKPS;    // 1/10 of HSPCLK: 150/10 = 15MHz
	AdcRegs.ADCTRL1.bit.CPS = ADC_CPS;          //maybe extra one-half fre, but 0 is non

// һ��ADCת��������
	AdcRegs.ADCTRL1.bit.ACQ_PS = ADC_SHCLK;     //�������ִ���ʱ�䣬1��A Cclock

// �����������˳�����ģʽ
//	AdcRegs.ADCTRL3.bit.SMODE_SEL = ADC_SMODESEL_SEQ; 	//˳�����ģʽ
// ����ģʽ
	AdcRegs.ADCTRL1.bit.SEQ_CASC = ADC_SEQCASC_SEQCNCT;	//����ģʽ

// ����epwm�¼�����adc
// 	AdcRegs.ADCTRL2.bit.INT_ENA_SEQ1 = ADC_INTSEQ_DISABLE;  //��ֹseq1�жϣ� disable SEQ1 interrupt (every EOS)
   	AdcRegs.ADCTRL2.bit.EPWM_SOCA_SEQ1 = ADC_PWMSOC_ENABLE;	// Enable EPwmSOC to start SEQ1

// ���ת����15
	AdcRegs.ADCMAXCONV.all = ADC_MAX_CONV;

// �������Զ�����ת����ʽ�����������ת�������Զ���ת����ʼ״̬��
// ����������ֹͣ��ʽ����Ϊֻ��ÿ���ز�Ƶ�ʲ�������
	AdcRegs.ADCTRL1.bit.CONT_RUN = ADC_CONTRUN_RUNSTOP;

//=========================================================================
// ADCINA0(0X0)  IV             ADCINB0(0X8)  IU
// ADCINA1(0X1)  IW             ADCINB1(0X9)  IDC
// ADCINA2(0X2)  agnd           ADCINB2(0XA)  VDC2
// ADCINA3(0X3)  agnd           ADCINB3(0XB)  VDC1
// ADCINA4(0X4)  agnd           ADCINB4(0XC)  TEMP
// ADCINA5(0X5)  agnd           ADCINB5(0XD)  VW0
// ADCINA6(0X6)  agnd           ADCINB6(0XE)  VVW
// ADCINA7(0X7)  agnd           ADCINB7(0XF)  VUV
//=========================================================================

	//Key signals are over sampled
	AdcRegs.ADCCHSELSEQ1.bit.CONV00=0x8; //IU
	AdcRegs.ADCCHSELSEQ1.bit.CONV01=0x8; //IU
	AdcRegs.ADCCHSELSEQ1.bit.CONV02=0x0; //IV
	AdcRegs.ADCCHSELSEQ1.bit.CONV03=0x0; //IV
	AdcRegs.ADCCHSELSEQ2.bit.CONV04=0x1; //IW
	AdcRegs.ADCCHSELSEQ2.bit.CONV05=0x1; //IW
	AdcRegs.ADCCHSELSEQ2.bit.CONV06=0x9; //IDC
	AdcRegs.ADCCHSELSEQ2.bit.CONV07=0x9; //IDC
	AdcRegs.ADCCHSELSEQ3.bit.CONV08=0xB; //VDC1
	AdcRegs.ADCCHSELSEQ3.bit.CONV09=0xB; //VDC1
	AdcRegs.ADCCHSELSEQ3.bit.CONV10=0xA; //VDC2
	AdcRegs.ADCCHSELSEQ3.bit.CONV11=0xA; //VDC2
	AdcRegs.ADCCHSELSEQ4.bit.CONV12=0xF; //VUV
	AdcRegs.ADCCHSELSEQ4.bit.CONV13=0xE; //VVW
	AdcRegs.ADCCHSELSEQ4.bit.CONV14=0xD; //VW0
	AdcRegs.ADCCHSELSEQ4.bit.CONV15=0xC; //Temperature

    //AdcRegs.ADCREFSEL.all =  0;                 // Set up the ADC reference select register
    //AdcRegs.ADCOFFTRIM.all = 0;                 // Set up the ADC offset trim register

// adc�ж�ģʽѡ��
	AdcRegs.ADCTRL2.bit.INT_MOD_SEQ1 = ADC_INTMOD_1ST;		//�ж�ģʽ0����ÿ��SEQ1���������жϱ�־λint_seq1
}

//===========================================================================
// End of file.
//===========================================================================
