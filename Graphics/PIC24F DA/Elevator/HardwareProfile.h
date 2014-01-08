/********************************************************************
 FileName:      HardwareProfile.h
 Dependencies:  See INCLUDES section
 Processor:     PIC24 Microcontrollers
 Hardware:      The code is natively intended to be used on the 
                  following hardware platforms: 
                    PIC24FJ256DA210 Development Board (DM240312)
 Compiler:  	Microchip C30 (for PIC24)
 Company:       Microchip Technology, Inc.

 Software License Agreement:

 The software supplied herewith by Microchip Technology Incorporated
 (the �Company�) for its PIC� Microcontroller is intended and
 supplied to you, the Company�s customer, for use solely and
 exclusively on Microchip PIC Microcontroller products. The
 software is owned by the Company and/or its supplier, and is
 protected under applicable copyright laws. All rights are reserved.
 Any use in violation of the foregoing restrictions may subject the
 user to criminal sanctions under applicable laws, as well as to
 civil liability for the breach of the terms and conditions of this
 license.

 THIS SOFTWARE IS PROVIDED IN AN �AS IS� CONDITION. NO WARRANTIES,
 WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED
 TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. THE COMPANY SHALL NOT,
 IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL OR
 CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.

********************************************************************
 File Description:

 Change History:
 Date         Description
 01/10/2011   Initial release
 01/23/2012   Added MPLAB X support
********************************************************************/
#ifndef CFG_INCLUDE_MPLAB_X

#if defined (__PIC24FJ256DA210__)
    	
    /*********************************************************************
	* Hardware Configuration for 
	* PIC24FJ256DA210 Development Board
	* Display PH480272T-005-I11Q
	* Using Parallel Flash (DISP_ORIENTATION = 0 degrees)
	********************************************************************/
    #include "Configs/HWP_DA210_BRD_16PMP_PFLASH_WQVGAv1.h"		

    /*********************************************************************
	* Hardware Configuration for 
	* PIC24FJ256DA210 Development Board
	* Display PH480272T-005-I11Q
	* Using SPI Flash (DISP_ORIENTATION = 90 degrees)
	********************************************************************/
    //#include "Configs/HWP_DA210_BRD_16PMP_WQVGAv1.h"		


#endif

#else

#if defined (CFG_INCLUDE_DA210_BRD_16PMP_PFLASH_WQVGAv1)
    #include "Configs/HWP_DA210_BRD_16PMP_PFLASH_WQVGAv1.h"
#elif defined (CFG_INCLUDE_DA210_BRD_16PMP_WQVGAv1)
    #include "Configs/HWP_DA210_BRD_16PMP_WQVGAv1.h"
#endif


#endif
