/*********************************************************************
 *
 *  UART Configuration Menu
 *    - UART configuration
 *    - XMODEM uploads of MPFS classic image
 *
 *********************************************************************
 * FileName:        UARTConfig.c
 * Dependencies:    TCP/IP stack
 * Processor:       PIC18, PIC24F, PIC24H, dsPIC30F, dsPIC33F, PIC32
 * Compiler:        Microchip C32 v1.05 or higher
 *					Microchip C30 v3.12 or higher
 *					Microchip C18 v3.30 or higher
 *					HI-TECH PICC-18 PRO 9.63PL2 or higher
 * Company:         Microchip Technology, Inc.
 *
 * Software License Agreement
 *
 * Copyright (C) 2002-2009 Microchip Technology Inc.  All rights
 * reserved.
 *
 * Microchip licenses to you the right to use, modify, copy, and
 * distribute:
 * (i)  the Software when embedded on a Microchip microcontroller or
 *      digital signal controller product ("Device") which is
 *      integrated into Licensee's product; or
 * (ii) ONLY the Software driver source files ENC28J60.c, ENC28J60.h,
 *		ENCX24J600.c and ENCX24J600.h ported to a non-Microchip device
 *		used in conjunction with a Microchip ethernet controller for
 *		the sole purpose of interfacing with the ethernet controller.
 *
 * You should refer to the license agreement accompanying this
 * Software for additional information regarding your rights and
 * obligations.
 *
 * THE SOFTWARE AND DOCUMENTATION ARE PROVIDED "AS IS" WITHOUT
 * WARRANTY OF ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT
 * LIMITATION, ANY WARRANTY OF MERCHANTABILITY, FITNESS FOR A
 * PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT SHALL
 * MICROCHIP BE LIABLE FOR ANY INCIDENTAL, SPECIAL, INDIRECT OR
 * CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF
 * PROCUREMENT OF SUBSTITUTE GOODS, TECHNOLOGY OR SERVICES, ANY CLAIMS
 * BY THIRD PARTIES (INCLUDING BUT NOT LIMITED TO ANY DEFENSE
 * THEREOF), ANY CLAIMS FOR INDEMNITY OR CONTRIBUTION, OR OTHER
 * SIMILAR COSTS, WHETHER ASSERTED ON THE BASIS OF CONTRACT, TORT
 * (INCLUDING NEGLIGENCE), BREACH OF WARRANTY, OR OTHERWISE.
 *
 *
 * V5.36 ---- STACK_USE_MPFS has been removed and DownloadMPFS has upgraded to MPFS2
 ********************************************************************/
#define __UARTCONFIG_C

#include "TCPIPConfig.h"

#if defined(STACK_USE_UART)

#include "TCPIP Stack/TCPIP.h"
#include "MainDemo.h"


#if (defined(MPFS_USE_EEPROM) || defined(MPFS_USE_SPI_FLASH)) && defined(STACK_USE_MPFS2)
	static BOOL DownloadMPFS(void);
#endif

#define MAX_USER_RESPONSE_LEN   (20u)
void DoUARTConfig(void)
{
    BYTE response[MAX_USER_RESPONSE_LEN];
    IP_ADDR tempIPValue;
    IP_ADDR *destIPValue;
	WORD_VAL wvTemp;
    BOOL bQuit = FALSE;

	while(!bQuit)
	{
		// Display the menu
	    putrsUART("\r\n\r\n\rMicrochip TCP/IP Config Application ("TCPIP_STACK_VERSION", " __DATE__ ")\r\n\r\n");
	    putrsUART("\t1: Change serial number:\t\t");
		wvTemp.v[1] = AppConfig.MyMACAddr.v[4];
		wvTemp.v[0] = AppConfig.MyMACAddr.v[5];
		uitoa(wvTemp.Val, response);
		putsUART(response);
		putrsUART("\r\n\t2: Change host name:\t\t\t");
		putsUART(AppConfig.NetBIOSName);
	    putrsUART("\r\n\t3: Change static IP address:\t\t");
	    DisplayIPValue(AppConfig.MyIPAddr);
	    putrsUART("\r\n\t4: Change static gateway address:\t");
	    DisplayIPValue(AppConfig.MyGateway);
	    putrsUART("\r\n\t5: Change static subnet mask:\t\t");
	    DisplayIPValue(AppConfig.MyMask);
		putrsUART("\r\n\t6: Change static primary DNS server:\t");
	    DisplayIPValue(AppConfig.PrimaryDNSServer);
		putrsUART("\r\n\t7: Change static secondary DNS server:\t");
	    DisplayIPValue(AppConfig.SecondaryDNSServer);
	    putrsUART("\r\n\t8: ");
		putrsUART((ROM BYTE*)(AppConfig.Flags.bIsDHCPEnabled ? "Dis" : "En"));
		putrsUART("able DHCP & IP Gleaning:\t\tDHCP is currently ");
		putrsUART((ROM BYTE*)(AppConfig.Flags.bIsDHCPEnabled ? "enabled" : "disabled"));
	    putrsUART("\r\n\t9: Download MPFS image.");
	    putrsUART("\r\n\t0: Save & Quit.");
	    putrsUART("\r\nEnter a menu choice: ");
	
	
		// Wait for the user to press a key
	    while(!DataRdyUART());
	
		putrsUART((ROM char*)"\r\n");
	
		// Execute the user selection
	    switch(ReadUART())
	    {
		    case '1':
				putrsUART("New setting: ");
				if(ReadStringUART(response, sizeof(response)))
				{
					wvTemp.Val = atoi((char*)response);
			        AppConfig.MyMACAddr.v[4] = wvTemp.v[1];
		    	    AppConfig.MyMACAddr.v[5] = wvTemp.v[0];
				}
		        break;
		
			case '2':
				putrsUART("New setting: ");
		        ReadStringUART(response, sizeof(response) > sizeof(AppConfig.NetBIOSName) ? sizeof(AppConfig.NetBIOSName) : sizeof(response));
				if(response[0] != '\0')
				{
					memcpy(AppConfig.NetBIOSName, (void*)response, sizeof(AppConfig.NetBIOSName));
			        FormatNetBIOSName(AppConfig.NetBIOSName);
				}
				break;
		
		    case '3':
		        destIPValue = &AppConfig.MyIPAddr;
		        goto ReadIPConfig;
		
		    case '4':
		        destIPValue = &AppConfig.MyGateway;
		        goto ReadIPConfig;
		
		    case '5':
		        destIPValue = &AppConfig.MyMask;
		        goto ReadIPConfig;
		
		    case '6':
		        destIPValue = &AppConfig.PrimaryDNSServer;
		        goto ReadIPConfig;
	
			case '7':
		        destIPValue = &AppConfig.SecondaryDNSServer;
		        goto ReadIPConfig;
		
ReadIPConfig:
				putrsUART("New setting: ");
		        ReadStringUART(response, sizeof(response));
		
		        if(StringToIPAddress(response, &tempIPValue))
		            destIPValue->Val = tempIPValue.Val;
				else
		            putrsUART("Invalid input.\r\n");

		        break;
		
		
		    case '8':
		        AppConfig.Flags.bIsDHCPEnabled = !AppConfig.Flags.bIsDHCPEnabled;
		        break;
		
		    case '9':
				#if (defined(MPFS_USE_EEPROM)|| defined(MPFS_USE_SPI_FLASH)) && defined(STACK_USE_MPFS2)
		        	DownloadMPFS();
				#endif
		        break;
		
		    case '0':
			    bQuit = TRUE;
				#if defined(EEPROM_CS_TRIS) || defined(SPIFLASH_CS_TRIS)
		        	SaveAppConfig(&AppConfig);
					putrsUART("Settings saved.\r\n");
				#else
					putrsUART("External EEPROM/Flash not present -- settings will be lost at reset.\r\n");
				#endif
		        break;
		}
	}
}


#if (defined(MPFS_USE_EEPROM)|| defined(MPFS_USE_SPI_FLASH)) && defined(STACK_USE_MPFS2)
/*********************************************************************
 * Function:        BOOL DownloadMPFS(void)
 *
 * PreCondition:    MPFSInit() is already called.
 *
 * Input:           None
 *
 * Output:          TRUE if successful
 *                  FALSE otherwise
 *
 * Side Effects:    This function uses 128 bytes of Bank 4 using
 *                  indirect pointer.  This requires that no part of
 *                  code is using this block during or before calling
 *                  this function.  Once this function is done,
 *                  that block of memory is available for general use.
 *
 * Overview:        This function implements XMODEM protocol to
 *                  be able to receive a binary file from PC
 *                  applications such as HyperTerminal.
 *
 * Note:            In current version, this function does not
 *                  implement user interface to set IP address and
 *                  other informations.  User should create their
 *                  own interface to allow user to modify IP
 *                  information.
 *                  Also, this version implements simple user
 *                  action to start file transfer.  User may
 *                  evaulate its own requirement and implement
 *                  appropriate start action.
 *
 ********************************************************************/
#define XMODEM_SOH      0x01u
#define XMODEM_EOT      0x04u
#define XMODEM_ACK      0x06u
#define XMODEM_NAK      0x15u
#define XMODEM_CAN      0x18u
#define XMODEM_BLOCK_LEN 128u
//////////////////////////////////////////////////////////////////////////////////////////
// NOTE: The following XMODEM code has been upgarded to MPFS2 from MPFS Classic.
//       Upgrading to HTTP2 and MPFS2 is *strongly* recommended for all new designs.
//       MPFS2 images can be uploaded directly using the MPFS2.exe tool.
//////////////////////////////////////////////////////////////////////////////////////////
static BOOL DownloadMPFS(void)
{
    enum SM_MPFS
    {
        SM_MPFS_SOH,
        SM_MPFS_BLOCK,
        SM_MPFS_BLOCK_CMP,
        SM_MPFS_DATA,
    } state;

    BYTE c;
    MPFS_HANDLE handle;
    BOOL lbDone;
    BYTE blockLen=0;
    BYTE lResult;
    BYTE tempData[XMODEM_BLOCK_LEN];
    DWORD lastTick;
    DWORD currentTick;

    state = SM_MPFS_SOH;
    lbDone = FALSE;

    handle = MPFSFormat();

    // Notify the host that we are ready to receive...
    lastTick = TickGet();
    do
    {
        currentTick = TickGet();
        if ( currentTick - lastTick >= (TICK_SECOND/2) )
        {
            lastTick = TickGet();
			while(BusyUART());
            WriteUART(XMODEM_NAK);

            /*
             * Blink LED to indicate that we are waiting for
             * host to send the file.
             */
            LED6_IO ^= 1;
        }

    } while(!DataRdyUART());


    while(!lbDone)
    {
        if(DataRdyUART())
        {
            // Toggle LED as we receive the data from host.
            LED6_IO ^= 1;
            c = ReadUART();
        }
        else
        {
            // Real application should put some timeout to make sure
            // that we do not wait forever.
            continue;
        }

        switch(state)
        {
        default:
            if ( c == XMODEM_SOH )
            {
                state = SM_MPFS_BLOCK;
            }
            else if ( c == XMODEM_EOT )
            {
                // Turn off LED when we are done.
                LED6_IO = 1;

                MPFSClose(handle);
				while(BusyUART());
                WriteUART(XMODEM_ACK);
                lbDone = TRUE;
            }
            else
			{
				while(BusyUART());
				WriteUART(XMODEM_NAK);
			}

            break;

        case SM_MPFS_BLOCK:

            // We do not use block information.
            lResult = XMODEM_ACK;
            blockLen = 0;
            state = SM_MPFS_BLOCK_CMP;
            break;

        case SM_MPFS_BLOCK_CMP:

            // We do not use 1's comp. block value.
            state = SM_MPFS_DATA;
            break;

        case SM_MPFS_DATA:

            // Buffer block data until it is over.
            tempData[blockLen++] = c;
            if ( blockLen > XMODEM_BLOCK_LEN )
            {

                lResult = XMODEM_ACK;
                for ( c = 0; c < XMODEM_BLOCK_LEN; c++ )
					MPFSPutArray(handle,&tempData[c],1);
                    
                MPFSPutEnd(handle);

				while(BusyUART());
                WriteUART(lResult);
                state = SM_MPFS_SOH;
            }
            break;

        }

    }

    return TRUE;
}
#endif	// #if defined(MPFS_USE_EEPROM) && defined(STACK_USE_MPFS2)

#endif //#if defined(STACK_USE_UART)
