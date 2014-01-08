/*******************************************************************
 * FileName: mib.h
 * Processor:       PIC18,PIC24E, PIC24F, PIC24H, dsPIC30F, dsPIC33F, dsPIC33E,PIC32
 * Compiler:        Microchip C18, C30, C32
 * This file was automatically generated on Mon Feb 13 2012 05:14:34
 * by mib2bib utility.
 * This file contains 'C' defines for dynamic OIDs and AgentID only.
 * Do not modify this file manually.
 * Include this file in your application source file that handles SNMP callbacks and TRAP.
 * 
 * Software License Agreement
 *
 * Copyright (C) 2012 Microchip Technology Inc.  All rights
 * reserved.
 *
 * Microchip licenses to you the right to use, modify, copy, and 
  * distribute: 
 * (i)  the Software when embedded on a Microchip microcontroller or 
 *      digital signal controller product ("Device") which is 
 *      integrated into Licensee's product; or 
 * (ii) ONLY the Software driver source files ENC28J60.c, ENC28J60.h,
 *		ENCX24J600.c and ENCX24J600.h ported to a non-Microchip device
 *		used in conjunction with a Microchip ethernet controller for
 *		the sole purpose of interfacing with the ethernet controller.
 *
 * You should refer to the license agreement accompanying this 
 * Software for additional information regarding your rights and 
 * obligations.
 *
 * THE SOFTWARE AND DOCUMENTATION ARE PROVIDED "AS IS" WITHOUT
 * WARRANTY OF ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT
 * LIMITATION, ANY WARRANTY OF MERCHANTABILITY, FITNESS FOR A 
 * PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT SHALL
 * MICROCHIP BE LIABLE FOR ANY INCIDENTAL, SPECIAL, INDIRECT OR
 * CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF
 * PROCUREMENT OF SUBSTITUTE GOODS, TECHNOLOGY OR SERVICES, ANY CLAIMS
 * BY THIRD PARTIES (INCLUDING BUT NOT LIMITED TO ANY DEFENSE 
 * THEREOF), ANY CLAIMS FOR INDEMNITY OR CONTRIBUTION, OR OTHER 
 * SIMILAR COSTS, WHETHER ASSERTED ON THE BASIS OF CONTRACT, TORT
 * (INCLUDING NEGLIGENCE), BREACH OF WARRANTY, OR OTHERWISE.
 *
 *******************************************************************/
#define MICROCHIP      255		// This is an Agent ID for use in SNMPNotify() only.
#define SYS_UP_TIME      250			//43.6.1.2.1.1.3 :  READONLY TIME_TICKS.
#define SNMP_DEMO_TRAP      500			//43.6.1.4.1.17095.1.0.1 :  READONLY BYTE.
#define TRAP_RECEIVER_ID      1			//43.6.1.4.1.17095.1.2.1.1.1 :  READWRITE BYTE.
#define TRAP_RECEIVER_ENABLED      2			//43.6.1.4.1.17095.1.2.1.1.2 :  READWRITE BYTE.
#define TRAP_RECEIVER_IP      3			//43.6.1.4.1.17095.1.2.1.1.3 :  READWRITE IP_ADDRESS.
#define TRAP_COMMUNITY      4			//43.6.1.4.1.17095.1.2.1.1.4 :  READWRITE ASCII_STRING.
#define LED_D5      5			//43.6.1.4.1.17095.1.3.1 :  READWRITE BYTE.
#define LED_D6      6			//43.6.1.4.1.17095.1.3.2 :  READWRITE BYTE.
#define PUSH_BUTTON      7			//43.6.1.4.1.17095.1.3.3 :  READONLY BYTE.
#define ANALOG_POT0      8			//43.6.1.4.1.17095.1.3.4 :  READONLY WORD.
#define LCD_DISPLAY      10			//43.6.1.4.1.17095.1.3.6 :  READWRITE ASCII_STRING.
#define USM_INDEX_ID      11			//43.6.1.4.1.17095.1.4.1.1.1 :  READONLY BYTE.
#define USER_SECURITY_NAME      12			//43.6.1.4.1.17095.1.4.1.1.2 :  READWRITE ASCII_STRING.
#define USM_AUTH_PROT      13			//43.6.1.4.1.17095.1.4.1.1.3 :  READWRITE BYTE.
#define USM_AUTH_KEY      14			//43.6.1.4.1.17095.1.4.1.1.4 :  READWRITE ASCII_STRING.
#define USM_PRIV_PROT      15			//43.6.1.4.1.17095.1.4.1.1.5 :  READWRITE BYTE.
#define USM_PRIV_KEY      16			//43.6.1.4.1.17095.1.4.1.1.6 :  READWRITE ASCII_STRING.
#define SNMP_TARGET_INDEX_ID      17			//43.6.1.4.1.17095.1.5.1.1.1 :  READONLY BYTE.
#define SNMP_TARGET_MP_MODEL      18			//43.6.1.4.1.17095.1.5.1.1.2 :  READWRITE BYTE.
#define SNMP_TARGET_SECURITY_MODEL      19			//43.6.1.4.1.17095.1.5.1.1.3 :  READWRITE BYTE.
#define SNMP_TARGET_SECURITY_NAME      20			//43.6.1.4.1.17095.1.5.1.1.4 :  READWRITE ASCII_STRING.
#define SNMP_TARGET_SECURITY_LEVEL      21			//43.6.1.4.1.17095.1.5.1.1.5 :  READWRITE BYTE.
#define ENGINE_ID      249			//43.6.1.6.3.10.2.1.1 :  READONLY ASCII_STRING.
#define ENGINE_BOOT      248			//43.6.1.6.3.10.2.1.2 :  READONLY DWORD.
#define ENGINE_TIME      247			//43.6.1.6.3.10.2.1.3 :  READONLY DWORD.
#define ENGINE_MAX_MSG      246			//43.6.1.6.3.10.2.1.4 :  READONLY WORD.
