/*************************************************************************
 *  © 2012 Microchip Technology Inc.                                       
 *  
 *  Project Name:    mTouch Framework v2.3
 *  FileName:        mComm_config.h
 *  Dependencies:    mComm_optionsForConfig.h
 *  Processor:       See documentation for supported PIC® microcontrollers 
 *  Compiler:        HI-TECH Ver. 9.81 or later
 *  IDE:             MPLAB® IDE v8.50 (or later) or MPLAB® X                        
 *  Hardware:         
 *  Company:         
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *  Description:     
 *************************************************************************/
/**************************************************************************
 * MICROCHIP SOFTWARE NOTICE AND DISCLAIMER: You may use this software, and 
 * any derivatives created by any person or entity by or on your behalf, 
 * exclusively with Microchip's products in accordance with applicable
 * software license terms and conditions, a copy of which is provided for
 * your referencein accompanying documentation. Microchip and its licensors 
 * retain all ownership and intellectual property rights in the 
 * accompanying software and in all derivatives hereto. 
 * 
 * This software and any accompanying information is for suggestion only. 
 * It does not modify Microchip's standard warranty for its products. You 
 * agree that you are solely responsible for testing the software and 
 * determining its suitability. Microchip has no obligation to modify, 
 * test, certify, or support the software. 
 * 
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER 
 * EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED 
 * WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A 
 * PARTICULAR PURPOSE APPLY TO THIS SOFTWARE, ITS INTERACTION WITH 
 * MICROCHIP'S PRODUCTS, COMBINATION WITH ANY OTHER PRODUCTS, OR USE IN ANY 
 * APPLICATION. 
 * 
 * IN NO EVENT, WILL MICROCHIP BE LIABLE, WHETHER IN CONTRACT, WARRANTY, 
 * TORT (INCLUDING NEGLIGENCE OR BREACH OF STATUTORY DUTY), STRICT 
 * LIABILITY, INDEMNITY, CONTRIBUTION, OR OTHERWISE, FOR ANY INDIRECT, 
 * SPECIAL, PUNITIVE, EXEMPLARY, INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, 
 * FOR COST OR EXPENSE OF ANY KIND WHATSOEVER RELATED TO THE SOFTWARE, 
 * HOWSOEVER CAUSED, EVEN IF MICROCHIP HAS BEEN ADVISED OF THE POSSIBILITY 
 * OR THE DAMAGES ARE FORESEEABLE. TO THE FULLEST EXTENT ALLOWABLE BY LAW, 
 * MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN ANY WAY RELATED TO THIS 
 * SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY, THAT YOU HAVE PAID 
 * DIRECTLY TO MICROCHIP FOR THIS SOFTWARE. 
 * 
 * MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF 
 * THESE TERMS. 
 *************************************************************************/
/** @file   mComm_config.h
*   @brief  mComm Configuration! Main configuration file for the mComm module.
*/
#ifndef __MCOMM_CONFIG_H
#define __MCOMM_CONFIG_H

    #include "mComm_optionsForConfig.h"
    

//****************************************************************************************************
//             ____                            ____       _               
//  _ __ ___  / ___|___  _ __ ___  _ __ ___   / ___|  ___| |_ _   _ _ __  
// | '_ ` _ \| |   / _ \| '_ ` _ \| '_ ` _ \  \___ \ / _ \ __| | | | '_ \ 
// | | | | | | |__| (_) | | | | | | | | | | |  ___) |  __/ |_| |_| | |_) |
// |_| |_| |_|\____\___/|_| |_| |_|_| |_| |_| |____/ \___|\__|\__,_| .__/ 
//                                                                 |_|   
//****************************************************************************************************

    //#define MCOMM_ENABLED                     // Define to enable communications
    
    //#define _XTAL_FREQ                        // Must be defined if not provided elsewhere
                                                // Note: This #define is also located in mTouch_config.h,
                                                //       so it has been commented-out here. If the
                                                //       mComm module is separated from the mTouch
                                                //       framework, this will need to be uncommented
                                                //       and defined with the processor's oscillation
                                                //       frequency in Hz.
    
    // mComm Communication Type
    //:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
    //  Option Label                :: Option Behavior
    //.................................................................................................
    //  MCOMM_UART_ONE_WAY          :: Transmit-only ASCII UART communications - Hardware/Software UART
    //  MCOMM_UART_TWO_WAY          :: Bidirectional Binary UART communications - UART module required
    //  MCOMM_I2C_TWO_WAY           :: Bidirectional Binary I2C communications - SSP module required   
    //  MCOMM_SPI_TWO_WAY           :: Bidirectional Binary SPI communications - SSP module required
    //:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
    #define MCOMM_TYPE                          MCOMM_UART_ONE_WAY
    
                                                        // IMPORTANT INITIALIZATION NOTES:
                                                        //
                                                        //  If I2C ::
                                                        //      SDA pin must be input
                                                        //      SCL pin must be input
                                                        //  If SPI ::
                                                        //      SDI pin must be input
                                                        //      SDO pin must be output
                                                        //      SCK pin must be input
                                                        //      nSS pin must be input
    
    #define MCOMM_UART_BAUDRATE                 38400   // Valid UART baudrate options depend on 
                                                        // whether the UART communications will be 
                                                        // implemented in hardware or software.
                                                        //              
                                                        //    1200  Choose one of the listed speeds and
                                                        //    2400  compile. If mComm is unable to 
                                                        // *  9600  support that speed (given your 
                                                        // * 19200  current Fosc or configuration 
                                                        // * 38400  options), it will help guide you to 
                                                        //   57600  choosing an alternative. 
                                                        //  115200  
                                                        //          
                                                        //  (*) Choose one of these speeds if using a
                                                        //      software UART - most Fosc values have
                                                        //      them supported.
    
    
    //#################################################################################################
    //  _____                  __        __             ____                             ___        _   _                 
    // |_   _|_      _____     \ \      / /_ _ _   _   / ___|___  _ __ ___  _ __ ___    / _ \ _ __ | |_(_) ___  _ __  ___ 
    //   | | \ \ /\ / / _ \ ____\ \ /\ / / _` | | | | | |   / _ \| '_ ` _ \| '_ ` _ \  | | | | '_ \| __| |/ _ \| '_ \/ __|
    //   | |  \ V  V / (_) |_____\ V  V / (_| | |_| | | |__| (_) | | | | | | | | | | | | |_| | |_) | |_| | (_) | | | \__ \
    //   |_|   \_/\_/ \___/       \_/\_/ \__,_|\__, |  \____\___/|_| |_| |_|_| |_| |_|  \___/| .__/ \__|_|\___/|_| |_|___/
    //                                         |___/                                         |_|  
    //#################################################################################################
        #define MCOMM_INPUTBUFFER_SIZE      11                              // Length (in bytes) of the input buffer
                                                                            //  Minimum for full write support: 7
                                                                            //  Minimum for full write w/ stream: 8
                                                                            //  Increment by 2 for best integer support
                                                                            //  Number of 'payload' bytes is this value minus 5
        
        #define MCOMM_ENABLE_NVM_ACCESS                                     // If defined, the mComm system will be
                                                                            //  enable access to EEPROM.
                                                                            //      Opcode: Read NVM     - 0x03
                                                                            //              Write NVM    - 0x02
                                                                            
            //#define MCOMM_NVM_MTOUCH_EE_DISABLED                          // If defined, the mComm system will not
                                                                            //  produce a warning at compile-time if
                                                                            //  NVM access is enabled, but mTouch
                                                                            //  EEPROM is not implemented. In most
                                                                            //  cases, you will want both enabled at
                                                                            //  the same time. Leave this commented
                                                                            //  unless you are receiving the warning
                                                                            //  and know that the configuration is
                                                                            //  how you want it.

        #define MCOMM_ENABLE_STREAM                                         // If defined, the streaming feature of
                                                                            //  the mComm system will be implemented.
                                                                            //      Opcode: Read Stream  - 0x05
                                                                            //              Write Stream - 0x04
                                                                            
            #define MCOMM_STREAM_STORED_IN_RAM                              // If defined, the stream array will be
                                                                            //  stored in RAM, allowing its output
                                                                            //  packet to be changed by the master.
                                                                            // If undefined, the stream array will be
                                                                            //  stored in program memory and will not
                                                                            //  be editable.
                                                                            
            //#define MCOMM_STREAM_EN_ON_POR                                // If defined, the stream will be enabled
                                                                            //  by default on power-up. Otherwise, the
                                                                            //  stream must be manually turned on by
                                                                            //  the master by editing bit 0 of the
                                                                            //  mComm_streamConfig variable.

        #define MCOMM_ENABLE_CUSTOM_OPCODE                                  // If defined, a custom read/write command
                                                                            //  will be generated for application use.
                                                                            // An example custom opcode implementation
                                                                            //  performing a sensor on-state-change
                                                                            //  notification is provided: mComm_custom.c
                                                                            //      Opcode: Read Custom  - 0x07
                                                                            //              Write Custom - 0x06
        
            //:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
            //  These configuration options define the callback functions used by mComm to allow custom
            //  commands and actions to be implemented by the application.
            // 
            //  These are only used if MCOMM_ENABLE_CUSTOM_OPCODE is defined.
            //
            //  An example custom packet is implemented in mComm_custom.c - it creates an onChange
            //  notification packet that outputs the new state mask if any sensors have been updated. This
            //  is used by the mTouch GUI, but may be manipulated to implement any custom action for your
            //  application.
            //
            //  There are ## steps to implementing a custom packet:
            //      1. Enable the #define for MCOMM_ENABLE_CUSTOM_OPCODE
            //      2. Create a callback function to notify mComm if you have data to send.
            //      3. Create a process function to initialize the mComm output vector.
            //          An example is provided in mComm_custom.c. You can also see how the mComm implements
            //          its RAM, NVM, and Stream process functions in mComm_opcodes.c
            //      4. Create a read and/or write iterator function.
            //          Read iterator functions should return the next byte to be output by the comms.
            //          Write iterator functions should write one byte each call and should clear the
            //              'hasNext' bit when finished.
            //
            //  The 'callback' function is used by mComm to see if any custom data is ready to be sent.
            //      This function should return a 1 if you wish to send data, and a 0 if not.
            //  If the 'callback' function returns a 1, mComm will eventually call the 'process' function.
            //      (Input packets have a higher priority. If one is waiting to be processed, the callback
            //       function may be called several times before 'process' is called.)
            //  The 'process' function is called once the custom packet has priority and is scheduled to be
            //      output next. During this function, all output buffer variables should be initialized.
            //  Finally, mComm will call your 'iterator' function each time it requires a new byte. The
            //      'read' iterator function should iterate through its output packet and return the next
            //      byte to be sent. Ex: The first time it's called, it may return a packet byte length.
            //      The next time it returns an opcode, then it returns the first payload byte, etc.
            //      By contrast, the 'write' iterator function will iterate over the input buffer and
            //      write its data to the location in memory pointed to by the output vector.
            //  With your custom command, mComm will use the 'read' iterator function anytime 'callback'
            //      returns a 1 or the master uses the 'read custom' opcode. mComm will use the 'write'
            //      iterator only when the master uses the 'write custom' opcode.
            //
            //  To stop mComm from calling the iterator function and to end the packet, 
            //      mComm_output.flags.bits.hasNext should be cleared.
            //
            //  The mComm code comes with iterators to read and write RAM and EEPROM by default. Using
            //      these for your custom packets will allow for a more efficient result. The example
            //      onChange packet in mComm_custom.c uses the ramReadIterator() function with a custom
            //      buffer.
            //:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
            #define MCOMM_CUSTOM_CALLBACK           mComm_CustomCallback        // Name of function that returns a number:
                                                                                //  1 : Data needs to be sent
                                                                                //  0 : No data needs to be sent
            
            #define MCOMM_CUSTOM_PROCESS            mComm_CustomProcess         // Name of function that initializes the
                                                                                //  output vector
                                                                            
            #define MCOMM_CUSTOM_READ_ITERATOR      mComm_ramReadIterator       // Name of function that iterates through
                                                                                //  the output vector and returns the next 
                                                                                //  byte of the payload
                                                                            
            #define MCOMM_CUSTOM_WRITE_ITERATOR     mComm_CustomWriteIterator   // Name of function that iterates through
                                                                                //  the input buffer and manipulates the data 
                                                                                
            #define MCOMM_CUSTOM_VARIABLE           mComm_GUICustomVariable     // If defined, the address of this variable
                                                                                //  will be stored in the mComm configuration
                                                                                //  address block for access by the master
                                                                                //
                                                                                // NOTE: If the master does not need to
                                                                                //       directly read/write the value 
                                                                                //       of your custom variable, this is 
                                                                                //       not required.



    //#################################################################################################
    //   ___                __        __             ____                             ___        _   _                 
    //  / _ \ _ __   ___    \ \      / /_ _ _   _   / ___|___  _ __ ___  _ __ ___    / _ \ _ __ | |_(_) ___  _ __  ___ 
    // | | | | '_ \ / _ \____\ \ /\ / / _` | | | | | |   / _ \| '_ ` _ \| '_ ` _ \  | | | | '_ \| __| |/ _ \| '_ \/ __|
    // | |_| | | | |  __/_____\ V  V / (_| | |_| | | |__| (_) | | | | | | | | | | | | |_| | |_) | |_| | (_) | | | \__ \
    //  \___/|_| |_|\___|      \_/\_/ \__,_|\__, |  \____\___/|_| |_| |_|_| |_| |_|  \___/| .__/ \__|_|\___/|_| |_|___/
    //                                      |___/                                         |_|       
    //#################################################################################################
    
        //:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
        //  Option Label                        :: Option Behavior
        //.............................................................................................
        //  MCOMM_UART_HARDWARE_MODULE          :: Implement communications using the PIC's UART module 
        //  MCOMM_UART_SOFTWARE_IMPLEMENTATION  :: Implement communications by bit-banging in software
        //:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
        #define MCOMM_UART_1WAY_MODULE          MCOMM_UART_HARDWARE_MODULE

            // One-Way (Transmit-only) Software-Implementation UART Communication Options
            //:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
            // NOTE: These values are only used if the UART is being implemented in software.
            #define MCOMM_UART_SOFT_TXPORT      PORTA   
            #define MCOMM_UART_SOFT_TXTRIS      TRISA   
            #define MCOMM_UART_SOFT_TXPIN       5           // <-- The bit of the PORT/TRIS register
                                                            //     NOT the hardware pin on the device
            
        //:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
        //  Option Label                        :: Option Behavior
        //.............................................................................................
        //  MCOMM_UART_1WAY_HEX                 :: Output ASCII values in hex format       
        //  MCOMM_UART_1WAY_DECIMAL             :: Output ASCII values in decimal format    
        //:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
        #define MCOMM_UART_1WAY_OUTPUT          MCOMM_UART_1WAY_DECIMAL
        #define MCOMM_UART_1WAY_DELIMITER       ','
  
        //  mComm One-Way Output Data
        //:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
        // NOTE: ONLY APPLIES TO ONE-WAY UART CONFIGURATION
        //
        // mTouch One-Way GUI Support:
        //  * STATE, TOGGLE, and SLIDER outputs must always been enabled. 
        //      (If toggle/slider is not being implemented in your configuration, a 0 will be output 
        //       instead of the variable's value.)
        //  * One or both: READING and/or BASELINE outputs must be enabled.
        //  * MATRIX output cannot be enabled. The matrix output is provided for viewing in a terminal
        //      window. It is not supported by the one-way GUI at this time.
        //:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
        
        #define MCOMM_UART_1WAY_OUT_STATE       // <-- If defined, outputs the state mask
        #define MCOMM_UART_1WAY_OUT_TOGGLE      // <-- If defined, outputs the toggle state mask
        #define MCOMM_UART_1WAY_OUT_SLIDER      // <-- If defined, outputs the slider output value
        //#define MCOMM_UART_1WAY_OUT_MATRIX    // <-- If defined, outputs the matrix press coordinate
        #define MCOMM_UART_1WAY_OUT_READING     // <-- If defined, outputs the raw reading values
        //#define MCOMM_UART_1WAY_OUT_BASELINE  // <-- If defined, outputs the sensor baseline values
        
        
//****************************************************************************************************
//  ____            _     _              _          _          _     
// |  _ \ ___  __ _(_)___| |_ ___ _ __  | |    __ _| |__   ___| |___ 
// | |_) / _ \/ _` | / __| __/ _ \ '__| | |   / _` | '_ \ / _ \ / __|
// |  _ <  __/ (_| | \__ \ ||  __/ |    | |__| (_| | |_) |  __/ \__ \
// |_| \_\___|\__, |_|___/\__\___|_|    |_____\__,_|_.__/ \___|_|___/
//            |___/   
//****************************************************************************************************
// These values are provided here for easy access. In most cases you will not need to edit them.
// If your processor has more than one SSP/UART module, you may need to rename these registers to
// specify which SSP/UART you are accessing. ex: SSPSTAT might become SSP1STAT or SSP2STAT.
    #define     MCOMM_I2C_ADDRESS               0xA0            // Address of I2C module when available for a new packet
    #define     MCOMM_I2C_ADDRESS_BUSY          0xA2            // Address of I2C module when not able to receive new packets
    #define     MCOMM_I2C_SSPSTAT               SSPSTAT         // aka SSP1STAT
    #define     MCOMM_I2C_RnW                   SSPSTATbits.R_nW
    #define     MCOMM_I2C_DnA                   SSPSTATbits.D_nA
    #define     MCOMM_I2C_STOP                  SSPSTATbits.P
    #define     MCOMM_I2C_SSPCON1               SSPCON1         // aka SSP1CON1
    #define     MCOMM_I2C_CKP                   SSPCON1bits.CKP
    #define     MCOMM_I2C_SSPOV                 SSPCON1bits.SSPOV
    #define     MCOMM_I2C_WCOL                  SSPCON1bits.WCOL
    #define     MCOMM_I2C_SSPCON2               SSPCON2         // aka SSP1CON2
    #define     MCOMM_I2C_SSPCON3               SSPCON3
    #define     MCOMM_I2C_SSPMSK                SSPMSK
    #define     MCOMM_I2C_SSPADD                SSPADD
    #define     MCOMM_I2C_SSPBUF                SSPBUF
    #define     MCOMM_I2C_PEIE                  INTCONbits.PEIE
    #define     MCOMM_I2C_SSPIE                 PIE1bits.SSPIE
    #define     MCOMM_I2C_SSPIF                 PIR1bits.SSPIF

    #define     MCOMM_SPI_SS_PIN                RB5             // Must be a pin with Interrupt-On-Change functionality
    #define     MCOMM_SPI_ADDRESS               0xA0            // The address is the first byte sent for SPI and must match with this value
    #define     MCOMM_SPI_SSPSTAT               SSPSTAT
    #define     MCOMM_SPI_SSPCON1               SSPCON1
    #define     MCOMM_SPI_SSPCON3               SSPCON3
    #define     MCOMM_SPI_SSPBUF                SSPBUF
    #define     MCOMM_SPI_WCOL                  SSPCON1bits.WCOL
    #define     MCOMM_SPI_SSPOV                 SSPCON1bits.SSPOV
    #define     MCOMM_SPI_SSPIE                 PIE1bits.SSPIE
    #define     MCOMM_SPI_SSPIF                 PIR1bits.SSPIF
    #define     MCOMM_SPI_PEIE                  INTCONbits.PEIE
    #define     MCOMM_SPI_IOCIF                 INTCONbits.IOCIF
    
    #define     MCOMM_UART_TXREG                TXREG
    #define     MCOMM_UART_RCREG                RCREG
    #define     MCOMM_UART_TXSTA                TXSTA
    #define     MCOMM_UART_BRGH                 TXSTAbits.BRGH
    #define     MCOMM_UART_SENDB                TXSTAbits.SENDB
    #define     MCOMM_UART_TXEN                 TXSTAbits.TXEN
    #define     MCOMM_UART_RCSTA                RCSTA
    #define     MCOMM_UART_OERR                 RCSTAbits.OERR
    #define     MCOMM_UART_CREN                 RCSTAbits.CREN
    #define     MCOMM_UART_SPEN                 RCSTAbits.SPEN
    #define     MCOMM_UART_BAUDCON              BAUDCON
    #define     MCOMM_UART_BRG16                BAUDCONbits.BRG16
    #define     MCOMM_UART_SPBRGL               SPBRGL
    #define     MCOMM_UART_SPBRGH               SPBRGH
    #define     MCOMM_UART_PEIE                 INTCONbits.PEIE
    #define     MCOMM_UART_TXIF                 PIR1bits.TXIF
    #define     MCOMM_UART_RCIF                 PIR1bits.RCIF
    #define     MCOMM_UART_RCIE                 PIE1bits.RCIE
    
    #if defined(MCOMM_ENABLE_STREAM) && defined(MCOMM_STREAM_STORED_IN_RAM)
        #define MCOMM_STREAM_ADDR           (uint8_t*)&mComm_stream
        #define MCOMM_STREAMCONFIG_ADDR     (uint8_t*)&mComm_streamConfig
    #elif defined(MCOMM_ENABLE_STREAM)
        #define MCOMM_STREAM_ADDR           (uint8_t*)0x0000
        #define MCOMM_STREAMCONFIG_ADDR     (uint8_t*)&mComm_streamConfig
    #else
        #define MCOMM_STREAM_ADDR           (uint8_t*)0x0000
        #define MCOMM_STREAMCONFIG_ADDR     (uint8_t*)0x0000
    #endif
    
    #if defined(MCOMM_ENABLE_CUSTOM_OPCODE)
        #if defined(MCOMM_CUSTOM_VARIABLE)
        #define MCOMM_CUSTOM_VAR_ADDR       ,(uint8_t*)&MCOMM_CUSTOM_VARIABLE       // Comma is intentional
        #else
        #define MCOMM_CUSTOM_VAR_ADDR
        #endif
    #else
        #define MCOMM_CUSTOM_VAR_ADDR
    #endif

//****************************************************************************************************
//  ___       _ _   _       _ _          _   _              __     __    _                 
// |_ _|_ __ (_) |_(_) __ _| (_)______ _| |_(_) ___  _ __   \ \   / /_ _| |_   _  ___  ___ 
//  | || '_ \| | __| |/ _` | | |_  / _` | __| |/ _ \| '_ \   \ \ / / _` | | | | |/ _ \/ __|
//  | || | | | | |_| | (_| | | |/ / (_| | |_| | (_) | | | |   \ V / (_| | | |_| |  __/\__ \
// |___|_| |_|_|\__|_|\__,_|_|_/___\__,_|\__|_|\___/|_| |_|    \_/ \__,_|_|\__,_|\___||___/
//
//****************************************************************************************************
    
    //========================================================================
    // Streaming Default Values
    //========================================================================
    // The stream is a list of output vectors that can be modified to send any
    // number/order of RAM arrays through the communications.
    //
    // If it is stored in RAM (configuration option, above) then the output
    // vectors of the stream may be edited at runtime by the master or PIC.
    
    #define MCOMM_STREAM_SIZE           7           // Maximum number of user-editable vector locations
                                                    // NOTE: The actual array size is one larger to fit
                                                    //       a permanent 0-length vector entry. This
                                                    //       should never be overwritten by the master!
                                                    //       If it is, the communications will break.
                                                    
    //                                      Variable Address                Length in Bytes
    #define MCOMM_STREAM_VALUE0         {   (uint8_t*)&mTouch_stateMask,    MTOUCH_STATEMASK_BYTES  }
    #define MCOMM_STREAM_VALUE1         {   (uint8_t*)&mTouch_sensorData,   2*MTOUCH_NUMBER_SENSORS }
    #define MCOMM_STREAM_VALUE2         {   (uint8_t*)&mTouch_average,      2*MTOUCH_NUMBER_SENSORS }

    
    //========================================================================
    // mComm Configuration Block Values
    //========================================================================
    // Two configuration blocks are implemented in constant memory to store
    // the current configuration of the system. Which values are stored depends
    // on the following values.
    //
    // This data is accessible by the master by reading from RAM locations:
    //  0x0000 :: Literals
    //  0x0001 :: Addresses
    //
    // These configuration blocks can then tell the master where the important
    // RAM arrays are located that it might wish to read/write to.
    //
    // Example:
    //  * The master wishes to read the current reading for each sensor.
    //      1. The master will request to read from RAM location 0x0000. 
    //          The PIC will respond with the values in MCOMM_CONFIG_LIT_INIT.
    //          The master then takes the 4th byte and knows the number of sensors ("X"). 
    //      2. The master will request to read from RAM location 0x0001.
    //          The PIC will respond with the values in MCOMM_CONFIG_ADDR_INIT.
    //          The master then takes the 5th address and knows where the raw
    //           data array is being stored in RAM ("Y"). 
    //      3. The master will then request to read 2*X bytes from RAM location Y.
    //
    //      NOTE: Steps 1 & 2 only need to be performed once.
    
    #define MCOMM_CONFIG_LIT_SIZE       10
    #define MCOMM_CONFIG_LIT_INIT       {                                                       \
                                            MCOMM_CONFIGURATION_BYTE,                           \
                                            MCOMM_INPUTBUFFER_SIZE,                             \
                                            MCOMM_STREAM_SIZE,                                  \
                                            MTOUCH_NUMBER_SENSORS,                              \
                                            MTOUCH_CONFIGURATION_BYTE,                          \
                                            MTOUCH_EEPROM_POW2_SIZE,                            \
                                            MTOUCH_FRAMEWORK_VERSION,                           \
                                            MTOUCH_SOFTWARE_VERSION,                            \
                                            MTOUCH_HARDWARE_VERSION,                            \
                                            MTOUCH_BOARD_ID                                     \
                                        }
                                    
    
    #define MCOMM_CONFIG_ADDR_SIZE      9
    #define MCOMM_CONFIG_ADDR_INIT      {                                                       \
                                            (uint8_t*) MCOMM_STREAMCONFIG_ADDR,                 \
                                            (uint8_t*) MCOMM_STREAM_ADDR,                       \
                                            (uint8_t*) MTOUCH_EEPROM_START_ADR,                 \
                                            (uint8_t*) &mTouch_stateMask,                       \
                                            (uint8_t*) &mTouch_sensorData,                      \
                                            (uint8_t*) &mTouch_average,                         \
                                            (uint8_t*) MCOMM_SLIDER_ADR,                        \
                                            (uint8_t*) MCOMM_TOGGLE_ADR                         \
                                            MCOMM_CUSTOM_VAR_ADDR                               \
                                        }
#endif
