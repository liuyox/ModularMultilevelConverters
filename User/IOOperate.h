#ifndef IOOPERATE_H
#define IOOPERATE_H

//Notice*: inline functions are essentially macros, and should be defined in .h files
//Define the operation of IO control pins
//Detailed funtions listed below
//EI 		Control0	GPIO12		1:Clear Error,Disable PWM; 0:Normal Protection
//Relay 	Control1	GPIO13		0:On; 1:Off
//Fan		Control2	GPIO14		0:On; 1:Off
//Backup1 	Control3	GPIO15		0:On; 1:Off
//Backup2 	Control4	GPIO16		0:On; 1:Off
//LE		Control5	GPIO17		0:On; 1:Off

inline void PWMOn(void)
{
	GpioDataRegs.GPACLEAR.bit.GPIO17=1;
}
inline void PWMOff(void)
{
	GpioDataRegs.GPASET.bit.GPIO17=1;
}
inline void PWMReset(void)
{
	GpioDataRegs.GPASET.bit.GPIO12=1;
	DSP28x_usDelay(1);
	GpioDataRegs.GPACLEAR.bit.GPIO12=1;
}
inline void RelayOn(void)
{
	GpioDataRegs.GPACLEAR.bit.GPIO13=1;
}
inline void RelayOff(void)
{
	GpioDataRegs.GPASET.bit.GPIO13=1;
}
inline void FanOn(void)
{
	GpioDataRegs.GPACLEAR.bit.GPIO14=1;
}
inline void FanOff(void)
{
	GpioDataRegs.GPASET.bit.GPIO14=1;
}
inline void Backup1On(void)
{
	GpioDataRegs.GPACLEAR.bit.GPIO15=1;
}
inline void Backup1Off(void)
{
	GpioDataRegs.GPASET.bit.GPIO15=1;
}
inline void Backup2On(void)
{
	GpioDataRegs.GPACLEAR.bit.GPIO16=1;
}
inline void Backup2Off(void)
{
	GpioDataRegs.GPASET.bit.GPIO16=1;
}
inline void DCBOn(void)
{
	EPwm4Regs.AQCSFRC.bit.CSFA = 0x2; //Turn On
}
inline void DCBOff(void)
{
	EPwm4Regs.AQCSFRC.bit.CSFA = 0x1; //Turn Off
}
inline void VSCOn(void)
{
	EALLOW;
	EPwm1Regs.TZCLR.bit.OST = 1;
	EPwm2Regs.TZCLR.bit.OST = 1;
	EPwm3Regs.TZCLR.bit.OST = 1;
	EDIS;
}
inline void VSCOff(void)
{
	EALLOW;
	EPwm1Regs.TZFRC.bit.OST = 1;
	EPwm2Regs.TZFRC.bit.OST = 1;
	EPwm3Regs.TZFRC.bit.OST = 1;
	EDIS;
}

#endif
