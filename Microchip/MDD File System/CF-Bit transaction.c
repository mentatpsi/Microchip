/******************************************************************************
 *
 *                Microchip Memory Disk Drive File System
 *
 ******************************************************************************
 * FileName:        CF- Bit transaction.c
 * Dependencies:    GenericTypeDefs.h
 *					FSIO.h
 *                  FSDefs.h
 *					CF- Bit transaction.h
 *					string.h
 * Processor:       PIC18/PIC24/dsPIC30/dsPIC33
 * Compiler:        C18/C30
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


#include "CF- Bit transaction.h"         // compact flash interface
#include "GenericTypeDefs.h"
#include "FSIO.h"
#include "FSDefs.h"
#include "string.h"



/*                       Prototypes                          */


static MEDIA_INFORMATION mediaInformation;

/******************************************************************************
 * Function:        void MDD_CFBT_InitIO(void)
 *
 * PreCondition:    None
 *
 * Input:           void
 *
 * Output:          void
 *
 * Side Effects:    None
 *
 * Overview:        None
 *
 * Note:            None
 *****************************************************************************/

void MDD_CFBT_InitIO( void)
{
   CF_CE = 1;
   CF_CEDIR = OUTPUT;
   CF_OE = 1;
   CF_OEDIR = OUTPUT;
   CF_WE = 1;
   CF_WEDIR = OUTPUT;
   CF_BT_RDY = 0;
   CF_BT_READYDIR = INPUT;
   CF_BT_CD1 = 0;   
   CF_BT_CD1DIR = INPUT;
   CF_BT_RST = 0;
   CF_BT_RESETDIR = OUTPUT;

#ifdef __18CXX
   ADDDIR = 0x00;
#endif

#ifdef __C30__
   ADRTRIS0 = 0;      // AD0-3
   ADRTRIS1 = 0;
   ADRTRIS2 = 0;
   ADRTRIS3 = 0;
#endif

   MDD_CFBT_DATABinput;          // D0-7 input
   
} // CFinit


/******************************************************************************
 * Function:        BYTE MDD_CFBT_MediaDetect(void)
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

BYTE MDD_CFBT_MediaDetect( void)
{
    return (CF_BT_CD1 == 0) ? TRUE : FALSE ; // test for CD1 pull down
} //CFDetect


/******************************************************************************
 * Function:        BYTE MDD_CFBT_WriteProtectState(void)
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

BYTE MDD_CFBT_WriteProtectState (void)
{
   return 0;
}



/******************************************************************************
 * Function:        BYTE MDD_CFBT_CFwait(void)
 *
 * PreCondition:    None
 *
 * Input:           void
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        Wait until the card is ready
 *
 * Note:            None
 *****************************************************************************/

void MDD_CFBT_CFwait(void)
{
   while(CF_BT_RDY == 0);
}


/******************************************************************************
 * Function:        BYTE MDD_CFBT_CFread(void)
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

BYTE MDD_CFBT_CFread( BYTE add)
// add  :   register address
{
   char d;

   MDD_CFBT_CFwait();
   MDD_CFBT_DATABinput;     // make the databus input

#ifdef __C30__
   ADDR0 = (BYTE)(add & 0x01) == 0x01;   // publish the register address
   ADDR1 = (BYTE)(add & 0x02) == 0x02;
   ADDR2 = (BYTE)(add & 0x04) == 0x04;
   ADDR3 = (BYTE)(add & 0x08) == 0x08;
#endif

#ifdef __18CXX
   ADDBL = (add | 0x30);
#endif

   CF_CE = 0;    // select the CF card
   Nop();
   CF_OE = 0;    // output enable
   Nop();
   d = MDD_CFBT_DATABIN;      // get the data
   Nop();
   CF_OE = 1;    
   CF_CE = 1;
   return d;
} // CFread


/******************************************************************************
 * Function:        BYTE MDD_CFBT_MediaInitialize(void)
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

MEDIA_INFORMATION * MDD_CFBT_MediaInitialize (void)
{
    mediaInformation.errorCode = MEDIA_NO_ERROR;
    mediaInformation.validityFlags.value = 0;
    return &mediaInformation;
}

/******************************************************************************
 * Function:        BYTE MDD_CFBT_CFwrite(void)
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

void MDD_CFBT_CFwrite( BYTE add, BYTE d)
// add  : CF register 
// d    : data
{
   MDD_CFBT_CFwait();
   CF_OE = 1;      // make sure the output is disabled first
   MDD_CFBT_DATABoutput;    // make the bus output

#ifdef __C30__
   ADDR0 = (add & 0x01) == 0x01;   // publish the register address
   ADDR1 = (add & 0x02) == 0x02;
   ADDR2 = (add & 0x04) == 0x04;
   ADDR3 = (add & 0x08) == 0x08;
#endif

#ifdef __18CXX
   ADDBL = (add | 0x30);
#endif

   MDD_CFBT_DATABOUT = d;      // publish the data
   CF_CE = 0;    // select the CF card
   Nop();
   Nop();
   CF_WE = 0;    // strobe write
   Nop();
   Nop();
   CF_WE = 1;
   CF_CE = 1;
} // CFwrite




/******************************************************************************
 * Function:        BYTE MDD_CFBT_SectorRead(DWORD sector_addr, BYTE *buffer)
 *
 * PreCondition:    None
 *
 * Input:           sector_addr - Sector address, each sector contains 512-byte
 *                  buffer      - Buffer where data will be stored, see
 *                                'ram_acs.h' for 'block' definition.
 *                                'Block' is dependent on whether internal or
 *                                external memory is used
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


BYTE MDD_CFBT_SectorRead(DWORD sector_addr, BYTE * buffer)
{
   BYTE test;
   WORD i = 0;

    MDD_CFBT_CFwrite( R_COUNT, 1); 
    MDD_CFBT_CFwrite( R_SECT, sector_addr);
    MDD_CFBT_CFwrite( R_CYLO, sector_addr>>8);
    MDD_CFBT_CFwrite( R_CYHI, sector_addr>>16);
    MDD_CFBT_CFwrite( R_DRIVE, ((BYTE)(sector_addr>>24) & 0xf)|0xe0);  // always select card #0
    MDD_CFBT_CFwrite( R_CMD, C_SECTOR_READ);

    while ((test = MDD_CFBT_CFread( R_STATUS)) != S_READY)
    {
      // If error flag is set . . . 
      if (MDD_CFBT_CFread (R_STATUS) & 0x01)
       {
          return FALSE;
       }
    }

#ifdef __C30__
   ADDR0 = (R_DATA & 0x01) == 0x01;   // publish the register address
   ADDR1 = (R_DATA & 0x02) == 0x02;
   ADDR2 = (R_DATA & 0x04) == 0x04;
   ADDR3 = (R_DATA & 0x08) == 0x08;
#endif

#ifdef __18CXX
   ADDBL = (R_DATA | 0x30);
#endif

   MDD_CFBT_DATABinput;         // make the databus input
    CF_CE = 0;        // CF selected 

    if (buffer != NULL)
    {
       while (i < 512)
       {
          CF_OE = 0;
          Nop();
          buffer[i++] = MDD_CFBT_DATABIN;
          CF_OE = 1;
       }
    }
    else
    {
        
    }

    CF_CE = 1;        // CF deselected when done

   return TRUE;       
} // read_sector


/******************************************************************************
 * Function:        BYTE MDD_CFBT_SectorWrite(DWORD sector_addr, BYTE *buffer, BYTE allowWriteToZero)
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

BYTE MDD_CFBT_SectorWrite( DWORD sector_addr, BYTE * buffer, BYTE allowWriteToZero)
// lda   sector lda 
// buf   512 byte block
{
   WORD i;

   if (sector_addr == 0 && allowWriteToZero == FALSE)
      return FALSE;

    MDD_CFBT_CFwrite( R_COUNT, 1); 
    MDD_CFBT_CFwrite( R_SECT, sector_addr);
    MDD_CFBT_CFwrite( R_CYLO, sector_addr>>8);
    MDD_CFBT_CFwrite( R_CYHI, sector_addr>>16);
    MDD_CFBT_CFwrite( R_DRIVE, ((BYTE)(sector_addr>>24) & 0xf)|0xe0);  // always select card #0

    MDD_CFBT_CFwrite( R_CMD, C_SECTOR_WRITE);

    while (MDD_CFBT_CFread( R_STATUS) != S_READY)
    {
       if (MDD_CFBT_CFread( R_STATUS) == S_ERROR)
       {
          return FALSE;
       }
    }

#ifdef __C30__
   ADDR0 = (R_DATA & 0x01) == 0x01;   // publish the register address
   ADDR1 = (R_DATA & 0x02) == 0x02;
   ADDR2 = (R_DATA & 0x04) == 0x04;
   ADDR3 = (R_DATA & 0x08) == 0x08;
#endif

#ifdef __18CXX
   ADDBL = (R_DATA| 0x30);
#endif

   MDD_CFBT_DATABoutput;         // make the databus output
    CF_CE = 0;        // CF selected 
    for ( i=0; i<512; i++)
    {
       MDD_CFBT_DATABOUT = RAMread( buffer, 0);  // read data
        CF_WE = 0;                  // WE enable
       CF_WE = 1;                  // WE disable
       buffer++;
    }
    CF_CE = 1;        // CF deselected when done

   return TRUE;       
} // write_sector


/******************************************************************************
 * Function:        void MDD_CFBT_ShutdownMedia(void)
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


void MDD_CFBT_ShutdownMedia (DISK * dsk)
{
   dsk->mount = FALSE;
   return;
}


