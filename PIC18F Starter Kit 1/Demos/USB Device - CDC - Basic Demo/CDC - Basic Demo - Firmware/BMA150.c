#include "Compiler.h"
#include "GenericTypeDefs.h"
#include "BMA150.h"

WORD x;
WORD y;
WORD z;

//**** Private Prototypes ****//
//static void BMA150_WriteByte(BYTE address, BYTE data);
//static BYTE BMA150_ReadByte(BYTE address);
void SpiPinRemap(void);





//**** TEMP JUNK ****//

    // Registers for the SPI module you want to use
    #define SPICON1             SSP2CON1
    #define SPISTAT             SSP2STAT
    #define SPIBUF              SSP2BUF
    #define SPISTAT_RBF         SSP2STATbits.BF
    #define SPICON1bits         SSP2CON1bits
    #define SPISTATbits         SSP2STATbits

    #define SPI_INTERRUPT_FLAG  PIR3bits.SSP2IF

    // Defines for the PIC18 Starter kit
    #define SPICLOCK            TRISCbits.TRISC2
    #define SPIIN               TRISDbits.TRISD6
    #define SPIOUT              TRISBbits.TRISB3

    // Latch pins for SCK/SDI/SDO lines
    #define SPICLOCKLAT         LATCbits.LATC2
    #define SPIINLAT            LATDbits.LATD6
    #define SPIOUTLAT           LATBbits.LATB3

    // Port pins for SCK/SDI/SDO lines
    #define SPICLOCKPORT        PORTCbits.RC2
    #define SPIINPORT           PORTDbits.RD6
    #define SPIOUTPORT          PORTBbits.RB3

    #define SPIENABLE           SSP2CON1bits.SSPEN

    #define BMA150_CS           LATCbits.LATC7
    #define BMA150_CS_TRIS      TRISCbits.TRISC7

void InitBma150 (void)
{
	WORD i;
	BMA150_REG reg;
	SpiPinRemap();
    SPISTAT = 0x0000;               // power on state
    SPICON1 = 0x82;
    SPISTATbits.CKE = 0;
    SPICLOCK = 0;
    SPIOUT = 0;                  // define SDO1 as output (master or slave)
    SPIIN = 1;                  // define SDI1 as input (master or slave)
    SPICON1bits.CKP = 1;
    SPIENABLE = 1;             // enable synchronous serial port

    BMA150_CS_TRIS = 0;
	
    //TRISD = 0;	
	reg.val = BMA150_ReadByte(BMA150_CHIP_ID);
    Nop();	

    if(reg.chip_id != 0x2)
    {
        Nop();
    }
	
	/* 
	Bits 5, 6 and 7 of register addresses 14h and 34h do contain critical sensor individual 
	calibration data which must not be changed or deleted by any means. 
 
	In order to properly modify addresses 14h and/or 34h for range and/or bandwidth selection 
	using bits 0, 1, 2, 3 and 4, it is highly recommended to read-out the complete byte, perform bit-
	slicing and write back the complete byte with unchanged bits 5, 6 and 7.  */ 
 
	reg.val = BMA150_ReadByte(BMA150_ADDR14);
    reg.range =BMA150_RANGE_8G;
	reg.bandwidth = BMA150_BW_50;
    BMA150_WriteByte(BMA150_ADDR14,reg.val);

	 i = BMA150_ReadByte(BMA150_VERSION);
    Nop();

    i = BMA150_ReadByte(BMA150_ADDR11);
    Nop();

    BMA150_WriteByte(BMA150_ADDR11, 0x00);
    Nop();

    i = BMA150_ReadByte(BMA150_ADDR11);
    Nop();

	
}

void SpiPinRemap(void)
{
	//  UnLock Registers
		EECON2 = 0x55;
		EECON2 = 0xAA;
		PPSCONbits.IOLOCK = 0;
	//  Unlock ends
		
	//  Pin Remapping
		RPOR6 = 9;   // RP6 as SDO2 (o/p)  
		RPOR13 = 10; // RP13 as SCK2 (o/p)
		RPINR21 = 23; // RP23 as SDI2 (i/P)
	//  Pin Remapping ends
	
	// 	Lock Registers
		EECON2 = 0x55;
		EECON2 = 0xAA;
		PPSCONbits.IOLOCK = 1;
	//  Lock Registers ends
}

void BMA150_WriteByte(BYTE address, BYTE data)
{
    BYTE storage;

    //See Important Notes section on page 10 note 1 of the v1.5 datasheet
    storage = 0x00;
    if(address == 0x14 || address == 0x34)
    {
        storage = BMA150_ReadByte(0x14) & 0xE0;
    }

    BMA150_CS = 0;
    SPI_INTERRUPT_FLAG = 0;
    SPIBUF = address;
    while (!SPI_INTERRUPT_FLAG);

    Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();
    Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();
    Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();
    Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();
    Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();
    Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();
    Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();
    Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();

    SPI_INTERRUPT_FLAG = 0;

    //See Important Notes section on page 10 note 2 of the v1.5 datasheet
    if(address == 0x0A)
    {
        SPIBUF = data & 0x7F;
    }
    else
    {
        SPIBUF = data | storage;
    }

    while (!SPI_INTERRUPT_FLAG);
    BMA150_CS = 1;
}


BYTE BMA150_ReadByte(BYTE address)
{
    BMA150_CS = 0;
    SPI_INTERRUPT_FLAG = 0;
    SPIBUF = BMA150_READ | address;
    while (!SPI_INTERRUPT_FLAG);

    Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();
    Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();
    Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();
    Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();
    Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();
    Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();
    Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();
    Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();

    SPI_INTERRUPT_FLAG = 0;
    SPIBUF = 0x00;
    while (!SPI_INTERRUPT_FLAG);
    BMA150_CS = 1;

    return SPIBUF;
}
