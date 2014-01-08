/*************************************************************************
 *  © 2012 Microchip Technology Inc.                                       
 *  
 *  Project Name:    mTouch Framework v2.3
 *  FileName:        mComm.c
 *  Dependencies:    mComm.h
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
/** @file   mComm.c
*/

#include "mTouch.h"
#include "mComm.h"

#if defined(MCOMM_ENABLED)

    //========================================================================
    // GLOBAL VARIABLES
    //========================================================================
    #if defined(MCOMM_TWO_WAY_ENABLED)
    mComm_OutputVector  mComm_output;
    mComm_InputBuffer   mComm_input;
    #endif
    
    #if defined(MCOMM_ONE_WAY_ENABLED)
    #if (MCOMM_UART_1WAY_MODULE == MCOMM_UART_SOFTWARE_IMPLEMENTATION)
    bank0 uint8_t       mComm_shiftReg;
    bank0 uint8_t       mComm_bitCount;
    bank0 uint8_t       mComm_delayCount;
    #endif
    #endif
    
    //========================================================================
    // FUNCTION PROTOTYPES
    //========================================================================
        void    mComm_Init              (void);
    
    #if defined(MCOMM_TWO_WAY_ENABLED)
        void    mComm_Receive           (void);
        void    mComm_Process           (uint8_t);
    #endif
    
    #if defined(MCOMM_UART_IMPLEMENTED)
        void    mComm_Service           (void);
        void    mComm_UART_PutChar      (uint8_t);
    #if defined(MCOMM_ONE_WAY_ENABLED)
        void    mComm_UART_Char2ASCII   (uint8_t);
        void    mComm_UART_Int2ASCII    (uint16_t);
    #else
        void    mComm_UART_SendACKorNACK(uint8_t);
    #endif
    #endif
    
    
    /*********************************************************************************
    *             ____                              ___       _ _   
    *  _ __ ___  / ___|___  _ __ ___  _ __ ___     |_ _|_ __ (_) |_ 
    * | '_ ` _ \| |   / _ \| '_ ` _ \| '_ ` _ \     | || '_ \| | __|
    * | | | | | | |__| (_) | | | | | | | | | | |    | || | | | | |_ 
    * |_| |_| |_|\____\___/|_| |_| |_|_| |_| |_|___|___|_| |_|_|\__|
    *                                         |_____|  
    *
    *   Initializes the variables and communication module registers to prepare for 
    *   the first packet to be sent/received.
    *********************************************************************************/
    void mComm_Init()
    {    
        //===========================
        // Variable Initialization
        //===========================
        #if defined(MCOMM_TWO_WAY_ENABLED)
            mComm_input.index           = 0;                //===========================
            mComm_input.counter         = 0;                // Initialize input buffer
            mComm_input.checksum        = 0;                //===========================
            #if defined(MCOMM_UART_IMPLEMENTED) || defined(MCOMM_SPI_IMPLEMENTED)
            mComm_input.state           = 0;
            #endif
            mComm_input.flags.all       = 0x00;
            
            for (int8_t i = MCOMM_INPUTBUFFER_SIZE-1; i >= 0; i--)
            {
                mComm_input.buffer[i]   = 0;
            }
        
            mComm_output.flags.all      = 0x00;             //===========================
            mComm_output.counter        = 0;                // Initialize output buffer
            mComm_output.latch          = 0;                //===========================
            mComm_output.iterator       = 0x0000;
            mComm_output.vector.pointer = 0x0000;
            mComm_output.vector.length  = 0;
            
            
            #if defined(MCOMM_ENABLE_STREAM)
            #if defined(MCOMM_STREAM_EN_ON_POR)
            mComm_streamConfig.enabled  = 1;                //===========================
            #else                                           // Initialize stream feature
            mComm_streamConfig.enabled  = 0;                //===========================
            #endif
            mComm_streamConfig.go       = 0;                
            mComm_streamConfig.index    = 0;                
            #endif
        #endif
        
        //===========================
        // Hardware Initialization
        //===========================
        #if defined(MCOMM_UART_IMPLEMENTED)
        
            #if (MCOMM_TYPE == MCOMM_UART_ONE_WAY) && (MCOMM_UART_1WAY_MODULE == MCOMM_UART_SOFTWARE_IMPLEMENTATION)
            
                //==============================
                // Software UART Initialization
                //==============================
            
            asm("BANKSEL    "   ___mkstr(       MCOMM_UART_SOFT_TXPORT));
            asm("bsf        "   ___mkstr(NOBANK(MCOMM_UART_SOFT_TXPORT)) ", " ___mkstr(MCOMM_UART_SOFT_TXPIN));
            
            asm("BANKSEL    "   ___mkstr(       MCOMM_UART_SOFT_TXTRIS));
            asm("bcf        "   ___mkstr(NOBANK(MCOMM_UART_SOFT_TXTRIS)) ", " ___mkstr(MCOMM_UART_SOFT_TXPIN));
            
            #else
            
                //==============================
                // Hardware UART Initialization
                //==============================
                
            MCOMM_UART_TXSTA    = 0;   
            MCOMM_UART_RCSTA    = 0;    
            MCOMM_UART_BAUDCON  = 0;    
                
            MCOMM_UART_SPBRGL   = MCOMM_UART_SPBRGL_VALUE;
            #if defined(MCOMM_UART_SPBRGH_AVAILABLE)
            MCOMM_UART_SPBRGH   = MCOMM_UART_SPBRGH_VALUE;
            MCOMM_UART_BRG16    = MCOMM_UART_BRG16_VALUE;
            #endif
            MCOMM_UART_BRGH     = MCOMM_UART_BRGH_VALUE;
                        
            MCOMM_UART_TXEN     = 1;            // Enable transmission
            MCOMM_UART_SPEN     = 1;            // Enable serial port
            #if (MCOMM_TYPE != MCOMM_UART_ONE_WAY)
            MCOMM_UART_CREN     = 1;            // Enable continuous reception            
            MCOMM_UART_RCIE     = 1;            // Turn on UART Receive interrupts
            #endif
            MCOMM_UART_PEIE     = 1;            // Turn on peripheral interrupts
            
            #endif
            
        #elif defined(MCOMM_I2C_IMPLEMENTED)
    
                //=============================
                // Hardware I2C Initialization
                //=============================
                
            MCOMM_I2C_SSPSTAT = 0b10000000;     // SMP:     Slew rate control disabled for standard speed mode.
                                                // CKE:     Disable SMBus specific inputs
            MCOMM_I2C_SSPCON1 = 0b00110110;     // WCOL:    Write Collision bit cleared
                                                // SSPOV:   Receive Overflow bit cleared
                                                // SSPEN:   Serial Port enabled
                                                // CKP:     Clock line released
                                                // SSPM:    I2C Slave mode, 7-bit address
            MCOMM_I2C_SSPCON2 = 0b00000001;     // GCEN:    General call disabled
                                                // SEN:     Clock stretching is enabled for both slave transmit and slave receive
            MCOMM_I2C_SSPCON3 = 0b01000000;     // SCIE:    Start condition interrupt is disabled
                                                // BOEN:    Buffer overwrite is disabled
                                                // SDAHT:   Minimum of 100ns hold time on SDA after the falling edge of SCL
                                                // SBCDE:   Slave mode bus collision detection interrupts are disabled
                                                // AHEN:    Address holding disabled
                                                // DHEN:    Data holding disabled
                                                
            MCOMM_I2C_SSPMSK  = 0b11111110;     // Address Mask
            MCOMM_I2C_SSPADD  = MCOMM_I2C_ADDRESS;
            
            MCOMM_I2C_SSPIE   = 1;              // Turn on I2C slave interrupts
            MCOMM_I2C_PEIE    = 1;              // Turn on peripheral interrupts
        
            // NOTE:
            // SDA pin must be input
            // SCL pin must be input
        
        #elif defined(MCOMM_SPI_IMPLEMENTED)
        
            MCOMM_SPI_SSPSTAT = 0b00000000;     // SMP:     Cleared for slave mode
                                                // CKE:     Tramsmit occurs on transition from idle to active clock state
                                                // BF:      Buffer full bit cleared
            MCOMM_SPI_SSPCON1 = 0b00100100;     // WCOL:    Write Collection bit cleared
                                                // SSPOV:   Receive overflow bit cleared
                                                // SSPEN:   Serial Port enabled
                                                // CKP:     Idle state for clock is low
                                                // SSPM:    SPI Slave mode, nSS pin enabled
            MCOMM_SPI_SSPCON3 = 0b00000000;     // BOEN:    Buffer overwrite disabled
            
            MCOMM_SPI_SSPIE   = 1;              // Turn on SPI slave interrupts
            MCOMM_SPI_PEIE    = 1;              // Turn on peripheral interrupts
            
            // NOTE:
            // SDI pin must be input
            // SDO pin must be output
            // SCK pin must be input (slave)
            // nSS pin must be input
    
        #endif
    }

    
    
    
    #if defined(MCOMM_TWO_WAY_ENABLED)
    /*********************************************************************************
    *
    *             ____                              ____               _           
    *  _ __ ___  / ___|___  _ __ ___  _ __ ___     |  _ \ ___  ___ ___(_)_   _____ 
    * | '_ ` _ \| |   / _ \| '_ ` _ \| '_ ` _ \    | |_) / _ \/ __/ _ \ \ \ / / _ \
    * | | | | | | |__| (_) | | | | | | | | | | |   |  _ <  __/ (_|  __/ |\ V /  __/
    * |_| |_| |_|\____\___/|_| |_| |_|_| |_| |_|___|_| \_\___|\___\___|_| \_/ \___|
    *                                         |_____|   
    *
    *   This is the common function across all communication types that receives a
    *   new byte from the hardware module. The received bytes are stored in the
    *   input buffer and provided to the application only after a correct checksum
    *   has been evaluated.
    *********************************************************************************/
    void mComm_Receive()
    {
        uint8_t data;                                   // Stores the received byte
        
        
        
    #if defined(MCOMM_UART_IMPLEMENTED)
    
        //===========================
        //  _   _   _    ____ _____ 
        // | | | | / \  |  _ \_   _|
        // | | | |/ _ \ | |_) || |  
        // | |_| / ___ \|  _ < | |  
        //  \___/_/   \_\_| \_\|_|  
        //
        //===========================
        
        data = MCOMM_UART_RCREG;
        
        if ((data == 0x00) && (FERR == 1))
        {
            /**************************************************************
            * 1. 'Break' character received. Start of new packet.
            **************************************************************/
            mComm_input.state = MCOMM_START;
            return;
        }       
       
        if (MCOMM_UART_OERR)
        {
            /**************************************************************
            * UART overrun error has occurred. Reset the state machine and
            * the hardware module and then exit.
            **************************************************************/
            MCOMM_UART_CREN = 0;
            asm("NOP");
            asm("NOP");
            MCOMM_UART_CREN = 1;
            mComm_input.state = MCOMM_IDLE;
            return;
        }
        else if (mComm_input.state == MCOMM_START)
        {
            /**************************************************************
            * 2. Get the byte count to determine the expected packet length
            *    and initialize the counter and index variables.
            **************************************************************/
            mComm_input.counter     = data;             // Store packet length
            mComm_input.index       = 0;                // Reset buffer index
            mComm_input.checksum    = 0;                // Reset checksum value
            mComm_input.state       = MCOMM_BUFFER;
            return;
        } 
        else if (mComm_input.state == MCOMM_BUFFER)
        {
            /**************************************************************
            * 3. Knowing the packet length, buffer all incoming data. Also,
            *    keep a running calculation of the checksum.
            **************************************************************/
            mComm_input.buffer[mComm_input.index] = data;

            if (--mComm_input.counter == 0)
            {
                mComm_input.state = MCOMM_PROCESS_DATA;
            } 
            else
            {  
                mComm_input.index++;
            }
            mComm_input.checksum ^= data;
        }
        else
        {
            /**************************************************************
            * Invalid packet received. Reset the state machine.
            **************************************************************/
            mComm_input.state = MCOMM_IDLE;
            return;
        }

        
        if (mComm_input.state == MCOMM_PROCESS_DATA)
        {
            /**************************************************************
            * 4. Complete packet received. Reset the state machine and set
            *    a flag to process the buffer's data.
            **************************************************************/
            if (mComm_input.checksum == 0)
            {
                mComm_input.flags.bits.full = 1;
            }
            else
            {
                mComm_input.flags.bits.invalid = 1;
            }
            mComm_input.state = MCOMM_IDLE;
        }
        
    // end - defined(MCOMM_UART_IMPLEMENTED)
    #elif defined(MCOMM_I2C_IMPLEMENTED)
    
   
        //===========================
        //  ___ ____   ____ 
        // |_ _|___ \ / ___|
        //  | |  __) | |    
        //  | | / __/| |___ 
        // |___|_____|\____|
        //
        //===========================

        MCOMM_I2C_SSPIF = 0;
        
        if (MCOMM_I2C_STOP)
        {
            if (mComm_input.flags.bits.write)   // If we've stopped after a write
            {                                   //  request, execute the write.
                /**************************************************************
                * Packet complete. Validate checksum, then provide the data 
                * to the application layer.
                **************************************************************/
                if (mComm_input.checksum == 0)
                {
                    MCOMM_I2C_SSPADD = MCOMM_I2C_ADDRESS_BUSY;  // Change I2C Address to stop writes
                    
                    mComm_Process(mComm_input.buffer[0]);       // Write Response
                    while (mComm_output.iterator());            // Execute writes
                    
                    MCOMM_I2C_SSPADD = MCOMM_I2C_ADDRESS;       // Change I2C Address back on
                }
            }
        }
        else if (MCOMM_I2C_RnW)          // Read or Write?
        {   
        
            mComm_input.flags.bits.write = 0;       // Set 'read' flag.    
            
            if (mComm_input.checksum == 0)          // Respond only if 'write' request
            {                                       //  checksum is valid.
            
                if (!MCOMM_I2C_DnA)                 // Special case: Address Byte
                {
                    /**************************************************************
                    * Read Address
                    *   This code is executed if the master sent a read address
                    *   in the last byte of the communication. Using this to
                    *   initialize variables for the new output stream.
                    **************************************************************/
                    mComm_output.checksum   = 0;
                    mComm_Process(mComm_input.buffer[0]);
                }
                
                /**************************************************************
                * Read 
                *   This code is executed to output the requested data to the
                *   master. The pointer has already been set up, so this code
                *   simply outputs it until the byteCounter reaches zero.
                **************************************************************/
                if (mComm_output.flags.bits.hasNext)  
                {
                    data = mComm_output.iterator();     // Get next output value
                    mComm_output.checksum ^= data;      // Update checksum calculation
                } else {
                    data = mComm_output.checksum;       // Last byte sent is checksum
                }
                
                do {
                    MCOMM_I2C_WCOL = 0;     // Clear write-collision flag
                    
                    // Output data to I2C bus
                    if (!MCOMM_I2C_CKP)     
                        MCOMM_I2C_SSPBUF = data;
                        
                } while (MCOMM_I2C_WCOL);   // Retry if failed until successful

            } // end: Valid Checksum
        }
        else        // Read or Write?   Write.
        {         

            mComm_input.flags.bits.write = 1;
        
            if (MCOMM_I2C_DnA)              
            {                           
                
                /**************************************************************
                * Write Data
                *   This state is called when we are receiving data from the
                *   master. This might be an opcode, an address, a payload
                *   value, or a checksum.
                **************************************************************/
                data = MCOMM_I2C_SSPBUF;                        // Store the received data
                mComm_input.buffer[mComm_input.index] = data;   // Insert into buffer
                mComm_input.checksum ^= data;                   // Update checksum
                mComm_input.index++;                            // Increment index
                
            }
            else                            
            {                           
            
                /**************************************************************
                * Write Address
                *   This state is called when a new packet is being received.
                **************************************************************/
                mComm_input.index       = 0;    // Reset the packet's byte counter
                mComm_input.checksum    = 0;    // Reset the packet's checksum value
                data = MCOMM_I2C_SSPBUF;        // Store the received I2C address to clear SSPBUF
                
            }   // end DnA
            
        }   // end RnW
        
        MCOMM_I2C_SSPOV = 0;    // Clearing overflow flag
        MCOMM_I2C_CKP = 1;      // Stop clock stretching and allow
                                //    the master to begin clocking
                                //    out the new data in SSPBUF.

        
    // end - defined(MCOMM_I2C_IMPLEMENTED)
    #elif defined(MCOMM_SPI_IMPLEMENTED)
    
        //===========================
        //  ____  ____ ___ 
        // / ___||  _ \_ _|
        // \___ \| |_) | | 
        //  ___) |  __/| | 
        // |____/|_|  |___|
        // 
        //===========================    
        
        MCOMM_SPI_SSPIF = 0;
        data = MCOMM_SPI_SSPBUF;
        
        if (mComm_input.state == MCOMM_SPI_START)
        {
            /**************************************************************
            * 1. 'CS' pin just activated. Start of new packet.
            **************************************************************/
            if (data == MCOMM_SPI_ADDRESS)  {   mComm_input.state == MCOMM_SPI_PACKET_LENGTH;   MCOMM_SPI_SSPBUF = MCOMM_OP_ACK;    }
            else                            {   mComm_input.state == MCOMM_SPI_IDLE;            MCOMM_SPI_SSPBUF = data;            }
            return;
        }
        else if (mComm_input.state == MCOMM_SPI_PACKET_LENGTH)
        {
            /**************************************************************
            * 2. Store the length of the packet and initialize the packet data.
            **************************************************************/
            mComm_input.state       = MCOMM_SPI_BUFFER;
            mComm_input.counter     = data;             // Store packet length
            mComm_input.index       = 0;                // Reset buffer index
            mComm_input.checksum    = 0;                // Reset checksum value
            MCOMM_SPI_SSPBUF        = 0x00;
            return;
        }
        else if (mComm_input.state == MCOMM_SPI_BUFFER)
        {
            /**************************************************************
            * 3. Knowing the packet length, buffer all incoming data. Also,
            *    keep a running calculation of the checksum.
            **************************************************************/
            mComm_input.buffer[mComm_input.index] = data;

            if (--mComm_input.counter == 0)
            {
                mComm_input.state   = MCOMM_SPI_PROCESS_DATA;
            } 
            else
            {  
                mComm_input.index++;
            }
            mComm_input.checksum ^= data;
        }
        else if (mComm_input.state == MCOMM_SPI_IDLE)
        {
            /**************************************************************
            * Invalid packet. 
            **************************************************************/
            MCOMM_SPI_SSPBUF    = data;
            return;
        }

        
        if (mComm_input.state == MCOMM_SPI_PROCESS_DATA)
        {
            /**************************************************************
            * 4. Complete packet received. Reset the state machine and 
            *    process the buffer's data.
            **************************************************************/
            if (mComm_input.checksum == 0)
            {
                mComm_Process(mComm_input.buffer[0]);
                
                if (!(mComm_input.buffer[0] & 0x01))        // If 'write' command
                {
                    while (mComm_output.iterator());        // Execute writes    
                    MCOMM_SPI_SSPBUF    = MCOMM_OP_ACK;     // Output 0xAA
                    mComm_input.state   = MCOMM_SPI_IDLE;   // Reset state machine
                    return;
                }
                mComm_input.state = MCOMM_SPI_OUTPUT;       // If 'read' command, output data
            }
        }
        
        if (mComm_input.state == MCOMM_SPI_OUTPUT)
        {
            /**************************************************************
            * 5. Master is expecting data. Find the next byte to output
            *    and place it in the SSP module's buffer register.
            **************************************************************/

            if (mComm_output.flags.bits.hasNext)  
            {
                data = mComm_output.iterator();
                mComm_output.checksum ^= data;
            } else {
                data = mComm_output.checksum;
            }
            
            do {
                MCOMM_SPI_WCOL = 0;         // Clear write-collision flag
                MCOMM_SPI_SSPBUF = data;    // Output data to SPI bus
            } while (MCOMM_SPI_WCOL);       // Retry if failed until successful

            MCOMM_SPI_SSPOV = 0;            // Clearing overflow flag
            return;
        }
        
        MCOMM_SPI_SSPBUF    = 0x00;
        
    #endif // end - defined(MCOMM_SPI_IMPLEMENTED)
    }
    
    
    #if defined(MCOMM_SPI_IMPLEMENTED)
    void mComm_SPI_ServiceSS(void)
    {
        if (MCOMM_SPI_IOCIF)
        {
            MCOMM_SPI_IOCIF  = 0;
            if (mComm_input.flags.bits.ssLatch != MCOMM_SPI_SS_PIN)     // Latch does not match actual value
            {
                if (mComm_input.flags.bits.ssLatch == 1)                // If we were not selected, but now are
                    mComm_input.state = MCOMM_SPI_START;                // Begin a new packet
                    
                mComm_input.flags.bits.ssLatch = MCOMM_SPI_SS_PIN;      // Reset latch value
            }
        }
    }
    #endif // end - defined(MCOMM_SPI_IMPLEMENTED)
    
    #endif // end - defined(MCOMM_TWO_WAY_ENABLED)
    

    /*********************************************************************************
    *
    *             ____                              ____                  _          
    *  _ __ ___  / ___|___  _ __ ___  _ __ ___     / ___|  ___ _ ____   _(_) ___ ___ 
    * | '_ ` _ \| |   / _ \| '_ ` _ \| '_ ` _ \    \___ \ / _ \ '__\ \ / / |/ __/ _ \
    * | | | | | | |__| (_) | | | | | | | | | | |    ___) |  __/ |   \ V /| | (_|  __/
    * |_| |_| |_|\____\___/|_| |_| |_|_| |_| |_|___|____/ \___|_|    \_/ |_|\___\___|
    *                                         |_____| 
    *
    *   When implementing the mComm module through the UART, this
    *   service function should be called to process any fully-
    *   received packets and output the stream if the 'enabled'
    *   and 'go' bits are set in its configuration register.
    *********************************************************************************/
    #if defined(MCOMM_UART_IMPLEMENTED)
    void mComm_Service()
    {
    
        //===========================
        //  _   _   _    ____ _____ 
        // | | | | / \  |  _ \_   _|
        // | | | |/ _ \ | |_) || |  
        // | |_| / ___ \|  _ < | |  
        //  \___/_/   \_\_| \_\|_|  
        //
        //===========================
        #if defined(MCOMM_TWO_WAY_ENABLED)
        union
        {
            uint8_t     all;
            
            struct 
            {
                unsigned R_nW           :1;
                unsigned processInput   :1;
                unsigned invalidInput   :1;
                unsigned run            :1;
                #if defined(MCOMM_ENABLE_CUSTOM_OPCODE)
                unsigned processCustom  :1;
                #endif
            } flags;
        } mode;                         // This temporary structure holds all
                                        //  flags for this function in 8-bits.

        uint8_t data;        
        mode.all = 0;                   // Clear all flags.
        
        if (mComm_input.flags.bits.invalid == 1)                    //===========================
        {                                                           // Invalid Packet
            mComm_input.flags.all = 0;                              //===========================
            mComm_UART_SendACKorNACK(MCOMM_OP_NACK);                // NACK
        }
      
        #if defined(MCOMM_ENABLE_STREAM)
        if (mComm_streamConfig.go && mComm_streamConfig.enabled)    //===========================
        {                                                           // Stream ready for output
            mode.flags.run              = 1;                        //===========================
            mode.flags.R_nW             = 1;  
        }
        #endif
        
        #if defined(MCOMM_ENABLE_CUSTOM_OPCODE)                     
        if (MCOMM_CUSTOM_CALLBACK())                                //================================
        {                                                           // Custom packet ready for output
            mode.flags.run              = 1;                        //================================
            mode.flags.R_nW             = 1;
            mode.flags.processCustom    = 1;
        }
        #endif
        
        if (mComm_input.flags.bits.full)                            //================================
        {                                                           // Input ready for processing
            mode.flags.run              = 1;                        //================================
            mode.flags.R_nW             = mComm_input.buffer[0] & 0x01;
            mode.flags.processInput     = 1;
            #if defined(MCOMM_ENABLE_CUSTOM_OPCODE)
            mode.flags.processCustom    = 0;
            #endif
        }
      
        if (mode.flags.run)     // If there is something to be output by the UART...
        {
            //=============================
            // Initialize the Iterator
            //=============================
            #if defined(MCOMM_ENABLE_STREAM) || defined(MCOMM_ENABLE_CUSTOM_OPCODE)
            if (mode.flags.processInput)
            {
            #endif
                mComm_Process(mComm_input.buffer[0]);
            #if defined(MCOMM_ENABLE_STREAM) || defined(MCOMM_ENABLE_CUSTOM_OPCODE)
            }
            else
            {
                #if defined(MCOMM_ENABLE_STREAM) && defined(MCOMM_ENABLE_CUSTOM_OPCODE)
                if (mode.flags.processCustom)
                {
                    mComm_Process(MCOMM_OP_CUSTOM_READ);
                } else {
                    mComm_Process(MCOMM_OP_STREAM_READ);
                }
                #else
                    #if defined(MCOMM_ENABLE_STREAM)
                    mComm_Process(MCOMM_OP_STREAM_READ);
                    #else
                    mComm_Process(MCOMM_OP_CUSTOM_READ);
                    #endif
                #endif
            }
            #endif
        
            //=============================
            // Execute
            //=============================
            if (mode.flags.R_nW)    // Read
            {
                mComm_output.checksum = 0;
            
                if (mComm_output.flags.bits.hasNext)        // Send BREAK character
                {
                    MCOMM_UART_SENDB = 1;
                    MCOMM_UART_TXREG = 0x00;
                    while(MCOMM_UART_SENDB);
                }
                
                while (mComm_output.flags.bits.hasNext)     // Execute reads. Send data.
                {
                    data = mComm_output.iterator();             
                    mComm_UART_PutChar(data);
                    if (mComm_output.flags.bits.second == 0)
                    {
                        mComm_output.checksum ^= data;
                    }
                }
                mComm_UART_PutChar(mComm_output.checksum);  // Send checksum.
            }
            else    // Write
            {
                while (mComm_output.iterator());            // Execute writes.
                mComm_UART_SendACKorNACK(MCOMM_OP_ACK);     // Send ACK.
            }
            
            //=============================
            // Clean-Up
            //=============================
            #if defined(MCOMM_ENABLE_STREAM)
            if (mode.flags.processInput)
            {
            #endif
                mComm_input.flags.bits.full = 0;            // Reset 'full' input buffer flag
            #if defined(MCOMM_ENABLE_STREAM)
            }
            else
            {
                #if defined(MCOMM_ENABLE_STREAM) && defined(MCOMM_ENABLE_CUSTOM_OPCODE)
                if (!mode.flags.processCustom)
                {
                    mComm_streamConfig.go = 0;              // Reset 'go' stream flag
                }
                #else
                    #if defined(MCOMM_ENABLE_STREAM)
                    mComm_streamConfig.go = 0;
                    #endif
                #endif
            }
            #endif
        }
        #endif // end - defined(MCOMM_TWO_WAY_ENABLED)
        
        #if defined(MCOMM_ONE_WAY_ENABLED)
        
            
            // OUTPUT LOGIC FOR ONE-WAY COMMUNICATIONS.
            
            // mComm_UART_Int2ASCII()  converts a 16-bit value to ASCII and outputs it using mComm_UART_PutChar()
            // mComm_UART_Char2ASCII() converts an 8-bit value to ASCII and outputs it using mComm_UART_PutChar()
            
            #if defined(MCOMM_UART_1WAY_OUT_STATE)
            mComm_UART_Int2ASCII((uint16_t)mTouch_stateMask);
            #else
            mComm_UART_Int2ASCII(0);
            #endif
            
            #if defined(MCOMM_UART_1WAY_OUT_TOGGLE) && defined(MTOUCH_TOGGLE_ENABLED)
            mComm_UART_Int2ASCII((uint16_t)mTouch_toggle);
            #else
            mComm_UART_Int2ASCII(0);
            #endif
            
            #if defined(MCOMM_UART_1WAY_OUT_SLIDER) && defined(MTOUCH_NUMBER_OF_SLIDERS) && (MTOUCH_NUMBER_OF_SLIDERS > 0)
            mComm_UART_Char2ASCII(mTouch_slider[0]);            
            #else
            mComm_UART_Char2ASCII(0);
            #endif

            #if defined(MCOMM_UART_1WAY_OUT_MATRIX) && defined(MTOUCH_MATRIX_ENABLED)
            mComm_UART_PutChar('(');
            if (mTouch_Matrix_isPressed())
            {
                mComm_UART_PutChar((uint8_t)(mTouch_Matrix_getColumn()) + 0x30);
                mComm_UART_PutChar(':');
                mComm_UART_PutChar((uint8_t)(mTouch_Matrix_getRow())    + 0x30);
            }
            else
            {
                mComm_UART_PutChar('x');
                mComm_UART_PutChar(':');
                mComm_UART_PutChar('x');
            }
            mComm_UART_PutChar(')');
            mComm_UART_PutChar(MCOMM_UART_1WAY_DELIMITER);
            #endif
            
            #if defined(MCOMM_UART_1WAY_OUT_READING) || defined(MCOMM_UART_1WAY_OUT_BASELINE)
            for (uint8_t i = 0; i < MTOUCH_NUMBER_SENSORS; i++)
            {
                #if defined(MCOMM_UART_1WAY_OUT_READING)
                mComm_UART_Int2ASCII(mTouch_GetSensor(i));      
                #endif                                          
                #if defined(MCOMM_UART_1WAY_OUT_BASELINE)
                mComm_UART_Int2ASCII(mTouch_GetAverage(i));     
                #endif
            }
            #endif
            
            mComm_UART_PutChar(0x0D);   // CR                   
            mComm_UART_PutChar(0x0A);   // LF
        
        #endif
    }
    #endif // end - defined(MCOMM_UART_IMPLEMENTED)

    
    #if defined(MCOMM_TWO_WAY_ENABLED)
    /*********************************************************************************
    *             ____                              ____                              
    *  _ __ ___  / ___|___  _ __ ___  _ __ ___     |  _ \ _ __ ___   ___ ___  ___ ___ 
    * | '_ ` _ \| |   / _ \| '_ ` _ \| '_ ` _ \    | |_) | '__/ _ \ / __/ _ \/ __/ __|
    * | | | | | | |__| (_) | | | | | | | | | | |   |  __/| | | (_) | (_|  __/\__ \__ \
    * |_| |_| |_|\____\___/|_| |_| |_|_| |_| |_|___|_|   |_|  \___/ \___\___||___/___/
    *                                         |_____|            
    *
    *   Interprets the master's request (stored in the input buffer), initializes 
    *   the output structure, and iterator variables.
    *
    *   This is performed using a lookup table of function pointers with the opcode
    *   parameter behaving as the index of the lookup. 
    *********************************************************************************/
    void mComm_Process(uint8_t opcode)
    {    
        //=============================
        // Valid Opcode
        //=============================
        if (opcode < MCOMM_NUMBER_OPCODES)              
        {
            mComm_output.flags.all          = 0x00;     // Reset output vector's flags
            mComm_output.flags.bits.first   = 1;
            mComm_output.flags.bits.hasNext = 1;
            
            mComm_output.counter    = 0;                // Reset output vector's variables
            mComm_output.latch      = 0;
            mComm_output.opcode     = opcode;
            mComm_output.iterator   = mComm_opcode[opcode].iterator;
            
            mComm_opcode[opcode].initialize();          // Initialize the iterator
            return;
        }
        
        //=============================
        // Invalid Opcode
        //=============================
        mComm_output.iterator = &mComm_ErrorIterator;   
    }
    #endif // end - defined(MCOMM_TWO_WAY_ENABLED)
    
    
    /*********************************************************************************
    *  _   _      _                   _____                 _   _                 
    * | | | | ___| |_ __   ___ _ __  |  ___|   _ _ __   ___| |_(_) ___  _ __  ___ 
    * | |_| |/ _ \ | '_ \ / _ \ '__| | |_ | | | | '_ \ / __| __| |/ _ \| '_ \/ __|
    * |  _  |  __/ | |_) |  __/ |    |  _|| |_| | | | | (__| |_| | (_) | | | \__ \
    * |_| |_|\___|_| .__/ \___|_|    |_|   \__,_|_| |_|\___|\__|_|\___/|_| |_|___/
    *              |_|                                              
    *********************************************************************************/
    #if defined(MCOMM_UART_IMPLEMENTED)
    
        #if defined(MCOMM_TWO_WAY_ENABLED)
        void mComm_UART_SendACKorNACK(uint8_t opcode)
        {
            while(MCOMM_UART_TXIF == 0);  
            MCOMM_UART_SENDB = 1;
            MCOMM_UART_TXREG = 0x00;
            while(MCOMM_UART_SENDB);        // BREAK
            mComm_UART_PutChar(0x02);       // Packet Length
            mComm_UART_PutChar(opcode);     // Acknowledge
            mComm_UART_PutChar(opcode);     // Checksum
        }    
        #endif
        
        // Performs the steps required to output 8-bits of data to the Tx pin of the
        //  microcontroller. If a UART hardware module is being used, this is simple.
        //  If the software will be bit-banging the data, the timing is critical so
        //  an assembly-coded list of instructions will run the output.
        void mComm_UART_PutChar(uint8_t data)
        {
            #if defined(MCOMM_TWO_WAY_ENABLED) || (MCOMM_UART_1WAY_MODULE == MCOMM_UART_HARDWARE_MODULE)
            
                while(MCOMM_UART_TXIF == 0);     
                MCOMM_UART_TXREG = data;
                
            #else
                
                #pragma regsused mComm_UART_PutChar wreg status fsr0
                
                mComm_shiftReg = data;   
                
                #if defined(_PIC14E)        // Implementation if using an enhanced-core PIC16F1 device
                asm("   movlw LOW  " ___mkstr(NOBANK(MCOMM_UART_SOFT_TXPORT)));
                asm("   movwf      " ___mkstr(FSR0L)                 );
                asm("   movlw HIGH " ___mkstr(NOBANK(MCOMM_UART_SOFT_TXPORT)));
                asm("   movwf      " ___mkstr(FSR0H)                 ); 
                asm("   BANKSEL    _mComm_bitCount                  "); 
                asm("   movlw      0x08                             "); 
                asm("   movwf      _mComm_bitCount                  "); 
                asm("   bcf        " ___mkstr(INDF0) ", " ___mkstr(MCOMM_UART_SOFT_TXPIN));                                 
                asm("   fcall      MCOMM_UART_BITDELAY              "); 
                asm("   ljmp       $+1                              "); 
                asm("   nop                                         "); 
                asm("MCOMM_UART_TXNEXTBIT:                          "); 
                asm("   rrf        _mComm_shiftReg, F               "); 
                asm("   btfss      " ___mkstr(NOBANK(STATUS))   ", 0"); 
                asm("   ljmp       MCOMM_UART_TXZERO                "); 
                asm("MCOMM_UART_TXONE:                              "); 
                asm("   nop                                         "); 
                asm("   bsf        " ___mkstr(INDF0) ", " ___mkstr(MCOMM_UART_SOFT_TXPIN));
                asm("   fcall      MCOMM_UART_BITDELAY              "); 
                asm("   decfsz     _mComm_bitCount, F               "); 
                asm("   ljmp       MCOMM_UART_TXNEXTBIT             "); 
                asm("   ljmp       MCOMM_UART_TXSTOP                "); 
                asm("MCOMM_UART_TXZERO:                             "); 
                asm("   bcf        " ___mkstr(INDF0) ", " ___mkstr(MCOMM_UART_SOFT_TXPIN));                    
                asm("   fcall      MCOMM_UART_BITDELAY              "); 
                asm("   decfsz     _mComm_bitCount, F               "); 
                asm("   ljmp       MCOMM_UART_TXNEXTBIT             "); 
                asm("   ljmp       MCOMM_UART_TXSTOP                "); 
                asm("MCOMM_UART_TXSTOP:                             "); 
                asm("   ljmp       $+1                              "); 
                asm("   nop                                         "); 
                asm("   bsf        " ___mkstr(INDF0) ", " ___mkstr(MCOMM_UART_SOFT_TXPIN));
                asm("   fcall      MCOMM_UART_BITDELAY              "); 
                asm("   return                                      "); 
                asm("MCOMM_UART_BITDELAY:                           "); 
                asm("   movlw      0x04                             "); 
                asm("   sublw      " ___mkstr(MCOMM_UART_FWBRG_VALUE)); 
                asm("   movwf      _mComm_delayCount                "); 
                asm("MCOMM_UART_BITWAITLOOP:                        "); 
                asm("   nop                                         "); 
                asm("MCOMM_UART_BITWAIT:                            "); 
                asm("   decfsz     _mComm_delayCount, F             "); 
                asm("   ljmp       MCOMM_UART_BITWAITLOOP           "); 
                asm("   ljmp       $+1                              "); 
                asm("   return                                      "); 
                #elif defined(_PIC14)       // Implementation if using a non-enhanced core PIC12/16F device                                          
                asm("   movlw LOW  "   ___mkstr(NOBANK(MCOMM_UART_SOFT_TXPORT))); 
                asm("   movwf      "   ___mkstr(FSR)                 ); 
                asm("   BANKSEL    _mComm_shiftReg                  "); 
                asm("   movlw      8                                "); 
                asm("   movwf      _mComm_bitCount                  "); 
                asm("   bcf " ___mkstr(INDF) ", " ___mkstr(MCOMM_UART_SOFT_TXPIN));                                          
                asm("   fcall      MCOMM_UART_BITDELAY              "); 
                asm("   ljmp       $+1                              "); 
                asm("   nop                                         "); 
                asm("MCOMM_UART_TXNEXTBIT:                          "); 
                asm("   rrf    _mComm_shiftReg, F                   "); 
                asm("   btfss  "   ___mkstr(NOBANK(STATUS))    ", 0"); 
                asm("   ljmp       MCOMM_UART_TXZERO                "); 
                asm("MCOMM_UART_TXONE:                              "); 
                asm("   nop                                         "); 
                asm("   bsf " ___mkstr(INDF) ", " ___mkstr(MCOMM_UART_SOFT_TXPIN));                      
                asm("   fcall      MCOMM_UART_BITDELAY              "); 
                asm("   decfsz     _mComm_bitCount, F        "); 
                asm("   ljmp       MCOMM_UART_TXNEXTBIT             "); 
                asm("   ljmp       MCOMM_UART_TXSTOP                "); 
                asm("MCOMM_UART_TXZERO:                             "); 
                asm("   bcf " ___mkstr(INDF) ", " ___mkstr(MCOMM_UART_SOFT_TXPIN));                          
                asm("   fcall      MCOMM_UART_BITDELAY              "); 
                asm("   decfsz     _mComm_bitCount, F        "); 
                asm("   ljmp       MCOMM_UART_TXNEXTBIT             "); 
                asm("   ljmp       MCOMM_UART_TXSTOP                "); 
                asm("MCOMM_UART_TXSTOP:                             "); 
                asm("   ljmp       $+1                              "); 
                asm("   nop                                         "); 
                asm("   bsf " ___mkstr(INDF) ", " ___mkstr(MCOMM_UART_SOFT_TXPIN));                            
                asm("   fcall      MCOMM_UART_BITDELAY              "); 
                asm("   return                                      "); 
                asm("MCOMM_UART_BITDELAY:                           "); 
                asm("   movlw      4                                "); 
                asm("   sublw  "   ___mkstr(MCOMM_UART_FWBRG_VALUE)  ); 
                asm("   movwf      _mComm_delayCount                "); 
                asm("MCOMM_UART_BITWAITLOOP:                        "); 
                asm("   nop                                         "); 
                asm("MCOMM_UART_BITWAIT:                            "); 
                asm("   decfsz     _mComm_delayCount, F             "); 
                asm("   ljmp       MCOMM_UART_BITWAITLOOP           "); 
                asm("   ljmp       $+1                              "); 
                asm("   return                                      "); 
                #endif
                
            #endif
        }
        
        #if (MCOMM_TYPE == MCOMM_UART_ONE_WAY)
        // Converts an 8-bit value into ASCII and then uses the mComm_UART_PutChar() 
        //  function to transmit it to the master. A delimiter character is added to
        //  the end automatically.
        void mComm_UART_Char2ASCII(uint8_t output)
        {
            #if (MCOMM_UART_1WAY_OUTPUT == MCOMM_UART_1WAY_HEX)
            
                uint8_t nibble;
            
                #define MCOMM_UART_OUTPUT_NIBBLE(value, shift)                      \
                    nibble = (uint8_t) value >> shift;                              \
                    if (nibble <= 9) { nibble += 0x30; } else { nibble += 0x37; }   \
                    mComm_UART_PutChar(nibble);
                    
                MCOMM_UART_OUTPUT_NIBBLE(output, 4);
                MCOMM_UART_OUTPUT_NIBBLE(output, 0);
                
                #undef MCOMM_UART_OUTPUT_NIBBLE
                
            #else
            
                uint8_t digit = 0;
                while (output >= 100)  { output -= 100; digit++; } mComm_UART_PutChar(digit + 0x30); digit = 0;
                while (output >=  10)  { output -=  10; digit++; } mComm_UART_PutChar(digit + 0x30); digit = 0;
                while (output >=   1)  { output -=   1; digit++; } mComm_UART_PutChar(digit + 0x30);
                
            #endif
            
            #if defined(MCOMM_UART_1WAY_OUT_GUIv1_1)
            mComm_UART_PutChar(',');
            #else
            mComm_UART_PutChar(MCOMM_UART_1WAY_DELIMITER);
            #endif
        }
        
        
        // Converts a 16-bit value into ASCII and then uses the mComm_UART_PutChar() 
        //  function to transmit it to the master. A delimiter character is added to
        //  the end automatically.
        void mComm_UART_Int2ASCII(uint16_t output)
        {
            
            #if (MCOMM_UART_1WAY_OUTPUT == MCOMM_UART_1WAY_HEX)
                
                uint8_t nibble;
            
                #define MCOMM_UART_OUTPUT_NIBBLE(value, shift)                      \
                    nibble = (uint8_t) value >> shift;                              \
                    if (nibble <= 9) { nibble += 0x30; } else { nibble += 0x37; }   \
                    mComm_UART_PutChar(nibble);
            
                MCOMM_UART_OUTPUT_NIBBLE(output, 12);
                MCOMM_UART_OUTPUT_NIBBLE(output, 8);
                MCOMM_UART_OUTPUT_NIBBLE(output, 4);
                MCOMM_UART_OUTPUT_NIBBLE(output, 0);
                
                #undef MCOMM_UART_OUTPUT_NIBBLE
                
            #else
            
                uint8_t digit = 0;
                while (output >= 10000) { output -= 10000; digit++; } mComm_UART_PutChar(digit + 0x30); digit = 0;
                while (output >=  1000) { output -=  1000; digit++; } mComm_UART_PutChar(digit + 0x30); digit = 0;
                while (output >=   100) { output -=   100; digit++; } mComm_UART_PutChar(digit + 0x30); digit = 0;
                while (output >=    10) { output -=    10; digit++; } mComm_UART_PutChar(digit + 0x30); digit = 0;
                while (output >=     1) { output -=     1; digit++; } mComm_UART_PutChar(digit + 0x30);
                
            #endif
            
            mComm_UART_PutChar(MCOMM_UART_1WAY_DELIMITER);
        }
        
        #endif
        
        
    #endif
    
#endif