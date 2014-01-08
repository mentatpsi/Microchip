
#ifndef __EEPROM_H
    #define __EEPROM_H
    
    #include "SystemProfile.h"
    
    #include "SystemProfile.h"
    #include "GenericTypeDefs.h"
    #include "Compiler.h"
        
        
    #define SPI_WRT_STATUS  0x01
    #define SPI_WRITE       0x02
    #define SPI_READ        0x03
    #define SPI_DIS_WRT     0x04
    #define SPI_RD_STATUS   0x05
    #define SPI_EN_WRT      0x06


    #define EEPROM_MAC_ADDR 0xFA
        
    void EEPROMRead(BYTE *dest, BYTE addr, BYTE count);

#endif
