/*********************************************************************
 *
 *   HID Device Bootloader Firmware for PIC18F97J94 Family Devices
 *
 *********************************************************************
 * FileName:        Boot18FxxJxxFamily.c
 * Dependencies:    See INCLUDES section below
 * Processor:       PIC18
 * Compiler:        C18 3.45+
 * Company:         Microchip Technology, Inc.
 *
 * Software License Agreement
 *
 * The software supplied herewith by Microchip Technology Incorporated
 * (the "Company") for its PIC(R) Microcontroller is intended and
 * supplied to you, the Company's customer, for use solely and
 * exclusively on Microchip PICmicro Microcontroller products. The
 * software is owned by the Company and/or its supplier, and is
 * protected under applicable copyright laws. All rights are reserved.
 * Any use in violation of the foregoing restrictions may subject the
 * user to criminal sanctions under applicable laws, as well as to
 * civil liability for the breach of the terms and conditions of this
 * license.
 *
 * THIS SOFTWARE IS PROVIDED IN AN "AS IS" CONDITION. NO WARRANTIES,
 * WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED
 * TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. THE COMPANY SHALL NOT,
 * IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL OR
 * CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 *
 * File version     Date        Comment
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * 1.0			    06/19/08	Original Version.  Adapted from 
 *								MCHPFSUSB v2.1 HID Bootloader
 *								for PIC18F87J50 Family devices.

 * 2.9j             06/11/13    Added sign flash command for ruggedized
 *                              software entry method recovery in event of
 *                              interrupt during erase/program/verify operation.
 *                              Implemented extended query command and bootloader
 *                              versioning system.  This release is version "1.01".
 *********************************************************************/

/** I N C L U D E S **********************************************************/
#include <p18cxxx.h>
#include "typedefs.h"
#include "string.h"
#include "usb.h"
#include "Boot18FxxJxxFamily.h"


/** C O N S T A N T S **********************************************************/
//The bootloader version, which the bootloader PC application can do extended query to get.
//Value provided is expected to be in the format of BOOTLOADER_VERSION_MAJOR.BOOTLOADER_VERSION_MINOR
//Ex: 1.01 would be BOOTLOADER_VERSION_MAJOR == 1, and BOOTLOADER_VERSION_MINOR == 1
#define BOOTLOADER_VERSION_MAJOR         1 //Legal value 0-255
#define BOOTLOADER_VERSION_MINOR         1 //Legal value 0-99.  (1 = X.01)

//Section defining the address range to erase for the erase device command, along with the valid programming range to be reported by the QUERY_DEVICE command.
#define StartPageToErase                8           //The 512 byte page starting at address 0x1000 will be erased.
#define PROGRAM_MEM_START_ADDRESS		0x1000      //Beginning of application program memory (not occupied by bootloader).  **THIS VALUE MUST BE ALIGNED WITH 64 BYTE BLOCK BOUNDRY** Also, in order to work correctly, make sure the StartPageToErase is set to erase this section.
#define CONFIG_WORDS_SECTION_LENGTH		0x10        //16 bytes worth of Configuration words on the PIC18F97J94 Family devices
#define WRITE_BLOCK_SIZE                64
#define ERASE_PAGE_SIZE                 512
#define ERASE_PAGE_ADDRESS_MASK         0xFFFE00

#if defined(__18F97J94)||defined(__18F87J94)||defined(__18F67J94)   //128kB flash devices
    // 512 byte flash erase block size
	#define MaxPageToEraseNoConfigs		254		 //Last page of flash on the device, which does not contain the flash configuration words.
	#define MaxPageToEraseWithConfigs	255		 //Page 255 contains the flash configurations words on this device.
	#define ProgramMemStopNoConfigs		0x01FE00    //**MUST BE WORD ALIGNED (EVEN) ADDRESS.  
                                                    // This address does not get updated, 
                                                    // but the one just below it does: 
                                                    // IE: If ProgramMemStopNoConfig = 0x200, 
                                                    //      0x1FF is the last programmed 
                                                    //      address (0x200 not programmed)**	
	#define ProgramMemStopWithConfigs	0x01FFF0 //**MUST BE WORD ALIGNED (EVEN) ADDRESS.  This address does not get updated, but the one just below it does: IE: If AddressToStopPopulating = 0x200, 0x1FF is the last programmed address (0x200 not programmed)**	
	#define CONFIG_WORDS_START_ADDRESS	0x01FFF0 //0xXXXF0 is CONFIG1L on this device
#elif defined(__18F96J99)||defined(__18F86J99)||defined(__18F66J99)     //96kB flash devices
	#define MaxPageToEraseNoConfigs		94		 //Last page of flash on the device, which does not contain the flash configuration words.
	#define MaxPageToEraseWithConfigs	95		 //Page 127 contains the flash configurations words on this device.
	#define ProgramMemStopNoConfigs		0x017C00 //**MUST BE WORD ALIGNED (EVEN) ADDRESS.  This address does not get updated, but the one just below it does: IE: If AddressToStopPopulating = 0x200, 0x1FF is the last programmed address (0x200 not programmed)**	
	#define ProgramMemStopWithConfigs	0x017FF8 //**MUST BE WORD ALIGNED (EVEN) ADDRESS.  This address does not get updated, but the one just below it does: IE: If AddressToStopPopulating = 0x200, 0x1FF is the last programmed address (0x200 not programmed)**	
	#define CONFIG_WORDS_START_ADDRESS	0x017FF8 //0xXXXF8 is CONFIG1L on this device
#elif defined(__18F96J94)||defined(__18F86J94)||defined(__18F66J94)    //64kB flash devices
	#define MaxPageToEraseNoConfigs		62		 //Last page of flash on the device, which does not contain the flash configuration words.
	#define MaxPageToEraseWithConfigs	63		 //Page 127 contains the flash configurations words on this device.
	#define ProgramMemStopNoConfigs		0x00FC00 //**MUST BE WORD ALIGNED (EVEN) ADDRESS.  This address does not get updated, but the one just below it does: IE: If AddressToStopPopulating = 0x200, 0x1FF is the last programmed address (0x200 not programmed)**	
	#define ProgramMemStopWithConfigs	0x00FFF8 //**MUST BE WORD ALIGNED (EVEN) ADDRESS.  This address does not get updated, but the one just below it does: IE: If AddressToStopPopulating = 0x200, 0x1FF is the last programmed address (0x200 not programmed)**	
	#define CONFIG_WORDS_START_ADDRESS	0x00FFF8 //0xXXXF8 is CONFIG1L on this device
#elif defined(__18F95J94)||defined(__18F85J94)||defined(__18F65J94)     //32kB flash devices
	#define MaxPageToEraseNoConfigs		30		 //Last page of flash on the device, which does not contain the flash configuration words.
	#define MaxPageToEraseWithConfigs	31		 //Page 127 contains the flash configurations words on this device.
	#define ProgramMemStopNoConfigs		0x007C00 //**MUST BE WORD ALIGNED (EVEN) ADDRESS.  This address does not get updated, but the one just below it does: IE: If AddressToStopPopulating = 0x200, 0x1FF is the last programmed address (0x200 not programmed)**	
	#define ProgramMemStopWithConfigs	0x007FF8 //**MUST BE WORD ALIGNED (EVEN) ADDRESS.  This address does not get updated, but the one just below it does: IE: If AddressToStopPopulating = 0x200, 0x1FF is the last programmed address (0x200 not programmed)**	
	#define CONFIG_WORDS_START_ADDRESS	0x007FF8 //0xXXXF8 is CONFIG1L on this device
#endif

//Switch State Variable Choices
#define	QUERY_DEVICE				0x02	//Command that the host uses to learn about the device (what regions can be programmed, and what type of memory is the region)
#define	UNLOCK_CONFIG				0x03	//Note, this command is used for both locking and unlocking the config bits (see the "//Unlock Configs Command Definitions" below)
#define ERASE_DEVICE				0x04	//Host sends this command to start an erase operation.  Firmware controls which pages should be erased.
#define PROGRAM_DEVICE				0x05	//If host is going to send a full RequestDataBlockSize to be programmed, it uses this command.
#define	PROGRAM_COMPLETE			0x06	//If host send less than a RequestDataBlockSize to be programmed, or if it wished to program whatever was left in the buffer, it uses this command.
#define GET_DATA					0x07	//The host sends this command in order to read out memory from the device.  Used during verify (and read/export hex operations)
#define	RESET_DEVICE				0x08	//Resets the microcontroller, so it can update the config bits (if they were programmed, and so as to leave the bootloader (and potentially go back into the main application)
#define SIGN_FLASH					0x09	//The host PC application should send this command after the verify operation has completed successfully.  If checksums are used instead of a true verify (due to ALLOW_GET_DATA_COMMAND being commented), then the host PC application should send SIGN_FLASH command after is has verified the checksums are as exected. The firmware will then program the SIGNATURE_WORD into flash at the SIGNATURE_ADDRESS.
#define QUERY_EXTENDED_INFO         0x0C    //Used by host PC app to get additional info about the device, beyond the basic NVM layout provided by the query device command

//Unlock Configs Command Definitions
#define UNLOCKCONFIG				0x00	//Sub-command for the ERASE_DEVICE command
#define LOCKCONFIG					0x01	//Sub-command for the ERASE_DEVICE command

//Query Device Response "Types" 
#define	MEMORY_REGION_PROGRAM_MEM	0x01	//When the host sends a QUERY_DEVICE command, need to respond by populating a list of valid memory regions that exist in the device (and should be programmed)
#define MEMORY_REGION_EEDATA		0x02
#define MEMORY_REGION_CONFIG		0x03
#define MEMORY_REGION_USERID		0x04
#define	MEMORY_REGION_END		    0xFF	//Sort of serves as a "null terminator" like number, which denotes the end of the memory region list has been reached.
#define BOOTLOADER_V1_01_OR_NEWER_FLAG   0xA5   //Tacked on in the VersionFlag byte, to indicate when using newer version of bootloader with extended query info available


//BootState Variable States
#define	IDLE						0x00
#define NOT_IDLE    				0x01

//OtherConstants
#define INVALID_ADDRESS				0xFFFFFFFF
#define CORRECT_UNLOCK_KEY          0xB5

//Application and Microcontroller constants
#define BYTES_PER_ADDRESS_PIC18		0x01		//One byte per address.  PIC24 uses 2 bytes for each address in the hex file.
#define	USB_PACKET_SIZE				0x40
#define WORDSIZE					0x02	//PIC18 uses 2 byte words, PIC24 uses 3 byte words.
#define REQUEST_DATA_BLOCK_SIZE		0x3A	//Number of data bytes in a standard request to the PC.  Must be an even number from 2-58 (0x02-0x3A).  Larger numbers make better use of USB bandwidth and 
											//yeild shorter program/verify times, but require more micrcontroller RAM for buffer space.



/** USB Packet Request/Response Formatting Structure **********************************************************/
typedef union 
{
		unsigned char Contents[USB_PACKET_SIZE];

		//General command (with data in it) packet structure used by PROGRAM_DEVICE and GET_DATA commands 		
		struct{
			unsigned char Command;
			unsigned long Address;
			unsigned char Size;
//			unsigned char PadBytes[58-REQUEST_DATA_BLOCK_SIZE];	//Uncomment this if using a smaller than 0x3A RequestDataBlockSize.  Compiler doesn't like 0 byte array when using 58 byte data block size.
			unsigned char Data[REQUEST_DATA_BLOCK_SIZE];
		};
		
		//This struct used for responding to QUERY_DEVICE command (on a device with four programmable sections)
		struct{
			unsigned char Command;
			unsigned char PacketDataFieldSize;
			unsigned char BytesPerAddress;
			unsigned char Type1;
			unsigned long Address1;
			unsigned long Length1;
			unsigned char Type2;
			unsigned long Address2;
			unsigned long Length2;
			unsigned char Type3;
			unsigned long Address3;
			unsigned long Length3;
			unsigned char Type4;
			unsigned long Address4;
			unsigned long Length4;						
			unsigned char Type5;
			unsigned long Address5;
			unsigned long Length5;
			unsigned char Type6;
			unsigned long Address6;
			unsigned long Length6;	
			unsigned char VersionFlag;	    //Used by host software to identify if device is new enough to support QUERY_EXTENDED_INFO command	
			unsigned char ExtraPadBytes[7];
		};	
		
		struct{						//For UNLOCK_CONFIG command
			unsigned char Command;
			unsigned char LockValue;
		};
		
		//Structure for the QUERY_EXTENDED_INFO command (and response)
		struct{
    		unsigned char Command;
    		unsigned int BootloaderVersion;
    		unsigned int ApplicationVersion;
    		unsigned long SignatureAddress;
    		unsigned int SignatureValue;
    		unsigned long ErasePageSize;
    		unsigned char Config1LMask;
    		unsigned char Config1HMask;
    		unsigned char Config2LMask;
    		unsigned char Config2HMask;
    		unsigned char Config3LMask;
    		unsigned char Config3HMask;
    		unsigned char Config4LMask;
    		unsigned char Config4HMask;
    		unsigned char Config5LMask;
    		unsigned char Config5HMask;
    		unsigned char Config6LMask;
    		unsigned char Config6HMask;
    		unsigned char Config7LMask;
    		unsigned char Config7HMask;
    		unsigned char Config8LMask;
    		unsigned char Config8HMask;
        };  		
} PacketToFromPC;		
	

/** V A R I A B L E S ********************************************************/
#pragma udata 
unsigned char MaxPageToErase;
unsigned short long ProgramMemStopAddress;
PacketToFromPC PacketFromPC;
#pragma udata SomeSectionName3
PacketToFromPC PacketToPC;
#pragma udata WriteBufferSection
unsigned char ProgrammingBuffer[ERASE_PAGE_SIZE];
#pragma udata OtherVariablesSection
unsigned short long ProgramMemStopAddress;
unsigned char BootState;
signed char BufferedDataIndex;
unsigned short long ProgrammedPointer;
unsigned char ConfigsLockValue;


/** P R I V A T E  P R O T O T Y P E S ***************************************/
void WriteFlashSubBlock(void);
void UnlockAndActivate(unsigned char UnlockKey);
void ResetDeviceCleanly(void);
void TableReadPostIncrement(void);
void SignFlash(void);
void EraseFlashPage(unsigned int PageNumToErase);
void LowVoltageCheck(void);


/** D E C L A R A T I O N S **************************************************/
#pragma code
void UserInit(void)
{
	//Initialize bootloader state variables
	MaxPageToErase = MaxPageToEraseNoConfigs;		//Assume we will not allow erase/programming of config words (unless host sends override command)
	ProgramMemStopAddress = ProgramMemStopNoConfigs;
	BootState = IDLE;
	ProgrammedPointer = INVALID_ADDRESS;	
	BufferedDataIndex = 0;
	ConfigsLockValue = TRUE;
}//end UserInit


/******************************************************************************
 * Function:        void ProcessIO(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        This function receives/sends USB packets to/from the USB 
 *                  host.  It also processes any received OUT packets and
 *                  is reponsible for generating USB IN packet data.
 *
 * Note:            None
 *****************************************************************************/
void ProcessIO(void)
{
	static unsigned char i;
	static unsigned int j;

    //Check if we are busy processing a command yet or not.
	if(BootState == IDLE)
	{
    	//We are currently in the IDLE state waiting for a command from the
    	//PC software on the USB host.
		if(!mHIDRxIsBusy())	//Did we receive a command?
		{
    		//We received a new command from the host.  Copy the OUT packet from 
    		//the host into a local buffer for processing.
			HIDRxReport((char *)&PacketFromPC, USB_PACKET_SIZE);     //Also re-arms the OUT endpoint to be able to receive the next packet
			BootState = NOT_IDLE;   //Set flag letting state machine know it has a command that needs processing.
			
			//Pre-initialize a response packet buffer (only used for some commands)
			for(i = 0; i < USB_PACKET_SIZE; i++)		//Prepare the next packet we will send to the host, by initializing the entire packet to 0x00.
				PacketToPC.Contents[i] = 0;				//This saves code space, since we don't have to do it independently in the QUERY_DEVICE and GET_DATA cases.
		}
	}
	else //(BootState must be NOT_IDLE)
	{	
    	//Check the latest command we received from the PC app, to determine what
    	//we should be doing.
		switch(PacketFromPC.Command)
		{
			case QUERY_DEVICE:
				//Prepare a response packet, which lets the PC software know about the memory ranges of this device.
				PacketToPC.Command = QUERY_DEVICE;
				PacketToPC.PacketDataFieldSize = REQUEST_DATA_BLOCK_SIZE;
				PacketToPC.BytesPerAddress = BYTES_PER_ADDRESS_PIC18;
				PacketToPC.Type1 = MEMORY_REGION_PROGRAM_MEM;
				PacketToPC.Address1 = (unsigned long)PROGRAM_MEM_START_ADDRESS;
				PacketToPC.Length1 = (unsigned long)(ProgramMemStopAddress - PROGRAM_MEM_START_ADDRESS);	//Size of program memory area
				PacketToPC.Type2 = MEMORY_REGION_CONFIG;
				PacketToPC.Address2 = (unsigned long)CONFIG_WORDS_START_ADDRESS;
				PacketToPC.Length2 = (unsigned long)CONFIG_WORDS_SECTION_LENGTH;
				PacketToPC.Type3 = MEMORY_REGION_END;		
				PacketToPC.VersionFlag = BOOTLOADER_V1_01_OR_NEWER_FLAG;
				//Init pad bytes to 0x00...  Already done after we received the QUERY_DEVICE command (just after calling HIDRxReport()).
	
	            //Now send the packet to the USB host software, assuming the USB endpoint is available/ready to accept new data.
				if(!mHIDTxIsBusy())
				{
					HIDTxReport((char *)&PacketToPC, USB_PACKET_SIZE);
					BootState = IDLE;
				}
				break;
			case UNLOCK_CONFIG:
				ConfigsLockValue = TRUE;
				if(PacketFromPC.LockValue == UNLOCKCONFIG)
				{
					ConfigsLockValue = FALSE;
					MaxPageToErase = MaxPageToEraseWithConfigs;		//Assume we will not allow erase/programming of config words (unless host sends override command)
					ProgramMemStopAddress = ProgramMemStopWithConfigs;
				}
				else	//LockValue must be == LOCKCONFIG
				{
					MaxPageToErase = MaxPageToEraseNoConfigs;		
					ProgramMemStopAddress = ProgramMemStopNoConfigs;
				}
				BootState = IDLE;
				break;
			case ERASE_DEVICE:
				//Erase all pages of the program flash memory, that are in the
				//application space (although could be one less, if config word
				//programming is not allowed, since we don't want to erase the
				//config bits in this case, which are on the last flash erase page.)
				for(j = StartPageToErase; j < (MaxPageToErase + 1); j++)
				{
					ClearWatchdog();
					EraseFlashPage(j);
					USBDriverService(); 	//Call USBDriverService() periodically to prevent falling off the bus if any SETUP packets should happen to arrive.
				}
				BootState = IDLE;				
				break;
			case PROGRAM_DEVICE:
				if(ProgrammedPointer == (unsigned short long)INVALID_ADDRESS)
					ProgrammedPointer = PacketFromPC.Address;
				
				if(ProgrammedPointer == (unsigned short long)PacketFromPC.Address)
				{
					for(i = 0; i < PacketFromPC.Size; i++)
					{
						ProgrammingBuffer[BufferedDataIndex] = PacketFromPC.Data[i+(REQUEST_DATA_BLOCK_SIZE-PacketFromPC.Size)];	//Data field is right justified.  Need to put it in the buffer left justified.
						BufferedDataIndex++;
						ProgrammedPointer++;
						if(BufferedDataIndex == REQUEST_DATA_BLOCK_SIZE)
						{
							WriteFlashSubBlock();
						}
					}
				}
				//else host sent us a non-contiguous packet address...  to make 
				//this firmware simpler, host should not do this without sending 
				//a PROGRAM_COMPLETE command in between program sections.
				BootState = IDLE;
				break;
			case PROGRAM_COMPLETE:
				WriteFlashSubBlock();
				ProgrammedPointer = INVALID_ADDRESS;		//Reinitialize pointer to an invalid range, so we know the next PROGRAM_DEVICE will be the start address of a contiguous section.
				BootState = IDLE;
				break;
			case GET_DATA:
				//Init pad bytes to 0x00...  Already done after we received the QUERY_DEVICE command (just after calling HIDRxReport()).
				PacketToPC.Command = GET_DATA;
				PacketToPC.Address = PacketFromPC.Address;
				PacketToPC.Size = PacketFromPC.Size;

				TBLPTR = (unsigned short long)PacketFromPC.Address;
				for(i = 0; i < PacketFromPC.Size; i++)
				{
					TableReadPostIncrement();
                    PacketToPC.Data[i+((USB_PACKET_SIZE - 6) - PacketFromPC.Size)] = TABLAT;
				}

				//Assuming the USB IN (to host) buffer is available/ready, copy the
				//data over so it can get sent to the USB host software.
				if(!mHIDTxIsBusy())
				{
					HIDTxReport((char *)&PacketToPC, USB_PACKET_SIZE);
					BootState = IDLE;
				}
				break;
			case SIGN_FLASH:
			    SignFlash();
			    BootState = IDLE;
			    break;
			case QUERY_EXTENDED_INFO:
			    //Prepare a response packet with the QUERY_EXTENDED_INFO response info in it.
			    //This command is only supported in bootloader firmware verison 1.01 or later.
			    //Make sure the regular QUERY_DEVIER reponse packet value "PacketToPC.Type6" is = BOOTLOADER_V1_01_OR_NEWER_FLAG;
			    //to let the host PC software know that the QUERY_EXTENDED_INFO command is implemented
			    //in this firmware and is available for requesting by the host software.
			    PacketToPC.Command = QUERY_EXTENDED_INFO;   //Echo the command byte
			    PacketToPC.BootloaderVersion = ((unsigned int)BOOTLOADER_VERSION_MAJOR << 8)| BOOTLOADER_VERSION_MINOR;
			    PacketToPC.ApplicationVersion = *(rom unsigned int*)APP_VERSION_ADDRESS;
			    PacketToPC.SignatureAddress = APP_SIGNATURE_ADDRESS;
			    PacketToPC.SignatureValue = APP_SIGNATURE_VALUE;
			    PacketToPC.ErasePageSize = ERASE_PAGE_SIZE;
			    PacketToPC.Config1LMask = 0xFF;
			    PacketToPC.Config1HMask = 0xFF;
			    PacketToPC.Config2LMask = 0xFF;
			    PacketToPC.Config2HMask = 0xFF;
			    PacketToPC.Config3LMask = 0xFF;
			    PacketToPC.Config3HMask = 0xFF;
			    PacketToPC.Config4LMask = 0xFF;
			    PacketToPC.Config4HMask = 0xFF;
			    PacketToPC.Config5LMask = 0x00;
			    PacketToPC.Config5HMask = 0x00;
			    PacketToPC.Config6LMask = 0x00;
			    PacketToPC.Config6HMask = 0x00;
			    PacketToPC.Config7LMask = 0x00;
			    PacketToPC.Config7HMask = 0x00;
			    PacketToPC.Config8LMask = 0x00;
			    PacketToPC.Config8HMask = 0x00;
			    
			    //Now actually command USB to send the packet to the host    			    
			    if(!mHIDTxIsBusy())
			    {
					HIDTxReport((char *)&PacketToPC, USB_PACKET_SIZE);
    			    BootState = IDLE;   //Packet will be sent, go back to idle state ready for next command from host
		        }	    
			    break;
			case RESET_DEVICE:
			    ResetDeviceCleanly();
			    //break;    //no need, commented to save space
			default:
			    //Should never hit the default
			    BootState = IDLE;
		}//End switch
	}//End if/else

}//End ProcessIO()



//Should be called once, only after the regular erase/program/verify sequence 
//has completed successfully.  This function will program the magic
//APP_SIGNATURE_VALUE into the magic APP_SIGNATURE_ADDRESS in the application
//flash memory space.  This is used on the next bootup to know that the the
//flash memory image of the application is intact, and can be executed.
//This is useful for recovery purposes, in the event that an unexpected
//failure occurs during the erase/program sequence (ex: power loss or user
//unplugging the USB cable).
void SignFlash(void)
{
    static unsigned int i;
    
    //First read in the erase page contents of the page with the signature WORD
    //in it, and temporarily store it in a RAM buffer.
    TBLPTR = (APP_SIGNATURE_ADDRESS & ERASE_PAGE_ADDRESS_MASK);
    for(i = 0; i < ERASE_PAGE_SIZE; i++)
    {
        _asm tblrdpostinc _endasm  
        ProgrammingBuffer[i] = TABLAT;        
    }    
    
    //Now change the signature WORD value at the correct address in the RAM buffer
    ProgrammingBuffer[(APP_SIGNATURE_ADDRESS & ~ERASE_PAGE_ADDRESS_MASK)] = (unsigned char)APP_SIGNATURE_VALUE;
    ProgrammingBuffer[(APP_SIGNATURE_ADDRESS & ~ERASE_PAGE_ADDRESS_MASK) + 1] = (unsigned char)(APP_SIGNATURE_VALUE >> 8);
    
    //Now erase the flash memory block with the signature WORD in it
    EraseFlashPage(APP_SIGNATURE_ADDRESS >> 10);
    
    //Now re-program the values from the RAM buffer into the flash memory.  Use
    //reverse order, so we program the larger addresses first.  This way, the 
    //write page with the flash signature word is the last page that gets 
    //programmed (assuming the flash signature resides on the lowest address
    //write page, which is recommended, so that it becomes the first page
    //erased, and the last page programmed).
    TBLPTR = (APP_SIGNATURE_ADDRESS & ERASE_PAGE_ADDRESS_MASK) + ERASE_PAGE_SIZE - 1;   //Point to last byte on the erase page
    i = ERASE_PAGE_SIZE - 1;
    while(1)
    {
        TABLAT = ProgrammingBuffer[i];
        _asm tblwt _endasm

        //Check if we are at a program write block size boundary
        if((i % WRITE_BLOCK_SIZE) == 0)
        {
            //The write latches are full, time to program the block.
            ClearWatchdog();
            EECON1 = 0x04;  //Write to flash (block write) on next WR = 1 operation
            UnlockAndActivate(CORRECT_UNLOCK_KEY);
        }    
        _asm tblrdpostdec _endasm   //Move TBLPTR back to next location
        
        //Check if we are done writing all blocks
        if(i == 0)
        {
            break;  //Exit while loop
        }    
        i--;
    }    
}    


//Before resetting the microcontroller, we should shut down the USB module 
//gracefully, to make sure the host correctly recognizes that we detached
//from the bus.  Some USB hosts malfunction/fail to re-enumerate the device
//correctly if the USB device does not stay detached for a minimum amount of
//time before re-attaching to the USB bus.  For reliable operation, the USB
//device should stay detached for as long as a human would require to unplug and
//reattach a USB device (ex: 100ms+), to ensure the USB host software has a 
//chance to process the detach event and configure itself for a state ready for 
//a new attachment event.
void ResetDeviceCleanly(void)
{
    DisableUSBandExecuteLongDelay();
	Reset();    
	Nop();
	Nop();
}    


//Disable the USB module and then wait a long time (long enough for the host
//to correctly detect the USB detach event, process the plug and play event,
//and get itself back into a state ready for a new USB attach event.
void DisableUSBandExecuteLongDelay(void)
{
    uint_delay_counter = 65535;
    while(uint_delay_counter--)
    {
        //Disable USB module (this doens't need to be done in the loop, but we 
        //add it to the loop to deliberately make the loop execute slower)
        UCONbits.SUSPND = 0;    //Make sure not in suspend mode
        UCON = 0x00;            
        ClearWatchdog();
    }   
}    


//Routine used to write data to the flash memory from the ProgrammingBuffer[].
//Note: Function uses WORD writes.  Always make sure the quantity of data in the
//buffer is an exact multiple of the program write word size (2 bytes).
void WriteFlashSubBlock(void)		//Use word writes to write code chunks less than a full 64 byte block size.
{
	static unsigned char i;
	
	i = 0;
	while(BufferedDataIndex > 0)		//While data is still in the buffer.
	{
		TBLPTR = (ProgrammedPointer - BufferedDataIndex);
		//Below section will need to be modified if the WORDSIZE of your processor is not 2 bytes.
		TABLAT = ProgrammingBuffer[i];
		_asm
		tblwtpostinc
		_endasm
		i++;		
		TABLAT = ProgrammingBuffer[i];
		_asm
		tblwt					//Do not increment TBLPTR on the second write.  See datasheet.
		_endasm
		i++;
		
		EECON1 = 0b00100100;	                //Word programming mode
		UnlockAndActivate(CORRECT_UNLOCK_KEY);  //Now make the operation take place
	
		BufferedDataIndex = BufferedDataIndex - WORDSIZE;	//Used up one word from the buffer.
	}
}






//It is preferrable to only place this sequence in only one place in the flash memory.
//This reduces the probabilty of the code getting executed inadvertently by
//errant code.  It is also recommended to enable BOR (in hardware) and/or add
//software checks to avoid microcontroller "overclocking".  Always make sure
//to obey the voltage versus frequency graph in the datasheet, even during
//momentary events (such as the power up and power down ramp of the microcontroller).
void UnlockAndActivate(unsigned char UnlockKey)
{
	INTCONbits.GIE = 0;		//Make certain interrupts disabled for unlock process.

    //Make sure voltage is sufficient for safe self erase/write operations
    LowVoltageCheck();

    //Check to make sure the caller really was trying to call this function.
    //If they were, they should always pass us the CORRECT_UNLOCK_KEY.
    if(UnlockKey != CORRECT_UNLOCK_KEY)
    {
        //Warning!  Errant code execution detected.  Somehow this 
        //UnlockAndActivate() function got called by someone that wasn't trying
        //to actually perform an NVM erase or write.  This could happen due to
        //microcontroller overclocking (or undervolting for an otherwise allowed
        //CPU frequency), or due to buggy code (ex: incorrect use of function 
        //pointers, etc.).  In either case, we should execute some fail safe 
        //code here to prevent corruption of the NVM contents.
        OSCCON = 0x05;  //Switch to LPRC at low frequency
        while(1)
        {
            Sleep();
        }    
        Reset();
    }    
    
	_asm
	//Now unlock sequence to set WR (make sure interrupts are disabled before executing this)
	MOVLW 0x55
	MOVWF EECON2, 0
	MOVLW 0xAA
	MOVWF EECON2, 0
	BSF EECON1, 1, 0		//Performs write
	_endasm	
	while(EECON1bits.WR);	//Wait until complete (relevant when programming EEPROM, not important when programming flash since processor stalls during flash program)	
	EECON1bits.WREN = 0;  	//Good practice now to clear the WREN bit, as further protection against any accidental activation of self write/erase operations.
}	


void EraseFlashPage(unsigned int PageNumToErase)
{
    //Error check to make sure we aren't erasing part of the bootloader itself
    if(PageNumToErase < 8)
        PageNumToErase = 8; //Erase the page with the flash signature to ensure staying in bootloader mode
 
	TBLPTR = ((unsigned short long)PageNumToErase << 9); 

	EECON1 = 0b00010100;
    UnlockAndActivate(CORRECT_UNLOCK_KEY);
}


//Note: The ClrWdt() and "_asm tblrdpostinc _endasm" are inline assembly language
//instructions.  The ClearWatchdog() and  TableReadPostIncrement() functions are 
//theoretically extraneous, since the operations being accomplished could be
//done without calling them as separate functions.  However, when using inline
//assembly language, the C compiler normally doesn't know what the code will
//actually do (ex: will it modify STATUS reg, WREG, BSR contents??).  As a 
//result, it is potentially dangerous for the C compiler to make assumptions,
//that might turn out not to be correct.  Therefore, the C18 compiler disables
//the compiler optimizations for a function, when one or more inline asm 
//instructions are located within the C function.  Therefore, to promote best
//code size optimizations from the C compiler, it is best to locate inline
//assembly sequences in their own separate C functions, that do not contain much
//other code (which could otherwise be optimized by the C compiler).  This often
//results in the smallest code size, and is the reason it is being done here.
void ClearWatchdog(void)
{
    ClrWdt();
}    
void TableReadPostIncrement(void)
{
	_asm tblrdpostinc _endasm    
}    

/** EOF Boot18FxxJxxFamily.c *********************************************************/
