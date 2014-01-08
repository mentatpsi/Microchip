/*********************************************************************
 *
 *                Thermo Sensor
 *
 *********************************************************************
 * FileName:        temperature.c
 * Dependencies:    See INCLUDES section below
 * Processor:       PIC18
 * Compiler:        C18 2.30.01+
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
 ********************************************************************/

/** I N C L U D E S **********************************************************/
#include "Compiler.h"
#include "GenericTypeDefs.h"
#include "HardwareProfile.h"
#include "temperature.h"

#if defined(__18CXX)
    #include <spi.h>
#endif

/** V A R I A B L E S ********************************************************/
#if defined(__18CXX)
    #pragma udata
#endif
unsigned int I2CStateVariable;	// Used for keeping track of the I2C state
								// machine when using the HPC Explorer board's
								// I2C based TC74 temperature sensor.
signed char TempAccumulator;	// Used for averaging temperature samples
signed char TempSave;
WORD_VAL temperature;           // Raw data format
char tempString[10];        // Buffer for storing data in ASCII format

/** P R I V A T E  P R O T O T Y P E S ***************************************/

/** D E C L A R A T I O N S **************************************************/
#if defined(__18CXX)
    #pragma code
#endif
/******************************************************************************
 * Function:        void InitTempSensor(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        Initializes SPI interface & chip select line
 *
 * Note:            None
 *****************************************************************************/
void InitTempSensor(void)
{
    #if defined(PICDEM_FS_USB) || defined(PICDEM_FS_USB_K50)
        cs_temp_sensor = 1;
        tris_cs_temp_sensor = OUTPUT_PIN;
        //OpenSPI(SPI_FOSC_16, MODE_11, SMPMID);
        mInitSPI();
       
        // Initialize readable values - default to room temperature
        temperature.Val = 0x0C87;       // 25 degree celsius
        UpdateCelsiusASCII();
	#elif defined(PIC18F87J50_PIM)
        temperature.Val = 0x0C87;       // 25 degree celsius
        UpdateCelsiusASCII();
    
    	//Need to initialize I2C Module to prepare for communication with
    	//TC74 temperature sensor on the HPC Explorer board.
    	mInitI2CPins();				// See io_cfg.h
    	SSP1STAT = 0xC0;			// Slew rate control disabled, SMBus
    	SSP1CON1 = 0x08;			// I2C Master mode
    	SSP1CON2 = 0x00;
    	SSP1ADD = 0x7D;				// Just under 100kHz at 48MHz core frequency
    	SSP1CON1bits.SSPEN = 1;		// Enable MSSP module	
    
    	I2CStateVariable = 0;		// Initial state for I2C state machine
    #elif defined(PIC18F97J94_PIM)
        //This board uses TC1047A analog output temperature sensor located on 
        //the LCD Explorer 3 board.  The analog temperature voltage is connected
        //to microcontroller pin RA4/AN6
        TRISAbits.TRISA4 = 1;
        ANCON1bits.ANSEL6 = 0;  //Analog mode

    #endif    
}//end InitTempSensor

/******************************************************************************
 * Function:        void AcquireTemperature(void)
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
BOOL AcquireTemperature(void)
{
    #if defined(PICDEM_FS_USB) || defined(PICDEM_FS_USB_K50)
        unsigned char Dummy;
		//The PICDEM FS USB Demo Board uses a TC77 (13 bit) temperature sensor and
		//communicates with it through the SPI interface.
		cs_temp_sensor = 0;     //Assert chip select for temperature sensor
		//Read a byte from SPI temp sensor
        PIR1bits.SSPIF = 0;
        Dummy = SSPBUF;
        SSPBUF = 0xFF;
        while(PIR1bits.SSPIF == 0);
      	temperature.v[1] = SSPBUF;	
      	//Read second byte from SPI temp sensor
        PIR1bits.SSPIF = 0;
        SSPBUF = 0xFF;
        while(PIR1bits.SSPIF == 0);	
		temperature.v[0] = SSPBUF;
        PIR1bits.SSPIF = 0;
		cs_temp_sensor = 1;     //De-assert chip select for temperature sensor
	    
		if(temperature.bits.b2 == 0)
			return FALSE;

	#elif defined(__C30__) || defined(__C32__) || defined __XC16__

		//Create temp variables to store the conversion data
		unsigned int tempADC;
		unsigned long int tempADClong;

		//get ready to sample the A/D

        #if defined(PIC24FJ256GB110_PIM)
    		AD1CHS = 0x4;           //MUXA uses AN4
    		AD1PCFGLbits.PCFG4 = 0;
    		for(tempADClong=0;tempADClong<1000;tempADClong++); //Sample delay
    		// Get an ADC sample
    		AD1CON1bits.SAMP = 1;           //Start sampling
    		for(tempADClong=0;tempADClong<1000;tempADClong++); //Sample delay, conversion start automatically
    		AD1CON1bits.SAMP = 0;           //Start sampling
    		for(tempADClong=0;tempADClong<1000;tempADClong++); //Sample delay, conversion start automatically
    		while(!AD1CON1bits.DONE);       //Wait for conversion to complete
        #elif defined(PIC24FJ256GB210_PIM)
    		AD1CHS = 0x4;           //MUXA uses AN4
    		ANSBbits.ANSB4 = 1; 
    		for(tempADClong=0;tempADClong<1000;tempADClong++); //Sample delay
    		// Get an ADC sample
    		AD1CON1bits.SAMP = 1;           //Start sampling
    		for(tempADClong=0;tempADClong<1000;tempADClong++); //Sample delay, conversion start automatically
    		AD1CON1bits.SAMP = 0;           //Start sampling
    		for(tempADClong=0;tempADClong<1000;tempADClong++); //Sample delay, conversion start automatically
    		while(!AD1CON1bits.DONE);       //Wait for conversion to complete
    	#elif defined (DSPIC33EP512MU810_PIM) || defined (PIC24EP512GU810_PIM)
//    		AD1CHS0 = 0x4;           //MUXA uses AN4
//    		for(tempADClong=0;tempADClong<1000;tempADClong++); //Sample delay
//    		// Get an ADC sample
//    		AD1CON1bits.SAMP = 1;           //Start sampling
//    		for(tempADClong=0;tempADClong<1000;tempADClong++); //Sample delay, conversion start automatically
//    		AD1CON1bits.SAMP = 0;           //Start sampling
//    		for(tempADClong=0;tempADClong<1000;tempADClong++); //Sample delay, conversion start automatically
//    		while(!AD1CON1bits.DONE);       //Wait for conversion to complete
            temperature.Val = 0x0000;
	        return TRUE;
    	
        #elif defined(PIC24FJ64GB004_PIM)
    		AD1CHS = 0x6;           //MUXA uses AN6
    		AD1PCFGLbits.PCFG6 = 0;
    		for(tempADClong=0;tempADClong<1000;tempADClong++); //Sample delay
    		// Get an ADC sample
    		AD1CON1bits.SAMP = 1;           //Start sampling
    		for(tempADClong=0;tempADClong<1000;tempADClong++); //Sample delay, conversion start automatically
    		AD1CON1bits.SAMP = 0;           //Start sampling
    		for(tempADClong=0;tempADClong<1000;tempADClong++); //Sample delay, conversion start automatically
    		while(!AD1CON1bits.DONE);       //Wait for conversion to complete
	    #elif defined(PIC24F_STARTER_KIT) || defined(PIC24FJ256DA210_DEV_BOARD) || defined(PIC24FJ64GB502_MICROSTICK)
	        temperature.Val = 0x0000;
	        return TRUE;

        #else
            AD1PCFGbits.PCFG4 = 0;

            AD1CON1 = 0x0000; // SAMP bit = 0 ends sampling ...
            // and starts converting
            AD1CHS = 0x00040000; // Connect RB4/AN4 as CH0 input ..
            // in this example RB2/AN2 is the input
            AD1CSSL = 0;
            AD1CON3 = 0x0002; // Manual Sample, Tad = internal 6 TPB
            AD1CON2 = 0;
            AD1CON1SET = 0x8000; // turn ADC ON

            AD1CON1SET = 0x0002; // start sampling ...
            for(tempADClong=0;tempADClong<1000;tempADClong++); //Sample delay, conversion start automatically
            AD1CON1CLR = 0x0002; // start Converting
            while (!(AD1CON1 & 0x0001));// conversion done?
        #endif

    	tempADC = (signed int)(ADC1BUF0 - 155);
            //At 0 deg C, TC1047A outputs ~500mV, which at Vdd/Vss 
            //  reference of (3.3V/0V) provides ADC result of 155.
            //  At 3.3V reference, each LSb is worth 3.2227mV.  However, 
            //  each TC1047A degree celcius is worth 10mV.
            //  Therefore each LSb of ADC result is worth 0.1652644 degrees C.

        //Convert to TC77 style output, since this is what the 
        //  PC application is expecting.
        //TC77 uses twos complement output with each LSb worth 0.0625 degrees C.

		tempADClong = (unsigned long)tempADC * 3900;
		tempADC = (signed int)(tempADClong >> 7);

		temperature.Val = (WORD)tempADC;
		temperature.Val |= 0x7;

	#elif defined(PIC18F87J50_PIM)	// Uses TC74 (8 bit) 
		//The PIC18F87J50 FS USB Plug-In Module (PIM) does not have a temperature 
		//sensor, but there is a TC74 (8 bit) I2C based temperature sensor on the
		//HPC Explorer demo board.  In order for this temperature demo code to do 
		//anything useful, the PIM should be used in conjunction with the HPC Explorer.
		//The TC74 comes in 5V and 3.3V optimized versions.  If a 5V part is run at
		//3.3V (as with the PIM installed), it may have relatively large offsets.
		return TRUE;	// Don't need to do anything in this function, temperature
						// polling with the PIC18F87J50 FS USB Plug-In Module is
						// done with the PollTempOnHPCExplorer() function.
						// This is done so the I2C communication can be done with
						// a non-blocking approach.
    #elif defined(LOW_PIN_COUNT_USB_DEVELOPMENT_KIT) || defined(PIC16F1_LPC_USB_DEVELOPMENT_KIT)
        temperature.Val = 0x0000;
        
    #elif defined(PIC18F46J50_PIM) || defined(PIC18F47J53_PIM)
		//Create temp variables to store the conversion data
		unsigned int tempADC;
		unsigned long int tempADClong;

		//get ready to sample the A/D
        ADCON0bits.CHS = 0x07;

		for(tempADC = 0; tempADC < 20; tempADC++); 
        ADCON0bits.GO = 1;              // Start AD acquisition and conversion cycle
        while(ADCON0bits.NOT_DONE);     // Wait for conversion
		
        tempADC = ((signed int)ADRES - 124);	
            //At 0 deg C, MCP9701A outputs ~400mV, which at Vdd/Vss 
            //  reference of (3.3V/0V) provides ADC result of 124.
            //  At 3.3V reference, each LSb is worth 3.2227mV.  However, 
            //  each MCP9701A degree celcius is worth 19.5mV.
            //  Therefore each LSb of ADC result is worth 0.1652644 degrees C.

        //Convert to TC77 style output, since this is what the 
        //  PC application is expecting.
        //TC77 uses twos complement output with each LSb worth 0.0625 degrees C.

        //2708 is magic number.  This and the below line used 
        //to avoid having to use floating point math.
        tempADClong = (signed long)tempADC * 2708;
        tempADC = (signed int)(tempADClong >> 7);

        temperature.Val = (WORD)tempADC;
        temperature.Val |= 0x7;

        return TRUE;
    #elif defined(PIC18F_STARTER_KIT_1)
        temperature.Val = 0x0000;
        return TRUE;

    #elif defined(PIC18F97J94_PIM)
		//Create temp variables to store the conversion data
		signed int tempADC;
		signed long int tempADClong;
		
        //TC1047A analog output temp sensor located on LCD Explorer 3 board.
        //Connected to microcontroller pin RA4/AN6
        ADCON1Hbits.MODE12 = 1;         //12-bit mode
        ADCHS0L = 0x06;                 //RA4/AN6 selected
        ADCON1Lbits.SAMP = 1;           //Start sample/convert sequence
        while(ADCON1Lbits.DONE == 0);   //Wait until result available

        //At 0 deg C, TC1047A outputs ~500mV, which at Vdd/Vss 
        //reference of (3.3V/0V) provides ADC result of ~621 (12-bit ADC mode).
        //At 3.3V reference, we get 806uV/LSB.  We can multiply
        //this by the tempco of the TC1047A (1 deg C / 0.010 V) to get
        //a value of 0.08057 deg C / LSB.
        tempADC = (signed int)ADCBUF6 - 621;

        //Convert to TC77 style output, since this is what the 
        //PC application is expecting.
        //TC77 uses twos complement output with each LSb worth 0.0625 degrees C.

        //To get TC77 equivalent value, we need to scale the tempADC value by:
        //(0.08057 deg C / LSB TC1047A) / (0.0625 deg C / LSB TC77) = (1.289 LSB TC77) / (1 LSB TC1047A)
        //To get the TC77 formatted data, multiply the ADC result (which is in 
        //units of LSB TC1047A) by the scaling factor, to get a value in units 
        //of LSB TC77.
        //To avoid floating point math, we can instead multiply both numerator
        //and denominator by big integers, to achieve the same effect as 
        //floating point multiply.
        //Ex: 1.289 = ((1.289 * 32768) / 32768) = ~42240 / 32768
        //In other words: we should mutliply by 42240, then divide the result
        //by 32768, to achieve the same effect as floating point multiply 
        //by 1.289.  Doing this as integer math can save code space and 
        //execution speed.
        tempADClong = (signed long)tempADC * 42240;
        temperature.Val = tempADClong >> 12;    //Equivalent to (dividing by 32768) and then multiplying by 8 (since TC77 bottom 3 LSBs don't contain data)
        temperature.Val |= 0x7;                 //Bottom three LSBs set for TC77 style coding.
        return TRUE;

    #elif defined(PIC18F87J94_PIM)
        //MCP9701A located on the PIC18 Explorer board.
        //Connected to microcontroller pin RA1/AN1
        
		//Create temp variables to store the conversion data
		signed int tempADC;
		signed long int tempADClong;
		
        //TC1047A analog output temp sensor located on LCD Explorer 3 board.
        //Connected to microcontroller pin RA4/AN6
        ADCON1Hbits.MODE12 = 1;         //12-bit mode
        ADCHS0L = 0x01;                 //RA1/AN1 selected
        ADCON1Lbits.SAMP = 1;           //Start sample/convert sequence
        while(ADCON1Lbits.DONE == 0);   //Wait until result available

        //At 0 deg C, MCP9701A outputs ~400mV, which at Vdd/Vss 
        //reference of (3.3V/0V) provides ADC result of ~496 (12-bit ADC mode).
        //At 3.3V reference, we get 806uV/LSB.  We can multiply
        //this by the tempco of the MCP9701A (1 deg C / 0.0195 V) to get
        //a value of 0.041316 deg C / LSB.
        tempADC = (signed int)ADCBUF1 - 496;

        //Convert to TC77 style output, since this is what the 
        //PC application is expecting.
        //TC77 uses twos complement output with each LSb worth 0.0625 degrees C.

        //To get TC77 equivalent value, we need to scale the tempADC value by:
        //(0.041316 deg C / LSB MCP9701A) / (0.0625 deg C / LSB TC77) = (0.6611 LSB TC77) / (1 LSB MCP9701A)
        //To get the TC77 formatted data, multiply the ADC result (which is in 
        //units of LSB MCP9701A) by the scaling factor, to get a value in units 
        //of LSB TC77.
        //To avoid floating point math, we can instead multiply both numerator
        //and denominator by big integers, to achieve the same effect as 
        //floating point multiply.
        //Ex: 0.6611 = ((0.6611 * 65536) / 65536) = ~43323 / 65536
        //In other words: we should mutliply by 43323, then divide the result
        //by 65536, to achieve the same effect as floating point multiply 
        //by 0.6611.  Doing this as integer math can save code space and 
        //execution speed.
        tempADClong = (signed long)tempADC * 43323;
        temperature.Val = tempADClong >> 13;    //Equivalent to (dividing by 65536) and then multiplying by 8 (since TC77 bottom 3 LSBs don't contain data)
        temperature.Val |= 0x7;                 //Bottom three LSBs set for TC77 style coding.
        return TRUE;

	#else
		#error "Unknown temperature acquire configuration.  See AcquireTemperature function in temperature.c"
    #endif
    return TRUE;
}//end AcquireTemperature

/******************************************************************************
 * Function:        void PollTempOnHPCExplorer(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          Temperature data from TC74 on HPC Explorer, but formatted 
 *					like the TC77, stored in the "temperature" variable.
 * Side Effects:    None
 *
 * Overview:        None
 *
 * Note:            None
 *****************************************************************************/
#if defined(PIC18F87J50_PIM)
void PollTempOnHPCExplorer(void)
{
	//The PIC18F87J50 FS USB Plug-In Module (PIM) does not have a temperature 
	//sensor, but there is a TC74 (8 bit) I2C based temperature sensor on the
	//HPC Explorer demo board.  In order for this temperature demo code to do 
	//anything useful, the PIM must be used in conjunction with the HPC Explorer.
	//The TC74 comes in 5V and 3.3V optimized versions.  If a 5V part is run at
	//3.3V (as with the PIM installed), it may have relatively large offsets.		 
	#define TC74AddressWrite	0b10011010	// This is the default address for the TC74, use this for writes
	#define TC74AddressRead		0b10011011	// This is the default address for the TC74, use this for reads
	#define RTR 				0x00		// This is the read temp command for TC74

	// Should not use blocking functions in USB code.  Therefore, these I2C
	// communications are done with a state machine as shown below.
	switch(I2CStateVariable)
	{
		case 0x00:
			PIR1bits.SSP1IF = 0;
			SSP1CON2bits.SEN = 1;				// Send Start Bit
			I2CStateVariable = 0x01;
			break;
		case 0x01:
			if(PIR1bits.SSP1IF == 0) {break;}
			PIR1bits.SSP1IF = 0;
			SSP1BUF = TC74AddressWrite;			// Begin sending the actual address
			I2CStateVariable = 0x02;
			break;
		case 0x02:
			if(PIR1bits.SSP1IF == 0) {break;}
			PIR1bits.SSP1IF = 0;
			SSP1BUF = RTR;						// Send command to select the TEMP register
			I2CStateVariable = 0x03;
			break;
		case 0x03:
			if(PIR1bits.SSP1IF == 0) {break;}
			PIR1bits.SSP1IF = 0;
			SSP1CON2bits.SEN = 1;				// Send another start bit
			I2CStateVariable = 0x04;
			break;
		case 0x04:
			if(PIR1bits.SSP1IF == 0) {break;}
			PIR1bits.SSP1IF = 0;
			SSP1BUF = TC74AddressRead;			// Send the address again, but this time "read"
			I2CStateVariable = 0x05;
			break;
		case 0x05:
			if(PIR1bits.SSP1IF == 0) {break;}
			PIR1bits.SSP1IF = 0;
			SSP1CON2bits.RCEN = 1;				// Initiate read from device
			I2CStateVariable = 0x06;
			break;
		case 0x06:
			if(PIR1bits.SSP1IF == 0) {break;}
			PIR1bits.SSP1IF = 0;
			TempSave = SSP1BUF;					// Finally got the result, need to save it
			SSP1CON2bits.ACKDT = 1;				// Prepare I2C NACK handshake
			SSP1CON2bits.ACKEN = 1;				// Send the acknowledge bit
			I2CStateVariable = 0x07;
			break;
		case 0x07:
			if(PIR1bits.SSP1IF == 0) {break;}
			PIR1bits.SSP1IF = 0;
			SSP1CON2bits.PEN = 1;				// Now send a stop bit
			I2CStateVariable = 0x08;
			break;
		case 0x08:
			if(PIR1bits.SSP1IF == 0) {break;}
			PIR1bits.SSP1IF = 0;
			I2CStateVariable = 0x09;
			break;
		case 0x09:
			TempAccumulator = ((TempAccumulator + TempSave) / 2);	// Get an average
			I2CStateVariable = 0x10;
			break;
		case 0x10:
			//Now format data like that of the TC77, which is what the PC side code expects,
			//since it was originally intended to be used with the TC77.
			temperature.v[1] = TempAccumulator;
			if(temperature.bits.b15 == 0)		// == 0 when positive temperature
			{
			    temperature.Val = temperature.Val >> 1;
			    temperature.bits.b15 = 0;			// Positive temperature, upper MSb clear
			}
			else
			{
			    temperature.Val = temperature.Val >> 1;
			    temperature.bits.b15 = 1;			// Negative temperature, upper MSb set
			}
			temperature.bits.b0 = 1;			// Lower three LSbs = 1 on TC74 format
			temperature.bits.b1 = 1;			// Lower three LSbs = 1 on TC74 format
			temperature.bits.b2 = 1;			// Lower three LSbs = 1 on TC74 format

			I2CStateVariable = 0x11;			// Could go back to 0, but don't want to poll that fast
			break;
		case 0x5000:							// This slows down the sample rate of the device.
			I2CStateVariable = 0x00;			// The temp sensor itself only updates around 8 times/sec.
			break;
        default:
			I2CStateVariable++;
            break;	
    }//end switch
}//end PollTempOnHPCExplorer
#endif

/******************************************************************************
 * Function:        void UpdateCelsiusASCII(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        This routine converts data output to ASCII string
 *
 * Note:            None
 *****************************************************************************/
void UpdateCelsiusASCII(void)
{
    WORD_VAL temp;
    BYTE i;
        
    temp.Val = temperature.Val >> 3;
    
    if(temp.bits.b12 == 0)
    {
        temp.byte.HB &= 0x1F;
        tempString[0] = '+';
    }
    else
    {
        temp.byte.HB |= 0xE0;
        tempString[0] = '-';
        temp.Val = temp.Val ^ 0xFFFF;   // Negate
        temp.Val++;
    }//end if

    temp.Val = (temp.Val*10U) >> 4;     // Turn into celsius xx.x
    
    /* Populate string */
    for(i=4;i>0;i--)
    {
        tempString[i] = (((char)(temp.Val % 10)) & 0x0F) | 0x30;
        temp.Val /= 10;
    }//end for
    
    /* Turn leading zeros into spaces */
    if(tempString[1] == '0')
    {
        tempString[1] = ' ';
        if(tempString[2] == '0')
            tempString[2] = ' ';
    }//end if
    
    /* Adjust decimal digit */
    tempString[5] = tempString[4];
    tempString[4]='.';
    tempString[6]=0xF8;     // Degree symbol
    tempString[7]='C';
    tempString[8]='\r';
    tempString[9]=0x00;     // Null-Terminated
    
}//end UpdateCelsiusASCII

/** EOF temperature.c ********************************************************/
