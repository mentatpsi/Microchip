/*************************************************************************
 *  © 2012 Microchip Technology Inc.                                       
 *  
 *  Project Name:    mTouch Library
 *  FileName:        system.h
 *  Dependencies:    Compiler.h, GenericTypeDefs.h, mTouchConfig.h, PICFamilies.h
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
 * accompanying software and in all derivatives hereto. 
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

#ifndef _SYSTEM_H_
#define _SYSTEM_H_

#include "Compiler.h"
#include "GenericTypeDefs.h"
#include "mTouchConfig.h"
#include "PICFamilies.h"


#ifdef _SYSTEM_C_
// PRIVATE
		
/****************************************************************************
 Configuration bits settings.
 ***************************************************************************/

#if defined(FAMILY_PIC18XXJ50)

#if defined(__PICC18__)		
__CONFIG(1, WDTEN_OFF & XINST_OFF & PLLDIV_2 & CPUDIV_OSC1);
__CONFIG(2, OSC_HSPLL & WDTPS_128);
__CONFIG(3, DSBOREN_OFF & DSWDTEN_OFF & IOL1WAY_OFF);		
#else		
#pragma config WDTEN = OFF, XINST = OFF, PLLDIV = 2, CPUDIV = OSC1 
#pragma config OSC=HSPLL, DSBOREN = OFF, IOL1WAY = OFF 
#endif

#elif defined(FAMILY_PIC18XXJ53)

#if defined(__PICC18__)		
__CONFIG(1, WDTEN_OFF & XINST_OFF & PLLDIV_2 & CPUDIV_OSC1);
__CONFIG(2, OSC_HSPLL & WDTPS_128);
__CONFIG(3, DSBOREN_OFF & DSWDTEN_OFF & IOL1WAY_OFF);		
#else		
#pragma config WDTEN = OFF, XINST = OFF, PLLDIV = 2, CPUDIV = OSC1 
#pragma config OSC=HSPLL, DSBOREN = OFF, IOL1WAY = OFF 
#endif

#elif defined(FAMILY_PIC18XXJ11)

#if defined(__PICC18__)		
__CONFIG(1, WDTEN_OFF & XINST_OFF);
__CONFIG(2, OSC_HSPLL);
__CONFIG(3, DSBOREN_OFF & DSWDTEN_OFF & IOL1WAY_OFF);		
#else	
#pragma config WDTEN = OFF, XINST = OFF 
#pragma config OSC=HSPLL, DSBOREN = OFF, IOL1WAY = OFF 
#endif

#elif defined(FAMILY_PIC18XXJ13)

#if defined(__PICC18__)		
__CONFIG(1, WDTEN_OFF & XINST_OFF);
__CONFIG(2, OSC_HSPLL);
__CONFIG(3, DSBOREN_OFF & DSWDTEN_OFF & IOL1WAY_OFF);		
#else	
#pragma config WDTEN = OFF, XINST = OFF 
#pragma config OSC=HSPLL, DSBOREN = OFF, IOL1WAY = OFF 
#endif

#elif defined(FAMILY_PIC18XXK22)								

#if defined(__PICC18__)		
__CONFIG(1, RETEN_OFF & XINST_OFF & FOSC_HS1 & PLLCFG_OFF);
__CONFIG(2, BOREN_OFF & WDTEN_OFF);		
#else	
#pragma config XINST = OFF, FOSC = HS1, BOREN = OFF
#pragma config WDTEN = OFF, MCLRE = ON
#endif	

#elif defined(FAMILY_PIC18_2x_4xK22)

#if defined(__PICC18__)		
__CONFIG(1, XINST_OFF & FOSC_HSMP & PLLCFG_OFF);
__CONFIG(2, BOREN_OFF & WDTEN_OFF);		
#else	
#pragma config XINST = OFF, FOSC = HSMP, BOREN = OFF
#pragma config WDTEN = OFF, MCLRE = EXTMCLR	
#endif	

#elif defined(FAMILY_PIC18XXJ90)	
#if defined(__PICC18__)		
__CONFIG(1, WDTEN_OFF & XINST_OFF);
__CONFIG(2, OSC_HSPLL & WDTPS_128);
__CONFIG(3, WDTEN_OFF );		
#else	
#pragma config WDTEN = OFF, XINST = OFF, OSC=HSPLL 	
#endif	

#elif defined(FAMILY_PIC18XXJ93)

#if defined(__PICC18__)		
__CONFIG(1, WDTEN_OFF & XINST_OFF);
__CONFIG(2, OSC_HSPLL & WDTPS_128);
__CONFIG(3, WDTEN_OFF);		
#else	
#pragma config WDTEN = OFF, XINST = OFF, OSC=HSPLL 	
#endif	

#elif defined(FAMILY_PIC18XXJ72)

#if defined(__PICC18__)		
__CONFIG(1, WDTEN_OFF & XINST_OFF );
__CONFIG(2, OSC_HSPLL & WDTPS_128);
__CONFIG(3, WDTEN_OFF );		
#else	
#pragma config WDTEN = OFF, XINST = OFF, OSC=HSPLL 	
#endif	

#elif defined(FAMILY_PIC18XXK90)

#if defined(__PICC18__)		
__CONFIG(1, WDTEN_OFF & XINST_OFF & PLLCFG_ON & FOSC_HS1);
__CONFIG(2, WDTPS_128);
__CONFIG(3, BOREN_OFF & WDTEN_OFF);		
#else	
#pragma config XINST = OFF, FOSC = HS1,PLLCFG = ON, BOREN = OFF
#pragma config WDTEN = OFF, MCLRE = ON
#endif

#elif defined(FAMILY_PIC18XXJ94)																

#if defined(__PICC18__)		
__CONFIG(1, RETEN_OFF & XINST_OFF & POSCMD_HS & PLLDIV_PLL4X);		
__CONFIG(2, BOREN_OFF & WDTEN_OFF);		
#else	
#pragma config XINST = OFF,POSCMD = HS,BOREN = OFF
#pragma config FWDTEN = OFF
#endif	

#elif defined(FAMILY_PIC18XXK50)									

#if defined(__PICC18__)		
__CONFIG(1, XINST_OFF & FOSC_XT & CFGPLLEN_OFF);
__CONFIG(2, BOREN_OFF & WDTEN_OFF);		
#else	
#pragma config XINST = OFF, BOREN = OFF
#pragma config WDTEN = OFF, MCLRE = ON
#endif	

#elif defined(FAMILY_PIC24F04KA20X)

_FICD( ICS_PGx2)
_FWDT( FWDTEN_OFF )
_FPOR( MCLRE_ON & BOREN_BOR0)
_FDS( DSWDTEN_OFF & DSLPBOR_OFF)
_FOSCSEL( FNOSC_PRIPLL & IESO_OFF )
_FOSC( FCKSM_CSDCMD & POSCFREQ_HS & POSCMOD_HS )

#elif defined(FAMILY_PIC24FXXKA10X)

_FICD( ICS_PGx2)
_FWDT( FWDTEN_OFF )
_FPOR( MCLRE_ON & BOREN_BOR0)
_FDS( DSWDTEN_OFF & DSBOREN_OFF)
_FOSCSEL( FNOSC_PRIPLL & IESO_OFF )
_FOSC( FCKSM_CSDCMD & POSCFREQ_HS & POSCMOD_HS )

#elif defined(FAMILY_PIC24FXXKA30X)
			
_FICD( ICS_PGx2)
_FPOR( MCLRE_ON & BOREN_BOR0)
_FDS( DSWDTEN_OFF & DSBOREN_OFF)
_FOSCSEL( FNOSC_PRIPLL & IESO_OFF )
_FOSC( FCKSM_CSDCMD & POSCFREQ_HS & POSCMOD_HS )

#elif defined(FAMILY_PIC24FJXXGA10X)

_CONFIG1( JTAGEN_OFF & GCP_OFF & GWRP_OFF & ICS_PGx2 & FWDTEN_OFF)
_CONFIG2( IESO_OFF & FNOSC_PRIPLL & FCKSM_CSDCMD & IOL1WAY_OFF & POSCMOD_HS )
_CONFIG4( DSWDTEN_OFF & DSBOREN_OFF)

#elif defined(FAMILY_PIC24FJXXGB10X)

_CONFIG1( JTAGEN_OFF & GCP_OFF & GWRP_OFF & ICS_PGx2 & FWDTEN_OFF)
_CONFIG2( IESO_OFF & PLLDIV_DIV2 & FNOSC_PRIPLL & FCKSM_CSDCMD & IOL1WAY_OFF & POSCMOD_HS )
_CONFIG4( DSWDTEN_OFF & DSBOREN_OFF)

#elif defined(FAMILY_PIC24FJXXXGA1XX)

_CONFIG1( JTAGEN_OFF & GCP_OFF & GWRP_OFF & ICS_PGx2 & FWDTEN_OFF)
_CONFIG2( IESO_OFF & FNOSC_PRIPLL & FCKSM_CSDCMD & IOL1WAY_OFF & POSCMOD_HS )

#elif defined(FAMILY_PIC24FJXXXGB1XX)

_CONFIG1( JTAGEN_OFF & GCP_OFF & GWRP_OFF & ICS_PGx2 & FWDTEN_OFF & WDTPS_PS128 & FWPSA_PR32)
_CONFIG2( IESO_OFF & PLLDIV_DIV2 & FNOSC_PRIPLL & FCKSM_CSDCMD & IOL1WAY_OFF & DISUVREG_ON & POSCMOD_HS)

#elif defined(FAMILY_PIC24FJXXXGB2XX)

_CONFIG1( JTAGEN_OFF & GCP_OFF & GWRP_OFF & ICS_PGx2 & FWDTEN_OFF)
_CONFIG2( IESO_OFF & PLLDIV_DIV2 & FNOSC_PRIPLL & FCKSM_CSDCMD & IOL1WAY_OFF & POSCMOD_HS )

#elif defined(FAMILY_PIC24FJXXXDAXXX)

_CONFIG1( WDTPS_PS32768 & FWPSA_PR128 & FWDTEN_OFF & ICS_PGx2 & GWRP_OFF & GCP_OFF & JTAGEN_OFF) 
_CONFIG2( POSCMOD_HS & IOL1WAY_OFF & OSCIOFNC_OFF & FCKSM_CSDCMD & FNOSC_PRIPLL & PLL96MHZ_ON & PLLDIV_DIV2 & IESO_ON)
_CONFIG3( WPFP_WPFP255 & SOSCSEL_SOSC & WUTSEL_LEG & WPDIS_WPDIS & WPCFG_WPCFGDIS & WPEND_WPENDMEM) 

#elif defined(FAMILY_PIC24FJXXXGA3XX)

_CONFIG4(DSWDTEN_OFF)
_CONFIG3(BOREN_OFF)
_CONFIG2(POSCMD_HS & IOL1WAY_OFF & FNOSC_PRIPLL)
_CONFIG1(FWDTEN_WDT_DIS & ICS_PGx2 & JTAGEN_OFF)

#elif defined(FAMILY_dsPIC33FJ12GP201_202)||defined(FAMILY_dsPIC33FJ32GP202_204_dsPIC33FJ16GP304)||\
      defined(FAMILY_dsPIC33FJ32GP302_304_dsPIC33FJ64GPX02_X04_dsPIC33FJ128GPX02_X04)||defined(FAMILY_dsPIC33FJXXXGPX06A_X08A_X10A)||\
      defined(FAMILY_dsPIC33EPXXXGP50X_dsPIC33EPXXXMC20X_50X_PIC24EPXXXGP_MC20X)||defined(FAMILY_dsPIC33EPXXX_GP_MC_MU_806_810_814_PIC24EPXXX_GP_GU_810_814)

_FOSCSEL(FNOSC_FRCPLL)
_FOSC(POSCMD_NONE)
_FWDT(FWDTEN_OFF)

#elif defined(FAMILY_dsPIC30F2011_2012_3012_3013)||defined(FAMILY_dsPIC30F3014_4013)||\
      defined(FAMILY_dsPIC30F6011A_6012A_6013A_6014A)

_FOSC(FRC_PLL16)
_FWDT(WDT_OFF)
_FBORPOR(MCLR_EN)

#elif defined(FAMILY_dsPIC30F5011_5013)

_FOSC(FRC_PLL16 & PRI)
_FWDT(WDT_OFF)
_FBORPOR(MCLR_EN)

#elif defined(FAMILY_PIC24HJ12GP201_202)||defined(FAMILY_PIC24HJ32GP202_204_PIC24HJ16GP304)||\
      defined(FAMILY_PIC24HJ32GP302_304_PIC24HJ64GPX02_X04_PIC24HJ128GPX02_X04)||defined(FAMILY_PIC24HJXXXGPX06A_X08A_X10A)

_FOSCSEL(FNOSC_FRCPLL)
_FOSC(POSCMD_NONE)
_FWDT(FWDTEN_OFF)

#else

#error PIC device family is not defined.

#endif

/****************************************************************************
  Macro:
    SysDelay1Ms()

  Summary:
    Delay execution on 1 mS.

  Description:
    Delay execution on 1 mS.
 
  Precondition:
    None.

  Parameters:
    None.

  Returns:
    None.

  Remarks:
    None.
  ***************************************************************************/
#if defined(__C30__)
#define SysDelay1Ms()  {asm volatile("Repeat #16000"); asm volatile("nop");}
#else
#define SysDelay1Ms()  { UINT16 d=800; while(d--){Nop();} }
#endif

#endif
	
// PUBLIC

#if defined(__C30__)	
// main(...) returns int for C30 compiler
#define RET_MAIN	int
#else
// main(...) returns void for C18 compiler
#define RET_MAIN	void
#endif	

// SENSORS CONNECTIONS

#if defined(FAMILY_PIC24FXXKA10X) || defined(FAMILY_PIC24F04KA20X)

#define CH0_TRIS    &TRISA
#define CH0_LAT     &LATA
#define CH0_IO_BIT  0
#define CH0_AN_NUM  0
	
#define CH1_TRIS    &TRISA
#define CH1_LAT     &LATA
#define CH1_IO_BIT  1
#define CH1_AN_NUM  1
	
#define CH2_TRIS    &TRISA
#define CH2_LAT     &LATA
#define CH2_IO_BIT  4
#define CH2_AN_NUM  2
	
#define CH3_TRIS    &TRISB
#define CH3_LAT     &LATB
#define CH3_IO_BIT  4
#define CH3_AN_NUM  3
	
#define CH4_TRIS    &TRISB
#define CH4_LAT     &LATB
#define CH4_IO_BIT  7
#define CH4_AN_NUM  4
	
#define CH5_TRIS    &TRISB
#define CH5_LAT     &LATB
#define CH5_IO_BIT  8
#define CH5_AN_NUM  5
	
#define CH6_TRIS    &TRISB
#define CH6_LAT     &LATB
#define CH6_IO_BIT  9
#define CH6_AN_NUM  6
	
#define CH7_TRIS    &TRISB
#define CH7_LAT     &LATB
#define CH7_IO_BIT  14
#define CH7_AN_NUM  7
	
#define CH8_TRIS    &TRISB
#define CH8_LAT     &LATB
#define CH8_IO_BIT  15
#define CH8_AN_NUM  8
	
#elif defined(FAMILY_PIC24FJXXGA10X) || defined(FAMILY_PIC24FJXXGB10X)

#define CH0_TRIS    &TRISB
#define CH0_LAT     &LATB
#define CH0_IO_BIT  0
#define CH0_AN_NUM  0
	
#define CH1_TRIS    &TRISB
#define CH1_LAT     &LATB
#define CH1_IO_BIT  1
#define CH1_AN_NUM  1
	
#define CH2_TRIS    &TRISB
#define CH2_LAT     &LATB
#define CH2_IO_BIT  2
#define CH2_AN_NUM  2
	
#define CH3_TRIS    &TRISB
#define CH3_LAT     &LATB
#define CH3_IO_BIT  3
#define CH3_AN_NUM  3
	
#define CH4_TRIS    &TRISB
#define CH4_LAT     &LATB
#define CH4_IO_BIT  4
#define CH4_AN_NUM  4
	
#define CH5_TRIS    &TRISB
#define CH5_LAT     &LATB
#define CH5_IO_BIT  5
#define CH5_AN_NUM  5
	
#define CH6_TRIS    &TRISB
#define CH6_LAT     &LATB
#define CH6_IO_BIT  7
#define CH6_AN_NUM  6
	
#define CH7_TRIS    &TRISB
#define CH7_LAT     &LATB
#define CH7_IO_BIT  8
#define CH7_AN_NUM  7
	
#define CH8_TRIS    &TRISB
#define CH8_LAT     &LATB
#define CH8_IO_BIT  9
#define CH8_AN_NUM  8
	
#define CH9_TRIS    &TRISB
#define CH9_LAT     &LATB
#define CH9_IO_BIT  10
#define CH9_AN_NUM  9
	
#define CH10_TRIS   &TRISB
#define CH10_LAT    &LATB
#define CH10_IO_BIT 11
#define CH10_AN_NUM 10
	
#define CH11_TRIS   &TRISB
#define CH11_LAT    &LATB
#define CH11_IO_BIT 13
#define CH11_AN_NUM 11
	
#elif defined(FAMILY_PIC24FJXXGA10X) || defined(FAMILY_PIC24FJXXXGA1XX) || defined(FAMILY_PIC24FJXXXGB1XX)

#define CH0_TRIS    &TRISB
#define CH0_LAT     &LATB
#define CH0_IO_BIT  0
#define CH0_AN_NUM  0
	
#define CH1_TRIS    &TRISB
#define CH1_LAT     &LATB
#define CH1_IO_BIT  1
#define CH1_AN_NUM  1
	
#define CH2_TRIS    &TRISB
#define CH2_LAT     &LATB
#define CH2_IO_BIT  2
#define CH2_AN_NUM  2
	
#define CH3_TRIS    &TRISB
#define CH3_LAT     &LATB
#define CH3_IO_BIT  3
#define CH3_AN_NUM  3
	
#define CH4_TRIS    &TRISB
#define CH4_LAT     &LATB
#define CH4_IO_BIT  4
#define CH4_AN_NUM  4
	
#define CH5_TRIS    &TRISB
#define CH5_LAT     &LATB
#define CH5_IO_BIT  5
#define CH5_AN_NUM  5
	
#define CH6_TRIS    &TRISB
#define CH6_LAT     &LATB
#define CH6_IO_BIT  6
#define CH6_AN_NUM  6
	
#define CH7_TRIS    &TRISB
#define CH7_LAT     &LATB
#define CH7_IO_BIT  7
#define CH7_AN_NUM  7
	
#define CH8_TRIS    &TRISB
#define CH8_LAT     &LATB
#define CH8_IO_BIT  8
#define CH8_AN_NUM  8
	
#define CH9_TRIS    &TRISB
#define CH9_LAT     &LATB
#define CH9_IO_BIT  9
#define CH9_AN_NUM  9
	
#define CH10_TRIS   &TRISB
#define CH10_LAT    &LATB
#define CH10_IO_BIT 10
#define CH10_AN_NUM 10
	
#define CH11_TRIS   &TRISB
#define CH11_LAT    &LATB
#define CH11_IO_BIT 11
#define CH11_AN_NUM 11
	
#define CH12_TRIS   &TRISB
#define CH12_LAT    &LATB
#define CH12_IO_BIT 12
#define CH12_AN_NUM 12
	
#define CH13_TRIS   &TRISB
#define CH13_LAT    &LATB
#define CH13_IO_BIT 13
#define CH13_AN_NUM 13
	
#define CH14_TRIS   &TRISB
#define CH14_LAT    &LATB
#define CH14_IO_BIT 14
#define CH14_AN_NUM 14
	
#define CH15_TRIS   &TRISB
#define CH15_LAT    &LATB
#define CH15_IO_BIT 15
#define CH15_AN_NUM 15
	
#define CH16_TRIS   &TRISC
#define CH16_LAT    &LATC
#define CH16_IO_BIT 1
#define CH16_AN_NUM 16
	
#define CH17_TRIS   &TRISB
#define CH17_LAT    &LATB
#define CH17_IO_BIT 2
#define CH17_AN_NUM 17
	
#elif defined(FAMILY_PIC24FXXKA30X)
	
#define CH0_TRIS    &TRISA
#define CH0_LAT     &LATA
#define CH0_IO_BIT  0
#define CH0_AN_NUM  0
	
#define CH1_TRIS    &TRISA
#define CH1_LAT     &LATA
#define CH1_IO_BIT  1
#define CH1_AN_NUM  1
	
#define CH2_TRIS    &TRISA
#define CH2_LAT     &LATA
#define CH2_IO_BIT  2
#define CH2_AN_NUM  2
	
#define CH3_TRIS    &TRISA
#define CH3_LAT     &LATA
#define CH3_IO_BIT  3
#define CH3_AN_NUM  3
	
#define CH4_TRIS    &TRISB
#define CH4_LAT     &LATB
#define CH4_IO_BIT  0
#define CH4_AN_NUM  4
	
#define CH5_TRIS    &TRISB
#define CH5_LAT     &LATB
#define CH5_IO_BIT  1
#define CH5_AN_NUM  5
	
#define CH6_TRIS    &TRISB
#define CH6_LAT     &LATB
#define CH6_IO_BIT  2
#define CH6_AN_NUM  6
	
#define CH7_TRIS    &TRISB
#define CH7_LAT     &LATB
#define CH7_IO_BIT  4
#define CH7_AN_NUM  7
	
#define CH8_TRIS    &TRISB
#define CH8_LAT     &LATB
#define CH8_IO_BIT  12
#define CH8_AN_NUM  8
	
#define CH9_TRIS    &TRISB
#define CH9_LAT     &LATB
#define CH9_IO_BIT  13
#define CH9_AN_NUM  9
	
#define CH10_TRIS   &TRISB
#define CH10_LAT    &LATB
#define CH10_IO_BIT 14
#define CH10_AN_NUM 10
	
#define CH11_TRIS   &TRISB
#define CH11_LAT    &LATB
#define CH11_IO_BIT 15
#define CH11_AN_NUM 11
	
#elif defined(FAMILY_PIC24FJXXXDAXXX) ||  defined(FAMILY_PIC24FJXXXGB2XX) || defined(FAMILY_PIC24FJXXXGA3XX)\
|| defined(FAMILY_PIC24FXXKA30X) || defined(FAMILY_PIC24FJXXXGA3XX)
	
#define CH0_TRIS    &TRISB
#define CH0_LAT     &LATB
#define CH0_IO_BIT  0
#define CH0_AN_NUM  0
	
#define CH1_TRIS    &TRISB
#define CH1_LAT     &LATB
#define CH1_IO_BIT  1
#define CH1_AN_NUM  1
	
#define CH2_TRIS    &TRISB
#define CH2_LAT     &LATB
#define CH2_IO_BIT  2
#define CH2_AN_NUM  2
	
#define CH3_TRIS    &TRISB
#define CH3_LAT     &LATB
#define CH3_IO_BIT  3
#define CH3_AN_NUM  3
	
#define CH4_TRIS    &TRISB
#define CH4_LAT     &LATB
#define CH4_IO_BIT  4
#define CH4_AN_NUM  4
	
#define CH5_TRIS    &TRISB
#define CH5_LAT     &LATB
#define CH5_IO_BIT  5
#define CH5_AN_NUM  5
	
#define CH6_TRIS    &TRISB
#define CH6_LAT     &LATB
#define CH6_IO_BIT  6
#define CH6_AN_NUM  6
	
#define CH7_TRIS    &TRISB
#define CH7_LAT     &LATB
#define CH7_IO_BIT  7
#define CH7_AN_NUM  7
	
#define CH8_TRIS    &TRISB
#define CH8_LAT     &LATB
#define CH8_IO_BIT  8
#define CH8_AN_NUM  8
	
#define CH9_TRIS    &TRISB
#define CH9_LAT     &LATB
#define CH9_IO_BIT  9
#define CH9_AN_NUM  9
	
#define CH10_TRIS   &TRISB
#define CH10_LAT    &LATB
#define CH10_IO_BIT 10
#define CH10_AN_NUM 10
	
#define CH11_TRIS   &TRISB
#define CH11_LAT    &LATB
#define CH11_IO_BIT 11
#define CH11_AN_NUM 11
	
#define CH12_TRIS   &TRISB
#define CH12_LAT    &LATB
#define CH12_IO_BIT 12
#define CH12_AN_NUM 12
	
#define CH13_TRIS   &TRISB
#define CH13_LAT    &LATB
#define CH13_IO_BIT 13
#define CH13_AN_NUM 13
	
#define CH14_TRIS   &TRISB
#define CH14_LAT    &LATB
#define CH14_IO_BIT 14
#define CH14_AN_NUM 14
	
#define CH15_TRIS   &TRISB
#define CH15_LAT    &LATB
#define CH15_IO_BIT 15
#define CH15_AN_NUM 15
	
#define CH16_TRIS   &TRISC
#define CH16_LAT    &LATC
#define CH16_IO_BIT 4
#define CH16_AN_NUM 16
	
#define CH17_TRIS   &TRISG
#define CH17_LAT    &LATG
#define CH17_IO_BIT 6
#define CH17_AN_NUM 17
	
#define CH18_TRIS   &TRISG
#define CH18_LAT    &LATG
#define CH18_IO_BIT 7
#define CH18_AN_NUM 18
	
#define CH19_TRIS   &TRISG
#define CH19_LAT    &LATG
#define CH19_IO_BIT 8
#define CH19_AN_NUM 19
	
#define CH20_TRIS   &TRISG
#define CH20_LAT    &LATG
#define CH20_IO_BIT 9
#define CH20_AN_NUM 20
	
#define CH21_TRIS   &TRISE
#define CH21_LAT    &LATE
#define CH21_IO_BIT 9
#define CH21_AN_NUM 21
	
#define CH22_TRIS   &TRISA
#define CH22LAT    &LATA
#define CH22_IO_BIT 6
#define CH22_AN_NUM 22
	
#define CH23TRIS   &TRISA
#define CH23_LAT    &LATA
#define CH23_IO_BIT 7
#define CH23_AN_NUM 23
	
#elif  defined(FAMILY_PIC18XXJ50) || defined(FAMILY_PIC18XXJ53) || defined(FAMILY_PIC18XXJ11) || defined(FAMILY_PIC18XXJ13)

#define CH0_TRIS    &TRISA
#define CH0_LAT     &LATA
#define CH0_IO_BIT  0
#define CH0_AN_NUM  0
						
#define CH1_TRIS    &TRISA
#define CH1_LAT     &LATA
#define CH1_IO_BIT  1
#define CH1_AN_NUM  1
						
#define CH2_TRIS    &TRISA
#define CH2_LAT     &LATA
#define CH2_IO_BIT  2
#define CH2_AN_NUM  2
						
#define CH3_TRIS    &TRISA
#define CH3_LAT     &LATA
#define CH3_IO_BIT  3
#define CH3_AN_NUM  3
					
#define CH4_TRIS    &TRISA
#define CH4_LAT     &LATA
#define CH4_IO_BIT  5
#define CH4_AN_NUM  4
					
#define CH5_TRIS    &TRISE
#define CH5_LAT     &LATE
#define CH5_IO_BIT  0
#define CH5_AN_NUM  5
					
#define CH6_TRIS    &TRISE
#define CH6_LAT     &LATE
#define CH6_IO_BIT  1
#define CH6_AN_NUM  6
					
#define CH7_TRIS    &TRISE
#define CH7_LAT     &LATE
#define CH7_IO_BIT  2
#define CH7_AN_NUM  7
						
#define CH8_TRIS    &TRISB
#define CH8_LAT     &LATB
#define CH8_IO_BIT  2
#define CH8_AN_NUM  8
					
#define CH9_TRIS    &TRISB
#define CH9_LAT     &LATB
#define CH9_IO_BIT  3
#define CH9_AN_NUM  9
						
#define CH10_TRIS   &TRISB
#define CH10_LAT    &LATB
#define CH10_IO_BIT 1
#define CH10_AN_NUM 10
					
#define CH11_TRIS   &TRISC
#define CH11_LAT    &LATC
#define CH11_IO_BIT 2
#define CH11_AN_NUM 11
						
#define CH12_TRIS   &TRISB
#define CH12_LAT    &LATB
#define CH12_IO_BIT 0
#define CH12_AN_NUM 12

#elif defined(FAMILY_PIC18XXJ90) || defined(FAMILY_PIC18XXJ93) || defined(FAMILY_PIC18XXJ72)

#define CH0_TRIS    &TRISA
#define CH0_LAT     &LATA
#define CH0_IO_BIT  0
#define CH0_AN_NUM  0
	
#define CH1_TRIS    &TRISA
#define CH1_LAT     &LATA
#define CH1_IO_BIT  1
#define CH1_AN_NUM  1
	
#define CH2_TRIS    &TRISA
#define CH2_LAT     &LATA
#define CH2_IO_BIT  2
#define CH2_AN_NUM  2
	
#define CH3_TRIS    &TRISA
#define CH3_LAT     &LATA
#define CH3_IO_BIT  3
#define CH3_AN_NUM  3
	
#define CH4_TRIS    &TRISA
#define CH4_LAT     &LATA
#define CH4_IO_BIT  5
#define CH4_AN_NUM  4
	
#define CH5_TRIS    &TRISF
#define CH5_LAT     &LATF
#define CH5_IO_BIT  7
#define CH5_AN_NUM  5
	
#define CH6_TRIS    &TRISF
#define CH6_LAT     &LATF
#define CH6_IO_BIT  1
#define CH6_AN_NUM  6
	
#define CH7_TRIS    &TRISF
#define CH7_LAT     &LATF
#define CH7_IO_BIT  2
#define CH7_AN_NUM  7
	
#define CH8_TRIS    &TRISF
#define CH8_LAT     &LATF
#define CH8_IO_BIT  3
#define CH8_AN_NUM  8
	
#define CH9_TRIS    &TRISF
#define CH9_LAT     &LATF
#define CH9_IO_BIT  4
#define CH9_AN_NUM  9
	
#define CH10_TRIS    &TRISF
#define CH10_LAT     &LATF
#define CH10_IO_BIT  5
#define CH10_AN_NUM  10
	
#define CH11_TRIS    &TRISF
#define CH11_LAT     &LATF
#define CH11_IO_BIT  6
#define CH11_AN_NUM  11

#elif defined(FAMILY_PIC18_2x_4xK22)

#define CH0_TRIS    &TRISA
#define CH0_LAT     &LATA
#define CH0_IO_BIT  0
#define CH0_AN_NUM  0		
			
#define CH1_TRIS    &TRISA
#define CH1_LAT     &LATA
#define CH1_IO_BIT  1
#define CH1_AN_NUM  1
					
#define CH2_TRIS    &TRISA
#define CH2_LAT     &LATA
#define CH2_IO_BIT  2
#define CH2_AN_NUM  2
				
#define CH3_TRIS    &TRISA
#define CH3_LAT     &LATA
#define CH3_IO_BIT  3
#define CH3_AN_NUM  3
					
#define CH4_TRIS    &TRISA
#define CH4_LAT     &LATA
#define CH4_IO_BIT  5
#define CH4_AN_NUM  4
						
#define CH5_TRIS    &TRISE
#define CH5_LAT     &LATE
#define CH5_IO_BIT  0
#define CH5_AN_NUM  5
					
#define CH6_TRIS    &TRISE
#define CH6_LAT     &LATE
#define CH6_IO_BIT  1
#define CH6_AN_NUM  6
					
#define CH7_TRIS    &TRISE
#define CH7_LAT     &LATE
#define CH7_IO_BIT  2
#define CH7_AN_NUM  7
					
#define CH8_TRIS    &TRISB
#define CH8_LAT     &LATB
#define CH8_IO_BIT  2
#define CH8_AN_NUM  8
					
#define CH9_TRIS    &TRISB
#define CH9_LAT     &LATB
#define CH9_IO_BIT  3
#define CH9_AN_NUM  9
					
#define CH10_TRIS   &TRISB
#define CH10_LAT    &LATB
#define CH10_IO_BIT 1
#define CH10_AN_NUM 10
						
#define CH11_TRIS   &TRISB
#define CH11_LAT    &LATB
#define CH11_IO_BIT 4
#define CH11_AN_NUM 11
					
#define CH12_TRIS   &TRISB
#define CH12_LAT    &LATB
#define CH12_IO_BIT 0
#define CH12_AN_NUM 12
					
#define CH13_TRIS    &TRISB
#define CH13_LAT     &LATB
#define CH13_IO_BIT  5
#define CH13_AN_NUM  13
					
#define CH14_TRIS    &TRISC
#define CH14_LAT     &LATC
#define CH14_IO_BIT  2
#define CH14_AN_NUM  14
					
#define CH15_TRIS    &TRISC
#define CH15_LAT     &LATC
#define CH15_IO_BIT  3
#define CH15_AN_NUM  15
					
#define CH16_TRIS    &TRISC
#define CH16_LAT     &LATC
#define CH16_IO_BIT  4
#define CH16_AN_NUM  16
					
#define CH17_TRIS    &TRISC
#define CH17_LAT     &LATC
#define CH17_IO_BIT  5
#define CH17_AN_NUM  17
					
#define CH18_TRIS    &TRISC
#define CH18_LAT     &LATC
#define CH18_IO_BIT  6
#define CH18_AN_NUM  18
						
#define CH19_TRIS    &TRISC
#define CH19_LAT     &LATC
#define CH19_IO_BIT  7
#define CH19_AN_NUM  19
					
#define CH20_TRIS    &TRISD
#define CH20_LAT     &LATD
#define CH20_IO_BIT  0
#define CH20_AN_NUM  20
						
#define CH21_TRIS    &TRISD
#define CH21_LAT     &LATD
#define CH21_IO_BIT  1
#define CH21_AN_NUM  21
					
#define CH22_TRIS    &TRISD
#define CH22_LAT     &LATD
#define CH22_IO_BIT  2
#define CH22_AN_NUM  22
					
#define CH23_TRIS   &TRISD
#define CH23_LAT    &LATD
#define CH23_IO_BIT 3
#define CH23_AN_NUM 23
					
#define CH24_TRIS   &TRISD
#define CH24_LAT    &LATD
#define CH24_IO_BIT 4
#define CH24_AN_NUM 24
						
#define CH25_TRIS   &TRISD
#define CH25_LAT    &LATD
#define CH25_IO_BIT 5
#define CH25_AN_NUM 25
				
#define CH26_TRIS   &TRISD
#define CH26_LAT    &LATD
#define CH26_IO_BIT 6
#define CH26_AN_NUM 26
				
#define CH27_TRIS   &TRISD
#define CH27_LAT    &LATD
#define CH27_IO_BIT 7
#define CH27_AN_NUM 27

#elif defined(FAMILY_PIC18XXK90) || defined(FAMILY_PIC18XXK22)

#define CH0_TRIS    &TRISA
#define CH0_LAT     &LATA
#define CH0_IO_BIT  0
#define CH0_AN_NUM  0
			
#define CH1_TRIS    &TRISA
#define CH1_LAT     &LATA
#define CH1_IO_BIT  1
#define CH1_AN_NUM  1
		 
#define CH2_TRIS    &TRISA
#define CH2_LAT     &LATA
#define CH2_IO_BIT  2
#define CH2_AN_NUM  2
	 
#define CH3_TRIS    &TRISA
#define CH3_LAT     &LATA
#define CH3_IO_BIT  3
#define CH3_AN_NUM  3
		
#define CH4_TRIS    &TRISA
#define CH4_LAT     &LATA
#define CH4_IO_BIT  5
#define CH4_AN_NUM  4

#define CH5_TRIS    &TRISF
#define CH5_LAT     &LATF
#define CH5_IO_BIT  7
#define CH5_AN_NUM  5
	
#define CH6_TRIS    &TRISF
#define CH6_LAT     &LATF
#define CH6_IO_BIT  1
#define CH6_AN_NUM  6
		 
#define CH7_TRIS    &TRISF
#define CH7_LAT     &LATF
#define CH7_IO_BIT  2
#define CH7_AN_NUM  7
		 
#define CH8_TRIS    &TRISF
#define CH8_LAT     &LATF
#define CH8_IO_BIT  3
#define CH8_AN_NUM  8
		 
#define CH9_TRIS    &TRISF
#define CH9_LAT     &LATF
#define CH9_IO_BIT  4
#define CH9_AN_NUM  9
		
#define CH10_TRIS   &TRISF
#define CH10_LAT    &LATF
#define CH10_IO_BIT 5
#define CH10_AN_NUM 10
		
#define CH11_TRIS   &TRISF
#define CH11_LAT    &LATF
#define CH11_IO_BIT 6
#define CH11_AN_NUM 11
	 
#define CH12_TRIS   &TRISH
#define CH12_LAT    &LATH
#define CH12_IO_BIT 4
#define CH12_AN_NUM 12
	 
#define CH13_TRIS    &TRISH
#define CH13_LAT     &LATH
#define CH13_IO_BIT  5
#define CH13_AN_NUM  13
	
#define CH14_TRIS    &TRISH
#define CH14_LAT     &LATH
#define CH14_IO_BIT  6
#define CH14_AN_NUM  14
	 
#define CH15_TRIS    &TRISH
#define CH15_LAT     &LATH
#define CH15_IO_BIT  7
#define CH15_AN_NUM  15
		 
#define CH16_TRIS    &TRISG
#define CH16_LAT     &LATG
#define CH16_IO_BIT  4
#define CH16_AN_NUM  16
	 
#define CH17_TRIS    &TRISG
#define CH17_LAT     &LATG
#define CH17_IO_BIT  3
#define CH17_AN_NUM  17
	 
#define CH18_TRIS    &TRISG
#define CH18_LAT     &LATG
#define CH18_IO_BIT  2
#define CH18_AN_NUM  18
	 
#define CH19_TRIS    &TRISG
#define CH19_LAT     &LATG
#define CH19_IO_BIT  1
#define CH19_AN_NUM  19
		 
#define CH20_TRIS    &TRISH
#define CH20_LAT     &LATH
#define CH20_IO_BIT  3
#define CH20_AN_NUM  20
	 
#define CH21_TRIS    &TRISH
#define CH21_LAT     &LATH
#define CH21_IO_BIT  2
#define CH21_AN_NUM  21
		
#define CH22_TRIS    &TRISH
#define CH22_LAT     &LATH
#define CH22_IO_BIT  1
#define CH22_AN_NUM  22
	 
#define CH23_TRIS   &TRISH
#define CH23_LAT    &LATH
#define CH23_IO_BIT 0
#define CH23_AN_NUM 23

#elif defined(FAMILY_PIC18XXK50) 

#define CH0_TRIS    &TRISA
#define CH0_LAT     &LATA
#define CH0_IO_BIT  0
#define CH0_AN_NUM  0

#define CH1_TRIS    &TRISA
#define CH1_LAT     &LATA
#define CH1_IO_BIT  1
#define CH1_AN_NUM  1
		 
#define CH2_TRIS    &TRISA
#define CH2_LAT     &LATA
#define CH2_IO_BIT  2
#define CH2_AN_NUM  2
	 
#define CH3_TRIS    &TRISA
#define CH3_LAT     &LATA
#define CH3_IO_BIT  3
#define CH3_AN_NUM  3
		
#define CH4_TRIS    &TRISA
#define CH4_LAT     &LATA
#define CH4_IO_BIT  5
#define CH4_AN_NUM  4

#define CH5_TRIS    &TRISE
#define CH5_LAT     &LATE
#define CH5_IO_BIT  0
#define CH5_AN_NUM  5
	
#define CH6_TRIS    &TRISE
#define CH6_LAT     &LATE
#define CH6_IO_BIT  1
#define CH6_AN_NUM  6
		 
#define CH7_TRIS    &TRISE
#define CH7_LAT     &LATE
#define CH7_IO_BIT  2
#define CH7_AN_NUM  7
		 
#define CH8_TRIS    &TRISB
#define CH8_LAT     &LATB
#define CH8_IO_BIT  2
#define CH8_AN_NUM  8
		 
#define CH9_TRIS    &TRISB
#define CH9_LAT     &LATB
#define CH9_IO_BIT  3
#define CH9_AN_NUM  9
		
#define CH10_TRIS   &TRISB
#define CH10_LAT    &LATB
#define CH10_IO_BIT 1
#define CH10_AN_NUM 10
		
#define CH11_TRIS   &TRISB
#define CH11_LAT    &LATB
#define CH11_IO_BIT 4
#define CH11_AN_NUM 11
	 
#define CH12_TRIS   &TRISB
#define CH12_LAT    &LATB
#define CH12_IO_BIT 0
#define CH12_AN_NUM 12
	 
#define CH13_TRIS    &TRISB
#define CH13_LAT     &LATB
#define CH13_IO_BIT  5
#define CH13_AN_NUM  13
	
#define CH14_TRIS    &TRISC
#define CH14_LAT     &LATC
#define CH14_IO_BIT  2
#define CH14_AN_NUM  14
	 
#define CH15_TRIS    &TRISH // NOT THERE IN DATASHEET
#define CH15_LAT     &LATH
#define CH15_IO_BIT  7
#define CH15_AN_NUM  15
		 
#define CH16_TRIS    &TRISG // NOT THERE IN DATASHEET
#define CH16_LAT     &LATG
#define CH16_IO_BIT  4
#define CH16_AN_NUM  16
	 
#define CH17_TRIS    &TRISG // NOT THERE IN DATASHEET
#define CH17_LAT     &LATG
#define CH17_IO_BIT  3
#define CH17_AN_NUM  17
	 
#define CH18_TRIS    &TRISC
#define CH18_LAT     &LATC
#define CH18_IO_BIT  6
#define CH18_AN_NUM  18
	 
#define CH19_TRIS    &TRISC
#define CH19_LAT     &LATC
#define CH19_IO_BIT  7
#define CH19_AN_NUM  19
		 
#define CH20_TRIS    &TRISD
#define CH20_LAT     &LATD
#define CH20_IO_BIT  0
#define CH20_AN_NUM  20
	 
#define CH21_TRIS    &TRISD
#define CH21_LAT     &LATD
#define CH21_IO_BIT  1
#define CH21_AN_NUM  21
		
#define CH22_TRIS    &TRISD
#define CH22_LAT     &LATD
#define CH22_IO_BIT  2
#define CH22_AN_NUM  22
	 
#define CH23_TRIS   &TRISD
#define CH23_LAT    &LATD
#define CH23_IO_BIT 3
#define CH23_AN_NUM 23

#define CH24_TRIS    &TRISD
#define CH24_LAT     &LATD
#define CH24_IO_BIT  4
#define CH24_AN_NUM  24
	 
#define CH25_TRIS    &TRISD
#define CH25_LAT     &LATD
#define CH25_IO_BIT  5
#define CH25_AN_NUM  25
		
#define CH26_TRIS    &TRISD
#define CH26_LAT     &LATD
#define CH26_IO_BIT  6
#define CH26_AN_NUM  26
	 
#define CH27_TRIS   &TRISD
#define CH27_LAT    &LATD
#define CH27_IO_BIT 7
#define CH27_AN_NUM 27

#elif defined(FAMILY_PIC18XXJ94) 

#define CH0_TRIS    &TRISA
#define CH0_LAT     &LATA
#define CH0_IO_BIT  0
#define CH0_AN_NUM  0
			
#define CH1_TRIS    &TRISA
#define CH1_LAT     &LATA
#define CH1_IO_BIT  1
#define CH1_AN_NUM  1
		 
#define CH2_TRIS    &TRISA
#define CH2_LAT     &LATA
#define CH2_IO_BIT  2
#define CH2_AN_NUM  2
	 
#define CH3_TRIS    &TRISA
#define CH3_LAT     &LATA
#define CH3_IO_BIT  3
#define CH3_AN_NUM  3
		
#define CH4_TRIS    &TRISA
#define CH4_LAT     &LATA
#define CH4_IO_BIT  5
#define CH4_AN_NUM  4

#define CH5_TRIS    &TRISF
#define CH5_LAT     &LATF
#define CH5_IO_BIT  7
#define CH5_AN_NUM  5
	
#define CH6_TRIS    &TRISA
#define CH6_LAT     &LATA
#define CH6_IO_BIT  4
#define CH6_AN_NUM  6
		 
#define CH7_TRIS    &TRISF
#define CH7_LAT     &LATF
#define CH7_IO_BIT  2
#define CH7_AN_NUM  7
		 
#define CH8_TRIS    &TRISG
#define CH8_LAT     &LATG
#define CH8_IO_BIT  0
#define CH8_AN_NUM  8
		 
#define CH9_TRIS    &TRISC
#define CH9_LAT     &LATC
#define CH9_IO_BIT  2
#define CH9_AN_NUM  9
		
#define CH10_TRIS   &TRISF
#define CH10_LAT    &LATF
#define CH10_IO_BIT 5
#define CH10_AN_NUM 10
		
#define CH11_TRIS   &TRISF
#define CH11_LAT    &LATF
#define CH11_IO_BIT 6
#define CH11_AN_NUM 11
	 
#define CH12_TRIS   &TRISH
#define CH12_LAT    &LATH
#define CH12_IO_BIT 4
#define CH12_AN_NUM 12
	 
#define CH13_TRIS    &TRISH
#define CH13_LAT     &LATH
#define CH13_IO_BIT  5
#define CH13_AN_NUM  13
	
#define CH14_TRIS    &TRISH
#define CH14_LAT     &LATH
#define CH14_IO_BIT  6
#define CH14_AN_NUM  14
	 
#define CH15_TRIS    &TRISH
#define CH15_LAT     &LATH
#define CH15_IO_BIT  7
#define CH15_AN_NUM  15
 
#define CH16_TRIS    &TRISG
#define CH16_LAT     &LATG
#define CH16_IO_BIT  4
#define CH16_AN_NUM  16
 
#define CH17_TRIS    &TRISG
#define CH17_LAT     &LATG
#define CH17_IO_BIT  3
#define CH17_AN_NUM  17
 
#define CH18_TRIS    &TRISG
#define CH18_LAT     &LATG
#define CH18_IO_BIT  2
#define CH18_AN_NUM  18
 
#define CH19_TRIS    &TRISG
#define CH19_LAT     &LATG
#define CH19_IO_BIT  1
#define CH19_AN_NUM  19
 
#define CH20_TRIS    &TRISH
#define CH20_LAT     &LATH
#define CH20_IO_BIT  3
#define CH20_AN_NUM  20
 
#define CH21_TRIS    &TRISH
#define CH21_LAT     &LATH
#define CH21_IO_BIT  2
#define CH21_AN_NUM  21

#define CH22_TRIS    &TRISH
#define CH22_LAT     &LATH
#define CH22_IO_BIT  1
#define CH22_AN_NUM  22
 
#define CH23_TRIS   &TRISH
#define CH23_LAT    &LATH
#define CH23_IO_BIT 0
#define CH23_AN_NUM 23

#elif defined(FAMILY_dsPIC30F2011_2012_3012_3013)

#define CH0_TRIS    &TRISB
#define CH0_LAT     &LATB
#define CH0_IO_BIT  0
#define CH0_AN_NUM  0
			
#define CH1_TRIS    &TRISB
#define CH1_LAT     &LATB
#define CH1_IO_BIT  1
#define CH1_AN_NUM  1
		 
#define CH2_TRIS    &TRISB
#define CH2_LAT     &LATB
#define CH2_IO_BIT  2
#define CH2_AN_NUM  2
	 
#define CH3_TRIS    &TRISB
#define CH3_LAT     &LATB
#define CH3_IO_BIT  3
#define CH3_AN_NUM  3
		
#define CH4_TRIS    &TRISB
#define CH4_LAT     &LATB
#define CH4_IO_BIT  4
#define CH4_AN_NUM  4

#define CH5_TRIS    &TRISB
#define CH5_LAT     &LATB
#define CH5_IO_BIT  5
#define CH5_AN_NUM  5
	
#define CH6_TRIS    &TRISB
#define CH6_LAT     &LATB
#define CH6_IO_BIT  6
#define CH6_AN_NUM  6
		 
#define CH7_TRIS    &TRISB
#define CH7_LAT     &LATB
#define CH7_IO_BIT  7
#define CH7_AN_NUM  7
		 
#define CH8_TRIS    &TRISB
#define CH8_LAT     &LATB
#define CH8_IO_BIT  8
#define CH8_AN_NUM  8
		 
#define CH9_TRIS    &TRISB
#define CH9_LAT     &LATB
#define CH9_IO_BIT  9
#define CH9_AN_NUM  9

#elif defined(FAMILY_dsPIC30F3014_4013)

#define CH0_TRIS    &TRISB
#define CH0_LAT     &LATB
#define CH0_IO_BIT  0
#define CH0_AN_NUM  0
			
#define CH1_TRIS    &TRISB
#define CH1_LAT     &LATB
#define CH1_IO_BIT  1
#define CH1_AN_NUM  1
		 
#define CH2_TRIS    &TRISB
#define CH2_LAT     &LATB
#define CH2_IO_BIT  2
#define CH2_AN_NUM  2
	 
#define CH3_TRIS    &TRISB
#define CH3_LAT     &LATB
#define CH3_IO_BIT  3
#define CH3_AN_NUM  3
		
#define CH4_TRIS    &TRISB
#define CH4_LAT     &LATB
#define CH4_IO_BIT  4
#define CH4_AN_NUM  4

#define CH5_TRIS    &TRISB
#define CH5_LAT     &LATB
#define CH5_IO_BIT  5
#define CH5_AN_NUM  5
	
#define CH6_TRIS    &TRISB
#define CH6_LAT     &LATB
#define CH6_IO_BIT  6
#define CH6_AN_NUM  6
		 
#define CH7_TRIS    &TRISB
#define CH7_LAT     &LATB
#define CH7_IO_BIT  7
#define CH7_AN_NUM  7
		 
#define CH8_TRIS    &TRISB
#define CH8_LAT     &LATB
#define CH8_IO_BIT  8
#define CH8_AN_NUM  8
		 
#define CH9_TRIS    &TRISB
#define CH9_LAT     &LATB
#define CH9_IO_BIT  9
#define CH9_AN_NUM  9

#define CH10_TRIS    &TRISB
#define CH10_LAT     &LATB
#define CH10_IO_BIT  10
#define CH10_AN_NUM  10
		 
#define CH11_TRIS    &TRISB
#define CH11_LAT     &LATB
#define CH11_IO_BIT  11
#define CH11_AN_NUM  11
		 
#define CH12_TRIS    &TRISB
#define CH12_LAT     &LATB
#define CH12_IO_BIT  12
#define CH12_AN_NUM  12

#elif defined(FAMILY_dsPIC30F5011_5013)||defined(FAMILY_dsPIC30F6011A_6012A_6013A_6014A)

#define CH0_TRIS    &TRISB
#define CH0_LAT     &LATB
#define CH0_IO_BIT  0
#define CH0_AN_NUM  0
			
#define CH1_TRIS    &TRISB
#define CH1_LAT     &LATB
#define CH1_IO_BIT  1
#define CH1_AN_NUM  1
		 
#define CH2_TRIS    &TRISB
#define CH2_LAT     &LATB
#define CH2_IO_BIT  2
#define CH2_AN_NUM  2
	 
#define CH3_TRIS    &TRISB
#define CH3_LAT     &LATB
#define CH3_IO_BIT  3
#define CH3_AN_NUM  3
		
#define CH4_TRIS    &TRISB
#define CH4_LAT     &LATB
#define CH4_IO_BIT  4
#define CH4_AN_NUM  4

#define CH5_TRIS    &TRISB
#define CH5_LAT     &LATB
#define CH5_IO_BIT  5
#define CH5_AN_NUM  5
	
#define CH6_TRIS    &TRISB
#define CH6_LAT     &LATB
#define CH6_IO_BIT  6
#define CH6_AN_NUM  6
		 
#define CH7_TRIS    &TRISB
#define CH7_LAT     &LATB
#define CH7_IO_BIT  7
#define CH7_AN_NUM  7
		 
#define CH8_TRIS    &TRISB
#define CH8_LAT     &LATB
#define CH8_IO_BIT  8
#define CH8_AN_NUM  8
		 
#define CH9_TRIS    &TRISB
#define CH9_LAT     &LATB
#define CH9_IO_BIT  9
#define CH9_AN_NUM  9

#define CH10_TRIS    &TRISB
#define CH10_LAT     &LATB
#define CH10_IO_BIT  10
#define CH10_AN_NUM  10
		 
#define CH11_TRIS    &TRISB
#define CH11_LAT     &LATB
#define CH11_IO_BIT  11
#define CH11_AN_NUM  11
		 
#define CH12_TRIS    &TRISB
#define CH12_LAT     &LATB
#define CH12_IO_BIT  12
#define CH12_AN_NUM  12

#define CH13_TRIS    &TRISB
#define CH13_LAT     &LATB
#define CH13_IO_BIT  13
#define CH13_AN_NUM  13
		 
#define CH14_TRIS    &TRISB
#define CH14_LAT     &LATB
#define CH14_IO_BIT  14
#define CH14_AN_NUM  14
		 
#define CH15_TRIS    &TRISB
#define CH15_LAT     &LATB
#define CH15_IO_BIT  15
#define CH15_AN_NUM  15

#elif defined(FAMILY_PIC24HJ12GP201_202)||defined(FAMILY_dsPIC33FJ12GP201_202)

#define CH0_TRIS    &TRISA
#define CH0_LAT     &LATA
#define CH0_IO_BIT  0
#define CH0_AN_NUM  0
			
#define CH1_TRIS    &TRISA
#define CH1_LAT     &LATA
#define CH1_IO_BIT  1
#define CH1_AN_NUM  1
		 
#define CH2_TRIS    &TRISB
#define CH2_LAT     &LATB
#define CH2_IO_BIT  0
#define CH2_AN_NUM  2
	 
#define CH3_TRIS    &TRISB
#define CH3_LAT     &LATB
#define CH3_IO_BIT  1
#define CH3_AN_NUM  3
		
#define CH4_TRIS    &TRISB
#define CH4_LAT     &LATB
#define CH4_IO_BIT  2
#define CH4_AN_NUM  4

#define CH5_TRIS    &TRISB
#define CH5_LAT     &LATB
#define CH5_IO_BIT  3
#define CH5_AN_NUM  5
	
#define CH6_TRIS    &TRISB
#define CH6_LAT     &LATB
#define CH6_IO_BIT  15
#define CH6_AN_NUM  6
		 
#define CH7_TRIS    &TRISB
#define CH7_LAT     &LATB
#define CH7_IO_BIT  14
#define CH7_AN_NUM  7
		 
#define CH8_TRIS    &TRISB
#define CH8_LAT     &LATB
#define CH8_IO_BIT  13
#define CH8_AN_NUM  8
		 
#define CH9_TRIS    &TRISB
#define CH9_LAT     &LATB
#define CH9_IO_BIT  12
#define CH9_AN_NUM  9

#elif defined(FAMILY_PIC24HJ32GP202_204_PIC24HJ16GP304)||defined(FAMILY_dsPIC33FJ32GP202_204_dsPIC33FJ16GP304)||\
	  defined(FAMILY_dsPIC33FJ32GP302_304_dsPIC33FJ64GPX02_X04_dsPIC33FJ128GPX02_X04)||\
	  defined(FAMILY_PIC24HJ32GP302_304_PIC24HJ64GPX02_X04_PIC24HJ128GPX02_X04)

#define CH0_TRIS    &TRISA
#define CH0_LAT     &LATA
#define CH0_IO_BIT  0
#define CH0_AN_NUM  0
			
#define CH1_TRIS    &TRISA
#define CH1_LAT     &LATA
#define CH1_IO_BIT  1
#define CH1_AN_NUM  1
		 
#define CH2_TRIS    &TRISB
#define CH2_LAT     &LATB
#define CH2_IO_BIT  0
#define CH2_AN_NUM  2
	 
#define CH3_TRIS    &TRISB
#define CH3_LAT     &LATB
#define CH3_IO_BIT  1
#define CH3_AN_NUM  3
		
#define CH4_TRIS    &TRISB
#define CH4_LAT     &LATB
#define CH4_IO_BIT  2
#define CH4_AN_NUM  4

#define CH5_TRIS    &TRISB
#define CH5_LAT     &LATB
#define CH5_IO_BIT  3
#define CH5_AN_NUM  5

#define CH6_TRIS    &TRISC
#define CH6_LAT     &LATC
#define CH6_IO_BIT  0
#define CH6_AN_NUM  6
		 
#define CH7_TRIS    &TRISC
#define CH7_LAT     &LATC
#define CH7_IO_BIT  1
#define CH7_AN_NUM  7
		 
#define CH8_TRIS    &TRISC
#define CH8_LAT     &LATC
#define CH8_IO_BIT  2
#define CH8_AN_NUM  8

#define CH9_TRIS    &TRISB
#define CH9_LAT     &LATB
#define CH9_IO_BIT  15
#define CH9_AN_NUM  9
		 
#define CH10_TRIS    &TRISB
#define CH10_LAT     &LATB
#define CH10_IO_BIT  14
#define CH10_AN_NUM  10
		 
#define CH11_TRIS    &TRISB
#define CH11_LAT     &LATB
#define CH11_IO_BIT  13
#define CH11_AN_NUM  11
		 
#define CH12_TRIS    &TRISB
#define CH12_LAT     &LATB
#define CH12_IO_BIT  12
#define CH12_AN_NUM  12

#elif defined(FAMILY_dsPIC33EPXXXGP50X_dsPIC33EPXXXMC20X_50X_PIC24EPXXXGP_MC20X)

#define CH0_TRIS    &TRISA
#define CH0_LAT     &LATA
#define CH0_IO_BIT  0
#define CH0_AN_NUM  0
			
#define CH1_TRIS    &TRISA
#define CH1_LAT     &LATA
#define CH1_IO_BIT  1
#define CH1_AN_NUM  1
		 
#define CH2_TRIS    &TRISB
#define CH2_LAT     &LATB
#define CH2_IO_BIT  0
#define CH2_AN_NUM  2
	 
#define CH3_TRIS    &TRISB
#define CH3_LAT     &LATB
#define CH3_IO_BIT  1
#define CH3_AN_NUM  3
		
#define CH4_TRIS    &TRISB
#define CH4_LAT     &LATB
#define CH4_IO_BIT  2
#define CH4_AN_NUM  4

#define CH5_TRIS    &TRISB
#define CH5_LAT     &LATB
#define CH5_IO_BIT  3
#define CH5_AN_NUM  5

#define CH6_TRIS    &TRISC
#define CH6_LAT     &LATC
#define CH6_IO_BIT  0
#define CH6_AN_NUM  6
		 
#define CH7_TRIS    &TRISC
#define CH7_LAT     &LATC
#define CH7_IO_BIT  1
#define CH7_AN_NUM  7
		 
#define CH8_TRIS    &TRISC
#define CH8_LAT     &LATC
#define CH8_IO_BIT  2
#define CH8_AN_NUM  8

#define CH9_TRIS    &TRISA
#define CH9_LAT     &LATA
#define CH9_IO_BIT  11
#define CH9_AN_NUM  9
		 
#define CH10_TRIS    &TRISA
#define CH10_LAT     &LATA
#define CH10_IO_BIT  12
#define CH10_AN_NUM  10
		 
#define CH11_TRIS    &TRISC
#define CH11_LAT     &LATC
#define CH11_IO_BIT  11
#define CH11_AN_NUM  11
		 
#define CH12_TRIS    &TRISE
#define CH12_LAT     &LATE
#define CH12_IO_BIT  12
#define CH12_AN_NUM  12

#define CH13_TRIS    &TRISE
#define CH13_LAT     &LATE
#define CH13_IO_BIT  13
#define CH13_AN_NUM  13
		 
#define CH14_TRIS    &TRISE
#define CH14_LAT     &LATE
#define CH14_IO_BIT  14
#define CH14_AN_NUM  14
		 
#define CH15_TRIS    &TRISE
#define CH15_LAT     &LATE
#define CH15_IO_BIT  15
#define CH15_AN_NUM  15

#elif defined(FAMILY_dsPIC33EPXXX_GP_MC_MU_806_810_814_PIC24EPXXX_GP_GU_810_814)

#define CH0_TRIS    &TRISB
#define CH0_LAT     &LATB
#define CH0_IO_BIT  0
#define CH0_AN_NUM  0
			
#define CH1_TRIS    &TRISB
#define CH1_LAT     &LATB
#define CH1_IO_BIT  1
#define CH1_AN_NUM  1
		 
#define CH2_TRIS    &TRISB
#define CH2_LAT     &LATB
#define CH2_IO_BIT  2
#define CH2_AN_NUM  2
	 
#define CH3_TRIS    &TRISB
#define CH3_LAT     &LATB
#define CH3_IO_BIT  3
#define CH3_AN_NUM  3
		
#define CH4_TRIS    &TRISB
#define CH4_LAT     &LATB
#define CH4_IO_BIT  4
#define CH4_AN_NUM  4

#define CH5_TRIS    &TRISB
#define CH5_LAT     &LATB
#define CH5_IO_BIT  5
#define CH5_AN_NUM  5
	
#define CH6_TRIS    &TRISB
#define CH6_LAT     &LATB
#define CH6_IO_BIT  6
#define CH6_AN_NUM  6
		 
#define CH7_TRIS    &TRISB
#define CH7_LAT     &LATB
#define CH7_IO_BIT  7
#define CH7_AN_NUM  7
		 
#define CH8_TRIS    &TRISB
#define CH8_LAT     &LATB
#define CH8_IO_BIT  8
#define CH8_AN_NUM  8
		 
#define CH9_TRIS    &TRISB
#define CH9_LAT     &LATB
#define CH9_IO_BIT  9
#define CH9_AN_NUM  9

#define CH10_TRIS    &TRISB
#define CH10_LAT     &LATB
#define CH10_IO_BIT  10
#define CH10_AN_NUM  10
		 
#define CH11_TRIS    &TRISB
#define CH11_LAT     &LATB
#define CH11_IO_BIT  11
#define CH11_AN_NUM  11
		 
#define CH12_TRIS    &TRISB
#define CH12_LAT     &LATB
#define CH12_IO_BIT  12
#define CH12_AN_NUM  12

#define CH13_TRIS    &TRISB
#define CH13_LAT     &LATB
#define CH13_IO_BIT  13
#define CH13_AN_NUM  13
		 
#define CH14_TRIS    &TRISB
#define CH14_LAT     &LATB
#define CH14_IO_BIT  14
#define CH14_AN_NUM  14
		 
#define CH15_TRIS    &TRISB
#define CH15_LAT     &LATB
#define CH15_IO_BIT  15
#define CH15_AN_NUM  15

#define CH16_TRIS    &TRISC
#define CH16_LAT     &LATC
#define CH16_IO_BIT  1
#define CH16_AN_NUM  16
		 
#define CH17_TRIS    &TRISC
#define CH17_LAT     &LATC
#define CH17_IO_BIT  2
#define CH17_AN_NUM  17
	 
#define CH18_TRIS    &TRISC
#define CH18_LAT     &LATC
#define CH18_IO_BIT  3
#define CH18_AN_NUM  18
		
#define CH19_TRIS    &TRISC
#define CH19_LAT     &LATC
#define CH19_IO_BIT  4
#define CH19_AN_NUM  19

#define CH20_TRIS    &TRISE
#define CH20_LAT     &LATE
#define CH20_IO_BIT  8
#define CH20_AN_NUM  20

#define CH21_TRIS    &TRISE
#define CH21_LAT     &LATE
#define CH21_IO_BIT  9
#define CH21_AN_NUM  21
	 
#define CH22_TRIS    &TRISA
#define CH22_LAT     &LATA
#define CH22_IO_BIT  6
#define CH22_AN_NUM  22
		
#define CH23_TRIS    &TRISA
#define CH23_LAT     &LATA
#define CH23_IO_BIT  7
#define CH23_AN_NUM  23

#define CH24_TRIS    &TRISE
#define CH24_LAT     &LATE
#define CH24_IO_BIT  0
#define CH24_AN_NUM  24

#define CH25_TRIS    &TRISE
#define CH25_LAT     &LATE
#define CH25_IO_BIT  1
#define CH25_AN_NUM  25

#define CH26_TRIS    &TRISE
#define CH26_LAT     &LATE
#define CH26_IO_BIT  2
#define CH26_AN_NUM  26

#define CH27_TRIS    &TRISE
#define CH27_LAT     &LATE
#define CH27_IO_BIT  3
#define CH27_AN_NUM  27

#define CH28_TRIS    &TRISE
#define CH28_LAT     &LATE
#define CH28_IO_BIT  4
#define CH28_AN_NUM  28

#define CH29_TRIS    &TRISE
#define CH29_LAT     &LATE
#define CH29_IO_BIT  5
#define CH29_AN_NUM  29

#define CH30_TRIS    &TRISE
#define CH30_LAT     &LATE
#define CH30_IO_BIT  6
#define CH30_AN_NUM  30

#define CH31_TRIS    &TRISE
#define CH31_LAT     &LATE
#define CH31_IO_BIT  7
#define CH31_AN_NUM  31

#elif defined(FAMILY_PIC24HJXXXGPX06A_X08A_X10A)||defined(FAMILY_dsPIC33FJXXXGPX06A_X08A_X10A)

#define CH0_TRIS    &TRISB
#define CH0_LAT     &LATB
#define CH0_IO_BIT  0
#define CH0_AN_NUM  0
			
#define CH1_TRIS    &TRISB
#define CH1_LAT     &LATB
#define CH1_IO_BIT  1
#define CH1_AN_NUM  1
		 
#define CH2_TRIS    &TRISB
#define CH2_LAT     &LATB
#define CH2_IO_BIT  2
#define CH2_AN_NUM  2
	 
#define CH3_TRIS    &TRISB
#define CH3_LAT     &LATB
#define CH3_IO_BIT  3
#define CH3_AN_NUM  3
		
#define CH4_TRIS    &TRISB
#define CH4_LAT     &LATB
#define CH4_IO_BIT  4
#define CH4_AN_NUM  4

#define CH5_TRIS    &TRISB
#define CH5_LAT     &LATB
#define CH5_IO_BIT  5
#define CH5_AN_NUM  5
	
#define CH6_TRIS    &TRISB
#define CH6_LAT     &LATB
#define CH6_IO_BIT  6
#define CH6_AN_NUM  6
		 
#define CH7_TRIS    &TRISB
#define CH7_LAT     &LATB
#define CH7_IO_BIT  7
#define CH7_AN_NUM  7
		 
#define CH8_TRIS    &TRISB
#define CH8_LAT     &LATB
#define CH8_IO_BIT  8
#define CH8_AN_NUM  8
		 
#define CH9_TRIS    &TRISB
#define CH9_LAT     &LATB
#define CH9_IO_BIT  9
#define CH9_AN_NUM  9

#define CH10_TRIS    &TRISB
#define CH10_LAT     &LATB
#define CH10_IO_BIT  10
#define CH10_AN_NUM  10
		 
#define CH11_TRIS    &TRISB
#define CH11_LAT     &LATB
#define CH11_IO_BIT  11
#define CH11_AN_NUM  11
		 
#define CH12_TRIS    &TRISB
#define CH12_LAT     &LATB
#define CH12_IO_BIT  12
#define CH12_AN_NUM  12

#define CH13_TRIS    &TRISB
#define CH13_LAT     &LATB
#define CH13_IO_BIT  13
#define CH13_AN_NUM  13
		 
#define CH14_TRIS    &TRISB
#define CH14_LAT     &LATB
#define CH14_IO_BIT  14
#define CH14_AN_NUM  14
		 
#define CH15_TRIS    &TRISB
#define CH15_LAT     &LATB
#define CH15_IO_BIT  15
#define CH15_AN_NUM  15

#define CH16_TRIS    &TRISC
#define CH16_LAT     &LATC
#define CH16_IO_BIT  1
#define CH16_AN_NUM  16
		 
#define CH17_TRIS    &TRISC
#define CH17_LAT     &LATC
#define CH17_IO_BIT  2
#define CH17_AN_NUM  17
	 
#define CH18_TRIS    &TRISC
#define CH18_LAT     &LATC
#define CH18_IO_BIT  3
#define CH18_AN_NUM  18
		
#define CH19_TRIS    &TRISC
#define CH19_LAT     &LATC
#define CH19_IO_BIT  4
#define CH19_AN_NUM  19

#define CH20_TRIS    &TRISA
#define CH20_LAT     &LATA
#define CH20_IO_BIT  12
#define CH20_AN_NUM  20

#define CH21_TRIS    &TRISA
#define CH21_LAT     &LATA
#define CH21_IO_BIT  13
#define CH21_AN_NUM  21
	 
#define CH22_TRIS    &TRISA
#define CH22_LAT     &LATA
#define CH22_IO_BIT  6
#define CH22_AN_NUM  22
		
#define CH23_TRIS    &TRISA
#define CH23_LAT     &LATA
#define CH23_IO_BIT  7
#define CH23_AN_NUM  23

#define CH24_TRIS    &TRISE
#define CH24_LAT     &LATE
#define CH24_IO_BIT  0
#define CH24_AN_NUM  24

#define CH25_TRIS    &TRISE
#define CH25_LAT     &LATE
#define CH25_IO_BIT  1
#define CH25_AN_NUM  25

#define CH26_TRIS    &TRISE
#define CH26_LAT     &LATE
#define CH26_IO_BIT  2
#define CH26_AN_NUM  26

#define CH27_TRIS    &TRISE
#define CH27_LAT     &LATE
#define CH27_IO_BIT  3
#define CH27_AN_NUM  27

#define CH28_TRIS    &TRISE
#define CH28_LAT     &LATE
#define CH28_IO_BIT  4
#define CH28_AN_NUM  28

#define CH29_TRIS    &TRISE
#define CH29_LAT     &LATE
#define CH29_IO_BIT  5
#define CH29_AN_NUM  29

#define CH30_TRIS    &TRISE
#define CH30_LAT     &LATE
#define CH30_IO_BIT  6
#define CH30_AN_NUM  30

#define CH31_TRIS    &TRISE
#define CH31_LAT     &LATE
#define CH31_IO_BIT  7
#define CH31_AN_NUM  31

#else

#error PIC device family is not defined.

#endif

/****************************************************************************
  Macro:
    SysTimerInit()

  Summary:
    Initializes timer.

  Description:
    Initializes timer.
 
  Precondition:
    SysTimerInterrupt() interrupt handler must be defined in application code.

  Parameters:
    None.

  Returns:
    None.

  Remarks:
    Timer generates interrupts and calls SysTimerInterrupt() every mS.
  ***************************************************************************/
#if defined(__C30__)
#define SysTimerInit() {PR2=4000; _T2IE=1; T2CONbits.TON = 1;}
#else
#define SysTimerInit() {T2CONbits.T2CKPS = 3; PR2 = 180; T2CONbits.TMR2ON = 1; PIR1bits.TMR2IF = 0; PIE1bits.TMR2IE = 1;}
#endif


/****************************************************************************
  Function:
    void SysDelayMs(UINT16 delay)

  Summary:
    Delay execution on defined time.

  Description:
    Delay execution on defined time.
 
  Precondition:
    None

  Parameters:
    delay  - delay time in mS.

  Returns:
    None.

  Remarks:
    None.
  ***************************************************************************/
void SysDelayMs(UINT16 delay);

/****************************************************************************
  Function:
    void SysBoardInit(void)

  Summary:
    Initializes the board hardware.

  Description:
    Initializes the board hardware.
 
  Precondition:
    None.

  Parameters:
    None.

  Returns:
    None.

  Remarks:
    None.
  ***************************************************************************/
void SysBoardInit(void);

/****************************************************************************
  Function:
    void SysTimerInterrupt(void)

  Summary:
    Callback function, called every 1 ms.

  Description:
    Callback function, called every 1 ms.
 
  Precondition:
    None.

  Parameters:
    None.

  Returns:
    None.

  Remarks:
    SysTimerInterrupt() must be defined in the application code.
  ***************************************************************************/
void SysTimerInterrupt(void);

#endif





