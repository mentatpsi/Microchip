#include "USB/usb.h"
#include "USB/usb_host_android.h"
#include "Compiler.h"
#include "HardwareProfile.h"


#ifndef MAX_ALLOWED_CURRENT
    #define MAX_ALLOWED_CURRENT             (500)         // Maximum power we can supply in mA
#endif


/****************************************************************************
  Function:
    void SetLEDs(BYTE setting)

  Summary:
    change the LED settings of the boards

  Description:
    change the LED settings of the boards

  Precondition:
    None

  Parameters:
    BYTE setting - bitmap for desired LED setting (1 = On, 0 = Off)
        bit 0 = LED 0
        bit 1 = LED 1
        bit 2 = LED 2
        ...
        bit 7 = LED 7

  Return Values:
    None

  Remarks:
    None
  ***************************************************************************/
static void SetLEDs(BYTE setting)
{
    if((setting & 0x01) == 0x01) { LED0_On(); } else { LED0_Off(); }
    if((setting & 0x02) == 0x02) { LED1_On(); } else { LED1_Off(); }
    if((setting & 0x04) == 0x04) { LED2_On(); } else { LED2_Off(); }
    if((setting & 0x08) == 0x08) { LED3_On(); } else { LED3_Off(); }
    if((setting & 0x10) == 0x10) { LED4_On(); } else { LED4_Off(); }
    if((setting & 0x20) == 0x20) { LED5_On(); } else { LED5_Off(); }
    if((setting & 0x40) == 0x40) { LED6_On(); } else { LED6_Off(); }
    if((setting & 0x80) == 0x80) { LED7_On(); } else { LED7_Off(); }
}


/****************************************************************************
  Function:
    BYTE GetPushbuttons(void)

  Summary:
    Reads the current push button status.

  Description:
    Reads the current push button status.

  Precondition:
    None

  Parameters:
    None

  Return Values:
    BYTE - bitmap for button representations (1 = pressed, 0 = not pressed)
        bit 0 = button 1
        bit 1 = button 2
        bit 2 = button 3
        bit 3 = button 4

  Remarks:
    None
  ***************************************************************************/
static BYTE GetPushbuttons(void)
{
    BYTE toReturn;

    InitAllSwitches();

    toReturn = 0;

    if(Switch1Pressed()){ toReturn |= 0x1; }
    if(Switch2Pressed()){ toReturn |= 0x2; }
    if(Switch3Pressed()){ toReturn |= 0x4; }
    if(Switch4Pressed()){ toReturn |= 0x8; }
    
    return toReturn;
}

/****************************************************************************
  Function:
    BYTE ReadPOT(void)

  Summary:
    Reads the pot value and returns the percentage of full scale (0-100)

  Description:
    Reads the pot value and returns the percentage of full scale (0-100)

  Precondition:
    A/D is initialized properly

  Parameters:
    None

  Return Values:
    None

  Remarks:
    None
  ***************************************************************************/
static BYTE ReadPOT(void)
{
    WORD_VAL w;
    DWORD temp;

    #if defined(__18CXX)
        mInitPOT();

        ADCON0bits.GO = 1;              // Start AD conversion
        while(ADCON0bits.NOT_DONE);     // Wait for conversion

        w.v[0] = ADRESL;
        w.v[1] = ADRESH;

    #elif defined(__C30__) || defined(__C32__) || defined __XC16__
        #if defined(PIC24FJ256GB110_PIM) || \
            defined(PIC24FJ256DA210_DEV_BOARD) || \
            defined(PIC24FJ256GB210_PIM)

            AD1CHS = 0x5;           //MUXA uses AN5

            // Get an ADC sample
            AD1CON1bits.SAMP = 1;           //Start sampling
            for(w.Val=0;w.Val<1000;w.Val++){Nop();} //Sample delay, conversion start automatically
            AD1CON1bits.SAMP = 0;           //Start sampling
            for(w.Val=0;w.Val<1000;w.Val++){Nop();} //Sample delay, conversion start automatically
            while(!AD1CON1bits.DONE);       //Wait for conversion to complete

            temp = (DWORD)ADC1BUF0;
            temp = temp * 100;
            temp = temp/1023;

        #elif defined(PIC24F_ADK_FOR_ANDROID)
            AD1CHS = 0x9;           //MUXA uses AN9

            // Get an ADC sample
            AD1CON1bits.SAMP = 1;           //Start sampling
            for(w.Val=0;w.Val<1000;w.Val++){Nop();} //Sample delay, conversion start automatically
            AD1CON1bits.SAMP = 0;           //Start sampling
            for(w.Val=0;w.Val<1000;w.Val++){Nop();} //Sample delay, conversion start automatically
            while(!AD1CON1bits.DONE);       //Wait for conversion to complete

            temp = (DWORD)ADC1BUF0;
            temp = temp * 100;
            temp = temp/1023;

        #elif defined(PIC24FJ64GB004_PIM) || defined(PIC24FJ64GB502_MICROSTICK)
            AD1CHS = 0x7;           //MUXA uses AN7

            // Get an ADC sample
            AD1CON1bits.SAMP = 1;           //Start sampling
            for(w.Val=0;w.Val<1000;w.Val++){Nop();} //Sample delay, conversion start automatically
            AD1CON1bits.SAMP = 0;           //Start sampling
            for(w.Val=0;w.Val<1000;w.Val++){Nop();} //Sample delay, conversion start automatically
            while(!AD1CON1bits.DONE);       //Wait for conversion to complete

            temp = (DWORD)ADC1BUF0;
            temp = temp * 100;
            temp = temp/1023;

        #elif defined(PIC24F_STARTER_KIT)
            AD1CHS = 0x0;           //MUXA uses AN0

            // Get an ADC sample
            AD1CON1bits.SAMP = 1;           //Start sampling
            for(w.Val=0;w.Val<1000;w.Val++){Nop();} //Sample delay, conversion start automatically
            AD1CON1bits.SAMP = 0;           //Start sampling
            for(w.Val=0;w.Val<1000;w.Val++){Nop();} //Sample delay, conversion start automatically
            while(!AD1CON1bits.DONE);       //Wait for conversion to complete

            temp = (DWORD)ADC1BUF0;
            temp = temp * 100;
            temp = temp/1023;

        #elif defined(PIC32MX460F512L_PIM) || defined(PIC32MX795F512L_PIM)
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
            for(w.Val=0;w.Val<1000;w.Val++){Nop();} //Sample delay, conversion start automatically
            AD1CON1CLR = 0x0002; // start Converting
            while (!(AD1CON1 & 0x0001));// conversion done?

            temp = (DWORD)ADC1BUF0;
            temp = temp * 100;
            temp = temp/1023;

        #elif   defined(PIC32_USB_STARTER_KIT) || \
                defined (DSPIC33E_USB_STARTER_KIT) || \
                defined(PIC32_ETHERNET_STARTER_KIT) || \
                defined(DSPIC33EP512MU810_PIM) || \
                defined (PIC24EP512GU810_PIM) || \
                defined (DSPIC33E_USB_STARTER_KIT) || \
                defined (CEREBOT_32MX7)
            //Doesn't have a Pot
            w.Val = 50;
            temp = 50;
        #else
            #error
        #endif

    #endif

    return (BYTE)temp;
}//end ReadPOT
