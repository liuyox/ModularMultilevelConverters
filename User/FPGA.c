#include "DSP2833x_Device.h"
#include "DSP2833x_Examples.h"
#include "ParaCalc.h"
#include "FPGA.h"
#include "main.h"
//---------------------------------------------------------------------------
//External References & Function Declarations:
//========================Control Registers========================
/*volatile struct FPGA_SCI_REGS        FPGASciRegs;   //SCI Format Control Regs
volatile struct FPGA_SORT_REGS    	UPSortRegs;    //
volatile struct FPGA_SORT_REGS    	UNSortRegs;
volatile struct FPGA_SORT_REGS    	VPSortRegs;
volatile struct FPGA_SORT_REGS    	VNSortRegs;
volatile struct FPGA_SORT_REGS    	WPSortRegs;
volatile struct FPGA_SORT_REGS    	WNSortRegs;

//====================Data & Status Registers=======================
volatile struct FPGA_SAMPLEA_REGS 	SampARegs;
volatile struct FPGA_SAMPLEB_REGS 	SampBRegs;

volatile struct FPGA_ARMST_REGS       UPARMSTRegs;      //SM Status Regs(Recieve, Err, Finish)
volatile struct FPGA_ARMST_REGS       UNARMSTRegs;      //SM Status Regs
volatile struct FPGA_ARMST_REGS       VPARMSTRegs;      //SM Status Regs
volatile struct FPGA_ARMST_REGS       VNARMSTRegs;      //SM Status Regs
volatile struct FPGA_ARMST_REGS       WPARMSTRegs;      //SM Status Regs
volatile struct FPGA_ARMST_REGS       WNARMSTRegs;      //SM Status Regs

volatile struct FPGA_SMDATA1_REGS    UPSMData1Regs;      //SM Status Reg(SM1~SM6)
volatile struct FPGA_SMDATA1_REGS    UNSMData1Regs;      //SM Status Reg(SM1~SM6)
volatile struct FPGA_SMDATA1_REGS    VPSMData1Regs;      //SM Status Reg(SM1~SM6)
volatile struct FPGA_SMDATA1_REGS    VNSMData1Regs;      //SM Status Reg(SM1~SM6)
volatile struct FPGA_SMDATA1_REGS    WPSMData1Regs;      //SM Status Reg(SM1~SM6)
volatile struct FPGA_SMDATA1_REGS    WNSMData1Regs;      //SM Status Reg(SM1~SM6)

volatile struct FPGA_SMDATA2_REGS    UPSMData2Regs;      //SM Status Reg(SM7~SM12)
volatile struct FPGA_SMDATA2_REGS    UNSMData2Regs;      //SM Status Reg(SM7~SM12)
volatile struct FPGA_SMDATA2_REGS    VPSMData2Regs;      //SM Status Reg(SM7~SM12)
volatile struct FPGA_SMDATA2_REGS    VNSMData2Regs;      //SM Status Reg(SM7~SM12)
volatile struct FPGA_SMDATA2_REGS    WPSMData2Regs;      //SM Status Reg(SM7~SM12)
volatile struct FPGA_SMDATA2_REGS    WNSMData2Regs;*/      //SM Status Reg(SM7~SM12)


//=========================register Definition======================*/

#pragma DATA_SECTION(FPGASciRegs,"FPGASciRegsFile");
volatile struct FPGA_SCI_REGS FPGASciRegs;

#pragma DATA_SECTION(SampARegs,"SampARegsFile");
volatile struct FPGA_SAMPLEA_REGS SampARegs;

#pragma DATA_SECTION(SampBRegs,"SampBRegsFile");
volatile struct FPGA_SAMPLEB_REGS SampBRegs;

//***********************UP Regs************************************/
#pragma DATA_SECTION(UPARMSTRegs,"UPARMSTRegsFile");
volatile struct FPGA_ARMST_REGS    UPARMSTRegs;      //ARM Status Reg


#pragma DATA_SECTION(UPSortRegs,"UPSortRegsFile");
volatile struct FPGA_SORT_REGS    UPSortRegs;

//***********************UN Regs************************************/
#pragma DATA_SECTION(UNARMSTRegs,"UNARMSTRegsFile");
volatile struct FPGA_ARMST_REGS    UNARMSTRegs;      //ARM Status Reg



#pragma DATA_SECTION(UNSortRegs,"UNSortRegsFile");
volatile struct FPGA_SORT_REGS    UNSortRegs;

//***********************VP Regs************************************/
#pragma DATA_SECTION(VPARMSTRegs,"VPARMSTRegsFile");
volatile struct FPGA_ARMST_REGS    VPARMSTRegs;      //ARM Status Reg

#pragma DATA_SECTION(VPSortRegs,"VPSortRegsFile");
volatile struct FPGA_SORT_REGS    VPSortRegs;

//***********************VN Regs************************************/

#pragma DATA_SECTION(VNARMSTRegs,"VNARMSTRegsFile");
volatile struct FPGA_ARMST_REGS    VNARMSTRegs;      //ARM Status Reg

#pragma DATA_SECTION(VNSortRegs,"VNSortRegsFile");
volatile struct FPGA_SORT_REGS    VNSortRegs;


//***********************WP Regs************************************/
#pragma DATA_SECTION(WPARMSTRegs,"WPARMSTRegsFile");
volatile struct FPGA_ARMST_REGS    WPARMSTRegs;      //ARM Status Reg

#pragma DATA_SECTION(WPSortRegs,"WPSortRegsFile");
volatile struct FPGA_SORT_REGS    WPSortRegs;

//***********************WN Regs************************************/
#pragma DATA_SECTION(WNARMSTRegs,"WNARMSTRegsFile");
volatile struct FPGA_ARMST_REGS    WNARMSTRegs;      //ARM Status Reg

#pragma DATA_SECTION(WNSortRegs,"WNSortRegsFile");
volatile struct FPGA_SORT_REGS    WNSortRegs;


Uint16 *UPSMSTRAddr_gusv;
Uint16 *UNSMSTRAddr_gusv;
Uint16 *VPSMSTRAddr_gusv;
Uint16 *VNSMSTRAddr_gusv;
Uint16 *WPSMSTRAddr_gusv;
Uint16 *WNSMSTRAddr_gusv;
Uint16* Addr_offset = ( Uint16* ) 0x0200000;
//=========================================================================
//For debug use only:
volatile struct FPGA_ARMST_REGS    UPARMSTRegsShadow;      //ARM Status Reg
volatile struct FPGA_ARMST_REGS    UNARMSTRegsShadow;      //ARM Status Reg
volatile struct FPGA_ARMST_REGS    VPARMSTRegsShadow;      //ARM Status Reg
volatile struct FPGA_ARMST_REGS    VNARMSTRegsShadow;      //ARM Status Reg
volatile struct FPGA_ARMST_REGS    WPARMSTRegsShadow;      //ARM Status Reg
volatile struct FPGA_ARMST_REGS    WNARMSTRegsShadow;      //ARM Status Reg
volatile struct FPGA_SORT_REGS     UPSortRegsShadow;
volatile struct FPGA_SORT_REGS     UNSortRegsShadow;
volatile struct FPGA_SORT_REGS     VPSortRegsShadow;
volatile struct FPGA_SORT_REGS     VNSortRegsShadow;
volatile struct FPGA_SORT_REGS     WPSortRegsShadow;
volatile struct FPGA_SORT_REGS     WNSortRegsShadow;
//==========================================================================
void InitFPGA(void)
{
	//wait 1s for FPGA get started
	DELAY_US(1000000);
	//Set Reset, Sync and INT pins
	InitFPGAGPIO();
	//Init Zone7   in LAN init function
//	InitZone7();
	//Reset FPGA
	FPGAReset();
	//Initialize FPGARegs
	FPGARegsInit();
}

void FPGAReset()
{
	int16 i;
	GpioDataRegs.GPBCLEAR.bit.GPIO63=1; //RST0
	GpioDataRegs.GPBCLEAR.bit.GPIO61=1; //RST1
	GpioDataRegs.GPBCLEAR.bit.GPIO59=1; //RST2
	for(i=1;i<1000;i++)    //wait 6.67 us for FPGA Reset
		asm("       NOP");
	GpioDataRegs.GPBSET.bit.GPIO63=1;
	GpioDataRegs.GPBSET.bit.GPIO61=1;
	GpioDataRegs.GPBSET.bit.GPIO59=1;
	for(i=1;i<1000;i++)   //wait 6.67 us for FPGARegs Reset
		asm("       NOP");
}

void FPGARegsInit(void)
{
	//Initialize FPGA SCI Regs
	FPGASciRegs.LCR.bit.WLS = 3;  //  8 data bit
	FPGASciRegs.LCR.bit.STB = 0;  //  1 stop bit
	FPGASciRegs.LCR.bit.PE = 1;   //  enable priority check
	FPGASciRegs.LCR.bit.EPS = 0;  //  odd priority check
	FPGASciRegs.BRR = 0x0000; // SCI Baud rate=37.5MHz/(16*(BRR+1))
	//Initialize FPGA Sort Regs
	ConfigSortRegs();
}

void InitZone7(void)
{

    // Make sure the XINTF clock is enabled
	SysCtrlRegs.PCLKCR3.bit.XINTFENCLK = 1;

	// Configure the GPIO for XINTF with a 16-bit data bus
	// This function is in DSP2833x_Xintf.c
	InitXintf16Gpio();

    EALLOW;
    // All Zones---------------------------------
    // Timing for all zones based on XTIMCLK = SYSCLKOUT/2
    XintfRegs.XINTCNF2.bit.XTIMCLK = 1;//1,actual situation
    // Buffer up to 3 writes
    XintfRegs.XINTCNF2.bit.WRBUFF = 3;	//3
    // XCLKOUT is enabled
    XintfRegs.XINTCNF2.bit.CLKOFF = 0;
    // XCLKOUT = XTIMCLK
    XintfRegs.XINTCNF2.bit.CLKMODE = 1;//1,actual situation

    // Zone 7------------------------------------
    // When using ready, ACTIVE must be 1 or greater
    // Lead must always be 1 or greater
    // Zone write timing
    XintfRegs.XTIMING7.bit.XWRLEAD = 1;//3,default,also actual situation
    XintfRegs.XTIMING7.bit.XWRACTIVE = 3;//7,ditto
    XintfRegs.XTIMING7.bit.XWRTRAIL = 1;//3,ditto
    // Zone read timing
    XintfRegs.XTIMING7.bit.XRDLEAD = 1;//3,ditto
    XintfRegs.XTIMING7.bit.XRDACTIVE = 3;//7,ditto
    XintfRegs.XTIMING7.bit.XRDTRAIL = 1;//3,ditto

    // don't double all Zone read/write lead/active/trail timing
    XintfRegs.XTIMING7.bit.X2TIMING = 0;//1,the number of wait states are doubled,which is default but also actual situation

    // Zone will not sample XREADY signal
    XintfRegs.XTIMING7.bit.USEREADY = 0;
    XintfRegs.XTIMING7.bit.READYMODE = 0;

    // 1,1 = x16 data bus
    // 0,1 = x32 data bus
    // other values are reserved
    XintfRegs.XTIMING7.bit.XSIZE = 3;
    EDIS;

   //Force a pipeline flush to ensure that the write to
   //the last register configured occurs before returning.
   asm(" RPT #7 || NOP");
}
void InitFPGAGPIO(void)
{
	 EALLOW;

	 /* disable internal pull-up to output*/
     GpioCtrlRegs.GPBPUD.bit.GPIO63 = 1;    // RST0
	 GpioCtrlRegs.GPBPUD.bit.GPIO61 = 1;    // RST1
	 GpioCtrlRegs.GPBPUD.bit.GPIO59 = 1;    // RST2

	 /* Configure RST pins as output */
	 GpioCtrlRegs.GPBDIR.bit.GPIO63 = 1;    // output
	 GpioCtrlRegs.GPBDIR.bit.GPIO61 = 1;
	 GpioCtrlRegs.GPBDIR.bit.GPIO59 = 1;

	 /* enable internal pull-up to input*/
     GpioCtrlRegs.GPBPUD.bit.GPIO62 = 0;    // INT0
	 GpioCtrlRegs.GPBPUD.bit.GPIO60 = 0;    // INT1
	 GpioCtrlRegs.GPBPUD.bit.GPIO58 = 0;    // INT2

	 /* Configure INT pins as Input */
	 GpioCtrlRegs.GPBDIR.bit.GPIO62 = 0;    // input
	 GpioCtrlRegs.GPBDIR.bit.GPIO60 = 0;
	 GpioCtrlRegs.GPBDIR.bit.GPIO58 = 0;

	 /* Configure GPIO6 as EPWMSY   */
	 GpioCtrlRegs.GPAPUD.bit.GPIO6 = 0;
	 GpioCtrlRegs.GPAMUX1.bit.GPIO6 = 3;
	 EDIS;
}

void ConfigSortRegs(void)
{
	ConfigUPSort();
	ConfigUNSort();
	ConfigVPSort();
	ConfigVNSort();
	ConfigWPSort();
	ConfigWNSort();
}


void ConfigUPSort(void)
{
	//Upper arm of phase U
	UPSortRegs.MCR.bit.SCIRxMode = 0; 		//Rx used as MMC mode
	UPSortRegs.MCR.bit.SCITxMode = 0; 		//Tx used as MMC mode
	//debug
	UPSortRegs.MCR.bit.SMNUM = NumTotal_gulv; 	//Active SM Number
	UPSortRegs.MCR.bit.ENSORT = 1;    		//open sort
	UPSortRegs.MCR.bit.SMTYPE = SMTYPE_gulv;//SMTYPE=0, all HBSMs; SMTYPE=1, all FBSMs

	UPSortRegs.TBPRD = FPGA_PERIOD;   		//Set FPGA TB period
	UPSortRegs.SMNUMCONFIG.bit.SORTNUM= NumSort_gulv; //Sort SM number
	UPSortRegs.SMNUMCONFIG.bit.HBSMNUM= NumHBSM_gulv; //Sort SM number

	UPSortRegs.DRVCMD = 0x0000;       		//clear cmd frame
	UPSortRegs.SMNR.bit.SMINNUM = NumTotal_gulv;//insert all sms in one arm

	/*******************ADD FOR PWM MODULATION*******************/
	UPSortRegs.MCR.bit.MODULMOD =FPGA_MODULMOD; //0-NLM, 1-level shift PWM modulation
	UPSortRegs.CPRDR = FPGA_CPRDR;   		//Set Carrier Period
	UPSortRegs.CMPMAXR = FPGA_CPRDR - 200; 	//Set Max Value of Carrier compare, Time 10us
	UPSortRegs.CMPMINR = 200; 				//Set Min Value of Carrier compare, Time 10us
	UPSortRegs.CMPLR = (Uint16)(NumNorm_gulv*3750);			//Modulation Wave, Low 16 Bit
	UPSortRegs.CMPHR = (Uint16)((NumNorm_gulv*3750)>>16);	//Modulation Wave, High 16 Bit

	UPSortRegs.MCR.bit.ENTB= 1;       		//Enable TB
}

void ConfigUNSort(void)
{
	//Lower arm of phase U
	UNSortRegs.MCR.bit.SCIRxMode = 0; 		//Rx used as MMC mode
	UNSortRegs.MCR.bit.SCITxMode = 0; 		//Rx used as MMC mode
	//debug
	UNSortRegs.MCR.bit.SMNUM = NumTotal_gulv; 	//Active SM Number
	UNSortRegs.MCR.bit.ENSORT = 1;    		//close sort
	UNSortRegs.MCR.bit.SMTYPE = SMTYPE_gulv;//SMTYPE=0, all HBSMs; SMTYPE=1, all FBSMs

	UNSortRegs.TBPRD = FPGA_PERIOD;   		//Set FPGA TB period
	UNSortRegs.SMNUMCONFIG.bit.SORTNUM = NumSort_gulv;
	UNSortRegs.SMNUMCONFIG.bit.HBSMNUM= NumHBSM_gulv; //set HBSM number in hybrid MMC

	UNSortRegs.DRVCMD = 0x0000;       		//clear cmd frame
	UNSortRegs.SMNR.bit.SMINNUM = NumTotal_gulv;//insert all sms in one arm

	UNSortRegs.MCR.bit.MODULMOD =FPGA_MODULMOD; //0-NLM, 1-DP
	UNSortRegs.CPRDR = FPGA_CPRDR;   		//Set Carrier Period
	UNSortRegs.CMPMAXR = FPGA_CPRDR - 200; 	//Set Max Value of Carrier compare, Time 10us
	UNSortRegs.CMPMINR = 200; 				//Set Min Value of Carrier compare, Time 10us
	UNSortRegs.CMPLR = (Uint16)(NumNorm_gulv*3750);    		//Modulation Wave, Low 16 Bit
	UNSortRegs.CMPHR = (Uint16)((NumNorm_gulv*3750)>>16);	//Modulation Wave, High 16 Bit

	UNSortRegs.MCR.bit.ENTB= 1;       		//enable TB
}

void ConfigVPSort(void)
{
	//Upper arm of phase V
	VPSortRegs.MCR.bit.SCIRxMode = 0; 		//Rx used as MMC mode
	VPSortRegs.MCR.bit.SCITxMode = 0; 		//Rx used as MMC mode
	VPSortRegs.MCR.bit.SMNUM = NumTotal_gulv; 	//Active SM Number
	VPSortRegs.MCR.bit.ENSORT = 1;    		//close sort
	VPSortRegs.MCR.bit.SMTYPE = SMTYPE_gulv;//SMTYPE=0, all HBSMs; SMTYPE=1, all FBSMs

	VPSortRegs.TBPRD = FPGA_PERIOD;   		//Set FPGA TB period
	VPSortRegs.SMNUMCONFIG.bit.SORTNUM = NumSort_gulv;
	VPSortRegs.SMNUMCONFIG.bit.HBSMNUM= NumHBSM_gulv;  //set HBSM number in hybrid MMC

	VPSortRegs.DRVCMD = 0x0000;       		//clear cmd frame
	VPSortRegs.SMNR.bit.SMINNUM = NumTotal_gulv;//insert all sms in one arm

	VPSortRegs.MCR.bit.MODULMOD =FPGA_MODULMOD; //0-NLM, 1-DP
	VPSortRegs.CPRDR = FPGA_CPRDR;   		//Set Carrier Period
	VPSortRegs.CMPMAXR = FPGA_CPRDR - 200; 	//Set Max Value of Carrier compare, Time 10us
	VPSortRegs.CMPMINR = 200; 				//Set Min Value of Carrier compare, Time 10us
	VPSortRegs.CMPLR = (Uint16)(NumNorm_gulv*3750);   		//Modulation Wave, Low 16 Bit
	VPSortRegs.CMPHR = (Uint16)((NumNorm_gulv*3750)>>16);	//Modulation Wave, High 16 Bit

	VPSortRegs.MCR.bit.ENTB= 1;       		//enable TB
}

void ConfigVNSort(void)
{
	//Lower arm of phase U
	VNSortRegs.MCR.bit.SCIRxMode = 0; 		//Rx used as MMC mode
	VNSortRegs.MCR.bit.SCITxMode = 0; 		//Rx used as MMC mode
	VNSortRegs.MCR.bit.SMNUM = NumTotal_gulv; 	//Active SM Number
	VNSortRegs.MCR.bit.ENSORT = 1;    		//close sort
	VNSortRegs.MCR.bit.SMTYPE = SMTYPE_gulv;//SMTYPE=0, all HBSMs; SMTYPE=1, all FBSMs

	VNSortRegs.TBPRD = FPGA_PERIOD;   		//Set FPGA TB period
	VNSortRegs.SMNUMCONFIG.bit.SORTNUM = NumSort_gulv;
	VNSortRegs.SMNUMCONFIG.bit.HBSMNUM= NumHBSM_gulv; //set HBSM number in hybrid MMC

	VNSortRegs.DRVCMD = 0x0000;       		//clear cmd frame
	VNSortRegs.SMNR.bit.SMINNUM = NumTotal_gulv;//insert all sms in one arm

	VNSortRegs.MCR.bit.MODULMOD =FPGA_MODULMOD; //0-NLM, 1-DP
	VNSortRegs.CPRDR = FPGA_CPRDR;   		//Set Carrier Period
	VNSortRegs.CMPMAXR = FPGA_CPRDR - 200; 	//Set Max Value of Carrier compare, Time 10us
	VNSortRegs.CMPMINR = 200; 				//Set Min Value of Carrier compare, Time 10us
	VNSortRegs.CMPLR = (Uint16)(NumNorm_gulv*3750);   		//Modulation Wave, Low 16 Bit
	VNSortRegs.CMPHR = (Uint16)((NumNorm_gulv*3750)>>16);   //Modulation Wave, High 16 Bit

	VNSortRegs.MCR.bit.ENTB= 1;       		//enable TB
}

void ConfigWPSort(void)
{
	//Upper arm of phase V
	WPSortRegs.MCR.bit.SCIRxMode = 0; 		//Rx used as MMC mode
	WPSortRegs.MCR.bit.SCITxMode = 0; 		//Rx used as MMC mode
	WPSortRegs.MCR.bit.SMNUM = NumTotal_gulv; 	//Active SM Number
	WPSortRegs.MCR.bit.ENSORT = 1;    		//close sort
	WPSortRegs.MCR.bit.SMTYPE = SMTYPE_gulv;//SMTYPE=0, all HBSMs; SMTYPE=1, all FBSMs

	WPSortRegs.TBPRD = FPGA_PERIOD;   		//Set FPGA TB period
	WPSortRegs.SMNUMCONFIG.bit.SORTNUM = NumSort_gulv;
	WPSortRegs.SMNUMCONFIG.bit.HBSMNUM= NumHBSM_gulv; //set HBSM number in hybrid MMC

	WPSortRegs.DRVCMD = 0x0000;       		//clear cmd frame
	WPSortRegs.SMNR.bit.SMINNUM = NumTotal_gulv;//insert all sms in one arm

	WPSortRegs.MCR.bit.MODULMOD =FPGA_MODULMOD; //0-NLM, 1-DP
	WPSortRegs.CPRDR = FPGA_CPRDR;  		//Set Carrier Period
	WPSortRegs.CMPMAXR = FPGA_CPRDR - 200; 	//Set Max Value of Carrier compare, Time 10us
	WPSortRegs.CMPMINR = 200; 				//Set Min Value of Carrier compare, Time 10us
	WPSortRegs.CMPLR = (Uint16)(NumNorm_gulv*3750); 		//Modulation Wave, Low 16 Bit
	WPSortRegs.CMPHR = (Uint16)((NumNorm_gulv*3750)>>16);	//Modulation Wave, High 16 Bit

	WPSortRegs.MCR.bit.ENTB= 1;       		//enable TB
}

void ConfigWNSort(void)
{
	//Lower arm of phase U
	WNSortRegs.MCR.bit.SCIRxMode = 0; 		//Rx used as MMC mode
	WNSortRegs.MCR.bit.SCITxMode = 0; 		//Rx used as MMC mode
	WNSortRegs.MCR.bit.SMNUM = NumTotal_gulv; 	//Active SM Number
	WNSortRegs.MCR.bit.ENSORT = 1;   		 //close sort
	WNSortRegs.MCR.bit.SMTYPE = SMTYPE_gulv;//SMTYPE=0, all HBSMs; SMTYPE=1, all FBSMs

	WNSortRegs.TBPRD = FPGA_PERIOD;   		//Set FPGA TB period
	WNSortRegs.SMNUMCONFIG.bit.SORTNUM = NumSort_gulv;
	WNSortRegs.SMNUMCONFIG.bit.HBSMNUM= NumHBSM_gulv; //set HBSM number in hybrid MMC

	WNSortRegs.DRVCMD = 0x0000;       		//clear cmd frame
	WNSortRegs.SMNR.bit.SMINNUM = NumTotal_gulv;//insert all sms in one arm

	WNSortRegs.MCR.bit.MODULMOD =FPGA_MODULMOD; //0-NLM, 1-DP
	WNSortRegs.CPRDR = FPGA_CPRDR;   		//Set Carrier Period
	WNSortRegs.CMPMAXR = FPGA_CPRDR - 200; 	//Set Max Value of Carrier compare, Time 10us
	WNSortRegs.CMPMINR = 200; 				//Set Min Value of Carrier compare, Time 10us
	WNSortRegs.CMPLR = (Uint16)(NumNorm_gulv*3750);   		//Modulation Wave, Low 16 Bit
	WNSortRegs.CMPHR = (Uint16)((NumNorm_gulv*3750)>>16);	//Modulation Wave, High 16 Bit

	WNSortRegs.MCR.bit.ENTB= 1;      		//enable TB
}

Uint16 ReceiveData_FPGA(Uint16 Addr)  //Addr is the Address of FPGA  registers
{
	return (*(Addr_offset + Addr));
}
