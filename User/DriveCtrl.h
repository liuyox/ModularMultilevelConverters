/*
 * DriveCtrl.h
 *
 *  Created on: 2016-7-22
 *      Author: Administrator
 */

#ifndef DRIVECTRL_H_
#define DRIVECTRL_H_

#include "main.h"


extern void DriveCtrl(void);
extern volatile enum CMDMOD Cmd_PWMDrive_glv;
extern volatile enum CMDISR Cmd_Isr_glv;
extern volatile enum STATISR Stat_Isr_glv;
extern float32 ConvVolUDiffPU_gfv;
extern float32 ConvVolVDiffPU_gfv;
extern float32 ConvVolWDiffPU_gfv;
extern float32 ConvVolUComPU_gfv;
extern float32 ConvVolVComPU_gfv;
extern float32 ConvVolWComPU_gfv;

extern float32 ConvVolUPPU_gfv;
extern float32 ConvVolUNPU_gfv;
extern float32 ConvVolVPPU_gfv;
extern float32 ConvVolVNPU_gfv;
extern float32 ConvVolWPPU_gfv;
extern float32 ConvVolWNPU_gfv;

extern Uint32 PWMDutyUP_gulv;
extern Uint32 PWMDutyUN_gulv;
extern Uint32 PWMDutyVP_gulv;
extern Uint32 PWMDutyVN_gulv;
extern Uint32 PWMDutyWP_gulv;
extern Uint32 PWMDutyWN_gulv;

extern Uint32 NumUP_gulv;
extern Uint32 NumUN_gulv;
extern Uint32 NumVP_gulv;
extern Uint32 NumVN_gulv;
extern Uint32 NumWP_gulv;
extern Uint32 NumWN_gulv;

extern union SMNR_REG UPSMNR;
extern union SMNR_REG UNSMNR;
extern union SMNR_REG VPSMNR;
extern union SMNR_REG VNSMNR;
extern union SMNR_REG WPSMNR;
extern union SMNR_REG WNSMNR;

# if Debug == 1
extern Uint16 ModuDIR; // 0 - positive voltage, 1- negative voltage
extern Uint16 INSMNUM;
#endif

#endif /* DRIVECTRL_H_ */
