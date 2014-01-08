/******************************************************************************

 MRF24WB0M Driver Customization
 Module for Microchip TCP/IP Stack
  -Provides access to MRF24WB0M WiFi controller
  -Reference: MRF24WB0M Data sheet, IEEE 802.11 Standard

*******************************************************************************
 FileName:		WF_Config.c
 Dependencies:	TCP/IP Stack header files
 Processor:		PIC18, PIC24F, PIC24H, dsPIC30F, dsPIC33F, PIC32
 Compiler:		Microchip C32 v1.10b or higher
				Microchip C30 v3.22 or higher
				Microchip C18 v3.34 or higher
 Company:		Microchip Technology, Inc.

 Software License Agreement

 Copyright (C) 2002-2010 Microchip Technology Inc.  All rights reserved.

 Microchip licenses to you the right to use, modify, copy, and distribute:
 (i)  the Software when embedded on a Microchip microcontroller or digital 
      signal controller product ("Device") which is integrated into 
      Licensee's product; or
 (ii) ONLY the Software driver source files ENC28J60.c, ENC28J60.h,
      ENCX24J600.c and ENCX24J600.h ported to a non-Microchip device used in 
	  conjunction with a Microchip ethernet controller for the sole purpose 
	  of interfacing with the ethernet controller.

 You should refer to the license agreement accompanying this Software for 
 additional information regarding your rights and obligations.

 THE SOFTWARE AND DOCUMENTATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
 KIND, EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY WARRANTY
 OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, TITLE AND
 NON-INFRINGEMENT. IN NO EVENT SHALL MICROCHIP BE LIABLE FOR ANY INCIDENTAL,
 SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST
 OF PROCUREMENT OF SUBSTITUTE GOODS, TECHNOLOGY OR SERVICES, ANY CLAIMS BY
 THIRD PARTIES (INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), ANY CLAIMS
 FOR INDEMNITY OR CONTRIBUTION, OR OTHER SIMILAR COSTS, WHETHER ASSERTED ON
 THE BASIS OF CONTRACT, TORT (INCLUDING NEGLIGENCE), BREACH OF WARRANTY, OR
 OTHERWISE.


 Author				Date		Comment
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 KH                 27 Jan 2010 Created for MRF24WB0M
******************************************************************************/


#include "HardwareProfile.h"

#if defined(WF_CS_TRIS)

/*==========================================================================*/
/*                                  INCLUDES                                */
/*==========================================================================*/
#include "TCPIP Stack/TCPIP.h"

#if defined ( EZ_CONFIG_SCAN )
#include "TCPIP Stack/WFEasyConfig.h"
#endif /* EZ_CONFIG_SCAN */


/*==========================================================================*/
/*                                  DEFINES                                 */
/*==========================================================================*/

/* used for assertions */
#if defined(WF_DEBUG)
    #define WF_MODULE_NUMBER    WF_MODULE_WF_CONFIG
#endif


/*****************************************************************************
 * FUNCTION: WF_ProcessEvent
 *
 * RETURNS:  None
 *
 * PARAMS:   event      -- event that occurred
 *           eventInfo  -- additional information about the event.  Not all events
 *                         have associated info, in which case this value will be
 *                         set to WF_NO_ADDITIONAL_INFO (0xff)
 *
 *  NOTES:   The Host application must NOT directly call this function.  This 
 *           function is called by the WiFi Driver code when an event occurs
 *           that may need processing by the Host CPU.  
 *
 *           No other WiFi Driver function should be called from this function, with the
 *           exception of WF_ASSERT.  It is recommended that if the application wishes to be 
 *           notified of an event that it simply set a flag and let application code in the 
 *           main loop handle the event.  
 *
 *           WFSetFuncState must be called when entering and leaving this function.  
 *           When WF_ASSERT is enabled this allows a runtime check if any illegal WF functions 
 *           are called from within this function.
 *
 *           For events that the application is not interested in simply leave the
 *           case statement empty.
  *
 *           Customize this function as needed for your application.
 *****************************************************************************/
void WF_ProcessEvent(UINT8 event, UINT16 eventInfo)
{
    #if defined(STACK_USE_UART)
    char buf[8];
    #endif
  
    /* this function tells the WF driver that we are in this function */
    WFSetFuncState(WF_PROCESS_EVENT_FUNC, WF_ENTERING_FUNCTION);
      
    switch (event)
    {
        /*--------------------------------------*/
        case WF_EVENT_CONNECTION_SUCCESSFUL:
        /*--------------------------------------*/   
            #if defined(STACK_USE_UART)
            putrsUART("Event: Connection Successful\r\n"); 
            #endif
            break;
        
        /*--------------------------------------*/            
        case WF_EVENT_CONNECTION_FAILED:
        /*--------------------------------------*/
            /* eventInfo will contain value from tWFConnectionFailureCodes */
            #if defined(STACK_USE_UART)
            putrsUART("Event: Connection Failed  -- eventInfo = ");
            sprintf(buf, "%d\r\n", eventInfo);
            putsUART(buf);
            #endif
            break; 
            
        /*--------------------------------------*/
        case WF_EVENT_CONNECTION_TEMPORARILY_LOST:
        /*--------------------------------------*/
            /* eventInfo will contain value from tWFConnectionLostCodes */
            #if defined(STACK_USE_UART)
            putrsUART("Event: Connection Temporarily Lost -- eventInfo = ");
            sprintf(buf, "%d\r\n", eventInfo);
            putsUART(buf);
            #endif
            break;
            
        /*--------------------------------------*/
        case WF_EVENT_CONNECTION_PERMANENTLY_LOST:            
        /*--------------------------------------*/
            /* eventInfo will contain value from tWFConnectionLostCodes */
            #if defined(STACK_USE_UART)       
            putrsUART("Event: Connection Permanently Lost -- eventInfo = ");
            sprintf(buf, "%d\r\n", eventInfo);
            putsUART(buf);
            #endif
            break;

        /*--------------------------------------*/    
        case WF_EVENT_CONNECTION_REESTABLISHED:
        /*--------------------------------------*/
            #if defined(STACK_USE_UART)
            putrsUART("Event: Connection Reestablished\r\n");
            #endif
            break;
            
        /*--------------------------------------*/
        case WF_EVENT_SCAN_RESULTS_READY:
        /*--------------------------------------*/  
            #if defined(STACK_USE_UART)
            putrsUART("Event: Scan Results Ready,");
            sprintf(buf, "%d", eventInfo);
            putsUART(buf);
            putrsUART("results\r\n");
			#endif
            #if defined ( EZ_CONFIG_SCAN )
            WFScanEventHandler(eventInfo);
			#endif /* EZ_CONFIG_SCAN */
            break;
            
        /*--------------------------------------*/                            
        case WF_EVENT_RX_PACKET_RECEIVED:
        /*--------------------------------------*/                        
            #if defined(STACK_USE_UART)
//            putrsUART("Event: Rx Packet Received - length = ");
//            sprintf(buf, "%d\r\n", eventInfo);
//          putsUART(buf);
			#endif
            break;
            

        default:
            WF_ASSERT(FALSE);  /* unknown event */
            break;
    }        
    
    /* Informs the WF driver that we are leaving this function */
    WFSetFuncState(WF_PROCESS_EVENT_FUNC, WF_LEAVING_FUNCTION);
}    

  
/*
*********************************************************************************************************
*                                   WF_AssertionFailed()
*    
* Description : Called by a WiFi library function when an assert occurs.
*
* Argument(s) : moduleNumber - module number (located in WFApi.h)
*                
*               lineNumber   - line number within module where assert occurred.
*
* Return(s)   : None
*
* Caller(s)   : WF Driver
*
* Notes:      : (1) If the WF_ASSERT macro is enabled (via the WF_DEBUG define in WF_Config.h) this is the 
*                   function that gets called when WF_ASSERT() fails.
*         
*               (2) Customize this function as desired to handle assertion errors
*
*********************************************************************************************************
*/
#if defined(WF_DEBUG)
#define WIFI_ASSERT_STRING "WiFi Assert     M:"
void WF_AssertionFailed(UINT8 moduleNumber, UINT16 lineNumber) 
{
    #if defined(STACK_USE_UART)
    char buf[8];
    
    putrsUART("WF ASSERTION: Module Number = ");
    
    sprintf(buf, "%d  ", moduleNumber);
    putsUART(buf);
    
    putrsUART("Line Number = ");
    
    sprintf(buf, "%d", lineNumber);
    putsUART(buf);
    #endif
    
    #if defined(USE_LCD)
    {
        char buf[] = {WIFI_ASSERT_STRING};
        memset(LCDText, ' ', sizeof(LCDText));
        memcpy((void *)LCDText, (void *)buf, strlen(buf));
        uitoa(moduleNumber, (BYTE*)buf);
        memcpy((void *)&LCDText[18], (void *)buf, strlen(buf));
        LCDText[23] = 'L';
        LCDText[24] = ':';
        uitoa(lineNumber, &LCDText[25]);
        LCDUpdate();
    }    
    #endif

    
    while(1);
}    
#endif /* WF_DEBUG */

#endif /* WF_CS_TRIS */

