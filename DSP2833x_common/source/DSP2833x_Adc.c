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

// ADCTRL1 寄存器
#define ADC_SHCLK  			 0x1   // 采样窗口时间S/H width in ADC module periods = 1 ADC clocks
#define ADC_CPS 			 0x0   // 内核时钟预分频器ADC module clock额外的一次内部二分频
#define ADC_CONTRUN_RUNSTOP  0x0   // 启停模式
#define ADC_CONTRUN_CONTRUN  0x1   // 连续转换模式
#define ADC_SEQCASC_DUALSEQ  0x0   // 双序列模式
#define ADC_SEQCASC_SEQCNCT  0x1   // 级联模式

// ADCTRL2寄存器
#define ADC_INTSEQ_ENABLE 	 0x1   // 启用中断请求
#define ADC_INTSEQ_DISABLE   0x0   // 禁用中断请求
#define ADC_PWMSOC_ENABLE    0x1   // 启用PWM触发启动AD转换
#define ADC_PWMSOC_DISABLE   0x0   // 禁用PWM触发启动AD转换
#define ADC_INTMOD_1ST       0x0   // 中断模式: 每个SEQ结束设置INT_SEQ
#define ADC_INTMOD_2ND       0x1   // 中断模式:每隔一个SEQ结束设置INT_SEQ

// ADCTRL3寄存器
#define ADC_CKPS   			 0x3   // ADC module clock内部分频 = HSPCLK/(2*ADC_CKPS)=75/6=12.5MHz
#define ADC_SMODESEL_SEQ     0x0   // 顺序采样模式
#define ADC_SMODESEL_SYN     0x1   // 同步采样模式

// ADCMAXCONV寄存器 (最大转换通道)
#define ADC_MAX_CONV 0x000F

void ConfigAdc(void)
{
// adc clock 15M
	AdcRegs.ADCTRL3.bit.ADCCLKPS = ADC_CKPS;    // 1/10 of HSPCLK: 150/10 = 15MHz
	AdcRegs.ADCTRL1.bit.CPS = ADC_CPS;          //maybe extra one-half fre, but 0 is non

// 一次ADC转换的周期
	AdcRegs.ADCTRL1.bit.ACQ_PS = ADC_SHCLK;     //采样保持窗口时间，1×A Cclock

// 后面采样采用顺序采样模式
//	AdcRegs.ADCTRL3.bit.SMODE_SEL = ADC_SMODESEL_SEQ; 	//顺序采样模式
// 级联模式
	AdcRegs.ADCTRL1.bit.SEQ_CASC = ADC_SEQCASC_SEQCNCT;	//级联模式

// 允许epwm事件触发adc
// 	AdcRegs.ADCTRL2.bit.INT_ENA_SEQ1 = ADC_INTSEQ_DISABLE;  //禁止seq1中断， disable SEQ1 interrupt (every EOS)
   	AdcRegs.ADCTRL2.bit.EPWM_SOCA_SEQ1 = ADC_PWMSOC_ENABLE;	// Enable EPwmSOC to start SEQ1

// 最大转换数15
	AdcRegs.ADCMAXCONV.all = ADC_MAX_CONV;

// 不采用自动连续转换方式（采样到最大转换数后，自动跳转到开始状态）
// 采用启动－停止方式，因为只是每个载波频率采样而已
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

// adc中断模式选择
	AdcRegs.ADCTRL2.bit.INT_MOD_SEQ1 = ADC_INTMOD_1ST;		//中断模式0，即每次SEQ1结束则置中断标志位int_seq1
}

//===========================================================================
// End of file.
//===========================================================================
