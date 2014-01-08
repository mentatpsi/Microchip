/******************************************************************************
 *
 *                Microchip Memory Disk Drive File System
 *
 ******************************************************************************
 * FileName:        CF-PMP.c
 * Dependencies:    GenericTypeDefs.h
 *					FSIO.h
 *                  FSDefs.h
 *					CF-PMP.h
 *					string.h
 * Processor:       PIC24/dsPIC30/dsPIC33
 * Compiler:        C30
 * Company:         Microchip Technology, Inc.
 * Version:         1.2.4
 *
 * Software License Agreement
 *
 * The software supplied herewith by Microchip Technology Incorporated
 * (the “Company”) for its PICmicro® Microcontroller is intended and
 * supplied to you, the Company’s customer, for use solely and
 * exclusively on Microchip PICmicro Microcontroller products. The
 * software is owned by the Company and/or its supplier, and is
 * protected under applicable copyright laws. All rights are reserved.
 * Any use in violation of the foregoing restrictions may subject the
 * user to criminal sanctions under applicable laws, as well as to
 * civil liability for the breach of the terms and conditions of this
 * license.
 *
 * THIS SOFTWARE IS PROVIDED IN AN “AS IS” CONDITION. NO WARRANTIES,
 * WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED
 * TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. THE COMPANY SHALL NOT,
 * IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL OR
 * CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 *
 *****************************************************************************/


#include "CF-PMP.h"         // compact flash interface
#include "GenericTypeDefs.h"
#include "FSIO.h"
#include "FSDefs.h"
#include "string.h"



/***************************************************************************/
/*                           Defines                                       */
/***************************************************************************/


/***************************************************************************/
/*                        Global Variables                                 */
/***************************************************************************/

static MEDIA_INFORMATION mediaInformation;

/************************************************************************/
/*                       Prototypes                                     */
/************************************************************************/



/******************************************************************************
 * Function:        BYTE MDD_CFPMP_InitIO(void)
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

void MDD_CFPMP_InitIO( void)
{
   PMADDR = 0x0000;
   PMMODE = 0x1AFF;
   PMAEN = 0xC00F;
   PMCON = 0x83A0;
   
   LATDbits.LATD4 = 1;
   LATDbits.LATD5 = 1;
   TRISDbits.TRISD4 = OUTPUT;
   TRISDbits.TRISD5 = OUTPUT;
   CF_PMP_RDY = 0;
   CF_PMP_READYDIR = INPUT;
   CF_PMP_CD1 = 0;   
   CF_PMP_CD1DIR = INPUT;
   CF_PMP_RST = 0;
   CF_PMP_RESETDIR = OUTPUT;
   
} // CFinit


/******************************************************************************
 * Function:        BYTE MDD_CFPMP_MediaDetect(void)
 *
 * PreCondition:    None
 *
 * Input:           void
 *
 * Output:          TRUE   - Card present
 *                  FALSE   - Card absent
 *
 * Side Effects:    None
 *
 * Overview:        Determines if a card is inserted
 *
 * Note:            None
 *****************************************************************************/

BYTE MDD_CFPMP_MediaDetect( void)
{
    return (CF_PMP_CD1 == 0) ? TRUE : FALSE ; // test for CD1 pull down
} //CFDetect


/******************************************************************************
 * Function:        BYTE MDD_CFPMP_WriteProtectState(void)
 *
 * PreCondition:    None
 *
 * Input:           void
 *
 * Output:          0
 *
 * Side Effects:    None
 *
 * Overview:        Added for compatibility- no write protect feature
 *
 * Note:            None
 *****************************************************************************/

BYTE MDD_CFPMP_WriteProtectState (void)
{
   return 0;
}


/******************************************************************************
 * Function:        BYTE MDD_CFPMP_MediaInitialize(void)
 *
 * PreCondition:    None
 *
 * Input:           void
 *
 * Output:          MEDIA_NO_ERROR - The media initialized successfully
 *
 * Side Effects:    None
 *
 * Overview:        Return a MEDIA_INFORMATION structure to FSIO.c
 *
 * Note:            None
 *****************************************************************************/

MEDIA_INFORMATION * MDD_CFPMP_MediaInitialize (void)
{
    mediaInformation.errorCode = MEDIA_NO_ERROR;
    mediaInformation.validityFlags.value = 0;
    return &mediaInformation;
}

/******************************************************************************
 * Function:        void MDD_CFPMP_CFwait(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        Wait until the card and PMP are ready
 *
 * Note:            None
 *****************************************************************************/

void MDD_CFPMP_CFwait(void)
{
   while(CF_PMP_RDY == 0);
   while(PMMODEbits.BUSY);
}


/******************************************************************************
 * Function:        BYTE MDD_CFPMP_CFread(BYTE add)
 *
 * PreCondition:    None
 *
 * Input:           BYTE add   - address to read from
 *
 * Output:          BYTE      - the byte read
 *
 * Side Effects:    None
 *
 * Overview:        Reads a byte from the CF card
 *
 * Note:            None
 *****************************************************************************/

BYTE MDD_CFPMP_CFread( BYTE add)
// add  :   register address
{
   char d;

   MDD_CFPMP_CFwait();

   MDD_CFPMP_DATABinput;

   PMADDR = (WORD)add | (PMADDR & 0xC000);

   d = PMDIN1;      // get the data

   return d;
} // CFread


/******************************************************************************
 * Function:        BYTE MDD_CFPMP_CFwrite(BYTE add, BYTE d)
 *
 * PreCondition:    None
 *
 * Input:           BYTE add   - the address to write to
 *               BYTE d      - the byte to write
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        Writes a byte to the CF card
 *
 * Note:            None
 *****************************************************************************/

void MDD_CFPMP_CFwrite( BYTE add, BYTE d)
// add  : CF register 
// d    : data
{
   MDD_CFPMP_CFwait();

   MDD_CFPMP_DATABoutput;

   PMADDR = (WORD)add | (PMADDR & 0xC000);

   PMDIN1 = d;      // publish the data
   
} // CFwrite




/******************************************************************************
 * Function:        BYTE MDD_CFPMP_SectorRead(DWORD sector_addr, BYTE *buffer)
 *
 * PreCondition:    None
 *
 * Input:           sector_addr - Sector address, each sector contains 512-byte
 *                  buffer      - Buffer where data will be stored
 *
 * Output:          TRUE      - Sector read
 *               FALSE      - Sector could not be read
 *
 * Side Effects:    None
 *
 * Overview:        SectorRead reads 512 bytes of data from the card starting
 *                  at the sector address specified by sector_addr and stores
 *                  them in the location pointed to by 'buffer'.
 *
 * Note:            None
 *****************************************************************************/


BYTE MDD_CFPMP_SectorRead(DWORD sector_addr, BYTE * buffer)
{
   BYTE test;
   WORD i = 0;

   PMADDRbits.CS1 = 1;

    MDD_CFPMP_CFwrite( R_COUNT, 1); 
    MDD_CFPMP_CFwrite( R_SECT, sector_addr);
    MDD_CFPMP_CFwrite( R_CYLO, sector_addr>>8);
    MDD_CFPMP_CFwrite( R_CYHI, sector_addr>>16);
    MDD_CFPMP_CFwrite( R_DRIVE, ((BYTE)(sector_addr>>24) & 0xf)|0xe0);  // always select card #0
    MDD_CFPMP_CFwrite( R_CMD, C_SECTOR_READ);


   // Dummy read to get the first byte
   MDD_CFPMP_CFread (0);

    while ((test = MDD_CFPMP_CFread( R_STATUS)) != S_READY)
    {
      // If error flag is set . . . 
      if (MDD_CFPMP_CFread (R_STATUS) & 0x01)
       {
          return FALSE;
       }
    }

   PMADDR = R_DATA | (PMADDR & 0xC000);
   
   MDD_CFPMP_CFread (0);

   while (i < 512)
   {
      MDD_CFPMP_CFwait();
      buffer[i++] = PMDIN1;
   }

    PMADDRbits.CS1 = 0;        // CF deselected when done

   return TRUE;       
} // read_sector


/******************************************************************************
 * Function:        BYTE MDD_CFPMP_SectorWrite(DWORD sector_addr, BYTE *buffer, BYTE allowWriteToZero)
 *
 * PreCondition:    None
 *
 * Input:           sector_addr - Sector address, each sector contains 512 bytes
 *                  buffer      - Buffer where data will be read from
 *               allowWriteToZero   - allows write to the MBR sector
 *
 * Output:          TRUE      - Sector written
 *               FALSE      - Sector could not be written
 *
 * Side Effects:    None
 *
 * Overview:        SectorWrite sends 512 bytes of data from the location
 *                  pointed to by 'buffer' to the card starting
 *                  at the sector address specified by sector_addr.
 *
 * Note:            None
 *****************************************************************************/

BYTE MDD_CFPMP_SectorWrite( DWORD sector_addr, BYTE * buffer, BYTE allowWriteToZero)
{
   WORD i;

   if (sector_addr == 0 && allowWriteToZero == FALSE)
      return FALSE;

   PMADDRbits.CS1 = 1;

    MDD_CFPMP_CFwrite( R_COUNT, 1); 
    MDD_CFPMP_CFwrite( R_SECT, sector_addr);
    MDD_CFPMP_CFwrite( R_CYLO, sector_addr>>8);
    MDD_CFPMP_CFwrite( R_CYHI, sector_addr>>16);
    MDD_CFPMP_CFwrite( R_DRIVE, ((BYTE)(sector_addr>>24) & 0xf)|0xe0);  // always select card #0

    MDD_CFPMP_CFwrite( R_CMD, C_SECTOR_WRITE);

   MDD_CFPMP_CFread (0);

    while (MDD_CFPMP_CFread( R_STATUS) != S_READY)
    {
       if (MDD_CFPMP_CFread( R_STATUS) == S_ERROR)
       {
          return FALSE;
       }
    }

   PMADDR = R_DATA | (PMADDR & 0xC000);

    for ( i=0; i<512; i++)
    {
      MDD_CFPMP_CFwait();
       PMDIN1 = RAMread( buffer, 0);  // write data
       buffer++;
    }
    PMADDRbits.CS1 = 0;        // CF deselected when done

   return TRUE;       
} // write_sector



/******************************************************************************
 * Function:        BYTE MDD_CFPMP_ShutdownMedia(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        Unmounts the card
 *
 * Note:            None
 *****************************************************************************/

void MDD_CFPMP_ShutdownMedia (DISK * dsk)
{
   dsk->mount = FALSE;
   return;
}




