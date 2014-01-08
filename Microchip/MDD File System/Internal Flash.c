/******************************************************************************
 
                Microchip Memory Disk Drive File System
 
 *****************************************************************************
  FileName:        Internal Flash.c
  Dependencies:    See includes section.
  Processor:       PIC18/PIC24/dsPIC33/PIC32 microcontrollers
  Compiler:        MPLAB(R) C18/C30/C32 compilers are supported by this code
  Company:         Microchip Technology, Inc.
 
  Software License Agreement
 
  The software supplied herewith by Microchip Technology Incorporated
  (the “Company”) for its PICmicro® Microcontroller is intended and
  supplied to you, the Company’s customer, for use solely and
  exclusively on Microchip PICmicro Microcontroller products. The
  software is owned by the Company and/or its supplier, and is
  protected under applicable copyright laws. All rights are reserved.
  Any use in violation of the foregoing restrictions may subject the
  user to criminal sanctions under applicable laws, as well as to
  civil liability for the breach of the terms and conditions of this
  license.
 
  THIS SOFTWARE IS PROVIDED IN AN “AS IS” CONDITION. NO WARRANTIES,
  WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED
  TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
  PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. THE COMPANY SHALL NOT,
  IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL OR
  CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 
*****************************************************************************/
//DOM-IGNORE-BEGIN
/********************************************************************
 Change History:
  Rev    Description
  -----  -----------
  1.2.5  Fixed bug where sector address was calculated incorrectly
  1.2.7  Re-implemented sector read/write functions for PIC24
         devices.  This removes the 32kB PSV size restriction.
         Also added some additional error checking.
  1.3.0  Expanded flash memory pointer size on PIC18 to 24-bits.  This
         allows the MSD volume to exceed 64kB on large flash memory devices.
  1.3.6   Modified "FSConfig.h" to "FSconfig.h" in '#include' directive.
  1.3.8   Modified "MDD_IntFlash_SectorRead" to write the correct word data
          in 'buffer' pointer
  1.4.4   Variable "file_buffer" attributed as (far,aligned).
********************************************************************/
//DOM-IGNORE-END

#include "Compiler.h"
#include "MDD File System/FSIO.h"
#include "MDD File System/FSDefs.h"
#include "string.h"
#include "MDD File System/Internal Flash.h"
#include "HardwareProfile.h"
#include "FSconfig.h"

/*************************************************************************/
/*  Note:  This file is included as a template of a C file for           */
/*         a new physical layer. It is designed to go with               */
/*         "TEMPLATEFILE.h"                                              */
/*************************************************************************/

/******************************************************************************
 * Global Variables
 *****************************************************************************/

#ifdef USE_PIC18
	#pragma udata
	#pragma code
#endif

static MEDIA_INFORMATION mediaInformation;

/******************************************************************************
 * Prototypes
 *****************************************************************************/
void EraseBlock(ROM BYTE* dest);
void WriteRow(void);
void WriteByte(unsigned char);
BYTE DISKmount( DISK *dsk);
BYTE LoadMBR(DISK *dsk);
BYTE LoadBootSector(DISK *dsk);
extern void Delayms(BYTE milliseconds);
MEDIA_INFORMATION * MediaInitialize(void);
void UnlockAndActivate(BYTE);

//Arbitray, but "uncommon" value.  Used by UnlockAndActivateWR() to enhance robustness.
#define NVM_UNLOCK_KEY  (BYTE)0xB5   

/******************************************************************************
 * Function:        BYTE MediaDetect(void)
 *
 * PreCondition:    InitIO() function has been executed.
 *
 * Input:           void
 *
 * Output:          TRUE   - Card detected
 *                  FALSE   - No card detected
 *
 * Side Effects:    None
 *
 * Overview:        None
 *
 * Note:            None
 *****************************************************************************/
BYTE MDD_IntFlash_MediaDetect()
{
	return TRUE;
}//end MediaDetect

/******************************************************************************
 * Function:        WORD ReadSectorSize(void)
 *
 * PreCondition:    MediaInitialize() is complete
 *
 * Input:           void
 *
 * Output:          WORD - size of the sectors for this physical media.
 *
 * Side Effects:    None
 *
 * Overview:        None
 *
 * Note:            None
 *****************************************************************************/
WORD MDD_IntFlash_ReadSectorSize(void)
{
    return MEDIA_SECTOR_SIZE;
}

/******************************************************************************
 * Function:        DWORD ReadCapacity(void)
 *
 * PreCondition:    MediaInitialize() is complete
 *
 * Input:           void
 *
 * Output:          DWORD - size of the "disk" - 1 (in terms of sector count).  
 *                  Ex: In other words, this function returns the last valid 
 *                  LBA address that may be read/written to.
 *
 * Side Effects:    None
 *
 * Overview:        None
 *
 * Note:            None
 *****************************************************************************/
DWORD MDD_IntFlash_ReadCapacity(void)
{
    //The SCSI READ_CAPACITY command wants to know the last valid LBA address 
    //that the host is allowed to read or write to.  Since LBA addresses start
    //at and include 0, a return value of 0 from this function would mean the 
    //host is allowed to read and write the LBA == 0x00000000, which would be 
    //1 sector worth of capacity.
    //Therefore, the last valid LBA that the host may access is 
    //MDD_INTERNAL_FLASH_TOTAL_DISK_SIZE - 1.
        
    return (MDD_INTERNAL_FLASH_TOTAL_DISK_SIZE - 1); 
}

/******************************************************************************
 * Function:        BYTE InitIO(void)
 *
 * PreCondition:    None
 *
 * Input:           void
 *
 * Output:          TRUE   - Card initialized
 *                  FALSE   - Card not initialized
 *
 * Side Effects:    None
 *
 * Overview:        None
 *
 * Note:            None
 *****************************************************************************/
BYTE MDD_IntFlash_InitIO (void)
{
    return  TRUE;
}

/******************************************************************************
 * Function:        BYTE MediaInitialize(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          Returns a pointer to a MEDIA_INFORMATION structure
 *
 * Overview:        MediaInitialize initializes the media card and supporting variables.
 *
 * Note:            None
 *****************************************************************************/
MEDIA_INFORMATION * MDD_IntFlash_MediaInitialize(void)
{
    mediaInformation.validityFlags.bits.sectorSize = TRUE;
    mediaInformation.sectorSize = MEDIA_SECTOR_SIZE;
    
	mediaInformation.errorCode = MEDIA_NO_ERROR;
	return &mediaInformation;
}//end MediaInitialize


/******************************************************************************
 * Function:        BYTE SectorRead(DWORD sector_addr, BYTE *buffer)
 *
 * PreCondition:    None
 *
 * Input:           sector_addr - Sector address, each sector contains 512-byte
 *                  buffer      - Buffer where data will be stored, see
 *                                'ram_acs.h' for 'block' definition.
 *                                'Block' is dependent on whether internal or
 *                                external memory is used
 *
 * Output:          Returns TRUE if read successful, false otherwise
 *
 * Side Effects:    None
 *
 * Overview:        SectorRead reads 512 bytes of data from the card starting
 *                  at the sector address specified by sector_addr and stores
 *                  them in the location pointed to by 'buffer'.
 *
 * Note:            The device expects the address field in the command packet
 *                  to be byte address. Therefore the sector_addr must first
 *                  be converted to byte address. This is accomplished by
 *                  shifting the address left 9 times.
 *****************************************************************************/
//The flash memory is organized differently on the different microcontroller
//families.  Therefore, multiple versions of this function are implemented.
#if defined(__C30__)    //PIC24 or dsPIC33 device (WORD organized flash memory)
BYTE MDD_IntFlash_SectorRead(DWORD sector_addr, BYTE* buffer)
{
    WORD i;
    DWORD flashAddress;
    BYTE TBLPAGSave;
    WORD_VAL temp;
    
    //Error check.  Make sure the host is trying to read from a legitimate
    //address, which corresponds to the MSD volume (and not some other program
    //memory region beyond the end of the MSD volume).
    if(sector_addr >= MDD_INTERNAL_FLASH_TOTAL_DISK_SIZE)
    {
        return FALSE;
    }    
    
    //Save TBLPAG register
    TBLPAGSave = TBLPAG;
   
    //Compute the 24 bit starting address.  Note: this is a word address, but we
    //only store data in and read from the lower word (even LSB).
    //Starting address will always be even, since MasterBootRecord[] uses aligned attribute in declaration.
    flashAddress = (DWORD)FILES_ADDRESS + (DWORD)(sector_addr*(WORD)MEDIA_SECTOR_SIZE);  
    
    //Read a sector worth of data from the flash, and copy to the user specified "buffer".
    for(i = 0; i < (MEDIA_SECTOR_SIZE / 2u); i++)
    {
        TBLPAG = (BYTE)(flashAddress >> 16);   //Load TBLPAG pointer (upper 8 bits of total address.  A sector could get split at 
                                        //a 16-bit address boundary, and therefore could exist on two TBLPAG pages.
                                        //Therefore, need to reload TBLPAG every iteration of the for() loop
        temp.Val = __builtin_tblrdl((WORD)flashAddress);
        *buffer++ = temp.v[0];
        *buffer++ = temp.v[1];
        flashAddress += 2u;             //Increment address by 2.  No MSD data stored in the upper WORD (which only has one implemented byte anyway).
        
    }   
    
    //Restore TBLPAG register to original value
    TBLPAG = TBLPAGSave;
    
    return TRUE;
}    
#else   //else must be PIC18 or PIC32 device (BYTE organized flash memory)
BYTE MDD_IntFlash_SectorRead(DWORD sector_addr, BYTE* buffer)
{
    //Error check.  Make sure the host is trying to read from a legitimate
    //address, which corresponds to the MSD volume (and not some other program
    //memory region beyond the end of the MSD volume).
    if(sector_addr >= MDD_INTERNAL_FLASH_TOTAL_DISK_SIZE)
    {
        return FALSE;
    }   
    
    //Read a sector worth of data, and copy it to the specified RAM "buffer".
    memcpypgm2ram
    (
        (void*)buffer,
        (ROM void*)(MASTER_BOOT_RECORD_ADDRESS + (sector_addr * MEDIA_SECTOR_SIZE)),
        MEDIA_SECTOR_SIZE
    );

	return TRUE;
}//end SectorRead
#endif


/******************************************************************************
 * Function:        BYTE SectorWrite(DWORD sector_addr, BYTE *buffer, BYTE allowWriteToZero)
 *
 * PreCondition:    None
 *
 * Input:           sector_addr - Sector address, each sector contains 512-byte
 *                  buffer      - Buffer where data will be read
 *                  allowWriteToZero - If true, writes to the MBR will be valid
 *
 * Output:          Returns TRUE if write successful, FALSE otherwise
 *
 * Side Effects:    None
 *
 * Overview:        SectorWrite sends 512 bytes of data from the location
 *                  pointed to by 'buffer' to the card starting
 *                  at the sector address specified by sector_addr.
 *
 * Note:            The sample device expects the address field in the command packet
 *                  to be byte address. Therefore the sector_addr must first
 *                  be converted to byte address. This is accomplished by
 *                  shifting the address left 9 times.
 *****************************************************************************/
#if defined(__18CXX)
#pragma udata myFileBuffer
#endif
#if defined (__dsPIC33E__) || defined (__PIC24E__)
volatile unsigned int file_buffer[ERASE_BLOCK_SIZE] __attribute__((far));
#else
volatile unsigned char file_buffer[ERASE_BLOCK_SIZE] __attribute__((far,aligned));
#endif
#if defined(__18CXX)
#pragma udata
#endif

#define INTERNAL_FLASH_PROGRAM_WORD        0x4003
#define INTERNAL_FLASH_ERASE               0x4042
#define INTERNAL_FLASH_PROGRAM_PAGE        0x4001


#if defined(__C32__)
    #define PTR_SIZE DWORD
#elif defined(__18CXX)
    #define PTR_SIZE UINT24
#else
    #define PTR_SIZE WORD
#endif
ROM BYTE *FileAddress = 0;


#if defined(__C30__)
BYTE MDD_IntFlash_SectorWrite(DWORD sector_addr, BYTE* buffer, BYTE allowWriteToZero)
{
#if !defined(INTERNAL_FLASH_WRITE_PROTECT)
    WORD i;
    BYTE j;
    WORD offset;
    DWORD flashAddress;
    WORD TBLPAGSave;


    //First, error check the resulting address, to make sure the MSD host isn't trying 
    //to erase/program illegal LBAs that are not part of the designated MSD volume space.
    if(sector_addr >= MDD_INTERNAL_FLASH_TOTAL_DISK_SIZE)
    {
        return FALSE;
    }  

    TBLPAGSave = TBLPAG;
    
#if defined (__dsPIC33E__) || defined (__PIC24E__)
    

    // First, save the contents of the entire erase page.  
    // To do this, we need to get a pointer to the start of the erase page.
    // AND mask 0xFFFFF800 is to clear the lower bits, 
    // so we go back to the start of the erase page.
    
    flashAddress = ((DWORD)FILES_ADDRESS + (DWORD)(sector_addr*MEDIA_SECTOR_SIZE)) 
                & (DWORD)0xFFFFF800;  
    
    //Now save all of the contents of the erase page.
    TBLPAG = (BYTE)(flashAddress >> 16);
    for(i = 0; i < ERASE_BLOCK_SIZE;i++)
    {
        file_buffer[i] = __builtin_tblrdl((WORD)flashAddress + (2 * i));
    }    

    // Now we want to overwrite the file_buffer[] contents 
    // for the sector that we are trying to write to.
    // The lower 2 bits of the helps to determine this.
   
    offset = 0x200 * (BYTE)(sector_addr & 0x3);   

    //Overwrite the file_buffer[] RAM contents for the sector that we are trying to write to.
    for(i = 0; i < MEDIA_SECTOR_SIZE; i++)
    {
        *((unsigned char *)file_buffer + offset + i) = *buffer++;
    }
#else

     //First, save the contents of the entire erase page.  To do this, we need to get a pointer to the start of the erase page.
    flashAddress = ((DWORD)FILES_ADDRESS + (DWORD)(sector_addr*MEDIA_SECTOR_SIZE)) & (DWORD)0xFFFFFC00;  //AND mask 0xFFFFFC00 is to clear the lower bits, so we go back to the start of the erase page.
    //Now save all of the contents of the erase page.
    for(i = 0; i < ERASE_BLOCK_SIZE;)
    {
        TBLPAG = (BYTE)(flashAddress >> 16);
        *(WORD*)&file_buffer[i] = __builtin_tblrdl((WORD)flashAddress);
        flashAddress += 2u;    //Skipping upper word.  Don't care about the implemented byte/don't use it when programming or reading from the sector.
        i += 2u;
    }    

    //Now we want to overwrite the file_buffer[] contents for the sector that we are trying to write to.
    //Need to figure out if the buffer[] data goes in the upper sector or the lower sector of the file_buffer[]
    if(sector_addr & 0x00000001)
    {
        //Odd sector address, must be the high file_buffer[] sector
        offset = MEDIA_SECTOR_SIZE;
    }
    else
    {
        offset = 0;
    }        

    //Overwrite the file_buffer[] RAM contents for the sector that we are trying to write to.
    for(i = 0; i < MEDIA_SECTOR_SIZE; i++)
    {
        file_buffer[offset + i] = *buffer++;
    }
    #endif
    

#if defined(__dsPIC33E__) || defined (__PIC24E__)

    INT gieBkUp;

    //Now erase the entire erase page of flash memory.  
    //First we need to calculate the actual flash memory 
    //address of the erase page. 
    
    gieBkUp = INTCON2bits.GIE;
    INTCON2bits.GIE = 0; // Disable interrupts
    NVMADRU = (WORD)(flashAddress >> 16);
    NVMADR = (WORD)(flashAddress & 0xFFFF);
    NVMCON = 0x4003;    // This value will erase a page.
    __builtin_write_NVM();
    INTCON2bits.GIE = gieBkUp; // Enable interrupts

    //Now reprogram the erase page with previously obtained contents of the file_buffer[]
    //We only write to the even flash word addresses, the odd word addresses are left blank.  
    //Therefore, we only store 2 bytes of application data for every 2 flash memory word addresses.
    //This "wastes" 1/3 of the flash memory, but it provides extra protection from accidentally executing
    //the data.  It also allows quick/convenient PSV access when reading back the flash contents.

    TBLPAG = 0xFA;
    j = 0;
    for(i = 0; i < ERASE_BLOCK_SIZE;i++)
    {

       //
        __builtin_tblwtl((j * 2), file_buffer[i]);
        __builtin_tblwth((j * 2), 0);
           
        j ++;

        //Check if we have reached a program block size boundary.  If so, program the last 128 
        //useful bytes (192 bytes total, but 64 of those are filled with '0' filler bytes).
        if(j >= 128u)
        {
            j = j - 128u;
            NVMADRU = (WORD)(flashAddress >> 16);
            NVMADR = (WORD)(flashAddress & 0xFFFF);
            NVMCON = 0x4002;
            gieBkUp = INTCON2bits.GIE;
            INTCON2bits.GIE = 0; // Disable interrupts
            __builtin_write_NVM();
            INTCON2bits.GIE = gieBkUp; // Enable interrupts
            flashAddress += 256;
        }    
    } 
#else 
    //Now erase the entire erase page of flash memory.  
    //First we need to calculate the actual flash memory address of the erase page.  The starting address of the erase page is as follows:
    flashAddress = ((DWORD)FILES_ADDRESS + (DWORD)(sector_addr*MEDIA_SECTOR_SIZE)) & (DWORD)0xFFFFFC00;

    //Peform NVM erase operation.
    NVMCON = INTERNAL_FLASH_ERASE;				    //Page erase on next WR
    __builtin_tblwtl((WORD)flashAddress, 0xFFFF);   //Perform dummy write to load address of erase page
    UnlockAndActivate(NVM_UNLOCK_KEY);

    //Now reprogram the erase page with previously obtained contents of the file_buffer[]
    //We only write to the even flash word addresses, the odd word addresses are left blank.  
    //Therefore, we only store 2 bytes of application data for every 2 flash memory word addresses.
    //This "wastes" 1/3 of the flash memory, but it provides extra protection from accidentally executing
    //the data.  It also allows quick/convenient PSV access when reading back the flash contents.
    NVMCON = INTERNAL_FLASH_PROGRAM_PAGE;
    j = 0;
    for(i = 0; i < ERASE_BLOCK_SIZE;)
    {
        TBLPAG = (BYTE)(flashAddress >> 16);
        __builtin_tblwtl((WORD)flashAddress, *((WORD*)&file_buffer[i]));
        flashAddress++;       
        __builtin_tblwth((WORD)flashAddress, 0);
        flashAddress++;       

        i += 2;
        j += 2;

        //Check if we have reached a program block size boundary.  If so, program the last 128 
        //useful bytes (192 bytes total, but 64 of those are filled with '0' filler bytes).
        if(j >= 128u)
        {
            j = j - 128u;
            asm("DISI #16");					//Disable interrupts for next few instructions for unlock sequence
            __builtin_write_NVM();                        
        }    
    }    
#endif

    TBLPAG = TBLPAGSave;   
    return TRUE;
#else //else of #if !defined(INTERNAL_FLASH_WRITE_PROTECT)
    return TRUE;
#endif  //endif of #if !defined(INTERNAL_FLASH_WRITE_PROTECT)

}    
#else   //else must be PIC18 or PIC32 device
BYTE MDD_IntFlash_SectorWrite(DWORD sector_addr, BYTE* buffer, BYTE allowWriteToZero)
{
    #if !defined(INTERNAL_FLASH_WRITE_PROTECT)
        ROM BYTE* dest;
        BOOL foundDifference;
        WORD blockCounter;
        WORD sectorCounter;

        #if defined(__18CXX)
            BYTE *p;
        #endif

        //First, error check the resulting address, to make sure the MSD host isn't trying 
        //to erase/program illegal LBAs that are not part of the designated MSD volume space.
        if(sector_addr >= MDD_INTERNAL_FLASH_TOTAL_DISK_SIZE)
        {
            return FALSE;
        }  

        dest = (ROM BYTE*)(MASTER_BOOT_RECORD_ADDRESS + (sector_addr * MEDIA_SECTOR_SIZE));

        sectorCounter = 0;

        while(sectorCounter < MEDIA_SECTOR_SIZE)
        {
            foundDifference = FALSE;
            for(blockCounter = 0; blockCounter < ERASE_BLOCK_SIZE; blockCounter++)
            {
                if(dest[sectorCounter] != buffer[sectorCounter])
                {
                    foundDifference = TRUE;
                    sectorCounter -= blockCounter;
                    break;
                }
                sectorCounter++;
            }
            if(foundDifference == TRUE)
            {
                BYTE i,j;
                PTR_SIZE address;

                #if (ERASE_BLOCK_SIZE >= MEDIA_SECTOR_SIZE)
                    address = ((PTR_SIZE)(dest + sectorCounter) & ~(ERASE_BLOCK_SIZE - 1));

                    memcpypgm2ram
                    (
                        (void*)file_buffer,
                        (ROM void*)address,
                        ERASE_BLOCK_SIZE
                    );

                    EraseBlock((ROM BYTE*)address);

                    address = ((PTR_SIZE)(dest + sectorCounter) & (ERASE_BLOCK_SIZE - 1));

                    memcpy
                    (
                        (void*)(&file_buffer[address]),
                        (void*)buffer,
                        MEDIA_SECTOR_SIZE
                    );

                #else

                    address = ((WORD)(&dest[sectorCounter]) & ~(ERASE_BLOCK_SIZE - 1));

                    EraseBlock((ROM BYTE*)address);

                    sectorCounter = sectorCounter & ~(ERASE_BLOCK_SIZE - 1);

                    memcpy
                    (
                        (void*)file_buffer,
                        (void*)buffer+sectorCounter,
                        ERASE_BLOCK_SIZE
                    );
                #endif

                //sectorCounter = sectorCounter & ~(ERASE_BLOCK_SIZE - 1);

                i=ERASE_BLOCK_SIZE/WRITE_BLOCK_SIZE;
                j=0;

                #if defined(__18CXX)
                    p = file_buffer;
                #endif

                while(i-->0)
                {
                    //Write the new data
                    for(blockCounter = 0; blockCounter < WRITE_BLOCK_SIZE; blockCounter++)
                    {
                        //Write the data
                        #if defined(__18CXX)
                            TABLAT = *p++;
                            _asm tblwtpostinc _endasm
                            sectorCounter++;
                        #endif

                        #if defined(__C32__)
                                NVMWriteWord((DWORD*)KVA_TO_PA(FileAddress), *((DWORD*)&file_buffer[sectorCounter]));
                                FileAddress += 4;
                                sectorCounter += 4;
                        #endif
                    }

                    j++;

                    //write the row
                    #if defined(__18CXX)
                        // Start the write process: reposition tblptr back into memory block that we want to write to.
                         _asm tblrdpostdec _endasm

                        // Write flash memory, enable write control.
                        EECON1 = 0x84;
                        UnlockAndActivate(NVM_UNLOCK_KEY);
                        TBLPTR++;
                    #endif
                }
            }
        }
    	return TRUE;
    #else
        return TRUE;
    #endif
} //end SectorWrite
#endif  //#if defined(__C30__)



#if !defined(INTERNAL_FLASH_WRITE_PROTECT)
void EraseBlock(ROM BYTE* dest)
{
    #if defined(__18CXX)
        TBLPTR = (unsigned short long)dest;

        //Erase the current block
        EECON1 = 0x94;
        UnlockAndActivate(NVM_UNLOCK_KEY);
    #endif

    #if defined(__C32__)
        FileAddress = dest;
        NVMErasePage((BYTE *)KVA_TO_PA(dest));
    #endif
}


//------------------------------------------------------------------------------
#warning "Double click this message and read inline code comments.  For production designs, recommend adding application specific robustness features here."
//Function: void UnlockAndActivate(BYTE UnlockKey)
//Description: Activates and initiates a flash memory self erase or program 
//operation.  Useful for writing to the MSD drive volume.
//Note: Self erase/writes to flash memory could potentially corrupt the
//firmware of the application, if the unlock sequence is ever executed
//unintentionally, or if the table pointer is pointing to an invalid
//range (not inside the MSD volume range).  Therefore, in order to ensure
//a fully reliable design that is suitable for mass production, it is strongly
//recommended to implement several robustness checks prior to actually
//performing any self erase/program unlock sequence.  See additional inline 
//code comments.
//------------------------------------------------------------------------------
void UnlockAndActivate(BYTE UnlockKey)
{
    #if defined(__18CXX)
        BYTE InterruptEnableSave;
    #endif
      
    //Should verify that the voltage on Vdd/Vddcore is high enough to meet
    //the datasheet minimum voltage vs. frequency graph for the device.
    //If the microcontroller is "overclocked" (ex: by running at maximum rated
    //frequency, but then not suppling enough voltage to meet the datasheet
    //voltage vs. frequency graph), errant code execution could occur.  It is
    //therefore strongly recommended to check the voltage prior to performing a 
    //flash self erase/write unlock sequence.  If the voltage is too low to meet
    //the voltage vs. frequency graph in the datasheet, the firmware should not 
    //inititate a self erase/program operation, and instead it should either:
    //1.  Clock switch to a lower frequency that does meet the voltage/frequency graph.  Or,
    //2.  Put the microcontroller to Sleep mode.
    
    //The method used to measure Vdd and/or Vddcore will depend upon the 
    //microcontroller model and the module features available in the device, but
    //several options are available on many of the microcontrollers, ex:
    //1.  HLVD module
    //2.  WDTCON<LVDSTAT> indicator bit
    //3.  Perform ADC operation, with the VBG channel selected, using Vdd/Vss as 
    //      references to the ADC.  Then perform math operations to calculate the Vdd.
    //      On some micros, the ADC can also measure the Vddcore voltage, allowing
    //      the firmware to calculate the absolute Vddcore voltage, if it has already
    //      calculated and knows the ADC reference voltage.
    //4.  Use integrated general purpose comparator(s) to sense Vdd/Vddcore voltage
    //      is above proper threshold.
    //5.  If the micrcontroller implements a user adjustable BOR circuit, enable
    //      it and set the trip point high enough to avoid overclocking altogether.
    
    //Example psuedo code.  Exact implementation will be application specific.
    //Please implement appropriate code that best meets your application requirements.
    //if(GetVddcoreVoltage() < MIN_ALLOWED_VOLTAGE)
    //{
    //    ClockSwitchToSafeFrequencyForGivenVoltage();    //Or even better, go to sleep mode.
    //    return;       
    //}    


    //Should also verify the TBLPTR is pointing to a valid range (part of the MSD
    //volume, and not a part of the application firmware space).
    //Example code for PIC18 (commented out since the actual address range is 
    //application specific):
    //if((TBLPTR > MSD_VOLUME_MAX_ADDRESS) || (TBLPTR < MSD_VOLUME_START_ADDRESS)) 
    //{
    //    return;
    //}  
    
    //Verify the UnlockKey is the correct value, to make sure this function is 
    //getting executed intentionally, from a calling function that knew it
    //should pass the correct NVM_UNLOCK_KEY value to this function.
    //If this function got executed unintentionally, then it would be unlikely
    //that the UnlockKey variable would have been loaded with the proper value.
    if(UnlockKey != NVM_UNLOCK_KEY)
    {
        #if defined(__18CXX)
            EECON1bits.WREN = 0;
        #endif
        return;
    }    
    
  
    //We passed the robustness checks.  Time to Erase/Write the flash memory.
    #if defined(__18CXX)
        InterruptEnableSave = INTCON;
        INTCONbits.GIEH = 0;    //Disable interrupts for unlock sequence.
        EECON2 = 0x55;
        EECON2 = 0xAA;
        EECON1bits.WR = 1;      //CPU stalls until flash erase/write is complete
        EECON1bits.WREN = 0;    //Good practice to disable any further writes now.
        //Safe to re-enable interrupts now, if they were previously enabled.
        if(InterruptEnableSave & 0x80)  //Check if GIEH was previously set
        {
            INTCONbits.GIEH = 1;
        }    
    #endif    
    #if defined(__C30__)
    	asm("DISI #16");					//Disable interrupts for next few instructions for unlock sequence
	    __builtin_write_NVM();
    #endif
        
}    
#endif  //endif of "#if !defined(INTERNAL_FLASH_WRITE_PROTECT)"


/******************************************************************************
 * Function:        BYTE WriteProtectState(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          BYTE    - Returns the status of the "write enabled" pin
 *
 * Side Effects:    None
 *
 * Overview:        Determines if the card is write-protected
 *
 * Note:            None
 *****************************************************************************/

BYTE MDD_IntFlash_WriteProtectState(void)
{
    #if defined(INTERNAL_FLASH_WRITE_PROTECT)
        return TRUE;
    #else
	    return FALSE;
    #endif
}

