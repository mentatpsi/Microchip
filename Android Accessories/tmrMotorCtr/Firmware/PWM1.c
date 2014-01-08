
#include "HardwareProfile.h"


#define  PWM_PERIOD 62500



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

  void PwmInit( void ) {
      /* Unmap RP31(RF13) */
      //RPOR15bits.RP31R = 0; //Change for output

      /* Let's unmap RD10 instead*/
      //RPOR1bits.RP3R = 0;

      mInitPWM_RD10();

      /*
      //LATDbits.LATD10 = 0; //TRISDbits.TRISD10 = 0;
      //PORTDbits.RD10 = 0;

      // Drive RF13 low and make it an output /
      //LATFbits.LATF13 = 0;  //Change for output
      //TRISFbits.TRISF13 = 0;    //Change for output

      //LATDbits.LATD10 = 0;
      //TRISDbits.TRISD10 = 0;
      */
      
      Nop();

      /* Reset PWM */
      OC1CON1 = 0x0000;
      OC1CON2 = 0x0000;

      /* set PWM duty cycle to 50% */


      //OC1R    = curPWM_PERIOD;

      /*
      BYTE gear;

      gear = getGear();
      DWORD temp = PWM_PERIOD > gear;

      DWORD curPWM = PWM_PERIOD - temp;
      */
      //DWORD curRate = 0x0F ;
      

      /*if (curRate > 60000) { SetLEDs(0b11110000); }
      if (curRate > 40000) { SetLEDs(0b11100000); }
      if (curRate > 20000) { SetLEDs(0b11000000); }
      if (curRate > 10000) { SetLEDs(0b10000000); }
      if (curRate < 1000)  { SetLEDs(0b00000000); }
      */


      OC1R    = 0x0F;  //Duty Cycle
      OC1RS   = 0x0F;  /* set the period */

      /* configure PWM */
      OC1CON2 = 0x001F;   /* Sync with This OC module                               */
      OC1CON1 = 0x1C08;   /* Clock sourc Fcyc, trigger mode 1, Mode 0 (disable OC1) */

      /* enable the PWM */
      OC1CON1 = OC1CON1 | 0x0006;   /* Mode 6, Edge-aligned PWM Mode */

      /* Make pin RP31(RF13) OC1 (PWM output) */
      //RPOR15bits.RP31R = 18;    //Change for output

      RPOR1bits.RP3R = 18;


      /*
      if (curRate > 1000) {
          SetLEDs(0b10000000);;

          if (curRate > 20000) {
              SetLEDs(0b11000000);
              //LED6_On();
              //LED5_Off();
              //LED4_Off();
          }
          if (curRate > 40000) {
              LED5_On();
          }

          if (curRate > 60000) {
              LED4_On();
          }

       * OC1R    = curRate
       * 
       */





      //PORTDbits.RD10 = 18;
      //RP3R = 18;
  }

      



  

  void SetDutyCycle_PWM(unsigned int DutyCycle) {

    OC1R = DutyCycle;
    OC1RS = DutyCycle - 1;  /*  with 488Hz frquency we have 15bits resolution   */

}

 void SetPWM(BYTE potPercentage) {
        DWORD temp = PWM_PERIOD * potPercentage;
        
        temp = temp / 100;

        SetDutyCycle_PWM(temp);

        

  }

 

  

