/**********************************************************************
* 2011 Microchip Technology Inc.
*
* FileName:        rtccinit.c
* Dependencies:    Header (.h) files if applicable, see below
* Processor:       PIC24FJ128GA010
* Compiler:        MPLAB C30 v3.11 or higher
*
* SOFTWARE LICENSE AGREEMENT:
* Microchip Technology Incorporated ("Microchip") retains all
* ownership and intellectual property rights in the code accompanying
* this message and in all derivatives hereto.  You may use this code,
* and any derivatives created by any person or entity by or on your
* behalf, exclusively with Microchip's proprietary products.  Your
* acceptance and/or use of this code constitutes agreement to the
* terms and conditions of this notice.
*
* CODE ACCOMPANYING THIS MESSAGE IS SUPPLIED BY MICROCHIP "AS IS". NO
* WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT
* NOT LIMITED TO, IMPLIED WARRANTIES OF NON-INFRINGEMENT,
* MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS
* CODE, ITS INTERACTION WITH MICROCHIP'S PRODUCTS, COMBINATION WITH
* ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION.
*
* YOU ACKNOWLEDGE AND AGREE THAT, IN NO EVENT, SHALL MICROCHIP BE
* LIABLE, WHETHER IN CONTRACT, WARRANTY, TORT (INCLUDING NEGLIGENCE OR
* BREACH OF STATUTORY DUTY), STRICT LIABILITY, INDEMNITY,
* CONTRIBUTION, OR OTHERWISE, FOR ANY INDIRECT, SPECIAL, PUNITIVE,
* EXEMPLARY, INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, FOR COST OR
* EXPENSE OF ANY KIND WHATSOEVER RELATED TO THE CODE, HOWSOEVER
* CAUSED, EVEN IF MICROCHIP HAS BEEN ADVISED OF THE POSSIBILITY OR THE
* DAMAGES ARE FORESEEABLE.  TO THE FULLEST EXTENT ALLOWABLE BY LAW,
* MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN ANY WAY RELATED TO THIS
* CODE, SHALL NOT EXCEED THE PRICE YOU PAID DIRECTLY TO MICROCHIP
* SPECIFICALLY TO HAVE THIS CODE DEVELOPED.
*
* You agree that you are solely responsible for testing the code and
* determining its suitability.  Microchip has no obligation to modify,
* test, certify, or support the code.
*
* REVISION HISTORY:
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* Author            Date      		Comments on this revision
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* Mark Pallones		02/25/11		First Release
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*
* ADDITIONAL NOTES:
* Code Tested on:
* Explorer 16 Demo board with  PIC24FJ128GA010 controller
*
* DESCRIPTION:
* This code example describes how to use RTCC on PIC24F. Here I display
* Year,Day,Week,Weekday,Hour,Minute,Seconds using RTCC on the LCD via PMP. It also display
* a message on the LCD once the alarm is triggered.Based on initial condition the alarm will trigger after 30 sec
* where the date will change from July 11 to July 12
***********************************************************************************************************/
#include <p24fj128ga010.h>
#include <HardwareProfile.h>
void initRTCC( void)
{
    // Enables the OSCON write and set
    //_SOSCEN =1;
 	asm volatile ("mov #OSCCON,W1");
	asm volatile ("mov.b	#0x46, W2");    // unlock sequence
	asm volatile ("mov.b	#0x57, W3");
	asm volatile ("mov.b	#0x02, W0");    // SOSCEN =1
	asm volatile ("mov.b	W2, [W1]");
	asm volatile ("mov.b	W3, [W1]");
	asm volatile ("mov.b	W0, [W1]");


    //_RTCWREN = 1;       // unlock setting
	asm volatile("disi	#5");
	asm volatile("mov	#0x55, w7");
	asm volatile("mov	w7, _NVMKEY");
	asm volatile("mov	#0xAA, w8");
	asm volatile("mov	w8, _NVMKEY");
        asm volatile("bset	_RCFGCAL, #13");    // RTCWREN =1;
	asm volatile("nop");
	asm volatile("nop");

    _RTCEN = 0; // disable the clock

    // set 07/11/2011 MON 23:59:30
    _RTCPTR = 3;        // start the sequence
    RTCVAL = 0x2011;    // YEAR
    RTCVAL = 0x0711;    // MONTH-1/DAY-1
    RTCVAL = 0x0123;    // WEEKDAY/HOURS
    RTCVAL = 0x5930;    // MINUTES/SECONDS


    // lock and enable
    _RTCEN = 1;         // start the clock
    _RTCWREN = 0;       // lock settings
} // initRTCC


void setALARM( void)
{
    // disable alarm
    _ALRMEN = 0;

    // set the ALARM for a specific day of the year (my birthday)
    _ALRMPTR = 2;       // start the sequence
    ALRMVAL = 0x0712;   // MONTH-1/DAY-1
    ALRMVAL = 0x0200;   // WEEKDAY/HOUR
    ALRMVAL = 0x0000;   // MINUTES/SECONDS

    // set the repeat counter
    _ARPT = 0;          // once
    _CHIME = 1;         // indefinitely

    // set the alarm mask
    _AMASK = 0b1000;    // once a month
    _ALRMEN = 1;        // enable alarm
    _RTCIF = 0;         // clear flag
    _RTCIE = 1;         // enable interrupt

} // set ALARM



