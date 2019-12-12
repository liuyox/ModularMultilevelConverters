/*
 * Comm485.h
 *
 *  Created on: 2018-7-13
 *      Author: LiuYongxia
 */

#ifndef FPGA485_H_
#define FPGA485_H_

//#define BRR 0x0000  //define the baud rate here
                    // baud rate=MCLK/(16*(BRR+1))  MCLK(37.5MHz) is the Working Frequency of FPGA

/*=================FPGA Register===================================*/
struct LCR_BITS {    // bits  description
   Uint16 WLS:2;      // 1:0   Word Length Select
                      //       00 --5 data 01--6 data 10--7data 11--8 data
   Uint16 STB:1;      // 2     stop bit length  0--1 stop
   Uint16 PE:1;       // 3     priority enable  1--enable
   Uint16 EPS:1;      // 4     Even/odd priority selected  1--even priority
   Uint16 rsvd:11;    // 15:7    reserved
};
union LCR_REG {
   Uint16           all;
   struct LCR_BITS  bit;
};

struct LSR_BITS {    // bits  description
   Uint16 DR:1;      // 0     Rx data ready
   Uint16 OE:1;      // 1     overrun error
   Uint16 PE:1;      // 2     priority error
   Uint16 FE:1;      // 3     frame error
   Uint16 BI:1;      // 4     break error
   Uint16 THRE:1;    // 5     THR empty flag 1--empty
   Uint16 rsvd:10;    // 15:7   reserved
};
union LSR_REG {
   Uint16           all;
   struct LSR_BITS  bit;
};

//******* MCR***********//
struct MCR_BITS {    // bits  description
   Uint16 SMNUM:5;      // 4:0
   Uint16 MODULMOD:2;  // 6:5  00-NLM, 01-DP(Carrier Disposition Modulation)
   Uint16 ENSORT:1;    // 7
   Uint16 ENTB:1;      // 8
   Uint16 SCIRxMode:1; // 9    0--MMC SCI Mode 1-- Conventional SCI Mode
   Uint16 SCITxMode:1; // 10    0--MMC SCI Mode 1--Conventional SCI Mode
   Uint16 SCIReset :1; // 11
   Uint16 SMTYPE   :1; // 12
   Uint16 rsvd:3;    // 15:13    reserved
};

union MCR_REG {
   Uint16           all;
   struct MCR_BITS  bit;
};
//******* DRSTR************//
struct DRSTR_BITS {    // bits  description
   Uint16 DR:1;        // 0 -- Data ready
   Uint16 DERR:1;      // 1 -- Data Error
   Uint16 RXCNT:5;     // 6-2 -- RxCount
   Uint16 RXERRCNT:5;  // 11-7 -- RxErrCount
   Uint16 STATE:4; // 15-12   DSP State
};
union DRSTR_REG {
   Uint16           all;
   struct DRSTR_BITS  bit;
};
//******* MSR************//
struct MSR_BITS {    // bits  description
   Uint16 DRVFIN:1;
   Uint16 SORTFIN:1;
   Uint16 rsvd:14;    // 15:2    reserved
};
union MSR_REG {
   Uint16           all;
   struct MSR_BITS  bit;
};
//******* SMNR************//
struct SMNR_BITS {    // bits  description
   Uint16 SMINNUM:5;  //bit 4-0, Insert SMNUM
   Uint16 CURDIR:1;   // 5, 1--charge 0-- discharge
   Uint16 MODUDIR:1;  // 6, MODUDIR=0, i.e. Modulation wave is positive; otherwise it's negative
   Uint16 CycleNUM:5; //bit 11-7, Cycle SMNUM for bypassing SMs
   Uint16 rsvd:4;    // 15:12    reserved
};
union SMNR_REG {
   Uint16           all;
   struct SMNR_BITS  bit;
};
//******* SMNUMCONFIG************//
struct SMNUMCONFIG_BITS {    // bits  description
   Uint16 SORTNUM:5;  //bit 4-0, SORT SM Number
   Uint16 HBSMNUM:5; //bit 9-5, Half-bridge SM Number, effective in hybrid MMC Mode
   Uint16 rsvd:6;    // 15:10    reserved
};
union SMNUMCONFIG_REG {
   Uint16           all;
   struct SMNUMCONFIG_BITS  bit;
};

//==================SCI Format Control==================//
struct  FPGA_SCI_REGS {
   Uint16                  BRR;     // BaudRate Reg
   union LCR_REG           LCR;     // SCI Format Definition
};

//====================ARM STATUS================================//
struct FPGA_ARMST_REGS {     // SM  status  (12 SM)
   Uint16                  	SMSR;     	// SubModule status Reg, 1--SubModule in Error stste
   Uint16                  	DRER;     	// SubModule Data Receive Error Reg,  1--data Receive Error
   Uint16                  	DRSR;    	//  SubModule Data Receive Status Reg, 1--data Receive Finish
   Uint16					VASTR;  	// Capacitor Voltage state (SM1~SM6), 1-- voltage in the range
   Uint16					VBSTR;  	// Capacitor Voltage state (SM7~SM12), 1--voltage in the range
   Uint16					VMAXR;   	// Define the max capacitor voltage
   Uint16					VMINR;   	// Define the Minus capacitor voltage
   Uint16					PREPSTR;   	// SM in preparation state(12 SM)
   Uint16					STBSTR;   	// SM in standby state(12 SM)
   Uint16					RUNSTR;   	// SM in run state(12 SM)
   Uint16					STOPSTR;   	// SM in STOP state(12 SM)
   Uint16					ERRSTR;   // SM in Error state(12 SM)
};

//===================SM Cabinet  Reg===============================//
struct  FPGA_SM_REGS {     // SM  Reg  (One SM)
   //Conventional SCI Regs
   Uint16                  RBR;
   Uint16                  THR;
   union  LSR_REG          LSR;
   //MMC SCI Regs
   Uint16                  SMVPR;//SM voltage VP
   Uint16                  SMVNR;//SM voltage VN
   Uint16 				   SMSTR;//SM STATE
   Uint16				   SMTMR;//SM Temperature
   Uint16				   SMCURR;//SM Current
};
struct  FPGA_SMDATA1_REGS {     // SM  Data Reg
   struct FPGA_SM_REGS     SM1; //Address=0x05~0x0C
   struct FPGA_SM_REGS     SM2;
   struct FPGA_SM_REGS     SM3;
   struct FPGA_SM_REGS     SM4;
   struct FPGA_SM_REGS     SM5;
   struct FPGA_SM_REGS     SM6;
};
struct  FPGA_SMDATA2_REGS {     // SM  Data Reg 2
	struct FPGA_SM_REGS     SM7;
	struct FPGA_SM_REGS     SM8;
	struct FPGA_SM_REGS     SM9;
	struct FPGA_SM_REGS     SM10;
	struct FPGA_SM_REGS     SM11;
	struct FPGA_SM_REGS     SM12; //Address=0x5D~0x64
};
//===================Sample Cabinet A Reg===============================//
struct  FPGA_SAMPLEA_REGS {     //SAMPLE  REG
   //Conventional SCI Regs
   Uint16                  RBR;  //Address=0x70
   Uint16                  THR;
   union  LSR_REG          LSR;
   //MMC SCI Regs
   Uint16                  IPA;
   Uint16                  IPB;
   Uint16 				   IPC;
   Uint16				   INA;
   Uint16				   INB;
   Uint16 				   INC;
   Uint16				   VUV;
   Uint16				   VVW;
   Uint16				   VWU;
   Uint16                  IVDC;
   Uint16                  VDCR;
   union  DRSTR_REG        DRSTR;  //Address=0x7E
   Uint16                  rsvd;   //Address=0x7F

};
//===================Sample Cabinet B Reg===============================//
struct  FPGA_SAMPLEB_REGS {     //SAMPLE  REG
   //Conventioal SCI Regs
   Uint16                  RBR;  //Address=0x80
   Uint16                  THR;
   union  LSR_REG          LSR;
   //MMC SCI Regs
   Uint16                  VUVG;
   Uint16                  VVWG;
   Uint16 				   VWUG;
   Uint16				   IVDC;
   Uint16				   VPA;
   Uint16 				   VPB;
   Uint16				   VPC;
   Uint16				   VAN;
   Uint16				   VBN;
   Uint16                  VCN;
   Uint16                  VDC;
   union DRSTR_REG         DRSTR;
   Uint16                  rsvd; //Address=0x8F
};


struct  FPGA_SORT_REGS {     //SORT  REG
    union  MCR_REG              MCR;
    union  MSR_REG	            MSR;
	Uint16	                    TBPRD; 	// FPGA TB Period
	Uint16 						CMPLR; 	// Low 16 Bit of Modulation Wave, used in DP
	Uint16 						CMPHR; 	// High 16 Bit of Modulation Wave,used in DP
	union  SMNR_REG  	        SMNR;  	// Set the Current Direction and Inserted SMs in NLM
    union  SMNUMCONFIG_REG	    SMNUMCONFIG; //SM Number Configure
	Uint16  				    DRVCMD; // 5 bits of Cmd frame (Bit6~2)
	Uint16 						CPRDR;  // Carrier Period, used in DP
	Uint16 						CMPMAXR;// Max Value of Carrier Compare , used in DP
	Uint16 						CMPMINR;// Min Value of Carrier Compare , used in DP
};
//---------------------------------------------------------------------------
//External References & Function Declarations:
/*========================Control Registers========================*/
extern volatile struct FPGA_SCI_REGS        FPGASciRegs;   //SCI Format Control Regs
extern volatile struct FPGA_SORT_REGS    	UPSortRegs;    //
extern volatile struct FPGA_SORT_REGS    	UNSortRegs;
extern volatile struct FPGA_SORT_REGS    	VPSortRegs;
extern volatile struct FPGA_SORT_REGS    	VNSortRegs;
extern volatile struct FPGA_SORT_REGS    	WPSortRegs;
extern volatile struct FPGA_SORT_REGS    	WNSortRegs;

/*====================Data & Status Registers=======================*/
extern volatile struct FPGA_SAMPLEA_REGS 	SampARegs;
extern volatile struct FPGA_SAMPLEB_REGS 	SampBRegs;

extern volatile struct FPGA_ARMST_REGS       UPARMSTRegs;      //SM Status Regs(Recieve, Err, Finish)
extern volatile struct FPGA_ARMST_REGS       UNARMSTRegs;      //SM Status Regs
extern volatile struct FPGA_ARMST_REGS       VPARMSTRegs;      //SM Status Regs
extern volatile struct FPGA_ARMST_REGS       VNARMSTRegs;      //SM Status Regs
extern volatile struct FPGA_ARMST_REGS       WPARMSTRegs;      //SM Status Regs
extern volatile struct FPGA_ARMST_REGS       WNARMSTRegs;      //SM Status Regs

extern volatile struct FPGA_SMDATA1_REGS     UPSMData1Regs;      //SM Status Reg(SM1~SM6)
extern volatile struct FPGA_SMDATA1_REGS     UNSMData1Regs;      //SM Status Reg(SM1~SM6)
extern volatile struct FPGA_SMDATA1_REGS     VPSMData1Regs;      //SM Status Reg(SM1~SM6)
extern volatile struct FPGA_SMDATA1_REGS     VNSMData1Regs;      //SM Status Reg(SM1~SM6)
extern volatile struct FPGA_SMDATA1_REGS     WPSMData1Regs;      //SM Status Reg(SM1~SM6)
extern volatile struct FPGA_SMDATA1_REGS     WNSMData1Regs;      //SM Status Reg(SM1~SM6)

extern volatile struct FPGA_SMDATA2_REGS    UPSMData2Regs;      //SM Status Reg(SM7~SM12)
extern volatile struct FPGA_SMDATA2_REGS    UNSMData2Regs;      //SM Status Reg(SM7~SM12)
extern volatile struct FPGA_SMDATA2_REGS    VPSMData2Regs;      //SM Status Reg(SM7~SM12)
extern volatile struct FPGA_SMDATA2_REGS    VNSMData2Regs;      //SM Status Reg(SM7~SM12)
extern volatile struct FPGA_SMDATA2_REGS    WPSMData2Regs;      //SM Status Reg(SM7~SM12)
extern volatile struct FPGA_SMDATA2_REGS    WNSMData2Regs;      //SM Status Reg(SM7~SM12)

//=========================================================================
//For debug use only:
extern volatile struct FPGA_ARMST_REGS    UPARMSTRegsShadow;      //ARM Status Reg
extern volatile struct FPGA_ARMST_REGS    UNARMSTRegsShadow;      //ARM Status Reg
extern volatile struct FPGA_ARMST_REGS    VPARMSTRegsShadow;      //ARM Status Reg
extern volatile struct FPGA_ARMST_REGS    VNARMSTRegsShadow;      //ARM Status Reg
extern volatile struct FPGA_ARMST_REGS    WPARMSTRegsShadow;      //ARM Status Reg
extern volatile struct FPGA_ARMST_REGS    WNARMSTRegsShadow;      //ARM Status Reg
extern volatile struct FPGA_SORT_REGS     UPSortRegsShadow;
extern volatile struct FPGA_SORT_REGS     UNSortRegsShadow;
extern volatile struct FPGA_SORT_REGS     VPSortRegsShadow;
extern volatile struct FPGA_SORT_REGS     VNSortRegsShadow;
extern volatile struct FPGA_SORT_REGS     WPSortRegsShadow;
extern volatile struct FPGA_SORT_REGS     WNSortRegsShadow;
//==========================================================================

/*==============extern function declaration============================*/


extern void InitZone7(void);
extern void FPGAReset();
extern void FPGARegsInit(void);
extern void InitFPGA(void);
extern void InitFPGAGPIO(void);

extern void ConfigSortRegs(void);
extern void ConfigUPSort(void);
extern void ConfigUNSort(void);
extern void ConfigVPSort(void);
extern void ConfigVNSort(void);
extern void ConfigWPSort(void);
extern void ConfigWNSort(void);

extern Uint16 ReceiveData_FPGA(Uint16 Addr);  //Addr is the Address of FPGA  registers



extern Uint16 *UPSMSTRAddr_gusv;
extern Uint16 *UNSMSTRAddr_gusv;
extern Uint16 *VPSMSTRAddr_gusv;
extern Uint16 *VNSMSTRAddr_gusv;
extern Uint16 *WPSMSTRAddr_gusv;
extern Uint16 *WNSMSTRAddr_gusv;


#define FPGA_CPRDR  	3750  	// Carrier Period: Tc = 3750*2/37.5us = 200us
#define FPGA_DCMRP    	200   	// Tsci= 200/37.5us = 5.3us

// SM Register Address Map (SM1~SM12)
// For SM N, Addr=AddrStart+8*(N-1). N is the SM Number
// SM IN UP
extern Uint16* Addr_offset;
#define UPSMVPRAddrStart   	0x043  // SM capacitor voltage VP in UP
#define UPSMVNRAddrStart   	0x044  // SM capacitor voltage VV
#define UPSMSTRAddrStart   	0x045  // SM State
#define UPSMTMRAddrStart   	0x046  // SM Temperature
#define UPSMCURAddrStart   	0x047  // SM Current

#define UNSMVPRAddrStart   	0x143  // SM capacitor voltage VP in UN
#define UNSMVNRAddrStart   	0x144  // SM capacitor voltage VV
#define UNSMSTRAddrStart   	0x145  // SM State
#define UNSMTMRAddrStart   	0x146  // SM Temperature
#define UNSMCURAddrStart   	0x147  // SM Current

#define VPSMVPRAddrStart   	0x243  // SM capacitor voltage VP in VP
#define VPSMVNRAddrStart   	0x244  // SM capacitor voltage VV
#define VPSMSTRAddrStart   	0x245  // SM State
#define VPSMTMRAddrStart   	0x246  // SM Temperature
#define VPSMCURAddrStart   	0x247  // SM Current

#define VNSMVPRAddrStart   	0x343  // SM capacitor voltage VP in VN
#define VNSMVNRAddrStart   	0x344  // SM capacitor voltage VV
#define VNSMSTRAddrStart   	0x345  // SM State
#define VNSMTMRAddrStart   	0x346  // SM Temperature
#define VNSMCURAddrStart   	0x347  // SM Current

#define WPSMVPRAddrStart   	0x443  // SM capacitor voltage VP in WP
#define WPSMVNRAddrStart   	0x444  // SM capacitor voltage VV
#define WPSMSTRAddrStart   	0x445  // SM State
#define WPSMTMRAddrStart   	0x446  // SM Temperature
#define WPSMCURAddrStart   	0x447  // SM Current

#define WNSMVPRAddrStart   	0x543  // SM capacitor voltage VP in WN
#define WNSMVNRAddrStart   	0x544  // SM capacitor voltage VV
#define WNSMSTRAddrStart   	0x545  // SM State
#define WNSMTMRAddrStart   	0x546  // SM Temperature
#define WNSMCURAddrStart   	0x547  // SM Current
#endif /* COMM485_H_ */
