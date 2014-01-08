/*********************************************************************
 *
 *  Application to Demo SNMP Server
 *  Support for SNMP module in Microchip TCP/IP Stack
 *	 - Implements the SNMP application
 *
 *********************************************************************
 * FileName:        CustomSNMPApp.c
 * Dependencies:    TCP/IP stack
 * Processor:       PIC18, PIC24F, PIC24H, dsPIC30F, dsPIC33F, PIC32
 * Compiler:        Microchip C32 v1.05 or higher
 *					Microchip C30 v3.12 or higher
 *					Microchip C18 v3.30 or higher
 *					HI-TECH PICC-18 PRO 9.63PL2 or higher
 * Company:         Microchip Technology, Inc.
 *
 * Software License Agreement
 *
 * Copyright (C) 2002-2009 Microchip Technology Inc.  All rights
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
 *
 * Author               Date      Comment
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * E. Wood     			4/26/08	  Moved from MainDemo.c
 * Amit Shirbhate 		09/24/08  SNMPv2c Support added.
 * Hrisikesh Sahu 		04/15/10  SNMPv2 trap format support.
 ********************************************************************/
#define __CUSTOMSNMPAPP_C

#include "TCPIPConfig.h"

#if defined(STACK_USE_SNMP_SERVER)

#include "TCPIP Stack/TCPIP.h"
#include "MainDemo.h"
#include "TCPIP Stack/SNMP.h"
#include "TCPIP Stack/SNMPv3.h"

/****************************************************************************
  Section:
	Global Variables
  ***************************************************************************/
/*This Macro is used to provide maximum try for a failure Trap server address  */
#if !defined(SNMP_TRAP_DISABLED)
#define MAX_TRY_TO_SEND_TRAP (10)
BYTE gSendTrapFlag=FALSE;//global flag to send Trap
BYTE gOIDCorrespondingSnmpMibID=MICROCHIP;
BYTE gGenericTrapNotification=ENTERPRISE_SPECIFIC;
BYTE gSpecificTrapNotification=VENDOR_TRAP_DEFAULT; // Vendor specific trap code
#endif 

#ifdef STACK_USE_SNMPV3_SERVER
static BYTE gSnmpv3UserSecurityName[SNMPV3_USER_SECURITY_NAME_LEN_MEM_USE];
#endif

/*
*  Default STACK_USE_SMIV2 is enabled . For Stack V5.31,  STACK_USE_SMIV2 should be disabled.
*/
#define STACK_USE_SMIV2

/* Update the Non record id OID value 
   which is part of CustomSnmpDemo.c file
*/
#define SNMP_MAX_NON_REC_ID_OID  3

/*
* gSnmpNonMibRecInfo[] is the list of static variable Parent OIDs which are not part of MIB.h file. 
* This structure is used to restrict access to static variables of SNMPv3 OIDs from SNMPv2c and SNMPv1 version. 
* With SNMPv3 all the OIDs accessible but when we are using SNMPv2c version , static variables of the SNMPv3 
* cannot be accessible with SNMPversion v2c.
* With V5.31 there was no MODULE-IDENTITY number in the SNMP.mib file. Now onwards we have supported SMIv2 
* standard and SNMP.mib has been updated with respect to SMIV2 standard and it also includes 
* MODULE-IDENTITY ( number 1)after ENTERPRISE-ID.
*/

/*
* This structure has been moved from snmp.c file to here. 
*/
#ifdef STACK_USE_SMIV2
/*
* With SMIv2 standard which includes MODULE-IDENTITY number with the existing OID string.
* For New snmp.mib file with SMIv2 standard
*/
/*
* ENTERPRISEID - 17095(Microchip) as per BER encoding standard 0x81,0x85,0x47
* Need to be modified with respect to customer enterprise ID 
*/

SNMPNONMIBRECDINFO gSnmpNonMibRecInfo[SNMP_MAX_NON_REC_ID_OID] =
{
#ifdef STACK_USE_SNMPV3_SERVER		
	/* SNMPv3 Static Variable OID string which is not part of mib.h file */
#endif			
	{{43,6,1,2,1,1},SNMP_V2C}, /* Max matching Subids of the iso+org (43),dod(6),internet(1),mgmt(2),MIB2(1),system(1) tree*/	
	{{43,6,1,4,1,0x81,0x85,0x47,0x1,1},SNMP_V2C}, 
	/*Max matching Subids of the iso+org (43),dod(6),internet(1),private(4),ENTERPRISE(17095),MODULE-IDENTITY(1),product tree*/			
	
};
/*
 * if snmp.mib file doesnot have MODULE-IDENTITY number then this is the following structure should be used
 */

#else 
/*
* OLD snmp.mib file with SMIv1 standard 
*/

SNMPNONMIBRECDINFO gSnmpNonMibRecInfo[SNMP_MAX_NON_REC_ID_OID] =
{
#ifdef STACK_USE_SNMPV3_SERVER		
	{{43,6,1,4,1,0x81,0x85,0x47,6},SNMP_V3},  /* SNMPv3 PVT test MIB OID is not part of mib.h file */
#endif			
	{{43,6,1,2,1,1},SNMP_V2C}, /* Max matching Subids of the iso+org (43),dod(6),internet(1),mgmt(2),MIB2(1),system(1) tree*/	
	{{43,6,1,4,1,0x81,0x85,0x47,0x1},SNMP_V2C}, 
	/*Max matching Subids of the iso+org (43),dod(6),internet(1),private(4),ENTERPRISE(17095),product tree*/			
	
};

#endif /* STACK_USE_SMIV2 */

#if !defined(SNMP_TRAP_DISABLED)
/*
#if defined(SNMP_STACK_USE_V2_TRAP) || defined(SNMP_V1_V2_TRAP_WITH_SNMPV3)
//if gSetTrapSendFlag == FALSE then the last varbind variable for 
//multiple varbind variable pdu structure or if there is only varbind variable send.
// if gSetTrapSendFlag == TRUE, then v2 trap pdu is expecting more varbind variable.
BYTE	gSetTrapSendFlag = FALSE;
#endif 
*/
BYTE	gSetTrapSendFlag = FALSE;

/*Initialize trap table with no entries.*/
TRAP_INFO trapInfo = { TRAP_TABLE_SIZE };


static DWORD SNMPGetTimeStamp(void);

static BOOL SendNotification(BYTE receiverIndex, SNMP_ID var, SNMP_VAL val,UINT8 targetIndex);

UINT8 	gSendTrapSMstate=0;
static BOOL gtrapSMStateUpdate=FALSE;

#endif
/****************************************************************************
  ===========================================================================
  Section:
	SNMP Routines
  ===========================================================================
  ***************************************************************************/



#if !defined(SNMP_TRAP_DISABLED)
/****************************************************************************
  Function:
 	 BOOL SendNotification(BYTE receiverIndex, SNMP_ID var, SNMP_VAL val,UINT8 targetIndex)
 
  Summary:			
	Prepare, validate remote node which will receive trap and send trap pdu.
 	  	  
  Description:		
    This routine prepares the trap notification pdu, sends ARP and get
	remote device MAC address to which notification to sent, sends
	the notification. 
	Notofication state machine is getting updated if there is any ARP resolution failure for 
	a perticular trap destination address.
	
  PreCondition:
	SNMPTrapDemo() is called.
 	
  parameters:
     receiverIndex - The index to array where remote ip address is stored.  
     var		   - SNMP var ID that is to be used in notification
	 val           - Value of var. Only value of BYTE, WORD or DWORD can be sent.
	 targetIndex - snmpv3 target index
  Return Values:          
 	 TRUE	-	If notification send is successful.
 	 FALSE	-	If send notification failed.
 	 
  Remarks:
     None.
 *************************************************************************/
static BOOL SendNotification(BYTE receiverIndex, SNMP_ID var, SNMP_VAL val,UINT8 targetIndex)
{
    static enum { SM_PREPARE, SM_NOTIFY_WAIT } smState = SM_PREPARE;
    IP_ADDR IPAddress;
	static BYTE tempRxIndex;
	static IP_ADDR tempIpAddress;
	
    // Convert local to network order.
    IPAddress.v[0] = trapInfo.table[receiverIndex].IPAddress.v[3];
    IPAddress.v[1] = trapInfo.table[receiverIndex].IPAddress.v[2];
    IPAddress.v[2] = trapInfo.table[receiverIndex].IPAddress.v[1];
    IPAddress.v[3] = trapInfo.table[receiverIndex].IPAddress.v[0];
    
	if(gtrapSMStateUpdate == TRUE)
	{
		smState = SM_PREPARE;
		gtrapSMStateUpdate = FALSE;
	}

    switch(smState)
    {
	    case SM_PREPARE:
			gSendTrapSMstate = smState;
			tempRxIndex=receiverIndex;			
			// Convert local to network order.
			tempIpAddress.v[0] = trapInfo.table[receiverIndex].IPAddress.v[3];
			tempIpAddress.v[1] = trapInfo.table[receiverIndex].IPAddress.v[2];
			tempIpAddress.v[2] = trapInfo.table[receiverIndex].IPAddress.v[1];
			tempIpAddress.v[3] = trapInfo.table[receiverIndex].IPAddress.v[0];
	        SNMPNotifyPrepare(&IPAddress,
	                          trapInfo.table[receiverIndex].community,
	                          trapInfo.table[receiverIndex].communityLen,
	                          MICROCHIP,            	   // Agent ID Var
	                          gSpecificTrapNotification,   // Specifc Trap notification code
	                          SNMPGetTimeStamp());
	        smState = SM_NOTIFY_WAIT;
			
	        break;
	
	    case SM_NOTIFY_WAIT:
			gSendTrapSMstate = smState;
	        if ( SNMPIsNotifyReady(&IPAddress) )
	        {
#if defined(SNMP_V1_V2_TRAP_WITH_SNMPV3)	
				if(targetIndex == (SNMPV3_USM_MAX_USER-1))
					smState = SM_PREPARE;
				if((gSnmpv3TrapConfigData[targetIndex].messageProcessingModelType == SNMPV3_MSG_PROCESSING_MODEL)
					&& (gSnmpv3TrapConfigData[targetIndex].securityModelType == SNMPV3_USM_SECURITY_MODEL))
			   		Snmpv3Notify(var, val, 0,targetIndex);
				else if((gSnmpv3TrapConfigData[targetIndex].messageProcessingModelType == SNMPV2C_MSG_PROCESSING_MODEL)
					&& (gSnmpv3TrapConfigData[targetIndex].securityModelType == SNMPV2C_SECURITY_MODEL))
					SNMPNotify(var, val, 0);
				else if((gSnmpv3TrapConfigData[targetIndex].messageProcessingModelType == SNMPV1_MSG_PROCESSING_MODEL)
					&& (gSnmpv3TrapConfigData[targetIndex].securityModelType == SNMPV1_SECURITY_MODEL))
					SNMPNotify(var, val, 0);
				else 
					return TRUE;
#else
	            smState = SM_PREPARE;
				SNMPNotify(var, val, 0);
#endif
	            return TRUE;
	        }
			/* if trapInfo table address for a perticular index is different comparing to the SM_PREPARE IP address
				then change the state to SM_PREPARE*/
			if((tempIpAddress.Val != IPAddress.Val) && 
					(tempRxIndex == receiverIndex))
			{
				smState = SM_PREPARE;
			}
			/* Change state machine from SM_NOTIFY_WAIT to SM_PREPARE if incoming trap destination 
			index is different from the SM_PREPARE  trap destination index*/
			if(tempRxIndex != receiverIndex)
				smState=SM_PREPARE;
			
    }

    return FALSE;
}

#if defined(SNMP_STACK_USE_V2_TRAP) || defined(SNMP_V1_V2_TRAP_WITH_SNMPV3)
/**************************************************************************
  Function:
 	void SNMPV2TrapDemo(void)
 
  Summary:	
  	Send SNMP V2 notification with multiple varbinds.
 	  	  
  Description:		
	This routine sends a trap v2 pdu with multiple varbind variables
	for the predefined ip addresses with the agent. And as per RFC1905 
	the first two variable bindings in the varbind pdu list of an
   	SNMPv2-Trap-PDU are sysUpTime.0 and snmpTrapOID.0 respectively.
   	To support multiple varbind, user need to call SendNotification()
    for the first varbind variable and SendNotification() will do the 
    arp resolve and adds sysUpTime.0 and snmpTrapOID.0 variable to 
    the pdu. For the second varbind variable onwards user need to 
   	call only SNMPNotify().
	In this demo , snmpv2 trap includes ANALOG_POT0,PUSH_BUTTON and LED_D5
	variable bindings and TrapCommunity variable is being used as part of the fourth varbind of
	the TRAP PDU which is ASCII string format.
	and this trap can be generated by using Analog portmeter value.
	and SNMPv2-Trap-PDU will be generated only when pot meter reading exceeds 12.
	
	gSetTrapSendFlag Should be set to TRUE when user is trying to send first 
	variable binding and gSetTrapSendFlag should be set to FALSE before 
    sending the last variable binding.

	* if user is sending only one variable binding then 
	* gSetTrapSendFlag should be set to False.	
    * user can add more variable bindings.
    * For ASCII STR trap , argument VAL contains the pointer address of the string variable.
  PreCondition:
 	Application defined event occurs to send the trap.
 	
  parameters:
     None.
 
  Returns:          
 	 None.
 
  Remarks:
    This routine guides how to build a event generated trap notification.
 *************************************************************************/
void SNMPV2TrapDemo(void)
{
	static DWORD tempTimerRead = 0;
	static BYTE	trapIndex=0;
	static SNMP_VAL		analogPotVal;
	static BYTE potReadLock = FALSE;
	static BYTE timeLock = FALSE;
#if defined(SNMP_V1_V2_TRAP_WITH_SNMPV3)	
	static BYTE userIndex=0;
#endif
	UINT8		targetIndex = 0;
	UINT8		retVal = 0;
	
	if(timeLock==(BYTE)FALSE)
	{
		tempTimerRead=TickGet();
		timeLock=TRUE;
	}
/*
	Specify SNMPV2 specific trap ID Here. Which will help Ireasoning and other SNMP manager tools to 
	recognise the trap information and it will help the SNMP manager tool to decrypt the 
	trap information. 

	This ID is only related to trap ID. and this implementaion is only for TRAPv2 specific.
*/

	SNMPNotifyInfo.trapIDVar = SNMP_DEMO_TRAP;


	for(;trapIndex<TRAP_TABLE_SIZE;trapIndex++)
	{	
		if(!trapInfo.table[trapIndex].Flags.bEnabled)
			continue;
		
		//Read POT reading once and send trap to all configured recipient
		if(potReadLock ==(BYTE)FALSE)
		{
#if defined(__18CXX)
			// Wait until A/D conversion is done
			ADCON0bits.GO = 1;
			while(ADCON0bits.GO);
			// Convert 10-bit value into ASCII string
			analogPotVal.word= (WORD)ADRES;
#else
			analogPotVal.word= (WORD)ADC1BUF0;
#endif
			potReadLock    = TRUE;
		}
	
	//	if(analogPotVal.word >512u)
		if(analogPotVal.word >12u)
		{
			/*
			 * prepare  and send multivarbind pdu using pot meter value. 
			 * for SNMP v2 trap sysUpTime.0 and SNMPv2TrapOID.0 are mandatory
			 * apart from these varbinds, push button and potmeter OID are included
			 * to this pdu.
			*/
			//gSpecificTrapNotification = 1; //expecting 1 should be the specific trap.
			//gGenericTrapNotification = ENTERPRISE_SPECIFIC;
			//gSetTrapSendFlag = TRUE;
			// insert ANALOG_POT0 OID value and OID to the varbind pdu
			//set global flag gSetTrapSendFlag to TRUE , it signifies that there are more than one 
			// variable need to be the part of SNMP v2 TRAP. 
			// if there is  only varbind variable to be the part of SNMP v2 trap, 
			// then user should set gSetTrapSendFlag to FALSE.
			//gSetTrapSendFlag = FALSE;
#if defined(SNMP_V1_V2_TRAP_WITH_SNMPV3)	
			for(targetIndex=userIndex;targetIndex<SNMPV3_USM_MAX_USER;targetIndex++)
			{
#endif			
				gSpecificTrapNotification = 1; //expecting 1 should be the specific trap.
				gGenericTrapNotification = ENTERPRISE_SPECIFIC;
				gSetTrapSendFlag = TRUE;
				retVal =SendNotification(trapIndex,ANALOG_POT0,analogPotVal,targetIndex);
				if((gSendTrapSMstate == 0x0) && (retVal == FALSE)) // gSendTrapSMstate == SM_PREPARE
				{
					retVal = SendNotification(trapIndex, ANALOG_POT0, analogPotVal,targetIndex);
				}
				if(retVal == FALSE)
				{
#if defined(SNMP_V1_V2_TRAP_WITH_SNMPV3) // to keep tarck of the usr index for SNMPv3
                    userIndex = targetIndex;
#endif
                    if((TickGet()- SNMPGetTrapTime())> 2*TICK_SECOND)
                    {
                        trapIndex++;
                        gtrapSMStateUpdate = TRUE;
                        if(SNMPNotifyInfo.socket != INVALID_UDP_SOCKET)
                        {
                                UDPClose(SNMPNotifyInfo.socket);
                                SNMPNotifyInfo.socket = INVALID_UDP_SOCKET;
                        }
                        return;
                    }
                    return ;
				}
				//prepare PUSH_BUTTON trap .for the next trap varbind we need to use snmp_notify instead of 
				// SendNotification(), because we have already prepared SNMP v2 trap header 
				//and arp has been resolved already.
				
				analogPotVal.byte = BUTTON0_IO;
				
#if defined(SNMP_V1_V2_TRAP_WITH_SNMPV3)	
				if((gSnmpv3TrapConfigData[targetIndex].messageProcessingModelType == SNMPV3_MSG_PROCESSING_MODEL)
					&& (gSnmpv3TrapConfigData[targetIndex].securityModelType == SNMPV3_USM_SECURITY_MODEL))
			   		Snmpv3Notify(PUSH_BUTTON,analogPotVal,0,targetIndex);
				else if((gSnmpv3TrapConfigData[targetIndex].messageProcessingModelType == SNMPV2C_MSG_PROCESSING_MODEL)
					&& (gSnmpv3TrapConfigData[targetIndex].securityModelType == SNMPV2C_SECURITY_MODEL))
					SNMPNotify(PUSH_BUTTON,analogPotVal,0);
				else if((gSnmpv3TrapConfigData[targetIndex].messageProcessingModelType == SNMPV1_MSG_PROCESSING_MODEL)
					&& (gSnmpv3TrapConfigData[targetIndex].securityModelType == SNMPV1_SECURITY_MODEL))
					SNMPNotify(PUSH_BUTTON,analogPotVal,0);
				else
						continue;
				//Snmpv3Notify(PUSH_BUTTON,analogPotVal,0);
#else
				SNMPNotify(PUSH_BUTTON,analogPotVal,0);
#endif
				// if this is the last trap variable need to be the part of SNMP v2 Trap,
				// then we should disable gSetTrapSendFlag to FALSE
				
				//gSetTrapSendFlag = FALSE;
				analogPotVal.byte = LED0_IO;
#if defined(SNMP_V1_V2_TRAP_WITH_SNMPV3)				
				//Snmpv3Notify(LED_D5,analogPotVal,0);
				if((gSnmpv3TrapConfigData[targetIndex].messageProcessingModelType == SNMPV3_MSG_PROCESSING_MODEL)
					&& (gSnmpv3TrapConfigData[targetIndex].securityModelType == SNMPV3_USM_SECURITY_MODEL))
					Snmpv3Notify(LED_D5,analogPotVal,0,targetIndex);
				else if((gSnmpv3TrapConfigData[targetIndex].messageProcessingModelType == SNMPV2C_MSG_PROCESSING_MODEL)
					&& (gSnmpv3TrapConfigData[targetIndex].securityModelType == SNMPV2C_SECURITY_MODEL))
					SNMPNotify(LED_D5,analogPotVal,0);
				else if((gSnmpv3TrapConfigData[targetIndex].messageProcessingModelType == SNMPV1_MSG_PROCESSING_MODEL)
					&& (gSnmpv3TrapConfigData[targetIndex].securityModelType == SNMPV1_SECURITY_MODEL))
					SNMPNotify(LED_D5,analogPotVal,0);
#else
				SNMPNotify(LED_D5,analogPotVal,0);

#endif
// if this is the last trap variable need to be the part of SNMP  Trap v2,
// then we should disable gSetTrapSendFlag to FALSE
				gSetTrapSendFlag = FALSE;
#if defined(SNMP_V1_V2_TRAP_WITH_SNMPV3)				
				//ASCII String is the fouth varbind of the TRAP ODU
				// for this TRAP_COMMUNITy string has been used as the 
				//  Fourth varbind.
				{
					BYTE asciiStr[] = {"ascii_str_trap"};
					analogPotVal.dword = (PTR_BASE)&asciiStr;
					if((gSnmpv3TrapConfigData[targetIndex].messageProcessingModelType == SNMPV3_MSG_PROCESSING_MODEL)
						&& (gSnmpv3TrapConfigData[targetIndex].securityModelType == SNMPV3_USM_SECURITY_MODEL))
						Snmpv3Notify(TRAP_COMMUNITY,analogPotVal,0,targetIndex);
					else if((gSnmpv3TrapConfigData[targetIndex].messageProcessingModelType == SNMPV2C_MSG_PROCESSING_MODEL)
						&& (gSnmpv3TrapConfigData[targetIndex].securityModelType == SNMPV2C_SECURITY_MODEL))
						SNMPNotify(TRAP_COMMUNITY,analogPotVal,0);
					else if((gSnmpv3TrapConfigData[targetIndex].messageProcessingModelType == SNMPV1_MSG_PROCESSING_MODEL)
						&& (gSnmpv3TrapConfigData[targetIndex].securityModelType == SNMPV1_SECURITY_MODEL))
						SNMPNotify(TRAP_COMMUNITY,analogPotVal,0);
				}
#else
				{
					BYTE asciiStr[] = {"ascii_str_trap"};
					analogPotVal.dword = (PTR_BASE)&asciiStr;
					SNMPNotify(TRAP_COMMUNITY,analogPotVal,0);
				}
#endif


#if defined(SNMP_V1_V2_TRAP_WITH_SNMPV3)
				if(userIndex == SNMPV3_USM_MAX_USER-1)
					userIndex = 0;
			}
#endif			
		}
	}

	//Try for max 5 seconds to send TRAP, do not get block in while()
	if((TickGet()-tempTimerRead)>(5*TICK_SECOND))
	{
            UDPDiscard();
            potReadLock = FALSE;
            timeLock = FALSE;
            analogPotVal.word = 0;
            if(trapIndex>=TRAP_TABLE_SIZE)
            {
                trapIndex = 0;
            }
            gSendTrapSMstate = 0;
            return;
	}
}

#endif /* SNMP_STACK_USE_V2_TRAP  || SNMP_V1_V2_TRAP_WITH_SNMPV3 */

/**************************************************************************
  Function:
 	void SNMPTrapDemo(void)
 
  Summary:	
  	Send trap pdu demo application.
 	  	  
  Description:		
	This routine is used to send various trap events for the predefined ip addresses with the
	agent. Events like ANALOG POT value, PUSH_BUTTON and ASCII Str notification.
	ANALOG POT event -  When there is a POT value greater than 12 and for this ANALOG_POT0
	is used as a TRAP PDU variable for this event.
	PUSH BUTTON event -  BUTTON2_IO is the varaible used for this trap event.
	ASCII STR event - TRAP_COMMUNITY variable is used for this event and this event will occur when
	both LED1 and LED2 are blinking.
	      
  PreCondition:
 	Application defined event occurs to send the trap.
 	
  parameters:
     None.
 
  Returns:          
 	 None.
 
  Remarks:
    This routine guides how to build a event generated trap notification.
    The application should make use of SNMPSendTrap() routine to generate 
    and send trap.
 *************************************************************************/
void SNMPTrapDemo(void)
{
	static DWORD TimerRead=0;
	static BOOL analogPotNotify = FALSE,buttonPushNotify=FALSE,asciiStrNotify=FALSE;
	static BYTE anaPotNotfyCntr=0,buttonPushNotfyCntr=0;
	static SNMP_VAL buttonPushval,analogPotVal;
	static BYTE potReadLock=FALSE,buttonLock=FALSE;
	static BYTE timeLock=FALSE;
	UINT8		targetIndex;
	BOOL		retVal=TRUE;
	static BYTE	trapIndex=0;
#if defined(SNMP_V1_V2_TRAP_WITH_SNMPV3)	
	static BYTE userIndex=0,userIndex1=0,userIndex2=0;
#endif

#if defined(SNMP_V1_V2_TRAP_WITH_SNMPV3) || defined(SNMP_STACK_USE_V2_TRAP)
	/*
		Specify SNMPV2 specific trap ID Here. Which will help Ireasoning and other SNMP manager tools to 
		recognise the trap information and it will help the SNMP manager tool to decrypt the 
		trap information. 
	
		This ID is only related to trap ID. and this implementaion is only for TRAPv2 specific.
	*/
	SNMPNotifyInfo.trapIDVar = SNMP_DEMO_TRAP;
#endif


	targetIndex = 0;
	if(timeLock==(BYTE)FALSE)
	{
		TimerRead=TickGet();
		timeLock=TRUE;
	}

	if(anaPotNotfyCntr >= trapInfo.Size)
	{
		anaPotNotfyCntr = 0;
		potReadLock=FALSE;
		//analogPotNotify = FALSE;
		analogPotNotify = TRUE;
	}
	
	if(!analogPotNotify)
	{
		//Read POT reading once and send trap to all configured recipient
		if(potReadLock ==(BYTE)FALSE)
		{
#if defined(__18CXX)
            // Wait until A/D conversion is done
            ADCON0bits.GO = 1;
            while(ADCON0bits.GO);

            // Convert 10-bit value into ASCII string
            analogPotVal.word= (WORD)ADRES;
#else
            analogPotVal.word= (WORD)ADC1BUF0;
#endif
			
			//Avoids Reading POT for every iteration unless trap sent to each configured recipients 
			potReadLock=TRUE; 
		}
		if(trapInfo.table[anaPotNotfyCntr].Flags.bEnabled)
		{
			if(analogPotVal.word >12u)
			{
#if defined(SNMP_V1_V2_TRAP_WITH_SNMPV3)	
				for(targetIndex=userIndex;targetIndex<SNMPV3_USM_MAX_USER;targetIndex++)
				{
#endif			
					gSpecificTrapNotification=POT_READING_MORE_512;
					gGenericTrapNotification=ENTERPRISE_SPECIFIC;
					gSetTrapSendFlag = FALSE;
					retVal = SendNotification(anaPotNotfyCntr, ANALOG_POT0, analogPotVal,targetIndex);
					if((gSendTrapSMstate == 0x0) && (retVal == FALSE)) // gSendTrapSMstate == SM_PREPARE
					{
						retVal = SendNotification(anaPotNotfyCntr, ANALOG_POT0, analogPotVal,targetIndex);
					}
#if defined(SNMP_V1_V2_TRAP_WITH_SNMPV3)                    
                    if((retVal == TRUE) && (targetIndex == (SNMPV3_USM_MAX_USER-1)))
                    {
                        anaPotNotfyCntr++;
                    }
					else if(retVal == FALSE)
#else
					if(retVal == TRUE)
						anaPotNotfyCntr++;
					else 
#endif						
                    {
#if defined(SNMP_V1_V2_TRAP_WITH_SNMPV3) // to keep tarck of the usr index for SNMPv3
			            userIndex = targetIndex;
#endif
                        if((TickGet()- SNMPGetTrapTime())> 2*TICK_SECOND)
                        {
                            anaPotNotfyCntr++;
                            gtrapSMStateUpdate = TRUE;
                            if(SNMPNotifyInfo.socket != INVALID_UDP_SOCKET)
                            {
                                UDPClose(SNMPNotifyInfo.socket);
                                SNMPNotifyInfo.socket = INVALID_UDP_SOCKET;
                            }
                            return;
                        }
			            return ;
                    }
#if defined(SNMP_V1_V2_TRAP_WITH_SNMPV3)
                    if(userIndex == SNMPV3_USM_MAX_USER-1)
                            userIndex = 0;
                }
#endif			
		}
		}
		else
           anaPotNotfyCntr++;
			
	}


	if(buttonPushNotfyCntr==trapInfo.Size)
	{
        buttonPushNotfyCntr = 0;
        buttonLock=FALSE;
        buttonPushNotify = FALSE;
	}


	if(buttonLock == (BYTE)FALSE)
	{
        if(BUTTON2_IO == 0u)
        {
            buttonPushNotify = TRUE;
            buttonLock =TRUE;
        }
	}

	if(buttonPushNotify)
	{			  
		buttonPushval.byte = 0;
		if ( trapInfo.table[buttonPushNotfyCntr].Flags.bEnabled )
		{
#if defined(SNMP_V1_V2_TRAP_WITH_SNMPV3)	
			for(targetIndex=userIndex1;targetIndex<SNMPV3_USM_MAX_USER;targetIndex++)
			{
#endif			
				gSpecificTrapNotification=POT_READING_MORE_512;
				gGenericTrapNotification=ENTERPRISE_SPECIFIC;
				gSetTrapSendFlag = FALSE;
				retVal =  SendNotification(buttonPushNotfyCntr, PUSH_BUTTON, buttonPushval,targetIndex);
				if((gSendTrapSMstate == 0x0) && (retVal == FALSE)) // gSendTrapSMstate == SM_PREPARE
				{
					retVal = SendNotification(buttonPushNotfyCntr, PUSH_BUTTON, buttonPushval,targetIndex);
				}
#if defined(SNMP_V1_V2_TRAP_WITH_SNMPV3)	
				if((retVal==TRUE)&& (targetIndex == (SNMPV3_USM_MAX_USER-1)))
#else
				if(retVal==TRUE)
#endif									
					buttonPushNotfyCntr++;					
#if defined(SNMP_V1_V2_TRAP_WITH_SNMPV3)
				else
					userIndex1 = targetIndex;				
				if(userIndex1 == SNMPV3_USM_MAX_USER-1)
					userIndex1 = 0;
			}
#endif			
		}
		else
			buttonPushNotfyCntr++;
	}

/*
	ASCII String trap support . When LED2 and LED1 are on then ASCII string trap will be send.
	TrapCommunity(4) is used as a variable for both TRAPv1 and TRAPv2 PDU.
*/
	if((!LED1_IO && !LED2_IO) && (trapIndex >= TRAP_TABLE_SIZE))
	{
		asciiStrNotify = FALSE;
	}
	else if((LED1_IO && LED2_IO) && (trapIndex < TRAP_TABLE_SIZE))
		asciiStrNotify = TRUE;

	if(asciiStrNotify)
	{
		for(;trapIndex<TRAP_TABLE_SIZE;trapIndex++)
		{	
			BYTE asciiStr[] = {"ascii_str_trap"};
			SNMP_VAL asciistrVal;
			asciistrVal.dword = (PTR_BASE)&asciiStr;
			
			if(!trapInfo.table[trapIndex].Flags.bEnabled)
			continue;		
			
			if(analogPotVal.word <= 12u)
				continue;

#if defined(SNMP_V1_V2_TRAP_WITH_SNMPV3)	
			for(targetIndex=userIndex2;targetIndex<SNMPV3_USM_MAX_USER;targetIndex++)
			{
#endif			
				gSpecificTrapNotification=POT_READING_MORE_512;
				gGenericTrapNotification=ENTERPRISE_SPECIFIC;
				gSetTrapSendFlag = FALSE;
				retVal =  SendNotification(trapIndex, TRAP_COMMUNITY, asciistrVal,targetIndex);
				if((gSendTrapSMstate == 0x0) && (retVal == FALSE)) // gSendTrapSMstate == SM_PREPARE
				{
					retVal = SendNotification(trapIndex, TRAP_COMMUNITY, asciistrVal,targetIndex);
				}
				if(retVal == FALSE)
				{
#if defined(SNMP_V1_V2_TRAP_WITH_SNMPV3) // to keep tarck of the usr index for SNMPv3
					userIndex2 = targetIndex;
#endif				
                    if((TickGet()- SNMPGetTrapTime())> 5*TICK_SECOND)
                    {
                        trapIndex++;
                        gtrapSMStateUpdate = TRUE;
                        if(SNMPNotifyInfo.socket != INVALID_UDP_SOCKET)
                        {
                            UDPClose(SNMPNotifyInfo.socket);
                            SNMPNotifyInfo.socket = INVALID_UDP_SOCKET;
                        }
                        return;
                    }
					return ;
				}
					
#if defined(SNMP_V1_V2_TRAP_WITH_SNMPV3)
			if(userIndex2 == SNMPV3_USM_MAX_USER-1)
				userIndex2 = 0;
			}
#endif		
		}
	}

	//Try for max 5 seconds to send TRAP, do not get block in while()
	if((TickGet()-TimerRead)>(5*TICK_SECOND))
	{
		UDPDiscard();
		buttonPushNotfyCntr = 0;
		buttonLock=FALSE;
		buttonPushNotify = FALSE;
		anaPotNotfyCntr = 0;
		potReadLock=FALSE;
		analogPotNotify = FALSE;
		asciiStrNotify = FALSE;
		timeLock=FALSE;
		gSpecificTrapNotification=VENDOR_TRAP_DEFAULT;
		gGenericTrapNotification=ENTERPRISE_SPECIFIC;
                if(trapIndex>=TRAP_TABLE_SIZE)
                {
                    trapIndex = 0;
                }
		gSendTrapSMstate = 0;	
		return;
	}

}


/**************************************************************************
  Function:
 	void SNMPSendTrap(void)
 
  Summary:	
  	 Prepare, validate remote node which will receive trap and send trap pdu.
 	 	  
  Description:		
     This function is used to send trap notification to previously 
     configured ip address if trap notification is enabled. There are
     different trap notification code. The current implementation
     sends trap for authentication failure (4).
  
  PreCondition:
 	 If application defined event occurs to send the trap.
 
  parameters:
     None.
 
  Returns:          
 	 None.
 
  Remarks:
     This is a callback function called by the application on certain 
     predefined events. This routine only implemented to send a 
     authentication failure Notification-type macro with PUSH_BUTTON
     oid stored in MPFS. If the ARP is no resolved i.e. if 
     SNMPIsNotifyReady() returns FALSE, this routine times 
     out in 5 seconds. This routine should be modified according to 
     event occured and should update corrsponding OID and notification
     type to the trap pdu.
 *************************************************************************/
void SNMPSendTrap(void)
{
	static BYTE timeLock=FALSE;
	static BYTE receiverIndex=0; ///is application specific
	IP_ADDR remHostIPAddress,* remHostIpAddrPtr;
	SNMP_VAL val;
	static DWORD TimerRead;
#if defined(SNMP_V1_V2_TRAP_WITH_SNMPV3)	
	static BYTE userIndex=0;
#endif
	static enum 
	{
		SM_PREPARE,
		SM_NOTIFY_WAIT 
	} smState = SM_PREPARE;

#if defined(SNMP_V1_V2_TRAP_WITH_SNMPV3) || defined(SNMP_STACK_USE_V2_TRAP)
	/*
		Specify SNMPV2 specific trap ID Here. Which will help Ireasoning and other SNMP manager tools to 
		recognise the trap information and it will help the SNMP manager tool to decrypt the 
		trap information. 
	
		This ID is only related to trap ID. and this implementaion is only for TRAPv2 specific.
	*/
	SNMPNotifyInfo.trapIDVar = SNMP_DEMO_TRAP;
#endif




	if(trapInfo.table[receiverIndex].Flags.bEnabled)
	{
		remHostIPAddress.v[0] = trapInfo.table[receiverIndex].IPAddress.v[3];
		remHostIPAddress.v[1] = trapInfo.table[receiverIndex].IPAddress.v[2];
		remHostIPAddress.v[2] = trapInfo.table[receiverIndex].IPAddress.v[1];
		remHostIPAddress.v[3] = trapInfo.table[receiverIndex].IPAddress.v[0];
		remHostIpAddrPtr = &remHostIPAddress;
		if(timeLock==(BYTE)FALSE)
		{
			TimerRead=TickGet();
			timeLock=TRUE;
		}
	}	
	else
	{
		receiverIndex++;
		if((receiverIndex == (BYTE)TRAP_TABLE_SIZE))
		{
			receiverIndex=0;
			timeLock=FALSE;
			gSendTrapFlag=FALSE;	
			UDPDiscard();
		}
		return;
		
	}
		
	switch(smState)
	{
	
		case SM_PREPARE:

			SNMPNotifyPrepare(remHostIpAddrPtr,trapInfo.table[receiverIndex].community,
						trapInfo.table[receiverIndex].communityLen,
						MICROCHIP,			  // Agent ID Var
						gSpecificTrapNotification,					  // Notification code.
						SNMPGetTimeStamp());
			smState++;
			break;
			
		case SM_NOTIFY_WAIT:
			if(SNMPIsNotifyReady(remHostIpAddrPtr))
			{
#if defined(SNMP_V1_V2_TRAP_WITH_SNMPV3)	
				UINT8 targetIndex = 0;
#endif
				smState = SM_PREPARE;
		 		val.byte = 0;
				receiverIndex++;
#if defined(SNMP_V1_V2_TRAP_WITH_SNMPV3)	
				for(targetIndex=userIndex;targetIndex<SNMPV3_USM_MAX_USER;targetIndex++)
				{
					if((gSnmpv3TrapConfigData[targetIndex].messageProcessingModelType == SNMPV3_MSG_PROCESSING_MODEL)
						&& (gSnmpv3TrapConfigData[targetIndex].securityModelType == SNMPV3_USM_SECURITY_MODEL))
						Snmpv3Notify(gOIDCorrespondingSnmpMibID, val, 0,targetIndex);
					else if((gSnmpv3TrapConfigData[targetIndex].messageProcessingModelType == SNMPV2C_MSG_PROCESSING_MODEL)
						&& (gSnmpv3TrapConfigData[targetIndex].securityModelType == SNMPV2C_SECURITY_MODEL))
						SNMPNotify(gOIDCorrespondingSnmpMibID, val, 0);
					else if((gSnmpv3TrapConfigData[targetIndex].messageProcessingModelType == SNMPV1_MSG_PROCESSING_MODEL)
						&& (gSnmpv3TrapConfigData[targetIndex].securityModelType == SNMPV1_SECURITY_MODEL))
						SNMPNotify(gOIDCorrespondingSnmpMibID, val, 0);
				}
				//Snmpv3Notify(PUSH_BUTTON,analogPotVal,0);
#else
				SNMPNotify(gOIDCorrespondingSnmpMibID, val, 0);
#endif
				//application has to decide on which SNMP var OID to send. Ex. PUSH_BUTTON	
				//SNMPNotify(gOIDCorrespondingSnmpMibID, val, 0);
            	smState = SM_PREPARE;
				UDPDiscard();
				break;
			}
	}	
		
	//Try for max 5 seconds to send TRAP, do not get block in while()
	if((TickGet()-TimerRead)>(5*TICK_SECOND)|| (receiverIndex == (BYTE)TRAP_TABLE_SIZE))
	{
		UDPDiscard();
		smState = SM_PREPARE;
		receiverIndex=0;
		timeLock=FALSE;
		gSendTrapFlag=FALSE;
		return;
	}

}

#endif

/*********************************************************************
  Function:
 	 BYTE SNMPValidateCommunity(BYTE* community)
 
  Summary:			
 	 Validates community name for access control. 
 
  Description:		
     This function validates the community name for the mib access to NMS.
 	 The snmp community name received in the request pdu is validated for
 	 read and write community names. The agent gives an access to the mib
 	 variables only if the community matches with the predefined values.
  	 This routine also sets a gloabal flag to send trap if authentication
 	 failure occurs.
  
  PreCondition:
 	 SNMPInit is already called.
 
  parameters:
     community - Pointer to community string as sent by NMS.
 
  Returns:          
 	 This routine returns the community validation result as 
  	 READ_COMMUNITY or WRITE_COMMUNITY or INVALID_COMMUNITY	
 
  Remarks:
     This is a callback function called by module. User application must 
  	 implement this function and verify that community matches with 
 	 predefined value. This validation occurs for each NMS request.
 ********************************************************************/
BYTE SNMPValidateCommunity(BYTE * community)
{
	BYTE i;
	BYTE *ptr;
	
	/*
	If the community name is encrypted in the request from the Manager,
	agent required to decrypt it to match with the community it is
	configured for. The response from the agent should contain encrypted community 
	name using the same encryption algorithm which Manager used while
	making the request.
	*/ 		

	// Validate that community string is a legal size
	if(strlen((char*)community) <= SNMP_COMMUNITY_MAX_LEN)
	{
		// Search to see if this is a write community.  This is done before 
		// searching read communities so that full read/write access is 
		// granted if a read and write community name happen to be the same.
		for(i = 0; i < SNMP_MAX_COMMUNITY_SUPPORT; i++)
		{
			ptr = AppConfig.writeCommunity[i];
			if(ptr == NULL)
				continue;
			if(*ptr == 0x00u)
				continue;
			if(strncmp((char*)community, (char*)ptr, SNMP_COMMUNITY_MAX_LEN) == 0)
				return WRITE_COMMUNITY;
		}
		
		// Did not find in write communities, search read communities
		for(i = 0; i < SNMP_MAX_COMMUNITY_SUPPORT; i++)
		{
			ptr = AppConfig.readCommunity[i];
			if(ptr == NULL)
				continue;
			if(*ptr == 0x00u)
				continue;
			if(strncmp((char*)community, (char*)ptr, SNMP_COMMUNITY_MAX_LEN) == 0)
				return READ_COMMUNITY;
		}
	}
#if !defined(SNMP_TRAP_DISABLED)	
	// Could not find any matching community, set up to send a trap
	gSpecificTrapNotification=VENDOR_TRAP_DEFAULT;
	gGenericTrapNotification=AUTH_FAILURE;
	gSendTrapFlag=TRUE;
#endif	
	return INVALID_COMMUNITY;
	
}

/*********************************************************************
  Function:
  	BOOL SNMPIsValidSetLen(SNMP_ID var, BYTE len,BYTE index)

  Summary: 	
	Validates the set variable data length to data type.
	
  Description:
  	This routine is used to validate the dyanmic variable data length
  	to the variable data type. It is used when SET request is processed.
  	This is a callback function called by module. User application
  	must implement this function.
  	
  PreCondition:
  	ProcessSetVar() is called.
 
  Parameters:  
  	var	-	Variable id whose value is to be set
  	len	-	Length value that is to be validated.
 	index -   instance of a OID
  Return Values:  
  	TRUE  - if given var can be set to given len
    FALSE - if otherwise.
 
  Remarks:
  	This function will be called for only dynamic variables that are
  	defined as ASCII_STRING and OCTET_STRING (i.e. data length greater
  	than 4 bytes)
 ********************************************************************/
BOOL SNMPIsValidSetLen(SNMP_ID var, BYTE len,BYTE index)
{
    switch(var)
    {
    case TRAP_COMMUNITY:
        if ( len < (BYTE)TRAP_COMMUNITY_MAX_LEN)
            return TRUE;
        break;
#ifdef STACK_USE_SNMPV3_SERVER			
	case USER_SECURITY_NAME:
		if(len <= USER_SECURITY_NAME_LEN)
		{
			//snmpV3UserDataBase[index].userNameLength = len;
			//memset(snmpV3UserDataBase[index].userName,'\0',USER_SECURITY_NAME_LEN);
			memset(gSnmpv3UserSecurityName,'\0',USER_SECURITY_NAME_LEN);
			return TRUE;
		}
		break;
	case USM_AUTH_KEY:
		if(len == AUTH_LOCALIZED_PASSWORD_KEY_LEN)
		{
			memset(snmpV3UserDataBase[index].userAuthPswdLoclizdKey,'\0',AUTH_LOCALIZED_PASSWORD_KEY_LEN);
			return TRUE;
		}
		break;
	case USM_PRIV_KEY:
		if(len == PRIV_LOCALIZED_PASSWORD_KEY_LEN)
		{
			memset(snmpV3UserDataBase[index].userPrivPswdLoclizdKey,'\0',PRIV_LOCALIZED_PASSWORD_KEY_LEN);
			return TRUE;
		}
		break;
#if defined(SNMP_V1_V2_TRAP_WITH_SNMPV3)&& !defined(SNMP_TRAP_DISABLED)
	case SNMP_TARGET_SECURITY_NAME :			// 43.6.1.4.1.17095.5.1.1.4: READWRITE ASCII_STRING.
		if(len <= USER_SECURITY_NAME_LEN)
		{
			//gSnmpv3TrapConfigData[index].userNameLength = len;
			memset(gSnmpv3UserSecurityName,'\0',USER_SECURITY_NAME_LEN);
			return TRUE;
		}
		break;
#endif		
#endif
#if defined(USE_LCD)
    case LCD_DISPLAY:
        if ( len < sizeof(LCDText)+1 )
            return TRUE;
        break;
#endif
    }
    return FALSE;
}

/*********************************************************************
  Function:  
 	BOOL SNMPSetVar(SNMP_ID var, SNMP_INDEX index,
                                   BYTE ref, SNMP_VAL val)
 
  Summary:
  	This routine Set the mib variable with the requested value.
 
  Description:
  	This is a callback function called by module for the snmp
  	SET request.User application must modify this function 
 	for the new variables address.

  Precondition:
 	ProcessVariables() is called.
 	
  Parameters:        
    var	-	Variable id whose value is to be set

    ref -   Variable reference used to transfer multi-byte data
            0 if first byte is set otherwise nonzero value to indicate
            corresponding byte being set.
            
    val -   Up to 4 byte data value.
            If var data type is BYTE, variable
               value is in val->byte
            If var data type is WORD, variable
               value is in val->word
            If var data type is DWORD, variable
               value is in val->dword.
            If var data type is IP_ADDRESS, COUNTER32,
               or GAUGE32, value is in val->dword
            If var data type is OCTET_STRING, ASCII_STRING
               value is in val->byte; multi-byte transfer
               will be performed to transfer remaining
               bytes of data.
 
  Return Values:  
  	TRUE	-	if it is OK to set more byte(s).
    FALSE	-	if otherwise.
 
  Remarks: 
  	This function may get called more than once depending on number 
	of bytes in a specific set request for given variable.
	only dynamic read-write variables needs to be handled.
********************************************************************/
BOOL SNMPSetVar(SNMP_ID var, SNMP_INDEX index, BYTE ref, SNMP_VAL val)
{
#if defined(SNMP_V1_V2_TRAP_WITH_SNMPV3) && !defined(SNMP_TRAP_DISABLED)
	BYTE tempUserNameLen = 0;
#endif 

    switch(var)
    {
    case LED_D5:
        LED2_IO = val.byte;
        return TRUE;

    case LED_D6:
        LED1_IO = val.byte;
        return TRUE;
#if !defined(SNMP_TRAP_DISABLED)
    case TRAP_RECEIVER_IP:
        // Make sure that index is within our range.
        if ( index < trapInfo.Size )
        {
            // This is just an update to an existing entry.
            trapInfo.table[index].IPAddress.Val = val.dword;
            return TRUE;
        }
        else if ( index < (BYTE)TRAP_TABLE_SIZE )
        {
            // This is an addition to table.
            trapInfo.table[index].IPAddress.Val = val.dword;
            trapInfo.table[index].communityLen = 0;
            trapInfo.Size++;
            return TRUE;
        }
        break;

    case TRAP_RECEIVER_ENABLED:
        // Make sure that index is within our range.
        if ( index < trapInfo.Size )
        {
            // Value of '1' means Enabled".
            if ( val.byte == 1u )
                trapInfo.table[index].Flags.bEnabled = 1;
            // Value of '0' means "Disabled.
            else if ( val.byte == 0u )
                trapInfo.table[index].Flags.bEnabled = 0;
            else
                // This is unknown value.
                return FALSE;
            return TRUE;
        }
        // Given index is more than our current table size.
        // If it is within our range, treat it as an addition to table.
        else if ( index < (BYTE)TRAP_TABLE_SIZE )
        {
            // Treat this as an addition to table.
            trapInfo.Size++;
            trapInfo.table[index].communityLen = 0;
        }

        break;

    case TRAP_COMMUNITY:
        // Since this is a ASCII_STRING data type, SNMP will call with
        // SNMP_END_OF_VAR to indicate no more bytes.
        // Use this information to determine if we just added new row
        // or updated an existing one.
        if ( ref ==  SNMP_END_OF_VAR )
        {
            // Index equal to table size means that we have new row.
            if ( index == trapInfo.Size )
                trapInfo.Size++;

            // Length of string is one more than index.
            trapInfo.table[index].communityLen++;

            return TRUE;
        }

        // Make sure that index is within our range.
        if ( index < trapInfo.Size )
        {
            // Copy given value into local buffer.
            trapInfo.table[index].community[ref] = val.byte;
            // Keep track of length too.
            // This may not be NULL terminate string.
            trapInfo.table[index].communityLen = (BYTE)ref;
            return TRUE;
        }
        break;
#endif 
#ifdef STACK_USE_SNMPV3_SERVER	
	case USM_AUTH_PROT:
		if(index>SNMPV3_USM_MAX_USER)
			return FALSE;
		if(val.byte == hmacMD5Auth)
		{
			snmpV3UserDataBase[index].userHashType = SNMPV3_HAMC_MD5;
		}
		else if(val.byte == hmacSHAAuth)
		{
			snmpV3UserDataBase[index].userHashType = SNMPV3_HMAC_SHA1;
		}
		else if(val.byte == noAuthProtocol)
		{
			snmpV3UserDataBase[index].userHashType = SNMPV3_NO_HMAC_AUTH;
		}
		else
			return FALSE;

		Snmpv3UsmSnmpEngnAuthPrivPswdLocalization(index);
		Snmpv3ComputeHMACIpadOpadForAuthLoclzedKey(index);
		
		//gSnmpV3USMDataBase[index].userAuthType = val.byte;
 		return TRUE;
		
	case USER_SECURITY_NAME:
		/* validate user security length*/
        // Since this is a ASCII_STRING data type, SNMP will call with
        // SNMP_END_OF_VAR to indicate no more bytes.
        // Use this information to determine if we just added new row
        // or updated an existing one.
        if ( ref ==  SNMP_END_OF_VAR )
        {
			//snmpV3UserDataBase[index].userName[ref] = '\0';
			// restrict the user security name "initial"
			if(strncmp((char*)gSnmpv3UserSecurityName,"initial",strlen((char*)gSnmpv3UserSecurityName))== 0)
				return FALSE;
			snmpV3UserDataBase[index].userNameLength = strlen((char*)gSnmpv3UserSecurityName);
			memset(snmpV3UserDataBase[index].userName,'\0',USER_SECURITY_NAME_LEN);
			strncpy((char*)snmpV3UserDataBase[index].userName,
				(char*)gSnmpv3UserSecurityName,strlen((char*)gSnmpv3UserSecurityName));
			snmpV3UserDataBase[index].userNameLength = strlen((char*)snmpV3UserDataBase[index].userName);
            return TRUE;
        }
        // Make sure that index is within our range.
        if ( index < SNMPV3_USM_MAX_USER )
        {
            // Copy given value into local buffer.
            gSnmpv3UserSecurityName[ref]=val.byte;
            return TRUE;
        }
		break;
	case USM_AUTH_KEY:
        if ( ref ==  SNMP_END_OF_VAR )
        {
			//snmpV3UserDataBase[index].userAuthPswdLoclizdKey[ref] = '\0';
			Snmpv3ComputeHMACIpadOpadForAuthLoclzedKey(index);
            return TRUE;
        }
        // Make sure that index is within our range.
        if ( index < SNMPV3_USM_MAX_USER )
        {
            // Copy given value into local buffer.
            snmpV3UserDataBase[index].userAuthPswdLoclizdKey[ref]=val.byte;
            return TRUE;
        }
		break;
	case USM_PRIV_PROT:
		if(index>SNMPV3_USM_MAX_USER)
			return SNMP_WRONG_VALUE;
		if((snmpV3UserDataBase[index].userHashType == SNMPV3_NO_HMAC_AUTH) 
			&& (val.byte != noPrivProtocol))
		{
			return SNMP_WRONG_VALUE;
		}
		if(val.byte == aesPrivProtocol)
		{
			snmpV3UserDataBase[index].userPrivType = SNMPV3_AES_PRIV;
		}
		else if(val.byte == desPrivProtocol)
		{
			snmpV3UserDataBase[index].userPrivType = SNMPV3_DES_PRIV;
		}
		else if(val.byte == noPrivProtocol)
		{
			snmpV3UserDataBase[index].userPrivType = SNMPV3_NO_PRIV;
		}
		else
			return FALSE;
		//gSnmpV3USMDataBase[index].userPrivType = val.byte;
		return TRUE;
		
	case USM_PRIV_KEY:
        if ( ref ==  SNMP_END_OF_VAR )
        {
			//snmpV3UserDataBase[index].userPrivPswdLoclizdKey[ref] = '\0';
            return TRUE;
        }
        // Make sure that index is within our range.
        if ( index < SNMPV3_USM_MAX_USER )
        {
            // Copy given value into local buffer.
            snmpV3UserDataBase[index].userPrivPswdLoclizdKey[ref]=val.byte;
            return TRUE;
        }
		break;
#if defined(SNMP_V1_V2_TRAP_WITH_SNMPV3) && !defined(SNMP_TRAP_DISABLED)
	case SNMP_TARGET_INDEX_ID : 		// 43.6.1.4.1.17095.5.1.1.1: READONLY BYTE.
		break;
	case SNMP_TARGET_MP_MODEL : 		// 43.6.1.4.1.17095.5.1.1.2: READWRITE BYTE.
		if(index < SNMPV3_USM_MAX_USER)
		{
			gSnmpv3TrapConfigData[index].messageProcessingModelType 
				= val.byte;
			return TRUE;
		}
		break;
	case  SNMP_TARGET_SECURITY_MODEL :			// 43.6.1.4.1.17095.5.1.1.3: READWRITE BYTE.
		{
			gSnmpv3TrapConfigData[index].securityModelType
				= val.byte;
			return TRUE;
		}
		break;
	case SNMP_TARGET_SECURITY_NAME :			// 43.6.1.4.1.17095.5.1.1.4: READWRITE ASCII_STRING.
        if ( ref ==  SNMP_END_OF_VAR )
        {
        	UINT8 userIndex = 0;
			// restrict the user security name "initial"
			tempUserNameLen = strlen((char*)gSnmpv3UserSecurityName);
			if(strncmp((char*)gSnmpv3UserSecurityName,"initial",tempUserNameLen)== 0)
				return FALSE;
			// check if the target security name is the part of the user security name table,
			// if target security name is not present in that table then return FALSE.
			
			for(userIndex=0;userIndex<SNMPV3_USM_MAX_USER;userIndex++)
			{
				if(strncmp((char*)gSnmpv3UserSecurityName,(char*)snmpV3UserDataBase[userIndex].userName,tempUserNameLen)== 0)
					break;
			}
			if(userIndex == SNMPV3_USM_MAX_USER)
				return FALSE;
			
			memset(gSnmpv3TrapConfigData[index].userSecurityName,'\0',USER_SECURITY_NAME_LEN);
			strncpy((char*)gSnmpv3TrapConfigData[index].userSecurityName,
				(char*)gSnmpv3UserSecurityName,strlen((char*)gSnmpv3UserSecurityName));
            return TRUE;
        }
        // Make sure that index is within our range.
        if ( index < SNMPV3_USM_MAX_USER )
        {
            // Copy given value into local buffer.
            gSnmpv3UserSecurityName[ref]=val.byte;
            return TRUE;
        }
		break;
	case SNMP_TARGET_SECURITY_LEVEL :			// 43.6.1.4.1.17095.5.1.1.5: READWRITE BYTE.
		{
			if(Snmpv3CmprTrapSecNameAndSecLvlWithUSMDb(index,strlen((char*)gSnmpv3TrapConfigData[index].userSecurityName),
				gSnmpv3TrapConfigData[index].userSecurityName,
				(STD_BASED_SNMPV3_SECURITY_LEVEL)val.byte)!= TRUE)
				return FALSE;
			gSnmpv3TrapConfigData[index].securityLevelType
				= val.byte;
			return TRUE;
		}
		break;
#endif 		
#endif
#if defined(USE_LCD)
    case LCD_DISPLAY:
        // Copy all bytes until all bytes are transferred
        if ( ref != SNMP_END_OF_VAR )
        {
            LCDText[ref] = val.byte;
            LCDText[ref+1] = 0;
        }
        else
        {
			LCDUpdate();
        }

        return TRUE;
#endif

    }

    return FALSE;
}


/*********************************************************************
  Function:        
  	BOOL SNMPGetExactIndex(SNMP_ID var,SNMP_INDEX index)

  Summary:
  	To search for exact index node in case of a Sequence variable.
	
  Description:    
  	This is a callback function called by SNMP module.
    SNMP user must implement this function in user application and 
    provide appropriate data when called.  This function will only
    be called for OID variable of type sequence.
    
  PreCondition: 
  	None
 
  Parameters:
  	var		-	Variable id as per mib.h (input)
  	index      -	 Index of variable (input)
 
  Return Values:
  	TRUE	-	 If the exact index value exists for given variable at given
                 index.
    FALSE	-	 Otherwise.
 
  Remarks:
	  Only sequence index needs to be handled in this function.
 ********************************************************************/
BOOL SNMPGetExactIndex(SNMP_ID var, SNMP_INDEX index)
{
    
    switch(var)
    {
#if !defined(SNMP_TRAP_DISABLED)
	    case TRAP_RECEIVER_ID:
	    case TRAP_RECEIVER_ENABLED:
		case TRAP_RECEIVER_IP:
		case TRAP_COMMUNITY:
	        // There is no next possible index if table itself is empty.
	        if ( trapInfo.Size == 0u )
	            return FALSE;

	        if ( index < trapInfo.Size)
	        {
	            return TRUE;
	        }
	    break;
#endif
	#ifdef STACK_USE_SNMPV3_SERVER		
		case USM_INDEX_ID:
		case USM_AUTH_KEY:
		case USM_AUTH_PROT:
		case USER_SECURITY_NAME:
		case USM_PRIV_KEY:
		case USM_PRIV_PROT:
	        if ( index < SNMPV3_USM_MAX_USER)
	        {
	            return TRUE;
	        }
		break;
#if defined(SNMP_V1_V2_TRAP_WITH_SNMPV3) && !defined(SNMP_TRAP_DISABLED)
		case SNMP_TARGET_INDEX_ID : 		// 43.6.1.4.1.17095.5.1.1.1: READONLY BYTE.
		case SNMP_TARGET_MP_MODEL : 		// 43.6.1.4.1.17095.5.1.1.2: READWRITE BYTE.
		case  SNMP_TARGET_SECURITY_MODEL :			// 43.6.1.4.1.17095.5.1.1.3: READWRITE BYTE.
		case SNMP_TARGET_SECURITY_NAME :			// 43.6.1.4.1.17095.5.1.1.4: READWRITE ASCII_STRING.
		case SNMP_TARGET_SECURITY_LEVEL :			// 43.6.1.4.1.17095.5.1.1.5: READWRITE BYTE.
	        if ( index < SNMPV3_USM_MAX_USER)
	        {
	            return TRUE;
	        }
		break;
#endif
#endif
    }
    return FALSE;
}


/*********************************************************************
  Function:        
  	BOOL SNMPGetNextIndex(SNMP_ID var,SNMP_INDEX* index)

  Summary:
  	To search for next index node in case of a Sequence variable.
	
  Description:    
  	This is a callback function called by SNMP module.
    SNMP user must implement this function in user application and 
    provide appropriate data when called.  This function will only
    be called for OID variable of type sequence.
    
  PreCondition: 
  	None
 
  Parameters:
  	var		-	Variable id whose value is to be returned
  	index   -	Next Index of variable that should be transferred
 
  Return Values:
  	TRUE	-	 If a next index value exists for given variable at given
                 index and index parameter contains next valid index.
    FALSE	-	 Otherwise.
 
  Remarks:
	  Only sequence index needs to be handled in this function.
 ********************************************************************/
BOOL SNMPGetNextIndex(SNMP_ID var, SNMP_INDEX* index)
{
    SNMP_INDEX tempIndex;

    tempIndex = *index;

    switch(var)
    {
#if !defined(SNMP_TRAP_DISABLED)
    case TRAP_RECEIVER_ID:
	case TRAP_RECEIVER_ENABLED:
	case TRAP_RECEIVER_IP:
	case TRAP_COMMUNITY:
        // There is no next possible index if table itself is empty.
        if ( trapInfo.Size == 0u )
            return FALSE;

        // INDEX_INVALID means start with first index.
        if ( tempIndex == (BYTE)SNMP_INDEX_INVALID )
        {
            *index = 0;
            return TRUE;
        }
        else if ( tempIndex < (trapInfo.Size-1) )
        {
            *index = tempIndex+1;
            return TRUE;
        }
        break;
#endif
#ifdef STACK_USE_SNMPV3_SERVER	
	case USM_INDEX_ID:
	case USM_AUTH_KEY:
	case USM_AUTH_PROT:
	case USER_SECURITY_NAME:
	case USM_PRIV_KEY:
	case USM_PRIV_PROT:
        if ( tempIndex == (BYTE)SNMP_INDEX_INVALID )
        {
            *index = 0;
            return TRUE;
        }
        else if ( tempIndex < (SNMPV3_USM_MAX_USER-1) )
        {
            *index = tempIndex+1;
            return TRUE;
        }
        break;
#if defined(SNMP_V1_V2_TRAP_WITH_SNMPV3) && !defined(SNMP_TRAP_DISABLED)
	case SNMP_TARGET_INDEX_ID : 		// 43.6.1.4.1.17095.5.1.1.1: READONLY BYTE.
	case SNMP_TARGET_MP_MODEL : 		// 43.6.1.4.1.17095.5.1.1.2: READWRITE BYTE.
	case SNMP_TARGET_SECURITY_MODEL :			// 43.6.1.4.1.17095.5.1.1.3: READWRITE BYTE.
	case SNMP_TARGET_SECURITY_NAME :			// 43.6.1.4.1.17095.5.1.1.4: READWRITE ASCII_STRING.
	case SNMP_TARGET_SECURITY_LEVEL :			// 43.6.1.4.1.17095.5.1.1.5: READWRITE BYTE.
        if ( tempIndex == (BYTE)SNMP_INDEX_INVALID )
        {
            *index = 0;
            return TRUE;
        }
        else if ( tempIndex < (SNMPV3_USM_MAX_USER-1) )
        {
            *index = tempIndex+1;
            return TRUE;
        }
	break;
#endif	
#endif
    }
    return FALSE;
}

/*********************************************************************
  Function:
  	BOOL SNMPIdRecrdValidation(PDU_INFO * pduPtr,OID_INFO *var,BYTE * oidValuePtr,BYTE oidLen)
                                   
  Summary:
  	Used to Restrict the access dynamic and non dynamic OID string for A perticular SNMP Version.

  Description:
 	This is a callback function called by SNMP module. SNMP user must 
 	implement this function as per SNMP version. One need to add the new SNMP
 	MIB IDs hereas per SNMP version.
 	e.g - SYS_UP_TIME (250) is common for V1/V2/V3
 	ENGINE_ID - is the part of V3, So put the all the SNMPv3 var ids within 
 	Macro STACK_USE_SNMPV3_SERVER.   
 	
  PreCondition:
  	None
 
  parameters:
  	var		-	Variable rec whose record id need to be validated
  	oidValuePtr - OID Value
  	oidLen - oidValuePtr length
    
  Return Values:
  	TRUE	-	If a Var ID exists .
    	FALSE 	-	Otherwise.
 
  Remarks:
 	None.
 ********************************************************************/
BOOL SNMPIdRecrdValidation(PDU_INFO * pduPtr,OID_INFO *var,BYTE * oidValuePtr,BYTE oidLen)
{
	
	int i=0,j=0;
	int len=0;
	BOOL flag=FALSE;
	BYTE size=0;

	if(var == NULL)
		return FALSE;
	
	if(!var->nodeInfo.Flags.bIsIDPresent)
	{
		if(oidValuePtr == NULL)
			return FALSE;
		
		for(i=0; i< SNMP_MAX_NON_REC_ID_OID; i++)
		{
			if((pduPtr->snmpVersion != SNMP_V3) && 
				(gSnmpNonMibRecInfo[i].version == SNMP_V3))
				continue;
			
			size = strlen((char*)gSnmpNonMibRecInfo[i].oidstr);
			if(size == 0)
				continue;
			if( size <= oidLen)
				len = size;
			else
				continue;

			// find the first unmatching byte
			while(len--)
			{
				if(gSnmpNonMibRecInfo[i].oidstr[j] != oidValuePtr[j])
				{
					flag = FALSE;
					j=0;
					break;
				}
				else
				{
					flag = TRUE;
					j++;
				}
			}
			if(flag == TRUE)
			{
				return TRUE;
			}
		}			
		return FALSE;
	}
	switch(var->id)
	{
		case MICROCHIP:
		case SYS_UP_TIME:
		case LED_D5:
	    case LED_D6:
	    case PUSH_BUTTON:
	   	case ANALOG_POT0:
#if !defined(SNMP_TRAP_DISABLED)	   	
	    case TRAP_RECEIVER_ID:
	    case TRAP_RECEIVER_ENABLED:
	    case TRAP_RECEIVER_IP:
	    case TRAP_COMMUNITY:	
#endif
#if defined(USE_LCD)
		case LCD_DISPLAY:
#endif

		return TRUE;
	}
	
#ifdef STACK_USE_SNMPV3_SERVER
	if(pduPtr->snmpVersion == SNMP_V3)
	{
		if(!var->nodeInfo.Flags.bIsIDPresent)
			return TRUE;
		
		switch(var->id)
		{

			case ENGINE_ID:
			case ENGINE_BOOT:
			case ENGINE_TIME:
			case ENGINE_MAX_MSG:
			case USM_INDEX_ID:
			case USM_AUTH_PROT:
			case USM_PRIV_PROT:
			case USER_SECURITY_NAME:
			case USM_AUTH_KEY:
			case USM_PRIV_KEY:
#if defined(SNMP_V1_V2_TRAP_WITH_SNMPV3) && !defined(SNMP_TRAP_DISABLED)
			case SNMP_TARGET_INDEX_ID :			// 43.6.1.4.1.17095.5.1.1.1: READONLY BYTE.
			case SNMP_TARGET_MP_MODEL :			// 43.6.1.4.1.17095.5.1.1.2: READWRITE BYTE.		
			case  SNMP_TARGET_SECURITY_MODEL :			// 43.6.1.4.1.17095.5.1.1.3: READWRITE BYTE.
			case SNMP_TARGET_SECURITY_NAME : 			// 43.6.1.4.1.17095.5.1.1.4: READWRITE ASCII_STRING.
			case SNMP_TARGET_SECURITY_LEVEL :			// 43.6.1.4.1.17095.5.1.1.5: READWRITE BYTE.
		
#endif /*SNMP_V1_V2_TRAP_WITH_SNMPV3 */
			return TRUE;
		}
	}
#endif /* STACK_USE_SNMPV3_SERVER */

    return FALSE;
}

/*********************************************************************
  Function:
  	BOOL SNMPGetVar(SNMP_ID var, SNMP_INDEX index,BYTE* ref, SNMP_VAL* val)
                                   
  Summary:
  	Used to Get/collect OID variable information.

  Description:
 	This is a callback function called by SNMP module. SNMP user must 
 	implement this function in user application and provide appropriate
 	data when called.
   	
  PreCondition:
  	None
 
  parameters:
  	var		-	Variable id whose value is to be returned
    index   -	Index of variable that should be transferred
    ref     -   Variable reference used to transfer
              	multi-byte data
                It is always SNMP_START_OF_VAR when very
                first byte is requested.
                Otherwise, use this as a reference to
                keep track of multi-byte transfers.
    val     -	Pointer to up to 4 byte buffer.
                If var data type is BYTE, transfer data
                  in val->byte
                If var data type is WORD, transfer data in
                  val->word
                If var data type is DWORD, transfer data in
                  val->dword
                If var data type is IP_ADDRESS, transfer data
                  in val->v[] or val->dword
                If var data type is COUNTER32, TIME_TICKS or
                  GAUGE32, transfer data in val->dword
                If var data type is ASCII_STRING or OCTET_STRING
                  transfer data in val->byte using multi-byte
                  transfer mechanism.
 
  Return Values:
  	TRUE	-	If a value exists for given variable at given index.
    FALSE 	-	Otherwise.
 
  Remarks:
 	None.
 ********************************************************************/
BOOL SNMPGetVar(SNMP_ID var, SNMP_INDEX index, BYTE* ref, SNMP_VAL* val)
{
    BYTE myRef;
 	DWORD_VAL dwvHigh, dwvLow;
    DWORD dw;
    DWORD dw10msTicks;

    myRef = *ref;

    switch(var)
    {
    case SYS_UP_TIME:
    {
	 
	    
	    // Get all 48 bits of the internal Tick timer
	    do
	   	{
		   	dwvHigh.Val = TickGetDiv64K();
		   	dwvLow.Val = TickGet();
		} while(dwvHigh.w[0] != dwvLow.w[1]);
	    dwvHigh.Val = dwvHigh.w[1];
	    
		// Find total contribution from lower DWORD
	    dw = dwvLow.Val/(DWORD)TICK_SECOND;
	    dw10msTicks = dw*100ul;
	    dw = (dwvLow.Val - dw*(DWORD)TICK_SECOND)*100ul;		// Find fractional seconds and convert to 10ms ticks
	    dw10msTicks += (dw+((DWORD)TICK_SECOND/2ul))/(DWORD)TICK_SECOND;

		// Itteratively add in the contribution from upper WORD
		while(dwvHigh.Val >= 0x1000ul)
		{
			dw10msTicks += (0x100000000000ull*100ull+(TICK_SECOND/2ull))/TICK_SECOND;
			dwvHigh.Val -= 0x1000;
		}	
		while(dwvHigh.Val >= 0x100ul)
		{
			dw10msTicks += (0x010000000000ull*100ull+(TICK_SECOND/2ull))/TICK_SECOND;
			dwvHigh.Val -= 0x100;
		}	
		while(dwvHigh.Val >= 0x10ul)
		{
			dw10msTicks += (0x001000000000ull*100ull+(TICK_SECOND/2ull))/TICK_SECOND;
			dwvHigh.Val -= 0x10;
		}	
		while(dwvHigh.Val)
		{
			dw10msTicks += (0x000100000000ull*100ull+(TICK_SECOND/2ull))/TICK_SECOND;
			dwvHigh.Val--;
		}
	    
        val->dword = dw10msTicks;
        return TRUE;
    }    

    case LED_D5:
        val->byte = LED2_IO;
        return TRUE;

    case LED_D6:
        val->byte = LED1_IO;
        return TRUE;

    case PUSH_BUTTON:
        // There is only one button - meaning only index of 0 is allowed.
        val->byte = BUTTON0_IO;
        return TRUE;

    case ANALOG_POT0:
        val->word = atoi((char*)AN0String);
        return TRUE;
#if !defined(SNMP_TRAP_DISABLED)
    case TRAP_RECEIVER_ID:
        if ( index < trapInfo.Size )
        {
            val->byte = index;
            return TRUE;
        }
        break;

    case TRAP_RECEIVER_ENABLED:
        if ( index < trapInfo.Size )
        {
            val->byte = trapInfo.table[index].Flags.bEnabled;
            return TRUE;
        }
        break;

    case TRAP_RECEIVER_IP:
        if ( index < trapInfo.Size )
        {
            val->dword = trapInfo.table[index].IPAddress.Val;
            return TRUE;
        }
        break;

    case TRAP_COMMUNITY:
        if ( index < trapInfo.Size )
        {
            if ( trapInfo.table[index].communityLen == 0u )
                *ref = SNMP_END_OF_VAR;
            else
            {
                val->byte = trapInfo.table[index].community[myRef];

                myRef++;

                if ( myRef == trapInfo.table[index].communityLen )
                    *ref = SNMP_END_OF_VAR;
                else
                    *ref = myRef;
            }
            return TRUE;
        }
        break;
#endif
#ifdef STACK_USE_SNMPV3_SERVER	
	case ENGINE_ID:
		if(snmpEngnIDLength == 0u)
			*ref = SNMP_END_OF_VAR;
		else
		{
			val->byte = snmpEngineID[myRef];
			
			myRef++;
			
			if ( myRef == snmpEngnIDLength )
				*ref = SNMP_END_OF_VAR;
			else
				*ref = myRef;
		}
		return TRUE;
	case ENGINE_BOOT:
		val->dword = (DWORD)snmpEngineBoots;
		return TRUE;
		
	case ENGINE_TIME:
		val->dword = (DWORD)snmpEngineTime.Val;
		return TRUE;
		
	case ENGINE_MAX_MSG:
		val->word = (WORD)snmpEngineMaxMessageSize.w[0];
		return TRUE;

	case USM_INDEX_ID:
        if ( index < SNMPV3_USM_MAX_USER)
        {
            val->byte = index;
            return TRUE;
        }
        break;
	case USM_AUTH_PROT:
		if(index < SNMPV3_USM_MAX_USER)
		{
			if(snmpV3UserDataBase[index].userHashType == SNMPV3_HAMC_MD5)
				val->byte = hmacMD5Auth;
			else if(snmpV3UserDataBase[index].userHashType == SNMPV3_HMAC_SHA1)
				val->byte = hmacSHAAuth;
			else
				val->byte = noAuthProtocol;				
		}
		else
			return FALSE;
		return TRUE;
	case USM_PRIV_PROT:
		// code change is required 
		if(index < SNMPV3_USM_MAX_USER)
		{
			if(snmpV3UserDataBase[index].userPrivType == SNMPV3_AES_PRIV)
				val->byte = aesPrivProtocol;
			else if(snmpV3UserDataBase[index].userPrivType == SNMPV3_DES_PRIV)
				val->byte = desPrivProtocol;
			else
				val->byte = noPrivProtocol; 			
		}
		else
			return FALSE;
		return TRUE;
	case USER_SECURITY_NAME:
		if(index < SNMPV3_USM_MAX_USER)
		{
            if ( snmpV3UserDataBase[index].userNameLength == 0u )
                *ref = SNMP_END_OF_VAR;
            else
            {
                val->byte = snmpV3UserDataBase[index].userName[myRef];

                myRef++;

                if ( myRef == snmpV3UserDataBase[index].userNameLength )
                    *ref = SNMP_END_OF_VAR;
                else
                    *ref = myRef;
            }
            return TRUE;
		}
		break;
	case USM_AUTH_KEY:
		if(index < SNMPV3_USM_MAX_USER)
		{
            val->byte = snmpV3UserDataBase[index].userAuthPswdLoclizdKey[myRef];

            myRef++;

            if ( myRef == AUTH_LOCALIZED_PASSWORD_KEY_LEN)
                *ref = SNMP_END_OF_VAR;
            else
                *ref = myRef;
            return TRUE;
		}
		break;
	case USM_PRIV_KEY:
		// code change is required / for temp- same auth passwd string is returned.
		if(index < SNMPV3_USM_MAX_USER)
		{            
            val->byte = snmpV3UserDataBase[index].userPrivPswdLoclizdKey[myRef];

            myRef++;

            if ( myRef == PRIV_LOCALIZED_PASSWORD_KEY_LEN)
                *ref = SNMP_END_OF_VAR;
            else
                *ref = myRef;
            return TRUE;
		}
		break;
#if defined(SNMP_V1_V2_TRAP_WITH_SNMPV3) && !defined(SNMP_TRAP_DISABLED)
	case SNMP_TARGET_INDEX_ID :			// 43.6.1.4.1.17095.5.1.1.1: READONLY BYTE.
		if(index < SNMPV3_USM_MAX_USER)
		{
			val->byte = index;
			return TRUE;
		}
		break;
	case SNMP_TARGET_MP_MODEL :			// 43.6.1.4.1.17095.5.1.1.2: READWRITE BYTE.		
		if(index < SNMPV3_USM_MAX_USER)
		{
			val->byte = gSnmpv3TrapConfigData[index].messageProcessingModelType;
			return TRUE;
		}
		break;
	case  SNMP_TARGET_SECURITY_MODEL :			// 43.6.1.4.1.17095.5.1.1.3: READWRITE BYTE.
		if(index < SNMPV3_USM_MAX_USER)
		{
			val->byte = gSnmpv3TrapConfigData[index].securityModelType;
			return TRUE;
		}
		break;
	case SNMP_TARGET_SECURITY_NAME : 			// 43.6.1.4.1.17095.5.1.1.4: READWRITE ASCII_STRING.
		if(index < SNMPV3_USM_MAX_USER)
		{
            if ( strlen((char*)gSnmpv3TrapConfigData[index].userSecurityName) == 0u )
                *ref = SNMP_END_OF_VAR;
			else
			{
	            val->byte = gSnmpv3TrapConfigData[index].userSecurityName[myRef];

	            myRef++;

	            if ( myRef == strlen((char*)gSnmpv3TrapConfigData[index].userSecurityName) )
	                *ref = SNMP_END_OF_VAR;
	            else
	                *ref = myRef;
			}
            return TRUE;
		}
		break;
	case SNMP_TARGET_SECURITY_LEVEL :			// 43.6.1.4.1.17095.5.1.1.5: READWRITE BYTE.
		if(index < SNMPV3_USM_MAX_USER)
		{
			val->byte = gSnmpv3TrapConfigData[index].securityLevelType;
			return TRUE;
		}
		break;
#endif
#endif
#if defined(USE_LCD)
    case LCD_DISPLAY:
        if ( LCDText[0] == 0u )
            myRef = SNMP_END_OF_VAR;
        else
        {
            val->byte = LCDText[myRef++];
            if ( LCDText[myRef] == 0u )
                myRef = SNMP_END_OF_VAR;
        }

        *ref = myRef;
        return TRUE;
#endif
    }

    return FALSE;
}

#if !defined(SNMP_TRAP_DISABLED)
/*********************************************************************
  Function:
  	static DWORD SNMPGetTimeStamp(void)
                                   
  Summary:
	Obtains the current Tick value for the SNMP time stamp.

  Description:
	This function retrieves the absolute time measurements for 
	SNMP time stamp.Use TickGet and TickGetDiv64K to collect all 48bits
	of the internal Tick Timer.

  PreCondition:
  	None
 
  parameters:
  	None
 
  Return Values:
  	timeStamp - DWORD timevalue
 
  Remarks:
 	None.
 ********************************************************************/
static DWORD SNMPGetTimeStamp(void)
{

	DWORD_VAL dwvHigh, dwvLow;
    DWORD dw;
    DWORD timeStamp;
	
	//TimeStamp
	// Get all 48 bits of the internal Tick timer
    do
   	{
	   	dwvHigh.Val = TickGetDiv64K();
	   	dwvLow.Val = TickGet();
	} while(dwvHigh.w[0] != dwvLow.w[1]);
    dwvHigh.Val = dwvHigh.w[1];
    
	// Find total contribution from lower DWORD
    dw = dwvLow.Val/(DWORD)TICK_SECOND;
    timeStamp = dw*100ul;
    dw = (dwvLow.Val - dw*(DWORD)TICK_SECOND)*100ul;		// Find fractional seconds and convert to 10ms ticks
    timeStamp += (dw+((DWORD)TICK_SECOND/2ul))/(DWORD)TICK_SECOND;

	// Itteratively add in the contribution from upper WORD
	while(dwvHigh.Val >= 0x1000ul)
	{
		timeStamp += (0x100000000000ull*100ull+(TICK_SECOND/2ull))/TICK_SECOND;
		dwvHigh.Val -= 0x1000;
	}	
	while(dwvHigh.Val >= 0x100ul)
	{
		timeStamp += (0x010000000000ull*100ull+(TICK_SECOND/2ull))/TICK_SECOND;
		dwvHigh.Val -= 0x100;
	}	
	while(dwvHigh.Val >= 0x10ul)
	{
		timeStamp += (0x001000000000ull*100ull+(TICK_SECOND/2ull))/TICK_SECOND;
		dwvHigh.Val -= 0x10;
	}	
	while(dwvHigh.Val)
	{
		timeStamp += (0x000100000000ull*100ull+(TICK_SECOND/2ull))/TICK_SECOND;
		dwvHigh.Val--;
	}
    
    return timeStamp;
}
#endif /* !defined(SNMP_TRAP_DISABLED)*/
#endif	//#if defined(STACK_USE_SNMP_SERVER)
