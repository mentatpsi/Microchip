#include <p24fxxxx.h>
#include <system.h>

_CONFIG2(FNOSC_PRIPLL & POSCMOD_HS & PLL_96MHZ_ON & PLLDIV_DIV2) // Primary HS OSC with PLL, USBPLL /2
_CONFIG1(JTAGEN_OFF & FWDTEN_OFF & ICS_PGx2)   // JTAG off, watchdog timer off

void _ISR __attribute__((__no_auto_psv__)) _AddressError(void)
{
    while(1){}
}
void _ISR __attribute__((__no_auto_psv__)) _StackError(void)
{
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
}
