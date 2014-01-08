#include "SystemProfile.h"
#include "WirelessProtocols/MSPI.h"

#if defined(ENABLE_EEPROM)

    #include "WirelessProtocols/EEPROM.h"

        
    void EEPROMRead(BYTE *dest, BYTE addr, BYTE count)
    {
        #if defined(__18CXX)
            BYTE oldGIEH = INTCONbits.GIEH;
            
            INTCONbits.GIEH = 0;
        #endif
        
        EE_nCS = 0;
        SPIPut(SPI_READ);
        SPIPut(addr);
        while( count )
        {
            *dest++ = SPIGet();
            count--;
        }
        EE_nCS = 1;
        
        #if defined(__18CXX)
            INTCONbits.GIEH = oldGIEH;
        #endif
    }
 
#else
    extern char bogusVar;
    
#endif   
