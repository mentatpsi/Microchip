/*************************************************************************
 *  © 2012 Microchip Technology Inc.                                       
 *  
 *  Project Name:    mTouch Library
 *  FileName:        PICFamilies.h
 *  Dependencies:    None.
 *  Processor:       PIC18, PIC24, dsPIC
 *  Compiler:        HI-TECH, MPLAB C18, XC8, MPLAB C30 and XC16
 *  IDE:             MPLAB® IDE or MPLAB® X                        
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *  Description:     This header contains hardware depended definitions.
 *************************************************************************/
/**************************************************************************
 * MICROCHIP SOFTWARE NOTICE AND DISCLAIMER: You may use this software, and 
 * any derivatives created by any person or entity by or on your behalf, 
 * exclusively with Microchip's products in accordance with applicable
 * software license terms and conditions, a copy of which is provided for
 * your referencein accompanying documentation. Microchip and its licensors 
 * retain all ownership and intellectual property rights in the 
 * accompanying software and in all derivatives here to. 
 * 
 * This software and any accompanying information is for suggestion only. 
 * It does not modify Microchip's standard warranty for its products. You 
 * agree that you are solely responsible for testing the software and 
 * determining its suitability. Microchip has no obligation to modify, 
 * test, certify, or support the software. 
 * 
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER 
 * EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED 
 * WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A 
 * PARTICULAR PURPOSE APPLY TO THIS SOFTWARE, ITS INTERACTION WITH 
 * MICROCHIP'S PRODUCTS, COMBINATION WITH ANY OTHER PRODUCTS, OR USE IN ANY 
 * APPLICATION. 
 * 
 * IN NO EVENT, WILL MICROCHIP BE LIABLE, WHETHER IN CONTRACT, WARRANTY, 
 * TORT (INCLUDING NEGLIGENCE OR BREACH OF STATUTORY DUTY), STRICT 
 * LIABILITY, INDEMNITY, CONTRIBUTION, OR OTHERWISE, FOR ANY INDIRECT, 
 * SPECIAL, PUNITIVE, EXEMPLARY, INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, 
 * FOR COST OR EXPENSE OF ANY KIND WHATSOEVER RELATED TO THE SOFTWARE, 
 * HOWSOEVER CAUSED, EVEN IF MICROCHIP HAS BEEN ADVISED OF THE POSSIBILITY 
 * OR THE DAMAGES ARE FORESEEABLE. TO THE FULLEST EXTENT ALLOWABLE BY LAW, 
 * MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN ANY WAY RELATED TO THIS 
 * SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY, THAT YOU HAVE PAID 
 * DIRECTLY TO MICROCHIP FOR THIS SOFTWARE. 
 * 
 * MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF 
 * THESE TERMS. 
 *************************************************************************/

#ifndef _PIC_FAMILIES_H_
#define _PIC_FAMILIES_H_
	
#if defined(__18F24J50) || defined(__18F25J50) || defined(__18F26J50) || defined(__18F44J50)  || defined(__18F45J50)\
|| defined(__18LF24J50) || defined(__18LF25J50)|| defined(__18LF26J50)|| defined(__18LF44J50)|| defined(__18LF45J50)\
|| defined(__18LF46J50) || defined(__18F46J50)

#define FAMILY_PIC18XXJ50

#elif defined(__18F26J53) || defined(__18F27J53) ||defined(__18F46J53) || defined(__18F47J53)\
|| defined(__18LF26J53) || defined(__18LF27J53)||defined(__18LF46J53) || defined(__18LF47J53)
	
#define FAMILY_PIC18XXJ53

#elif defined (__18F24J11) || defined(__18F25J11) || defined(__18F26J11)|| defined(__18F44J11)  || defined(__18F45J11) || defined(__18F46J11)\
|| defined(__18LF24J11) || defined(__18LF25J11)|| defined(__18LF26J11)|| defined(__18LF44J11) || defined(__18LF45J11)|| defined(__18LF46J11)

#define FAMILY_PIC18XXJ11

#elif defined(__18F26J13) || defined(__18F27J13) || defined(__18F46J13)|| defined(__18F47J13)\
|| defined(__18LF26J13) || defined(__18LF27J13)|| defined(__18LF46J13)|| defined(__18LF47J13) 

#define FAMILY_PIC18XXJ13

#elif defined(__18F65K22) || defined(__18F66K22) || defined(__18F67K22)\
|| defined(__18F85K22) || defined(__18F86K22) || defined(__18F87K22)

#define FAMILY_PIC18XXK22

#elif defined (__18F23K22) || defined (__18F24K22) || defined(__18F25K22) || defined(__18F26K22)|| defined(__18F43K22) || defined(__18F44K22)  || defined(__18F45K22) || defined(__18F46K22)\
|| defined (__18LF23K22) || defined(__18LF24K22) || defined(__18LF25K22)|| defined(__18LF26K22)|| defined(__18LF43K22) || defined(__18LF44K22) || defined(__18LF45K22)|| defined(__18LF46K22)

#define FAMILY_PIC18_2x_4xK22

#elif defined(__18F66J90) || defined(__18F67J90) || defined(__18F86J90) || defined(__18F87J90)

#define FAMILY_PIC18XXJ90

#elif defined(__18F66J93) || defined(__18F67J93) || defined(__18F86J93) || defined(__18F87J93)

#define FAMILY_PIC18XXJ93

#elif defined(__18F86J72) || defined(__18F87J72)

#define FAMILY_PIC18XXJ72

#elif defined(__18F65K90) || defined(__18F66K90) || defined(__18F67K90)\
|| defined(__18F85K90) || defined(__18F86K90) || defined(__18F87K90)

#define FAMILY_PIC18XXK90

#elif defined(__18F65J94)|| defined(__18F66J94)|| defined(__18F67J94) || defined(__18F85J94) || defined(__18F86J94)\
|| defined(__18F87J94) || defined(__18F95J94) || defined(__18F96J94)|| defined(__18F97J94)|| defined(__18F96J99) || defined(__18F86J99)|| defined(__18F66J99)

#define FAMILY_PIC18XXJ94

#elif defined(__18F24K50)|| defined(__18F25K50)|| defined(__18F45K50)\
|| defined(__18LF24K50)|| defined(__18LF25K50)|| defined(__18LF45K50)

#define FAMILY_PIC18XXK50

#elif defined(__PIC24F04KA200__) || defined(__PIC24F04KA201__) 

#define FAMILY_PIC24F04KA20X

#elif defined(__PIC24F08KA101__) || defined(__PIC24F08KA102__) || defined(__PIC24F16KA101__ ) || defined(__PIC24F16KA102__ )

#define FAMILY_PIC24FXXKA10X

#elif defined(__PIC24F16KA301__)|| defined (__PIC24F16KA302__) || defined(__PIC24F16KA304__)\
|| defined(__PIC24F32KA301__) || defined (__PIC24F32KA302__) || defined(__PIC24F32KA304__)\
|| defined(__PIC24FV16KA301__)|| defined (__PIC24FV16KA302__)|| defined(__PIC24FV16KA304__)\
|| defined(__PIC24FV32KA301__)|| defined (__PIC24FV32KA302__)|| defined(__PIC24FV32KA304__)

#define FAMILY_PIC24FXXKA30X

#elif defined(__PIC24FJ32GA102__) || defined(__PIC24FJ32GA104__) || defined(__PIC24FJ64GA102__) || defined(__PIC24FJ64GA104__)

#define FAMILY_PIC24FJXXGA10X

#elif defined(__PIC24FJ32GB002__) || defined(__PIC24FJ32GB004__) || defined(__PIC24FJ64GB002__) || defined(__PIC24FJ64GB004__)

#define FAMILY_PIC24FJXXGB10X

#elif defined(__PIC24FJ64GA106__) || defined(__PIC24FJ64GA108__)  || defined(__PIC24FJ64GA110__)\
|| defined(__PIC24FJ128GA106__)|| defined(__PIC24FJ128GA108__) || defined(__PIC24FJ128GA110__)\
|| defined(__PIC24FJ192GA106__)|| defined(__PIC24FJ192GA108__) || defined(__PIC24FJ192GA110__)\
|| defined(__PIC24FJ256GA106__)|| defined(__PIC24FJ256GA108__) || defined(__PIC24FJ256GA110__)

#define FAMILY_PIC24FJXXXGA1XX

#elif defined(__PIC24FJ64GB106__) || defined(__PIC24FJ64GB108__)  || defined(__PIC24FJ64GB110__)\
|| defined(__PIC24FJ128GB106__)|| defined(__PIC24FJ128GB108__) || defined(__PIC24FJ128GB110__)\
|| defined(__PIC24FJ192GB106__)|| defined(__PIC24FJ192GB108__) || defined(__PIC24FJ192GB110__)\
|| defined(__PIC24FJ256GB106__)|| defined(__PIC24FJ256GB108__) || defined(__PIC24FJ256GB110__)

#define FAMILY_PIC24FJXXXGB1XX

#elif defined(__PIC24FJ128GB206__) || defined (__PIC24FJ128GB210__)	|| defined(__PIC24FJ256GB206__) || defined (__PIC24FJ256GB210__)

#define FAMILY_PIC24FJXXXGB2XX

#elif defined(__PIC24FJ128DA106__) || defined(__PIC24FJ128DA110__) ||  defined(__PIC24FJ128DA206__)\
|| defined(__PIC24FJ128DA210__) ||  defined(__PIC24FJ256DA106__)|| defined(__PIC24FJ256DA110__)\
|| defined(__PIC24FJ256DA206__) || defined(__PIC24FJ256DA210__ ) 

#define FAMILY_PIC24FJXXXDAXXX

#elif defined(__PIC24FJ64GA306__)   ||  defined(__PIC24FJ64GA308__)  ||  defined(__PIC24FJ64GA310__)\
||  defined(__PIC24FJ128GA306__)  ||  defined(__PIC24FJ128GA308__) || defined(__PIC24FJ128GA310__)

#define FAMILY_PIC24FJXXXGA3XX

#elif defined(__dsPIC33FJ12GP201__)||defined(__dsPIC33FJ12GP202__)

#define FAMILY_dsPIC33FJ12GP201_202

#elif defined(__dsPIC33FJ32GP202__)||defined(__dsPIC33FJ32GP204__)||defined(__dsPIC33FJ16GP304__)

#define FAMILY_dsPIC33FJ32GP202_204_dsPIC33FJ16GP304

#elif defined(__dsPIC33FJ128GP804__)||defined(__dsPIC33FJ128GP802__)||defined(__dsPIC33FJ128GP204__)||\
    defined(__dsPIC33FJ128GP202__)||defined(__dsPIC33FJ64GP804__)||defined(__dsPIC33FJ64GP802__)||\
    defined(__dsPIC33FJ64GP204__)||defined(__dsPIC33FJ64GP202__)||defined(__dsPIC33FJ32GP304__)||defined(__dsPIC33FJ32GP302__)

#define FAMILY_dsPIC33FJ32GP302_304_dsPIC33FJ64GPX02_X04_dsPIC33FJ128GPX02_X04 

#elif defined(__dsPIC33FJ64GP206A__)||defined(__dsPIC33FJ64GP306A__)||defined(__dsPIC33FJ64GP310A__)||\
    defined(__dsPIC33FJ64GP706A__)||defined(__dsPIC33FJ64GP708A__)||defined(__dsPIC33FJ64GP710A__)||\
    defined(__dsPIC33FJ128GP206A__)||defined(__dsPIC33FJ128GP306A__)||defined(__dsPIC33FJ128GP310A__)||\
    defined(__dsPIC33FJ128GP706A__)||defined(__dsPIC33FJ128GP708A__)||defined(__dsPIC33FJ128GP710A__)||\
    defined(__dsPIC33FJ256GP506A__)||defined(__dsPIC33FJ256GP510A__)||defined(__dsPIC33FJ256GP710A__)

#define FAMILY_dsPIC33FJXXXGPX06A_X08A_X10A 

#elif defined(__PIC24EP32GP202__)||defined(__PIC24EP64GP202__)||defined(__PIC24EP128GP202__)||defined(__PIC24EP256GP202__)||\
    defined(__PIC24EP512GP202__)||defined(__PIC24EP32GP203__)||defined(__PIC24EP64GP203__)||defined(__PIC24EP32GP204__)||\
    defined(__PIC24EP64GP204__)||defined(__PIC24EP128GP204__)||defined(__PIC24EP256GP204__)||defined(__PIC24EP512GP204__)||\
    defined(__PIC24EP64GP206__)||defined(__PIC24EP128GP206__)||defined(__PIC24EP256GP206__)||defined(__PIC24EP512GP206__)||\
    defined(__dsPIC33EP32GP502__)||defined(__dsPIC33EP64GP502__)||defined(__dsPIC33EP128GP502__)||defined(__dsPIC33EP256GP502__)||\
    defined(__dsPIC33EP512GP502__)||defined(__dsPIC33EP32GP503__)||defined(__dsPIC33EP64GP503__)||defined(__dsPIC33EP32GP504__)||\
    defined(__dsPIC33EP64GP504__)||defined(__dsPIC33EP128GP504__)||defined(__dsPIC33EP256GP504__)||defined(__dsPIC33EP512GP504__)||\
    defined(__dsPIC33EP64GP506__)||defined(__dsPIC33EP128GP506__)||defined(__dsPIC33EP256GP506__)||defined(__dsPIC33EP512GP506__)||\
    defined(__PIC24EP32MC202__)||defined(__PIC24EP64MC202__)||defined(__PIC24EP128MC202__)||defined(__PIC24EP256MC202__)||\
    defined(__PIC24EP512MC202__)||defined(__PIC24EP32MC203__)||defined(__PIC24EP64MC203__)||defined(__PIC24EP32MC204__)||\
    defined(__PIC24EP64MC204__)||defined(__PIC24EP128MC204__)||defined(__PIC24EP256MC204__)||defined(__PIC24EP512MC204__)||\
    defined(__PIC24EP64MC206__)||defined(__PIC24EP128MC206__)||defined(__PIC24EP256MC206__)||defined(__PIC24EP512MC206__)||\
    defined(__dsPIC33EP32MC202__)||defined(__dsPIC33EP64MC202__)||defined(__dsPIC33EP128MC202__)||defined(__dsPIC33EP256MC202__)||\
    defined(__dsPIC33EP512MC202__)||defined(__dsPIC33EP32MC203__)||defined(__dsPIC33EP64MC203__)||defined(__dsPIC33EP32MC204__)||\
    defined(__dsPIC33EP64MC204__)||defined(__dsPIC33EP128MC204__)||defined(__dsPIC33EP256MC204__)||defined(__dsPIC33EP512MC204__)||\
    defined(__dsPIC33EP64MC206__)||defined(__dsPIC33EP128MC206__)||defined(__dsPIC33EP256MC206__)||defined(__dsPIC33EP512MC206__)||\
    defined(__dsPIC33EP32MC502__)||defined(__dsPIC33EP64MC502__)||defined(__dsPIC33EP128MC502__)||defined(__dsPIC33EP256MC502__)||\
    defined(__dsPIC33EP512MC502__)||defined(__dsPIC33EP32MC503__)||defined(__dsPIC33EP64MC503__)||defined(__dsPIC33EP32MC504__)||\
    defined(__dsPIC33EP64MC504__)||defined(__dsPIC33EP128MC504__)||defined(__dsPIC33EP256MC504__)||defined(__dsPIC33EP512MC504__)||\
    defined(__dsPIC33EP64MC506__)||defined(__dsPIC33EP128MC506__)||defined(__dsPIC33EP256MC506__)||defined(__dsPIC33EP512MC506__)

#define FAMILY_dsPIC33EPXXXGP50X_dsPIC33EPXXXMC20X_50X_PIC24EPXXXGP_MC20X 

#elif defined(__dsPIC33EP256MU806__)||defined(__dsPIC33EP256MU810__)||defined(__dsPIC33EP256MU814__)||defined(__dsPIC33EP512GP806__)||\
    defined(__dsPIC33EP512MC806__)||defined(__dsPIC33EP512MU810__)||defined(__dsPIC33EP512MU814__)||defined(__PIC24EP256GU810__)||\
    defined(__PIC24EP256GU814__)||defined(__PIC24EP512GP806__)||defined(__PIC24EP512GU810__)||defined(__PIC24EP512GU814__)

#define FAMILY_dsPIC33EPXXX_GP_MC_MU_806_810_814_PIC24EPXXX_GP_GU_810_814

#elif defined(__dsPIC30F2011__)||defined(__dsPIC30F2012__)||defined(__dsPIC30F3012__)||defined(__dsPIC30F3013__)

#define FAMILY_dsPIC30F2011_2012_3012_3013

#elif defined(__dsPIC30F3014__)||defined(__dsPIC30F4013__)

#define FAMILY_dsPIC30F3014_4013 

#elif defined(__dsPIC30F5011__)||defined(__dsPIC30F5013__)

#define FAMILY_dsPIC30F5011_5013 

#elif defined(__dsPIC30F6011A__)||defined(__dsPIC30F6012A__)||defined(__dsPIC30F6013A__)||defined(__dsPIC30F6014A__)

#define FAMILY_dsPIC30F6011A_6012A_6013A_6014A

#elif defined(__PIC24HJ12GP201__)||defined(__PIC24HJ12GP202__)
 
#define FAMILY_PIC24HJ12GP201_202

#elif defined(__PIC24HJ32GP202__)||defined(__PIC24HJ32GP204__)||defined(__PIC24HJ16GP304__)

#define FAMILY_PIC24HJ32GP202_204_PIC24HJ16GP304 

#elif defined(__PIC24HJ128GP504__)||defined(__PIC24HJ128GP502__)||defined(__PIC24HJ128GP204__)||defined(__PIC24HJ128GP202__)||\
    defined(__PIC24HJ64GP504__)||defined(__PIC24HJ64GP502__)||defined(__PIC24HJ64GP204__)||defined(__PIC24HJ64GP202__)||\
    defined(__PIC24HJ32GP304__)||defined(__PIC24HJ32GP302__)

#define FAMILY_PIC24HJ32GP302_304_PIC24HJ64GPX02_X04_PIC24HJ128GPX02_X04 

#elif defined(__PIC24HJ64GP206A__)||defined(__PIC24HJ64GP210A__)||defined(__PIC24HJ64GP506A__)||defined(__PIC24HJ64GP510A__)||\
    defined(__PIC24HJ128GP206A__)||defined(__PIC24HJ128GP210A__)||defined(__PIC24HJ128GP506A__)||defined(__PIC24HJ128GP510A__)||\
    defined(__PIC24HJ128GP306A__)||defined(__PIC24HJ128GP310A__)||defined(__PIC24HJ256GP206A__)||defined(__PIC24HJ256GP210A__)||\
    defined(__PIC24HJ256GP610A__)

#define FAMILY_PIC24HJXXXGPX06A_X08A_X10A 

#else

#error The selected PIC device is not supported in this demo.

#endif

#endif
