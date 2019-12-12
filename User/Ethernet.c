#include "DSP2833x_Device.h"     // DSP2833x Headerfile Include File
#include "DSP2833x_Examples.h"   // DSP2833x Examples Include File
#include "Main.h"
#include "Ethernet.h"
#include "Sample.h"
#include "CurCtrl.h"
#include "ParaCalc.h"



// LAN9118 Defines
#define TRUE    1
#define FALSE   0
static Uint32 g_pLAN911x = 0x4000UL;
static Uint32 dwPhyAddr = 0x01UL;
Uint32 IniLink;
static Uint32 g_chipRevision;

//for Ethernet
Uint16	TxEthnetFrameBuffer[999];
//Uint16	TxDebugEthnetFrameBuffer[99];
Uint16	ARPAnswerFrameBuffer[99];
Uint16	RxEthnetFrameBuffer[999];
Uint16  RecieveData[99];
Uint16  RecieveCmd;

Uint32 IniLAN;
Uint16 FramNum = 0;
Uint16 SMNum = 0;
int16 Cmd_Watch = 0;
int16 Cmd_Ctrl = 0;
Uint16 Send_Watch_Flag = 0;
Uint16 Send_Debug_Cnt = 0;

const Uint16 Nsm24IP[2]={0xa8c0,0x0200};	//Number of SMs
const Uint16 Nsm16IP[2]={0xa8c0,0x0400};
const Uint16 Nsm08IP[2]={0xa8c0,0x0600};
const Uint16 CPC24IP[2]={0xa8c0,0x0100};	//Control PC
const Uint16 CPC16IP[2]={0xa8c0,0x0300};
const Uint16 CPC08IP[2]={0xa8c0,0x0500};
Uint16 MyIP[2];
Uint16 RemoteIP[2];

const Uint16 Nsm24MAC[3] = {0xE000,0x51B4,0x19DC};
const Uint16 Nsm16MAC[3] = {0xFFFF,0xFFFF,0x04FF};
const Uint16 Nsm08MAC[3] = {0xFFFF,0xFFFF,0x06FF};
const Uint16 CPC24MAC[3] = {0xE000,0x51B4,0x17DC};
const Uint16 CPC16MAC[3] = {0x5BC8,0x9976,0x836B};
const Uint16 CPC08MAC[3] = {0x5BC8,0x9976,0x836B};
Uint16 MyMAC[3];
Uint16 RemoteMAC[3];

const Uint16 Nsm24Port = 3001;
const Uint16 Nsm16Port = 3002;
const Uint16 Nsm08Port = 3003;
const Uint16 CPC24Port = 2001;
const Uint16 CPC16Port = 2002;
const Uint16 CPC08Port = 2003;
Uint16 MyPort;
Uint16 RemotePort;

const Uint16 TxUDPDataNum = 151;//151;

Uint16 RecdFrameMAC[3];
Uint16 RecdFrameIP[2];
Uint16 RecdIpFrameLenth;
Uint16 TxFrameSize;
Uint16 ARPAnswerFrameSize;

extern enum CMDISR Cmd_Isr_glv;
extern enum STATISR Stat_Isr_glv;
extern enum STATERR Stat_ErrPrt_glv;

Uint16 SendSuccess = 0;
Uint16 erroron =0;

//调试界面数据观察
Uint32 k=0;
void Send_Debug_Frame()
{
	int cnt=0;
	int offset = 7;
	int powerflag = 0;
	if(CurMaxPU_gfv == 0.5)
	{
		powerflag = 200;
	}
	else if(CurMaxPU_gfv == 0.25)
	{
		powerflag = 100;
	}

//	Prepare_UDP_Head();
	//数据部分
	*(TxEthnetFrameBuffer+USER_DATA_START+0)=0xFF;  //debug帧标志位

	*(TxEthnetFrameBuffer+USER_DATA_START+1)= Cmd_Isr_glv;	//指令
	*(TxEthnetFrameBuffer+USER_DATA_START+2)= Stat_Isr_glv;	//状态
	*(TxEthnetFrameBuffer+USER_DATA_START+3)= Stat_ErrPrt_glv;	//错误状态

	*(TxEthnetFrameBuffer+USER_DATA_START+4)= VNTSM_glv[0];	//错误状态WNTSM_glv[4];	//
	*(TxEthnetFrameBuffer+USER_DATA_START+5)= WNTSM_glv[0];	//错误状态
	*(TxEthnetFrameBuffer+USER_DATA_START+6)= powerflag;  //功率标志位

	for(cnt = 0; cnt < 24; cnt++)
	{
		*(TxEthnetFrameBuffer+USER_DATA_START+offset+cnt) = 10 * UPVSM_gfv[cnt];
		*(TxEthnetFrameBuffer+USER_DATA_START+offset+24+cnt) = 10 * UNVSM_gfv[cnt];
		*(TxEthnetFrameBuffer+USER_DATA_START+offset+48+cnt) = 10 * VPVSM_gfv[cnt];
		*(TxEthnetFrameBuffer+USER_DATA_START+offset+72+cnt) = 10 * VNVSM_gfv[cnt];
		*(TxEthnetFrameBuffer+USER_DATA_START+offset+96+cnt) = 10 * WPVSM_gfv[cnt];
		*(TxEthnetFrameBuffer+USER_DATA_START+offset+120+cnt) = 10 * WNVSM_gfv[cnt];
	}


//	*(TxEthnetFrameBuffer+USER_DATA_START+5)= 100;//IsrTimeSendDebug_glv;  //
//
//	*(TxEthnetFrameBuffer+USER_DATA_START+6)= 10 * IVDC_gfv + 10000;  //
//	*(TxEthnetFrameBuffer+USER_DATA_START+7)= 10 * VDCR_gfv + 10000;  //
//	*(TxEthnetFrameBuffer+USER_DATA_START+8)= 10 * CurUDiff_gfv + 10000;  //
//	*(TxEthnetFrameBuffer+USER_DATA_START+9)=1;  //
//	*(TxEthnetFrameBuffer+USER_DATA_START+10)=1;  //
//	*(TxEthnetFrameBuffer+USER_DATA_START+11)=1;  //
//	*(TxEthnetFrameBuffer+USER_DATA_START+12)=1;  //
//	*(TxEthnetFrameBuffer+USER_DATA_START+13)=1;  //
//	*(TxEthnetFrameBuffer+USER_DATA_START+14)=1;  //
//	*(TxEthnetFrameBuffer+USER_DATA_START+15)=1;  //
//	*(TxEthnetFrameBuffer+USER_DATA_START+16)=1;  //

	SendSuccess=LAN911xSendFrame(TxEthnetFrameBuffer,TxFrameSize);//
}
//#pragma CODE_SECTION(Send_Debug_Frame,"ramfuncs");
//UDP数据帧处理
void Process_RxData_Frame(void)
{
	RecieveCmd = *(RxEthnetFrameBuffer+UDP_HEADER_START+4);
	switch (RecieveCmd)
	{
		case 10: Cmd_Watch = 0; break;
		case 11: Cmd_Watch = 1; Cmd_Ctrl = 0; break;
		case 20: Cmd_Ctrl = 0; break;
		case 21: Cmd_Ctrl = 1; Cmd_Watch = 0; break;
		case 101:if(Stat_Isr_glv == StatIsrPrep)
				{
					Cmd_Isr_glv = CmdIsrCharge;
				} break;
		case 102: if(Stat_Isr_glv == StatIsrStandby)
				{
					Cmd_Isr_glv = CmdIsrStart;
				} break;
		case 103: if(Stat_Isr_glv == StatIsrRun)
				{
					Cmd_Isr_glv = CmdIsrStop;
				} break;
		case 104: if(Stat_Isr_glv == StatIsrRun)
				{
					Cmd_Isr_glv = CmdIsrTrip;
				} break;
		//功率指令
		case 111:if(Stat_Isr_glv == StatIsrRun)
				{
					CurMaxPU_gfv =0.1;
				} break;
		case 112:if(Stat_Isr_glv == StatIsrRun)
				{
					CurMaxPU_gfv =0.3;
				} break;
		case 113:if(Stat_Isr_glv == StatIsrRun)
				{
					CurMaxPU_gfv =0.5;
				} break;
		case 114:if(Stat_Isr_glv == StatIsrRun)
				{
					CurMaxPU_gfv =0.8;
				} break;
		case 115:if(Stat_Isr_glv == StatIsrRun)
				{
					CurMaxPU_gfv =1.0;
				} break;
		default: break;
	}

	//try sendback to computer for debug
	//SendBack();
}
//#pragma CODE_SECTION(Process_RxData_Frame,"ramfuncs");
//===========================================
//UDP帧发送
//===========================================
void Send_Watch_Frame(Uint16 FrameNum,Uint16 SMNum)
{
	//calculate the upload parameters
	CurUDiff_gfv = CurADiffPU_gfv * CurBase_gfv;
	CurVDiff_gfv = CurBDiffPU_gfv * CurBase_gfv;
	CurWDiff_gfv = CurCDiffPU_gfv * CurBase_gfv;

	VolU_gfv = VolUPU_gfv * VolBase_gfv;
	VolV_gfv = VolVPU_gfv * VolBase_gfv;
	VolW_gfv = VolWPU_gfv * VolBase_gfv;


	//数据部分
	*(TxEthnetFrameBuffer+USER_DATA_START+0)=FrameNum;  //帧序号标志位

    *(TxEthnetFrameBuffer+USER_DATA_START+1)=10 * IVDC_gfv + 10000;  //DC Current
	*(TxEthnetFrameBuffer+USER_DATA_START+2)=10 * VDCR_gfv + 10000;  //DC Voltage

	*(TxEthnetFrameBuffer+USER_DATA_START+3)=0;		//阀侧d轴电流
	*(TxEthnetFrameBuffer+USER_DATA_START+4)=0;		//阀侧q轴电流
	*(TxEthnetFrameBuffer+USER_DATA_START+5)=0;		//阀侧d轴电压
	*(TxEthnetFrameBuffer+USER_DATA_START+6)=0;		//阀侧q轴电压

	*(TxEthnetFrameBuffer+USER_DATA_START+7)=10 * CurUDiff_gfv + 10000;  //Phase A Current
	*(TxEthnetFrameBuffer+USER_DATA_START+8)=10 * CurVDiff_gfv + 10000;  //Phase B Current
	*(TxEthnetFrameBuffer+USER_DATA_START+9)=10 * CurWDiff_gfv + 10000;  //Phase C Current
	*(TxEthnetFrameBuffer+USER_DATA_START+10)=0;		//D Axis Current
	*(TxEthnetFrameBuffer+USER_DATA_START+11)=0;		//Q Axis Current

	*(TxEthnetFrameBuffer+USER_DATA_START+12)=10 * VolU_gfv + 10000;  //Phase A Voltage
	*(TxEthnetFrameBuffer+USER_DATA_START+13)=10 * VolV_gfv + 10000;  //Phase B Voltage
	*(TxEthnetFrameBuffer+USER_DATA_START+14)=10 * VolW_gfv + 10000;  //Phase C Voltage
	*(TxEthnetFrameBuffer+USER_DATA_START+15)=0;	  //D Axis Voltage
	*(TxEthnetFrameBuffer+USER_DATA_START+16)=0;	  //Q Axis Voltage

	*(TxEthnetFrameBuffer+USER_DATA_START+17)=10 * IPA_gfv + 10000;   //Phase A Positive Arm Current
	*(TxEthnetFrameBuffer+USER_DATA_START+18)=10 * IAN_gfv + 10000;   //Phase A Negative Arm Current
	*(TxEthnetFrameBuffer+USER_DATA_START+19)=10 * IPB_gfv + 10000;   //Phase B Positive Arm Current
	*(TxEthnetFrameBuffer+USER_DATA_START+20)=10 * IBN_gfv + 10000;   //Phase B Negative Arm Current
	*(TxEthnetFrameBuffer+USER_DATA_START+21)=10 * IPC_gfv + 10000;   //Phase C Positive Arm Current
	*(TxEthnetFrameBuffer+USER_DATA_START+22)=10 * ICN_gfv + 10000;   //Phase C Negative Arm Current

	//子模块电压
	*(TxEthnetFrameBuffer+USER_DATA_START+23) = 10 * UPVSM_gfv[0];
	*(TxEthnetFrameBuffer+USER_DATA_START+24) = 10 * UPVSM_gfv[1];
	*(TxEthnetFrameBuffer+USER_DATA_START+25) = 10 * UNVSM_gfv[0];
	*(TxEthnetFrameBuffer+USER_DATA_START+26) = 10 * UNVSM_gfv[1];
	*(TxEthnetFrameBuffer+USER_DATA_START+27) = 10 * VPVSM_gfv[0];
	*(TxEthnetFrameBuffer+USER_DATA_START+28) = 10 * VPVSM_gfv[1];
	*(TxEthnetFrameBuffer+USER_DATA_START+29) = 10 * VNVSM_gfv[0];
	*(TxEthnetFrameBuffer+USER_DATA_START+30) = 10 * VNVSM_gfv[1];
	*(TxEthnetFrameBuffer+USER_DATA_START+31) = 10 * WPVSM_gfv[0];
	*(TxEthnetFrameBuffer+USER_DATA_START+32) = 10 * WPVSM_gfv[1];
	*(TxEthnetFrameBuffer+USER_DATA_START+33) = 10 * WNVSM_gfv[0];
	*(TxEthnetFrameBuffer+USER_DATA_START+34) = 10 * WNVSM_gfv[1];

	//子模块1温度
	*(TxEthnetFrameBuffer+USER_DATA_START+35)= 10 * UPTSM_glv[0];//10*Sintable[k]+400;
	*(TxEthnetFrameBuffer+USER_DATA_START+36)= 10 * UPTSM_glv[1];//10*Sintable[k]+400;

	//子模块编号
	*(TxEthnetFrameBuffer+USER_DATA_START+37)= SMNum;
	//编号电压、温度
	if(SMNum < 24/2)
	{
		*(TxEthnetFrameBuffer+USER_DATA_START+38)= 10 * UPVSM_gfv[2*SMNum];		//one SM has two cap	//voltage of cap
		*(TxEthnetFrameBuffer+USER_DATA_START+39)= 10 * UPVSM_gfv[2*SMNum+1];
		*(TxEthnetFrameBuffer+USER_DATA_START+40)= 10 * UPTSM_glv[2*SMNum];//10*Sintable[k]+380;	//temperature of cap
		*(TxEthnetFrameBuffer+USER_DATA_START+41)= 10 * UPTSM_glv[2*SMNum+1];//-10*Sintable[k]+420;
	}
	else if(SMNum < 24 )
	{
		*(TxEthnetFrameBuffer+USER_DATA_START+38)= 10 * UNVSM_gfv[2*(SMNum-24/2)];
		*(TxEthnetFrameBuffer+USER_DATA_START+39)= 10 * UNVSM_gfv[2*(SMNum-24/2)+1];
		*(TxEthnetFrameBuffer+USER_DATA_START+40)= 10 * UNTSM_glv[2*(SMNum-24/2)];//10*Sintable[k]+380;
		*(TxEthnetFrameBuffer+USER_DATA_START+41)= 10 * UNTSM_glv[2*(SMNum-24/2)+1];//-10*Sintable[k]+420;
	}
	else if(SMNum < 24 * 3 /2)
	{
		*(TxEthnetFrameBuffer+USER_DATA_START+38)= 10 * VPVSM_gfv[2*(SMNum-24)];
		*(TxEthnetFrameBuffer+USER_DATA_START+39)= 10 * VPVSM_gfv[2*(SMNum-24)+1];
		*(TxEthnetFrameBuffer+USER_DATA_START+40)=10 * VPTSM_glv[2*(SMNum-24)];//10*Sintable[k]+380;
		*(TxEthnetFrameBuffer+USER_DATA_START+41)=10 * VPTSM_glv[2*(SMNum-24)+1];//-10*Sintable[k]+420;
	}
	else if(SMNum < 24 * 2)
	{
		*(TxEthnetFrameBuffer+USER_DATA_START+38)= 10 * VNVSM_gfv[2*(SMNum-24 * 3 /2)];
		*(TxEthnetFrameBuffer+USER_DATA_START+39)= 10 * VNVSM_gfv[2*(SMNum-24 * 3 /2)+1];
		*(TxEthnetFrameBuffer+USER_DATA_START+40)=10 * VNTSM_glv[2*(SMNum-24 * 3 /2)];//10*Sintable[k]+380;
		*(TxEthnetFrameBuffer+USER_DATA_START+41)=10 * VNTSM_glv[2*(SMNum-24 * 3 /2)+1];//-10*Sintable[k]+420;
	}
	else if(SMNum < 24 * 5 /2 )
	{
		*(TxEthnetFrameBuffer+USER_DATA_START+38)= 10 * WPVSM_gfv[2*(SMNum-24 * 2)];
		*(TxEthnetFrameBuffer+USER_DATA_START+39)= 10 * WPVSM_gfv[2*(SMNum-24 * 2)+1];
		*(TxEthnetFrameBuffer+USER_DATA_START+40)=10 * WPTSM_glv[2*(SMNum-24 * 2)];//10*Sintable[k]+380;
		*(TxEthnetFrameBuffer+USER_DATA_START+41)=10 * WPTSM_glv[2*(SMNum-24 * 2)+1];//-10*Sintable[k]+420;
	}
	else if(SMNum < 24 * 3)
	{
		*(TxEthnetFrameBuffer+USER_DATA_START+38)= 10 * WNVSM_gfv[2*(SMNum-24 * 5 /2)];
		*(TxEthnetFrameBuffer+USER_DATA_START+39)= 10 * WNVSM_gfv[2*(SMNum-24 * 5 /2)+1];
		*(TxEthnetFrameBuffer+USER_DATA_START+40)=10 * WNTSM_glv[2*(SMNum-24 * 5 /2)];//10*Sintable[k]+380;
		*(TxEthnetFrameBuffer+USER_DATA_START+41)=10 * WNTSM_glv[2*(SMNum-24 * 5 /2)+1];//-10*Sintable[k]+420;
	}

	//其他调试数据  最大不要超过USER_DATA_START+50
/*	*(TxEthnetFrameBuffer+USER_DATA_START+42)=SMNum;
	*(TxEthnetFrameBuffer+USER_DATA_START+43)=SMNum;
	*(TxEthnetFrameBuffer+USER_DATA_START+44)=SMNum;
	*(TxEthnetFrameBuffer+USER_DATA_START+45)=SMNum;
	*(TxEthnetFrameBuffer+USER_DATA_START+46)=SMNum;
	*(TxEthnetFrameBuffer+USER_DATA_START+47)=SMNum;
	*(TxEthnetFrameBuffer+USER_DATA_START+48)=SMNum;
	*(TxEthnetFrameBuffer+USER_DATA_START+49)=SMNum;*/

    LAN911xSendFrame(TxEthnetFrameBuffer,TxFrameSize);//
}
//#pragma CODE_SECTION(Send_Watch_Frame,"ramfuncs");

//处理收到的以太网帧
void Process_Ethernet_Frame(void)
{
    Uint16 FrameType,IpHeaderLenth,ProtocolType,TargetIP[2];

    memcpy(&RecdFrameMAC,(RxEthnetFrameBuffer+ETH_HEADER_START+3),3); //缓存上位机mac
    FrameType = *(RxEthnetFrameBuffer+ETH_HEADER_START+6);  //获取协议类型

    if(FrameType == Frame_ARP)
    {
    	if(*(RxEthnetFrameBuffer+ARP_HEADER_START) == HARDW_ETH)  //以太网
		 {
			  if(*(RxEthnetFrameBuffer+ARP_HEADER_START+1) == Frame_IP) //IP协议
			  {
				   if(*(RxEthnetFrameBuffer+ARP_HEADER_START+2) == IP_HLEN_PLEN) //IP/MAC长度
				   {
						if(*(RxEthnetFrameBuffer+ARP_HEADER_START+3) == OP_ARP_REQUEST) //ARP请求帧
						{
							 memcpy(&RecdFrameIP,(RxEthnetFrameBuffer+ARP_HEADER_START+7),2); //缓存上位机IP
							 memcpy(&TargetIP,(RxEthnetFrameBuffer+ARP_HEADER_START+12),2); //缓存目标IP
							 if((MyIP[0] == TargetIP[0]) && (MyIP[1] == TargetIP[1]))  //寻找本机IP
							 {
								 ArpAnswer();    //ARP 应答，发送本机MAC地址
							 }
						}
				   }
			  }
		 }

    }
    else if(FrameType == Frame_IP) //判断是否IP帧
    {
		IpHeaderLenth = *(RxEthnetFrameBuffer+IP_HEADER_START); //获取IP头部长度信息
		if((IpHeaderLenth&0x00FF) == Ip_Edition)   //IP头部长度信息为0x4500
		{
		   IpHeaderLenth=5*(32/8)/2;  //10dw
		   CopyFrameFromBE(IP_HEADER_START+1,IpHeaderLenth-1-4);
		   RecdIpFrameLenth = *(RxEthnetFrameBuffer+IP_HEADER_START+1)/2;  //IP帧总长度
		   ProtocolType = *(RxEthnetFrameBuffer+IP_HEADER_START+4)&0x00FF; //获取传输协议UDP
		   memcpy(&RecdFrameIP,(RxEthnetFrameBuffer+IP_HEADER_START+6),2); //缓存上位机IP
		   if(ProtocolType == PROTOCOL_UDP)
		   {
			   if(*(RxEthnetFrameBuffer+UDP_HEADER_START+1) == SwapWord(MyPort))
			   {
				   Process_RxData_Frame();
			   }

		   }
		}
    }
}
//#pragma CODE_SECTION(Process_Ethernet_Frame,"ramfuncs");

void SendBack(void)
{
	*(TxEthnetFrameBuffer+USER_DATA_START) = RecieveCmd;
	LAN911xSendFrame(TxEthnetFrameBuffer,TxFrameSize);
}
//#pragma CODE_SECTION(SendBack,"ramfuncs");


//准备Ethernet/IP帧头部
void Prepare_UDP_Head()
{
	Uint16 CalcCkSum;
	//Ethernet首部
	*(TxEthnetFrameBuffer+ETH_HEADER_START+0) = RemoteMAC[0];
	*(TxEthnetFrameBuffer+ETH_HEADER_START+1) = RemoteMAC[1];
	*(TxEthnetFrameBuffer+ETH_HEADER_START+2) = RemoteMAC[2];

	*(TxEthnetFrameBuffer+ETH_HEADER_START+3) = MyMAC[0];
	*(TxEthnetFrameBuffer+ETH_HEADER_START+4) = MyMAC[1];
	*(TxEthnetFrameBuffer+ETH_HEADER_START+5) = MyMAC[2];

	*(TxEthnetFrameBuffer+ETH_HEADER_START+6) = Frame_IP;	//SwapWord(Frame_IP);

	//IP首部
	*(TxEthnetFrameBuffer+IP_HEADER_START+0) = Ip_Edition;	//SwapWord(Ip_Edition); //IP版本，IP数据包首部长度，8位服务类型
	*(TxEthnetFrameBuffer+IP_HEADER_START+1) = SwapWord((TxUDPDataNum+IP_HEADER_SIZE+UDP_HEADER_SIZE)<<1); //总长度，字节数
	*(TxEthnetFrameBuffer+IP_HEADER_START+2) = 0x3930;	//SwapWord(0x3039);
	*(TxEthnetFrameBuffer+IP_HEADER_START+3) = 0x0000;	//SwapWord(0x0000);
	*(TxEthnetFrameBuffer+IP_HEADER_START+4) = SwapWord((DEFUALT_TTL<<8)|PROTOCOL_UDP);
	*(TxEthnetFrameBuffer+IP_HEADER_START+5) = 0x0000;

	*(TxEthnetFrameBuffer+IP_HEADER_START+6) = MyIP[0];
	*(TxEthnetFrameBuffer+IP_HEADER_START+7) = MyIP[1];

	*(TxEthnetFrameBuffer+IP_HEADER_START+8) = RemoteIP[0];
	*(TxEthnetFrameBuffer+IP_HEADER_START+9) = RemoteIP[1];

	CalcCkSum=CalcCheckSum((TxEthnetFrameBuffer+IP_HEADER_START),IP_HEADER_SIZE,0,0);
	if(!CalcCkSum)
	{
		CalcCkSum=0xFFFF;
	}
	*(TxEthnetFrameBuffer+IP_HEADER_START+5) = CalcCkSum;

	//UDP首部
	*(TxEthnetFrameBuffer+UDP_HEADER_START+0) = SwapWord(MyPort);
	*(TxEthnetFrameBuffer+UDP_HEADER_START+1) = SwapWord(RemotePort);
	*(TxEthnetFrameBuffer+UDP_HEADER_START+2) = SwapWord((TxUDPDataNum+UDP_HEADER_SIZE)<<1);
	*(TxEthnetFrameBuffer+UDP_HEADER_START+3) = 0x0000;

	TxFrameSize=ETH_HEADER_SIZE+IP_HEADER_SIZE+UDP_HEADER_SIZE+TxUDPDataNum;
}
//ARP应答
void ArpAnswer(void)
{
	LAN911xSendFrame(ARPAnswerFrameBuffer,ARPAnswerFrameSize);
}
//#pragma CODE_SECTION(ArpAnswer,"ramfuncs");

void Prepare_Arp_Answer(void)
{
    memcpy((ARPAnswerFrameBuffer+ETH_HEADER_START+0), &RemoteMAC, 3); //上位机笔记本mac地址
    memcpy((ARPAnswerFrameBuffer+ETH_HEADER_START+3), &MyMAC, 3);      //本地MAC地址
    *(ARPAnswerFrameBuffer+ETH_HEADER_START+6) = Frame_ARP;	//SwapWord(Frame_ARP);  //0x0806 ARP帧类型
    *(ARPAnswerFrameBuffer+ARP_HEADER_START+0) = HARDW_ETH;	//SwapWord(HARDW_ETH); //硬件类型0x0001
    *(ARPAnswerFrameBuffer+ARP_HEADER_START+1) = Frame_IP;	//SwapWord(Frame_IP);  //ip协议代号0x0800
    *(ARPAnswerFrameBuffer+ARP_HEADER_START+2) = IP_HLEN_PLEN;	//SwapWord(IP_HLEN_PLEN); //mac长度与IP地址长度0x0604
    *(ARPAnswerFrameBuffer+ARP_HEADER_START+3) = OP_ARP_ANSWER;	//SwapWord(OP_ARP_ANSWER); //ARP应答帧 0x0002
    memcpy((ARPAnswerFrameBuffer+ARP_HEADER_START+4), &MyMAC, 3);    //本地MAC地址
    memcpy((ARPAnswerFrameBuffer+ARP_HEADER_START+7), &MyIP, 2);     //本机IP
    memcpy((ARPAnswerFrameBuffer+ARP_HEADER_START+9), &RemoteMAC, 3); //上位机MAC
    memcpy((ARPAnswerFrameBuffer+ARP_HEADER_START+12), &RemoteIP, 2); //上位机IP
    ARPAnswerFrameSize=ETH_HEADER_SIZE+ARP_FRAME_SIZE+1;    //加1！！！长度为奇数
}

//求和校验 TCP和UDP求和校验
Uint16 CalcCheckSum(Uint16 *Start, Uint16 Count, Uint16 IsTCP, Uint16 IsUDP)
{
    volatile Uint32 Sum;

    Sum = 0;
    if ((IsTCP)||(IsUDP)) //伪首部
    {
        Sum += (MyIP[0]);
        Sum += (MyIP[1]);
        Sum += (RemoteIP[0]);
        Sum += (RemoteIP[1]);
        Sum += SwapWord(Count*2);
        if(IsTCP)
             Sum += SwapWord(PROTOCOL_TCP);
        else
             Sum += SwapWord(PROTOCOL_UDP);
    }
    while (Count > 0)
    {
        Sum += *Start;
        Start ++;
        Count --;
    }

    while(Sum&0xFFFF0000)
    {
    	Sum = (Sum & 0xFFFF) + ((Sum >> 16)&0xFFFF);
    }
    return((Uint16)((~Sum)&0xFFFF));
}
//字节序交换
Uint16 SwapWord(Uint16 Data)
{
    return((Data>>8)|(Data<<8));
}
void CopyFrameFromBE(Uint16 Offset,Uint16 Size)
{
    Uint16 i,Temp;
    for(i=0;i<Size;i++)
    {
         Temp=*(RxEthnetFrameBuffer+Offset+i);
         *(RxEthnetFrameBuffer+Offset+i)=SwapWord(Temp);
    }
}


// LAN9118 Functions
void SetRegDW(Uint32 dwBase, Uint32 dwOffset, Uint32 dwVal)    //系统控制状态寄存器(CSR)写
{
	Uint32 dwAddrSet;
	dwAddrSet = dwBase + (dwOffset>>1);
	(*(Uint16 *)(dwAddrSet)) = dwVal & 0x0000ffff;
	(*(Uint16 *)(dwAddrSet + 1)) = (dwVal & 0xffff0000)>>16;
}
Uint32 GetRegDW(Uint32 dwBase, Uint32 dwOffset)                //系统控制状态寄存器(CSR)读
{
	Uint32 datalow;
	Uint32 datahigh;
	Uint32 dwAddrGet;
	dwAddrGet = dwBase + (dwOffset>>1);
	datalow = (*(Uint16 *)(dwAddrGet)) & 0x0000ffff;
	datahigh = (*(Uint16 *)(dwAddrGet + 1)) & 0x0000ffff;
	return (Uint32)((datahigh<<16) + datalow);
}
void WriteFifo(const Uint32 dwBase, const Uint32 dwOffset, Uint16 *pdwBuf, Uint32 dwUint16Count)
{
	Uint32 dwAddrSet;
	dwAddrSet = dwBase + (dwOffset>>1);
		while (dwUint16Count)
		{
			(*(Uint16 *)(dwAddrSet)) = *(pdwBuf);
			(*(Uint16 *)(dwAddrSet + 1)) = *(pdwBuf+1);
			pdwBuf = pdwBuf+2;
			dwUint16Count--;
		}
}
void ReadFifo(const Uint32 dwBase, const Uint32 dwOffset, Uint16 *pdwBuf, Uint32 dwUint16Count)
{
	Uint32 dwAddrSet;
	dwAddrSet = dwBase + (dwOffset>>1);

	while (dwUint16Count)
	{
		*(pdwBuf++) = (*(Uint16 *)(dwAddrSet));
		*(pdwBuf++) = (*(Uint16 *)(dwAddrSet+1));
		dwUint16Count--;
	}
}

int Lan_MacNotBusy(const Uint32 dwLanBase)
{
	int i=0;
	for(i=0;i<40;i++)
	{
		if((GetRegDW(dwLanBase, MAC_CSR_CMD) & MAC_CSR_CMD_CSR_BUSY_)==(0UL))
		{
			return 1;    //not busy
		}
	}
	return 0;
}
void Lan_SetMacRegDW(const Uint32 dwLanBase, const Uint32 dwOffset, const Uint32 dwVal)
{
	if (GetRegDW(dwLanBase, MAC_CSR_CMD) & MAC_CSR_CMD_CSR_BUSY_)
	{
		return;
	}
	SetRegDW(dwLanBase, MAC_CSR_DATA, dwVal);
	SetRegDW(dwLanBase, MAC_CSR_CMD, ((dwOffset & 0x000000FFUL) | MAC_CSR_CMD_CSR_BUSY_)); //Busy位写1，将数据写入对应寄存器
}
Uint32 Lan_GetMacRegDW(const Uint32 dwLanBase, const Uint32 dwOffset)
{
	Uint32	dwRet;
	if (GetRegDW(dwLanBase, MAC_CSR_CMD) & MAC_CSR_CMD_CSR_BUSY_)
	{
		return 0xFFFFFFFFUL;
	}
	SetRegDW(dwLanBase, MAC_CSR_CMD, ((dwOffset & 0x000000FFUL) | MAC_CSR_CMD_CSR_BUSY_ | MAC_CSR_CMD_R_NOT_W_));
	if (!Lan_MacNotBusy(dwLanBase))
	{
		dwRet = 0xFFFFFFFFUL;
	}
	else
	{
		dwRet = GetRegDW(dwLanBase, MAC_CSR_DATA);
	}
	return dwRet;
}

void Lan_SetPhyRegW(const Uint32 dwLanBase, const Uint32 dwPhyAddress, const Uint32 dwMiiIndex, const Uint16 wVal)
{
	Uint32 dwAddr;
	int i=0;
	if ((Lan_GetMacRegDW(dwLanBase, MII_ACC) & MII_ACC_MII_BUSY_) != 0UL)
	{
		return;
	}
	Lan_SetMacRegDW(dwLanBase, MII_DATA, (Uint32)wVal);
	dwAddr = ((dwPhyAddress & 0x01UL)<<11) | ((dwMiiIndex & 0x1FUL)<<6) | MII_ACC_MII_WRITE_;
	Lan_SetMacRegDW(dwLanBase, MII_ACC, dwAddr);
	for(i=0;i<100;i++)
	{
		if ((Lan_GetMacRegDW(dwLanBase, MII_ACC) & MII_ACC_MII_BUSY_) == 0UL)
		{
			return;
		}
	}
	return;
}
Uint16 Lan_GetPhyRegW(const Uint32 dwLanBase, const Uint32 dwPhyAddress, const Uint32 dwMiiIndex)
{
	Uint32 dwAddr;
	Uint16 wRet = (Uint16)0xFFFF;
	int i=0;
	if ((Lan_GetMacRegDW(dwLanBase, MII_ACC) & MII_ACC_MII_BUSY_) != 0UL)
	{
		return (Uint16)0;
	}
	dwAddr = ((dwPhyAddress & 0x01UL)<<11) | ((dwMiiIndex & 0x1FUL)<<6);
	Lan_SetMacRegDW(dwLanBase, MII_ACC, dwAddr);
	for(i=0;i<100;i++)
	{
		if ((Lan_GetMacRegDW(dwLanBase, MII_ACC) & MII_ACC_MII_BUSY_) == 0UL)
		{
			wRet = ((Uint16)Lan_GetMacRegDW(dwLanBase, MII_DATA));
			break;
		}
	}
	if (i == 100)
	{
		wRet = ((Uint16)0xFFFFU);
	}
	return wRet;
}

Uint32 Lan_GetLinkMode(const Uint32 dwLanBase)
{
	Uint32 result = LINK_NO_LINK;
	const Uint16 wRegBSR = Lan_GetPhyRegW(dwLanBase, dwPhyAddr, PHY_BSR);
	if(wRegBSR & PHY_BSR_LINK_STATUS_)
	{
		Uint16 wTemp;
		const Uint16 wRegADV = Lan_GetPhyRegW(dwLanBase, dwPhyAddr, PHY_ANEG_ADV);
		const Uint16 wRegLPA = Lan_GetPhyRegW(dwLanBase, dwPhyAddr, PHY_ANEG_LPA);
		wTemp = (Uint16)(wRegLPA & wRegADV);

		if(wTemp & PHY_ANEG_LPA_100FDX_)
		{
			result = LINK_100MPS_FULL;
		}
		else if(wTemp & PHY_ANEG_LPA_100HDX_)
		{
			result = LINK_100MPS_HALF;
		}
		else if(wTemp & PHY_ANEG_LPA_10FDX_)
		{
			result = LINK_10MPS_FULL;
		}
		else if(wTemp & PHY_ANEG_LPA_10HDX_)
		{
			result = LINK_10MPS_HALF;
		}
	}
	return result;
}
//------------------------------------------------------------------------------
void LAN911xEnableInts()
{
	volatile Uint32	dwTemp;

	// enable master IRQ_EN
	dwTemp = GetRegDW(g_pLAN911x, INT_CFG); //获取原始IRQ_CFG寄存器值
	dwTemp |= INT_CFG_IRQ_EN_;      //INT_CFG_IRQ_EN为0x0000 0100UL第8bit为总中断使能位
	SetRegDW(g_pLAN911x, INT_CFG, dwTemp);

	// clear pending one before enable
	SetRegDW(g_pLAN911x, INT_STS, INT_STS_RSFL_); //清RX Status FIFO Full Interrupt位

	//设置RSFL的中断RX_STS的level
	dwTemp = GetRegDW(g_pLAN911x, FIFO_INT); //
	dwTemp |= FIFO_INT_RX_STS_LEVEL_;      //
	SetRegDW(g_pLAN911x, FIFO_INT, dwTemp);

    // Only enable receive interrupts (we poll for Tx completion)
	dwTemp = GetRegDW(g_pLAN911x, INT_EN);
	dwTemp |= INT_EN_RSFL_EN_;     //INT_EN_RSFL_EN_第4bit为RX Status FIFO Full Interrupt使能
	SetRegDW(g_pLAN911x, INT_EN, dwTemp);
}
int Lan_Initialize()
{
	Uint16 dwLanBase = 0x4000UL;
	int Initresult = FALSE;
	Uint32 dwTemp, dwTimeOut, dwRegVal;
	Uint16 wTemp;
	int delaycnt;

	InitXintf();	//set peripheral speed

    // Chip settle time.
    DELAY_US(750000);

	// Reset the Chip
	SetRegDW(dwLanBase, HW_CFG, HW_CFG_SRST_);
	dwTimeOut = 100000UL;
	do
	{
    	DELAY_US(10);
		dwTemp = GetRegDW(dwLanBase, HW_CFG);	//正确结果是0005 0000，错误结果全是0000 0000
		dwTimeOut--;
	}
	while((dwTimeOut-- > 0UL) && (dwTemp & HW_CFG_SRST_));  //HW_CFG_SRST_ is self-clearing

	if (dwTemp & HW_CFG_SRST_)     //SRST在正常复位后会自动清零
	{
		return FALSE;
	}

	//according to Nsm, choose IP/MAC/Port
	if(NumTotal_gulv == HBSM25L || FBSM13L)
	{
		memcpy(&MyIP,&Nsm24IP,2);
		memcpy(&MyMAC,&Nsm24MAC,3);
		MyPort = Nsm24Port;
		memcpy(&RemoteIP,&CPC24IP,2);
		memcpy(&RemoteMAC,&CPC24MAC,3);
		RemotePort = CPC24Port;
	}
	else if(NumTotal_gulv == HBSM17L)
	{
		memcpy(&MyIP,&Nsm16IP,2);
		memcpy(&MyMAC,&Nsm16MAC,3);
		MyPort = Nsm16Port;
		memcpy(&RemoteIP,&CPC16IP,2);
		memcpy(&RemoteMAC,&CPC16MAC,3);
		RemotePort = CPC16Port;
	}
	else if(NumTotal_gulv == HBSM9L)
	{
		memcpy(&MyIP,&Nsm08IP,2);
		memcpy(&MyMAC,&Nsm08MAC,3);
		MyPort = Nsm08Port;
		memcpy(&RemoteIP,&CPC08IP,2);
		memcpy(&RemoteMAC,&CPC08MAC,3);
		RemotePort = CPC08Port;
	}

	//set MAC address
	dwTemp = ((Uint32)((Uint16)MyMAC[1])<<16) | MyMAC[2];
	Lan_SetMacRegDW(dwLanBase, ADDRH, MyMAC[0]);
	Lan_SetMacRegDW(dwLanBase, ADDRL, dwTemp);		//(MyMAC[1]<<16)|MyMAC[2]

    // read the chip ID and revision.
	g_chipRevision = GetRegDW(dwLanBase, ID_REV);

	if (((g_chipRevision & 0x0FF00000) == 0x01100000) ||
		((g_chipRevision & 0xFF000000) == 0x11000000) ||
		((g_chipRevision & 0xFFF00000) == 0x92100000) ||
		((g_chipRevision & 0xFFF00000) == 0x92200000) ||
		((g_chipRevision & 0xFF0F0000) == 0x210A0000) ||
		((g_chipRevision & 0xFFF00000) == 0x93100000))
	{
		;
	}
	else
	{
		return FALSE;
	}

	// reset PHY
	Lan_SetPhyRegW(dwLanBase, dwPhyAddr, PHY_BCR, PHY_BCR_RESET_);

	dwTimeOut = 1000UL;
	do
	{
		DELAY_US(1000);			// 10mSec
		dwTemp = (Uint32)Lan_GetPhyRegW(dwLanBase, dwPhyAddr, PHY_BCR);
		dwTimeOut--;
	}
	while ((dwTimeOut>0UL) && (dwTemp & (Uint32)PHY_BCR_RESET_));

	if (dwTemp & (Uint32)PHY_BCR_RESET_)
	{
		return FALSE;
	}

	// Auto Neg
	Lan_SetPhyRegW(dwLanBase, dwPhyAddr, PHY_BCR, PHY_BCR_AUTO_NEG_ENABLE_);
	wTemp = Lan_GetPhyRegW(dwLanBase, dwPhyAddr, PHY_ANEG_ADV);
	wTemp &= ~(PHY_ANEG_ADV_PAUSE_OP_ | PHY_ANEG_ADV_SPEED_);
	wTemp |= PHY_ANEG_ADV_10H_| PHY_ANEG_ADV_10F_| PHY_ANEG_ADV_100H_| PHY_ANEG_ADV_100F_;
	Lan_SetPhyRegW(dwLanBase, dwPhyAddr, PHY_ANEG_ADV, wTemp);
	Lan_SetPhyRegW(dwLanBase, dwPhyAddr, PHY_BCR, PHY_BCR_AUTO_NEG_ENABLE_ | PHY_BCR_RESTART_AUTO_NEG_);

	dwTimeOut = 10000;
	do {
		DELAY_US(1000);			// 1mSec
		wTemp = Lan_GetPhyRegW(dwLanBase, dwPhyAddr, PHY_BSR);
	}
	while ((dwTimeOut-- > 0UL) && !((wTemp & (Uint16)(PHY_BSR_REMOTE_FAULT_|PHY_BSR_AUTO_NEG_COMP_))));

	dwTemp = Lan_GetLinkMode(dwLanBase);

	dwRegVal = Lan_GetMacRegDW(dwLanBase, MAC_CR);
	dwRegVal &= ~(MAC_CR_FDPX_|MAC_CR_RCVOWN_);

	switch(dwTemp)
	{
		case LINK_NO_LINK:
			break;
		case LINK_10MPS_HALF:
			dwRegVal|=MAC_CR_RCVOWN_;
			break;
		case LINK_10MPS_FULL:
			dwRegVal|=MAC_CR_FDPX_;
			break;
		case LINK_100MPS_HALF:
			dwRegVal|=MAC_CR_RCVOWN_;
			break;
		case LINK_100MPS_FULL:
			dwRegVal|=MAC_CR_FDPX_;
			break;
	}

	Lan_SetMacRegDW(dwLanBase, MAC_CR, dwRegVal);

	// initialize TX
	dwTemp = Lan_GetMacRegDW(dwLanBase, MAC_CR);
	dwTemp |= (MAC_CR_TXEN_);
	Lan_SetMacRegDW(dwLanBase, MAC_CR, dwTemp); //enable mac's transmitter
	dwTemp = Lan_GetMacRegDW(dwLanBase, MAC_CR);

	//setup TLI store-and-forward, and preserve TxFifo size
	dwTemp = GetRegDW(dwLanBase, HW_CFG);
	dwTemp |= HW_CFG_MBO_;                       //must be one
	SetRegDW(dwLanBase, HW_CFG, dwTemp);

	dwTemp = GetRegDW(dwLanBase, HW_CFG);
	dwTemp |= 0x00060000UL;//HW_CFG_TX_FIF_SZ_;//Set TX FIFO size to maximum
	SetRegDW(dwLanBase, HW_CFG, dwTemp);

	dwTemp = GetRegDW(dwLanBase, TX_CFG);
	dwTemp |=TX_CFG_TX_ON_;
	SetRegDW(dwLanBase, TX_CFG, dwTemp);	//enable transmission



	//enable RX IRQ
//	LAN911xEnableInts();
	// enable master IRQ_EN
	dwTemp = GetRegDW(g_pLAN911x, INT_CFG); //获取原始IRQ_CFG寄存器值
	dwTemp |= INT_CFG_IRQ_EN_;      //INT_CFG_IRQ_EN为0x0000 0100UL第8bit为总中断使能位
	SetRegDW(g_pLAN911x, INT_CFG, dwTemp);

	// clear pending one before enable
	SetRegDW(g_pLAN911x, INT_STS, INT_STS_RSFL_); //清RX Status FIFO Full Interrupt位

	//设置RSFL的中断RX_STS的level
	dwTemp = GetRegDW(g_pLAN911x, FIFO_INT); //
	dwTemp |= FIFO_INT_RX_STS_LEVEL_;      //
	SetRegDW(g_pLAN911x, FIFO_INT, dwTemp);

	// Only enable receive interrupts (we poll for Tx completion)
	dwTemp = GetRegDW(g_pLAN911x, INT_EN);
	dwTemp |= INT_EN_RSFL_EN_;     //INT_EN_RSFL_EN_第4bit为RX Status FIFO Full Interrupt使能
	SetRegDW(g_pLAN911x, INT_EN, dwTemp);

	// initialize RX
//	SetRegDW(dwLanBase, RX_CFG, 0x00000000UL);
	dwTemp = Lan_GetMacRegDW(dwLanBase, MAC_CR);
	dwTemp |= MAC_CR_RXEN_;
	Lan_SetMacRegDW(dwLanBase, MAC_CR, dwTemp); //enable mac's receiver

	//Enable LEDs
	SetRegDW(dwLanBase, GPIO_CFG, GPIO_CFG_LED3_EN_|GPIO_CFG_LED2_EN_|GPIO_CFG_LED1_EN_);

	DELAY_US(10000);
	IniLink = Lan_GetPhyRegW(dwLanBase, dwPhyAddr, PHY_BSR);

	if(IniLink & PHY_BSR_LINK_STATUS_)
	{
		for(delaycnt = 0; delaycnt < 200; delaycnt++)
		{
			DELAY_US(10000);
		}
	}

	//初始化准备好UDP、ARP帧头部，缩减DSP读写时间。
	Prepare_Arp_Answer();
	Prepare_UDP_Head();

	Initresult = TRUE;

    return Initresult;
}


Uint16 LAN911xSendFrame(Uint16 *pBuffer, Uint16 length)
{
//	Uint32 dwCounter;
	Uint32 dwTemp;
	Uint32 dwTxCmdA, dwTxCmdB;
//	Uint32 dwSpcNeed, dwTxSpc;
	static Uint32 dwSeqNum = 0x07;

	// wait until space available
//	dwCounter = 0;
//	dwSpcNeed = length<<1; //length已经包含了头部长度
//	while (1)
//	{
//		dwTxSpc = GetRegDW(g_pLAN911x, TX_FIFO_INF) & TX_FIFO_INF_TDFREE_;
//		if (dwSpcNeed <= dwTxSpc)
//		{
//			break;
//		}
//		//DELAY_US(10);
//		if (dwCounter++ > 10)
//		{
//			return FALSE;    //没有足够的空间
//		}
//	}

	dwTxCmdA = length<<1;   //start offset设置为0，end alighment为4byte
	dwTxCmdA |= (TX_CMD_A_INT_FIRST_SEG_ | TX_CMD_A_INT_LAST_SEG_);  //只有一个buffer
	dwTxCmdB = length<<1;
	dwTxCmdB |= (dwSeqNum << 16);  //package tag
	dwSeqNum ++;
	SetRegDW(g_pLAN911x, TX_DATA_FIFO_PORT, dwTxCmdA);
	SetRegDW(g_pLAN911x, TX_DATA_FIFO_PORT, dwTxCmdB);  //先将cmd压入FIFO

	WriteFifo(g_pLAN911x, TX_DATA_FIFO_PORT, (Uint16 *)pBuffer, length>>1);  //写FIFO时按照DWORD长度计数

	dwTemp = GetRegDW(g_pLAN911x, TX_FIFO_INF);
	if ((dwTemp >> 16) & 0xFFFF) //为什么原来是注释掉的？？？20170413
	{
		dwTemp = GetRegDW(g_pLAN911x, TX_STATUS_FIFO_PORT); //TX_STATUS_FIFO_PORT一次破坏性读取
		if (dwTemp & TX_STS_ES) //error state
		{
			return FALSE;
		}
		else
		{
			return TRUE;	//transmission success
		}
	}
	return FALSE;
}
//#pragma CODE_SECTION(LAN911xSendFrame,"ramfuncs");

void LAN911xGetFrame(Uint16 *pBuffer)
{
	Uint32 Int_status,Rx_FIFO_Info;
	Uint32	RxStatus, pkt_len;
	Int_status = GetRegDW(g_pLAN911x, INT_STS);
	//先读取CSR里的由RX_status_FIFO full引发的中断状态位有没有set，有表明接收到数据待处理
	if (Int_status & INT_STS_RSFL_)
	{
		//CSR里的RX_FIFO_INF存储了RX_STS和RX_DATA使用空间
		Rx_FIFO_Info = GetRegDW(g_pLAN911x, RX_FIFO_INF);
		if ((Rx_FIFO_Info & 0x00FF0000) >> 16) //RX_STS使用空间非零
		{
			RxStatus = GetRegDW(g_pLAN911x, RX_STATUS_FIFO_PORT);  //读取RX_STATUS_FIFO_PORT(破坏性读取)
			if (!(RxStatus & RX_STS_ES))  //读取error status，未出错，继续读RX_DATA_FIFO
			{
				pkt_len = (RxStatus & 0x3FFF0000) >>16;  //获取数据长度,byte计数
				ReadFifo(g_pLAN911x, RX_DATA_FIFO_PORT, pBuffer, (pkt_len+3)>>2);	//DW计数
				Process_Ethernet_Frame();	//KEY CODE
			}
			// clear pending Intr
//			SetRegDW(g_pLAN911x, INT_STS, INT_STS_RSFL_);	//error code
		}
	}
	//RX error
/*	if (GetRegDW(g_pLAN911x, INT_STS) & INT_STS_RXE_) //error code
	{
		SetRegDW(g_pLAN911x, INT_STS, INT_STS_RXE_);
	}*/
}
//#pragma CODE_SECTION(LAN911xGetFrame,"ramfuncs");
