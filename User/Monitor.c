/* =================================================================================
 Description:   Half Portable C Blocks for USBCAN Monitor                                      
=====================================================================================
 Version:		MonitorV3.B1
 History:		Created by LiuYongxia, 2018.04.08, Zhejiang University                                                  
------------------------------------------------------------------------------*/

#include "DSP2833x_Device.h"
#include "DSP2833x_Examples.h"
#include "Monitor.h"

Uint32 WaveRequest=0;
Uint32 WaveLength=0;
Uint32 FreqDivision=0;
Uint32 TrigEnable=0;
Uint32 TrigDelay=0;
Uint32 TrigLevelType=0;
Uint32 TrigLevel=0;
Uint32 TrigType=0;
Uint32 TrigSource=0;
Uint32 WaitPoint=0;

int16  MonitorIsrTick=0;
int16  WaveFlowFlag=0;
int16  WaveCatchupFlag=1;
int16  TrigState=0;
int16  WaveQueuePtMain=0;

int16  WavePtIsr=0;
int16  WavePtStart=-1;
int16  WavePtMain=-1;

int16  WaveRequestFlag=0;
int16  WaveQueueLength=0;

Uint32 WaveQueue[2000];
Uint16 WaveLabel[31];//WaveLabel[0] 保存波形个数
Uint32 ScopeCmd[30]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

Uint32 MonitorPara[30]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
Uint32 ParaChangeFlag=0;

Uint32 *WaveVarPt[30]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
Uint32 *ParaVarPt[30]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};


int16 MonitorMain(volatile struct ECAN_REGS *pECanRegs,volatile struct ECAN_MBOXES *pECanMboxes)
{
	Uint32 Id;
	int16 Temp;

	MonitorReceive(pECanRegs,pECanMboxes);

	if(WaveRequestFlag)
	{
		if(WavePtMain>=0)
		{
			if((WavePtMain != WavePtIsr)||(WaveCatchupFlag == 1))
			{
				Id=WaveLabel[WaveQueuePtMain+1]<<2;

				Temp = WavePtMain - WavePtStart;
				if(Temp < 0)
				{
					Temp += WaveLength;
				}
				
				if((Temp & 0x01) ==1)
				{
					Id = Id|0x403;
				}
				else
				{
					Id = Id|0x401;
				}

				if( MonitorTransmit(Id , WaveQueue[(WavePtMain * WaveLabel[0]) + WaveQueuePtMain], pECanRegs,pECanMboxes) == 1 )
				{
					WaveQueuePtMain++;

					if(WaveQueuePtMain>=WaveLabel[0])
					{
						WaveQueuePtMain = 0;
						WavePtMain++;
						if(WavePtMain>=WaveLength)
						{
							WavePtMain = 0;
						}
						WaveCatchupFlag = 0;
					}
				}
			}
			else
			{
				if(WavePtMain == WavePtStart)
				{
					WaveRequestFlag = 0;
				}
			}

		}
		
	}

	return 1;
}

int16 MonitorIsr(void)
{
	int16 i;
	Uint32 *Pt;
	int32 FreqDivisionLocal;

	if(WaveRequestFlag)
	{
		if(TrigEnable)
		{
			WaitPoint --;
			if(((int32)WaitPoint) <= 0)
			{
				WaitPoint = 0;
				TrigEnable = 0;
			}
		}

		if(MonitorIsrTick==0)
		{
			if(WavePtIsr != WavePtStart)
			{
				for(i=0;i<WaveLabel[0];i++)
				{
					Pt = WaveVarPt[WaveLabel[i+1]];

					if(Pt == ((Uint32 *)0) )
					{
						WaveQueue[((Uint16)WavePtIsr) * WaveLabel[0] + i]=0;
					}
					else
					{
						WaveQueue[((Uint16)WavePtIsr) * WaveLabel[0] + i]= *Pt;
					}

					if((WaveLabel[i+1] == TrigSource - 1)&&TrigEnable)
					{
						TrigManage(*Pt);
					}
				}

				if(!TrigEnable)
				{
					if(WavePtStart == -1)
					{
						WavePtStart = WavePtIsr;
						WavePtMain = WavePtIsr;
					}
				}
					
				WavePtIsr++;
				if(WavePtIsr>=WaveLength)
				{
					WavePtIsr = 0;
					WaveFlowFlag = 1;
				}
			}
		}
		
		FreqDivisionLocal = FreqDivision;
		if((FreqDivisionLocal > 100)||(FreqDivisionLocal <= 0))
		{
			FreqDivisionLocal = 1;
		}
		MonitorIsrTick ++;
		if(MonitorIsrTick >= FreqDivisionLocal)
		{
			MonitorIsrTick = 0;
		}
	}

	return 1;
}

int16 TrigManage(Uint32 Source)
{
	int16 TrigLogic; 
	int16 Temp;

	if(WavePtStart != -1)
	{
		return 0 ;
	}
	if(TrigType == 1)
	{
		if(((int32)TrigLevelType) < 0)
		{
			if((*((float32 *)(&Source)))<(*((float32 *)(&TrigLevel))))
			{
				TrigLogic = 0;
			}
			else
			{
				TrigLogic = 1;
			}
		}
		else
		{
			if((*((int32 *)(&Source)))<(*((int32 *)(&TrigLevel))))
			{
				TrigLogic = 0;
			}
			else
			{
				TrigLogic = 1;
			}
		}
	}
	else if(TrigType == 2)
	{
		if(((int32)TrigLevelType) < 0)
		{
			if((*((float32 *)(&Source)))>(*((float32 *)(&TrigLevel))))
			{
				TrigLogic = 0;
			}
			else
			{
				TrigLogic = 1;
			}
		}
		else
		{
			if((*((int32 *)(&Source)))>(*((int32 *)(&TrigLevel))))
			{
				TrigLogic = 0;
			}
			else
			{
				TrigLogic = 1;
			}
		}
	}

	if(TrigState == 0)
	{
		if(TrigLogic == 0)
		{
			TrigState = 1;
		}
	}
	else
	{
		if(TrigLogic == 1)
		{
			Temp = WavePtIsr - (int16)TrigDelay;
			if(Temp < 0)
			{	
				if(WaveFlowFlag == 1)
				{
					Temp += WaveLength;
				}
				else
				{	
					if(TrigEnable == 1)
					{
						Temp = 0;
					}
					else if(TrigEnable == 2)
					{
						TrigState = 0;
						return 0;
					}
				}
			}

			WavePtStart = Temp;
			WavePtMain = Temp;	
		}

	}
	
	return 1;

}

void InitMonECan(volatile struct ECAN_REGS *pECanRegs,volatile struct ECAN_MBOXES *pECanMboxes,volatile struct LAM_REGS *pECanLAMRegs)
{
	InitMonECanAGpio(); //modified
	InitMonECanBGpio(); //modified
	InitMonECanBox(pECanRegs,pECanMboxes);  //modified
	ConfigMonECanBox(pECanRegs,pECanLAMRegs,pECanMboxes);
}


#if MON_ECAN == ECANA

int16 MonitorReceive(void)
{
	struct ECAN_REGS ECanaShadow;
	struct MBOX *PtMbox;
	Uint32 Temp;
	int16  i,j,k;

	ECanaShadow.CANGIF0.all=ECanaRegs.CANGIF0.all;
	if(ECanaShadow.CANGIF0.bit.GMIF0)
	{
		Temp=ECanaRegs.CANRMP.all;
		i=0;
		PtMbox = (struct MBOX*)(&(ECanaMboxes.MBOX0));

		while(Temp!=0)
		{
			if(Temp & 0x01)
			{
				if(i==30)
				{
					if(WaveRequestFlag==0)
					{
						if (PtMbox[i].MDL.all == 1)
						{
							//ScopeCmd Pending
							WaveRequest = ScopeCmd[0];
							WaveLength = ScopeCmd[1];
							FreqDivision = ScopeCmd[2];
							TrigEnable = ScopeCmd[3];
							TrigDelay = ScopeCmd[4];
							TrigLevelType = ScopeCmd[5];
							TrigLevel = ScopeCmd[6];
							TrigType = ScopeCmd[7];
							TrigSource = ScopeCmd[8];
							WaitPoint = ScopeCmd[9];

							k=1;
							for(j=0;j<30;j++)
							{
								if((WaveRequest>>j) & 0x01L)
								{
									WaveLabel[k]=j;
									k++;
								}
							}
							WaveLabel[0]=k-1;

							MonitorIsrTick = 0;
							WaveFlowFlag=0;
							WaveCatchupFlag=1;
							TrigState=0;
							WaveQueuePtMain=0;
							
							WavePtIsr=0;
							WavePtStart=-1;			
							WavePtMain=-1;
						
							WaveQueueLength = WaveLabel[0]*WaveLength;
							if((WaveQueueLength <= 2000)&&(FreqDivision<=100)&&(FreqDivision>=1))
							{
								if(TrigEnable)
								{
									if((TrigEnable >=1)&&(TrigEnable <= 2))
									{
										if((WaveRequest & (1UL << (TrigSource-1)))!=0)
										{
											if(TrigDelay < WaveLength)
											{
												if((TrigType == 1)||(TrigType == 2))
												{
													WaveRequestFlag = 1;
												}
											}	
										}
									}
								}
								else
								{
									WaveRequestFlag = 1;
								}
							}
				
						}
					}
				}
				else if(i<30)
				{
					if(PtMbox[i].MSGCTRL.bit.DLC == 8)
					{
						if(PtMbox[i].MDH.all == 1)
						{
							//Get parameter
							MonitorPara[i] = PtMbox[i].MDL.all;
							if( ParaVarPt[i] != ((Uint32*)0) )
							{
								*(ParaVarPt[i]) = MonitorPara[i];
							}
							ParaChangeFlag |= (0x01L<<i);
						}
						else if(PtMbox[i].MDH.all == 2)
						{
							//Get Scopecmd
							ScopeCmd[i] = PtMbox[i].MDL.all;	
						}
					}

				}
				
				//Ack Receive
				ECanaShadow.CANRMP.all = ECanaRegs.CANRMP.all;
				ECanaShadow.CANRMP.all |= (0x01L<<i);
				ECanaRegs.CANRMP.all = ECanaShadow.CANRMP.all;
			}

			i++;
			Temp=Temp>>1;
		}
	}
	return 1;
}

int16 MonitorTransmit(Uint32 Id, Uint32 Data)
{
	struct ECAN_REGS ECanaShadow;
	int16  Result;

	ECanaShadow.CANTRS.all = ECanaRegs.CANTRS.all;

	if(ECanaShadow.CANTRS.bit.TRS31 == 0)
	{
		ECanaShadow.CANME.all = ECanaRegs.CANME.all;
		ECanaShadow.CANME.bit.ME31 = 0;
		ECanaRegs.CANME.all = ECanaShadow.CANME.all;

		ECanaMboxes.MBOX31.MSGID.bit.STDMSGID = Id;
		ECanaMboxes.MBOX31.MDL.all = Data;

		ECanaShadow.CANME.bit.ME31=1;
		ECanaRegs.CANME.all=ECanaShadow.CANME.all;

		ECanaShadow.CANTRS.all=ECanaRegs.CANTRS.all;
		ECanaShadow.CANTRS.bit.TRS31=1;
		ECanaRegs.CANTRS.all=ECanaShadow.CANTRS.all;

		Result = 1;
	}
	else
		Result = 0;

	ECanaShadow.CANTA.all=ECanaRegs.CANTA.all;
	if(ECanaShadow.CANTA.bit.TA31)
	{
		ECanaShadow.CANTA.all=0;
		ECanaShadow.CANTA.bit.TA31=1;
		ECanaRegs.CANTA.all=ECanaShadow.CANTA.all;
	}

	return(Result);
}

void InitMonECanBox(void)		// Initialize eCAN-A module
{
	/* Create a shadow register structure for the CAN control registers. This is
	needed, since, only 32-bit access is allowed to these registers. 16-bit access
	to these registers could potentially corrupt the register contents. This is
	especially true while writing to a bit (or group of bits) among bits 16 - 31 */
	struct ECAN_REGS ECanaShadow;

	EALLOW;		// EALLOW enables access to protected bits

	/* Configure eCAN RX and TX pins for eCAN transmissions using eCAN regs*/    
	ECanaRegs.CANTIOC.bit.TXFUNC = 1;
	ECanaRegs.CANRIOC.bit.RXFUNC = 1;  

	/* Configure eCAN for HECC mode - (reqd to access mailboxes 16 thru 31) */
	// HECC mode also enables time-stamping feature
	ECanaShadow.CANMC.all = ECanaRegs.CANMC.all;
	ECanaShadow.CANMC.bit.SCB = 1;				
	ECanaRegs.CANMC.all = ECanaShadow.CANMC.all;
	
	/* Initialize all bits of 'Master Control Field' to zero */
	// Some bits of MSGCTRL register come up in an unknown state. For proper operation,
	// all bits (including reserved bits) of MSGCTRL must be initialized to zero
	ECanaMboxes.MBOX0.MSGCTRL.all = 0x00000000;
	ECanaMboxes.MBOX1.MSGCTRL.all = 0x00000000;
	ECanaMboxes.MBOX2.MSGCTRL.all = 0x00000000;
	ECanaMboxes.MBOX3.MSGCTRL.all = 0x00000000;
	ECanaMboxes.MBOX4.MSGCTRL.all = 0x00000000;
	ECanaMboxes.MBOX5.MSGCTRL.all = 0x00000000;
	ECanaMboxes.MBOX6.MSGCTRL.all = 0x00000000;
	ECanaMboxes.MBOX7.MSGCTRL.all = 0x00000000;
	ECanaMboxes.MBOX8.MSGCTRL.all = 0x00000000;
	ECanaMboxes.MBOX9.MSGCTRL.all = 0x00000000;
	ECanaMboxes.MBOX10.MSGCTRL.all = 0x00000000;
	ECanaMboxes.MBOX11.MSGCTRL.all = 0x00000000;
	ECanaMboxes.MBOX12.MSGCTRL.all = 0x00000000;
	ECanaMboxes.MBOX13.MSGCTRL.all = 0x00000000;
	ECanaMboxes.MBOX14.MSGCTRL.all = 0x00000000;
	ECanaMboxes.MBOX15.MSGCTRL.all = 0x00000000;
	ECanaMboxes.MBOX16.MSGCTRL.all = 0x00000000;
	ECanaMboxes.MBOX17.MSGCTRL.all = 0x00000000;
	ECanaMboxes.MBOX18.MSGCTRL.all = 0x00000000;
	ECanaMboxes.MBOX19.MSGCTRL.all = 0x00000000;
	ECanaMboxes.MBOX20.MSGCTRL.all = 0x00000000;
	ECanaMboxes.MBOX21.MSGCTRL.all = 0x00000000;
	ECanaMboxes.MBOX22.MSGCTRL.all = 0x00000000;
	ECanaMboxes.MBOX23.MSGCTRL.all = 0x00000000;
	ECanaMboxes.MBOX24.MSGCTRL.all = 0x00000000;
	ECanaMboxes.MBOX25.MSGCTRL.all = 0x00000000;
	ECanaMboxes.MBOX26.MSGCTRL.all = 0x00000000;
	ECanaMboxes.MBOX27.MSGCTRL.all = 0x00000000;
	ECanaMboxes.MBOX28.MSGCTRL.all = 0x00000000;
	ECanaMboxes.MBOX29.MSGCTRL.all = 0x00000000;
	ECanaMboxes.MBOX30.MSGCTRL.all = 0x00000000;
	ECanaMboxes.MBOX31.MSGCTRL.all = 0x00000000;
    
	// TAn, RMPn, GIFn bits are all zero upon reset and are cleared again
	//	as a matter of precaution. 
	ECanaRegs.CANTA.all	= 0xFFFFFFFF;	/* Clear all TAn bits */      
	ECanaRegs.CANRMP.all = 0xFFFFFFFF;	/* Clear all RMPn bits */      
	ECanaRegs.CANGIF0.all = 0xFFFFFFFF;	/* Clear all interrupt flag bits */ 
	ECanaRegs.CANGIF1.all = 0xFFFFFFFF;
	
	/* Configure bit timing parameters for ECana*/
	ECanaShadow.CANMC.all = ECanaRegs.CANMC.all;
	ECanaShadow.CANMC.bit.CCR = 1 ;           // Set CCR = 1
	ECanaRegs.CANMC.all = ECanaShadow.CANMC.all;

	while(ECanaRegs.CANES.bit.CCE != 1 ) {}   // Wait for CCE bit to be set..

	ECanaShadow.CANBTC.all = 0;
	
#if BAUD_RATE == 1000
	ECanaShadow.CANBTC.bit.BRPREG = 4;  //28335
	ECanaShadow.CANBTC.bit.TSEG2REG = 3;
    ECanaShadow.CANBTC.bit.TSEG1REG = 9;
#else
#if BAUD_RATE == 500
	ECanaShadow.CANBTC.bit.BRPREG = 9;  //28335
	ECanaShadow.CANBTC.bit.TSEG2REG = 3;
	ECanaShadow.CANBTC.bit.TSEG1REG = 9;
#else
#if BAUD_RATE == 250
	ECanaShadow.CANBTC.bit.BRPREG = 19; //28335
	ECanaShadow.CANBTC.bit.TSEG2REG = 3;
	ECanaShadow.CANBTC.bit.TSEG1REG = 9;
#else
#if BAUD_RATE == 50
	ECanaShadow.CANBTC.bit.BRPREG = 99; //28335
	ECanaShadow.CANBTC.bit.TSEG2REG = 3;
	ECanaShadow.CANBTC.bit.TSEG1REG = 9;
#else
#endif
#endif
#endif
#endif

	ECanaShadow.CANBTC.bit.SAM = 1;
	ECanaRegs.CANBTC.all = ECanaShadow.CANBTC.all;

	ECanaShadow.CANMC.all = ECanaRegs.CANMC.all;
	ECanaShadow.CANMC.bit.CCR = 0 ;            // Set CCR = 0
	ECanaRegs.CANMC.all = ECanaShadow.CANMC.all;

	while(ECanaRegs.CANES.bit.CCE == !0 ) {}   // Wait for CCE bit to be cleared..

	/* Disable all Mailboxes  */
	ECanaRegs.CANME.all = 0;		// Required before writing the MSGIDs

	EDIS;
}

void ConfigMonECanBox(void)
{
	struct ECAN_REGS ECanaShadow;

	EALLOW;

	//Disable the mailboxs
	ECanaRegs.CANME.all = 0x00000000;

	//ECan master control
	ECanaShadow.CANMC.all=0;
	ECanaShadow.CANMC.bit.SUSP=1;	//Free run when emulator stop the cpu
	ECanaShadow.CANMC.bit.SCB=1;	//eCAN mode
	ECanaShadow.CANMC.bit.DBO=0;  //Data bit order
	ECanaRegs.CANMC.all = ECanaShadow.CANMC.all;

	//Global mask
	ECanaRegs.CANGAM.all=0x00000000;	//Global marks is useless in eCAN mode

	//Over write protect disable
	ECanaRegs.CANOPC.all=0xFFFFFFFF;

	//Disable timeout
	ECanaRegs.CANTOC.all=0x00000000;

	//Mailbox 0~30 for receive, 31 for transmit
	ECanaRegs.CANMD.all = 0x7FFFFFFF;

	//Local mask configure
	//Receive mask
	ECanaLAMRegs.LAM0.all=0x80000000;//1000 0000 0000 0000
	ECanaLAMRegs.LAM1.all=0x80000000;
	ECanaLAMRegs.LAM2.all=0x80000000;
	ECanaLAMRegs.LAM3.all=0x80000000;
	ECanaLAMRegs.LAM4.all=0x80000000;
	ECanaLAMRegs.LAM5.all=0x80000000;
	ECanaLAMRegs.LAM6.all=0x80000000;
	ECanaLAMRegs.LAM7.all=0x80000000;
	ECanaLAMRegs.LAM8.all=0x80000000;
	ECanaLAMRegs.LAM9.all=0x80000000;
	ECanaLAMRegs.LAM10.all=0x80000000;
	ECanaLAMRegs.LAM11.all=0x80000000;
	ECanaLAMRegs.LAM12.all=0x80000000;
	ECanaLAMRegs.LAM13.all=0x80000000;
	ECanaLAMRegs.LAM14.all=0x80000000;
	ECanaLAMRegs.LAM15.all=0x80000000;
	ECanaLAMRegs.LAM16.all=0x80000000;
	ECanaLAMRegs.LAM17.all=0x80000000;
	ECanaLAMRegs.LAM18.all=0x80000000;
	ECanaLAMRegs.LAM19.all=0x80000000;
	ECanaLAMRegs.LAM20.all=0x80000000;
	ECanaLAMRegs.LAM21.all=0x80000000;
	ECanaLAMRegs.LAM22.all=0x80000000;
	ECanaLAMRegs.LAM23.all=0x80000000;
	ECanaLAMRegs.LAM24.all=0x80000000;
	ECanaLAMRegs.LAM25.all=0x80000000;
	ECanaLAMRegs.LAM26.all=0x80000000;
	ECanaLAMRegs.LAM27.all=0x80000000;
	ECanaLAMRegs.LAM28.all=0x80000000;
	ECanaLAMRegs.LAM29.all=0x80000000;
	ECanaLAMRegs.LAM30.all=0x80000000;
	//Transmit mask is useless
	ECanaLAMRegs.LAM31.all=0L;

	//Mail box configure
	//Receive Mailbox id
	ECanaMboxes.MBOX0.MSGID.all=0x40080000L;//0 0000 0000 10
	ECanaMboxes.MBOX1.MSGID.all=0x40180000L;//0 0000 0001 10
	ECanaMboxes.MBOX2.MSGID.all=0x40280000L;//0 0000 0010 10
	ECanaMboxes.MBOX3.MSGID.all=0x40380000L;
	ECanaMboxes.MBOX4.MSGID.all=0x40480000L;
	ECanaMboxes.MBOX5.MSGID.all=0x40580000L;
	ECanaMboxes.MBOX6.MSGID.all=0x40680000L;
	ECanaMboxes.MBOX7.MSGID.all=0x40780000L;
	ECanaMboxes.MBOX8.MSGID.all=0x40880000L;
	ECanaMboxes.MBOX9.MSGID.all=0x40980000L;
	ECanaMboxes.MBOX10.MSGID.all=0x40A80000L;
	ECanaMboxes.MBOX11.MSGID.all=0x40B80000L;
	ECanaMboxes.MBOX12.MSGID.all=0x40C80000L;
	ECanaMboxes.MBOX13.MSGID.all=0x40D80000L;
	ECanaMboxes.MBOX14.MSGID.all=0x40E80000L;
	ECanaMboxes.MBOX15.MSGID.all=0x40F80000L;
	ECanaMboxes.MBOX16.MSGID.all=0x41080000L;
	ECanaMboxes.MBOX17.MSGID.all=0x41180000L;
	ECanaMboxes.MBOX18.MSGID.all=0x41280000L;
	ECanaMboxes.MBOX19.MSGID.all=0x41380000L;
	ECanaMboxes.MBOX20.MSGID.all=0x41480000L;
	ECanaMboxes.MBOX21.MSGID.all=0x41580000L;
	ECanaMboxes.MBOX22.MSGID.all=0x41680000L;
	ECanaMboxes.MBOX23.MSGID.all=0x41780000L;
	ECanaMboxes.MBOX24.MSGID.all=0x41880000L;
	ECanaMboxes.MBOX25.MSGID.all=0x41980000L;
	ECanaMboxes.MBOX26.MSGID.all=0x41A80000L;
	ECanaMboxes.MBOX27.MSGID.all=0x41B80000L;
	ECanaMboxes.MBOX28.MSGID.all=0x41C80000L;
	ECanaMboxes.MBOX29.MSGID.all=0x41D80000L;//0 0001 1101 10
	ECanaMboxes.MBOX30.MSGID.all=0x4F080000L;//0 1111 0000 10
	//Transmit mail box id
	ECanaMboxes.MBOX31.MSGID.all=0x100C0000L;//1 0000 0000 x1

	//Receive box ctrl
	ECanaMboxes.MBOX0.MSGCTRL.all=0x00000004;//Data length 4 byte
	ECanaMboxes.MBOX1.MSGCTRL.all=0x00000004;
	ECanaMboxes.MBOX2.MSGCTRL.all=0x00000004;
	ECanaMboxes.MBOX3.MSGCTRL.all=0x00000004;
	ECanaMboxes.MBOX4.MSGCTRL.all=0x00000004;
	ECanaMboxes.MBOX5.MSGCTRL.all=0x00000004;
	ECanaMboxes.MBOX6.MSGCTRL.all=0x00000004;
	ECanaMboxes.MBOX7.MSGCTRL.all=0x00000004;
	ECanaMboxes.MBOX8.MSGCTRL.all=0x00000004;
	ECanaMboxes.MBOX9.MSGCTRL.all=0x00000004;
	ECanaMboxes.MBOX10.MSGCTRL.all=0x00000004;
	ECanaMboxes.MBOX11.MSGCTRL.all=0x00000004;
	ECanaMboxes.MBOX12.MSGCTRL.all=0x00000004;
	ECanaMboxes.MBOX13.MSGCTRL.all=0x00000004;
	ECanaMboxes.MBOX14.MSGCTRL.all=0x00000004;
	ECanaMboxes.MBOX15.MSGCTRL.all=0x00000004;
	ECanaMboxes.MBOX16.MSGCTRL.all=0x00000004;
	ECanaMboxes.MBOX17.MSGCTRL.all=0x00000004;
	ECanaMboxes.MBOX18.MSGCTRL.all=0x00000004;
	ECanaMboxes.MBOX19.MSGCTRL.all=0x00000004;
	ECanaMboxes.MBOX20.MSGCTRL.all=0x00000004;
	ECanaMboxes.MBOX21.MSGCTRL.all=0x00000004;
	ECanaMboxes.MBOX22.MSGCTRL.all=0x00000004;
	ECanaMboxes.MBOX23.MSGCTRL.all=0x00000004;
	ECanaMboxes.MBOX24.MSGCTRL.all=0x00000004;
	ECanaMboxes.MBOX25.MSGCTRL.all=0x00000004;
	ECanaMboxes.MBOX26.MSGCTRL.all=0x00000004;
	ECanaMboxes.MBOX27.MSGCTRL.all=0x00000004;
	ECanaMboxes.MBOX28.MSGCTRL.all=0x00000004;
	ECanaMboxes.MBOX29.MSGCTRL.all=0x00000004;
	ECanaMboxes.MBOX30.MSGCTRL.all=0x00000004;
	//Transmit box ctrl
	ECanaMboxes.MBOX31.MSGCTRL.all=0x00000004;

	//Interrupt configure : Disable interrupts
	ECanaRegs.CANMIM.all=0xFFFFFFFF;
	ECanaRegs.CANMIL.all=0x00000000;
	ECanaShadow.CANGIM.all = 0;
	ECanaShadow.CANGIM.bit.GIL=1;
	ECanaShadow.CANGIM.bit.BOIM=1;
	ECanaShadow.CANGIM.bit.I0EN=0;
	ECanaShadow.CANGIM.bit.I1EN=0;
	ECanaRegs.CANGIM.all=ECanaShadow.CANGIM.all;

	//Enable mailbox
	ECanaRegs.CANME.all = 0xFFFFFFFF;

	EDIS;
}

void InitMonECanGpio(void)	//ECan-A
{
   EALLOW;

/* Enable internal pull-up for the selected CAN pins */
// Pull-ups can be enabled or disabled by the user.
// This will enable the pullups for the specified pins.
// Comment out other unwanted lines.

//	GpioCtrlRegs.GPAPUD.bit.GPIO30 = 0;	    // Enable pull-up for GPIO30 (CANRXA)
	GpioCtrlRegs.GPAPUD.bit.GPIO18 = 0;	    // Enable pull-up for GPIO18 (CANRXA)

//	GpioCtrlRegs.GPAPUD.bit.GPIO31 = 0;	    // Enable pull-up for GPIO31 (CANTXA)
	GpioCtrlRegs.GPAPUD.bit.GPIO19 = 0;	    // Enable pull-up for GPIO19 (CANTXA)

/* Set qualification for selected CAN pins to asynch only */
// Inputs are synchronized to SYSCLKOUT by default.
// This will select asynch (no qualification) for the selected pins.

//  GpioCtrlRegs.GPAQSEL2.bit.GPIO30 = 3;   // Asynch qual for GPIO30 (CANRXA)
    GpioCtrlRegs.GPAQSEL2.bit.GPIO18 = 3;   // Asynch qual for GPIO18 (CANRXA)


/* Configure eCAN-A pins using GPIO regs*/
// This specifies which of the possible GPIO pins will be eCAN functional pins.

//	GpioCtrlRegs.GPAMUX2.bit.GPIO30 = 1;	// Configure GPIO30 for CANRXA operation
    GpioCtrlRegs.GPAMUX2.bit.GPIO18 = 3;	// Configure GPIO18 for CANRXA operation
//	GpioCtrlRegs.GPAMUX2.bit.GPIO31 = 1;	// Configure GPIO31 for CANTXA operation
    GpioCtrlRegs.GPAMUX2.bit.GPIO19 = 3;	// Configure GPIO19 for CANTXA operation

    EDIS;
}

#endif 


#if MON_ECAN == ECANB

int16 MonitorReceive(volatile struct ECAN_REGS *pECanRegs,volatile struct ECAN_MBOXES *pECanMboxes)
{
	//struct ECAN_REGS ECanbShadow;
	struct ECAN_REGS ECanShadow;  //modified
	struct MBOX *PtMbox;
	Uint32 Temp;
	int16  i,j,k;

	//ECanbShadow.CANGIF0.all=ECanbRegs.CANGIF0.all;
	ECanShadow.CANGIF0.all=pECanRegs->CANGIF0.all; //modified
	//if(ECanbShadow.CANGIF0.bit.GMIF0)
	if(ECanShadow.CANGIF0.bit.GMIF0) //modified
	{
		//Temp=ECanbRegs.CANRMP.all;
		Temp=pECanRegs->CANRMP.all;  //modified
		i=0;
		//PtMbox = (struct MBOX*)(&(ECanbMboxes.MBOX0));
		PtMbox = (struct MBOX*)(&(pECanMboxes->MBOX0));  //modified

		while(Temp!=0)
		{
			if(Temp & 0x01)
			{
				if(i==30)
				{
					if(WaveRequestFlag==0)
					{
						if (PtMbox[i].MDL.all == 1)
						{
							//ScopeCmd Pending
							WaveRequest = ScopeCmd[0];
							WaveLength = ScopeCmd[1];
							FreqDivision = ScopeCmd[2];
							TrigEnable = ScopeCmd[3];
							TrigDelay = ScopeCmd[4];
							TrigLevelType = ScopeCmd[5];
							TrigLevel = ScopeCmd[6];
							TrigType = ScopeCmd[7];
							TrigSource = ScopeCmd[8];
							WaitPoint = ScopeCmd[9];

							k=1;
							for(j=0;j<30;j++)
							{
								if((WaveRequest>>j) & 0x01L)
								{
									WaveLabel[k]=j;
									k++;
								}
							}
							WaveLabel[0]=k-1;

							MonitorIsrTick = 0;
							WaveFlowFlag=0;
							WaveCatchupFlag=1;
							TrigState=0;
							WaveQueuePtMain=0;
							
							WavePtIsr=0;
							WavePtStart=-1;			
							WavePtMain=-1;
						
							WaveQueueLength = WaveLabel[0]*WaveLength;
							if((WaveQueueLength <= 2000)&&(FreqDivision<=100)&&(FreqDivision>=1))
							{
								if(TrigEnable)
								{
									if((TrigEnable >=1)&&(TrigEnable <= 2))
									{
										if((WaveRequest & (1UL << (TrigSource-1)))!=0)
										{
											if(TrigDelay < WaveLength)
											{
												if((TrigType == 1)||(TrigType == 2))
												{
													WaveRequestFlag = 1;
												}
											}	
										}
									}
								}
								else
								{
									WaveRequestFlag = 1;
								}
							}
				
						}
					}
				}
				else if(i<30)
				{
					if(PtMbox[i].MSGCTRL.bit.DLC == 8)
					{
						if(PtMbox[i].MDH.all == 1)
						{
							//Get parameter
							MonitorPara[i] = PtMbox[i].MDL.all;
							if( ParaVarPt[i] != ((Uint32*)0) )
							{
								*(ParaVarPt[i]) = MonitorPara[i];
							}
							ParaChangeFlag |= (0x01L<<i);
						}
						else if(PtMbox[i].MDH.all == 2)
						{
							//Get Scopecmd
							ScopeCmd[i] = PtMbox[i].MDL.all;	
						}
					}

				}
				
				//Ack Receive
				//ECanbShadow.CANRMP.all = ECanbRegs.CANRMP.all;
				//ECanbShadow.CANRMP.all |= (0x01L<<i);
				//ECanbRegs.CANRMP.all = ECanbShadow.CANRMP.all;
				//modified
				ECanShadow.CANRMP.all = pECanRegs->CANRMP.all;
				ECanShadow.CANRMP.all |= (0x01L<<i);
				pECanRegs->CANRMP.all = ECanShadow.CANRMP.all;
			}

			i++;
			Temp=Temp>>1;
		}
	}
	return 1;
}

int16 MonitorTransmit(Uint32 Id, Uint32 Data, volatile struct ECAN_REGS *pECanRegs,volatile struct ECAN_MBOXES *pECanMboxes)
{
	//struct ECAN_REGS ECanbShadow;
	struct ECAN_REGS ECanShadow; //modified
	int16  Result;

	//ECanbShadow.CANTRS.all = ECanbRegs.CANTRS.all;
	ECanShadow.CANTRS.all = pECanRegs->CANTRS.all; //modified

	//if(ECanbShadow.CANTRS.bit.TRS31 == 0)
	if(ECanShadow.CANTRS.bit.TRS31 == 0)  //modified
	{
		//ECanbShadow.CANME.all = ECanbRegs.CANME.all;
		//ECanbShadow.CANME.bit.ME31 = 0;
		//ECanbRegs.CANME.all = ECanbShadow.CANME.all;
		//modified
		ECanShadow.CANME.all = pECanRegs->CANME.all;
		ECanShadow.CANME.bit.ME31 = 0;
		pECanRegs->CANME.all = ECanShadow.CANME.all;

		//ECanbMboxes.MBOX31.MSGID.bit.STDMSGID = Id;
		//ECanbMboxes.MBOX31.MDL.all = Data;
		//modified
		pECanMboxes->MBOX31.MSGID.bit.STDMSGID = Id;
		pECanMboxes->MBOX31.MDL.all = Data;

		//ECanbShadow.CANME.bit.ME31=1;
		//ECanbRegs.CANME.all=ECanbShadow.CANME.all;
		//modified
		ECanShadow.CANME.bit.ME31=1;
		pECanRegs->CANME.all=ECanShadow.CANME.all;

		//ECanbShadow.CANTRS.all=ECanbRegs.CANTRS.all;
		//ECanbShadow.CANTRS.bit.TRS31=1;
		//ECanbRegs.CANTRS.all=ECanbShadow.CANTRS.all;
		//modified
		ECanShadow.CANTRS.all=pECanRegs->CANTRS.all;
		ECanShadow.CANTRS.bit.TRS31=1;
		pECanRegs->CANTRS.all=ECanShadow.CANTRS.all;

		Result = 1;
	}
	else
		Result = 0;

	//ECanbShadow.CANTA.all=ECanbRegs.CANTA.all;
	ECanShadow.CANTA.all=pECanRegs->CANTA.all; //modified
	//if(ECanbShadow.CANTA.bit.TA31)
	if(ECanShadow.CANTA.bit.TA31)  //modified
	{
		//ECanbShadow.CANTA.all=0;
		//ECanbShadow.CANTA.bit.TA31=1;
		//ECanbRegs.CANTA.all=ECanbShadow.CANTA.all;
		//modified
		ECanShadow.CANTA.all=0;
		ECanShadow.CANTA.bit.TA31=1;
		pECanRegs->CANTA.all=ECanShadow.CANTA.all;
	}

	return(Result);
}

void InitMonECanBox(volatile struct ECAN_REGS *pECanRegs,volatile struct ECAN_MBOXES *pECanMboxes)		// Initialize eCAN-B module
{
	/* Create a shadow register structure for the CAN control registers. This is
	needed, since, only 32-bit access is allowed to these registers. 16-bit access
	to these registers could potentially corrupt the register contents. This is
	especially true while writing to a bit (or group of bits) among bits 16 - 31 */
	//struct ECAN_REGS ECanbShadow;
	struct ECAN_REGS ECanShadow; //modified

	EALLOW;		// EALLOW enables access to protected bits

	/* Configure eCAN RX and TX pins for eCAN transmissions using eCAN regs*/    
	//ECanbRegs.CANTIOC.bit.TXFUNC = 1;
	//ECanbRegs.CANRIOC.bit.RXFUNC = 1;

	//modified
	pECanRegs->CANTIOC.bit.TXFUNC = 1;
	pECanRegs->CANRIOC.bit.RXFUNC = 1;

	/* Configure eCAN for HECC mode - (reqd to access mailboxes 16 thru 31) */
	// HECC mode also enables time-stamping feature
	//ECanbShadow.CANMC.all = ECanbRegs.CANMC.all;
	//ECanbShadow.CANMC.bit.SCB = 1;
	//ECanbRegs.CANMC.all = ECanbShadow.CANMC.all;

	//modified
	ECanShadow.CANMC.all = pECanRegs->CANMC.all;
	ECanShadow.CANMC.bit.SCB = 1;
	pECanRegs->CANMC.all = ECanShadow.CANMC.all;
	
	/* Initialize all bits of 'Master Control Field' to zero */
	// Some bits of MSGCTRL register come up in an unknown state. For proper operation,
	// all bits (including reserved bits) of MSGCTRL must be initialized to zero
	/*
	ECanbMboxes.MBOX0.MSGCTRL.all = 0x00000000;
	ECanbMboxes.MBOX1.MSGCTRL.all = 0x00000000;
	ECanbMboxes.MBOX2.MSGCTRL.all = 0x00000000;
	ECanbMboxes.MBOX3.MSGCTRL.all = 0x00000000;
	ECanbMboxes.MBOX4.MSGCTRL.all = 0x00000000;
	ECanbMboxes.MBOX5.MSGCTRL.all = 0x00000000;
	ECanbMboxes.MBOX6.MSGCTRL.all = 0x00000000;
	ECanbMboxes.MBOX7.MSGCTRL.all = 0x00000000;
	ECanbMboxes.MBOX8.MSGCTRL.all = 0x00000000;
	ECanbMboxes.MBOX9.MSGCTRL.all = 0x00000000;
	ECanbMboxes.MBOX10.MSGCTRL.all = 0x00000000;
	ECanbMboxes.MBOX11.MSGCTRL.all = 0x00000000;
	ECanbMboxes.MBOX12.MSGCTRL.all = 0x00000000;
	ECanbMboxes.MBOX13.MSGCTRL.all = 0x00000000;
	ECanbMboxes.MBOX14.MSGCTRL.all = 0x00000000;
	ECanbMboxes.MBOX15.MSGCTRL.all = 0x00000000;
	ECanbMboxes.MBOX16.MSGCTRL.all = 0x00000000;
	ECanbMboxes.MBOX17.MSGCTRL.all = 0x00000000;
	ECanbMboxes.MBOX18.MSGCTRL.all = 0x00000000;
	ECanbMboxes.MBOX19.MSGCTRL.all = 0x00000000;
	ECanbMboxes.MBOX20.MSGCTRL.all = 0x00000000;
	ECanbMboxes.MBOX21.MSGCTRL.all = 0x00000000;
	ECanbMboxes.MBOX22.MSGCTRL.all = 0x00000000;
	ECanbMboxes.MBOX23.MSGCTRL.all = 0x00000000;
	ECanbMboxes.MBOX24.MSGCTRL.all = 0x00000000;
	ECanbMboxes.MBOX25.MSGCTRL.all = 0x00000000;
	ECanbMboxes.MBOX26.MSGCTRL.all = 0x00000000;
	ECanbMboxes.MBOX27.MSGCTRL.all = 0x00000000;
	ECanbMboxes.MBOX28.MSGCTRL.all = 0x00000000;
	ECanbMboxes.MBOX29.MSGCTRL.all = 0x00000000;
	ECanbMboxes.MBOX30.MSGCTRL.all = 0x00000000;
	ECanbMboxes.MBOX31.MSGCTRL.all = 0x00000000;
	*/

	//modified
	pECanMboxes->MBOX0.MSGCTRL.all = 0x00000000;
	pECanMboxes->MBOX1.MSGCTRL.all = 0x00000000;
	pECanMboxes->MBOX2.MSGCTRL.all = 0x00000000;
	pECanMboxes->MBOX3.MSGCTRL.all = 0x00000000;
	pECanMboxes->MBOX4.MSGCTRL.all = 0x00000000;
	pECanMboxes->MBOX5.MSGCTRL.all = 0x00000000;
	pECanMboxes->MBOX6.MSGCTRL.all = 0x00000000;
	pECanMboxes->MBOX7.MSGCTRL.all = 0x00000000;
	pECanMboxes->MBOX8.MSGCTRL.all = 0x00000000;
	pECanMboxes->MBOX9.MSGCTRL.all = 0x00000000;
	pECanMboxes->MBOX10.MSGCTRL.all = 0x00000000;
	pECanMboxes->MBOX11.MSGCTRL.all = 0x00000000;
	pECanMboxes->MBOX12.MSGCTRL.all = 0x00000000;
	pECanMboxes->MBOX13.MSGCTRL.all = 0x00000000;
	pECanMboxes->MBOX14.MSGCTRL.all = 0x00000000;
	pECanMboxes->MBOX15.MSGCTRL.all = 0x00000000;
	pECanMboxes->MBOX16.MSGCTRL.all = 0x00000000;
	pECanMboxes->MBOX17.MSGCTRL.all = 0x00000000;
	pECanMboxes->MBOX18.MSGCTRL.all = 0x00000000;
	pECanMboxes->MBOX19.MSGCTRL.all = 0x00000000;
	pECanMboxes->MBOX20.MSGCTRL.all = 0x00000000;
	pECanMboxes->MBOX21.MSGCTRL.all = 0x00000000;
	pECanMboxes->MBOX22.MSGCTRL.all = 0x00000000;
	pECanMboxes->MBOX23.MSGCTRL.all = 0x00000000;
	pECanMboxes->MBOX24.MSGCTRL.all = 0x00000000;
	pECanMboxes->MBOX25.MSGCTRL.all = 0x00000000;
	pECanMboxes->MBOX26.MSGCTRL.all = 0x00000000;
	pECanMboxes->MBOX27.MSGCTRL.all = 0x00000000;
	pECanMboxes->MBOX28.MSGCTRL.all = 0x00000000;
	pECanMboxes->MBOX29.MSGCTRL.all = 0x00000000;
	pECanMboxes->MBOX30.MSGCTRL.all = 0x00000000;
	pECanMboxes->MBOX31.MSGCTRL.all = 0x00000000;
    
	// TAn, RMPn, GIFn bits are all zero upon reset and are cleared again
	//	as a matter of precaution.

	//ECanbRegs.CANTA.all	= 0xFFFFFFFF;	/* Clear all TAn bits */
	//ECanbRegs.CANRMP.all = 0xFFFFFFFF;	/* Clear all RMPn bits */
	//ECanbRegs.CANGIF0.all = 0xFFFFFFFF;	/* Clear all interrupt flag bits */
	//ECanbRegs.CANGIF1.all = 0xFFFFFFFF;

	pECanRegs->CANTA.all	= 0xFFFFFFFF;	/* Clear all TAn bits */
	pECanRegs->CANRMP.all = 0xFFFFFFFF;	/* Clear all RMPn bits */
	pECanRegs->CANGIF0.all = 0xFFFFFFFF;	/* Clear all interrupt flag bits */
	pECanRegs->CANGIF1.all = 0xFFFFFFFF;
	
	/* Configure bit timing parameters for ECanb*/
	/*ECanbShadow.CANMC.all = ECanbRegs.CANMC.all;
	ECanbShadow.CANMC.bit.CCR = 1 ;           // Set CCR = 1
	ECanbRegs.CANMC.all = ECanbShadow.CANMC.all;

	while(ECanbRegs.CANES.bit.CCE != 1 ) {}   // Wait for CCE bit to be set..

	ECanbShadow.CANBTC.all = 0;*/

	//modified
	ECanShadow.CANMC.all = pECanRegs->CANMC.all;
	ECanShadow.CANMC.bit.CCR = 1 ;           // Set CCR = 1
	pECanRegs->CANMC.all = ECanShadow.CANMC.all;

	while(pECanRegs->CANES.bit.CCE != 1 ) {}   // Wait for CCE bit to be set..

	ECanShadow.CANBTC.all = 0;
	
#if BAUD_RATE == 1000
	//ECanbShadow.CANBTC.bit.BRPREG = 4;  //28335
	//ECanbShadow.CANBTC.bit.TSEG2REG = 3;
    //ECanbShadow.CANBTC.bit.TSEG1REG = 9;

    //modified
    ECanShadow.CANBTC.bit.BRPREG = 4;  //28335
	ECanShadow.CANBTC.bit.TSEG2REG = 3;
	ECanShadow.CANBTC.bit.TSEG1REG = 9;
#else
#if BAUD_RATE == 500
	//ECanbShadow.CANBTC.bit.BRPREG = 9;  //28335
	//ECanbShadow.CANBTC.bit.TSEG2REG = 3;
	//ECanbShadow.CANBTC.bit.TSEG1REG = 9;

	//modified
	ECanShadow.CANBTC.bit.BRPREG = 9;  //28335
	ECanShadow.CANBTC.bit.TSEG2REG = 3;
	ECanShadow.CANBTC.bit.TSEG1REG = 9;
#else
#if BAUD_RATE == 250
	//ECanbShadow.CANBTC.bit.BRPREG = 19; //28335
	//ECanbShadow.CANBTC.bit.TSEG2REG = 3;
	//ECanbShadow.CANBTC.bit.TSEG1REG = 9;

	//modified
	ECanShadow.CANBTC.bit.BRPREG = 19; //28335
	ECanShadow.CANBTC.bit.TSEG2REG = 3;
	ECanShadow.CANBTC.bit.TSEG1REG = 9;
#else
#if BAUD_RATE == 50
	//ECanbShadow.CANBTC.bit.BRPREG = 99; //28335
	//ECanbShadow.CANBTC.bit.TSEG2REG = 3;
	//ECanbShadow.CANBTC.bit.TSEG1REG = 9;

	//modified
	ECanShadow.CANBTC.bit.BRPREG = 99; //28335
	ECanShadow.CANBTC.bit.TSEG2REG = 3;
	ECanShadow.CANBTC.bit.TSEG1REG = 9;
#else
#endif
#endif
#endif
#endif

	/*
	ECanbShadow.CANBTC.bit.SAM = 1;
	ECanbRegs.CANBTC.all = ECanbShadow.CANBTC.all;

	ECanbShadow.CANMC.all = ECanbRegs.CANMC.all;
	ECanbShadow.CANMC.bit.CCR = 0 ;            // Set CCR = 0
	ECanbRegs.CANMC.all = ECanbShadow.CANMC.all;

	while(ECanbRegs.CANES.bit.CCE == !0 ) {}   // Wait for CCE bit to be cleared..
	*/

	//modified
	ECanShadow.CANBTC.bit.SAM = 1;
	pECanRegs->CANBTC.all = ECanShadow.CANBTC.all;

	ECanShadow.CANMC.all = pECanRegs->CANMC.all;
	ECanShadow.CANMC.bit.CCR = 0 ;            // Set CCR = 0
	pECanRegs->CANMC.all = ECanShadow.CANMC.all;

	while(pECanRegs->CANES.bit.CCE == !0 ) {}   // Wait for CCE bit to be cleared..

	/* Disable all Mailboxes  */
	//ECanbRegs.CANME.all = 0;		// Required before writing the MSGIDs
	//modified
	pECanRegs->CANME.all = 0;		// Required before writing the MSGIDs

	EDIS;
}

void ConfigMonECanBox(volatile struct ECAN_REGS *pECanRegs,volatile struct LAM_REGS *pECanLAMRegs,volatile struct ECAN_MBOXES *pECanMboxes)
{
	//struct ECAN_REGS ECanbShadow;
	struct ECAN_REGS ECanShadow; //modified

	EALLOW;

	//Disable the mailboxs
	//ECanbRegs.CANME.all = 0x00000000;
	pECanRegs->CANME.all = 0x00000000; //modified

	//ECan master control
	/*
	ECanbShadow.CANMC.all=0;
	ECanbShadow.CANMC.bit.SUSP=1;	//Free run when emulator stop the cpu
	ECanbShadow.CANMC.bit.SCB=1;	//eCAN mode
	ECanbShadow.CANMC.bit.DBO=0;  //Data bit order
	ECanbRegs.CANMC.all = ECanbShadow.CANMC.all;
	*/

	//modified
	ECanShadow.CANMC.all=0;
	ECanShadow.CANMC.bit.SUSP=1;	//Free run when emulator stop the cpu
	ECanShadow.CANMC.bit.SCB=1;	//eCAN mode
	ECanShadow.CANMC.bit.DBO=0;  //Data bit order
	pECanRegs->CANMC.all = ECanShadow.CANMC.all;

	//Global mask
	//ECanbRegs.CANGAM.all=0x00000000;	//Global marks is useless in eCAN mode
	//modified
	pECanRegs->CANGAM.all=0x00000000;	//Global marks is useless in eCAN mode

	//Over write protect disable
	//ECanbRegs.CANOPC.all=0xFFFFFFFF;
	//modified
	pECanRegs->CANOPC.all=0xFFFFFFFF;

	//Disable timeout
	//ECanbRegs.CANTOC.all=0x00000000;
	//modified
	pECanRegs->CANTOC.all=0x00000000;

	//Mailbox 0~30 for receive, 31 for transmit
	//ECanbRegs.CANMD.all = 0x7FFFFFFF;
	//modified
	pECanRegs->CANMD.all = 0x7FFFFFFF;

	//Local mask configure
	//Receive mask
	/*
	ECanbLAMRegs.LAM0.all=0x80000000;//1000 0000 0000 0000
	ECanbLAMRegs.LAM1.all=0x80000000;
	ECanbLAMRegs.LAM2.all=0x80000000;
	ECanbLAMRegs.LAM3.all=0x80000000;
	ECanbLAMRegs.LAM4.all=0x80000000;
	ECanbLAMRegs.LAM5.all=0x80000000;
	ECanbLAMRegs.LAM6.all=0x80000000;
	ECanbLAMRegs.LAM7.all=0x80000000;
	ECanbLAMRegs.LAM8.all=0x80000000;
	ECanbLAMRegs.LAM9.all=0x80000000;
	ECanbLAMRegs.LAM10.all=0x80000000;
	ECanbLAMRegs.LAM11.all=0x80000000;
	ECanbLAMRegs.LAM12.all=0x80000000;
	ECanbLAMRegs.LAM13.all=0x80000000;
	ECanbLAMRegs.LAM14.all=0x80000000;
	ECanbLAMRegs.LAM15.all=0x80000000;
	ECanbLAMRegs.LAM16.all=0x80000000;
	ECanbLAMRegs.LAM17.all=0x80000000;
	ECanbLAMRegs.LAM18.all=0x80000000;
	ECanbLAMRegs.LAM19.all=0x80000000;
	ECanbLAMRegs.LAM20.all=0x80000000;
	ECanbLAMRegs.LAM21.all=0x80000000;
	ECanbLAMRegs.LAM22.all=0x80000000;
	ECanbLAMRegs.LAM23.all=0x80000000;
	ECanbLAMRegs.LAM24.all=0x80000000;
	ECanbLAMRegs.LAM25.all=0x80000000;
	ECanbLAMRegs.LAM26.all=0x80000000;
	ECanbLAMRegs.LAM27.all=0x80000000;
	ECanbLAMRegs.LAM28.all=0x80000000;
	ECanbLAMRegs.LAM29.all=0x80000000;
	ECanbLAMRegs.LAM30.all=0x80000000;
	//Transmit mask is useless
	ECanbLAMRegs.LAM31.all=0L;
	*/

	//modified
	pECanLAMRegs->LAM0.all=0x80000000;//1000 0000 0000 0000
	pECanLAMRegs->LAM1.all=0x80000000;
	pECanLAMRegs->LAM2.all=0x80000000;
	pECanLAMRegs->LAM3.all=0x80000000;
	pECanLAMRegs->LAM4.all=0x80000000;
	pECanLAMRegs->LAM5.all=0x80000000;
	pECanLAMRegs->LAM6.all=0x80000000;
	pECanLAMRegs->LAM7.all=0x80000000;
	pECanLAMRegs->LAM8.all=0x80000000;
	pECanLAMRegs->LAM9.all=0x80000000;
	pECanLAMRegs->LAM10.all=0x80000000;
	pECanLAMRegs->LAM11.all=0x80000000;
	pECanLAMRegs->LAM12.all=0x80000000;
	pECanLAMRegs->LAM13.all=0x80000000;
	pECanLAMRegs->LAM14.all=0x80000000;
	pECanLAMRegs->LAM15.all=0x80000000;
	pECanLAMRegs->LAM16.all=0x80000000;
	pECanLAMRegs->LAM17.all=0x80000000;
	pECanLAMRegs->LAM18.all=0x80000000;
	pECanLAMRegs->LAM19.all=0x80000000;
	pECanLAMRegs->LAM20.all=0x80000000;
	pECanLAMRegs->LAM21.all=0x80000000;
	pECanLAMRegs->LAM22.all=0x80000000;
	pECanLAMRegs->LAM23.all=0x80000000;
	pECanLAMRegs->LAM24.all=0x80000000;
	pECanLAMRegs->LAM25.all=0x80000000;
	pECanLAMRegs->LAM26.all=0x80000000;
	pECanLAMRegs->LAM27.all=0x80000000;
	pECanLAMRegs->LAM28.all=0x80000000;
	pECanLAMRegs->LAM29.all=0x80000000;
	pECanLAMRegs->LAM30.all=0x80000000;
	//Transmit mask is useless
	pECanLAMRegs->LAM31.all=0L;

	//Mail box configure
	//Receive Mailbox id
	/*
	ECanbMboxes.MBOX0.MSGID.all=0x40080000L;//0 0000 0000 10
	ECanbMboxes.MBOX1.MSGID.all=0x40180000L;//0 0000 0001 10
	ECanbMboxes.MBOX2.MSGID.all=0x40280000L;//0 0000 0010 10
	ECanbMboxes.MBOX3.MSGID.all=0x40380000L;
	ECanbMboxes.MBOX4.MSGID.all=0x40480000L;
	ECanbMboxes.MBOX5.MSGID.all=0x40580000L;
	ECanbMboxes.MBOX6.MSGID.all=0x40680000L;
	ECanbMboxes.MBOX7.MSGID.all=0x40780000L;
	ECanbMboxes.MBOX8.MSGID.all=0x40880000L;
	ECanbMboxes.MBOX9.MSGID.all=0x40980000L;
	ECanbMboxes.MBOX10.MSGID.all=0x40A80000L;
	ECanbMboxes.MBOX11.MSGID.all=0x40B80000L;
	ECanbMboxes.MBOX12.MSGID.all=0x40C80000L;
	ECanbMboxes.MBOX13.MSGID.all=0x40D80000L;
	ECanbMboxes.MBOX14.MSGID.all=0x40E80000L;
	ECanbMboxes.MBOX15.MSGID.all=0x40F80000L;
	ECanbMboxes.MBOX16.MSGID.all=0x41080000L;
	ECanbMboxes.MBOX17.MSGID.all=0x41180000L;
	ECanbMboxes.MBOX18.MSGID.all=0x41280000L;
	ECanbMboxes.MBOX19.MSGID.all=0x41380000L;
	ECanbMboxes.MBOX20.MSGID.all=0x41480000L;
	ECanbMboxes.MBOX21.MSGID.all=0x41580000L;
	ECanbMboxes.MBOX22.MSGID.all=0x41680000L;
	ECanbMboxes.MBOX23.MSGID.all=0x41780000L;
	ECanbMboxes.MBOX24.MSGID.all=0x41880000L;
	ECanbMboxes.MBOX25.MSGID.all=0x41980000L;
	ECanbMboxes.MBOX26.MSGID.all=0x41A80000L;
	ECanbMboxes.MBOX27.MSGID.all=0x41B80000L;
	ECanbMboxes.MBOX28.MSGID.all=0x41C80000L;
	ECanbMboxes.MBOX29.MSGID.all=0x41D80000L;//0 0001 1101 10
	ECanbMboxes.MBOX30.MSGID.all=0x4F080000L;//0 1111 0000 10
	//Transmit mail box id
	ECanbMboxes.MBOX31.MSGID.all=0x100C0000L;//1 0000 0000 x1
	*/

	//modified
	pECanMboxes->MBOX0.MSGID.all=0x40080000L;//0 0000 0000 10
	pECanMboxes->MBOX1.MSGID.all=0x40180000L;//0 0000 0001 10
	pECanMboxes->MBOX2.MSGID.all=0x40280000L;//0 0000 0010 10
	pECanMboxes->MBOX3.MSGID.all=0x40380000L;
	pECanMboxes->MBOX4.MSGID.all=0x40480000L;
	pECanMboxes->MBOX5.MSGID.all=0x40580000L;
	pECanMboxes->MBOX6.MSGID.all=0x40680000L;
	pECanMboxes->MBOX7.MSGID.all=0x40780000L;
	pECanMboxes->MBOX8.MSGID.all=0x40880000L;
	pECanMboxes->MBOX9.MSGID.all=0x40980000L;
	pECanMboxes->MBOX10.MSGID.all=0x40A80000L;
	pECanMboxes->MBOX11.MSGID.all=0x40B80000L;
	pECanMboxes->MBOX12.MSGID.all=0x40C80000L;
	pECanMboxes->MBOX13.MSGID.all=0x40D80000L;
	pECanMboxes->MBOX14.MSGID.all=0x40E80000L;
	pECanMboxes->MBOX15.MSGID.all=0x40F80000L;
	pECanMboxes->MBOX16.MSGID.all=0x41080000L;
	pECanMboxes->MBOX17.MSGID.all=0x41180000L;
	pECanMboxes->MBOX18.MSGID.all=0x41280000L;
	pECanMboxes->MBOX19.MSGID.all=0x41380000L;
	pECanMboxes->MBOX20.MSGID.all=0x41480000L;
	pECanMboxes->MBOX21.MSGID.all=0x41580000L;
	pECanMboxes->MBOX22.MSGID.all=0x41680000L;
	pECanMboxes->MBOX23.MSGID.all=0x41780000L;
	pECanMboxes->MBOX24.MSGID.all=0x41880000L;
	pECanMboxes->MBOX25.MSGID.all=0x41980000L;
	pECanMboxes->MBOX26.MSGID.all=0x41A80000L;
	pECanMboxes->MBOX27.MSGID.all=0x41B80000L;
	pECanMboxes->MBOX28.MSGID.all=0x41C80000L;
	pECanMboxes->MBOX29.MSGID.all=0x41D80000L;//0 0001 1101 10
	pECanMboxes->MBOX30.MSGID.all=0x4F080000L;//0 1111 0000 10
	//Transmit mail box id
	pECanMboxes->MBOX31.MSGID.all=0x100C0000L;//1 0000 0000 x1

	//Receive box ctrl
	/*
	ECanbMboxes.MBOX0.MSGCTRL.all=0x00000004;//Data length 4 byte
	ECanbMboxes.MBOX1.MSGCTRL.all=0x00000004;
	ECanbMboxes.MBOX2.MSGCTRL.all=0x00000004;
	ECanbMboxes.MBOX3.MSGCTRL.all=0x00000004;
	ECanbMboxes.MBOX4.MSGCTRL.all=0x00000004;
	ECanbMboxes.MBOX5.MSGCTRL.all=0x00000004;
	ECanbMboxes.MBOX6.MSGCTRL.all=0x00000004;
	ECanbMboxes.MBOX7.MSGCTRL.all=0x00000004;
	ECanbMboxes.MBOX8.MSGCTRL.all=0x00000004;
	ECanbMboxes.MBOX9.MSGCTRL.all=0x00000004;
	ECanbMboxes.MBOX10.MSGCTRL.all=0x00000004;
	ECanbMboxes.MBOX11.MSGCTRL.all=0x00000004;
	ECanbMboxes.MBOX12.MSGCTRL.all=0x00000004;
	ECanbMboxes.MBOX13.MSGCTRL.all=0x00000004;
	ECanbMboxes.MBOX14.MSGCTRL.all=0x00000004;
	ECanbMboxes.MBOX15.MSGCTRL.all=0x00000004;
	ECanbMboxes.MBOX16.MSGCTRL.all=0x00000004;
	ECanbMboxes.MBOX17.MSGCTRL.all=0x00000004;
	ECanbMboxes.MBOX18.MSGCTRL.all=0x00000004;
	ECanbMboxes.MBOX19.MSGCTRL.all=0x00000004;
	ECanbMboxes.MBOX20.MSGCTRL.all=0x00000004;
	ECanbMboxes.MBOX21.MSGCTRL.all=0x00000004;
	ECanbMboxes.MBOX22.MSGCTRL.all=0x00000004;
	ECanbMboxes.MBOX23.MSGCTRL.all=0x00000004;
	ECanbMboxes.MBOX24.MSGCTRL.all=0x00000004;
	ECanbMboxes.MBOX25.MSGCTRL.all=0x00000004;
	ECanbMboxes.MBOX26.MSGCTRL.all=0x00000004;
	ECanbMboxes.MBOX27.MSGCTRL.all=0x00000004;
	ECanbMboxes.MBOX28.MSGCTRL.all=0x00000004;
	ECanbMboxes.MBOX29.MSGCTRL.all=0x00000004;
	ECanbMboxes.MBOX30.MSGCTRL.all=0x00000004;
	//Transmit box ctrl
	ECanbMboxes.MBOX31.MSGCTRL.all=0x00000004;
	*/

	//modified
	pECanMboxes->MBOX0.MSGCTRL.all=0x00000004;//Data length 4 byte
	pECanMboxes->MBOX1.MSGCTRL.all=0x00000004;
	pECanMboxes->MBOX2.MSGCTRL.all=0x00000004;
	pECanMboxes->MBOX3.MSGCTRL.all=0x00000004;
	pECanMboxes->MBOX4.MSGCTRL.all=0x00000004;
	pECanMboxes->MBOX5.MSGCTRL.all=0x00000004;
	pECanMboxes->MBOX6.MSGCTRL.all=0x00000004;
	pECanMboxes->MBOX7.MSGCTRL.all=0x00000004;
	pECanMboxes->MBOX8.MSGCTRL.all=0x00000004;
	pECanMboxes->MBOX9.MSGCTRL.all=0x00000004;
	pECanMboxes->MBOX10.MSGCTRL.all=0x00000004;
	pECanMboxes->MBOX11.MSGCTRL.all=0x00000004;
	pECanMboxes->MBOX12.MSGCTRL.all=0x00000004;
	pECanMboxes->MBOX13.MSGCTRL.all=0x00000004;
	pECanMboxes->MBOX14.MSGCTRL.all=0x00000004;
	pECanMboxes->MBOX15.MSGCTRL.all=0x00000004;
	pECanMboxes->MBOX16.MSGCTRL.all=0x00000004;
	pECanMboxes->MBOX17.MSGCTRL.all=0x00000004;
	pECanMboxes->MBOX18.MSGCTRL.all=0x00000004;
	pECanMboxes->MBOX19.MSGCTRL.all=0x00000004;
	pECanMboxes->MBOX20.MSGCTRL.all=0x00000004;
	pECanMboxes->MBOX21.MSGCTRL.all=0x00000004;
	pECanMboxes->MBOX22.MSGCTRL.all=0x00000004;
	pECanMboxes->MBOX23.MSGCTRL.all=0x00000004;
	pECanMboxes->MBOX24.MSGCTRL.all=0x00000004;
	pECanMboxes->MBOX25.MSGCTRL.all=0x00000004;
	pECanMboxes->MBOX26.MSGCTRL.all=0x00000004;
	pECanMboxes->MBOX27.MSGCTRL.all=0x00000004;
	pECanMboxes->MBOX28.MSGCTRL.all=0x00000004;
	pECanMboxes->MBOX29.MSGCTRL.all=0x00000004;
	pECanMboxes->MBOX30.MSGCTRL.all=0x00000004;
	//Transmit box ctrl
	pECanMboxes->MBOX31.MSGCTRL.all=0x00000004;

	//Interrupt configure : Disable interrupts
	/*ECanbRegs.CANMIM.all=0xFFFFFFFF;
	ECanbRegs.CANMIL.all=0x00000000;
	ECanbShadow.CANGIM.all = 0;
	ECanbShadow.CANGIM.bit.GIL=1;
	ECanbShadow.CANGIM.bit.BOIM=1;
	ECanbShadow.CANGIM.bit.I0EN=0;
	ECanbShadow.CANGIM.bit.I1EN=0;
	ECanbRegs.CANGIM.all=ECanbShadow.CANGIM.all;*/
	//modified
	pECanRegs->CANMIM.all=0xFFFFFFFF;
	pECanRegs->CANMIL.all=0x00000000;
	ECanShadow.CANGIM.all = 0;
	ECanShadow.CANGIM.bit.GIL=1;
	ECanShadow.CANGIM.bit.BOIM=1;
	ECanShadow.CANGIM.bit.I0EN=0;
	ECanShadow.CANGIM.bit.I1EN=0;
	pECanRegs->CANGIM.all=ECanShadow.CANGIM.all;

	//Enable mailbox
	//ECanbRegs.CANME.all = 0xFFFFFFFF;
	//modified
	pECanRegs->CANME.all = 0xFFFFFFFF;

	EDIS;
}

void InitMonECanAGpio(void)	//ECan-A
{
   EALLOW;

/* Enable internal pull-up for the selected CAN pins */
// Pull-ups can be enabled or disabled by the user.
// This will enable the pullups for the specified pins.
// Comment out other unwanted lines.

//	GpioCtrlRegs.GPAPUD.bit.GPIO30 = 0;	    // Enable pull-up for GPIO30 (CANRXA)
	GpioCtrlRegs.GPAPUD.bit.GPIO18 = 0;	    // Enable pull-up for GPIO18 (CANRXA)

//	GpioCtrlRegs.GPAPUD.bit.GPIO31 = 0;	    // Enable pull-up for GPIO31 (CANTXA)
	GpioCtrlRegs.GPAPUD.bit.GPIO19 = 0;	    // Enable pull-up for GPIO19 (CANTXA)

/* Set qualification for selected CAN pins to asynch only */
// Inputs are synchronized to SYSCLKOUT by default.
// This will select asynch (no qualification) for the selected pins.

//  GpioCtrlRegs.GPAQSEL2.bit.GPIO30 = 3;   // Asynch qual for GPIO30 (CANRXA)
    GpioCtrlRegs.GPAQSEL2.bit.GPIO18 = 3;   // Asynch qual for GPIO18 (CANRXA)


/* Configure eCAN-A pins using GPIO regs*/
// This specifies which of the possible GPIO pins will be eCAN functional pins.

//	GpioCtrlRegs.GPAMUX2.bit.GPIO30 = 1;	// Configure GPIO30 for CANRXA operation
    GpioCtrlRegs.GPAMUX2.bit.GPIO18 = 3;	// Configure GPIO18 for CANRXA operation
//	GpioCtrlRegs.GPAMUX2.bit.GPIO31 = 1;	// Configure GPIO31 for CANTXA operation
    GpioCtrlRegs.GPAMUX2.bit.GPIO19 = 3;	// Configure GPIO19 for CANTXA operation

    EDIS;
}

void InitMonECanBGpio(void)	//ECan-B
{
   EALLOW;

/* Enable internal pull-up for the selected CAN pins */
// Pull-ups can be enabled or disabled by the user.
// This will enable the pullups for the specified pins.
// Comment out other unwanted lines.

//	GpioCtrlRegs.GPAPUD.bit.GPIO8 = 0;	  // Enable pull-up for GPIO8  (CANTXB)
    GpioCtrlRegs.GPAPUD.bit.GPIO12 = 0;   // Enable pull-up for GPIO12 (CANTXB)
//  GpioCtrlRegs.GPAPUD.bit.GPIO16 = 0;   // Enable pull-up for GPIO16 (CANTXB)
//    GpioCtrlRegs.GPAPUD.bit.GPIO20 = 0;   // Enable pull-up for GPIO20 (CANTXB)

//	GpioCtrlRegs.GPAPUD.bit.GPIO10 = 0;	  // Enable pull-up for GPIO10 (CANRXB)
    GpioCtrlRegs.GPAPUD.bit.GPIO13 = 0;   // Enable pull-up for GPIO13 (CANRXB)
//  GpioCtrlRegs.GPAPUD.bit.GPIO17 = 0;   // Enable pull-up for GPIO17 (CANRXB)
//    GpioCtrlRegs.GPAPUD.bit.GPIO21 = 0;   // Enable pull-up for GPIO21 (CANRXB)

/* Set qualification for selected CAN pins to asynch only */
// Inputs are synchronized to SYSCLKOUT by default.
// This will select asynch (no qualification) for the selected pins.
// Comment out other unwanted lines.

//  GpioCtrlRegs.GPAQSEL1.bit.GPIO10 = 3; // Asynch qual for GPIO10 (CANRXB)
    GpioCtrlRegs.GPAQSEL1.bit.GPIO13 = 3; // Asynch qual for GPIO13 (CANRXB)
//  GpioCtrlRegs.GPAQSEL2.bit.GPIO17 = 3; // Asynch qual for GPIO17 (CANRXB)
//    GpioCtrlRegs.GPAQSEL2.bit.GPIO21 = 3; // Asynch qual for GPIO21 (CANRXB)

/* Configure eCAN-B pins using GPIO regs*/
// This specifies which of the possible GPIO pins will be eCAN functional pins.

//	GpioCtrlRegs.GPAMUX1.bit.GPIO8 = 2;   // Configure GPIO8 for CANTXB operation
    GpioCtrlRegs.GPAMUX1.bit.GPIO12 = 2;  // Configure GPIO12 for CANTXB operation
//  GpioCtrlRegs.GPAMUX2.bit.GPIO16 = 2;  // Configure GPIO16 for CANTXB operation
//    GpioCtrlRegs.GPAMUX2.bit.GPIO20 = 3;  // Configure GPIO20 for CANTXB operation

//	GpioCtrlRegs.GPAMUX1.bit.GPIO10 = 2;  // Configure GPIO10 for CANRXB operation
    GpioCtrlRegs.GPAMUX1.bit.GPIO13 = 2;  // Configure GPIO13 for CANRXB operation
//  GpioCtrlRegs.GPAMUX2.bit.GPIO17 = 2;  // Configure GPIO17 for CANRXB operation
//    GpioCtrlRegs.GPAMUX2.bit.GPIO21 = 3;  // Configure GPIO21 for CANRXB operation

    EDIS;
}

#endif




