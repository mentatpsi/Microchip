
1. The hardware profile for this demo is unique in terms of the usage 
   of the SPI in PIC24FJ256DA210 Development Board.
   When this hardware is used, the SPI speed is set to 8 Mhz 
   and SPI Channel 1 is used:

	#define SPI_CLK_SPEED_8MHZ
	#define USE_SST25_SPI1    // <- this is for Google Map Demo.

2. The LED D2 on the PIC24FJ256DA210 Development Board is used 
   as a status LED that blinks every 1 second.

3. EPMP address 17 (PMA17) is enabled to use the upper memory space of 
   the SRAM. The extra space is used to buffer the decoded image.
   To enable this line JP11 must be shunt to pins 2 and 3.
   This is only to be enabled when using PIC24FJ256DA210 Development
   Board.