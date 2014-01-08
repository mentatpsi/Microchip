/*********************************************************************
*
*     Microchip Ethernet/Internet Bootloader
*
*********************************************************************
* FileName:        Main.c
* Dependencies:    GenericTypeDefs.h, ETH97J60.h
* Processor:       PIC18F97J60 family device
* Compiler:        Microchip C18 v3.13 or higher
* Company:         Microchip Technology, Inc.
*
* Software License Agreement
*
* This software is owned by Microchip Technology Inc. ("Microchip") 
* and is supplied to you for use exclusively as described in the 
* associated software agreement.  This software is protected by 
* software and other intellectual property laws.  Any use in 
* violation of the software license may subject the user to criminal 
* sanctions as well as civil liability.  Copyright 2010 Microchip
* Technology Inc.  All rights reserved.
*
* This software is provided "AS IS."  MICROCHIP DISCLAIMS ALL 
* WARRANTIES, EXPRESS, IMPLIED, STATUTORY OR OTHERWISE, NOT LIMITED 
* TO MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, AND 
* INFRINGEMENT.  Microchip shall in no event be liable for special, 
* incidental, or consequential damages.
*
* Author               Date   	 Comment
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* Howard Schlunder		08/28/07 Original
********************************************************************/
#include <p18cxxx.h>
#include <string.h>
#include "GenericTypeDefs.h"
#include "TCPIP Stack/ETH97J60.h"
#include <stdlib.h>


//////////// VERY IMPORTANT NOTE ////////////
// The linker script omits the C18 RAM memory initializer to save code 
// space in this project.  As a result, you cannot use static/global 
// initialization code.  Ex: static BYTE MyVar = 123;
// Instead, declare the variable as global and then at the beginning 
// of the main() function, do: MyVar = 123;


/** D E F I N I T I O N S ****************************************************/
// These are defaults right out of POR.
#define MAC_ADDRESS1		0x00
#define MAC_ADDRESS2		0x04
#define MAC_ADDRESS3		0xA3
#define MAC_ADDRESS4		0x00
#define MAC_ADDRESS5		0x00
#define MAC_ADDRESS6		0x00
#define IP_ADDRESS0			192
#define IP_ADDRESS1			168
#define IP_ADDRESS2			97
#define IP_ADDRESS3			60

#define TFTP_UDP_PORT		69

#if defined(__18F66J60) || defined(__18F86J60) || defined(__18F96J60)
#define INTERNAL_FLASH_SIZE	(1024ul*64ul)
#elif defined(__18F66J65) || defined(__18F86J65) || defined(__18F96J65)
#define INTERNAL_FLASH_SIZE	(1024ul*96ul)
#elif defined(__18F67J60) || defined(__18F87J60) || defined(__18F97J60)
#define INTERNAL_FLASH_SIZE	(1024ul*128ul)
#else
#error "Unknown processor/internal Flash memory size."
#endif		

#define TOTAL_MEMORY_SIZE	INTERNAL_FLASH_SIZE	// Add more memory here if you are using an EMB and would like the bootloader to access it.  NOTE: EMB support is not currently implmented.

#define FLASH_WRITE_SIZE	(64ul)
#define FLASH_ERASE_SIZE	(1024ul)

#define BOOTLOADER_SIZE		(8ul*1024ul)
#define CONFIG_FUSE_SIZE	(8ul)

#define CLOCK_FREQ			(41666667ul)
#define BOOTLOADER_TIMEOUT	(4ul*SECOND)
#define LED_ONTIME_TIMEOUT	(SECOND/20ul)
#define LED_OFFTIME_TIMEOUT	(SECOND/2ul - LED_ONTIME_TIMEOUT)

#define ENABLE_READ_BACK

#define SECOND				(CLOCK_FREQ/4ul/256ul)	// 1:256 scalar on Timer0

// Pseudo Functions: Intended for compile time use only (not as efficient as casting to a pointer of a 16 bit type and then taking the proper byte)
#define LOW(a) 				(a & 0xFF)
#define HIGH(a) 			((a>>8) & 0xFF)

// Ethernet buffer RAM definitions
#define RAMSIZE	8192ul
#define APPSTART (RAMSIZE-FLASH_ERASE_SIZE)
#define TXSTART (APPSTART-(1514ul + 8ul))
#define RXSTART	(0ul)						// Should be an even memory address
#define	RXSTOP	((TXSTART-2ul) | 0x0001ul)	// Odd for errata workaround
#define RXSIZE	(RXSTOP-RXSTART+1ul)

// Return codes for DecodeHex() function
#define DECODE_ERROR_CHECKSUM 				-2
#define DECODE_ERROR_INVALID_FORMAT 		-1
#define DECODE_SUCCESS_NEED_MORE_HEXDATA	0
#define DECODE_SUCCESS_NEW_ADDRESS			1
#define DECODE_SUCCESS_EOF					2

// A generic structure representing the Ethernet header starting all Ethernet 
// frames
typedef struct
{
	BYTE			DestMAC[6];
	BYTE			SourceMAC[6];
	WORD_VAL		Type;
} MAC_HEADER;

// A header appended at the start of all RX frames by the hardware
typedef struct
{
	WORD			NextPacketPointer;
	RXSTATUS		StatusVector;
	MAC_HEADER		MACHeader;
} ETH_HEADER;

typedef struct
{
	WORD			HardwareType;
	WORD			ProtocolType;
	BYTE			HardwareSize;
	BYTE			ProtocolSize;
	WORD			Opcode;
	BYTE			SenderMACAddr[6];
	DWORD_VAL		SenderIPAddr;
	BYTE			TargetMACAddr[6];
	DWORD_VAL		TargetIPAddr;
} ARP_HEADER;

typedef struct
{
	BYTE			VersionLength;
	BYTE			DifferentiatedServices;
	WORD_VAL		Length;
	WORD_VAL		Identification;
	WORD_VAL		FlagsFragmentOffset;
	BYTE			TimeToLive;
	BYTE			Protocol;
	WORD_VAL		HeaderChecksum;
	DWORD_VAL		SourceIP;
	DWORD_VAL		DestinationIP;
} IP_HEADER;

typedef struct
{
	WORD_VAL		SourcePort;
	WORD_VAL		DestinationPort;
	WORD_VAL		Length;
	WORD_VAL		Checksum;
} UDP_HEADER;


// Function prototypes
void main(void);
static void ARPProcess(ETH_HEADER* ETHHeaderPtr, ARP_HEADER* ARPHeaderPtr);
static void TFTPBootloadProcess(ETH_HEADER* ETHHeaderPtr, IP_HEADER* IPHeaderPtr, UDP_HEADER* UDPHeaderPtr, BYTE* AppDataPtr, WORD AppDataLen);
static CHAR DecodeHex(BYTE *vHexData, WORD *wHexDataLen, BYTE *vMemoryData, DWORD *dwAddress, BOOL bInitialize);
static void EraseFlash(DWORD Address, DWORD EraseByteLength);
static void RawEraseFlashBlock(DWORD dwAddress);
static BOOL WriteFlashBlock(DWORD Address, BYTE *BlockData, BYTE *StatusData);
static BOOL RawWriteFlashBlock(DWORD Address, BYTE *BlockData, BYTE *StatusData);
static void EraseLastPage(BOOL bEraseAppData, BOOL bEraseFuses);
static DWORD GetTime(void);
static BYTE HexToBin(BYTE c);
static WORD CalcIPChecksum(BYTE* Data, WORD Length);
static void MACDiscardRx(void);
#define		MACGet()	(EDATA)
#define 	MACPut(val)	{PRODL = val; _asm movff PRODL, EDATA _endasm}
static WORD MACGetArray(BYTE *val, WORD len);
static void MACPutArray(BYTE *val, WORD len);
static void MACFlush(void);
static BOOL MACIsTxReady(void);
static void WritePHYReg(BYTE Register, WORD Data);


// RAM variables
static WORD_VAL		NextPacketLocation;
static WORD_VAL		CurrentPacketLocation;
static WORD			wTXWatchdog;
static WORD			wTimerHigh;
static DWORD		dwBootloaderExitTimer;
static BOOL			bWriteModeEnabled;
static BOOL			bNeedUpdatedJumpTable;
static struct
{
	BYTE			vMAC[6];
	DWORD_VAL		dwvIP;
} 					MyAddress;

#pragma udata BUFFERS
static BYTE			vTXBuffer[64];
static BYTE 		vJumpTableRAM[FLASH_WRITE_SIZE];
#pragma udata

#pragma udata FLASHWRITEMEM
static BYTE			vFlashData[FLASH_WRITE_SIZE];
#pragma udata

// RAM buffering of incoming Ethernet frames
#pragma udata BOOTMEM
static BYTE vTXRXBuffer[576+6];
#pragma udata 

// RAM buffering of incoming Ethernet frames
#pragma udata SAVEMEM
static BYTE vFlashSave[FLASH_ERASE_SIZE];
#pragma udata

// ROM goto application instruction
#pragma romdata BOOTJUMPROM
static far rom DWORD dwJumpInstructions[2] = {0xFFFFFFFF, 0xFFFFFFFF};	// 0xF000EF02 is a goto 0x000004 instruction
#pragma romdata

typedef struct
{
	unsigned char 		: 2;
	unsigned char CP0	: 1;
	unsigned char Zero	: 1;
	unsigned char Ones	: 4;
} CONFIG1Hbitsx;
#define CONFIG1H		(*((far rom BYTE*)INTERNAL_FLASH_SIZE-(0x20000-0x1FFF9)))
#define CONFIG1Hbits	(*((far rom CONFIG1Hbitsx*)&CONFIG1H))


#if defined(__EXTENDED18__)
#pragma config XINST=ON, CP0=OFF, WDT=ON, WDTPS=32, FOSC2=ON, FOSC=HSPLL, ETHLED=ON
#else
#pragma config XINST=OFF, CP0=OFF, WDT=ON, WDTPS=32, FOSC2=ON, FOSC=HSPLL, ETHLED=ON
#endif


/******************************************************************************
* Function:        void main(void)
*
* PreCondition:    None
*
* Input:           None
*
* Output:          None
*
* Side Effects:    None
*
* Overview:        None
*
* Note:            None
*****************************************************************************/
#pragma code BOOTROM
void main(void)
{
	// These must all be static since in Auto storage class mode, the 
	// variables get allocated on the stack before we can initialize the stack 
	// pointer
	static ETH_HEADER	ETHHeader;
	static IP_HEADER	*IPHeaderPtr;
	static UDP_HEADER	*UDPHeaderPtr;
	static BYTE			*AppDataPtr;
	static WORD			BufferLen;
	static WORD_VAL		wv;
	static DWORD_VAL	dwvNewPC;
	static DWORD		dwLEDBlinkTimer;
	static BOOL			bLEDOn;
	static BYTE 		i;
	static BYTE			vExitRCONState;	// This byte must not change between resets
	static WORD_VAL 	wvChecksum;


	// Disable all interrupts if the application called us
	INTCONbits.GIEL = 0;
	INTCONbits.GIEH = 0;
	ClrWdt();

	// Initialize the C18 stack pointers
	_asm
		lfsr	1, _stack
		lfsr	2, _stack
	_endasm

	// If the application reset for some reason and set the 
	// !POR bit, then immediately jump out of the bootloader and 
	// enter the application.
	if(RCONbits.NOT_POR)
	{
		// Change the PC to the application start vector, if an application has been bootloaded
		if(dwJumpInstructions[0] != 0xFFFFFFFF)
		{
			RCON = vExitRCONState;
			dwvNewPC.Val = (DWORD)&dwJumpInstructions;
			PCLATU = dwvNewPC.v[2];
			PCLATH = dwvNewPC.v[1];
			PCL    = dwvNewPC.v[0]+1-1;		//+1-1 is required to prevent C18 from using MOVFF instruction, which cannot move to the PCL register
		}
	}

	// Operate at 41.66667MHz
	OSCTUNE = 0x40;

	// Switch to primary oscillator mode, regardless of if the config fuses 
	// tell us to start operating using the the internal RC
	// The external clock must be running and must be 25MHz for the 
	// Ethernet module and thus this Ethernet bootloader to operate.
	if(OSCCONbits.IDLEN)
		OSCCON = 0x82;
	else
		OSCCON = 0x02;

	// Load up the MAC and IP address that the application left us, 
	// if present.  
	// IMPORTANT NOTE: This must be one of the first steps done in 
	// the bootloader.  Otherwise, the C compiler may overwrite the 
	// memory which originally held the MAC and IP address.
	FSR0 = PROD;
	MyAddress.vMAC[0] = POSTINC0;
	MyAddress.vMAC[1] = POSTINC0;
	MyAddress.vMAC[2] = POSTINC0;
	MyAddress.vMAC[3] = POSTINC0;
	MyAddress.vMAC[4] = POSTINC0;
	MyAddress.vMAC[5] = POSTINC0;
	MyAddress.dwvIP.v[0] = POSTINC0;
	MyAddress.dwvIP.v[1] = POSTINC0;
	MyAddress.dwvIP.v[2] = POSTINC0;
	MyAddress.dwvIP.v[3] = POSTINC0;
	wvChecksum.v[0] = POSTINC0;
	wvChecksum.v[1] = POSTINC0;		
	if(RCONbits.NOT_RI || (wvChecksum.Val != CalcIPChecksum((BYTE*)&MyAddress, 6+4)))
	{
		MyAddress.vMAC[0] = MAC_ADDRESS1;
		MyAddress.vMAC[1] = MAC_ADDRESS2;
		MyAddress.vMAC[2] = MAC_ADDRESS3;
		MyAddress.vMAC[3] = MAC_ADDRESS4;
		MyAddress.vMAC[4] = MAC_ADDRESS5;
		MyAddress.vMAC[5] = MAC_ADDRESS6;
		MyAddress.dwvIP.v[0] = IP_ADDRESS0;
		MyAddress.dwvIP.v[1] = IP_ADDRESS1;
		MyAddress.dwvIP.v[2] = IP_ADDRESS2;
		MyAddress.dwvIP.v[3] = IP_ADDRESS3;
	}	

	bWriteModeEnabled = FALSE;
	bNeedUpdatedJumpTable = FALSE;

	//
	// Configure the Ethernet module for operation
	//
	// Configure Ethernet LED pins
	LATAbits.LATA0 = 0;
	LATAbits.LATA1 = 0;
	TRISAbits.TRISA0 = 0;
	TRISAbits.TRISA1 = 0;

	ECON2bits.ETHEN = 1;    // Enable Ethernet!

	// Wait for PHYRDY to become set.
	while(!ESTATbits.PHYRDY)
		ClrWdt();

	// Configure the receive buffer boundary pointers 
	// and the buffer write protect pointer (receive buffer read pointer)
	NextPacketLocation.Val = RXSTART;

	ERXST = RXSTART;
	ERXRDPTL = LOW(RXSTOP);	// Write low byte first
	ERXRDPTH = HIGH(RXSTOP);// Write high byte last
	ERXND = RXSTOP;
	ETXST = TXSTART;
	EWRPT = TXSTART;		// Put permanent 0x00 per packet control byte
	MACPut(0x00);	

	// Configure the MAC
	// Enable the receive portion of the MAC
	MACON1 = MACON1_TXPAUS | MACON1_RXPAUS | MACON1_MARXEN; //Nop();

	// Pad packets to 60 bytes, add CRC, and check Type/Length field.
	MACON3 = MACON3_PADCFG0 | MACON3_TXCRCEN | MACON3_FRMLNEN; //Nop();

	// Allow infinite deferals if the medium is continuously busy 
	// (do not time out a transmission if the half duplex medium is 
	// completely saturated with other people's data)
	MACON4 = MACON4_DEFER; //Nop();

	// Set non-back-to-back inter-packet gap to 9.6us.  The back-to-back 
	// inter-packet gap (MABBIPG) is set by MACSetDuplex() which is called 
	// later.
	MAIPGL = 0x12; //Nop();
	MAIPGH = 0x0C; //Nop();

	// Set the maximum packet size which the controller will accept
	MAMXFLL = LOW(1518); //Nop();
	MAMXFLH = HIGH(1518); //Nop();

	// Initialize physical MAC address registers
	MAADR1 = MyAddress.vMAC[0]; //Nop();
	MAADR2 = MyAddress.vMAC[1]; //Nop();
	MAADR3 = MyAddress.vMAC[2]; //Nop();
	MAADR4 = MyAddress.vMAC[3]; //Nop();
	MAADR5 = MyAddress.vMAC[4]; //Nop();
	MAADR6 = MyAddress.vMAC[5];

	// Disable half duplex loopback and RXAP in PHY
	WritePHYReg(PHCON2, PHCON2_HDLDIS | PHCON2_RXAPDIS);

#if defined(FULL_DUPLEX)
	WritePHYReg(PHCON1, PHCON1_PDPXMD);
	MACON3bits.FULDPX = 1; //Nop();
	// Set the back-to-back inter-packet gap time to 9.6us in full duplex
	MABBIPG = 0x15; //Nop();
#else
	// Set the back-to-back inter-packet gap time to 9.6us in half duplex
	MABBIPG = 0x12; //Nop();
#endif

	// Enable packet reception
	ECON1bits.RXEN = 1;


	// Set up a timer so we can automatically exit the bootloader and start 
	// the application (if this chip is programmed with an application)
	T0CON = 0x87;	// TMR0ON = 1, T0PS = 111
	TMR0H = 0x00;
	TMR0L = 0x00;
	wTimerHigh = 0;
	INTCONbits.TMR0IF = 0;

	dwLEDBlinkTimer = GetTime();
	dwBootloaderExitTimer = dwLEDBlinkTimer;

	//
	// Enter main application loop checking for new packets
	//
	while(1)
	{
		// Wait for a packet to arrive
		do
		{
			ClrWdt();

			// Jump out of the bootloader and into the application 
			// if BOOTLOADER_TIMEOUT elapses (and an application exists)
			if(GetTime() - dwBootloaderExitTimer >= BOOTLOADER_TIMEOUT)
			{
				// Change the PC to the application start vector, if an application has been bootloaded
				if(dwJumpInstructions[0] != 0xFFFFFFFF)
				{
					// Make sure that when we exit the bootloader, the RCONbits.NOT_POR bit is correctly set (0 for real POR, 1 for any other reason)
					vExitRCONState = RCON;
					if(vExitRCONState != 0x3Cu)
						vExitRCONState |= 0x02;

					// NOT_POR must be set for us to exit the bootloader
					RCONbits.NOT_POR = 1;
					Reset();
				}

				dwBootloaderExitTimer = GetTime();
			}

			// Handle LEDA blinking
			if(GetTime() - dwLEDBlinkTimer >= (bLEDOn ? LED_ONTIME_TIMEOUT : LED_OFFTIME_TIMEOUT))
			{
				dwLEDBlinkTimer = GetTime();
				WritePHYReg(PHLCON, bLEDOn ? 0x3970 : 0x3870);
				bLEDOn = !bLEDOn;
			}
		} while(EPKTCNT == 0u);

		// Switch back to normal TX/RX display on LEDB so the 
		// user can see the activity
		WritePHYReg(PHLCON, 0x3472);

		// Save the location of this packet
		CurrentPacketLocation.Val = NextPacketLocation.Val;

		// Set the read pointer to the beginning of the next unprocessed packet
		ERDPT = NextPacketLocation.Val;

		// Obtain the MAC header from the Ethernet buffer
		MACGetArray((BYTE*)&ETHHeader, sizeof(ETHHeader));

		// Do a little sanity check
		wv.Val = CurrentPacketLocation.Val + 6 + ETHHeader.StatusVector.bits.ByteCount;
		if(wv.v[0] & 0x01)
			wv.Val++;
		if(wv.Val > RXSTOP)
			wv.Val -= RXSIZE;
		if(ETHHeader.NextPacketPointer != wv.Val || 
			ETHHeader.StatusVector.bits.Zero ||
			ETHHeader.StatusVector.bits.CRCError ||
			ETHHeader.StatusVector.bits.ByteCount > 1518u ||
			!ETHHeader.StatusVector.bits.ReceiveOk)
		{
			Reset();
		}

		// Save the location where the hardware will write the next packet to
		NextPacketLocation.Val = ETHHeader.NextPacketPointer;

		// Obtain the packet data (or first 576+6 data bytes, if too big)
		BufferLen = sizeof(vTXRXBuffer) > ETHHeader.StatusVector.bits.ByteCount-18u ? ETHHeader.StatusVector.bits.ByteCount-18 : sizeof(vTXRXBuffer);
		MACGetArray(vTXRXBuffer, BufferLen);

		// Discard the packet so future ones can be received
		MACDiscardRx();

		// See if this is an IP or ARP packet; we don't know how to handle anything else
		switch(ETHHeader.MACHeader.Type.Val)
		{
		case 0x0608:		// 0x0806 is ARP type, but this is big endian
			ARPProcess(&ETHHeader, (ARP_HEADER*)vTXRXBuffer);
			break;

		case 0x0008:		// 0x0800 is IP type, but this is big endian
			// "Get" IP Header
			IPHeaderPtr = (IP_HEADER*)vTXRXBuffer;

			// Make sure this is IPv4; we don't know how to handle anything else
			if((IPHeaderPtr->VersionLength & 0xF0) != 0x40u)
				continue;

			// Make sure that this isn't an IP fragment.  We 
			// aren't sophisticated enough or have enough RAM 
			// to reconstruct fragmented datagrams.  IP 
			// specification assures that packets of 576 
			// bytes or less can propagate across the Internet 
			// without being fragmented.
			// Note: FlagsFragmentOffset is big endian, so 
			// we must use 0xFF3F instead of 0x3FFF.
			if(IPHeaderPtr->FlagsFragmentOffset.Val & 0xFF3F)
				continue;

			// Little-endianize the important IP field (Length)
			wv.Val = IPHeaderPtr->Length.Val;
			wv.v[0] ^= wv.v[1];
			wv.v[1] ^= wv.v[0];
			wv.v[0] ^= wv.v[1];

			// Calculate IP header length and data length
			i = (IPHeaderPtr->VersionLength & 0x0F)<<2;
			if(BufferLen > wv.Val - i)
				BufferLen = wv.Val - i;

			// "Get" UDP header
			UDPHeaderPtr = (UDP_HEADER*)&vTXRXBuffer[i];

			// Validate the UDP header
			if(UDPHeaderPtr->DestinationPort.Val != (HIGH(TFTP_UDP_PORT) | ((WORD)LOW(TFTP_UDP_PORT))<<8))
				continue;

			// Validate the application data length
			BufferLen -= sizeof(UDP_HEADER);
			if((SHORT)BufferLen <= 0)
				continue;

			// This seems to be data for the bootloader application, let's process it
			TFTPBootloadProcess(&ETHHeader, IPHeaderPtr, UDPHeaderPtr, (BYTE*)((WORD)UDPHeaderPtr+sizeof(UDP_HEADER)), BufferLen);
			break;
		}
	}
}//end Bootload()


/******************************************************************************
* Function:        static void ARPProcess(ETH_HEADER* ETHHeaderPtr, ARP_HEADER* ARPHeaderPtr)
*
* PreCondition:    None
*
* Input:           ETHHeaderPtr: Pointer to the Ethernet header for the last received packet
*					ARPHeaderPtr: Pointer to the ARP header for the last received packet
*
* Output:          Any incomming ARP broadcast requests for our IP address 
*					is serviced with an ARP unicast reply.
*
* Side Effects:    None
*
* Overview:        None
*
* Note:            None
*****************************************************************************/
static void ARPProcess(ETH_HEADER* ETHHeaderPtr, ARP_HEADER* ARPHeaderPtr)
{
	DWORD_VAL dwv;
	ARP_HEADER ARPHeader;

	// Validate ARP fields
	memcpy((void*)&ARPHeader, (void*)ARPHeaderPtr, sizeof(ARPHeader));
	if(	(ARPHeader.TargetIPAddr.Val != MyAddress.dwvIP.Val) || // Are we the destination?
		(ARPHeader.HardwareType != 0x0100u) ||	// Ethernet Type
		(ARPHeader.ProtocolType != 0x0008u) ||	// IP Type
		(ARPHeader.HardwareSize != 6u)		||	// Sizeof MAC address
		(ARPHeader.ProtocolSize != 4u)		||	// Sizeof IP address
		(ARPHeader.Opcode != 0x0100u) )			// ARP Request opcode
		return;

	// Put Ethernet controller header
	// Put MAC header
	memcpy((void*)&vTXBuffer[0], (void*)&ETHHeaderPtr->MACHeader.SourceMAC[0], 6);// MAC Header: Destination MAC address
	vTXBuffer[6]  = MyAddress.vMAC[0];				// MAC Header: Source MAC address
	vTXBuffer[7]  = MyAddress.vMAC[1];
	vTXBuffer[8]  = MyAddress.vMAC[2];
	vTXBuffer[9]  = MyAddress.vMAC[3];
	vTXBuffer[10] = MyAddress.vMAC[4];
	vTXBuffer[11] = MyAddress.vMAC[5];
	vTXBuffer[12] = 0x08;				// MAC Header: Type (ARP)
	vTXBuffer[13] = 0x06;

	// Put ARP Header
	vTXBuffer[14] = 0x00;				// ARP Header: Hardware Type (Ethernet)
	vTXBuffer[15] = 0x01;
	vTXBuffer[16] = 0x08;				// ARP Header: Protocol Type (IP)
	vTXBuffer[17] = 0x00;
	vTXBuffer[18] = 0x06;				// ARP Header: Hardware Size (6)
	vTXBuffer[19] = 0x04;				// ARP Header: Protocol Size (4)
	vTXBuffer[20] = 0x00;				// ARP Header: Opcode (Reply)
	vTXBuffer[21] = 0x02;
	vTXBuffer[22] = MyAddress.vMAC[0];		// ARP Header: Sender MAC address
	vTXBuffer[23] = MyAddress.vMAC[1];
	vTXBuffer[24] = MyAddress.vMAC[2];
	vTXBuffer[25] = MyAddress.vMAC[3];
	vTXBuffer[26] = MyAddress.vMAC[4];
	vTXBuffer[27] = MyAddress.vMAC[5];
	vTXBuffer[28] = MyAddress.dwvIP.v[0];			// ARP Header: Sender IP address
	vTXBuffer[29] = MyAddress.dwvIP.v[1];
	vTXBuffer[30] = MyAddress.dwvIP.v[2];
	vTXBuffer[31] = MyAddress.dwvIP.v[3];
	memcpy((void*)&vTXBuffer[32], (void*)&ARPHeaderPtr->SenderMACAddr[0], 6); // ARP Header: Target MAC address
	((DWORD_VAL*)&vTXBuffer[38])->Val = ARPHeaderPtr->SenderIPAddr.Val;	// ARP Header: Target IP address

	// Put the packet in the Ethernet controller and send it
	EWRPT = TXSTART+1;
	while(!MACIsTxReady());
	MACPutArray(vTXBuffer, sizeof(MAC_HEADER)+sizeof(ARP_HEADER));
	ETXND = TXSTART+sizeof(MAC_HEADER)+sizeof(ARP_HEADER);
	MACFlush();
}


/******************************************************************************
* Function:        static void TFTPBootloadProcess(ETH_HEADER* ETHHeaderPtr, 
*												IP_HEADER* IPHeaderPtr, 
*												UDP_HEADER* UDPHeaderPtr, 
*												BYTE* AppDataPtr, 
*												WORD AppDataLen)
*
* PreCondition:    None
*
* Input:           ETHHeaderPtr: Pointer to the Ethernet header for the last 
*						received packet
*					IPHeaderPtr: Pointer to the IP header for the last 
*						received packet
*					UDPHeaderPtr: Pointer to the UDP header for the last 
*						received packet
*					AppDataPtr: Pointer to the application data contined in 
*						the last received UDP packet
*					AppDataLen: Number of applications bytes available at 
*						the AppDataPtr location
*
* Output:          Core bootloader operations occur (Read, Write, Erase)
*
* Side Effects:    In the worst case, this function can take several seconds 
*					to execute due to the time required for programming and 
*					erase operations.
*
* Overview:        None
*
* Note:            None
*****************************************************************************/
#define TFTP_READ	0x0001u
#define TFTP_WRITE	0x0002u
#define TFTP_DATA	0x0003u
#define TFTP_ACK	0x0004u
#define TFTP_ERROR	0x0005u
static void TFTPBootloadProcess(ETH_HEADER* ETHHeaderPtr, IP_HEADER* IPHeaderPtr, UDP_HEADER* UDPHeaderPtr, BYTE* AppDataPtr, WORD AppDataLen)
{
	static WORD		wNeededBlock;
	WORD_VAL		wvTFTPOpcode;
	WORD_VAL		wvBlockNumber;
	WORD_VAL		wvPacketLen;
	BYTE			i, j;

	// Make sure we don't exit the bootloader while it is being used
	dwBootloaderExitTimer = GetTime();

	// Get the TFTP opcode
	wvTFTPOpcode.v[1] = *AppDataPtr++;
	wvTFTPOpcode.v[0] = *AppDataPtr++;
	AppDataLen -= 2;

	// Set the correct location to write the TFTP response packet
	EWRPT = ETXST + 1 + sizeof(MAC_HEADER) + sizeof(IP_HEADER) + sizeof(UDP_HEADER);
	while(!MACIsTxReady());

	// Execute the TFTP opcode
	switch(wvTFTPOpcode.Val)
	{
#if defined(ENABLE_READ_BACK)
		case TFTP_ACK:		// ACK (request more read data)
			wvBlockNumber.v[1] = *AppDataPtr++;
			wvBlockNumber.v[0] = *AppDataPtr++;
			AppDataLen -= 2;
			wvBlockNumber.Val++;
			// No break
	
		case TFTP_READ:		// Read Request
			if(!CONFIG1Hbits.CP0)
			{
				wvPacketLen.Val = 4;
				vTXBuffer[0] = HIGH(TFTP_ERROR);		// TFTP Opcode: Error
				vTXBuffer[1] = LOW(TFTP_ERROR);
				vTXBuffer[2] = 0x00;					// TFTP Errorcode: Access violation
				vTXBuffer[3] = 0x02;
				#undef ERROR_MESSAGE
				#define ERROR_MESSAGE	"Code Protect Enabled"
				memcpypgm2ram(&vTXBuffer[4], (rom void*)ERROR_MESSAGE, sizeof(ERROR_MESSAGE));
				MACPutArray(vTXBuffer, 4+sizeof(ERROR_MESSAGE));
				wvPacketLen.Val += sizeof(ERROR_MESSAGE);
				break;
			}
		
			if(wvTFTPOpcode.Val == TFTP_READ)
				wvBlockNumber.Val = 0x0001;
	
			// Calculate the size of our response packet, excluding the Ethernet header
			wvPacketLen.Val = 512 + 4;
			if(wvBlockNumber.Val == (TOTAL_MEMORY_SIZE/512ul)+1)
				wvPacketLen.Val = 0 + 4;
			else if(wvBlockNumber.Val > (TOTAL_MEMORY_SIZE/512ul)+1)
				return;
	
			// Put Application data
			vTXBuffer[0] = HIGH(TFTP_DATA);		// TFTP Opcode: Data
			vTXBuffer[1] = LOW(TFTP_DATA);
			vTXBuffer[2] = wvBlockNumber.v[1];	// TFTP Block # (0x0001 for first packet)
			vTXBuffer[3] = wvBlockNumber.v[0];
			MACPutArray(vTXBuffer, 4);
	
			// Read FLASH words
			TBLPTR = ((DWORD)(wvBlockNumber.Val - 1)) * 512ul;
			for(j = 0; j < 8u; j++)
			{
				for(i = 0; i < 64u; i++)
				{
					_asm TBLRDPOSTINC _endasm
						vTXBuffer[i] = TABLAT;
				}
				MACPutArray(vTXBuffer, 64);
			}
			break;
#endif
	
		case TFTP_WRITE:	// Write Request
			wvBlockNumber.Val = 0;
	
			// Calculate the size of our response packet, excluding the Ethernet header
			wvPacketLen.Val = 0 + 4;
	
			// Put Application data
			vTXBuffer[0] = HIGH(TFTP_ACK);		// TFTP Opcode: ACK
			vTXBuffer[1] = LOW(TFTP_ACK);
			vTXBuffer[2] = wvBlockNumber.v[1];	// TFTP Block # (0 for initial write request)
			vTXBuffer[3] = wvBlockNumber.v[0];
			MACPutArray(vTXBuffer, 4);
	
			// Erase all of our Flash memory, we are about to receive data
			bWriteModeEnabled = TRUE;
			memset((void*)vFlashData, 0xFF, FLASH_WRITE_SIZE);
			EraseFlash(0x00000000, 0xFFFFFFFF);
	
			// Initialize the HEX file decoder
			wNeededBlock = 1;
			DecodeHex(NULL, NULL, NULL, NULL, TRUE);
			break;
	
		case TFTP_DATA:		// Writing data
			if(!bWriteModeEnabled)
				return;
	
			// Get block number
			wvBlockNumber.v[1] = *AppDataPtr++;
			wvBlockNumber.v[0] = *AppDataPtr++;
			AppDataLen -= 2;
	
			// Calculate the size of our response packet, excluding the Ethernet header
			wvPacketLen.Val = 0 + 4;
	
			// TFTP packets cannot have more than 512 bytes of data in it
			if(AppDataLen > 512u)
				return;
	
			// Retransmit an ACK if the remote node doesn't receive our 
			// last ACK and retransmits old data to us
			if(wvBlockNumber.Val == wNeededBlock - 1)
			{
				goto SendACK;
			}

			if(wvBlockNumber.Val != wNeededBlock)
				return;
				
			wNeededBlock++;
	
	
			// Perform the decode and flash write
			while(AppDataLen)
			{
				static DWORD dwAddress;	// Doesn't need to be declared static, but done so to reduce code size.
				WORD w;
				CHAR cRet;
	
				w = AppDataLen;
				cRet = DecodeHex(AppDataPtr, &w, vFlashData, &dwAddress, FALSE);
				switch(cRet)
				{
					case DECODE_ERROR_CHECKSUM:
					case DECODE_ERROR_INVALID_FORMAT:
						bWriteModeEnabled = FALSE;
						// Put Application data
						vTXBuffer[0] = HIGH(TFTP_ERROR);		// TFTP Opcode: Error
						vTXBuffer[1] = LOW(TFTP_ERROR);
						vTXBuffer[2] = 0x00;					// TFTP Errorcode: Not defined, see error message
						vTXBuffer[3] = 0x00;
						#undef ERROR_MESSAGE
						#define ERROR_MESSAGE	"Invalid .hex file"
						memcpypgm2ram(&vTXBuffer[4], (rom void*)ERROR_MESSAGE, sizeof(ERROR_MESSAGE));
						MACPutArray(vTXBuffer, 4+sizeof(ERROR_MESSAGE));
						wvPacketLen.Val += sizeof(ERROR_MESSAGE);
						goto SendTFTPResponse;
	
					case DECODE_SUCCESS_NEED_MORE_HEXDATA:
						break;
	
					case DECODE_SUCCESS_EOF:
						// See if there is any last data that we need to write
						for(i = 0; i < sizeof(vFlashData); i++)
						{
							if(vFlashData[i] != 0xFF)
							{
								if(!WriteFlashBlock(dwAddress, vFlashData, NULL))
								{
									goto ValidationError;
								}
								
								break;
							}
						}

						// Flash write the new application start vector into our remapped address
						if(bNeedUpdatedJumpTable)
						{
							if(!RawWriteFlashBlock((DWORD)&dwJumpInstructions[0], vJumpTableRAM, NULL))
							{
								goto ValidationError;
							}
						}
						
						break;

					case DECODE_SUCCESS_NEW_ADDRESS:
						if(!WriteFlashBlock(dwAddress, vFlashData, NULL))
						{
ValidationError:							
							bWriteModeEnabled = FALSE;
							// Put Application data
							vTXBuffer[0] = HIGH(TFTP_ERROR);		// TFTP Opcode: Error
							vTXBuffer[1] = LOW(TFTP_ERROR);
							vTXBuffer[2] = 0x00;					// TFTP Errorcode: Not defined, see error message
							vTXBuffer[3] = 0x00;
							#undef ERROR_MESSAGE
							#define ERROR_MESSAGE	"Verification failed.  Attempting to overwrite bootloader memory?"
							memcpypgm2ram(&vTXBuffer[4], (rom void*)ERROR_MESSAGE, sizeof(ERROR_MESSAGE));
							MACPutArray(vTXBuffer, 4+sizeof(ERROR_MESSAGE));
							wvPacketLen.Val += sizeof(ERROR_MESSAGE);
							goto SendTFTPResponse;
						}
						memset((void*)vFlashData, 0xFF, FLASH_WRITE_SIZE);
						break;
				}
				AppDataLen -= w;
				AppDataPtr += w;
			}
	
SendACK:							
			// Put Application data
			vTXBuffer[0] = HIGH(TFTP_ACK);		// TFTP Opcode: ACK
			vTXBuffer[1] = LOW(TFTP_ACK);
			vTXBuffer[2] = wvBlockNumber.v[1];	// TFTP Block # (1 for first data packet)
			vTXBuffer[3] = wvBlockNumber.v[0];
			MACPutArray(vTXBuffer, 4);
			break;
	
		case TFTP_ERROR:	// Error: User aborted while flashing perhaps?
			if(bWriteModeEnabled)
			{
				// Erase all of our Flash memory, we may have only a partial write
				EraseFlash(0x00000000, 0xFFFFFFFF);
				bWriteModeEnabled = FALSE;
			}
			return;

		default:
			// Unsupported operation
			wvPacketLen.Val = 4;
			vTXBuffer[0] = HIGH(TFTP_ERROR);		// TFTP Opcode: Error
			vTXBuffer[1] = LOW(TFTP_ERROR);
			vTXBuffer[2] = 0x00;					// TFTP Errorcode: 0x0004 Illegal TFTP operation
			vTXBuffer[3] = 0x04;
			#undef ERROR_MESSAGE
			#define ERROR_MESSAGE	"Unsupported"
			memcpypgm2ram(&vTXBuffer[4], (rom void*)ERROR_MESSAGE, sizeof(ERROR_MESSAGE));
			MACPutArray(vTXBuffer, 4+sizeof(ERROR_MESSAGE));
			wvPacketLen.Val += sizeof(ERROR_MESSAGE);
			break;
	}

SendTFTPResponse:

	// Set correct location to write to
	EWRPT = ETXST + 1;

	// Put MAC header
	memcpy((void*)&vTXBuffer[0], (void*)ETHHeaderPtr->MACHeader.SourceMAC, 6);// MAC Header: Destination MAC address
	vTXBuffer[6]  = MyAddress.vMAC[0];		// MAC Header: Source MAC address
	vTXBuffer[7]  = MyAddress.vMAC[1];
	vTXBuffer[8]  = MyAddress.vMAC[2];
	vTXBuffer[9]  = MyAddress.vMAC[3];
	vTXBuffer[10] = MyAddress.vMAC[4];
	vTXBuffer[11] = MyAddress.vMAC[5];
	vTXBuffer[12] = 0x08;				// MAC Header: Type (IP)
	vTXBuffer[13] = 0x00;

	// Put IP Header
	wvPacketLen.Val += sizeof(IP_HEADER) + sizeof(UDP_HEADER);
	vTXBuffer[14] = 0x45;				// IP Header: IP Version & Header Length
	vTXBuffer[15] = 0x00;				// IP Header: Differentiated Services
	vTXBuffer[16] = wvPacketLen.v[1];	// IP Header: Total Length
	vTXBuffer[17] = wvPacketLen.v[0];
	vTXBuffer[18] = 0x00;				// IP Header: Identification
	vTXBuffer[19] = 0x00;
	vTXBuffer[20] = 0x00;				// IP Header: Flags and Fragment Offset
	vTXBuffer[21] = 0x00;
	vTXBuffer[22] = 128;				// IP Header: Time to Live (TTL)
	vTXBuffer[23] = 0x11;				// IP Header: Protocol (UDP)
	vTXBuffer[24] = 0x00;				// IP Header: Header Checksum (a zero place holder right now)
	vTXBuffer[25] = 0x00;
	vTXBuffer[26] = MyAddress.dwvIP.v[0];	// IP Header: Source IP (ours)
	vTXBuffer[27] = MyAddress.dwvIP.v[1];
	vTXBuffer[28] = MyAddress.dwvIP.v[2];
	vTXBuffer[29] = MyAddress.dwvIP.v[3];
	*((DWORD*)&vTXBuffer[30]) = IPHeaderPtr->SourceIP.Val;			// IP Header: Destination IP (UDP)
	*((WORD*)&vTXBuffer[24]) = CalcIPChecksum(&vTXBuffer[14], 20);	// IP Header: Header Checksum (this time with real value)

	// Put UDP header
	wvPacketLen.Val -= sizeof(IP_HEADER);	
	vTXBuffer[34] = HIGH(TFTP_UDP_PORT);					// UDP Header: Source Port
	vTXBuffer[35] = LOW(TFTP_UDP_PORT);
	*((WORD_VAL*)&vTXBuffer[36]) = UDPHeaderPtr->SourcePort;// UDP Header: Destination Port
	vTXBuffer[38] = wvPacketLen.v[1];						// UDP Header: Length
	vTXBuffer[39] = wvPacketLen.v[0];
	vTXBuffer[40] = 0x00;									// UDP Header: Checksum
	vTXBuffer[41] = 0x00;

	// Put the headers in the Ethernet controller and send the packet
	MACPutArray(vTXBuffer, sizeof(MAC_HEADER) + sizeof(IP_HEADER) + sizeof(UDP_HEADER));
	ETXND = TXSTART + sizeof(MAC_HEADER) + sizeof(IP_HEADER) + wvPacketLen.Val;
	MACFlush();
}

/******************************************************************************
* Function:        static BYTE HexToBin(BYTE c)
*
* PreCondition:    None
*
* Input:           One byte of ASCII data in the set
*					{'0','1','2','3','4','5','6','7',
*					 '8','9','a','b','c','d','e','f',
*					 'A','B','C','D','E','F'}
*
* Output:          One byte of binary data in the set:
*					{0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,
*					 0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F}
*
* Side Effects:    None
*
* Overview:        None
*
* Note:            None
*****************************************************************************/
static BYTE HexToBin(BYTE c)
{
	if(c >= '0' && c <= '9')
		return c - '0';
	if(c >= 'a' && c <= 'f')
		return c - 'a' + 10;

	return c - 'A' + 10;
}


/******************************************************************************
* Function:        static CHAR DecodeHex(BYTE *vHexData, WORD *wHexDataLen, BYTE *vMemoryData, DWORD *dwAddress, BOOL bInitialize)
*
* PreCondition:    None
*
* Input:           *vHexData: Pointer to Intel HEX formatted data to read
*					*wHexDataLen: Pointer to a WORD containing the number of wHexData bytes that are available
*					*vMemoryData: Pointer to RAM bytes were binary decoded data will be written.  There must be exactly FLASH_WRITE_SIZE bytes available at this address.
*					bInitialize: TRUE for very first call, FALSE for all subsequent calls of the same .hex file.
*
* Output:          wHexDataLen: WORD containing the number of wHexData bytes that were processed from the *vHexData stream
*					vMemoryData: RAM bytes containing binary data decoded from the hex stream
*					dwAddress: DWORD containing the base memory address where you should write the vMemoryData array
*					CHAR: < 0 on error (checksum error, illegal data in hex file, etc)
*						  0 on success and need more data
*						  > 0 on success and end of file record found
*
* Side Effects:    None
*
* Overview:        Converts an Intel HEX file stream into a stream of binary 
*					data to be programming into Flash memory.
*
* Note:            None
*****************************************************************************/
static CHAR DecodeHex(BYTE *vHexData, WORD *wHexDataLen, BYTE *vMemoryData, DWORD *dwAddress, BOOL bInitialize)
{
	static enum
	{
		GetColon = 0,
		GetRecordLength1,
		GetRecordLength0,
		GetAddress3,
		GetAddress2,
		GetAddress1,
		GetAddress0,
		GetRecordType1,
		GetRecordType0,
		GetDataBytes1,
		GetDataBytes0,
		GetChecksum1,
		GetChecksum0
	} smRecordParser;
	static BYTE vRecordByte;
	static BYTE vRecordLen;
	static WORD_VAL wvVolatileAddress;
	static WORD_VAL wvLatchedAddress;
	static DWORD_VAL dwvVolatileExtAddress;
	static DWORD_VAL dwvLatchedExtAddress;
	static DWORD dwTemp;
	static DWORD dwLastWriteBlockAddress;
	static BYTE vRecordType;
	static BYTE vChecksum;
	static BOOL bByteWritten;

	BYTE vEncodedByte; 
	BYTE vDecodedByte;
	WORD w;
	static WORD wHexLen;	// Doesn't need to be declared static, but done so to reduce code size.
	WORD wHexDataLenLocal;

	// Reset state machine if this is a first time
	if(bInitialize)
	{
		wvVolatileAddress.Val = 0;
		wvLatchedAddress.Val = 0;
		dwvVolatileExtAddress.Val = 0;
		dwvLatchedExtAddress.Val = 0;
		smRecordParser = GetColon;
		bByteWritten = FALSE;
		return DECODE_SUCCESS_NEED_MORE_HEXDATA;
	}

	wHexLen = *wHexDataLen;
	wHexDataLenLocal = 0;

	while(wHexLen)
	{
		wHexLen--;
		vEncodedByte = vHexData[wHexDataLenLocal];
		wHexDataLenLocal++;
		*wHexDataLen = wHexDataLenLocal;
		vDecodedByte = HexToBin(vEncodedByte);
		switch(smRecordParser)
		{
			case GetColon:
				// Skip whitespace
				if(vEncodedByte == '\r')
					break;
				if(vEncodedByte == '\n')
					break;
				if(vEncodedByte == ' ')
					break;

				// Make sure this character is a colon
				if(vEncodedByte != ':')
					return DECODE_ERROR_INVALID_FORMAT;

				smRecordParser = GetRecordLength1;
				break;
	
			case GetRecordLength1:
				vRecordLen = vDecodedByte<<4;
				smRecordParser = GetRecordLength0;
				break;
			case GetRecordLength0:
				vRecordLen |= vDecodedByte;
				vChecksum = vRecordLen;
				smRecordParser = GetAddress3;
				break;
	
			case GetAddress3:
				wvVolatileAddress.v[1] = vDecodedByte<<4;
				smRecordParser = GetAddress2;
				break;
			case GetAddress2:
				wvVolatileAddress.v[1] |= vDecodedByte;
				vChecksum += wvVolatileAddress.v[1];
				smRecordParser = GetAddress1;
				break;
			case GetAddress1:
				wvVolatileAddress.v[0] = vDecodedByte<<4;
				smRecordParser = GetAddress0;
				break;
			case GetAddress0:
				wvVolatileAddress.v[0] |= vDecodedByte;
				vChecksum += wvVolatileAddress.v[0];
				smRecordParser = GetRecordType1;
				w = wvLatchedAddress.Val;
				wvLatchedAddress.Val = wvVolatileAddress.Val;

				// Make sure we haven't moved out of this flash write page
				if(bByteWritten)
				{
					dwTemp = dwvLatchedExtAddress.Val + wvLatchedAddress.Val;
					if((dwTemp >= dwLastWriteBlockAddress + FLASH_WRITE_SIZE) || (dwTemp < dwLastWriteBlockAddress))
					{
						// Fill unprogramed locations in this write block with 0xFF bytes
						w &= (FLASH_WRITE_SIZE-1);
						memset(&vMemoryData[w], 0xFF, FLASH_WRITE_SIZE - w);
						
						bByteWritten = FALSE;
						return DECODE_SUCCESS_NEW_ADDRESS;
					}
				}
				break;
	
			case GetRecordType1:
				vRecordType = vDecodedByte<<4;
				smRecordParser = GetRecordType0;
				break;
			case GetRecordType0:
				vRecordType |= vDecodedByte;
				vChecksum += vRecordType;
				if(vRecordType == 0x01u)  // End of records marker found
				{
					smRecordParser = GetChecksum1;
					return DECODE_SUCCESS_EOF;
				}
				else if(vRecordType == 0x02u || vRecordType == 0x04u)     // Extended Segment Address Record or Extended Linear Address Record
				{
					if(vRecordLen != 2u)
						return DECODE_ERROR_INVALID_FORMAT;
				}
				else if(vRecordType != 0x00u) // Unrecognized record type found
				{
					return DECODE_ERROR_INVALID_FORMAT;
				}
	
				smRecordParser = vRecordLen ? GetDataBytes1 : GetChecksum1;
				break;
	
			case GetDataBytes1:
				vRecordByte = vDecodedByte<<4;
				smRecordParser = GetDataBytes0;
				break;
			case GetDataBytes0:
				vRecordLen--;
				smRecordParser = GetDataBytes1;
				vRecordByte |= vDecodedByte;
				vChecksum += vRecordByte;
	
				if(vRecordType == 0x00u)
				{
					if(!bByteWritten)
					{
						dwTemp = dwvLatchedExtAddress.Val + wvLatchedAddress.Val;
						dwLastWriteBlockAddress = dwTemp & ~(FLASH_WRITE_SIZE-1ul);
						*dwAddress = dwLastWriteBlockAddress;
						bByteWritten = TRUE;
					}
					vMemoryData[wvLatchedAddress.v[0] & (FLASH_WRITE_SIZE-1)] = vRecordByte;
					wvLatchedAddress.Val++;
				}
				else if(vRecordType == 0x02u)
				{
					// See if this is the high byte or low byte
					if(vRecordLen)
					{
						dwvVolatileExtAddress.v[3] = 0x00;
						dwvVolatileExtAddress.v[2] = vRecordByte>>4;
						dwvVolatileExtAddress.v[1] = vRecordByte<<4;
					}
					else
					{
						dwvVolatileExtAddress.v[1] |= vRecordByte>>4;
						dwvVolatileExtAddress.v[0] = vRecordByte<<4;
						dwvLatchedExtAddress.Val = dwvVolatileExtAddress.Val;
					}
				}
				else if(vRecordType == 0x04u)
				{
					// See if this is the high byte or low byte
					if(vRecordLen)
					{
						dwvVolatileExtAddress.v[3] = vRecordByte;
						dwvVolatileExtAddress.v[1] = 0x00;
						dwvVolatileExtAddress.v[0] = 0x00;
					}
					else
					{
						dwvVolatileExtAddress.v[2] = vRecordByte;
						dwvLatchedExtAddress.Val = dwvVolatileExtAddress.Val;
					}
				}

				if(vRecordLen == 0u)
					smRecordParser = GetChecksum1;

				// Make sure we haven't moved out of this flash write page
				if(bByteWritten)
				{
					dwTemp = dwvLatchedExtAddress.Val + wvLatchedAddress.Val;
					if((dwTemp >= *dwAddress + FLASH_WRITE_SIZE) || (dwTemp < *dwAddress))
					{
						bByteWritten = FALSE;
						return DECODE_SUCCESS_NEW_ADDRESS;
					}
				}

				break;
	
			case GetChecksum1:
				vRecordByte = vDecodedByte<<4;
				smRecordParser = GetChecksum0;
				break;
			case GetChecksum0:
				vRecordByte |= vDecodedByte;
				if(vRecordByte != (~vChecksum + 1))
					return DECODE_ERROR_CHECKSUM;
				smRecordParser = GetColon;
				break;
		}
	}

	return DECODE_SUCCESS_NEED_MORE_HEXDATA;
}


/******************************************************************************
* Function:        static void EraseFlash(DWORD Address, DWORD EraseByteLength)
*
* PreCondition:    None
*
* Input:           Address: Begining byte address to start erasing at.  Note 
*							 that this address must be page aligned (ie: evenly 
*							 divisible by FLASH_ERASE_SIZE).  If it is not page 
*							 aligned, the function will make it page aligned 
*							 before performing the erase.  If erasing the 
*							 entire part, use address 0x00000000.
*					EraseByteLength: Number of bytes to erase.  This number 
*									 must be evenly divisible by FLASH_ERASE_SIZE.  
*									 If not, this function will erase extra 
*									 bytes to meet the required FLASH_ERASE_SIZE.
*									 If erasing the entire part, use length 
*									 0xFFFFFFFF.
*
* Output:          Erases the specified location, limited by the Flash 
*					technology's FLASH_ERASE_SIZE.  Extra bytes before and 
*					after the region will be erased if using non-FLASH_ERASE_SIZE 
*					aligned addresses or sizes.
*
* Side Effects:    None
*
* Overview:        None
*
* Note:            This function is bootloader safe.  If an attempt to erase 
*					the addresses containing the bootloader is made, the 
*					function will not perform the erase.
*****************************************************************************/
static void EraseFlash(DWORD Address, DWORD EraseByteLength)
{
	static DWORD ErasePageAddress;		// Doesn't need to be declared static, but done so to reduce code size.
	static DWORD ErasePageEndAddress;	// Doesn't need to be declared static, but done so to reduce code size.
	DWORD dwBootloaderStart, dwBootloaderEnd;
	WORD w;
	BYTE vAndSum;
	BOOL bNeedLastPageErased;

	// Calculate starting and ending address to erase
	ErasePageAddress = Address & ~(FLASH_ERASE_SIZE-1ul);
	ErasePageEndAddress = (Address + EraseByteLength - 1ul) & ~((DWORD)(FLASH_ERASE_SIZE-1ul));
	if(ErasePageEndAddress >= (DWORD)TOTAL_MEMORY_SIZE)
		ErasePageEndAddress = TOTAL_MEMORY_SIZE-FLASH_ERASE_SIZE;

	// Begin looping and erasing pages, if any, if needed
	if(EraseByteLength)
	{
		bNeedLastPageErased = FALSE;

		// Note that this loop progresses backwards through memory.  This is to ensure that the 
		// goto main instruction at address 0x00000000 does not get erased before any of the other code.
		// In the event of a power failure or incomplete erase, we need either the goto main 
		// instruction at address 0x00000000 or a full block of empty program memory between 
		// address 0x0000000 and the start of main() (execute as Nop()).
		for(; ErasePageEndAddress >= ErasePageAddress; ErasePageEndAddress -= FLASH_ERASE_SIZE)
		{
			// The last page containing the config fuses and bootloader 
			// configuration data is special.  It can be erased, and can 
			// contain application data, but it must be done through the 
			// EraseLastPage() function.  Also, because it contains the 
			// code protect configuration bit, we want to erase it last
			// to prevent a customer from performing a "bulk" erase and 
			// resetting the part before the rest of the code is erased, 
			// thus defeating the code protection.  By erasing it last,
			// we are ensured that no code is in the part, except for 
			// the bootloader itself when the code protect bit is erased.
			if(ErasePageEndAddress == ((INTERNAL_FLASH_SIZE-1) & ~(FLASH_ERASE_SIZE-1ul)))
			{
				bNeedLastPageErased = TRUE;
			}
			else
			{
				// Perform the normal erase
				RawEraseFlashBlock(ErasePageEndAddress);
			}

			// Make sure to not underflow and continue forever if 
			// ErasePageAddress is zero
			if(ErasePageEndAddress == 0ul)
			{
				break;
			}
		}

		// Finally, erase the last page, including code protect, if we were 
		// commanded to do so
		if(bNeedLastPageErased)
		{
			EraseLastPage(TRUE, FALSE);
		}
	}
}

/******************************************************************************
* Function:        static void RawEraseFlashBlock(DWORD dwAddress)
*
* PreCondition:    None
*
* Input:           dwAddress: Begining byte address to start erasing at.  Note 
*							   that this address must be page aligned (ie: evenly 
*							   divisible by FLASH_ERASE_SIZE). 
*
* Output:          Erases the specified page, limited by the Flash 
*					technology's FLASH_ERASE_SIZE.
*
* Side Effects:    None
*
* Overview:        None
*
* Note:            This function is bootloader safe.  If an attempt to erase 
*					the addresses containing the bootloader is made, the 
*					function will not perform the erase.
*****************************************************************************/
static void RawEraseFlashBlock(DWORD dwAddress)
{
	static DWORD dwBootloaderStart, dwBootloaderEnd;	// Doesn't need to be declared static, but done so to reduce code size.
	WORD w;
	BYTE vAndSum;

	// Guarantee that we make calculations using the real erase address 
	// supported by the hardware
	dwAddress &= ~(FLASH_ERASE_SIZE-1ul);

	// Calculate non-erasable addresses
	dwBootloaderStart = (DWORD)&main & ~(FLASH_ERASE_SIZE-1ul);
	dwBootloaderEnd = dwBootloaderStart + BOOTLOADER_SIZE-1ul;

	// Make sure that we do not erase the Bootloader
	if((dwAddress >= dwBootloaderStart) && (dwAddress <= dwBootloaderEnd))
		return;

	// Blank checking and erasing can take considerable time
	ClrWdt();

	// Do a blank check.  No need to waste time erasing a blank page.
	vAndSum = 0xFF;
	TBLPTR = dwAddress;
#if FLASH_ERASE_SIZE % 8 == 0
	for(w = 0; w < FLASH_ERASE_SIZE/8; w++)
	{
		// Read 8 bytes per loop iteration to increase performance
		_asm TBLRDPOSTINC _endasm
			vAndSum &= TABLAT;
		_asm TBLRDPOSTINC _endasm
			vAndSum &= TABLAT;
		_asm TBLRDPOSTINC _endasm
			vAndSum &= TABLAT;
		_asm TBLRDPOSTINC _endasm
			vAndSum &= TABLAT;
		_asm TBLRDPOSTINC _endasm
			vAndSum &= TABLAT;
		_asm TBLRDPOSTINC _endasm
			vAndSum &= TABLAT;
		_asm TBLRDPOSTINC _endasm
			vAndSum &= TABLAT;
		_asm TBLRDPOSTINC _endasm
			vAndSum &= TABLAT;

		// Check to see if any of the 8 bytes has data in it
		if(vAndSum != 0xFF)
		{
			// Erasing can take considerable time
			ClrWdt();

			// Data found, erase this page
			TBLPTR = dwAddress;
			EECON1bits.WREN = 1;
			EECON1bits.FREE = 1;
			_asm
				movlw	0x55
				movwf	EECON2, ACCESS
				movlw	0xAA
				movwf	EECON2, ACCESS
				bsf		EECON1, 1, ACCESS	//WR
				_endasm
				EECON1bits.WREN = 0;

			break;
		}
	}
#else
	for(w = 0; w < FLASH_ERASE_SIZE; w++)
	{
		_asm TBLRDPOSTINC _endasm

			// Check to see if any of the 8 bytes has data in it
			if(TABLAT != 0xFF)
			{
				// Erasing can take considerable time
				ClrWdt();

				// Data found, erase this page
				TBLPTR = dwAddress;
				EECON1bits.WREN = 1;
				EECON1bits.FREE = 1;
				_asm
					movlw	0x55
					movwf	EECON2, ACCESS
					movlw	0xAA
					movwf	EECON2, ACCESS
					bsf		EECON1, 1, ACCESS	//WR
					_endasm
					EECON1bits.WREN = 0;

				break;
			}
	}
#endif

	// Blank checking and erasing can take considerable time
	ClrWdt();
}

/******************************************************************************
* Function:        static void EraseLastPage(BOOL bEraseAppData, BOOL bEraseFuses)
*
* PreCondition:    None
*
* Input:           bEraseAppData: TRUE if an erase of the application data is needed
*								   FALSE if the application data should be preserved
*					bEraseFuses: TRUE if the configuration fuses should be unprogrammed -- the caller is going to write them
*								 FALSE if the configuration fuses should be immediately reprogrammed
*
* Output:          None
*
* Side Effects:    None
*
* Overview:        None
*
* Note:            None
*****************************************************************************/
static void EraseLastPage(BOOL bEraseAppData, BOOL bEraseFuses)
{
	WORD w;

	// Save the contents of the last Flash erase page
	TBLPTR = INTERNAL_FLASH_SIZE - FLASH_ERASE_SIZE;
	for(w = 0; w < FLASH_ERASE_SIZE; w++)
	{
		_asm TBLRDPOSTINC _endasm
			vFlashSave[w] = TABLAT;
	}

	// Erase the application data, if needed
	if(bEraseAppData)
		memset((void*)&vFlashSave[0], 0xFF, FLASH_ERASE_SIZE - CONFIG_FUSE_SIZE);

	// Erase the configuration fuses, if needed
	if(bEraseFuses)
		memset((void*)&vFlashSave[FLASH_ERASE_SIZE-CONFIG_FUSE_SIZE], 0xFF, CONFIG_FUSE_SIZE);

	// Erase the last Flash page
	RawEraseFlashBlock(INTERNAL_FLASH_SIZE - FLASH_ERASE_SIZE);

	// Restore all the data that we weren't supposed to erase, starting with the configuration fuses to minimize the time we could lose power and break the boot loader due to lost configuration fuses
	w = FLASH_ERASE_SIZE;
	while(w)
	{
		w -= FLASH_WRITE_SIZE;
		RawWriteFlashBlock(INTERNAL_FLASH_SIZE - FLASH_ERASE_SIZE + w, &vFlashSave[w], NULL);
	}
}


/******************************************************************************
* Function:        static BOOL WriteFlashBlock(DWORD Address, BYTE *BlockData, BYTE *StatusData)
*
* PreCondition:    None
*
* Input:           Address: Location to write to.  This address MUST be an 
*							 integer multiple of the FLASH_WRITE_SIZE constant.
*					*BlockData: Pointer to an array of bytes to write.  The 
*								array is assumed to be exactly FLASH_WRITE_SIZE 
*								in length (or longer).  If longer, only the 
*								first FLASH_WRITE_SIZE bytes will be written.  
*								You must call WriteFlashBlock() again with an 
*								updated Address to write more bytes.
*					*StatusData: Pointer to a byte array of size FLASH_WRITE_SIZE/8 
*								 which will be written with the results of the 
*								 write.  Each bit in the array represents one 
*								 data byte successfully or unsuccessfully 
*								 written.  If the bit is set, the write was 
*								 successful.  Pointer can be NULL if individual 
*								 byte status data is not needed.
*
* Output:          TRUE: If the write was successful or no write was needed 
*						  because the data was already matching.
*					FALSE: If the write failed either because some other data 
*						   was already in the memory (need to perform Erase 
*						   first), or if, following the write, the data did 
*						   not verify correctly (may have run out of Flash 
*						   Erase-Write cycles).
*
* Side Effects:    None
*
* Overview:        Writes one FLASH_WRITE_SIZE block of data to the Flash 
*					memory.  The memory must be erased prior to writing.  
*					Safeguards are in place to prevent multiple writes between 
*					erase cycles or needless writes to locations that already 
*					contain exactly what we want to write.  All needed 
*					bootloader data translation is performed in this function.
*
* Note:            None
*****************************************************************************/
static BOOL WriteFlashBlock(DWORD Address, BYTE *BlockData, BYTE *StatusData)
{
	BYTE i;
	WORD w;
	DWORD dwNewStartupVector;

	// Replace first instruction so the bootloader always starts on power up
	if(Address == 0x00000000ul)
	{
		dwNewStartupVector = 0xF000EF00 | ((((far DWORD)&main)<<7) & 0x0FFF0000) | ((((far DWORD)&main)>>1) & 0x000000FF);

		// See if the user is programming a .hex file read using the bootloader (already has the modified startup vector at address 0x000000)
		// These hex files are illegal (would result in the bootloader just rebooting and relaunching the bootloader, never the application)
		if(*((DWORD*)BlockData) == dwNewStartupVector)
			return FALSE;

		// Save the old instructions at address 0-3 (likely a goto instruction)
		memset((void*)vJumpTableRAM+8, 0xFF, sizeof(vJumpTableRAM)-8);
		vJumpTableRAM[0] = BlockData[0];
		vJumpTableRAM[1] = BlockData[1];
		vJumpTableRAM[2] = BlockData[2];
		vJumpTableRAM[3] = BlockData[3];

		// Insert a goto 0x00000004 instruction following the above application 
		// start vector.  This is required in case if the instruction at address 
		// 0 is not a goto instruction and it is instead at address 4 or higher
		vJumpTableRAM[4] = 0x02;
		vJumpTableRAM[5] = 0xEF;
		vJumpTableRAM[6] = 0x00;
		vJumpTableRAM[7] = 0xF0;

		bNeedUpdatedJumpTable = TRUE;

		// Convert the first instruction at address 0 to a goto main so that the bootloader always starts after reset
		*((DWORD*)BlockData) = dwNewStartupVector;
	}
	else if(Address == INTERNAL_FLASH_SIZE-FLASH_WRITE_SIZE)
	{
		// Ensure the configuration fuses are compatible with the 
		// bootloader to prevent loss of access to the bootloader
#if defined(__EXTENDED18__)
		BlockData[FLASH_WRITE_SIZE-CONFIG_FUSE_SIZE+0] |= 0x40;	// Force CONFIG1L<XINST> bit set
#else
		BlockData[FLASH_WRITE_SIZE-CONFIG_FUSE_SIZE+0] &= 0xBF;	// Force CONFIG1L<XINST> bit clear
#endif
		BlockData[FLASH_WRITE_SIZE-CONFIG_FUSE_SIZE+2] |= 0x04;	// Force CONFIG2L<FOSC2> bit set	(OSC1/OSC2 as primary, not INTRC)
		BlockData[FLASH_WRITE_SIZE-CONFIG_FUSE_SIZE+2] &= 0xFD;	// Force CONFIG2L<FOSC1> bit clear	(HS or HS+PLL mode, not EC or EC+PLL mode)
		// Force Watchdog postscallar 1:4 if either 1:1 or 1:2 is set and the Watchdog enable fuse is set
		if(BlockData[FLASH_WRITE_SIZE-CONFIG_FUSE_SIZE+0] & 0x01)				// Check CONFIG1L<WDTEN>
		{
			if((BlockData[FLASH_WRITE_SIZE-CONFIG_FUSE_SIZE+3] & 0xF) <= 0x1u)	// Check CONFIG2H<WDTPS> is not 1:1 or 1:2
			{
				BlockData[FLASH_WRITE_SIZE-CONFIG_FUSE_SIZE+3] = 0xF2; 
			}
		}
		BlockData[FLASH_WRITE_SIZE-CONFIG_FUSE_SIZE+1] &= 0xF7;	// Force CONFIG1H<3> bit clear, as per data sheet

		// See if the data in this address differs from what we want 
		// to write here.  Since this page contains the device 
		// configuration fuses, this page is special and must always 
		// be written immediately after being erased to prevent losing 
		// access to the bootloader in the event of an interrupted 
		// bootload procedure.  To preserve Flash endurance, we will 
		// check to see if reprogramming is necessary instead of 
		// blindly erasing the last page, even if the configuration 
		// fuses are the same as the old ones.
		TBLPTR = INTERNAL_FLASH_SIZE-FLASH_WRITE_SIZE;
		for(w = 0; w < FLASH_WRITE_SIZE; w++)
		{
			_asm TBLRDPOSTINC _endasm
			i = BlockData[w];
			if(TABLAT != i)
			{
				// Erase last page, preserving application data, but 
				// clearing the last write page, including the 
				// configuration fuses 
				EraseLastPage(FALSE, TRUE);
				break;
			}
		}
	}

	// Perform the Flash write
	return RawWriteFlashBlock(Address, BlockData, StatusData);
}


/******************************************************************************
* Function:        static BOOL RawWriteFlashBlock(DWORD Address, BYTE *BlockData, BYTE *StatusData)
*
* PreCondition:    None
*
* Input:           Address: Location to write to.  This address MUST be an 
*							 integer multiple of the FLASH_WRITE_SIZE constant.
*					*BlockData: Pointer to an array of bytes to write.  The 
*								array is assumed to be exactly FLASH_WRITE_SIZE 
*								in length (or longer).  If longer, only the 
*								first FLASH_WRITE_SIZE bytes will be written.  
*								You must call WriteFlashBlock() again with an 
*								updated Address to write more bytes.
*					*StatusData: Pointer to a byte array of size FLASH_WRITE_SIZE/8 
*								 which will be written with the results of the 
*								 write.  Each bit in the array represents one 
*								 data byte successfully or unsuccessfully 
*								 written.  If the bit is set, the write was 
*								 successful.  Pointer can be NULL if individual 
*								 byte status data is not needed.
*
* Output:          TRUE: If the write was successful or no write was needed 
*						  because the data was already matching.
*					FALSE: If the write failed either because some other data 
*						   was already in the memory (need to perform Erase 
*						   first), or if, following the write, the data did 
*						   not verify correctly (may have run out of Flash 
*						   Erase-Write cycles).
*
* Side Effects:    None
*
* Overview:        Writes one FLASH_WRITE_SIZE block of data to the Flash 
*					memory.  The memory must be erased prior to writing.  
*
* Note:            None
*****************************************************************************/
static BOOL RawWriteFlashBlock(DWORD Address, BYTE *BlockData, BYTE *StatusData)
{
	BYTE i;
	WORD w;
	BOOL WriteNeeded;
	BOOL WriteAllowed;
	BOOL VerifyPassed;


	WriteNeeded = FALSE;
	WriteAllowed = TRUE;
	VerifyPassed = TRUE;

	// Load up the internal Flash holding registers in preperation for the write
	TBLPTR = Address;
	for(w = 0; w < FLASH_WRITE_SIZE; w++)
	{
		_asm TBLRD _endasm
			if(TABLAT != 0xFF)
				WriteAllowed = FALSE;
		i = BlockData[w];
		if(TABLAT != i)
		{
			TABLAT = i;
			WriteNeeded = TRUE;
		}
		_asm TBLWTPOSTINC _endasm
	}
	TBLPTR = Address;

	// Start out assuming that all bytes were written successfully
	if(StatusData != NULL)
		memset(StatusData, 0xFF, FLASH_WRITE_SIZE>>3);

	if(WriteNeeded)
	{
		// Perform the write
		if(WriteAllowed)
		{
			// Writing can take some real time
			ClrWdt();

			// Copy the holding registers into FLASH.  This takes approximately 2.8ms.
			EECON1bits.FREE = 0;
			EECON1bits.WREN = 1;
			_asm
				movlw	0x55
				movwf	EECON2, ACCESS
				movlw	0xAA
				movwf	EECON2, ACCESS
				bsf		EECON1, 1, ACCESS	//WR
				_endasm
				EECON1bits.WREN = 0;

			// Writing can take some real time
			ClrWdt();
		}

		// Verify that the write was performed successfully
		for(w = 0; w < FLASH_WRITE_SIZE; w++)
		{
			_asm TBLRDPOSTINC _endasm
				if(TABLAT != BlockData[w])
				{
					// Uhh oh.  Data is invalid.  
					// You need to perform an erase first 
					// if WriteAllowed is FALSE.
					// If WriteAllowed is TRUE maybe FLASH 
					// ran out of Erase/Write cycles 
					// (endurance problem).
					if(StatusData != NULL)
						StatusData[w>>3] &= ~(1<<(w&0x07));
					VerifyPassed = FALSE;
				}
		}

		return VerifyPassed;
	}

	return TRUE;
}


/******************************************************************************
* Function:        static DWORD GetTime(void)
*
* PreCondition:    None
*
* Input:           None
*
* Output:          Value of a 32-bit free running timer is returned
*
* Side Effects:    None
*
* Overview:        None
*
* Note:            Since no interrupts are used, and only 16-bit timers are 
*					available in hardware, you must call this function 
*					periodically to maintain an accurate 32-bit time.  You may 
*					wait up to 1.2 seconds between calls before losing 
*					accuracy.
*****************************************************************************/
static DWORD GetTime(void)
{
	static DWORD_VAL ret;	// Doesn't need to be declared static, but done so to reduce code size.

	if(INTCONbits.TMR0IF)
	{
		INTCONbits.TMR0IF = 0;
		wTimerHigh++;
	}
	ret.v[0] = TMR0L;
	ret.v[1] = TMR0H;
	ret.w[1] = wTimerHigh;

	return ret.Val;
}


/******************************************************************************
* Function:        static WORD CalcIPChecksum(BYTE* Data, WORD Length)
*
* PreCondition:    None
*
* Input:           Data: Pointer to data to perform the IP checksum over
*					Length: Number of bytes to include in the checksum
*
* Output:          IP style 16 bit checksum (one's complement sum of all 
*					one's complemented words of data).  
*
* Side Effects:    None
*
* Overview:        None
*
* Note:            Returns data in big endian format
*****************************************************************************/
static WORD CalcIPChecksum(BYTE* Data, WORD Length)
{
	WORD_VAL Checksum;
	static WORD_VAL i;	// Doesn't need to be declared static, but done so to reduce code size.

	Checksum.Val = 0x0000;

	while(Length--)
	{
		i.Val = *Data++;
		if(Length)
		{
			Length--;
			i.v[1] = *Data++;
		}

		Checksum.Val += i.Val;
		if(STATUSbits.C)		// WARNING: Relies on C compiler not damaging C status bit between these two instructions
			Checksum.Val++;
	}

	return ~Checksum.Val;
}	//end CalcIPChecksum


/******************************************************************************
* Function:        void MACDiscardRx(void)
*
* PreCondition:    None
*
* Input:           None
*
* Output:          None
*
* Side Effects:    None
*
* Overview:        Marks the last received packet (obtained using 
*					MACGetHeader())as being processed and frees the buffer 
*					memory associated with it
*
* Note:            None
*****************************************************************************/
static void MACDiscardRx(void)
{
	static WORD_VAL NewRXRDLocation;	// Doesn't need to be declared static, but done so to reduce code size.

	// Decrement the next packet pointer before writing it into 
	// the ERXRDPT registers.  This is a silicon errata workaround.
	// RX buffer wrapping must be taken into account if the 
	// NextPacketLocation is precisely RXSTART.
	NewRXRDLocation.Val = NextPacketLocation.Val - 1;
	if(NewRXRDLocation.Val > (WORD)RXSTOP)
	{
		NewRXRDLocation.Val = RXSTOP;
	}

	// Decrement the RX packet counter register, EPKTCNT
	ECON2bits.PKTDEC = 1;

	// Move the receive read pointer to unwrite-protect the memory used by the 
	// last packet.  The writing order is important: set the low byte first, 
	// high byte last.
	ERXRDPTL = NewRXRDLocation.v[0];
	ERXRDPTH = NewRXRDLocation.v[1];
} //end MACDiscardRx


/******************************************************************************
* Function:        WORD MACGetArray(BYTE *val, WORD len)
*
* PreCondition:    ERDPT must point to the place to read from.
*
* Input:           *val: Pointer to storage location
*					len:  Number of bytes to read from the data buffer.
*
* Output:          Byte(s) of data read from the data buffer.
*
* Side Effects:    None
*
* Overview:        Reads several sequential bytes from the data buffer 
*					and places them into local memory.  ERDPT is incremented 
*					after each byte, following the same rules as MACGet().
*
* Note:            None
*****************************************************************************/
static WORD MACGetArray(BYTE *val, WORD len)
{
	WORD w;
	static volatile BYTE i;	// Doesn't need to be declared static, but done so to reduce code size.

	w = len;

	if(val)
	{
		while(w--)
		{
			*val++ = EDATA;
		}
	}
	else
	{
		while(w--)
		{
			i = EDATA;
		}
	}

	return len;
}//end MACGetArray


/******************************************************************************
* Function:        void MACPutArray(BYTE *val, WORD len)
*
* PreCondition:    EWRPT must point to the location to begin writing.
*
* Input:           *val: Pointer to source of bytes to copy.
*					len:  Number of bytes to write to the data buffer.
*
* Output:          None
*
* Side Effects:    None
*
* Overview:        MACPutArray writes several sequential bytes to the 
*					Ethernet buffer RAM.  It performs faster than multiple MACPut()
*					calls.  EWRPT is incremented by len.
*
* Note:            None
*****************************************************************************/
static void MACPutArray(BYTE *val, WORD len)
{
	while(len--)
	{
		// Note:  Due to a PIC18F97J60 bug, you must use the MOVFF instruction to 
		// write to EDATA or else the read pointer (ERDPT) will inadvertently 
		// increment.
		PRODL = *val++;
#if defined(HI_TECH_C)
		asm("movff	_PRODL, _EDATA");
#else
		_asm movff	PRODL, EDATA _endasm
#endif
	}
}//end MACPutArray


/******************************************************************************
* Function:        static void MACFlush(void)
*
* PreCondition:    A packet has been created by calling MACPut() and 
*					MACPutHeader().
*
* Input:           None
*
* Output:          None
*
* Side Effects:    None
*
* Overview:        MACFlush causes the current TX packet to be sent out on 
*					the Ethernet medium.  The hardware MAC will take control
*					and handle CRC generation, collision retransmission and 
*					other details.
*
* Note:			After transmission completes (MACIsTxReady() returns TRUE), 
*					the packet can be modified and transmitted again by calling 
*					MACFlush() again.  Until MACPutHeader() or MACPut() is 
*					called (in the TX data area), the data in the TX buffer 
*					will not be corrupted.
*****************************************************************************/
static void MACFlush(void)
{
	// Reset the Ethernet TX logic.  This is a (suspected) errata workaround to 
	// prevent the TXRTS bit from getting stuck set indefinitely, causing the 
	// stack to lock up under certain bad conditions.
	ECON1bits.TXRST = 1;
	ECON1bits.TXRST = 0;	

	// Wait at least 1.6us after TX Reset before setting TXRTS.
	// If you don't wait long enough, the TX logic won't be finished resetting.
	{static volatile BYTE i = 8; while(i--);}	
	EIRbits.TXERIF = 0;

	// Start the transmission
	// After transmission completes (MACIsTxReady() returns TRUE), the packet 
	// can be modified and transmitted again by calling MACFlush() again.
	// Until MACPutHeader() is called, the data in the TX buffer will not be 
	// corrupted.
	ECON1bits.TXRTS = 1;
	((WORD_VAL*)&wTXWatchdog)->v[0] = TMR0L;
	((WORD_VAL*)&wTXWatchdog)->v[1] = TMR0H;
}


/******************************************************************************
* Function:        static BOOL MACIsTxReady(void)
*
* PreCondition:    None
*
* Input:           None
*
* Output:          TRUE: If no Ethernet transmission is in progress
*					FALSE: If a previous transmission was started, and it has 
*						   not completed yet.  While FALSE, the data in the 
*						   transmit buffer and the TXST/TXND pointers must not
*						   be changed.
*
* Side Effects:    None
*
* Overview:        Returns the ECON1.TXRTS bit
*
* Note:            None
*****************************************************************************/
static BOOL MACIsTxReady(void)
{
	static WORD wTimerVal;	// Doesn't need to be declared static, but done so to reduce code size.

	if(!ECON1bits.TXRTS)
		return TRUE;

	// Retry transmission if the current packet seems to be not completing
	// Wait 3ms before triggering the retry.
	((WORD_VAL*)&wTimerVal)->v[0] = TMR0L;
	((WORD_VAL*)&wTimerVal)->v[1] = TMR0H;
	if(wTimerVal - wTXWatchdog >= (3ul*SECOND/1000ul))
	{
		ECON1bits.TXRTS = 0;
		MACFlush();
	}

	return FALSE;
}


/******************************************************************************
* Function:        WritePHYReg
*
* PreCondition:    Ethernet module must be enabled (ECON1.ETHEN = 1).
*
* Input:           Address of the PHY register to write to.
*					16 bits of data to write to PHY register.
*
* Output:          None
*
* Side Effects:    None
*
* Overview:        WritePHYReg performs an MII write operation.  While in 
*					progress, it simply polls the MII BUSY bit wasting time 
*					(10.24us).
*
* Note:            None
*****************************************************************************/
void WritePHYReg(BYTE Register, WORD Data)
{
	// Write the register address
	MIREGADR = Register;

	// Write the data through the MIIM interface
	// Order is important: write low byte first, high byte last
	//
	// Due to a silicon problem, you cannot access any register with LSb address 
	// bits of 0x16 between your write to MIWRL and MIWRH or else the value in 
	// MIWRL will be corrupted.  This inline assembly prevents this by copying 
	// the value to PRODH:PRODL first, which is at fixed locations of 
	// 0xFF4:0xFF3.  These addresses have LSb address bits of 0x14 and 0x13.
	// Interrupts must be disabled to prevent arbitrary ISR code from accessing
	// memory with LSb bits of 0x16 and corrupting the MIWRL value.
	PRODL = ((WORD_VAL*)&Data)->v[0];
	PRODH = ((WORD_VAL*)&Data)->v[1];
#if defined(HI_TECH_C)
	asm("movff	_PRODL, _MIWRL");
	asm("nop");
	asm("movff	_PRODH, _MIWRH");
#else
	_asm
		movff	PRODL, MIWRL
		nop
		movff	PRODH, MIWRH
	_endasm
#endif

	// Wait at least 2 Tcy
	Nop();
	Nop();

	// Wait until the PHY register has been written
	// This operation requires 10.24us
	while(MISTATbits.BUSY);
}//end WritePHYReg

