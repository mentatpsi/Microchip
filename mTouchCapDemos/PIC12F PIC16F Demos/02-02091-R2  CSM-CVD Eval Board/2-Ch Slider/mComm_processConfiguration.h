
#ifndef __MCOMM_PROCESS_CONFIGURATION_H
#define __MCOMM_PROCESS_CONFIGURATION_H

#if defined(MCOMM_ENABLED)
    
    #if !defined(MCOMM_TYPE)
        #error The MCOMM_TYPE value has not been defined in mComm_config.h. Please use this value to specify the type of communication to implement.
    #else
        #if     MCOMM_TYPE == MCOMM_UART_ONE_WAY
            #define MCOMM_ONE_WAY_ENABLED
        #elif   MCOMM_TYPE == MCOMM_UART_TWO_WAY
            #define MCOMM_TWO_WAY_ENABLED
        #elif   MCOMM_TYPE == MCOMM_I2C_TWO_WAY
            #define MCOMM_TWO_WAY_ENABLED
        #elif   MCOMM_TYPE == MCOMM_SPI_TWO_WAY
            #define MCOMM_TWO_WAY_ENABLED
        #endif
    #endif
    
    #if (MCOMM_TYPE == MCOMM_UART_ONE_WAY) || (MCOMM_TYPE == MCOMM_UART_TWO_WAY)
        #define MCOMM_UART_IMPLEMENTED
    #endif
    #if (MCOMM_TYPE == MCOMM_I2C_TWO_WAY)
        #define MCOMM_I2C_IMPLEMENTED
    #endif
    #if (MCOMM_TYPE == MCOMM_SPI_TWO_WAY)
        #define MCOMM_SPI_IMPLEMENTED
    #endif
    
    #if (MCOMM_TYPE == MCOMM_UART_ONE_WAY)
        #if !defined(MCOMM_UART_1WAY_MODULE)
            #warning The MCOMM_UART_1WAY_MODULE value has not been defined in mComm_config.h. Defaulting to a software implementation.
            #define MCOMM_UART_1WAY_MODULE MCOMM_UART_SOFTWARE_IMPLEMENTATION
        #endif
        #if !defined(MCOMM_UART_1WAY_OUTPUT)
            #warning The MCOMM_UART_1WAY_OUTPUT value has not been defined in mComm_config.h. Defaulting to decimal output.
            #define MCOMM_UART_1WAY_OUTPUT MCOMM_UART_1WAY_DECIMAL
        #endif
        #if !defined(MCOMM_UART_1WAY_DELIMITER)
            #warning The MCOMM_UART_1WAY_DELIMITER value has not been defined in mComm_config.h. Defaulting to ';'.
            #define MCOMM_UART_1WAY_DELIMITER ';'
        #endif
        
        #if (MCOMM_UART_1WAY_MODULE == MCOMM_UART_SOFTWARE_IMPLEMENTATION)
            #if !defined(MCOMM_UART_SOFT_TXPORT)
                #error The MCOMM_UART_SOFT_TXPORT value has not been defined in mComm_config.h. Please assign this to the port which will be used for the TX output pin. Ex: PORTA
            #endif
            #if !defined(MCOMM_UART_SOFT_TXTRIS)
                #error The MCOMM_UART_SOFT_TXTRIS value has not been defined in mComm_config.h. Please assign this to the tris which will be used for the TX output pin. Ex: TRISA
            #endif
            #if !defined(MCOMM_UART_SOFT_TXPIN)
                #error The MCOMM_UART_SOFT_TXPIN value has not been defined in mComm_config.h. Please assign this to the pin which will be used for the TX output pin. Ex: 5
            #endif
        #endif
    #endif
        
    #if defined(MCOMM_UART_IMPLEMENTED)
    
        #if !defined(PIC_UART_AVAILABLE)
            #if (MCOMM_TYPE != MCOMM_UART_ONE_WAY)
                #error Your chosen PIC microcontroller does not contain a UART module. Edit the MCOMM_TYPE value in mComm_config.h to continue or enable the software UART implementation.
            #endif
            #if (MCOMM_TYPE == MCOMM_UART_ONE_WAY) && (MCOMM_UART_1WAY_MODULE != MCOMM_UART_SOFTWARE_IMPLEMENTATION)
                #error Your chosen PIC microcontroller does not contain a UART module. Edit the MCOMM_TYPE value in mComm_config.h to continue or enable the software UART implementation.
            #endif
        #else
            #define MCOMM_UART_HARDWARE_USED
        #endif
        
        #if !defined(MCOMM_UART_BAUDRATE)
            #warning The MCOMM_UART_BAUDRATE value in mComm_config.h has not been defined. Defaulting to 38400bps.
            #define MCOMM_UART_BAUDRATE 38400
        #endif
        
        #if !defined(_XTAL_FREQ)
            #error mComm requires _XTAL_FREQ be defined to correctly initialize the UART module. Please #define it in the configuration file to the frequency of the oscillator in Hz.
        #endif
        
        #if !defined(PIC_UART_BAUD_BITS)
            #define PIC_UART_BAUD_BITS 8
        #endif
        
        #if PIC_UART_BAUD_BITS == 16
            #define MCOMM_UART_SPBRGH_AVAILABLE
        #endif
        
        #if _XTAL_FREQ == 32000000
            #if     MCOMM_UART_BAUDRATE == 1200
                #define MCOMM_UART_SPBRG_VALUE  3332
                #define MCOMM_UART_BRG16_VALUE  1
                #define MCOMM_UART_BRGH_VALUE   0
            #elif   MCOMM_UART_BAUDRATE == 2400
                #define MCOMM_UART_SPBRG_VALUE  207
                #define MCOMM_UART_BRG16_VALUE  0
                #define MCOMM_UART_BRGH_VALUE   0
            #elif   MCOMM_UART_BAUDRATE == 9600
                #define MCOMM_UART_SPBRG_VALUE  51
                #define MCOMM_UART_BRG16_VALUE  0
                #define MCOMM_UART_BRGH_VALUE   0
                #define MCOMM_UART_FWBRG_VALUE  208
            #elif   MCOMM_UART_BAUDRATE == 19200
                #define MCOMM_UART_SPBRG_VALUE  25
                #define MCOMM_UART_BRG16_VALUE  0
                #define MCOMM_UART_BRGH_VALUE   0
                #define MCOMM_UART_FWBRG_VALUE  104
            #elif   MCOMM_UART_BAUDRATE == 38400
                #define MCOMM_UART_SPBRG_VALUE  12
                #define MCOMM_UART_BRG16_VALUE  0
                #define MCOMM_UART_BRGH_VALUE   0
                #define MCOMM_UART_FWBRG_VALUE  52
            #elif   MCOMM_UART_BAUDRATE == 57600
                #define MCOMM_UART_SPBRG_VALUE  34
                #define MCOMM_UART_BRG16_VALUE  0
                #define MCOMM_UART_BRGH_VALUE   1
                #define MCOMM_UART_FWBRG_VALUE  35
            #elif   MCOMM_UART_BAUDRATE == 115200
                #define MCOMM_UART_SPBRG_VALUE  16
                #define MCOMM_UART_BRG16_VALUE  0
                #define MCOMM_UART_BRGH_VALUE   1
            #else
                #define MCOMM_UART_BADBAUDRATE
            #endif
        #elif _XTAL_FREQ == 20000000
            #if     MCOMM_UART_BAUDRATE == 1200
                #define MCOMM_UART_SPBRG_VALUE  255
                #define MCOMM_UART_BRG16_VALUE  0
                #define MCOMM_UART_BRGH_VALUE   0
            #elif   MCOMM_UART_BAUDRATE == 2400
                #define MCOMM_UART_SPBRG_VALUE  129
                #define MCOMM_UART_BRG16_VALUE  0
                #define MCOMM_UART_BRGH_VALUE   0
            #elif   MCOMM_UART_BAUDRATE == 9600
                #define MCOMM_UART_SPBRG_VALUE  32
                #define MCOMM_UART_BRG16_VALUE  0
                #define MCOMM_UART_BRGH_VALUE   0
            #elif   MCOMM_UART_BAUDRATE == 19200
                #define MCOMM_UART_SPBRG_VALUE  64
                #define MCOMM_UART_BRG16_VALUE  0
                #define MCOMM_UART_BRGH_VALUE   1
            #elif   MCOMM_UART_BAUDRATE == 38400
                #define MCOMM_UART_SPBRG_VALUE  31
                #define MCOMM_UART_BRG16_VALUE  0
                #define MCOMM_UART_BRGH_VALUE   1
            #elif   MCOMM_UART_BAUDRATE == 57600
                #define MCOMM_UART_SPBRG_VALUE  21
                #define MCOMM_UART_BRG16_VALUE  0
                #define MCOMM_UART_BRGH_VALUE   1
            #elif   MCOMM_UART_BAUDRATE == 115200
                #define MCOMM_UART_SPBRG_VALUE  10
                #define MCOMM_UART_BRG16_VALUE  0
                #define MCOMM_UART_BRGH_VALUE   1
            #else
                #define MCOMM_UART_BADBAUDRATE
            #endif
        #elif _XTAL_FREQ == 18432000
            #if     MCOMM_UART_BAUDRATE == 1200
                #define MCOMM_UART_SPBRG_VALUE  239
                #define MCOMM_UART_BRG16_VALUE  0
                #define MCOMM_UART_BRGH_VALUE   0
            #elif   MCOMM_UART_BAUDRATE == 2400
                #define MCOMM_UART_SPBRG_VALUE  119
                #define MCOMM_UART_BRG16_VALUE  0
                #define MCOMM_UART_BRGH_VALUE   0
            #elif   MCOMM_UART_BAUDRATE == 9600
                #define MCOMM_UART_SPBRG_VALUE  29
                #define MCOMM_UART_BRG16_VALUE  0
                #define MCOMM_UART_BRGH_VALUE   0
            #elif   MCOMM_UART_BAUDRATE == 19200
                #define MCOMM_UART_SPBRG_VALUE  14
                #define MCOMM_UART_BRG16_VALUE  0
                #define MCOMM_UART_BRGH_VALUE   0
            #elif   MCOMM_UART_BAUDRATE == 38400
                #define MCOMM_UART_SPBRG_VALUE  29
                #define MCOMM_UART_BRG16_VALUE  0
                #define MCOMM_UART_BRGH_VALUE   1
            #elif   MCOMM_UART_BAUDRATE == 57600
                #define MCOMM_UART_SPBRG_VALUE  7
                #define MCOMM_UART_BRG16_VALUE  0
                #define MCOMM_UART_BRGH_VALUE   0
            #elif   MCOMM_UART_BAUDRATE == 115200
                #define MCOMM_UART_SPBRG_VALUE  9
                #define MCOMM_UART_BRG16_VALUE  0
                #define MCOMM_UART_BRGH_VALUE   1
            #else
                #define MCOMM_UART_BADBAUDRATE
            #endif
        #elif _XTAL_FREQ == 16000000
            #if     MCOMM_UART_BAUDRATE == 1200
                #define MCOMM_UART_SPBRG_VALUE  207
                #define MCOMM_UART_BRG16_VALUE  0
                #define MCOMM_UART_BRGH_VALUE   0
            #elif   MCOMM_UART_BAUDRATE == 2400
                #define MCOMM_UART_SPBRG_VALUE  103
                #define MCOMM_UART_BRG16_VALUE  0
                #define MCOMM_UART_BRGH_VALUE   0
            #elif   MCOMM_UART_BAUDRATE == 9600
                #define MCOMM_UART_SPBRG_VALUE  25
                #define MCOMM_UART_BRG16_VALUE  0
                #define MCOMM_UART_BRGH_VALUE   0
                #define MCOMM_UART_FWBRG_VALUE  104
            #elif   MCOMM_UART_BAUDRATE == 19200
                #define MCOMM_UART_SPBRG_VALUE  12
                #define MCOMM_UART_BRG16_VALUE  0
                #define MCOMM_UART_BRGH_VALUE   0
                #define MCOMM_UART_FWBRG_VALUE  52
            #elif   MCOMM_UART_BAUDRATE == 38400
                #define MCOMM_UART_SPBRG_VALUE  25
                #define MCOMM_UART_BRG16_VALUE  0
                #define MCOMM_UART_BRGH_VALUE   1
                #define MCOMM_UART_FWBRG_VALUE  26
            #elif   MCOMM_UART_BAUDRATE == 57600
                #define MCOMM_UART_SPBRG_VALUE  16
                #define MCOMM_UART_BRG16_VALUE  0
                #define MCOMM_UART_BRGH_VALUE   1
            #elif   MCOMM_UART_BAUDRATE == 115200
                #define MCOMM_UART_SPBRG_VALUE  34
                #define MCOMM_UART_BRG16_VALUE  1
                #define MCOMM_UART_BRGH_VALUE   1
            #else
                #define MCOMM_UART_BADBAUDRATE
            #endif
        #elif _XTAL_FREQ == 11059200
            #if     MCOMM_UART_BAUDRATE == 1200
                #define MCOMM_UART_SPBRG_VALUE  143
                #define MCOMM_UART_BRG16_VALUE  0
                #define MCOMM_UART_BRGH_VALUE   0
            #elif   MCOMM_UART_BAUDRATE == 2400
                #define MCOMM_UART_SPBRG_VALUE  71
                #define MCOMM_UART_BRG16_VALUE  0
                #define MCOMM_UART_BRGH_VALUE   0
            #elif   MCOMM_UART_BAUDRATE == 9600
                #define MCOMM_UART_SPBRG_VALUE  17
                #define MCOMM_UART_BRG16_VALUE  0
                #define MCOMM_UART_BRGH_VALUE   0
            #elif   MCOMM_UART_BAUDRATE == 19200
                #define MCOMM_UART_SPBRG_VALUE  8
                #define MCOMM_UART_BRG16_VALUE  0
                #define MCOMM_UART_BRGH_VALUE   0
            #elif   MCOMM_UART_BAUDRATE == 38400
                #define MCOMM_UART_SPBRG_VALUE  17
                #define MCOMM_UART_BRG16_VALUE  0
                #define MCOMM_UART_BRGH_VALUE   1
            #elif   MCOMM_UART_BAUDRATE == 57600
                #define MCOMM_UART_SPBRG_VALUE  2
                #define MCOMM_UART_BRG16_VALUE  0
                #define MCOMM_UART_BRGH_VALUE   0
            #elif   MCOMM_UART_BAUDRATE == 115200
                #define MCOMM_UART_SPBRG_VALUE  5
                #define MCOMM_UART_BRG16_VALUE  0
                #define MCOMM_UART_BRGH_VALUE   1
            #else
                #define MCOMM_UART_BADBAUDRATE
            #endif
        #elif _XTAL_FREQ ==  8000000
            #if     MCOMM_UART_BAUDRATE == 1200
                #define MCOMM_UART_SPBRG_VALUE  103
                #define MCOMM_UART_BRG16_VALUE  0
                #define MCOMM_UART_BRGH_VALUE   0
            #elif   MCOMM_UART_BAUDRATE == 2400
                #define MCOMM_UART_SPBRG_VALUE  51
                #define MCOMM_UART_BRG16_VALUE  0
                #define MCOMM_UART_BRGH_VALUE   0
            #elif   MCOMM_UART_BAUDRATE == 9600
                #define MCOMM_UART_SPBRG_VALUE  12
                #define MCOMM_UART_BRG16_VALUE  0
                #define MCOMM_UART_BRGH_VALUE   0
                #define MCOMM_UART_FWBRG_VALUE  52
            #elif   MCOMM_UART_BAUDRATE == 19200
                #define MCOMM_UART_SPBRG_VALUE  25
                #define MCOMM_UART_BRG16_VALUE  0
                #define MCOMM_UART_BRGH_VALUE   1
                #define MCOMM_UART_FWBRG_VALUE  26
            #elif   MCOMM_UART_BAUDRATE == 38400
                #define MCOMM_UART_SPBRG_VALUE  12
                #define MCOMM_UART_BRG16_VALUE  0
                #define MCOMM_UART_BRGH_VALUE   1
                #define MCOMM_UART_FWBRG_VALUE  13
            #elif   MCOMM_UART_BAUDRATE == 57600
                #define MCOMM_UART_SPBRG_VALUE  8
                #define MCOMM_UART_BRG16_VALUE  0
                #define MCOMM_UART_BRGH_VALUE   1
            #elif   MCOMM_UART_BAUDRATE == 115200
                #define MCOMM_UART_SPBRG_VALUE  16
                #define MCOMM_UART_BRG16_VALUE  1
                #define MCOMM_UART_BRGH_VALUE   1
            #else
                #define MCOMM_UART_BADBAUDRATE
            #endif
        #elif _XTAL_FREQ ==  4000000
            #if     MCOMM_UART_BAUDRATE == 1200
                #define MCOMM_UART_SPBRG_VALUE  51
                #define MCOMM_UART_BRG16_VALUE  0
                #define MCOMM_UART_BRGH_VALUE   0
            #elif   MCOMM_UART_BAUDRATE == 2400
                #define MCOMM_UART_SPBRG_VALUE  25
                #define MCOMM_UART_BRG16_VALUE  0
                #define MCOMM_UART_BRGH_VALUE   0
            #elif   MCOMM_UART_BAUDRATE == 9600
                #define MCOMM_UART_SPBRG_VALUE  25
                #define MCOMM_UART_BRG16_VALUE  0
                #define MCOMM_UART_BRGH_VALUE   1
                #define MCOMM_UART_FWBRG_VALUE  26
            #elif   MCOMM_UART_BAUDRATE == 19200
                #define MCOMM_UART_SPBRG_VALUE  12
                #define MCOMM_UART_BRG16_VALUE  0
                #define MCOMM_UART_BRGH_VALUE   1
                #define MCOMM_UART_FWBRG_VALUE  13
            #elif   MCOMM_UART_BAUDRATE == 38400
                #define MCOMM_UART_UNREACHABLE
            #elif   MCOMM_UART_BAUDRATE == 57600
                #define MCOMM_UART_SPBRG_VALUE  16
                #define MCOMM_UART_BRG16_VALUE  1
                #define MCOMM_UART_BRGH_VALUE   1
            #elif   MCOMM_UART_BAUDRATE == 115200
                #define MCOMM_UART_SPBRG_VALUE  8
                #define MCOMM_UART_BRG16_VALUE  1
                #define MCOMM_UART_BRGH_VALUE   1
            #else
                #define MCOMM_UART_BADBAUDRATE
            #endif
        #elif _XTAL_FREQ ==  3686400
            #if     MCOMM_UART_BAUDRATE == 1200
                #define MCOMM_UART_SPBRG_VALUE  47
                #define MCOMM_UART_BRG16_VALUE  0
                #define MCOMM_UART_BRGH_VALUE   0
            #elif   MCOMM_UART_BAUDRATE == 2400
                #define MCOMM_UART_SPBRG_VALUE  23
                #define MCOMM_UART_BRG16_VALUE  0
                #define MCOMM_UART_BRGH_VALUE   0
            #elif   MCOMM_UART_BAUDRATE == 9600
                #define MCOMM_UART_SPBRG_VALUE  5
                #define MCOMM_UART_BRG16_VALUE  0
                #define MCOMM_UART_BRGH_VALUE   0
            #elif   MCOMM_UART_BAUDRATE == 19200
                #define MCOMM_UART_SPBRG_VALUE  2
                #define MCOMM_UART_BRG16_VALUE  0
                #define MCOMM_UART_BRGH_VALUE   0
            #elif   MCOMM_UART_BAUDRATE == 38400
                #define MCOMM_UART_SPBRG_VALUE  6
                #define MCOMM_UART_BRG16_VALUE  0
                #define MCOMM_UART_BRGH_VALUE   1
            #elif   MCOMM_UART_BAUDRATE == 57600
                #define MCOMM_UART_SPBRG_VALUE  0
                #define MCOMM_UART_BRG16_VALUE  0
                #define MCOMM_UART_BRGH_VALUE   0
            #elif   MCOMM_UART_BAUDRATE == 115200
                #define MCOMM_UART_SPBRG_VALUE  1
                #define MCOMM_UART_BRG16_VALUE  0
                #define MCOMM_UART_BRGH_VALUE   1
            #else
                #define MCOMM_UART_BADBAUDRATE
            #endif
        #elif _XTAL_FREQ ==  2000000
            #if     MCOMM_UART_BAUDRATE == 1200
                #define MCOMM_UART_SPBRG_VALUE  25
                #define MCOMM_UART_BRG16_VALUE  0
                #define MCOMM_UART_BRGH_VALUE   0
            #elif   MCOMM_UART_BAUDRATE == 2400
                #define MCOMM_UART_SPBRG_VALUE  12
                #define MCOMM_UART_BRG16_VALUE  0
                #define MCOMM_UART_BRGH_VALUE   0
            #elif   MCOMM_UART_BAUDRATE == 9600
                #define MCOMM_UART_SPBRG_VALUE  12
                #define MCOMM_UART_BRG16_VALUE  0
                #define MCOMM_UART_BRGH_VALUE   1
                #define MCOMM_UART_FWBRG_VALUE  13
            #elif   MCOMM_UART_BAUDRATE == 19200
                #define MCOMM_UART_SPBRG_VALUE  25
                #define MCOMM_UART_BRG16_VALUE  1
                #define MCOMM_UART_BRGH_VALUE   1
            #elif   MCOMM_UART_BAUDRATE == 38400
                #define MCOMM_UART_SPBRG_VALUE  12
                #define MCOMM_UART_BRG16_VALUE  1
                #define MCOMM_UART_BRGH_VALUE   1
            #elif   MCOMM_UART_BAUDRATE == 57600
                #define MCOMM_UART_SPBRG_VALUE  8
                #define MCOMM_UART_BRG16_VALUE  1
                #define MCOMM_UART_BRGH_VALUE   1
            #elif   MCOMM_UART_BAUDRATE == 115200
                #define MCOMM_UART_UNREACHABLE
            #else
                #define MCOMM_UART_BADBAUDRATE
            #endif
        #elif _XTAL_FREQ ==  1000000
            #if     MCOMM_UART_BAUDRATE == 1200
                #define MCOMM_UART_SPBRG_VALUE  12
                #define MCOMM_UART_BRG16_VALUE  0
                #define MCOMM_UART_BRGH_VALUE   0
            #elif   MCOMM_UART_BAUDRATE == 2400
                #define MCOMM_UART_SPBRG_VALUE  51
                #define MCOMM_UART_BRG16_VALUE  0
                #define MCOMM_UART_BRGH_VALUE   1
            #elif   MCOMM_UART_BAUDRATE == 9600
                #define MCOMM_UART_SPBRG_VALUE  25
                #define MCOMM_UART_BRG16_VALUE  1
                #define MCOMM_UART_BRGH_VALUE   1
            #elif   MCOMM_UART_BAUDRATE == 19200
                #define MCOMM_UART_SPBRG_VALUE  12
                #define MCOMM_UART_BRG16_VALUE  1
                #define MCOMM_UART_BRGH_VALUE   1
            #elif   MCOMM_UART_BAUDRATE == 38400
                #define MCOMM_UART_UNREACHABLE
            #elif   MCOMM_UART_BAUDRATE == 57600
                #define MCOMM_UART_UNREACHABLE
            #elif   MCOMM_UART_BAUDRATE == 115200
                #define MCOMM_UART_UNREACHABLE
            #else
                #define MCOMM_UART_BADBAUDRATE
            #endif
        #else
            #error Current _XTAL_FREQ value is not compatible with the predefined values in the mComm code.
        #endif
        
        #if defined(MCOMM_UART_BADBAUDRATE)
            #error Requested baud rate is not compatible with the predefined values in the mComm code.
        #endif
        
        #if defined(MCOMM_ONE_WAY_ENABLED) && (MCOMM_UART_1WAY_MODULE == MCOMM_UART_SOFTWARE_IMPLEMENTATION) 
        
            #if !defined(MCOMM_UART_FWBRG_VALUE)
                #error Requested baud rate is not a defined option for the software-implemented UART. Try 9600, 19200, or 38400 for the baud rate in mComm_config.h
            #endif
            
        #else
        
            #if defined(MCOMM_UART_BRG16_VALUE) && (MCOMM_UART_BRG16_VALUE == 1)
                #if PIC_UART_BAUD_BITS != 16
                    #error Requested baud rate is unreachable without a 16-bit baud rate generator which your PIC microcontroller does not have. Please redefine MCOMM_UART_BAUDRATE in mComm_config.h
                #endif
            #endif
            
            #if defined(MCOMM_UART_UNREACHABLE)
                #error Requested baud rate is unreachable given the current _XTAL_FREQ and module limitations.
            #endif
            
            #if MCOMM_UART_SPBRG_VALUE > 255
                #define MCOMM_UART_SPBRGL_VALUE     (uint8_t)(MCOMM_UART_SPBRG_VALUE & 0xFF)
                #define MCOMM_UART_SPBRGH_VALUE     (uint8_t)(MCOMM_UART_SPBRG_VALUE >> 8)
            #else
                #define MCOMM_UART_SPBRGL_VALUE     (uint8_t)(MCOMM_UART_SPBRG_VALUE)
                #define MCOMM_UART_SPBRGH_VALUE     (uint8_t)(0x00)
            #endif
            
        #endif
        
    #endif
    
    #if defined(MCOMM_TWO_WAY_ENABLED)
    
        #if defined(MCOMM_ENABLE_CUSTOM_OPCODE)
            #if !defined(MCOMM_CUSTOM_PROCESS)
                #error MCOMM_ENABLE_CUSTOM_OPCODE is defined, but MCOMM_CUSTOM_PROCESS has not been. Please define MCOMM_CUSTOM_PROCESS with the name of your custom mComm process function.
            #endif
            #if !defined(MCOMM_CUSTOM_READ_ITERATOR) && !defined(MCOMM_CUSTOM_WRITE_ITERATOR)
                #error MCOMM_ENABLE_CUSTOM_OPCODE is defined, but neither MCOMM_CUSTOM_READ_ITERATOR nor MCOMM_CUSTOM_WRITE_ITERATOR has been. Please define at least one of those values with the name of your custom read/write iterator function.
            #endif
            
            #define MCOMM_CUSTOM_PROCESS_ADR            &MCOMM_CUSTOM_PROCESS
            #define MCOMM_CUSTOM_READ_ITER_ADR          0x0000
            #define MCOMM_CUSTOM_WRITE_ITER_ADR         0x0000
            
            #if defined(MCOMM_CUSTOM_READ_ITERATOR)
                #define MCOMM_CUSTOM_READ_ENABLED
                #undef  MCOMM_CUSTOM_READ_ITER_ADR
                #define MCOMM_CUSTOM_READ_ITER_ADR      &MCOMM_CUSTOM_READ_ITERATOR
            #endif
            #if defined(MCOMM_CUSTOM_WRITE_ITERATOR)
                #define MCOMM_CUSTOM_WRITE_ENABLED
                #undef  MCOMM_CUSTOM_WRITE_ITER_ADR
                #define MCOMM_CUSTOM_WRITE_ITER_ADR     &MCOMM_CUSTOM_WRITE_ITERATOR
            #endif
            
        #endif
    
    
        #define MCOMM_STREAM_WRITE_ITER_ADR         &mComm_ErrorIterator
    
        #define MCOMM_OPCODES_ALL           {                                                               \
                                              { &mComm_processRAM,          &mComm_ramWriteIterator     },  \
                                              { &mComm_processRAM,          &mComm_ramReadIterator      },  \
                                              { &mComm_processNVM,          &mComm_nvmWriteIterator     },  \
                                              { &mComm_processNVM,          &mComm_nvmReadIterator      },  \
                                              { &mComm_processStream,       MCOMM_STREAM_WRITE_ITER_ADR },  \
                                              { &mComm_processStream,       &mComm_StreamReadIterator   },  \
                                              { MCOMM_CUSTOM_PROCESS_ADR,   MCOMM_CUSTOM_WRITE_ITER_ADR },  \
                                              { MCOMM_CUSTOM_PROCESS_ADR,   MCOMM_CUSTOM_READ_ITER_ADR  }   \
                                            }
                                            
        #define MCOMM_OPCODES_NONVM         {                                                               \
                                              { &mComm_processRAM,          &mComm_ramWriteIterator     },  \
                                              { &mComm_processRAM,          &mComm_ramReadIterator      },  \
                                              { &mComm_ErrorProcess,        &mComm_ErrorIterator        },  \
                                              { &mComm_ErrorProcess,        &mComm_ErrorIterator        },  \
                                              { &mComm_processStream,       MCOMM_STREAM_WRITE_ITER_ADR },  \
                                              { &mComm_processStream,       &mComm_StreamReadIterator   },  \
                                              { MCOMM_CUSTOM_PROCESS_ADR,   MCOMM_CUSTOM_WRITE_ITER_ADR },  \
                                              { MCOMM_CUSTOM_PROCESS_ADR,   MCOMM_CUSTOM_READ_ITER_ADR  }   \
                                            }
                                            
        #define MCOMM_OPCODES_NONVMSTREAM   {                                                               \
                                              { &mComm_processRAM,          &mComm_ramWriteIterator     },  \
                                              { &mComm_processRAM,          &mComm_ramReadIterator      },  \
                                              { &mComm_ErrorProcess,        &mComm_ErrorIterator        },  \
                                              { &mComm_ErrorProcess,        &mComm_ErrorIterator        },  \
                                              { &mComm_ErrorProcess,        &mComm_ErrorIterator        },  \
                                              { &mComm_ErrorProcess,        &mComm_ErrorIterator        },  \
                                              { MCOMM_CUSTOM_PROCESS_ADR,   MCOMM_CUSTOM_WRITE_ITER_ADR },  \
                                              { MCOMM_CUSTOM_PROCESS_ADR,   MCOMM_CUSTOM_READ_ITER_ADR  }   \
                                            }
                                            
        #define MCOMM_OPCODES_NOSTREAM      {                                                               \
                                              { &mComm_processRAM,          &mComm_ramWriteIterator     },  \
                                              { &mComm_processRAM,          &mComm_ramReadIterator      },  \
                                              { &mComm_processNVM,          &mComm_nvmWriteIterator     },  \
                                              { &mComm_processNVM,          &mComm_nvmReadIterator      },  \
                                              { &mComm_ErrorProcess,        &mComm_ErrorIterator        },  \
                                              { &mComm_ErrorProcess,        &mComm_ErrorIterator        },  \
                                              { MCOMM_CUSTOM_PROCESS_ADR,   MCOMM_CUSTOM_WRITE_ITER_ADR },  \
                                              { MCOMM_CUSTOM_PROCESS_ADR,   MCOMM_CUSTOM_READ_ITER_ADR  }   \
                                            }

        #define MCOMM_OPCODES_NOCUST        {                                                               \
                                              { &mComm_processRAM,          &mComm_ramWriteIterator     },  \
                                              { &mComm_processRAM,          &mComm_ramReadIterator      },  \
                                              { &mComm_processNVM,          &mComm_nvmWriteIterator     },  \
                                              { &mComm_processNVM,          &mComm_nvmReadIterator      },  \
                                              { &mComm_processStream,       MCOMM_STREAM_WRITE_ITER_ADR },  \
                                              { &mComm_processStream,       &mComm_StreamReadIterator   },  \
                                              { &mComm_ErrorProcess,        &mComm_ErrorIterator        },  \
                                              { &mComm_ErrorProcess,        &mComm_ErrorIterator        }   \
                                            }
                                            
        #define MCOMM_OPCODES_NONVMCUST     {                                                               \
                                              { &mComm_processRAM,          &mComm_ramWriteIterator     },  \
                                              { &mComm_processRAM,          &mComm_ramReadIterator      },  \
                                              { &mComm_ErrorProcess,        &mComm_ErrorIterator        },  \
                                              { &mComm_ErrorProcess,        &mComm_ErrorIterator        },  \
                                              { &mComm_processStream,       MCOMM_STREAM_WRITE_ITER_ADR },  \
                                              { &mComm_processStream,       &mComm_StreamReadIterator   },  \
                                              { &mComm_ErrorProcess,        &mComm_ErrorIterator        },  \
                                              { &mComm_ErrorProcess,        &mComm_ErrorIterator        }   \
                                            }
                                            
        #define MCOMM_OPCODES_NONVMSTRCUST  {                                                               \
                                              { &mComm_processRAM,          &mComm_ramWriteIterator     },  \
                                              { &mComm_processRAM,          &mComm_ramReadIterator      },  \
                                              { &mComm_ErrorProcess,        &mComm_ErrorIterator        },  \
                                              { &mComm_ErrorProcess,        &mComm_ErrorIterator        },  \
                                              { &mComm_ErrorProcess,        &mComm_ErrorIterator        },  \
                                              { &mComm_ErrorProcess,        &mComm_ErrorIterator        },  \
                                              { &mComm_ErrorProcess,        &mComm_ErrorIterator        },  \
                                              { &mComm_ErrorProcess,        &mComm_ErrorIterator        }   \
                                            }
                                            
        #define MCOMM_OPCODES_NOSTRCUST     {                                                               \
                                              { &mComm_processRAM,          &mComm_ramWriteIterator     },  \
                                              { &mComm_processRAM,          &mComm_ramReadIterator      },  \
                                              { &mComm_processNVM,          &mComm_nvmWriteIterator     },  \
                                              { &mComm_processNVM,          &mComm_nvmReadIterator      },  \
                                              { &mComm_ErrorProcess,        &mComm_ErrorIterator        },  \
                                              { &mComm_ErrorProcess,        &mComm_ErrorIterator        },  \
                                              { &mComm_ErrorProcess,        &mComm_ErrorIterator        },  \
                                              { &mComm_ErrorProcess,        &mComm_ErrorIterator        }   \
                                            }
                                        
        //========================================================================
        // Opcode Definitions          // ONLY EDIT IF IMPLEMENTING CUSTOM OPCODES
        //========================================================================
        #define MCOMM_NUMBER_OPCODES        8 
        
        // This defines the values stored in the opcode lookup table which tells mComm which function
        // to call when an opcode is received. The first pointer is to the initialization function (called
        // once when a packet with that opcode has been received) and the second pointer is to the 
        // iterator which will be called continuously until the mComm_output.hasNext bit is cleared.
        #if defined(MCOMM_ENABLE_CUSTOM_OPCODE)
            #if defined(MCOMM_ENABLE_NVM_ACCESS)
                #if defined(MCOMM_ENABLE_STREAM)
                    #define MCOMM_OPCODE_VALUES         MCOMM_OPCODES_ALL
                #else
                    #define MCOMM_OPCODE_VALUES         MCOMM_OPCODES_NOSTREAM
                #endif
            #else
                #if defined(MCOMM_ENABLE_STREAM)
                    #define MCOMM_OPCODE_VALUES         MCOMM_OPCODES_NONVM
                #else
                    #define MCOMM_OPCODE_VALUES         MCOMM_OPCODES_NONVMSTREAM
                #endif
            #endif
        #else
            #if defined(MCOMM_ENABLE_NVM_ACCESS)
                #if defined(MCOMM_ENABLE_STREAM)
                    #define MCOMM_OPCODE_VALUES         MCOMM_OPCODES_NOCUST
                #else
                    #define MCOMM_OPCODE_VALUES         MCOMM_OPCODES_NOSTRCUST
                #endif
            #else
                #if defined(MCOMM_ENABLE_STREAM)
                    #define MCOMM_OPCODE_VALUES         MCOMM_OPCODES_NONVMCUST
                #else
                    #define MCOMM_OPCODE_VALUES         MCOMM_OPCODES_NONVMSTRCUST
                #endif
            #endif
        #endif
        
        // Not used in the mComm code, but provided here as an easy quick-reference. The opcode is the
        // index of the lookup table that mComm will jump to when processing the packet.
        #define MCOMM_OP_RAM_WRITE          0x00
        #define MCOMM_OP_RAM_READ           0x01
        #define MCOMM_OP_NVM_WRITE          0x02
        #define MCOMM_OP_NVM_READ           0x03
        #define MCOMM_OP_STREAM_WRITE       0x04
        #define MCOMM_OP_STREAM_READ        0x05
        #define MCOMM_OP_CUSTOM_WRITE       0x06
        #define MCOMM_OP_CUSTOM_READ        0x07
        
        #define MCOMM_OP_ACK                0xAA        // Value to send when 'ACK'ing to the Master
        #define MCOMM_OP_NACK               0xA5
        
        
        #define MCOMM_CONFIG_NVM_ACCESS     0
        #define MCOMM_CONFIG_STREAM_ACCESS  0
        #define MCOMM_CONFIG_STREAM_RAM     0
        #define MCOMM_CONFIG_CUSTOM_ACCESS  0
        
        #if defined(MCOMM_ENABLE_NVM_ACCESS)
            #undef  MCOMM_CONFIG_NVM_ACCESS
            #define MCOMM_CONFIG_NVM_ACCESS     0x01
            
            #if !defined(MTOUCH_EEPROM_ENABLED) && !defined(MCOMM_NVM_MTOUCH_EE_DISABLED)
            #warning MCOMM NVM access is enabled, but the mTouch Framework is not using the EEPROM to store values. Either enable mTouch's EEPROM implementation by defining MTOUCH_EEPROM_ENABLED in mTouch_config.h or define MCOMM_NVM_MTOUCH_EE_DISABLED in mComm_config.h to remove this warning.
            #endif
        #endif
        #if defined(MCOMM_ENABLE_STREAM)
                #undef  MCOMM_CONFIG_STREAM_ACCESS
                #define MCOMM_CONFIG_STREAM_ACCESS  0x02
            #if defined(MCOMM_STREAM_STORED_IN_RAM)
                #undef  MCOMM_CONFIG_STREAM_RAM
                #define MCOMM_CONFIG_STREAM_RAM     0x04
            #endif
        #endif
        #if defined(MCOMM_ENABLE_CUSTOM_OPCODE)
                #undef  MCOMM_CONFIG_CUSTOM_ACCESS
                #define MCOMM_CONFIG_CUSTOM_ACCESS  0x08
        #endif
        
        #define MCOMM_CONFIGURATION_BYTE   (MCOMM_CONFIG_NVM_ACCESS | MCOMM_CONFIG_STREAM_ACCESS | MCOMM_CONFIG_STREAM_RAM | MCOMM_CONFIG_CUSTOM_ACCESS)
    
        
        #define MCOMM_TOGGLE_ADR            0x0000
        #if defined(MTOUCH_TOGGLE_ENABLED)
            #undef  MCOMM_TOGGLE_ADR
            #define MCOMM_TOGGLE_ADR        &mTouch_toggle
        #endif
    
        #define MCOMM_SLIDER_ADR            0x0000
        #if defined(MTOUCH_SLIDERS_ENABLED)
            #undef  MCOMM_SLIDER_ADR
            #define MCOMM_SLIDER_ADR        &mTouch_slider
        #endif
    
        #if     _EEPROMSIZE == 0
            #define MTOUCH_EEPROM_POW2_SIZE     0
        #elif   _EEPROMSIZE == 128
            #define MTOUCH_EEPROM_POW2_SIZE     7
        #elif   _EEPROMSIZE == 256
            #define MTOUCH_EEPROM_POW2_SIZE     8
        #elif   _EEPROMSIZE == 512
            #define MTOUCH_EEPROM_POW2_SIZE     9
        #elif   _EEPROMSIZE == 1024
            #define MTOUCH_EEPROM_POW2_SIZE     10
        #elif   _EEPROMSIZE == 2048
            #define MTOUCH_EEPROM_POW2_SIZE     11
        #elif   _EEPROMSIZE == 4096
            #define MTOUCH_EEPROM_POW2_SIZE     12
        #elif   _EEPROMSIZE == 8192
            #define MTOUCH_EEPROM_POW2_SIZE     13
        #elif   _EEPROMSIZE == 16384
            #define MTOUCH_EEPROM_POW2_SIZE     14
        #elif   _EEPROMSIZE == 32768
            #define MTOUCH_EEPROM_POW2_SIZE     15
        #elif   _EEPROMSIZE == 65536
            #define MTOUCH_EEPROM_POW2_SIZE     16
        #endif
        
        #define MCOMM_STREAM_VALUE_LAST     { 0x0000, 0 }
        
        #define MCOMM_STREAM_COMMA0
        #define MCOMM_STREAM_COMMA1
        #define MCOMM_STREAM_COMMA2
        #define MCOMM_STREAM_COMMA3
        #define MCOMM_STREAM_COMMA4
        #define MCOMM_STREAM_COMMA5
        #define MCOMM_STREAM_COMMA6
        #define MCOMM_STREAM_COMMA7
        #define MCOMM_STREAM_COMMA8
        #define MCOMM_STREAM_COMMA9
        #define MCOMM_STREAM_COMMA10
        #define MCOMM_STREAM_COMMA11
        #define MCOMM_STREAM_COMMA12
        #define MCOMM_STREAM_COMMA13
        #define MCOMM_STREAM_COMMA14
        #define MCOMM_STREAM_COMMA15
        #define MCOMM_STREAM_COMMA16
        #define MCOMM_STREAM_COMMA17
        #define MCOMM_STREAM_COMMA18
        #define MCOMM_STREAM_COMMA19
            
        #if MCOMM_STREAM_SIZE > 0
            #if !defined(MCOMM_STREAM_VALUE0)
            #define MCOMM_STREAM_VALUE0     MCOMM_STREAM_VALUE_LAST
            #endif
        #endif
        #if MCOMM_STREAM_SIZE > 1
            #if !defined(MCOMM_STREAM_VALUE1)
            #define MCOMM_STREAM_VALUE1     MCOMM_STREAM_VALUE_LAST
            #endif
            #undef  MCOMM_STREAM_COMMA0
            #define MCOMM_STREAM_COMMA0     ,
        #endif
        #if MCOMM_STREAM_SIZE > 2    
            #if !defined(MCOMM_STREAM_VALUE2)    
            #define MCOMM_STREAM_VALUE2     MCOMM_STREAM_VALUE_LAST    
            #endif    
            #undef  MCOMM_STREAM_COMMA1     
            #define MCOMM_STREAM_COMMA1     ,     
        #endif
        #if MCOMM_STREAM_SIZE > 3    
            #if !defined(MCOMM_STREAM_VALUE3)    
            #define MCOMM_STREAM_VALUE3     MCOMM_STREAM_VALUE_LAST    
            #endif    
            #undef  MCOMM_STREAM_COMMA2     
            #define MCOMM_STREAM_COMMA2     ,     
        #endif
        #if MCOMM_STREAM_SIZE > 4    
            #if !defined(MCOMM_STREAM_VALUE4)    
            #define MCOMM_STREAM_VALUE4     MCOMM_STREAM_VALUE_LAST    
            #endif    
            #undef  MCOMM_STREAM_COMMA3     
            #define MCOMM_STREAM_COMMA3     ,     
        #endif
        #if MCOMM_STREAM_SIZE > 5    
            #if !defined(MCOMM_STREAM_VALUE5)    
            #define MCOMM_STREAM_VALUE5     MCOMM_STREAM_VALUE_LAST    
            #endif    
            #undef  MCOMM_STREAM_COMMA4     
            #define MCOMM_STREAM_COMMA4     ,     
        #endif
        #if MCOMM_STREAM_SIZE > 6    
            #if !defined(MCOMM_STREAM_VALUE6)    
            #define MCOMM_STREAM_VALUE6     MCOMM_STREAM_VALUE_LAST    
            #endif    
            #undef  MCOMM_STREAM_COMMA5     
            #define MCOMM_STREAM_COMMA5     ,     
        #endif
        #if MCOMM_STREAM_SIZE > 7    
            #if !defined(MCOMM_STREAM_VALUE7)    
            #define MCOMM_STREAM_VALUE7     MCOMM_STREAM_VALUE_LAST    
            #endif    
            #undef  MCOMM_STREAM_COMMA6     
            #define MCOMM_STREAM_COMMA6     ,     
        #endif
        #if MCOMM_STREAM_SIZE > 8    
            #if !defined(MCOMM_STREAM_VALUE8)    
            #define MCOMM_STREAM_VALUE8     MCOMM_STREAM_VALUE_LAST    
            #endif    
            #undef  MCOMM_STREAM_COMMA7     
            #define MCOMM_STREAM_COMMA7     ,     
        #endif
        #if MCOMM_STREAM_SIZE > 9    
            #if !defined(MCOMM_STREAM_VALUE9)    
            #define MCOMM_STREAM_VALUE9     MCOMM_STREAM_VALUE_LAST    
            #endif    
            #undef  MCOMM_STREAM_COMMA8     
            #define MCOMM_STREAM_COMMA8     ,     
        #endif
        #if MCOMM_STREAM_SIZE > 10    
            #if !defined(MCOMM_STREAM_VALUE10)    
            #define MCOMM_STREAM_VALUE10    MCOMM_STREAM_VALUE_LAST    
            #endif    
            #undef  MCOMM_STREAM_COMMA9     
            #define MCOMM_STREAM_COMMA9     ,     
        #endif
        #if MCOMM_STREAM_SIZE > 11    
            #if !defined(MCOMM_STREAM_VALUE11)    
            #define MCOMM_STREAM_VALUE11    MCOMM_STREAM_VALUE_LAST    
            #endif    
            #undef  MCOMM_STREAM_COMMA10     
            #define MCOMM_STREAM_COMMA10     ,     
        #endif
        #if MCOMM_STREAM_SIZE > 12    
            #if !defined(MCOMM_STREAM_VALUE12)    
            #define MCOMM_STREAM_VALUE12    MCOMM_STREAM_VALUE_LAST    
            #endif    
            #undef  MCOMM_STREAM_COMMA11     
            #define MCOMM_STREAM_COMMA11     ,     
        #endif
        #if MCOMM_STREAM_SIZE > 13    
            #if !defined(MCOMM_STREAM_VALUE13)    
            #define MCOMM_STREAM_VALUE13    MCOMM_STREAM_VALUE_LAST    
            #endif    
            #undef  MCOMM_STREAM_COMMA12     
            #define MCOMM_STREAM_COMMA12     ,     
        #endif
        #if MCOMM_STREAM_SIZE > 14    
            #if !defined(MCOMM_STREAM_VALUE14)    
            #define MCOMM_STREAM_VALUE14    MCOMM_STREAM_VALUE_LAST    
            #endif    
            #undef  MCOMM_STREAM_COMMA13     
            #define MCOMM_STREAM_COMMA13     ,     
        #endif
        #if MCOMM_STREAM_SIZE > 15    
            #if !defined(MCOMM_STREAM_VALUE15)    
            #define MCOMM_STREAM_VALUE15    MCOMM_STREAM_VALUE_LAST    
            #endif    
            #undef  MCOMM_STREAM_COMMA14     
            #define MCOMM_STREAM_COMMA14     ,     
        #endif
        #if MCOMM_STREAM_SIZE > 16    
            #if !defined(MCOMM_STREAM_VALUE16)    
            #define MCOMM_STREAM_VALUE16    MCOMM_STREAM_VALUE_LAST    
            #endif    
            #undef  MCOMM_STREAM_COMMA15     
            #define MCOMM_STREAM_COMMA15     ,     
        #endif
        #if MCOMM_STREAM_SIZE > 17    
            #if !defined(MCOMM_STREAM_VALUE17)    
            #define MCOMM_STREAM_VALUE17    MCOMM_STREAM_VALUE_LAST    
            #endif    
            #undef  MCOMM_STREAM_COMMA16     
            #define MCOMM_STREAM_COMMA16     ,     
        #endif
        #if MCOMM_STREAM_SIZE > 18    
            #if !defined(MCOMM_STREAM_VALUE18)    
            #define MCOMM_STREAM_VALUE18    MCOMM_STREAM_VALUE_LAST    
            #endif    
            #undef  MCOMM_STREAM_COMMA17     
            #define MCOMM_STREAM_COMMA17     ,     
        #endif
        #if MCOMM_STREAM_SIZE > 19    
            #if !defined(MCOMM_STREAM_VALUE19)    
            #define MCOMM_STREAM_VALUE19    MCOMM_STREAM_VALUE_LAST    
            #endif    
            #undef  MCOMM_STREAM_COMMA18     
            #define MCOMM_STREAM_COMMA18     ,     
        #endif

        #if MCOMM_STREAM_SIZE < 20      
        #if defined(MCOMM_STREAM_VALUE19)            
        #undef  MCOMM_STREAM_VALUE19        
        #endif    
        #define MCOMM_STREAM_VALUE19
        #endif
                
        #if MCOMM_STREAM_SIZE < 19      
        #if defined(MCOMM_STREAM_VALUE18)            
        #undef  MCOMM_STREAM_VALUE18        
        #endif    
        #define MCOMM_STREAM_VALUE18
        #endif
                
        #if MCOMM_STREAM_SIZE < 18      
        #if defined(MCOMM_STREAM_VALUE17)            
        #undef  MCOMM_STREAM_VALUE17        
        #endif    
        #define MCOMM_STREAM_VALUE17
        #endif
                
        #if MCOMM_STREAM_SIZE < 17      
        #if defined(MCOMM_STREAM_VALUE16)            
        #undef  MCOMM_STREAM_VALUE16        
        #endif    
        #define MCOMM_STREAM_VALUE16
        #endif
                
        #if MCOMM_STREAM_SIZE < 16      
        #if defined(MCOMM_STREAM_VALUE15)            
        #undef  MCOMM_STREAM_VALUE15        
        #endif    
        #define MCOMM_STREAM_VALUE15
        #endif
                
        #if MCOMM_STREAM_SIZE < 15      
        #if defined(MCOMM_STREAM_VALUE14)            
        #undef  MCOMM_STREAM_VALUE14        
        #endif    
        #define MCOMM_STREAM_VALUE14
        #endif
                
        #if MCOMM_STREAM_SIZE < 14      
        #if defined(MCOMM_STREAM_VALUE13)            
        #undef  MCOMM_STREAM_VALUE13        
        #endif    
        #define MCOMM_STREAM_VALUE13
        #endif
                
        #if MCOMM_STREAM_SIZE < 13      
        #if defined(MCOMM_STREAM_VALUE12)            
        #undef  MCOMM_STREAM_VALUE12        
        #endif    
        #define MCOMM_STREAM_VALUE12
        #endif
                
        #if MCOMM_STREAM_SIZE < 12      
        #if defined(MCOMM_STREAM_VALUE11)            
        #undef  MCOMM_STREAM_VALUE11        
        #endif    
        #define MCOMM_STREAM_VALUE11
        #endif
                
        #if MCOMM_STREAM_SIZE < 11      
        #if defined(MCOMM_STREAM_VALUE10)            
        #undef  MCOMM_STREAM_VALUE10        
        #endif    
        #define MCOMM_STREAM_VALUE10
        #endif
                
        #if MCOMM_STREAM_SIZE < 10      
        #if defined(MCOMM_STREAM_VALUE9)             
        #undef  MCOMM_STREAM_VALUE9         
        #endif    
        #define MCOMM_STREAM_VALUE9
        #endif
                
        #if MCOMM_STREAM_SIZE < 9       
        #if defined(MCOMM_STREAM_VALUE8)             
        #undef  MCOMM_STREAM_VALUE8         
        #endif    
        #define MCOMM_STREAM_VALUE8
        #endif
                
        #if MCOMM_STREAM_SIZE < 8       
        #if defined(MCOMM_STREAM_VALUE7)             
        #undef  MCOMM_STREAM_VALUE7         
        #endif    
        #define MCOMM_STREAM_VALUE7
        #endif
                
        #if MCOMM_STREAM_SIZE < 7       
        #if defined(MCOMM_STREAM_VALUE6)             
        #undef  MCOMM_STREAM_VALUE6         
        #endif    
        #define MCOMM_STREAM_VALUE6
        #endif
                
        #if MCOMM_STREAM_SIZE < 6       
        #if defined(MCOMM_STREAM_VALUE5)             
        #undef  MCOMM_STREAM_VALUE5         
        #endif    
        #define MCOMM_STREAM_VALUE5
        #endif
                
        #if MCOMM_STREAM_SIZE < 5       
        #if defined(MCOMM_STREAM_VALUE4)             
        #undef  MCOMM_STREAM_VALUE4         
        #endif    
        #define MCOMM_STREAM_VALUE4
        #endif
                
        #if MCOMM_STREAM_SIZE < 4       
        #if defined(MCOMM_STREAM_VALUE3)             
        #undef  MCOMM_STREAM_VALUE3         
        #endif    
        #define MCOMM_STREAM_VALUE3
        #endif
                
        #if MCOMM_STREAM_SIZE < 3       
        #if defined(MCOMM_STREAM_VALUE2)             
        #undef  MCOMM_STREAM_VALUE2         
        #endif    
        #define MCOMM_STREAM_VALUE2
        #endif
                
        #if MCOMM_STREAM_SIZE < 2       
        #if defined(MCOMM_STREAM_VALUE1)             
        #undef  MCOMM_STREAM_VALUE1         
        #endif   
        #define MCOMM_STREAM_VALUE1 
        #endif
                
        #if MCOMM_STREAM_SIZE < 1       
        #if defined(MCOMM_STREAM_VALUE0)             
        #undef  MCOMM_STREAM_VALUE0         
        #endif    
        #define MCOMM_STREAM_VALUE0
        #endif


        #define MCOMM_STREAM_VALUES         {   MCOMM_STREAM_VALUE0     MCOMM_STREAM_COMMA0   \
                                                MCOMM_STREAM_VALUE1     MCOMM_STREAM_COMMA1   \
                                                MCOMM_STREAM_VALUE2     MCOMM_STREAM_COMMA2   \
                                                MCOMM_STREAM_VALUE3     MCOMM_STREAM_COMMA3   \
                                                MCOMM_STREAM_VALUE4     MCOMM_STREAM_COMMA4   \
                                                MCOMM_STREAM_VALUE5     MCOMM_STREAM_COMMA5   \
                                                MCOMM_STREAM_VALUE6     MCOMM_STREAM_COMMA6   \
                                                MCOMM_STREAM_VALUE7     MCOMM_STREAM_COMMA7   \
                                                MCOMM_STREAM_VALUE8     MCOMM_STREAM_COMMA8   \
                                                MCOMM_STREAM_VALUE9     MCOMM_STREAM_COMMA9   \
                                                MCOMM_STREAM_VALUE10    MCOMM_STREAM_COMMA10  \
                                                MCOMM_STREAM_VALUE11    MCOMM_STREAM_COMMA11  \
                                                MCOMM_STREAM_VALUE12    MCOMM_STREAM_COMMA12  \
                                                MCOMM_STREAM_VALUE13    MCOMM_STREAM_COMMA13  \
                                                MCOMM_STREAM_VALUE14    MCOMM_STREAM_COMMA14  \
                                                MCOMM_STREAM_VALUE15    MCOMM_STREAM_COMMA15  \
                                                MCOMM_STREAM_VALUE16    MCOMM_STREAM_COMMA16  \
                                                MCOMM_STREAM_VALUE17    MCOMM_STREAM_COMMA17  \
                                                MCOMM_STREAM_VALUE18    MCOMM_STREAM_COMMA18  \
                                                MCOMM_STREAM_VALUE19    MCOMM_STREAM_COMMA19  \
                                                , MCOMM_STREAM_VALUE_LAST                     \
                                            };
            
    
    #endif
#endif
#endif