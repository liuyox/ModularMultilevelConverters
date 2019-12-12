/*
 * CurCtrl.h
 *
 *  Created on: 2016-7-19
 *      Author: Administrator
 */

#ifndef CURCTRL_H_
#define CURCTRL_H_

#include "clarke.h"             // DMC head files
#include "park.h"               // DMC head files
#include "ipark.h"              // DMC head files
#include "iclarke.h"            // DMC head files
#include "svgen.h"              // DMC head files
#include "tf_order2.h"			// DMC head files
#include "pid_reg3.h"			// DMC head files
#include "rampgen.h"            // DMC head files
#include "main.h"

extern void UITrans(void);
extern void SeqJudge(void);
extern void SeqTrans(void);
extern void ThetaEst(void);
extern void CurCtrl(void);

extern float32 VolUPU_gfv;
extern float32 VolVPU_gfv;
extern float32 VolWPU_gfv;
extern float32 CurUDiffPU_gfv;
extern float32 CurVDiffPU_gfv;
extern float32 CurWDiffPU_gfv;
extern float32 CurUComPU_gfv;
extern float32 CurVComPU_gfv;
extern float32 CurWComPU_gfv;

//==========Sequence Judge==========
extern volatile enum CMDMOD   Cmd_SeqJudge_glv;
extern volatile enum STATMOD  Stat_SeqJudge_glv;
extern int32   Cnt_SJDir_glv;
extern float32 Sum_SJPrdt_gfv;
extern float32 SJVolPrevBePU_gfv;
extern CLARKE  SJVol_CLARKE;

//==========Sequence Transformation==========
//UVW is from hardware, ABC is in positive sequence
extern volatile enum CMDMOD  Cmd_SeqTrans_glv;
extern float32 VolAPU_gfv;
extern float32 VolBPU_gfv;
extern float32 VolCPU_gfv;
extern float32 CurADiffPU_gfv;
extern float32 CurBDiffPU_gfv;
extern float32 CurCDiffPU_gfv;
extern float32 Cur0DiffPU_gfv;
extern float32 CurAComPU_gfv;
extern float32 CurBComPU_gfv;
extern float32 CurCComPU_gfv;
extern float32 Cur0ComPU_gfv;

//==========Theta Estimate==========
extern volatile enum CMDMOD  Cmd_ThetaEst_glv;
extern volatile enum STATMOD Stat_ThetaEst_glv;
extern int32   Cnt_ThetaEstOK_glv;      //State
extern float32 OmegaEstPU_gfv;		    //Angular frequency of grid
extern float32 DeltaEstPU_gfv;			//Output
extern float32 DeltaFilPU_gfv;			//Output,State
extern float32 ThetaEstPU_gfv;			//1st phase angle
extern float32 ThetaEst2rdPU_gfv;		//2rd phase angle
extern float32 VolAlPU_gfv;		        //Output
extern float32 VolBePU_gfv;		        //Output
extern float32 VolDPU_gfv;		        //Output
extern float32 VolQPU_gfv;		        //Output
extern float32 CurAlDiffPU_gfv;	    	//Output
extern float32 CurBeDiffPU_gfv;	    	//Output
extern float32 CurDDiffPU_gfv;	        //Output
extern float32 CurQDiffPU_gfv;         	//Output
extern float32 CurAlComPU_gfv;		    //Output
extern float32 CurBeComPU_gfv;		    //Output
extern float32 CurDComPU_gfv;	        //Output
extern float32 CurQComPU_gfv;	        //Output
extern float32 CurDComFilPU_gfv;		//Output
extern float32 CurQComFilPU_gfv;		//Output
extern float32 VolAmpPU_gfv;            //Output
extern float32 VolAmpFilPU_gfv;         //Output
extern PIDREG3 ThetaEst_PID;     //PID for PLL
extern RAMPGEN ThetaEst_ROL;     //Omega to 1st Theta
extern RAMPGEN ThetaEst2rd_ROL;  //Omega to 2rd Theta
extern CLARKE  Vol_CLARKE;
extern CLARKE  Cur_CLARKE;
extern CLARKE  Cur2rd_CLARKE;
extern PARK    Vol_PARK;
extern PARK    Cur_PARK;
extern PARK    Cur2rd_PARK;

//==========Current & Voltage Control==========
extern volatile enum CMDMOD   Cmd_CurCtrl_glv;
extern volatile enum STATMOD   Stat_CurCtrl_glv;
extern int32   Flag_ACVolFwd_glv;
extern float32 CurQMaxCmdPU_gfv;
extern float32 CurQMaxPU_gfv;
extern float32 CurDCmdPU_gfv;
extern float32 CurQCmdPU_gfv;
extern float32 CurDComCmdPU_gfv;
extern float32 CurQComCmdPU_gfv;
extern float32 ConvVolDDiffPU_gfv;
extern float32 ConvVolQDiffPU_gfv;
extern float32 ConvVolAlDiffPU_gfv;
extern float32 ConvVolBeDiffPU_gfv;
extern float32 ConvVolADiffPU_gfv;
extern float32 ConvVolBDiffPU_gfv;
extern float32 ConvVolCDiffPU_gfv;
extern float32 ConvVolDComPU_gfv;
extern float32 ConvVolQComPU_gfv;
extern float32 ConvVolAlComPU_gfv;
extern float32 ConvVolBeComPU_gfv;
extern float32 ConvVolAComPU_gfv;
extern float32 ConvVolBComPU_gfv;
extern float32 ConvVolCComPU_gfv;
extern float32 DutyA_gfv;
extern float32 DutyB_gfv;
extern float32 DutyC_gfv;
extern float32 PWMPrd_gfv;
extern float32 PWMPrdPU_gfv;
extern float32 VolDC1FilPU_gfv;
extern float32 CurDCFilPU_gfv;
extern TFORDER2  ResoD6_TF;
extern TFORDER2  ResoQ6_TF;
extern IPARK   ConvVol_IPARK;
extern ICLARKE ConvVol_ICLARKE;
extern PIDREG3 CurD_PID;
extern PIDREG3 CurQ_PID;
extern PIDREG3 CurDCom_PID;
extern PIDREG3 CurQCom_PID;
extern PIDREG3 VolDC_PID;
extern float32 CurMaxPU_gfv;
extern int32 ENCCSCtl;
//==========End of Current & Voltage Control==========
#endif /* CURCTRL_H_ */
