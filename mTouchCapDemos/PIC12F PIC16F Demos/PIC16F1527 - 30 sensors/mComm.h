/*************************************************************************
 *  © 2012 Microchip Technology Inc.                                       
 *  
 *  Project Name:    mTouch Framework v2.3
 *  FileName:        mComm.h
 *  Dependencies:    mComm_config.h
 *
 *  Processor:       See documentation for supported PIC® microcontrollers 
 *  Compiler:        HI-TECH Ver. 9.81 or later
 *  IDE:             MPLAB® IDE v8.50 (or later) or MPLAB® X                        
 *  Hardware:         
 *  Company:         
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *  Description:     mTouch Framework Include File
 *                   - No application code should be implemented in this
 *                     or any other mTouch framework file. This will allow
 *                     for better customer support and easier upgrades to
 *                     later firmware versions. Use the main.c and user-
 *                     generated files to implement your application.
 *                   - See the documentation located in the docs/ folder
 *                     for a more information about how the framework is
 *                     implemented.
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
//=============================================================================================
/** @file   mComm.h
*   @brief  
*/
#ifndef __MCOMM_H
#define __MCOMM_H

//================================================================================================
//  ___            _           _           
// |_ _|_ __   ___| |_   _  __| | ___  ___ 
//  | || '_ \ / __| | | | |/ _` |/ _ \/ __|
//  | || | | | (__| | |_| | (_| |  __/\__ \
// |___|_| |_|\___|_|\__,_|\__,_|\___||___/
//                                                                    
//================================================================================================
    #if defined(HI_TECH_C)
        #include <htc.h>                // Provides PIC microcontroller SFR information
        #include <stdint.h>             // Provides "uint#_t" typedefs for each compiler
    #else
        #include <p18cxxx.h>
    #endif

    #include "mTouch.h"                 
    #include "mComm_config.h"           // Stores the mComm configuration details
    #include "mComm_processConfiguration.h"

    
//================================================================================================
//  ____  _                   _       
// / ___|| |_ _ __ _   _  ___| |_ ___ 
// \___ \| __| '__| | | |/ __| __/ __|
//  ___) | |_| |  | |_| | (__| |_\__ \
// |____/ \__|_|   \__,_|\___|\__|___/
//                                
//================================================================================================
    typedef struct 
    {   
        uint8_t*    pointer;
        uint8_t     length;
    } mComm_Vector;                     ///< Stores a pointer and a length. Used for pointing to arrays data.

    typedef struct 
    {   
        union
        {
            uint8_t     all;
            
            struct
            {
                unsigned        first   :1;     ///< '1' if this is the first byte being output
                #if defined(MCOMM_UART_IMPLEMENTED)
                unsigned        second  :1;     ///< '1' if this is the second byte being output
                #endif
                unsigned        hasNext :1;     ///< '1' if this output vector has more data to send
                unsigned        high    :1;     ///< '1' if this is the high byte of an integer value
                unsigned        gieLatch:1;     ///< Temporary latch of the current GIE value to restore it
            } bits;
            
        } flags;                        ///< Output state flags
        
        uint8_t         counter;        ///< Counter value used as 'i' to track current position
        uint8_t         latch;          ///< Temporary output latch storage
        uint8_t         opcode;         ///< Temporary opcode storage
        uint8_t         checksum;       ///< Temporary checksum calculation storage
        
        uint8_t (*iterator)(void);      ///< Global function pointer used by mComm to query for the output stream
        
        mComm_Vector    vector;         ///< Global current output vector to quickly access data
    } mComm_OutputVector;               ///< Stores the variables for the current output data
    
    typedef struct 
    {       
        uint8_t         index;
        uint8_t         counter;
        uint8_t         checksum;
        #if defined(MCOMM_UART_IMPLEMENTED) || defined(MCOMM_SPI_IMPLEMENTED)
        uint8_t         state;
        #endif
        
        union
        {
            uint8_t     all;
            struct
            {
                unsigned        full:1;                     ///< '1' if the input buffer is full and ready for processing
                
                #if defined(MCOMM_UART_IMPLEMENTED)
                unsigned        invalid:1;                  ///< '1' if the data packet had an invalid checksum
                #endif
                
                #if defined(MCOMM_I2C_IMPLEMENTED)
                unsigned        write:1;                    ///< '1' if this is a write command
                #endif
                
                #if defined(MCOMM_SPI_IMPLEMENTED)
                unsigned        startPacket:1;
                unsigned        ssLatch:1;                  
                #endif
                
            } bits;
        } flags;
        
        uint8_t         buffer[MCOMM_INPUTBUFFER_SIZE];     ///< Stores the input packet data
        
    } mComm_InputBuffer;                ///< Stores the data input by the master
    
    typedef struct 
    {   
        void (*initialize)(void);       ///< Points to the function which will initialize the iterator and output structure for the new command from the master
        uint8_t (*iterator)(void);      ///< Points to the function which will perform the iteration over the data being processed.
    } mComm_Opcode;                     ///< Defines the array of function pointers for each opcode
    
    typedef struct 
    {   
        unsigned    enabled:1;          ///< If '1', the stream is enabled
        unsigned    go:1;               ///< If '1', the stream will attempt to output
        unsigned    empty:6;
        
        uint8_t     index;              ///< The index of the current stream vector
    } mComm_StreamConfig;               ///< Defines the data used to track the state of the stream
    
    
//================================================================================================
//  _____                 _   _                            ____ _       _           _ 
// |  ___|   _ _ __   ___| |_(_) ___  _ __  ___           / ___| | ___ | |__   __ _| |
// | |_ | | | | '_ \ / __| __| |/ _ \| '_ \/ __|  _____  | |  _| |/ _ \| '_ \ / _` | |
// |  _|| |_| | | | | (__| |_| | (_) | | | \__ \ |_____| | |_| | | (_) | |_) | (_| | |
// |_|   \__,_|_| |_|\___|\__|_|\___/|_| |_|___/          \____|_|\___/|_.__/ \__,_|_| 
//
//================================================================================================
    void    mComm_Init                  (void);     // Initializes mComm and its hardware module
    
    #if defined(MCOMM_TWO_WAY_ENABLED)
    void    mComm_Receive               (void);     // ISR Receive Function
    #endif
    
    #if defined(MCOMM_UART_IMPLEMENTED)
    void    mComm_Service               (void);     // Output servicing for UART
    #endif
    
    #if defined(MCOMM_TWO_WAY_ENABLED)
        void    mComm_processRAM            (void); // Initialize for RAM access
        uint8_t mComm_ramReadIterator       (void); // RAM Memory               - Read Iterator 
        uint8_t mComm_ramWriteIterator      (void); // RAM Memory               - Write Iterator
        
        #if defined(MCOMM_ENABLE_NVM_ACCESS)
        void    mComm_processNVM            (void); // Initialize for NVM access
        uint8_t mComm_nvmReadIterator       (void); // Non-Volatile Memory      - Read Iterator
        uint8_t mComm_nvmWriteIterator      (void); // Non-Volatile Memory      - Write Iterator
        #endif
        
        #if defined(MCOMM_ENABLE_STREAM)
        void    mComm_processStream         (void); // Initialize for Stream access
        uint8_t mComm_StreamReadIterator    (void); // Streamed List            - Read Iterator
        #endif
        
        void    mComm_ErrorProcess          (void); // Opcode not found - Error behavior definition
        uint8_t mComm_ErrorIterator         (void); // Opcode not found - Error behavior definition
        
        #if defined(MCOMM_ENABLE_CUSTOM_OPCODE)
        uint8_t mComm_CustomCallback(void);
        void    mComm_CustomProcess(void);
        uint8_t mComm_CustomWriteIterator(void);
        #endif
    #endif
    
//================================================================================================
// __     __         _       _     _                      ____ _       _           _ 
// \ \   / /_ _ _ __(_) __ _| |__ | | ___  ___           / ___| | ___ | |__   __ _| |
//  \ \ / / _` | '__| |/ _` | '_ \| |/ _ \/ __|  _____  | |  _| |/ _ \| '_ \ / _` | |
//   \ V / (_| | |  | | (_| | |_) | |  __/\__ \ |_____| | |_| | | (_) | |_) | (_| | |
//    \_/ \__,_|_|  |_|\__,_|_.__/|_|\___||___/          \____|_|\___/|_.__/ \__,_|_|
// 
//================================================================================================
    #if defined(MCOMM_TWO_WAY_ENABLED)
    extern          mComm_OutputVector  mComm_output;                   // Output vector and flags
    extern          mComm_InputBuffer   mComm_input;                    // Input buffer and flags
    extern          mComm_StreamConfig  mComm_streamConfig;
    extern const    mComm_Opcode        mComm_opcode[MCOMM_NUMBER_OPCODES];
    extern const    uint8_t             mComm_ConfigBlock_Literals[MCOMM_CONFIG_LIT_SIZE]; 
    extern          uint8_t * const     mComm_ConfigBlock_Addrs[MCOMM_CONFIG_ADDR_SIZE];
    #if defined(MCOMM_STREAM_STORED_IN_RAM)
    extern          mComm_Vector        mComm_stream[MCOMM_STREAM_SIZE + 1];
    #else
    extern const    mComm_Vector        mComm_stream[MCOMM_STREAM_SIZE + 1];
    #endif
    #endif
    
    #if defined(MCOMM_ENABLE_CUSTOM_OPCODE)
    extern          uint8_t             mComm_GUICustomVariable;
    #endif
//================================================================================================
//  _____                           
// | ____|_ __  _   _ _ __ ___  ___ 
// |  _| | '_ \| | | | '_ ` _ \/ __|
// | |___| | | | |_| | | | | | \__ \
// |_____|_| |_|\__,_|_| |_| |_|___/
//                               
//================================================================================================
    enum    mComm_UART_States
            {
                MCOMM_IDLE,          ///< RX communications are idling
                MCOMM_START,         ///< New RX packet is starting
                MCOMM_BUFFER,        ///< RX packet is being stored in the buffer
                MCOMM_PROCESS_DATA   ///< RX packet is ready to be processed by the state machine
            };

    enum    mComm_SPI_States
            {
                MCOMM_SPI_IDLE,          ///< RX communications are idling
                MCOMM_SPI_START,         ///< New RX packet is starting
                MCOMM_SPI_PACKET_LENGTH, ///< RX byte is the packet length
                MCOMM_SPI_BUFFER,        ///< RX packet is being stored in the buffer
                MCOMM_SPI_PROCESS_DATA,  ///< RX packet is ready to be processed by the state machine
                MCOMM_SPI_OUTPUT
            };
            
            
#endif      // end: ifndef __MCOMM_H


