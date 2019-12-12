/*
// TI File $Revision: /main/8 $
// Checkin $Date: June 2, 2008   11:12:24 $
//###########################################################################
//Notion:
//1. this cmd file is used to allocate the XIntf Address for register.
//
//###########################################################################
*/

MEMORY
{
 PAGE 0:    /* Program Memory */

 PAGE 1:    /* Data Memory */

   FPGASCI       : origin = 0x0200000, length = 0x000002
   FPGASAMPA     : origin = 0x0200020, length = 0x000010 //SCI used in V
   FPGASAMPB     : origin = 0x0200030, length = 0x000010 //SCI used in V
   //UP
   UPARMST       : origin = 0x0200002, length = 0x00000C
   UPSORT        : origin = 0x0200010, length = 0x000010
   UPSMDATA      : origin = 0x0200040, length = 0x000060
   //UN
   UNARMST       : origin = 0x0200102, length = 0x00000C
   UNSORT        : origin = 0x0200110, length = 0x000010
   UNSMDATA      : origin = 0x0200140, length = 0x000060
   //VP
   VPARMST       : origin = 0x0200202, length = 0x00000C
   VPSORT        : origin = 0x0200210, length = 0x000010
   VPSMDATA      : origin = 0x0200240, length = 0x000060
   //UN
   VNARMST       : origin = 0x0200302, length = 0x00000C
   VNSORT        : origin = 0x0200310, length = 0x000010
   VNSMDATA      : origin = 0x0200340, length = 0x000060
   //WP
   WPARMST       : origin = 0x0200402, length = 0x00000C
   WPSORT        : origin = 0x0200410, length = 0x000010
   WPSMDATA      : origin = 0x0200440, length = 0x000060
   //WN
   WNARMST       : origin = 0x0200502, length = 0x00000C
   WNSORT        : origin = 0x0200510, length = 0x000010
   WNSMDATA      : origin = 0x0200540, length = 0x000060
}


SECTIONS
{
   FPGASciRegsFile	 : > FPGASCI,     	PAGE = 1
   SampARegsFile  	 : > FPGASAMPA, 	PAGE = 1
   SampBRegsFile  	 : > FPGASAMPB,  	PAGE = 1
//UP
   UPARMSTRegsFile   : > UPARMST,    	PAGE = 1
   UPSortRegsFile    : > UPSORT,     	PAGE = 1

//UN
   UNARMSTRegsFile   : > UNARMST,    	PAGE = 1
   UNSortRegsFile    : > UNSORT,    	PAGE = 1
//VP
   VPARMSTRegsFile   : > VPARMST,    	PAGE = 1
   VPSortRegsFile    : > VPSORT,    	PAGE = 1
//VN
   VNARMSTRegsFile   : > VNARMST,    	PAGE = 1
   VNSortRegsFile    : > VNSORT,    	PAGE = 1
//WP
   WPARMSTRegsFile   : > WPARMST,    	PAGE = 1
   WPSortRegsFile    : > WPSORT,    	PAGE = 1
//WN
   WNARMSTRegsFile   : > WNARMST,    	PAGE = 1
   WNSortRegsFile    : > WNSORT,    	PAGE = 1
   //WNSMDataRegsFile : > WNSMDATA, PAGE = 1
   //WNSMData2RegsFile : > WNSMDATA2, PAGE = 1

}


/*
//===========================================================================
// End of file.
//===========================================================================
*/
