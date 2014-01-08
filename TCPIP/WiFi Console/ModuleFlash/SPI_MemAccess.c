/*********************************************************************
 *
 *                  SPI and Memory specific definitions
 *
 *********************************************************************
 * FileName:        SPI_MemAccess.c
 * Dependencies:    None
 * Processor:       PIC32
 * Compiler:        Microchip C32 v1.11b or higher
 * Company:         Microchip Technology, Inc.
 *
 * Software License Agreement
 *
 * Copyright (C) 2002-2012 Microchip Technology Inc.  All rights
 * reserved.
 *
 * Microchip licenses to you the right to use, modify, copy, and
 * distribute:
 * (i)  the Software when embedded on a Microchip microcontroller or
 *      digital signal controller product ("Device") which is
 *      integrated into Licensee's product; or
 * (ii) ONLY the Software driver source files ENC28J60.c and
 *      ENC28J60.h ported to a non-Microchip device used in
 *      conjunction with a Microchip ethernet controller for the
 *      sole purpose of interfacing with the ethernet controller.
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
 * Date         Comment
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * 6/15/2015	Original
 ********************************************************************/
#include "HardwareProfile_update.h"
#include "SPI_MemAccess.h"

#if !defined( MRF24WG) && defined( __PIC32MX__ )


#if defined( __PIC32MX__ )

    static inline __attribute__((__always_inline__)) void WaitForDataByte( void )
    {
        while (!WF_SPISTATbits.SPITBE || !WF_SPISTATbits.SPIRBF);
    }
    #define SPI_ON_BIT          (WF_SPICON1bits.ON)
#else
    #error Determine SPI flag mechanism
#endif

    
UINT8 g_txBuffer[5];
UINT8 g_rxBuffer[5];


extern void Write16BitWFRegister(UINT8 regId, UINT16 value);
extern UINT16 Read16BitWFRegister(UINT8 regId);


/*****************************************************************************
 * FUNCTION: Read32BitWFRegisterr_Update
 *
 * RETURNS: register value
 *
 * PARAMS:
 *      regId -- ID of 32-bit register being read
 *
 *  NOTES: Reads WF 32-bit register
 *****************************************************************************/
UINT32 Read32BitWFRegisterr_Update(UINT8 regId)
{
    g_txBuffer[0] = regId | WF_READ_REGISTER_MASK;
    WF_SpiEnableChipSelect();
    
    WFSpiTxRx(g_txBuffer, 
              1,
              g_rxBuffer,
              5);

    WF_SpiDisableChipSelect();
    
    return (((UINT32)g_rxBuffer[1]) << 24) |
           (((UINT32)g_rxBuffer[2]) << 16) |
           (((UINT32)g_rxBuffer[3]) << 8)  |
           (((UINT32)g_rxBuffer[4]) & 0xFF);
}

/*****************************************************************************
 * FUNCTION: Write32BitWFRegister_Update
 *
 * RETURNS: None
 *
 * PARAMS:
 *      regId -- ID of 32-bit register being written to
 *      value -- value to write
 *
 *  NOTES: Writes WF 32-bit register
 *****************************************************************************/
void Write32BitWFRegister_Update(UINT8 regId, UINT32 value)
{
    g_txBuffer[0] = regId | WF_WRITE_REGISTER_MASK;
    g_txBuffer[1] = (UINT8)(value >> 24);   // MSB 
    g_txBuffer[2] = (UINT8)(value >> 16);   
    g_txBuffer[3] = (UINT8)(value >> 8);
    g_txBuffer[4] = (UINT8)(value & 0xFF);  // LSB

    WF_SpiEnableChipSelect();

    WFSpiTxRx(g_txBuffer, 
              5,
              g_rxBuffer,
              1);

    WF_SpiDisableChipSelect();
}


/*****************************************************************************
 * FUNCTION: MemReadU32
 *
 * RETURNS: 32 bit memory read
 *
 * PARAMS:
 *      address -- address of 32-bit value being read
 *
 *  NOTES: Reads 32-bit value from WiFi address
 *****************************************************************************/
UINT32 MemReadU32(UINT32 address)
{
    UINT32 val;
    Write32BitWFRegister_Update(HOST_MEM_WIN_BASE_REG, address);
    Write16BitWFRegister(HOST_MEM_WIN_CTRL_REG, CTRL_READ_32_BIT_MASK);
    val = Read32BitWFRegisterr_Update(HOST_MEM_WIN_DATA_REG);
    return val;
}


/*****************************************************************************
 * FUNCTION: MemWriteU32
 *
 * RETURNS: none
 *
 * PARAMS:
 *      address -- address of 32-bit value being written
 *      value   -- 32-bit value to be written
 *
 *  NOTES: Writes 32-bit value to WiFi address
 *****************************************************************************/
void MemWriteU32(UINT32 address, UINT32 value)
{
    Write32BitWFRegister_Update(HOST_MEM_WIN_BASE_REG, address);
    Write32BitWFRegister_Update(HOST_MEM_WIN_DATA_REG, value);
    Write16BitWFRegister(HOST_MEM_WIN_CTRL_REG, CTRL_WRITE_32_BIT_MASK);
}


/*****************************************************************************
 * FUNCTION: FlashBusSend8
 *
 * RETURNS: none
 *
 * PARAMS:
 *      val -- 8-bit value to send to MRF24WB flash memory IC
 *
 *  NOTES: Writes 8-bit value to MRF24WB flash memory IC
 *****************************************************************************/
void FlashBusSend8(UINT8 val)
{
    UINT16 Temp16 = (val << 8);
    MemWriteU32(F_SPI_WRT_DATA_REG, Temp16);
    MemWriteU32(F_SPI_CTRL_REG, F_SPI_ACTIVATE_8BIT_OP);
    while(MemReadU32(F_SPI_CTRL_REG) & F_SPI_CTRL_ACTIVE_MASK);
}


/*****************************************************************************
 * FUNCTION: FlashBusSend16
 *
 * RETURNS: none
 *
 * PARAMS:
 *      val -- 16-bit value to send to MRF24WB flash memory IC
 *
 *  NOTES: Writes 16-bit value to MRF24WB flash memory IC
 *****************************************************************************/
void FlashBusSend16(UINT16 val)
{
    MemWriteU32(F_SPI_WRT_DATA_REG, val);
    MemWriteU32(F_SPI_CTRL_REG, F_SPI_ACTIVATE_16BIT_OP);
    while(MemReadU32(F_SPI_CTRL_REG) & F_SPI_CTRL_ACTIVE_MASK);
}


/*****************************************************************************
 * FUNCTION: ReadSerialFlashArray
 *
 * RETURNS: none
 *
 * PARAMS:
 *      buffer -- address indicating where to store data read
 *      address -- 32-bit starting address of flash memory to read
 *      length -- number of bytes to be read into the buffer
 *
 *  NOTES: Reads flash memory from MRF24WB into supplied buffer
 *****************************************************************************/
void ReadSerialFlashArray(UINT8 *buffer, UINT32 address, UINT32 length)
{

    UINT32 i;

    Flash_ChipEnableOn();
    
    // Read CMD opcode
    FlashBusSend8(SERIAL_FLASH_READ);

    // 3-Bytes of address
    FlashBusSend8((address >> 16) & 0xFF);
    FlashBusSend8((address >> 8)  & 0xFF);
    FlashBusSend8((address >> 0)  & 0xFF);

    for (i = 0; i < length; i++)
    {
        // toggle out 8-bytes
        MemWriteU32(F_SPI_CTRL_REG, F_SPI_ACTIVATE_8BIT_OP);
        while(MemReadU32(F_SPI_CTRL_REG) & F_SPI_CTRL_ACTIVE_MASK);
        buffer[i] = (MemReadU32(F_SPI_RD_DATA_REG) >> 8);
    }

    Flash_ChipEnableOff();
}


/*****************************************************************************
 * FUNCTION: WriteSerialFlashArray
 *
 * RETURNS: none
 *
 * PARAMS:
 *      buffer -- buffer containing data to be written to MRF24WB flash
 *      address -- 32-bit starting address of flash memory to write
 *      length -- number of bytes to write from buffer to MRF24WB flash
 *
 *  NOTES: Writes MRF24WB memory from supplied buffer
 *****************************************************************************/
void WriteSerialFlashArray(UINT8 *buffer, UINT32 address, UINT32 length)
{
    // program the serial flash with the given patch image
    // 1. enable write 
    // 2. program image
    UINT32 index;

    // in auto address increment programming mode
    // after the first byte programming
    // subsequent bytes don't require address
    // perform first byte program first
    FlashWriteEnable();

    Flash_ChipEnableOn();
    FlashBusSend8(SERIAL_FLASH_AAI_PROGRAM);
    FlashBusSend8((address >> 16) & 0xFF);
    FlashBusSend8((address >> 8)  & 0xFF);
    FlashBusSend8((address >> 0)  & 0xFF);
    FlashBusSend8(buffer[0]);    // first byte of the image
    Flash_ChipEnableOff();

    // starting with the second byte, no address issue is required
    for (index = 1; index < length; index++)
    {
        Flash_ChipEnableOn();
        FlashBusSend16( (SERIAL_FLASH_AAI_PROGRAM << 8) | buffer[index]);
        Flash_ChipEnableOff();
    }

    // disable AAI operation
    Flash_ChipEnableOn();
    FlashBusSend8(SERIAL_FLASH_WRDI);
    Flash_ChipEnableOff();
    
    // poll for flash to finish programming
    while(ReadFlashStatus());
}


/*****************************************************************************
 * FUNCTION: FlashWriteEnable
 *
 * RETURNS: none
 *
 * PARAMS: none
 *
 *  NOTES: Enables write of MRF24WB flash memory
 *****************************************************************************/
void FlashWriteEnable(void)
{
    // write enable sequence
    // 1. enable status reg write
    // 2. clear the status register
    // 3. enable write

    // enable status reg write CMD
    Flash_ChipEnableOn();
    FlashBusSend8(0x50);
    Flash_ChipEnableOff();

    // write state reg (CMD 0x01) with 0x00 (clear it)
    Flash_ChipEnableOn();
    FlashBusSend16(SERIAL_FLASH_WRSR << 8);
    Flash_ChipEnableOff();

    // enable write CMD
    Flash_ChipEnableOn();
    FlashBusSend8(SERIAL_FLASH_WREN);
    Flash_ChipEnableOff();
}


/*****************************************************************************
 * FUNCTION: ReadFlashStatus
 *
 * RETURNS: 8-bit val (0 = not-busy, 1 = busy)
 *
 * PARAMS: none
 *
 *  NOTES: Checks busy status of MRF24WB flash memory
 *****************************************************************************/
UINT8 ReadFlashStatus(void)
{
    UINT32 retVal;

    Flash_ChipEnableOn();

    // Read status reg CMD opcode
    FlashBusSend16(SERIAL_FLASH_RDSR << 8);
    while(MemReadU32(F_SPI_CTRL_REG) & F_SPI_CTRL_ACTIVE_MASK);

    Flash_ChipEnableOff();

    // read it from the register
    retVal = MemReadU32(F_SPI_RD_DATA_REG);
    
    // bit 0 on the status byte = busy bit
    return ((retVal >> 8) & 0x01);
}


/*****************************************************************************
 * FUNCTION: FlashEraseChip
 *
 * RETURNS: none
 *
 * PARAMS: none
 *
 *  NOTES: Erase entire MRF24WB flash memory
 *****************************************************************************/
void FlashEraseChip(void)
{
    // erase serial flash device
    // this is serial flash for the wifi module,
    // NOT the PIC's external serial flash
    // 1. enable write
    // 2. perform chip erase

    FlashWriteEnable();

    // chip erase CMD
    Flash_ChipEnableOn();
    FlashBusSend8(SERIAL_FLASH_CHIP_ERASE);
    Flash_ChipEnableOff();

    // poll for the chip to finish erasing (read status register)
    while(ReadFlashStatus());
}


/*****************************************************************************
 * FUNCTION: SectorErase
 *
 * RETURNS: none
 *
 * PARAMS: 32-bit address of 4kB sector to erase
 *
 *  NOTES: Erase 4kB sector of MRF24WB flash memory at address
 *****************************************************************************/
void SectorErase(UINT32 address)
{
    FlashWriteEnable();

    // chip erase CMD
    Flash_ChipEnableOn();
    FlashBusSend8(0x20);
    // 3-Bytes of address
    FlashBusSend8((address >> 16) & 0xFF);
    FlashBusSend8((address >> 8)  & 0xFF);
    FlashBusSend8((address >> 0)  & 0xFF);
    Flash_ChipEnableOff();

    // poll for the chip to finish erasing (read status register)
    while(ReadFlashStatus());
}


/*****************************************************************************
 * FUNCTION: WFEnableMemAccess
 *
 * RETURNS: none
 *
 * PARAMS: none
 *
 *  NOTES: Configure MRF24WB GPIO for access to flash memory
 *****************************************************************************/
void WFEnableMemAccess(void)
{
    // setup GPIO pins for M_SPI
    MemWriteU32(GPIO_SEL0_REG, 0x00);           // 
    MemWriteU32(GPIO_SEL1_REG, 0x00);           // set the mode to select the SPI signals
    MemWriteU32(GPIO_SEL2_REG, 0x07);           // 
    MemWriteU32(GPIO_DATA_EN_REG, 0x07);        // GPIO_DATA_EN, pins 0,1,2 = output, 3 = input

    MemWriteU32(0x00F000E0, 0);                 // clock gating off
    MemWriteU32(0x00F00210, 0);                 // DMA engine off
}


/*****************************************************************************
 * FUNCTION: WFDisableMemAccess
 *
 * RETURNS: none
 *
 * PARAMS: none
 *
 *  NOTES: Configure MRF24WB GPIO to disable access to flash memory
 *****************************************************************************/
void WFDisableMemAccess(void)
{
    // prevent flash corruption from unintended SPI toggling
    MemWriteU32(GPIO_DATA_EN_REG, 0x00);   // set GPIOs back to input 
}


/*****************************************************************************
  Function:
	void WF_SpiInit_Update(void)

  Summary:
	Initializes the SPI interface to the MRF24WB0M device.

  Description:
	Configures the SPI interface for communications with the MRF24WB0M.

  Precondition:
	None

  Parameters:
	None

  Returns:
  	None
  	
  Remarks:
	This function is called by WFHardwareInit.
*****************************************************************************/
void WF_SpiInit_Update(void)
{
    /* disable the spi interrupt */
    #if defined( __PIC32MX__ )
        WF_SPI_IE_CLEAR = WF_SPI_INT_BITS;
    #else
        WF_SPI_IE = 0;
    #endif

    // Set up the SPI module on the PIC for communications with the MRF24WB0M
    WF_CS_IO       = 1;
    WF_CS_TRIS     = 0;     // Drive SPI MRF24WB0M chip select pin

    ConfigureSpiMRF24WB0M();  
}


/*****************************************************************************
  Function:
	void ConfigureSpiMRF24WB0M(void)

  Summary:
	Configures the SPI interface to the MRF24WB0M.

  Description:
	Configures the SPI interface for communications with the MRF24WB0M.

  Precondition:
	None

  Parameters:
	None

  Returns:
  	None
  	
  Remarks:
	1) If the SPI bus is shared with other peripherals this function is called
	   each time an SPI transaction occurs by WF_SpiEnableChipSelect.  Otherwise it 
	   is called once during initialization by WF_SpiInit_Update. 
	   
	2) Maximum SPI clock rate for the MRF24WB0M is 25MHz.
*****************************************************************************/
void ConfigureSpiMRF24WB0M(void)
{
    /*----------------------------------------------------------------*/
    /* After we save context, configure SPI for MRF24WB0M communications */
    /*----------------------------------------------------------------*/
    /* enable the SPI clocks            */
    /* set as master                    */
    /* clock idles high                 */
    /* ms bit first                     */
    /* 8 bit tranfer length             */
    /* data changes on falling edge     */
    /* data is sampled on rising edge   */
    /* set the clock divider            */
    #if defined(__C30__)
        WF_SPICON1 = 0x027B;    // Fcy Primary prescaler 1:1, secondary prescaler 2:1, CKP=1, CKE=0, SMP=1
        WF_SPICON2 = 0x0000;
        WF_SPISTAT = 0x8000;    // Enable the module
    #elif defined( __PIC32MX__ )
        WF_SPI_BRG = (GetPeripheralClock()-1ul)/2ul/WF_MAX_SPI_FREQ;
        WF_SPICON1 = 0x00000260;    // sample at end, data change idle to active, clock idle high, master
        WF_SPICON1bits.ON = 1;
    #else
        #error Configure SPI for the selected processor
    #endif
}    




/*****************************************************************************
 * FUNCTION: WFChipReset
 *
 * RETURNS: N/A
 *
 * PARAMS:
 *      N/A
 *
 *
 *  NOTES: Performs the necessary SPI operations to cause the MRF24WB0M to reset.
 *      This function also implements a delay so that it will not return until
 *      the WiFi device is ready to receive messages again.  The delay time will
 *      vary depending on the amount of code that must be loaded from serial
 *      flash.
 *****************************************************************************/
void WFChipReset(void)
{
    UINT16 value;

    /* needed for Microchip PICTail (chip enable active low) */
    WF_SetCE_N(WF_LOW); /* set low to enable regulator */

    /* Wait Tinit time.*/
    //DelayMs(50);
    
    /* Configure reset pin */
    WF_SetRST_N(WF_HIGH);

    /* clear the power bit to disable low power mode on the MRF24WB0M */
    Write16BitWFRegister(WF_PSPOLL_H_REG, 0x0000);

    /* Set HOST_RESET bit in register to put device in reset */
    Write16BitWFRegister(WF_HOST_RESET_REG, Read16BitWFRegister(WF_HOST_RESET_REG) | WF_HOST_RESET_MASK);

    /* Clear HOST_RESET bit in register to take device out of reset */
    Write16BitWFRegister(WF_HOST_RESET_REG, Read16BitWFRegister(WF_HOST_RESET_REG) & ~WF_HOST_RESET_MASK);


    /* after reset is started poll register to determine when HW reset has completed */
    do
    {
        Write16BitWFRegister(WF_INDEX_ADDR_REG, WF_HW_STATUS_REG);
        value = Read16BitWFRegister(WF_INDEX_DATA_REG);
    } while ( (value & WF_HW_STATUS_NOT_IN_RESET_MASK) == 0);

    /* if SPI not connected will read all 1's */
    while(value == 0xffff);

    /* now that chip has come out of HW reset, poll the FIFO byte count register */
    /* which will be set to a non-zero value when the MRF24WB0M initialization is   */
    /* complete.                                                                 */
    do
    {
        value = Read16BitWFRegister(WF_HOST_WFIFO_BCNT0_REG) & 0x0fff;
    } while (value == 0);
}

#endif
