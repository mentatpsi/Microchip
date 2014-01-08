/*****************************************************************************
 * FileName:        FlashProgrammer.c
 * Dependencies:    See Includes Section
 * Processor:       PIC24F, PIC24H, dsPIC, PIC32
 * Compiler:       	MPLAB C30, MPLAB C32
 * Linker:          MPLAB LINK30, MPLAB LINK32
 * Company:         Microchip Technology Incorporated
 *
 * Software License Agreement
 *
 * Copyright © 2010 Microchip Technology Inc.  All rights reserved.
 * Microchip licenses to you the right to use, modify, copy and distribute
 * Software only when embedded on a Microchip microcontroller or digital
 * signal controller, which is integrated into your product or third party
 * product (pursuant to the sublicense terms in the accompanying license
 * agreement).  
 *
 * You should refer to the license agreement accompanying this Software
 * for additional information regarding your rights and obligations.
 *
 * SOFTWARE AND DOCUMENTATION ARE PROVIDED “AS IS” WITHOUT WARRANTY OF ANY
 * KIND, EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY WARRANTY
 * OF MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR
 * PURPOSE. IN NO EVENT SHALL MICROCHIP OR ITS LICENSORS BE LIABLE OR
 * OBLIGATED UNDER CONTRACT, NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION,
 * BREACH OF WARRANTY, OR OTHER LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT
 * DAMAGES OR EXPENSES INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL,
 * INDIRECT, PUNITIVE OR CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA,
 * COST OF PROCUREMENT OF SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY
 * CLAIMS BY THIRD PARTIES (INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF),
 * OR OTHER SIMILAR COSTS.
 *****************************************************************************/
/*****************************************************************************
 * Section: Description
 *
 * This module is used to program an external memory source.  The data is sent
 * via the PC utility external_memory_programmer.  Using the comm_pkt protocol, 
 * the data is parsed and programed to the external memory.  
 *
 * This module is used by serval different Graphics demos and is part of the 
 * common directory under the Graphics demo.
 *****************************************************************************/
/*****************************************************************************
 * Section: Includes
 *****************************************************************************/
#include "FlashProgrammer.h"
#include "comm_pkt.h"
#include "HardwareProfile.h"
#ifdef USE_COMM_PKT_MEDIA_SERIAL_PORT
#include "UART.h"
#endif
#ifdef USE_COMM_PKT_MEDIA_USB
#include "USB/usb.h"
#include "USB/usb_function_generic.h"
#endif
#include "ExternalMemory.h"

/*****************************************************************************
 * Section: Module Defines
 *****************************************************************************/
#ifdef USE_COMM_PKT_MEDIA_USB
#define FLASH_PROGRAMMER_COMMUNICATION_MEDIUM (COMM_PKT_MEDIA_USB)
#else
#define FLASH_PROGRAMMER_COMMUNICATION_MEDIUM (COMM_PKT_MEDIA_SERIAL_PORT)
#endif

/*****************************************************************************
 * Section: Function Prototypes
 *****************************************************************************/
void BinaryMemoryUpload(void);
BOOL BinaryHandlePacket(void);
BYTE WriteMemory(DWORD addr, BYTE *data, WORD length);
DWORD CalculateCheckSum(DWORD addr, BYTE *buffer, DWORD range);

/*****************************************************************************
 * Section: Externs
 *****************************************************************************/
#ifdef USE_COMM_PKT_MEDIA_USB
extern unsigned char OUTPacket[64];	//User application buffer for receiving and holding OUT packets sent from the host
extern unsigned char INPacket[64];		//User application buffer for sending IN packets to the host
extern USB_HANDLE USBGenericOutHandle;
extern USB_HANDLE USBGenericInHandle;
#endif


/*****************************************************************************
 * Section: Module Structures
 *****************************************************************************/
typedef struct
{
    DWORD addr __attribute__((packed));
    BYTE data;
}COMM_PKT_MEMORY_PAYLOAD;

typedef struct
{
    DWORD addr __attribute__((packed));
    DWORD range __attribute__((packed));
}COMM_PKT_VERIFY_PAYLOAD;

/*****************************************************************************
 * Section: Functions
 *****************************************************************************/


/*****************************************************************************
 * int ProgramFlash(void)
 *****************************************************************************/
int ProgramFlash(void)
{


 	#if defined (PIC24FJ256DA210_DEV_BOARD)
    
    // Make all the analog pins digital.
	ANSA = 0x0000;
    ANSB = 0x0000;		
    ANSC = 0x0000;		
    ANSD = 0x0000;
    ANSE = 0x0000;
    ANSF = 0x0000;
    ANSG = 0x0000;		
    
    #else

		#if defined (__PIC24FJ256GB210__) 
		    // Make all the analog pins digital.
			ANSA = 0x0000;
		    ANSB = 0x0000;		
		    ANSC = 0x0000;		
		    ANSD = 0x0000;
		    ANSE = 0x0000;
		    ANSF = 0x0000;
		    ANSG = 0x0000;		
        #elif defined (__dsPIC33E__)
		    // Make all the analog pins digital.
			ANSELA = 0x0000;
		    ANSELB = 0x0000;		
		    ANSELC = 0x0000;		
		    ANSELD = 0x0000;
		    ANSELE = 0x0000;
		    ANSELG = 0x0000;		
		#endif

    /////////////////////////////////////////////////////////////////////////////
    // ADC Explorer 16 Development Board Errata (work around 2)
    // RB15 should be output
    /////////////////////////////////////////////////////////////////////////////
    LATBbits.LATB15 = 0;
    TRISBbits.TRISB15 = 0;
	#endif
    
    
    #ifdef USE_COMM_PKT_MEDIA_USB
    #if defined(PIC24FJ64GB004_PIM) || defined(PIC24FJ256DA210_DEV_BOARD)
	//On the PIC24FJ64GB004 Family of USB microcontrollers, the PLL will not power up and be enabled
	//by default, even if a PLL enabled oscillator configuration is selected (such as HS+PLL).
	//This allows the device to power up at a lower initial operating frequency, which can be
	//advantageous when powered from a source which is not gauranteed to be adequate for 32MHz
	//operation.  On these devices, user firmware needs to manually set the CLKDIV<PLLEN> bit to
	//power up the PLL.
    {
        unsigned int pll_startup_counter = 600;
        CLKDIVbits.PLLEN = 1;
        while(pll_startup_counter--);
    }

    //Device switches over automatically to PLL output after PLL is locked and ready.
    #endif
	#endif

    /////////////////////////////////////////////////////////////////////////////
    #ifdef __PIC32MX__
    INTEnableSystemMultiVectoredInt();
    #endif

    #ifdef USE_COMM_PKT_MEDIA_USB
	USBGenericOutHandle = 0;	
	USBGenericInHandle = 0;		

    USBDeviceInit();	//usb_device.c.  Initializes USB module SFRs and firmware
    #endif

    #ifdef USE_COMM_PKT_MEDIA_SERIAL_PORT
    UARTInit();
    #endif

    HardwareButtonInit();
    BinaryMemoryUpload();

    return (0);
}

/*****************************************************************************
 * void BinaryMemoryUpload(void)
 *****************************************************************************/
void BinaryMemoryUpload(void)
{
    #ifdef USE_COMM_PKT_MEDIA_USB
    #if defined(USB_INTERRUPT)
        USBDeviceAttach();
    #endif
    #endif

    COMM_PKT_Init();

    while(!BinaryHandlePacket())
    {
        COMM_PKT_Update(FLASH_PROGRAMMER_COMMUNICATION_MEDIUM);

        #ifdef USE_COMM_PKT_MEDIA_USB
        #if defined(USB_POLLING)
		// Check bus status and service USB interrupts.
        USBDeviceTasks(); // Interrupt or polling method.  If using polling, must call
        				  // this function periodically.  This function will take care
        				  // of processing and responding to SETUP transactions 
        				  // (such as during the enumeration process when you first
        				  // plug in).  USB hosts require that USB devices should accept
        				  // and process SETUP packets in a timely fashion.  Therefore,
        				  // when using polling, this function should be called 
        				  // regularly (such as once every 1.8ms or faster** [see 
        				  // inline code comments in usb_device.c for explanation when
        				  // "or faster" applies])  In most cases, the USBDeviceTasks() 
        				  // function does not take very long to execute (ex: <100 
        				  // instruction cycles) before it returns.
        #endif
        #endif
    }
}

/*****************************************************************************
 * BOOL BinaryHandlePacket(void)
 *****************************************************************************/
BOOL BinaryHandlePacket(void)
{
    BYTE *packet;
    BYTE *payload;
    COMM_PKT_HDR *hdr;
    BYTE ack_nack;
    BOOL result = FALSE;

    if(COMM_PKT_RxPacketAvailable() == FALSE)
        return result; 

    packet = COMM_PKT_GetRxPacket();
    
    if(COMM_PKT_IsPacketValid(packet) == FALSE)
        return result;

    hdr = (COMM_PKT_HDR *)packet;
    payload = packet + sizeof(COMM_PKT_HDR);

    ack_nack = COMM_PKT_ACK;
    
    switch(hdr->cmd)
    {

    case COMM_PKT_ECHO:
        break;

    case COMM_PKT_MAX_PAYLOAD_SIZE:
        *((WORD *)payload) = COMM_PKT_RX_MAX_SIZE;
        hdr->length = 2;
        break;

    case COMM_PKT_MEMORY_ERASE:
    	ChipErase();
        break;

    case COMM_PKT_MEMORY_WRITE:
        {    
            COMM_PKT_MEMORY_PAYLOAD *memPayload = (COMM_PKT_MEMORY_PAYLOAD *)payload;

            if(!WriteMemory(memPayload->addr, (BYTE *)&memPayload->data, hdr->length - 4))
                ack_nack = COMM_PKT_NACK;

            hdr->length = 4;
        }
        break;

    case COMM_PKT_MEMORY_READ:
        {
            COMM_PKT_MEMORY_PAYLOAD *memPayload = (COMM_PKT_MEMORY_PAYLOAD *)payload;

            hdr->length = *((WORD *)(payload + 4));
            ReadArray(memPayload->addr, (BYTE *)&memPayload->data, hdr->length);
            hdr->length += 4;
        }
        break;

    case COMM_PKT_MEMORY_VERIFY:
        {
            COMM_PKT_VERIFY_PAYLOAD *verifyPayload = (COMM_PKT_VERIFY_PAYLOAD *)payload;

            *((DWORD *)payload) = CalculateCheckSum(verifyPayload->addr, payload, verifyPayload->range);
            hdr->length = 4;
        }
        break;

    case COMM_PKT_MEMORY_DONE:
        result = TRUE;
        break;

    default:
        ack_nack = COMM_PKT_NACK;
        break;
    }

    COMM_PKT_SendReply( FLASH_PROGRAMMER_COMMUNICATION_MEDIUM,
                        hdr->cmd,
                        ack_nack,
                        payload,
                        hdr->length);

    return result;
}
/*****************************************************************************
 * BYTE WriteMemory(DWORD addr, BYTE *data, WORD length)
 *****************************************************************************/
BYTE WriteMemory(DWORD addr, BYTE *data, WORD length)
{
    return WriteArray(addr, data, length);
}

/*****************************************************************************
 * DWORD CalculateCheckSum(DWORD addr, BYTE *buffer, DWORD range)
 *****************************************************************************/
DWORD CalculateCheckSum(DWORD addr, BYTE *buffer, DWORD range)
{
    DWORD   checksum = 0xFFFFFFFF;
    
    while(range > 0)
    {
        WORD arraySize = COMM_PKT_RX_MAX_SIZE;
        WORD    i;

        if(arraySize > range)
            arraySize = range;

        ReadArray(addr, buffer, arraySize);

        for(i = 0; i < arraySize; i++)
        {
            checksum += (DWORD)buffer[i]; 
        }

        range -= arraySize;
        addr += (DWORD)arraySize;
    }

    checksum *= -1;

    return checksum;

}
