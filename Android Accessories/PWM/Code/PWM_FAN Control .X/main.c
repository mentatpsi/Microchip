/* 
 * File:   main.c
 * Author: IBRAHIM LABS
 *
 * Created on July 5, 2013, 10:37 AM
 */

#define CRYSTAL_FREQUENCY   16000000         // Crystal of 16MHz
#define FCY CRYSTAL_FREQUENCY/2UL            // Instruction Cycle Clock must be defined

#include <p24F16KA102.h>
#include <libpic30.h>
#include "PWM.h"

/******************************************************************************/

/*  *** Configuration Bits Setting  *** */

_FGS(GWRP_OFF & GCP_OFF)
_FOSCSEL(FNOSC_PRI & IESO_OFF)
_FOSC(POSCMOD_HS & OSCIOFNC_ON & POSCFREQ_HS & FCKSM_CSDCMD)
_FWDT(WINDIS_OFF & FWDTEN_OFF)
_FPOR(BOREN_BOR2 & PWRTEN_OFF  & MCLRE_ON)
_FDS(DSBOREN_OFF & DSWDTEN_OFF)

/******************************************************************************/
/*
 *
 */



int main(void)
{
    unsigned int DutyCycle = 10;
    
    InitializeTimer2For_PWM();
    Initialize_PWM();

    /*  Internal Pullups enabled    */
    CNPU1bits.CN0PUE = 1;
    CNPU1bits.CN9PUE = 1;

    UP_DIRECTION = Input;
    DOWN_DIRECTION = Input;


    while(1)
    {

        if (!UP)
        {
            __delay_us(100);
            if (DutyCycle < 0x7FFA)     
                SetDutyCycle_PWM(DutyCycle++);
        }
        else if (!DOWN)
        {
            __delay_us(100);
            if (DutyCycle > 0x0F)
                SetDutyCycle_PWM(DutyCycle--);
        }
    
    }
    return 0;
}
