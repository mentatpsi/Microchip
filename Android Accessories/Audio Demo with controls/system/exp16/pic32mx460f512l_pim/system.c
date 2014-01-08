#include <p32xxxx.h>
#include <system.h>
#include <plib.h>

#pragma config UPLLEN   = ON            // USB PLL Enabled
#pragma config FPLLMUL  = MUL_15        // PLL Multiplier
#pragma config UPLLIDIV = DIV_2         // USB PLL Input Divider
#pragma config FPLLIDIV = DIV_2         // PLL Input Divider
#pragma config FPLLODIV = DIV_1         // PLL Output Divider
#pragma config FPBDIV   = DIV_1         // Peripheral Clock divisor
#pragma config FWDTEN   = OFF           // Watchdog Timer
#pragma config WDTPS    = PS1           // Watchdog Timer Postscale
//#pragma config FCKSM    = CSDCMD        // Clock Switching & Fail Safe Clock Monitor
#pragma config OSCIOFNC = OFF           // CLKO Enable
#pragma config POSCMOD  = HS            // Primary Oscillator
#pragma config IESO     = OFF           // Internal/External Switch-over
#pragma config FSOSCEN  = OFF           // Secondary Oscillator Enable (KLO was off)
#pragma config FNOSC    = PRIPLL        // Oscillator Selection
#pragma config CP       = OFF           // Code Protect
#pragma config BWP      = OFF           // Boot Flash Write Protect
#pragma config PWP      = OFF           // Program Flash Write Protect
#pragma config ICESEL   = ICS_PGx2      // ICE/ICD Comm Channel Select
//#pragma config DEBUG    = ON            // Background Debugger Enable

void _general_exception_handler(unsigned cause, unsigned status)
{
    unsigned long address = _CP0_GET_EPC();

    while(1){}
}

/****************************************************************************
  Function:
    void SYS_Init(void)

  Summary:
    Initialize the PIC32 core to the correct modes and clock speeds

  Description:
    Initialize the PIC32 core to the correct modes and clock speeds

  Precondition:
    Only runs on PIC32

  Parameters:
    None

  Return Values:
    None

  Remarks:
    None
  ***************************************************************************/
void SYS_Init(void)
{
    int  value;

    #if defined(RUN_AT_60MHZ)
        // Use OSCCON default
    #else
        OSCCONCLR = 0x38000000; //PLLODIV
        #if defined(RUN_AT_48MHZ)
            OSCCONSET = 0x08000000; //PLLODIV /2
        #elif defined(RUN_AT_24MHZ)
            OSCCONSET = 0x10000000; //PLLODIV /4
        #else
            #error Cannot set OSCCON
        #endif
    #endif

    value = SYSTEMConfigWaitStatesAndPB( GetSystemClock() );

    // Enable the cache for the best performance
    CheKseg0CacheOn();

    INTEnableSystemMultiVectoredInt();

    DDPCONbits.JTAGEN = 0;

    value = OSCCON;
    while (!(value & 0x00000020))
    {
        value = OSCCON;    // Wait for PLL lock to stabilize
    }

    INTEnableInterrupts();
}
