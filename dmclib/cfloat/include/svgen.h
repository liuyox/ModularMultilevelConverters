/*
 * svgen.h
 *
 *  Created on: 2017-12-13
 *      Author: LiuYongxia
 */

#ifndef SVGEN_H_
#define SVGEN_H_

typedef struct 	{ float32  Ualpha; 			// Input: reference alpha-axis phase voltage
				  float32  Ubeta;			// Input: reference beta-axis phase voltage
				  float32  Ta;				// Output: reference phase-a switching function
				  float32  Tb;				// Output: reference phase-b switching function
				  float32  Tc;				// Output: reference phase-c switching function
				  void (*calc)();	    // Pointer to calculation function
				} SVGENDQ;

typedef SVGENDQ *SVGENDQ_handle;
/*-----------------------------------------------------------------------------
Default initalizer for the SVGENDQ object.
-----------------------------------------------------------------------------*/
#define SVGENDQ_DEFAULTS { 0,0,0,0,0, \
                       (void (*)(Uint32))svgendq_calc }

/*------------------------------------------------------------------------------
Prototypes for the functions in SVGEN_DQ.C
------------------------------------------------------------------------------*/
void svgendq_calc(SVGENDQ_handle);


#endif /* SVGEN_H_ */
