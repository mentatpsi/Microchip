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
#if defined(MCOMM_TWO_WAY_ENABLED)

    //========================================================================
    // GLOBAL VARIABLES
    //========================================================================
        const   mComm_Opcode        mComm_opcode[MCOMM_NUMBER_OPCODES]  =   MCOMM_OPCODE_VALUES;
    
        //  mComm Opcode Array:
        //
        //  OPCODE      POINTER TO INITIALIZER      POINTER TO ITERATOR
        //  ------------------------------------------------------------------
        //  00          mComm_processRAM()          mComm_ramWriteIterator()    // RAM Write
        //  01          mComm_processRAM()          mComm_ramReadIterator()     // RAM Read
        //  02          mComm_processNVM()          mComm_nvmWriteIterator()    // EEPROM Write
        //  03          mComm_processNVM()          mComm_nvmReadIterator()     // EEPROM Read
        //  04          mComm_processStream()       mComm_ramWriteIterator()    // Stream Write
        //  05          mComm_processStream()       mComm_StreamReadIterator()  // Stream Read
        //  06          mComm_CustomProcess()       mComm_ramWriteIterator()    // Custom Write-to-RAM Function
        //  07          mComm_CustomProcess()       mComm_ramReadIterator()     // Custom Read-from-RAM Function
        
        
        const   uint8_t             mComm_ConfigBlock_Literals[MCOMM_CONFIG_LIT_SIZE]   = MCOMM_CONFIG_LIT_INIT;
        uint8_t * const             mComm_ConfigBlock_Addrs[MCOMM_CONFIG_ADDR_SIZE]     = MCOMM_CONFIG_ADDR_INIT;

    #if defined(MCOMM_ENABLE_STREAM)
        #if defined(MCOMM_STREAM_STORED_IN_RAM)
                mComm_Vector        mComm_stream[MCOMM_STREAM_SIZE + 1] = MCOMM_STREAM_VALUES;
        #else
        const   mComm_Vector        mComm_stream[MCOMM_STREAM_SIZE + 1] = MCOMM_STREAM_VALUES;
        #endif
        mComm_StreamConfig          mComm_streamConfig;
    #endif
    
    //========================================================================
    // FUNCTION PROTOTYPES
    //========================================================================
    
    #if defined(MCOMM_ENABLE_NVM_ACCESS)
    void    mComm_processNVM            (void);     // Initialize for NVM access
    uint8_t mComm_nvmReadIterator       (void);     // Non-Volatile Memory      - Read Iterator
    uint8_t mComm_nvmWriteIterator      (void);     // Non-Volatile Memory      - Write Iterator
    #endif
    
    void    mComm_processRAM            (void);     // Initialize for RAM access
    uint8_t mComm_ramReadIterator       (void);     // RAM Memory               - Read Iterator 
    uint8_t mComm_ramWriteIterator      (void);     // RAM Memory               - Write Iterator
    
    #if defined(MCOMM_ENABLE_STREAM)
    void    mComm_processStream         (void);     // Initialize for Stream access
    uint8_t mComm_StreamReadIterator    (void);     // Streamed List            - Read Iterator
    #endif
    
    void    mComm_ErrorProcess          (void);
    uint8_t mComm_ErrorIterator         (void);

//=============================================================================================
//  _   ___     ____  __ 
// | \ | \ \   / /  \/  |
// |  \| |\ \ / /| |\/| |   
// | |\  | \ V / | |  | |
// |_| \_|  \_/  |_|  |_|   Non-Volatile Memory
//
//=============================================================================================

    #define MCOMM_INPUT_ADDRL_OFFSET        1
    #define MCOMM_INPUT_ADDRH_OFFSET        2
    #define MCOMM_INPUT_LENGTH_OFFSET       3
    #define MCOMM_INPUT_PAYLOAD_OFFSET      4
    
    #if defined(MCOMM_ENABLE_NVM_ACCESS)

    void mComm_processNVM(void)
    {
        // Inititalize the output vector with the read/write request
        mComm_output.vector.pointer = (uint8_t*) mComm_input.buffer[MCOMM_INPUT_ADDRL_OFFSET];  // Supports 256byte EEPROMs only
        mComm_output.vector.length  = mComm_input.buffer[MCOMM_INPUT_LENGTH_OFFSET];
    }
    
    uint8_t mComm_nvmWriteIterator(void)
    {        
        uint8_t address = (uint8_t)( mComm_output.vector.pointer) + mComm_output.counter;

        #if defined(MTOUCH_EEPROM_ENABLED)
        if ((uint16_t) mComm_output.vector.pointer == 0)    // Special case: Writing to address 0x0000
        {                                                   //  resets the mTouch EEPROM space to 
            mTouch_EEPROM_Reset();                          //  compile-time values.
            mComm_output.flags.bits.hasNext = 0;
        }
        else
        {
        #endif
            if (mComm_output.vector.length & 0x01)
            {
                #if !defined(MTOUCH_EEPROM_STORED_IN_RAM)
                eeprom_write(address,   mComm_input.buffer[mComm_output.counter + MCOMM_INPUT_PAYLOAD_OFFSET]   );
                #else
                mTouch_configOptions[address] = mComm_input.buffer[mComm_output.counter + MCOMM_INPUT_PAYLOAD_OFFSET];
                #endif

                mComm_output.counter++;
            }
            else
            {
                #if !defined(MTOUCH_EEPROM_STORED_IN_RAM)
                eeprom_write(address,   mComm_input.buffer[mComm_output.counter + MCOMM_INPUT_PAYLOAD_OFFSET] );
                eeprom_write(address+1, mComm_input.buffer[mComm_output.counter + MCOMM_INPUT_PAYLOAD_OFFSET + 1] );
                #else
                mTouch_configOptions[address]   = mComm_input.buffer[mComm_output.counter + MCOMM_INPUT_PAYLOAD_OFFSET      ];
                mTouch_configOptions[address+1] = mComm_input.buffer[mComm_output.counter + MCOMM_INPUT_PAYLOAD_OFFSET + 1  ];
                #endif
                
                mComm_output.counter    = mComm_output.counter + 2;
            }
            
            if ( mComm_output.counter >= mComm_output.vector.length)
            {
                mComm_output.flags.bits.hasNext = 0;
            }
        #if defined(MTOUCH_EEPROM_ENABLED)
        }
        #endif
        return mComm_output.flags.bits.hasNext;
    }

    uint8_t mComm_nvmReadIterator(void)
    {
        // If this is the first output byte of the payload, send
        // the length, adding one for the checksum generated by
        // the protocol.
        if (mComm_output.flags.bits.first)
        {            
            mComm_output.flags.bits.first  = 0;
            #if defined(MCOMM_UART_IMPLEMENTED)
            mComm_output.flags.bits.second = 1;
            return (mComm_output.vector.length) + 2;
            #else
            return (mComm_output.vector.length) + 1;
            #endif
        }
        #if defined(MCOMM_UART_IMPLEMENTED)
        else if (mComm_output.flags.bits.second)
        {
            mComm_output.flags.bits.second = 0;
            return mComm_output.opcode;
        }
        #endif
        else
        {
            uint8_t data;

            if (mComm_output.flags.bits.high)
            {
                data = mComm_output.latch;
            }
            else
            {
                // Calculate the address of the data to read.
                data = (uint8_t)(mComm_output.vector.pointer) + mComm_output.counter;
                
                //mComm_output.flags.bits.gieLatch   = INTCONbits.GIE;
                //INTCONbits.GIE = 0;                                 // Start critical section
                
                #if !defined(MTOUCH_EEPROM_STORED_IN_RAM)
                mComm_output.latch      = eeprom_read(data + 1);    // Compiler-provided function
                data                    = eeprom_read(data    );
                #else
                mComm_output.latch      = mTouch_configOptions[data + 1];    // Compiler-provided function
                data                    = mTouch_configOptions[data];
                #endif
                
                //INTCONbits.GIE = mComm_output.flags.bits.gieLatch;  // End critical section
            }
            mComm_output.flags.bits.high ^= 1;
            
            // Once we've output all of the data, flip the 'hasNext'
            // flag to stop the loop.
            mComm_output.counter++;        
            if (mComm_output.counter >= mComm_output.vector.length)
            {
                mComm_output.flags.bits.hasNext = 0;
            }
            
            return data;
        }
    }

    #endif
    


//=============================================================================================
//  ____      _    __  __ 
// |  _ \    / \  |  \/  |
// | |_) |  / _ \ | |\/| |
// |  _ <  / ___ \| |  | |
// |_| \_\/_/   \_\_|  |_|      Random Access Memory
//
//=============================================================================================
    void mComm_processRAM(void)
    {
        mComm_output.vector.pointer = (uint8_t*) ((mComm_input.buffer[MCOMM_INPUT_ADDRH_OFFSET] << 8) + mComm_input.buffer[MCOMM_INPUT_ADDRL_OFFSET]);
        mComm_output.vector.length  = mComm_input.buffer[MCOMM_INPUT_LENGTH_OFFSET];
        
        // Reading from RAM address 0? That's a special case for reading from the 'constant' configuration block.
        // Redirect the pointer to the constant array and set the length to 
        if ((uint16_t) mComm_output.vector.pointer == 0) 
        {
            mComm_output.vector.length  = MCOMM_CONFIG_LIT_SIZE;        // Literal array length
        } 
        else if ((uint16_t) mComm_output.vector.pointer == 1)
        {
            mComm_output.vector.length  = MCOMM_CONFIG_ADDR_SIZE * 2;   // Address array length * Length of each address
        }
    }


    uint8_t mComm_ramReadIterator(void)     
    {
        // If this is the first output byte of the payload, send
        // the length, adding one for the checksum generated by
        // the protocol.
        if (mComm_output.flags.bits.first)
        {            
            mComm_output.flags.bits.first  = 0;
            #if defined(MCOMM_UART_IMPLEMENTED)
            mComm_output.flags.bits.second = 1;
            return (mComm_output.vector.length) + 2;
            #else
            return (mComm_output.vector.length) + 1;
            #endif
        }
        #if defined(MCOMM_UART_IMPLEMENTED)
        else if (mComm_output.flags.bits.second)
        {
            mComm_output.flags.bits.second = 0;
            mComm_output.flags.bits.high   = 0;
            return mComm_output.opcode;
        }
        #endif
        else
        {
            uint8_t data;

            if (mComm_output.flags.bits.high)
            {
                data = mComm_output.latch;
            }
            else
            {
                //mComm_output.flags.bits.gieLatch   = INTCONbits.GIE;
                //INTCONbits.GIE = 0;                         // Start critical section
                
                if ((uint16_t) mComm_output.vector.pointer == 0) 
                {
                    data                = mComm_ConfigBlock_Literals[mComm_output.counter  ];
                    mComm_output.latch  = mComm_ConfigBlock_Literals[mComm_output.counter+1];
                }
                else if ((uint16_t) mComm_output.vector.pointer == 1) 
                {
                    uint8_t*    address = mComm_ConfigBlock_Addrs   [mComm_output.counter >> 1];
                    
                    data                = (uint8_t)(address);
                    mComm_output.latch  = (uint8_t)((uint16_t)(address) >> 8);
                }
                else
                {
                    data                = mComm_output.vector.pointer[mComm_output.counter  ];
                    mComm_output.latch  = mComm_output.vector.pointer[mComm_output.counter+1];
                }
                
                //INTCONbits.GIE = mComm_output.flags.bits.gieLatch;     // End critical section
            }
            mComm_output.flags.bits.high ^= 1;
        
            mComm_output.counter++;
            if (mComm_output.counter >= mComm_output.vector.length)
            {
                mComm_output.flags.bits.hasNext = 0;
            }
            
            return data;
        }
    }

    //
    // mComm_ramWriteIterator
    //
    // Iterates through the input buffer, storing data each time the function is called.
    //
    //  If the total requested length is divisible by 2, it assumes the data is 16-bit
    //  integers and stores two bytes at a time to avoid possible data corruption issues.
    //
    //  If the total requested length is an odd number, the data is assumed to be 8-bit
    //  and is stored one byte at a time.
    //    
    uint8_t mComm_ramWriteIterator(void)
    {    
        //if (mComm_output.vector.length & 0x01)
        //{
            // Write to RAM Location
            mComm_output.vector.pointer[mComm_output.counter    ] = mComm_input.buffer[mComm_output.counter + MCOMM_INPUT_PAYLOAD_OFFSET];

            mComm_output.counter++;
        //}
        //else
        //{
            // Write two bytes to the given RAM location
            //
            // This line of code, step by step:
            // 
            // 1. The address being pointed to by the output vector...
            // 2. ...is to be treated like the address to a 16-bit int...
            // 3. ...and we want to load a 16-bit value to this address.
            // 4. The value is located at the input buffer
            // 5. But the input buffer is an 8-bit array, so we make the compiler treat it like a 16-bit value instead.
            //
            // 1.              &mComm_output.vector.pointer[mComm_output.counter]  =
            // 2.   (uint16_t*)&mComm_output.vector.pointer[mComm_output.counter]  =
            // 3. (*(uint16_t*)&mComm_output.vector.pointer[mComm_output.counter]) =
            // 4.                                                                  =            mComm_input.buffer[mComm_output.counter + MCOMM_INPUT_PAYLOAD_OFFSET]
            // 5.                                                                  = (uint16_t) mComm_input.buffer[mComm_output.counter + MCOMM_INPUT_PAYLOAD_OFFSET]
            //
            // Final result: Take the next two values in the input buffer and store them in the location pointed to by the output vector.
            //               Thus, we avoid creating an actual pointer and halve the instructions required to execute this operation.
            // Instruction count: 19
            //(*(uint16_t*)&mComm_output.vector.pointer[mComm_output.counter]) = (uint16_t) mComm_input.buffer[mComm_output.counter + MCOMM_INPUT_PAYLOAD_OFFSET];
            
            // NOTE: Other possible implementations...
            // 1. 33 instructions:
            //      mComm_output.vector.pointer[mComm_output.counter    ] = mComm_input.buffer[mComm_output.counter + MCOMM_INPUT_PAYLOAD_OFFSET];
            //      mComm_output.vector.pointer[mComm_output.counter + 1] = mComm_input.buffer[mComm_output.counter + MCOMM_INPUT_PAYLOAD_OFFSET + 1];
            // 2. 23 instructions + 1 temporary pointer variable:
            //      uint16_t* tempPointer = (uint16_t*) &mComm_output.vector.pointer[mComm_output.counter];
            //      (*tempPointer) = (uint16_t) mComm_input.buffer[mComm_output.counter + MCOMM_INPUT_PAYLOAD_OFFSET];
            
            //mComm_output.counter    = mComm_output.counter + 2;
        //}
        
        if (mComm_output.counter >= mComm_output.vector.length)
        {
            mComm_output.flags.bits.hasNext = 0;
        }
        return mComm_output.flags.bits.hasNext;
    }
    

//=============================================================================================
//  ____  _                            
// / ___|| |_ _ __ ___  __ _ _ __ ___  
// \___ \| __| '__/ _ \/ _` | '_ ` _ \ 
//  ___) | |_| | |  __/ (_| | | | | | |
// |____/ \__|_|  \___|\__,_|_| |_| |_|     Streaming Feature
//
//=============================================================================================

    #if defined(MCOMM_ENABLE_STREAM)
    
    //
    // mComm_processStream
    //
    // Initializes the output buffer for either a read or write command.
    //
    //  Input Buffer:
    //  Byte0 = Opcode (Read / Write)
    //  Byte1 = Which vector to start with
    //  Byte2 = n/a
    //  Byte3 = Length (used only for write, overwritten for read)
    //
    //  Note: The ramWriteIterator() is used to write to the stream's vector array. The
    //        ram write iterator will place the payload values into the address being
    //        pointed to by the output vector.
    //
    void mComm_processStream(void)
    {
        mComm_streamConfig.index    = 0;    // Vector to start with
        mComm_output.vector.pointer = mComm_stream[mComm_streamConfig.index].pointer;
    }

    //
    // mComm_StreamReadIterator
    //
    // Outputs the next byte of the 'read stream' packet. The 'stream' is an array of vectors
    // (a pointer + a length). To get the next byte, the iterator function accesses the pointer
    // within the current vector and grabs the Nth value until N is incremented to equal the
    // length value of the vector. When this happens, the current vector is incremented. It
    // continues until the vector it is pointing to has a length of 0. When this happens, the
    // hasNext flag is cleared to stop the iteration.
    //
    //  Call Number:        Outputs:
    //      1               Packet Length (including mComm's automatic checksum)
    //      2               'Read Stream' Opcode
    //      3               First byte of the RAM location pointed to by the first vector
    //      4               ...
    //
    uint8_t mComm_StreamReadIterator()
    {
        // If this is the first output byte of the payload, send
        // the length, adding one for the checksum generated by
        // the protocol.
        if (mComm_output.flags.bits.first)
        {            
            uint8_t i       = 0;
            uint8_t length  = 0;
            uint8_t count   = 0;
            
            mComm_output.flags.bits.high   = 0;
            mComm_output.flags.bits.first  = 0;
            #if defined(MCOMM_UART_IMPLEMENTED)
            mComm_output.flags.bits.second = 1;
            #endif
            
            mComm_output.vector.length = mComm_stream[mComm_streamConfig.index].length;
            
            do {
                length = mComm_stream[i].length;
                count += length;
                i++;
            } while (length > 0);
            #if defined(MCOMM_UART_IMPLEMENTED)
            return count + 2;
            #else
            return count + 1;
            #endif
        }
        
        #if defined(MCOMM_UART_IMPLEMENTED)
        else if (mComm_output.flags.bits.second)
        {
            mComm_output.flags.bits.second = 0;
            
            if (mComm_output.vector.length == 0)
            {
                mComm_output.flags.bits.hasNext = 0;
            }
            
            return mComm_output.opcode;                     
        }
        #endif
        
        else
        {            
            uint8_t data;
        
            if (mComm_output.flags.bits.high)
            {
                data = mComm_output.latch;
            }
            else
            {
                //mComm_output.flags.bits.gieLatch   = INTCONbits.GIE;
                //INTCONbits.GIE = 0;                         // Start critical section
                
                data                = mComm_output.vector.pointer[mComm_output.counter  ];
                mComm_output.latch  = mComm_output.vector.pointer[mComm_output.counter+1];
                
                //INTCONbits.GIE = mComm_output.flags.bits.gieLatch;     // End critical section
            }
            mComm_output.flags.bits.high ^= 1;
        
            mComm_output.counter++;
            if (mComm_output.counter >= mComm_output.vector.length)
            {
                mComm_streamConfig.index++;
                mComm_output.flags.bits.high    = 0;
                mComm_output.counter            = 0;
                mComm_output.vector.pointer     = mComm_stream[mComm_streamConfig.index].pointer;
                mComm_output.vector.length      = mComm_stream[mComm_streamConfig.index].length;
                
                if ((mComm_output.vector.length == 0) || (mComm_streamConfig.index >= MCOMM_STREAM_SIZE))
                {
                    mComm_output.flags.bits.hasNext = 0;
                }
            }
            
            return data;
        }
    }
    
    #endif

    /*********************************************************************************
    *  _   _      _                   _____                 _   _                 
    * | | | | ___| |_ __   ___ _ __  |  ___|   _ _ __   ___| |_(_) ___  _ __  ___ 
    * | |_| |/ _ \ | '_ \ / _ \ '__| | |_ | | | | '_ \ / __| __| |/ _ \| '_ \/ __|
    * |  _  |  __/ | |_) |  __/ |    |  _|| |_| | | | | (__| |_| | (_) | | | \__ \
    * |_| |_|\___|_| .__/ \___|_|    |_|   \__,_|_| |_|\___|\__|_|\___/|_| |_|___/
    *              |_|                                              
    *********************************************************************************/
    // These 'error' functions are used if an unimplemented opcode is requested.
    uint8_t mComm_ErrorIterator(void)
    {
        mComm_output.flags.bits.hasNext = 0;
        return 0x00;
    }
    void mComm_ErrorProcess(void)
    {
        return;
    }
    
#endif
#endif