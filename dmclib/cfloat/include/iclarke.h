/* =================================================================================
File name:       ICLARKE.H

Originator:	Digital Control Systems Group
			Texas Instruments

Description:
Header file containing constants, data type definitions, and
function prototypes for the ICLARKE.
=====================================================================================
 History:
-------------------------------------------------------------------------------------
 04-15-2005	Version 3.20
------------------------------------------------------------------------------*/

typedef struct {  float32  Alpha;  	// Input: stationary d-axis stator variable
				  float32  Beta;	// Input: stationary q-axis stator variable
				  float32  As;		// Output: phase-a stator variable
				  float32  Bs;		// Output: phase-b stator variable
				  float32  Cs;		// Output: phase-c stator variable
		 	 	  void  (*calc)();	// Pointer to calculation function
				 } ICLARKE;

typedef ICLARKE *ICLARKE_handle;

/*-----------------------------------------------------------------------------
Default initalizer for the CLARKE object.
-----------------------------------------------------------------------------*/
#define ICLARKE_DEFAULTS { 0, \
                          0, \
                          0, \
                          0, \
                          0, \
              			  (void (*)(Uint32))iclarke_calc }

/*------------------------------------------------------------------------------
Prototypes for the functions in ICLARKE.C
------------------------------------------------------------------------------*/
void iclarke_calc(ICLARKE_handle);

