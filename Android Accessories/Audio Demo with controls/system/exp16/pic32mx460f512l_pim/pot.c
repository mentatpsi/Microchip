#include <stdint.h>
#include <p32xxxx.h>

/****************************************************************************
  Function:
    uint16_t PotentiometerRead10bit(void)

  Summary:
    Reads the pot value and returns the percentage of full scale (0-1023)

  Description:
    Reads the pot value and returns the percentage of full scale (0-1023)

  Precondition:
    A/D is initialized properly

  Parameters:
    None

  Return Values:
    None

  Remarks:
    None
  ***************************************************************************/
uint16_t PotentiometerRead10bit(void)
{
    uint16_t w;

    AD1PCFG = 0xFFFB; // PORTB = Digital; RB2 = analog
    AD1CON1 = 0x0000; // SAMP bit = 0 ends sampling ...
    // and starts converting
    AD1CHS = 0x00020000; // Connect RB2/AN2 as CH0 input ..
    // in this example RB2/AN2 is the input
    AD1CSSL = 0;
    AD1CON3 = 0x0002; // Manual Sample, Tad = internal 6 TPB
    AD1CON2 = 0;
    AD1CON1SET = 0x8000; // turn ADC ON

    AD1CON1SET = 0x0002; // start sampling ...
    for(w=0;w<1000;w++){Nop();} //Sample delay, conversion start automatically
    
    AD1CON1CLR = 0x0002; // start Converting
    while (!(AD1CON1 & 0x0001));// conversion done?

    return (uint16_t)ADC1BUF0;
}

void PotentiometerInit(void)
{
    AD1PCFGbits.PCFG5 = 0;
    AD1CON2bits.VCFG = 0x0;
    AD1CON3bits.ADCS = 0xFF;
    AD1CON1bits.SSRC = 0x0;
    AD1CON3bits.SAMC = 0x10;
    AD1CON1bits.FORM = 0x0;
    AD1CON2bits.SMPI = 0x0;
    AD1CON1bits.ADON = 1;
}