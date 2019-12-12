// TI File $Revision: /main/1 $
// Checkin $Date: August 18, 2006   13:46:25 $
//###########################################################################
//
// FILE:	DSP2833x_Gpio.c
//
// TITLE:	DSP2833x General Purpose I/O Initialization & Support Functions.
//
//###########################################################################
// $TI Release: DSP2833x/DSP2823x C/C++ Header Files V1.31 $
// $Release Date: August 4, 2009 $
//###########################################################################

#include "DSP2833x_Device.h"     // DSP2833x Headerfile Include File
#include "DSP2833x_Examples.h"   // DSP2833x Examples Include File

//---------------------------------------------------------------------------
// InitGpio: 
//---------------------------------------------------------------------------
// This function initializes the Gpio to a known (default) state.
//
// For more details on configuring GPIO's as peripheral functions,
// refer to the individual peripheral examples and/or GPIO setup example. 
void InitGpio(void)
{
   EALLOW;
   
   // Each GPIO pin can be: 
   // a) a GPIO input/output
   // b) peripheral function 1
   // c) peripheral function 2
   // d) peripheral function 3
   // By default, all are GPIO Inputs 
   GpioCtrlRegs.GPAMUX1.all = 0x0000;     // GPIO functionality GPIO0-GPIO15
   GpioCtrlRegs.GPAMUX2.all = 0x0000;     // GPIO functionality GPIO16-GPIO31
   GpioCtrlRegs.GPBMUX1.all = 0x0000;     // GPIO functionality GPIO32-GPIO39
   GpioCtrlRegs.GPBMUX2.all = 0x0000;     // GPIO functionality GPIO48-GPIO63
   GpioCtrlRegs.GPCMUX1.all = 0x0000;     // GPIO functionality GPIO64-GPIO79
   GpioCtrlRegs.GPCMUX2.all = 0x0000;     // GPIO functionality GPIO80-GPIO95

   GpioCtrlRegs.GPADIR.all = 0x0000;      // GPIO0-GPIO31 are inputs
   GpioCtrlRegs.GPBDIR.all = 0x0000;      // GPIO32-GPIO63 are inputs   
   GpioCtrlRegs.GPCDIR.all = 0x0000;      // GPI064-GPIO95 are inputs

   // Each input can have different qualification
   // a) input synchronized to SYSCLKOUT
   // b) input qualified by a sampling window
   // c) input sent asynchronously (valid for peripheral inputs only)
   GpioCtrlRegs.GPAQSEL1.all = 0x0000;    // GPIO0-GPIO15 Synch to SYSCLKOUT 
   GpioCtrlRegs.GPAQSEL2.all = 0x0000;    // GPIO16-GPIO31 Synch to SYSCLKOUT
   GpioCtrlRegs.GPBQSEL1.all = 0x0000;    // GPIO32-GPIO39 Synch to SYSCLKOUT 
   GpioCtrlRegs.GPBQSEL2.all = 0x0000;    // GPIO48-GPIO63 Synch to SYSCLKOUT 

   // Pull-ups can be enabled or disabled. 
   GpioCtrlRegs.GPAPUD.all = 0x0000;      // Pullup's enabled GPIO0-GPIO31
   GpioCtrlRegs.GPBPUD.all = 0x0000;      // Pullup's enabled GPIO32-GPIO63
   GpioCtrlRegs.GPCPUD.all = 0x0000;      // Pullup's enabled GPIO64-GPIO79

   //GpioCtrlRegs.GPAPUD.all = 0xFFFF;    // Pullup's disabled GPIO0-GPIO31
   //GpioCtrlRegs.GPBPUD.all = 0xFFFF;    // Pullup's disabled GPIO32-GPIO34
   //GpioCtrlRegs.GPCPUD.all = 0xFFFF     // Pullup's disabled GPIO64-GPIO79

   //=======================================
   //The following code are added by GuYunjie
   //Control IO configurations
   //Output
   GpioCtrlRegs.GPADIR.bit.GPIO12=1;
   GpioCtrlRegs.GPADIR.bit.GPIO13=1;
   GpioCtrlRegs.GPADIR.bit.GPIO14=1;
   GpioCtrlRegs.GPADIR.bit.GPIO15=1;
   GpioCtrlRegs.GPADIR.bit.GPIO16=1;
   GpioCtrlRegs.GPADIR.bit.GPIO17=1;
   //Output all set to 1
   GpioDataRegs.GPASET.bit.GPIO12=1;
   GpioDataRegs.GPASET.bit.GPIO13=1;
   GpioDataRegs.GPASET.bit.GPIO14=1;
   GpioDataRegs.GPASET.bit.GPIO15=1;
   GpioDataRegs.GPASET.bit.GPIO16=1;
   GpioDataRegs.GPASET.bit.GPIO17=1;

   //Isolated Output
   GpioCtrlRegs.GPBDIR.bit.GPIO48=1;
   GpioCtrlRegs.GPBDIR.bit.GPIO49=1;
   GpioCtrlRegs.GPBDIR.bit.GPIO50=1;
   GpioCtrlRegs.GPBDIR.bit.GPIO51=1;
   //Isolated Output Set to 1
   GpioDataRegs.GPBSET.bit.GPIO48=1;
   GpioDataRegs.GPBSET.bit.GPIO49=1;
   GpioDataRegs.GPBSET.bit.GPIO50=1;
   GpioDataRegs.GPBSET.bit.GPIO51=1;

   //Input
   GpioCtrlRegs.GPADIR.bit.GPIO8=0;   	//F0
   GpioCtrlRegs.GPADIR.bit.GPIO9=0;   	//F1
   GpioCtrlRegs.GPADIR.bit.GPIO10=0;   	//F2
   //Input Qualifier for GPIO8~GPIO10
   GpioCtrlRegs.GPACTRL.bit.QUALPRD2=0x5;  //鉴定周期:10ns*5*2
   GpioCtrlRegs.GPAQSEL1.bit.GPIO8 = 2;	   //使能6周期鉴定
   GpioCtrlRegs.GPAQSEL1.bit.GPIO9 = 2;
   GpioCtrlRegs.GPAQSEL1.bit.GPIO10 = 2;

   //Isolated Input
   GpioCtrlRegs.GPBDIR.bit.GPIO52=0;
   GpioCtrlRegs.GPBDIR.bit.GPIO53=0;
   GpioCtrlRegs.GPBDIR.bit.GPIO54=0;
   GpioCtrlRegs.GPBDIR.bit.GPIO55=0;
   //Input Qualifier for GPIO52~GPIO55
   GpioCtrlRegs.GPBCTRL.bit.QUALPRD2=0x5;   //鉴定周期:10ns*5*2
   GpioCtrlRegs.GPBQSEL2.bit.GPIO52 = 2;	//使能6周期鉴定
   GpioCtrlRegs.GPBQSEL2.bit.GPIO53 = 2;
   GpioCtrlRegs.GPBQSEL2.bit.GPIO54 = 2;
   GpioCtrlRegs.GPBQSEL2.bit.GPIO55 = 2;

   //End of the code adde by GuYunjie
   //=======================================

   EDIS;

}	
	
//===========================================================================
// End of file.
//===========================================================================
