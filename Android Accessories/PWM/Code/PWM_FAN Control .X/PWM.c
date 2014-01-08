/*
 * File:   PWM.c
 * Author: IBRAHIM LABS
 *
 * Created on July 5, 2013, 10:37 AM
 */

#include <p24F16KA102.h>

void InitializeTimer2For_PWM(void)
{
    T2CONbits.TON = 0;      /*  Timer2 is used for generating PWM frequency */
    T2CONbits.TCS = 0;
    T2CONbits.TGATE = 0;
    T2CONbits.TCKPS = 0b00;
    TMR2 = 0x00;
    PR2 = 0x7FFF;           // FOR 488 Hz
    T2CONbits.TON = 1;      // TIMER 2 ON

}

/******************************************************************************/

void Initialize_PWM(void)
{
    OC1CONbits.OCM = 0b000; /*  clearing OCM bits   */
    OC1R = 0x0F;
    OC1RS = 0x0F;           /*  starting module with min PWM    */
    OC1CONbits.OCTSEL = 0;
    OC1R = 0x0F;
    OC1CONbits.OCM = 0b110;

}

/******************************************************************************/

void SetDutyCycle_PWM(unsigned int DutyCycle)
{
    OC1RS = DutyCycle;  /*  with 488Hz frquency we have 15bits resolution   */
    
}

/******************************************************************************/
