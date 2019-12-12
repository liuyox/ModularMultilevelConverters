// TI File $Revision: /main/1 $
// Checkin $Date: August 18, 2006   13:46:19 $
//###########################################################################
//
// FILE:   DSP2833x_EPwm.c
//
// TITLE:  DSP2833x ePWM Initialization & Support Functions.
//
//###########################################################################
// $TI Release: DSP2833x/DSP2823x C/C++ Header Files V1.31 $
// $Release Date: August 4, 2009 $
//###########################################################################

#include "DSP2833x_Device.h"     // DSP2833x Headerfile Include File
#include "DSP2833x_Examples.h"   // DSP2833x Examples Include File

//---------------------------------------------------------------------------
// InitEPwm: 
//---------------------------------------------------------------------------
// This function initializes the ePWM(s) to a known state.
//
void InitEPwm(void)
{
   // Initialize ePWM1/2/3/4/5/6

   //tbd...
 
}

//---------------------------------------------------------------------------
// Example: InitEPwmGpio: 
//---------------------------------------------------------------------------
// This function initializes GPIO pins to function as ePWM pins
//
// Each GPIO pin can be configured as a GPIO pin or up to 3 different
// peripheral functional pins. By default all pins come up as GPIO
// inputs after reset.  
// 

void InitEPwmGpio(void)
{
   //主控dsp不需要输出GPIO，此口留作测试
   InitEPwm1Gpio();
   //InitEPwm2Gpio();
   //InitEPwm3Gpio();
#if DSP28_EPWM4
   //InitEPwm4Gpio();
#endif // endif DSP28_EPWM4

//========================
//This blocked of codes are useless for user, GuYunjie
/*
#if DSP28_EPWM5    
   InitEPwm5Gpio();
#endif // endif DSP28_EPWM5
#if DSP28_EPWM6
   InitEPwm6Gpio();
#endif // endif DSP28_EPWM6 
*/
//========================

}

void InitEPwm1Gpio(void)
{
   EALLOW;
   
/* Enable internal pull-up for the selected pins */
// Pull-ups can be enabled or disabled by the user. 
// This will enable the pullups for the specified pins.
// Comment out other unwanted lines.

    GpioCtrlRegs.GPAPUD.bit.GPIO0 = 0;    // Enable pull-up on GPIO0 (EPWM1A)
    GpioCtrlRegs.GPAPUD.bit.GPIO1 = 0;    // Enable pull-up on GPIO1 (EPWM1B)   
   
/* Configure ePWM-1 pins using GPIO regs*/
// This specifies which of the possible GPIO pins will be ePWM1 functional pins.
// Comment out other unwanted lines.

    GpioCtrlRegs.GPAMUX1.bit.GPIO0 = 1;   // Configure GPIO0 as EPWM1A
    GpioCtrlRegs.GPAMUX1.bit.GPIO1 = 1;   // Configure GPIO1 as EPWM1B
   
    EDIS;
}

void InitEPwm2Gpio(void)
{
   EALLOW;
	
/* Enable internal pull-up for the selected pins */
// Pull-ups can be enabled or disabled by the user. 
// This will enable the pullups for the specified pins.
// Comment out other unwanted lines.

    GpioCtrlRegs.GPAPUD.bit.GPIO2 = 0;    // Enable pull-up on GPIO2 (EPWM2A)
    GpioCtrlRegs.GPAPUD.bit.GPIO3 = 0;    // Enable pull-up on GPIO3 (EPWM3B)

/* Configure ePWM-2 pins using GPIO regs*/
// This specifies which of the possible GPIO pins will be ePWM2 functional pins.
// Comment out other unwanted lines.

    GpioCtrlRegs.GPAMUX1.bit.GPIO2 = 1;   // Configure GPIO2 as EPWM2A
    GpioCtrlRegs.GPAMUX1.bit.GPIO3 = 1;   // Configure GPIO3 as EPWM2B
   
    EDIS;
}

void InitEPwm3Gpio(void)
{
   EALLOW;
   
/* Enable internal pull-up for the selected pins */
// Pull-ups can be enabled or disabled by the user. 
// This will enable the pullups for the specified pins.
// Comment out other unwanted lines.

    GpioCtrlRegs.GPAPUD.bit.GPIO4 = 0;    // Enable pull-up on GPIO4 (EPWM3A)
    GpioCtrlRegs.GPAPUD.bit.GPIO5 = 0;    // Enable pull-up on GPIO5 (EPWM3B)
       
/* Configure ePWM-3 pins using GPIO regs*/
// This specifies which of the possible GPIO pins will be ePWM3 functional pins.
// Comment out other unwanted lines.

    GpioCtrlRegs.GPAMUX1.bit.GPIO4 = 1;   // Configure GPIO4 as EPWM3A
    GpioCtrlRegs.GPAMUX1.bit.GPIO5 = 1;   // Configure GPIO5 as EPWM3B
	
    EDIS;
}


#if DSP28_EPWM4
void InitEPwm4Gpio(void)
{
   EALLOW;
/* Enable internal pull-up for the selected pins */
// Pull-ups can be enabled or disabled by the user. 
// This will enable the pullups for the specified pins.
// Comment out other unwanted lines.

    GpioCtrlRegs.GPAPUD.bit.GPIO6 = 0;    // Enable pull-up on GPIO6 (EPWM4A)
    GpioCtrlRegs.GPAPUD.bit.GPIO7 = 0;    // Enable pull-up on GPIO7 (EPWM4B)

/* Configure ePWM-4 pins using GPIO regs*/
// This specifies which of the possible GPIO pins will be ePWM4 functional pins.
// Comment out other unwanted lines.

    GpioCtrlRegs.GPAMUX1.bit.GPIO6 = 1;   // Configure GPIO6 as EPWM4A
    GpioCtrlRegs.GPAMUX1.bit.GPIO7 = 1;   // Configure GPIO7 as EPWM4B
	
    EDIS;
}
#endif // endif DSP28_EPWM4  


#if DSP28_EPWM5
void InitEPwm5Gpio(void)
{
   EALLOW;
/* Enable internal pull-up for the selected pins */
// Pull-ups can be enabled or disabled by the user. 
// This will enable the pullups for the specified pins.
// Comment out other unwanted lines.

    GpioCtrlRegs.GPAPUD.bit.GPIO8 = 0;    // Enable pull-up on GPIO8 (EPWM5A)
    GpioCtrlRegs.GPAPUD.bit.GPIO9 = 0;    // Enable pull-up on GPIO9 (EPWM5B)

/* Configure ePWM-5 pins using GPIO regs*/
// This specifies which of the possible GPIO pins will be ePWM5 functional pins.
// Comment out other unwanted lines.

    GpioCtrlRegs.GPAMUX1.bit.GPIO8 = 1;   // Configure GPIO8 as EPWM5A
    GpioCtrlRegs.GPAMUX1.bit.GPIO9 = 1;   // Configure GPIO9 as EPWM5B
	
    EDIS;
}
#endif // endif DSP28_EPWM5


#if DSP28_EPWM6
void InitEPwm6Gpio(void)
{
   EALLOW;

/* Enable internal pull-up for the selected pins */
// Pull-ups can be enabled or disabled by the user. 
// This will enable the pullups for the specified pins.
// Comment out other unwanted lines.

    GpioCtrlRegs.GPAPUD.bit.GPIO10 = 0;    // Enable pull-up on GPIO10 (EPWM6A)
    GpioCtrlRegs.GPAPUD.bit.GPIO11 = 0;    // Enable pull-up on GPIO11 (EPWM6B)

/* Configure ePWM-6 pins using GPIO regs*/
// This specifies which of the possible GPIO pins will be ePWM6 functional pins.
// Comment out other unwanted lines.

    GpioCtrlRegs.GPAMUX1.bit.GPIO10 = 1;   // Configure GPIO10 as EPWM6A
    GpioCtrlRegs.GPAMUX1.bit.GPIO11 = 1;   // Configure GPIO11 as EPWM6B
	
    EDIS;
}
#endif // endif DSP28_EPWM6  

//---------------------------------------------------------------------------
// Example: InitEPwmSyncGpio: 
//---------------------------------------------------------------------------
// This function initializes GPIO pins to function as ePWM Synch pins
//

void InitEPwmSyncGpio(void)
{

   EALLOW;

/* Configure EPWMSYNCI  */
   
/* Enable internal pull-up for the selected pins */
// Pull-ups can be enabled or disabled by the user. 
// This will enable the pullups for the specified pins.
// Comment out other unwanted lines.

   GpioCtrlRegs.GPAPUD.bit.GPIO6 = 0;    // Enable pull-up on GPIO6 (EPWMSYNCI)
// GpioCtrlRegs.GPBPUD.bit.GPIO32 = 0;   // Enable pull-up on GPIO32 (EPWMSYNCI)    

/* Set qualification for selected pins to asynch only */
// This will select synch to SYSCLKOUT for the selected pins.
// Comment out other unwanted lines.

   GpioCtrlRegs.GPAQSEL1.bit.GPIO6 = 0;   // Synch to SYSCLKOUT GPIO6 (EPWMSYNCI)
// GpioCtrlRegs.GPBQSEL1.bit.GPIO32 = 0;  // Synch to SYSCLKOUT GPIO32 (EPWMSYNCI)    

/* Configure EPwmSync pins using GPIO regs*/
// This specifies which of the possible GPIO pins will be EPwmSync functional pins.
// Comment out other unwanted lines.   

   GpioCtrlRegs.GPAMUX1.bit.GPIO6 = 2;    // Enable pull-up on GPIO6 (EPWMSYNCI)
// GpioCtrlRegs.GPBMUX1.bit.GPIO32 = 2;   // Enable pull-up on GPIO32 (EPWMSYNCI)    



/* Configure EPWMSYNC0  */

/* Enable internal pull-up for the selected pins */
// Pull-ups can be enabled or disabled by the user. 
// This will enable the pullups for the specified pins.
// Comment out other unwanted lines.

// GpioCtrlRegs.GPAPUD.bit.GPIO6 = 0;    // Enable pull-up on GPIO6 (EPWMSYNC0)
   GpioCtrlRegs.GPBPUD.bit.GPIO33 = 0;   // Enable pull-up on GPIO33 (EPWMSYNC0)    

// GpioCtrlRegs.GPAMUX1.bit.GPIO6 = 3;    // Enable pull-up on GPIO6 (EPWMSYNC0)
   GpioCtrlRegs.GPBMUX1.bit.GPIO33 = 2;   // Enable pull-up on GPIO33 (EPWMSYNC0)    

}



//---------------------------------------------------------------------------
// Example: InitTzGpio: 
//---------------------------------------------------------------------------
// This function initializes GPIO pins to function as Trip Zone (TZ) pins
//
// Each GPIO pin can be configured as a GPIO pin or up to 3 different
// peripheral functional pins. By default all pins come up as GPIO
// inputs after reset.  
// 

void InitTzGpio(void)
{
   EALLOW;
   
/* Enable internal pull-up for the selected pins */
// Pull-ups can be enabled or disabled by the user. 
// This will enable the pullups for the specified pins.
// Comment out other unwanted lines.
   GpioCtrlRegs.GPAPUD.bit.GPIO12 = 0;    // Enable pull-up on GPIO12 (TZ1)
   GpioCtrlRegs.GPAPUD.bit.GPIO13 = 0;    // Enable pull-up on GPIO13 (TZ2)
   GpioCtrlRegs.GPAPUD.bit.GPIO14 = 0;    // Enable pull-up on GPIO14 (TZ3)
   GpioCtrlRegs.GPAPUD.bit.GPIO15 = 0;    // Enable pull-up on GPIO15 (TZ4)

   GpioCtrlRegs.GPAPUD.bit.GPIO16 = 0;    // Enable pull-up on GPIO16 (TZ5)
// GpioCtrlRegs.GPAPUD.bit.GPIO28 = 0;    // Enable pull-up on GPIO28 (TZ5)

   GpioCtrlRegs.GPAPUD.bit.GPIO17 = 0;    // Enable pull-up on GPIO17 (TZ6) 
// GpioCtrlRegs.GPAPUD.bit.GPIO29 = 0;    // Enable pull-up on GPIO29 (TZ6)  
   
/* Set qualification for selected pins to asynch only */
// Inputs are synchronized to SYSCLKOUT by default.  
// This will select asynch (no qualification) for the selected pins.
// Comment out other unwanted lines.

   GpioCtrlRegs.GPAQSEL1.bit.GPIO12 = 3;  // Asynch input GPIO12 (TZ1)
   GpioCtrlRegs.GPAQSEL1.bit.GPIO13 = 3;  // Asynch input GPIO13 (TZ2)
   GpioCtrlRegs.GPAQSEL1.bit.GPIO14 = 3;  // Asynch input GPIO14 (TZ3)
   GpioCtrlRegs.GPAQSEL1.bit.GPIO15 = 3;  // Asynch input GPIO15 (TZ4)

   GpioCtrlRegs.GPAQSEL2.bit.GPIO16 = 3;  // Asynch input GPIO16 (TZ5)
// GpioCtrlRegs.GPAQSEL2.bit.GPIO28 = 3;  // Asynch input GPIO28 (TZ5)

   GpioCtrlRegs.GPAQSEL2.bit.GPIO17 = 3;  // Asynch input GPIO17 (TZ6) 
// GpioCtrlRegs.GPAQSEL2.bit.GPIO29 = 3;  // Asynch input GPIO29 (TZ6)  

   
/* Configure TZ pins using GPIO regs*/
// This specifies which of the possible GPIO pins will be TZ functional pins.
// Comment out other unwanted lines.   
   GpioCtrlRegs.GPAMUX1.bit.GPIO12 = 1;  // Configure GPIO12 as TZ1
   GpioCtrlRegs.GPAMUX1.bit.GPIO13 = 1;  // Configure GPIO13 as TZ2
   GpioCtrlRegs.GPAMUX1.bit.GPIO14 = 1;  // Configure GPIO14 as TZ3
   GpioCtrlRegs.GPAMUX1.bit.GPIO15 = 1;  // Configure GPIO15 as TZ4

   GpioCtrlRegs.GPAMUX2.bit.GPIO16 = 3;  // Configure GPIO16 as TZ5
// GpioCtrlRegs.GPAMUX2.bit.GPIO28 = 3;  // Configure GPIO28 as TZ5

   GpioCtrlRegs.GPAMUX2.bit.GPIO17 = 3;  // Configure GPIO17 as TZ6               
// GpioCtrlRegs.GPAMUX2.bit.GPIO29 = 3;  // Configure GPIO29 as TZ6  

   EDIS;
}

//###########################################################################
//EPwm Configuration Codes
//By GuYunjie
//2014.04 Zhejiang University
//###########################################################################
//Mapping : EPWM1A:AP    EPWM1B:AN		(A,U Phase)
//          EPWM2A:BP    EPWM2B:BN		(B,V Phase)
//          EPWM3A:CP    EPWM3B:CN		(C,W Phase)
//          EPWM4A:DP    EPWM4B:DN		(DC Breaker)
//Mode :Offers two modes, boost or VSC. Use the compile switch PWMMODE_BOOST to select
//Boost:Duty cycle is for low switch, while high switch is always turned off, no dead band
//VSC  :Buty cycle is for high switch, while low switch is complamenraty, dead time defined in EPWMDB

#define EPWM_PERIOD  	15000  // Ts = 7500*2/150us = 100us
#define EPWM_CMP 	 	10000  // 1/2 Duty Cycle
#define EPWM_DB      	450   // 3us
//#define PWMMODE_BOOST   1	  // define this macro for boost mode, otherwise comment it
#define DCCIRBRK		  1   // define this macro if EPWM4 is used for DC circuit breaker, otherwise comment it

void ConfigEPwm1(void)
{
   // Setup TB
   EPwm1Regs.TBPRD = EPWM_PERIOD;  					// Set timer period
   EPwm1Regs.TBPHS.half.TBPHS = 0;            		// Phase is 0
   EPwm1Regs.TBCTR = 0x0000;                  		// Clear counter

   // Setup TBCTL
   EPwm1Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN; 	// up-down count mode
   EPwm1Regs.TBCTL.bit.PHSEN = TB_DISABLE;        	// 不使用移相方式
   EPwm1Regs.TBCTL.bit.PRDLD = TB_SHADOW;         	// 变化TBPRD时，使用shadow寄存器(即当计数器为0时才更新)
   EPwm1Regs.TBCTL.bit.SYNCOSEL = TB_CTR_CMPB;     	// SyncOut: CTR == CMPB
   EPwm1Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;       	// Clock ratio to SYSCLKOUT
   EPwm1Regs.TBCTL.bit.CLKDIV = TB_DIV1;          	// 两个均直通，即计数器时钟为系统时钟
   EPwm1Regs.TBCTL.bit.PHSDIR = TB_UP;            	// Phase Direction Bit
   EPwm1Regs.TBCTL.bit.FREE_SOFT = 0;             	// Emulation Mode Bits: Free run

   // Setup CMPCTL
   EPwm1Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;		// CC_IMMEDIATE;// CC_SHADOW;    // Load registers every ZERO
   EPwm1Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;		// CC_IMMEDIATE;// CC_SHADOW;
#ifdef PWMMODE_BOOST
   EPwm1Regs.CMPCTL.bit.LOADAMODE = CC_CTR_PRD;		// Boost mode reload on top for symetrical waveform
   EPwm1Regs.CMPCTL.bit.LOADBMODE = CC_CTR_PRD;
#else
   EPwm1Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO;
   EPwm1Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;
#endif

   // Setup CMP
   EPwm1Regs.CMPA.half.CMPA = EPWM_CMP;
   EPwm1Regs.CMPB = EPWM_PERIOD;
#ifdef PWMMODE_BOOST
   EPwm1Regs.CMPB = 0;					 //Constant off for boost mode
#endif

   // Set actions
   // Set PWM1A
   EPwm1Regs.AQCTLA.bit.CAU=AQ_CLEAR;    // 计数器(增加时)=比较器A时,输出low
   EPwm1Regs.AQCTLA.bit.CAD=AQ_SET;  	 // 计数器(减小时)=比较器A时,输出high
#ifdef PWMMODE_BOOST
   // Set PWM1B
   // PWMB is for boost mode, upper switch constant off
   EPwm1Regs.AQCTLB.bit.CBU=AQ_CLEAR;  	 // 计数器(增加时)=比较器A时,输出low
   EPwm1Regs.AQCTLB.bit.CBD=AQ_CLEAR;    // 计数器(减小时)=比较器A时,输出low
#endif

   // Set Deadband
   EPwm1Regs.DBCTL.bit.OUT_MODE = DB_FULL_ENABLE;  // 启用死区发生器
#ifdef PWMMODE_BOOST
   EPwm1Regs.DBCTL.bit.POLSEL = DB_ACTV_HI;        // No Inverting
   EPwm1Regs.DBCTL.bit.IN_MODE = DBB_RED_DBA_FED;  // Twist the AB channel
   EPwm1Regs.DBCTL.bit.OUT_MODE = DB_FULL_ENABLE;  // EPWMA feeded into EPWMB, EPWMB feeded into EPWMA
   EPwm1Regs.DBRED = 0;                 	   	   // Rising edge delay time zero
   EPwm1Regs.DBFED = 0;                      	   // Falling edge delay time zero
#else
   EPwm1Regs.DBCTL.bit.POLSEL = DB_ACTV_HIC;
   EPwm1Regs.DBCTL.bit.IN_MODE = DBA_ALL;		   // Use EPWMA to generate final output signal
   EPwm1Regs.DBCTL.bit.OUT_MODE = DB_FULL_ENABLE;
   EPwm1Regs.DBRED = EPWM_DB;                 	   // Rising edge delay time
   EPwm1Regs.DBFED = EPWM_DB;                      // Falling edge delay time
#endif

   // Set TZ   用于单独封锁EPwm
   EALLOW;
   // Init Trip Zone Select Register
   EPwm1Regs.TZSEL.all = 0x0000;            	// 禁用跳闸区域
   // Init Trip Zone Control Register
   EPwm1Regs.TZCTL.bit.TZA = TZ_FORCE_LO;   	// 强制拉低
   EPwm1Regs.TZCTL.bit.TZB = TZ_FORCE_LO;   	// 强制拉低
   EDIS;

   // 设置中断
   EPwm1Regs.ETSEL.bit.INTEN=1;             // 使能epwm1中断
   EPwm1Regs.ETSEL.bit.INTSEL= ET_CTR_PRD;  // 选择epwm1上溢中断
   EPwm1Regs.ETPS.bit.INTPRD = ET_1ST;      // Generate INT on 第一个 event!!!!!!!
   EPwm1Regs.ETCLR.bit.INT = 1;             // Enable more interrupts

   // 设置epwm启动adcA
   EPwm1Regs.ETSEL.bit.SOCAEN=0;            // 不允许epwm1事件开启adcA
   EPwm1Regs.ETSEL.bit.SOCASEL=0x1;         // 当epwm1的TB=0即下溢时启动adc
   EPwm1Regs.ETPS.bit.SOCAPRD=ET_1ST;       // 在第一个事件即启动ADC
   EPwm1Regs.ETCLR.bit.SOCA = 1;            // Clear SOCA flag
}

void ConfigEPwm2(void)
{
   // Setup TB
   EPwm2Regs.TBPRD = EPWM_PERIOD;  					// Set timer period
   EPwm2Regs.TBPHS.half.TBPHS = 0;            		// Phase is 0
   EPwm2Regs.TBCTR = 0x0000;                  		// Clear counter

   // Setup TBCTL
   EPwm2Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN; 	// up-down count mode
   EPwm2Regs.TBCTL.bit.PHSEN = TB_DISABLE;        	// 不使用移相方式
   EPwm2Regs.TBCTL.bit.PRDLD = TB_SHADOW;         	// 变化TBPRD时，使用shadow寄存器(即当计数器为0时才更新)
   EPwm2Regs.TBCTL.bit.SYNCOSEL = TB_SYNC_IN;     	// Setup Sync: pass through
   EPwm2Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;       	// Clock ratio to SYSCLKOUT
   EPwm2Regs.TBCTL.bit.CLKDIV = TB_DIV1;          	// 两个均直通，即计数器时钟为系统时钟
   EPwm2Regs.TBCTL.bit.PHSDIR = TB_UP;            	// Phase Direction Bit
   EPwm2Regs.TBCTL.bit.FREE_SOFT = 0;             	// Emulation Mode Bits: Free run

   // Setup CMPCTL
   EPwm2Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;		// CC_IMMEDIATE;// CC_SHADOW;    // Load registers every ZERO
   EPwm2Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;		// CC_IMMEDIATE;// CC_SHADOW;
#ifdef PWMMODE_BOOST
   EPwm2Regs.CMPCTL.bit.LOADAMODE = CC_CTR_PRD;		// Boost mode reload on top for symetrical waveform
   EPwm2Regs.CMPCTL.bit.LOADBMODE = CC_CTR_PRD;
#else
   EPwm2Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO;
   EPwm2Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;
#endif

   // Setup CMP
   EPwm2Regs.CMPA.half.CMPA = EPWM_CMP;
#ifdef PWMMODE_BOOST
   EPwm2Regs.CMPB = 0;					 //Constant off for boost mode
#endif

   // Set actions
   // Set PWM1A
   EPwm2Regs.AQCTLA.bit.CAU=AQ_CLEAR;    // 计数器(增加时)=比较器A时,输出low
   EPwm2Regs.AQCTLA.bit.CAD=AQ_SET;  	 // 计数器(减小时)=比较器A时,输出high
#ifdef PWMMODE_BOOST
   // Set PWM1B
   // PWMB is for boost mode, upper switch constant off
   EPwm2Regs.AQCTLB.bit.CBU=AQ_CLEAR;  	 // 计数器(增加时)=比较器A时,输出low
   EPwm2Regs.AQCTLB.bit.CBD=AQ_CLEAR;    // 计数器(减小时)=比较器A时,输出low
#endif

   // Set Deadband
   EPwm2Regs.DBCTL.bit.OUT_MODE = DB_FULL_ENABLE;  // 启用死区发生器
#ifdef PWMMODE_BOOST
   EPwm2Regs.DBCTL.bit.POLSEL = DB_ACTV_HI;        // No Inverting
   EPwm2Regs.DBCTL.bit.IN_MODE = DBB_RED_DBA_FED;  // Twist the AB channel
   EPwm2Regs.DBCTL.bit.OUT_MODE = DB_FULL_ENABLE;  // EPWMA feeded into EPWMB, EPWMB feeded into EPWMA
   EPwm2Regs.DBRED = 0;                 	   	   // Rising edge delay time zero
   EPwm2Regs.DBFED = 0;                      	   // Falling edge delay time zero
#else
   EPwm2Regs.DBCTL.bit.POLSEL = DB_ACTV_HIC;
   EPwm2Regs.DBCTL.bit.IN_MODE = DBA_ALL;		   // Use EPWMA to generate final output signal
   EPwm2Regs.DBCTL.bit.OUT_MODE = DB_FULL_ENABLE;
   EPwm2Regs.DBRED = EPWM_DB;                 	   // Rising edge delay time
   EPwm2Regs.DBFED = EPWM_DB;                      // Falling edge delay time
#endif

   // Set TZ   用于单独封锁EPwm
   EALLOW;
   // Init Trip Zone Select Register
   EPwm2Regs.TZSEL.all = 0x0000;            	// 禁用跳闸区域
   // Init Trip Zone Control Register
   EPwm2Regs.TZCTL.bit.TZA = TZ_FORCE_LO;   	// 强制拉低
   EPwm2Regs.TZCTL.bit.TZB = TZ_FORCE_LO;   	// 强制拉低
   EDIS;
}

void ConfigEPwm3(void)
{
   // Setup TB
   EPwm3Regs.TBPRD = EPWM_PERIOD;  					// Set timer period
   EPwm3Regs.TBPHS.half.TBPHS = 0;            		// Phase is 0
   EPwm3Regs.TBCTR = 0x0000;                  		// Clear counter

   // Setup TBCTL
   EPwm3Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN; 	// up-down count mode
   EPwm3Regs.TBCTL.bit.PHSEN = TB_DISABLE;        	// 不使用移相方式
   EPwm3Regs.TBCTL.bit.PRDLD = TB_SHADOW;         	// 变化TBPRD时，使用shadow寄存器(即当计数器为0时才更新)
   EPwm3Regs.TBCTL.bit.SYNCOSEL = TB_SYNC_IN;     	// Setup Sync: pass through
   EPwm3Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;       	// Clock ratio to SYSCLKOUT
   EPwm3Regs.TBCTL.bit.CLKDIV = TB_DIV1;          	// 两个均直通，即计数器时钟为系统时钟
   EPwm3Regs.TBCTL.bit.PHSDIR = TB_UP;            	// Phase Direction Bit
   EPwm3Regs.TBCTL.bit.FREE_SOFT = 0;             	// Emulation Mode Bits: Free run

   // Setup CMPCTL
   EPwm3Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;		// CC_IMMEDIATE;// CC_SHADOW;    // Load registers every ZERO
   EPwm3Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;		// CC_IMMEDIATE;// CC_SHADOW;
#ifdef PWMMODE_BOOST
   EPwm3Regs.CMPCTL.bit.LOADAMODE = CC_CTR_PRD;		// Boost mode reload on top for symetrical waveform
   EPwm3Regs.CMPCTL.bit.LOADBMODE = CC_CTR_PRD;
#else
   EPwm3Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO;
   EPwm3Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;
#endif

   // Setup CMP
   EPwm3Regs.CMPA.half.CMPA = EPWM_CMP;
#ifdef PWMMODE_BOOST
   EPwm3Regs.CMPB = 0;					 //Constant off for boost mode
#endif

   // Set actions
   // Set PWM1A
   EPwm3Regs.AQCTLA.bit.CAU=AQ_CLEAR;    // 计数器(增加时)=比较器A时,输出low
   EPwm3Regs.AQCTLA.bit.CAD=AQ_SET;  	 // 计数器(减小时)=比较器A时,输出high
#ifdef PWMMODE_BOOST
   // Set PWM1B
   // PWMB is for boost mode, upper switch constant off
   EPwm3Regs.AQCTLB.bit.CBU=AQ_CLEAR;  	 // 计数器(增加时)=比较器A时,输出low
   EPwm3Regs.AQCTLB.bit.CBD=AQ_CLEAR;    // 计数器(减小时)=比较器A时,输出low
#endif

   // Set Deadband
   EPwm3Regs.DBCTL.bit.OUT_MODE = DB_FULL_ENABLE;  // 启用死区发生器
#ifdef PWMMODE_BOOST
   EPwm3Regs.DBCTL.bit.POLSEL = DB_ACTV_HI;        // No Inverting
   EPwm3Regs.DBCTL.bit.IN_MODE = DBB_RED_DBA_FED;  // Twist the AB channel
   EPwm3Regs.DBCTL.bit.OUT_MODE = DB_FULL_ENABLE;  // EPWMA feeded into EPWMB, EPWMB feeded into EPWMA
   EPwm3Regs.DBRED = 0;                 	   	   // Rising edge delay time zero
   EPwm3Regs.DBFED = 0;                      	   // Falling edge delay time zero
#else
   EPwm3Regs.DBCTL.bit.POLSEL = DB_ACTV_HIC;
   EPwm3Regs.DBCTL.bit.IN_MODE = DBA_ALL;		   // Use EPWMA to generate final output signal
   EPwm3Regs.DBCTL.bit.OUT_MODE = DB_FULL_ENABLE;
   EPwm3Regs.DBRED = EPWM_DB;                 	   // Rising edge delay time
   EPwm3Regs.DBFED = EPWM_DB;                      // Falling edge delay time
#endif

   // Set TZ   用于单独封锁EPwm
   EALLOW;
   // Init Trip Zone Select Register
   EPwm3Regs.TZSEL.all = 0x0000;            	// 禁用跳闸区域
   // Init Trip Zone Control Register
   EPwm3Regs.TZCTL.bit.TZA = TZ_FORCE_LO;   	// 强制拉低
   EPwm3Regs.TZCTL.bit.TZB = TZ_FORCE_LO;   	// 强制拉低
   EDIS;
}

#ifndef DCCIRBRK
void ConfigEPwm4(void)
{
   // Setup TB
   EPwm4Regs.TBPRD = EPWM_PERIOD;  					// Set timer period
   EPwm4Regs.TBPHS.half.TBPHS = 0;            		// Phase is 0
   EPwm4Regs.TBCTR = 0x0000;                  		// Clear counter

   // Setup TBCTL
   EPwm4Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN; 	// up-down count mode
   EPwm4Regs.TBCTL.bit.PHSEN = TB_DISABLE;        	// 不使用移相方式
   EPwm4Regs.TBCTL.bit.PRDLD = TB_SHADOW;         	// 变化TBPRD时，使用shadow寄存器(即当计数器为0时才更新)
   EPwm4Regs.TBCTL.bit.SYNCOSEL = TB_SYNC_IN;     	// Setup Sync: pass through
   EPwm4Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;       	// Clock ratio to SYSCLKOUT
   EPwm4Regs.TBCTL.bit.CLKDIV = TB_DIV1;          	// 两个均直通，即计数器时钟为系统时钟
   EPwm4Regs.TBCTL.bit.PHSDIR = TB_UP;            	// Phase Direction Bit
   EPwm4Regs.TBCTL.bit.FREE_SOFT = 0;             	// Emulation Mode Bits: Free run

   // Setup CMPCTL
   EPwm4Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;		// CC_IMMEDIATE;// CC_SHADOW;    // Load registers every ZERO
   EPwm4Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;		// CC_IMMEDIATE;// CC_SHADOW;
#ifdef PWMMODE_BOOST
   EPwm4Regs.CMPCTL.bit.LOADAMODE = CC_CTR_PRD;		// Boost mode reload on top for symetrical waveform
   EPwm4Regs.CMPCTL.bit.LOADBMODE = CC_CTR_PRD;
#else
   EPwm4Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO;
   EPwm4Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;
#endif

   // Setup CMP
   EPwm4Regs.CMPA.half.CMPA = EPWM_CMP;
#ifdef PWMMODE_BOOST
   EPwm4Regs.CMPB = 0;					 //Constant off for boost mode
#endif

   // Set actions
   // Set PWM1A
   EPwm4Regs.AQCTLA.bit.CAU=AQ_CLEAR;    // 计数器(增加时)=比较器A时,输出low
   EPwm4Regs.AQCTLA.bit.CAD=AQ_SET;  	 // 计数器(减小时)=比较器A时,输出high
#ifdef PWMMODE_BOOST
   // Set PWM1B
   // PWMB is for boost mode, upper switch constant off
   EPwm4Regs.AQCTLB.bit.CBU=AQ_CLEAR;  	 // 计数器(增加时)=比较器A时,输出low
   EPwm4Regs.AQCTLB.bit.CBD=AQ_CLEAR;    // 计数器(减小时)=比较器A时,输出low
#endif

   // Set Deadband
   EPwm4Regs.DBCTL.bit.OUT_MODE = DB_FULL_ENABLE;  // 启用死区发生器
#ifdef PWMMODE_BOOST
   EPwm4Regs.DBCTL.bit.POLSEL = DB_ACTV_HI;        // No Inverting
   EPwm4Regs.DBCTL.bit.IN_MODE = DBB_RED_DBA_FED;  // Twist the AB channel
   EPwm4Regs.DBCTL.bit.OUT_MODE = DB_FULL_ENABLE;  // EPWMA feeded into EPWMB, EPWMB feeded into EPWMA
   EPwm4Regs.DBRED = 0;                 	   	   // Rising edge delay time zero
   EPwm4Regs.DBFED = 0;                      	   // Falling edge delay time zero
#else
   EPwm4Regs.DBCTL.bit.POLSEL = DB_ACTV_HIC;
   EPwm4Regs.DBCTL.bit.IN_MODE = DBA_ALL;		   // Use EPWMA to generate final output signal
   EPwm4Regs.DBCTL.bit.OUT_MODE = DB_FULL_ENABLE;
   EPwm4Regs.DBRED = EPWM_DB;                 	   // Rising edge delay time
   EPwm4Regs.DBFED = EPWM_DB;                      // Falling edge delay time
#endif

   // Set TZ   用于单独封锁EPwm
   EALLOW;
   // Init Trip Zone Select Register
   EPwm4Regs.TZSEL.all = 0x0000;            	// 禁用跳闸区域
   // Init Trip Zone Control Register
   EPwm4Regs.TZCTL.bit.TZA = TZ_FORCE_LO;   	// 强制拉低
   EPwm4Regs.TZCTL.bit.TZB = TZ_FORCE_LO;   	// 强制拉低
   EDIS;
}
#else
void ConfigEPwm4(void)
{
   // Setup TB
   EPwm4Regs.TBPRD = EPWM_PERIOD;  					// Set timer period
   EPwm4Regs.TBPHS.half.TBPHS = 0;            		// Phase is 0
   EPwm4Regs.TBCTR = 0x0000;                  		// Clear counter

   // Setup TBCTL
   EPwm4Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN; 	// up-down count mode
   EPwm4Regs.TBCTL.bit.PHSEN = TB_DISABLE;        	// 不使用移相方式
   EPwm4Regs.TBCTL.bit.PRDLD = TB_SHADOW;         	// 变化TBPRD时，使用shadow寄存器(即当计数器为0时才更新)
   EPwm4Regs.TBCTL.bit.SYNCOSEL = TB_SYNC_IN;     	// Setup Sync: pass through
   EPwm4Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;       	// Clock ratio to SYSCLKOUT
   EPwm4Regs.TBCTL.bit.CLKDIV = TB_DIV1;          	// 两个均直通，即计数器时钟为系统时钟
   EPwm4Regs.TBCTL.bit.PHSDIR = TB_UP;            	// Phase Direction Bit
   EPwm4Regs.TBCTL.bit.FREE_SOFT = 0;             	// Emulation Mode Bits: Free run

   // Setup CMPCTL
   EPwm4Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;		// CC_SHADOW;
   EPwm4Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;		// CC_SHADOW;
   EPwm4Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO;    // Load registers every ZERO
   EPwm4Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;    // Load registers every ZERO

   // Setup CMP
   EPwm4Regs.CMPA.half.CMPA = EPWM_CMP;

   // Set actions
   // in circuit breaker mode, both switch is constant off
   // Set PWM1A
   EPwm4Regs.AQCTLA.bit.PRD=AQ_CLEAR;
   EPwm4Regs.AQCTLA.bit.ZRO=AQ_CLEAR;
   EPwm4Regs.AQCTLA.bit.CAU=AQ_CLEAR;    // 计数器(增加时)=比较器A时,输出low
   EPwm4Regs.AQCTLA.bit.CAD=AQ_CLEAR;  	 // 计数器(减小时)=比较器A时,输出low
   // Set PWM1B
   EPwm4Regs.AQCTLB.bit.PRD=AQ_CLEAR;
   EPwm4Regs.AQCTLB.bit.ZRO=AQ_CLEAR;
   EPwm4Regs.AQCTLB.bit.CAU=AQ_CLEAR;  	 // 计数器(增加时)=比较器A时,输出low
   EPwm4Regs.AQCTLB.bit.CAD=AQ_CLEAR;    // 计数器(减小时)=比较器A时,输出low
   EPwm4Regs.AQSFRC.bit.RLDCSF=  0x3;	 // Force load immediately

   // Set Deadband
   EPwm4Regs.DBCTL.bit.OUT_MODE = DB_DISABLE;  // 禁用死区发生器

   // Set TZ
   EALLOW;
   EPwm4Regs.TZSEL.all = 0x0000;            	// 禁用跳闸区域
   EPwm4Regs.TZCTL.bit.TZA = TZ_NO_CHANGE;   	// 不做变化
   EPwm4Regs.TZCTL.bit.TZB = TZ_NO_CHANGE;   	// 不做变化
   EDIS;
}
#endif

void ConfigEPwm(void)
{
   // epwm时钟为系统时钟
   EALLOW;
   SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 0;   // 启用epwm时钟同步，可以使所有epwm模块完美同步，不过必须按一定步骤，要先不启用，然后将每个epwm模块的tbctl配置成一致
   EDIS;

   // 为了同步，必须将每个epwm模块的tbctl配置成一致
   // 主控dsp中，epwm的TB配置为控制系统时钟基准
   ConfigEPwm1();
   //ConfigEPwm2();
   //ConfigEPwm3();
   //ConfigEPwm4();

   EALLOW;
   SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 1;   // 最后启用epwm时钟同步
   EDIS;

}

//===========================================================================
// End of file.
//===========================================================================
