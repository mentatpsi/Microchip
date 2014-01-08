#include <stdint.h>
#include <p24fxxxx.h>

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

    AD1CHS = 0x9;           //MUXA uses AN9

    // Get an ADC sample
    AD1CON1bits.SAMP = 1;           //Start sampling
    for(w=0;w<1000;w++){Nop();} //Sample delay, conversion start automatically
    AD1CON1bits.SAMP = 0;           //Start sampling
    for(w=0;w<1000;w++){Nop();} //Sample delay, conversion start automatically
    while(!AD1CON1bits.DONE);       //Wait for conversion to complete

    return (uint16_t)ADC1BUF0;
}

void PotentiometerInit(void)
{
    AD1PCFGLbits.PCFG9 = 0;
    AD1CON2bits.VCFG = 0x0;
    AD1CON3bits.ADCS = 0xFF;
    AD1CON1bits.SSRC = 0x0;    
    AD1CON3bits.SAMC = 0b10000;
    AD1CON1bits.FORM = 0b00;
    AD1CON2bits.SMPI = 0x0;
    AD1CON1bits.ADON = 1;
}