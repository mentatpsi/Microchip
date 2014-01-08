 /*
 ** file: main.c
 ** target: PIC24FJ256GB110
 ** compiler: C30 version 3.11
 ** IDE: MPLAB 8.30
 **
 ** This works in the real hardware but the
 ** PWM does not work in the simulator.
 */

 #include <p24Fxxxx.h>

#include "USB/usb.h"
#include "USB/usb_host_android.h"
#include "Compiler.h"
#include "HardwareProfile.h"

 /*
 ** Only one invocation of CONFIGx should appear in a project,
 ** at the top of a C source file (outside of any function).
 **
 */
     _CONFIG3( WPEND_WPENDMEM & /* Write Protect from WPFP to the last page of memory */
               WPCFG_WPCFGDIS & /* Write Protect Configuration Page Disabled */
               WPDIS_WPDIS    & /* Write Protection Disable */

               WPFP_WPFP511 )   /* Write Protection Flash Page 511 */

     _CONFIG2( IESO_OFF       & /* Two Speed Start-up Disabled */
               FNOSC_FRC      & /* Oscillator Selection Fast RC oscillator */
               PLL_96MHZ_ON   & /* 96MHz PLL Enabled */
               PLLDIV_DIV2    & /* 96MHz PLL Prescaler Divide by 2 (8MHz input) */
               FCKSM_CSDCMD   & /* Clock switching and clock monitor Both disabled */
               OSCIOFNC_ON    & /* OSCO/RC15 function RC15 */
               IOL1WAY_OFF    & /* RP Register Protection Unlimited Writes To RP Registers */
               DISUVREG_OFF   & /* Disable Internal USB Voltage Regulator Disabled */
               FNOSC_PRIPLL   & /* PLL, USBPLL /2*/
               POSCMOD_HS     & /* Primary HS OSC*/
               POSCMOD_NONE );  /* Oscillator Selection Primary disabled */

      _CONFIG1( JTAGEN_OFF     & /* JTAG Disabled */
                GCP_OFF        & /* Code Protect Disabled */
                GWRP_OFF       & /* Write Protect Disabled */
                ICS_PGx2       & /* ICD pins select EMUC/EMUD share PGC1/PGD1 */
                FWDTEN_OFF     & /* Watchdog Timer Disabled */
                WINDIS_OFF     & /* Windowed WDT Disabled */
                FWPSA_PR128    & /* Watchdog prescaler 1:128 */
                WDTPS_PS32768 ); /* Watchdog postscale 1:32,768 */

  /* PWM example
  ** Assign OC1 to be output on RF13
  **
  */
  #define  PWM_PERIOD 62500
  void
  PwmInit(
      void
     )
  {
      /* Unmap RP31(RF13) */
      RPOR15bits.RP31R = 0;


      RPOR1bits.RP3R = 0;
      LATDbits.LATD10 = 0;
      TRISDbits.TRISD10 = 0;

      /* Drive RF13 low and make it an output */
      LATFbits.LATF13 = 0;
      TRISFbits.TRISF13 = 0;
      Nop();

      /* Reset PWM */
      OC1CON1 = 0x0000;
      OC1CON2 = 0x0000;

      /* set PWM duty cycle to 50% */
      OC1R    = PWM_PERIOD >> 1; /* set the duty cycle tp 50% */
      OC1RS   = PWM_PERIOD - 1;  /* set the period */

      /* configure PWM */
      OC1CON2 = 0x001F;   /* Sync with This OC module                               */
      OC1CON1 = 0x1C08;   /* Clock sourc Fcyc, trigger mode 1, Mode 0 (disable OC1) */

      /* enable the PWM */
      OC1CON1 = OC1CON1 | 0x0006;   /* Mode 6, Edge-aligned PWM Mode */

      /* Make pin RP31(RF13) OC1 (PWM output) */
      RPOR15bits.RP31R = 18;
      RPOR1bits.RP3R = 18;
  }

  int
  main(
      void
      )
  {

      CLKDIV =  0; /* set for default clock operations Fcyc = 4MHz */
      AD1PCFGL = 0xffff;
      AD1PCFGH = 0x0003;

      PwmInit();
      for( ; ; );     /* hang here forever */
      return 0;
  }
 