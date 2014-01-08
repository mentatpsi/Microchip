/*****************************************************************************
 * FileName:        Controller.h
 * Dependencies:
 * Processor:       PIC18, PIC24
 * Compiler:        See Release Notes
 * Linker:          See Release Notes
 * Company:         Microchip Technology Incorporated
 *
 * Software License Agreement
 *
 * Copyright © 2010 Microchip Technology Inc.  All rights reserved.
 *
 * Microchip licenses to you the right to use, modify, copy and distribute 
 * Software only when embedded on a Microchip microcontroller or digital 
 * signal controller and used with a Microchip radio frequency transceiver, 
 * which are integrated into your product or third party product (pursuant 
 * to the terms in the accompanying license agreement).   
 *
 * You should refer to the license agreement accompanying this Software for 
 * additional information regarding your rights and obligations.
 *
 * SOFTWARE AND DOCUMENTATION ARE PROVIDED “AS IS” WITHOUT WARRANTY OF ANY 
 * KIND, EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY 
 * WARRANTY OF MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A 
 * PARTICULAR PURPOSE. IN NO EVENT SHALL MICROCHIP OR ITS LICENSORS BE 
 * LIABLE OR OBLIGATED UNDER CONTRACT, NEGLIGENCE, STRICT LIABILITY, 
 * CONTRIBUTION, BREACH OF WARRANTY, OR OTHER LEGAL EQUITABLE THEORY ANY 
 * DIRECT OR INDIRECT DAMAGES OR EXPENSES INCLUDING BUT NOT LIMITED TO 
 * ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE OR CONSEQUENTIAL DAMAGES, 
 * LOST PROFITS OR LOST DATA, COST OF PROCUREMENT OF SUBSTITUTE GOODS, 
 * TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES (INCLUDING BUT 
 * NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.
 *****************************************************************************
 *****************************************************************************/

#ifndef RF4CE_CONTROLLER_HEADER_FILE
#define RF4CE_CONTROLLER_HEADER_FILE

/*****************************************************************************
 * Section:  Includes
 *****************************************************************************/
#include "GenericTypeDefs.h"
#include "WirelessTransceivers/RF4CE.h"

/*****************************************************************************
 * Section:  RF4CE defines
 *****************************************************************************/
#define ZRC_USER_CONTROL_PRESSED        1
#define ZRC_USER_CONTROL_REPEATED       2
#define ZRC_USER_CONTROL_RELEASED       3
#define ZRC_COMMAND_DISCOVERY_REQUEST   4
#define ZRC_COMMAND_DISCOVERY_RESPONSE  5

#define RF4CE_NVM_SIGNATURE_LENGTH      10                          // length of the signature
#define RF4CE_NVM_INFO                  0x4D434850524634434500ul    // this spells out "MCHPRF4CE"

/*****************************************************************************
 * Section:  RF4CE Commands
 *****************************************************************************/
typedef enum
{
    RF4CE_CONTROLLER_TV_SELECT_CMD        = 0x00,
    RF4CE_CONTROLLER_TV_UP_CMD            = 0x01,
    RF4CE_CONTROLLER_TV_DOWN_CMD          = 0x02,
    RF4CE_CONTROLLER_TV_LEFT_CMD          = 0x03,
    RF4CE_CONTROLLER_TV_RIGHT_CMD         = 0x04,
    
    RF4CE_CONTROLLER_TV_NUMBER_0_CMD      = 0x20,
    RF4CE_CONTROLLER_TV_NUMBER_1_CMD      = 0x21,
    RF4CE_CONTROLLER_TV_NUMBER_2_CMD      = 0x22,
    RF4CE_CONTROLLER_TV_NUMBER_3_CMD      = 0x23,
    RF4CE_CONTROLLER_TV_NUMBER_4_CMD      = 0x24,
    RF4CE_CONTROLLER_TV_NUMBER_5_CMD      = 0x25,
    RF4CE_CONTROLLER_TV_NUMBER_6_CMD      = 0x26,
    RF4CE_CONTROLLER_TV_NUMBER_7_CMD      = 0x27,
    RF4CE_CONTROLLER_TV_NUMBER_8_CMD      = 0x28,
    RF4CE_CONTROLLER_TV_NUMBER_9_CMD      = 0x29,
                                    
    RF4CE_CONTROLLER_TV_VOL_UP_CMD        = 0x41,
    RF4CE_CONTROLLER_TV_VOL_DOWN_CMD      = 0x42,

    RF4CE_CONTROLLER_TV_CHANNEL_UP_CMD    = 0x30,
    RF4CE_CONTROLLER_TV_CHANNLE_DOWN_CMD  = 0x31,

    RF4CE_CONTROLLER_TV_PREV_CHANNEL_CMD  = 0x32,
    
    RF4CE_CONTROLLER_TV_POWER_CMD         = 0x40,
    RF4CE_CONTROLLER_TV_MUTE_CMD          = 0x43,

    RF4CE_CONTROLLER_TV_PLAY_CMD          = 0x44,
    RF4CE_CONTROLLER_TV_STOP_CMD          = 0x45,
    RF4CE_CONTROLLER_TV_PAUSE_CMD         = 0x46,
    RF4CE_CONTROLLER_TV_RECORD_CMD        = 0x47,
    RF4CE_CONTROLLER_TV_REWIND_CMD        = 0x48,
    RF4CE_CONTROLLER_TV_FF_CMD            = 0x49,
    RF4CE_CONTROLLER_TV_EJECT_CMD         = 0x4A,
    RF4CE_CONTROLLER_TV_FORWARD_CMD       = 0x4B,
    RF4CE_CONTROLLER_TV_BACKWARD_CMD      = 0x4C,

    RF4CE_CONTROLLER_TV_SETUP_MENU_CMD    = 0x0A,
    RF4CE_CONTROLLER_TV_ROOT_MENU_CMD     = 0x09
}RF4CE_CONTROLLER_TV_CMD;

/*****************************************************************************
 * Section:  RF4CE Run Modes
 *****************************************************************************/
typedef enum
{
    REMOTE_RF4CE_MODE   = 0xC0,     // using RF4CE 
    REMOTE_NORF_MODE    = 0xCC,     // using dummy RF4CE 
       
}  REMOTE_CONTROL_RUN_MODE;

/*****************************************************************************
 * Section:  RF4CE Active Device
 *****************************************************************************/
typedef struct
{
    BYTE  signature[RF4CE_NVM_SIGNATURE_LENGTH];    // this will contain the signature of the saved data in NVM
    BYTE  activeDevice;                             // current active device
} RF4CE_SELECTED_DEVICE;

/*****************************************************************************
 * void RF4CE_ControllerNVM_PutSelectedDevice(BYTE *pData)
 *****************************************************************************/
void RF4CE_ControllerNVM_PutSelectedDevice(BYTE *pData);

/*****************************************************************************
 * void RF4CE_ControllerNVM_GetSelectedDevice(BYTE *pData)
 *****************************************************************************/
void RF4CE_ControllerNVM_GetSelectedDevice(BYTE *pData);

/*****************************************************************************
 * void RF4CE_ControllerNVM_Clear(void)
 *****************************************************************************/
void RF4CE_ControllerNVM_Clear(void);

/*****************************************************************************
 * BYTE RF4CEControllerInit(void)
 *****************************************************************************/
BYTE RF4CEControllerInit(void);

/*****************************************************************************
 * BYTE RF4CEControllerCheckPairTable(BYTE *pFirstValidEntryIndex)
 * 
 * Overview: Checks the pair table.
 * 
 * Input: Pointer to byte variable that will contain the index
 *        number of the first valid entry. 
 * 
 * Output: returns the number of valid entries, when table is empty
 *         value returned is zero.
 * 
 *****************************************************************************/
BYTE RF4CEControllerCheckPairTable(BYTE *pFirstValidEntryIndex);

/*****************************************************************************
 * BOOL RF4CEControllerCheckPairTableEntry(BYTE index)
 * 
 * Overview: Checks if the pair table entry is valid.
 * 
 * Input: index number of the entry in the table. 
 * 
 * Output: returns TRUE if valid and FALSE if invalid.
 * 
 *****************************************************************************/
BOOL RF4CEControllerCheckPairTableEntry(BYTE index);

/*****************************************************************************
 * BOOL RF4CEControllerPairing(void)
 *
 * You must call this function when the controller has not been pairing to the 
 * target.  Currently, this funciton is blocking and can take up to ten seconds
 * to pair.  There is a provision for adding a call back function.  However, the
 * callback function must not be blocking.
 *
 * If the pairing is successfully completed, the function will return TRUE.
 *****************************************************************************/
BOOL RF4CEControllerPairing(void);

/*****************************************************************************
 * void RF4CEControllerSendTVCommand(RF4CE_CONTROLLER_TV_CMD cmd)
 *
 * After the controller and target have been successfully paired, the controller
 * may send commands to the target.
 *
 * This funciton is passed a tv command from the RF4CE_CONTROLLER_TV_CMD 
 * enumeration.
 *****************************************************************************/
void RF4CEControllerSendTVCommand(RF4CE_CONTROLLER_TV_CMD cmd);

/*****************************************************************************
 * BOOL RF4CEControllerSetActiveDevice(BYTE index)
 *
 * Call this function to set the active device in the pairing table or 
 * make all devices inactive. When RF4CE_SendFrame() is called, the 
 * current active device's index number will be used. "index" value must 
 * be 0 < index < maximum number pairing table entries.
 *
 *****************************************************************************/
BOOL RF4CEControllerSetActiveDevice(BYTE index);

/*****************************************************************************
 * BYTE RF4CEControllerSetActiveDevice(void)
 *
 * Call this function to get the current active device in the pairing table.
 * When value returned is 0xFF, no entry is active or list is empty.
 *
 *****************************************************************************/
BYTE RF4CEControllerGetActiveDevice(void);

/*****************************************************************************
 * extern inline void __attribute__ ((always_inline)) RF4CEControllerVolumnUp(void)
 *
 * Sends the volumn up command to the target.
 *****************************************************************************/
extern inline void __attribute__ ((always_inline)) RF4CEControllerVolumnUp(void)
{
    RF4CEControllerSendTVCommand(RF4CE_CONTROLLER_TV_VOL_UP_CMD);
}

/*****************************************************************************
 * extern inline void __attribute__ ((always_inline)) RF4CEControllerVolumnDown(void)
 *
 * Sends the volumn down command to the target.
 *****************************************************************************/
extern inline void __attribute__ ((always_inline)) RF4CEControllerVolumnDown(void)
{
    RF4CEControllerSendTVCommand(RF4CE_CONTROLLER_TV_VOL_DOWN_CMD);
}

/*****************************************************************************
 * extern inline void __attribute__ ((always_inline)) RF4CEControllerChannelUp(void)
 *
 * Sends the channel up command to the target.
 *****************************************************************************/
extern inline void __attribute__ ((always_inline)) RF4CEControllerChannelUp(void)
{
    RF4CEControllerSendTVCommand(RF4CE_CONTROLLER_TV_CHANNEL_UP_CMD);
}

/*****************************************************************************
 * extern inline void __attribute__ ((always_inline)) RF4CEControllerChannelDown(void)
 *
 * Sends the channel down command to the target.
 *****************************************************************************/
extern inline void __attribute__ ((always_inline)) RF4CEControllerChannelDown(void)
{
    RF4CEControllerSendTVCommand(RF4CE_CONTROLLER_TV_CHANNLE_DOWN_CMD);
}

/*****************************************************************************
 * extern inline void __attribute__ ((always_inline)) RF4CEControllerPower(void)
 *
 * Sends the mute command to the target.
 *****************************************************************************/
extern inline void __attribute__ ((always_inline)) RF4CEControllerPower(void)
{
    RF4CEControllerSendTVCommand(RF4CE_CONTROLLER_TV_POWER_CMD);
}

/*****************************************************************************
 * extern inline void __attribute__ ((always_inline)) RF4CEControllerMute(void)
 *
 * Sends the mute command to the target.
 *****************************************************************************/
extern inline void __attribute__ ((always_inline)) RF4CEControllerMute(void)
{
    RF4CEControllerSendTVCommand(RF4CE_CONTROLLER_TV_MUTE_CMD);
}

/*****************************************************************************
 * extern inline void __attribute__ ((always_inline)) RF4CEControllerPreviousChannel(void)
 *
 * Sends the previous channel command to the target.
 *****************************************************************************/
extern inline void __attribute__ ((always_inline)) RF4CEControllerPreviousChannel(void)
{
    RF4CEControllerSendTVCommand(RF4CE_CONTROLLER_TV_PREV_CHANNEL_CMD);
}

/*****************************************************************************
 * extern inline void __attribute__ ((always_inline)) RF4CEControllerSelect(void)
 *
 * Sends the select command to the target.
 *****************************************************************************/
extern inline void __attribute__ ((always_inline)) RF4CEControllerSelect(void)
{
    RF4CEControllerSendTVCommand(RF4CE_CONTROLLER_TV_SELECT_CMD);
}

/*****************************************************************************
 * extern inline void __attribute__ ((always_inline)) RF4CEControllerUp(void)
 *
 * Sends the up command to the target.
 *****************************************************************************/
extern inline void __attribute__ ((always_inline)) RF4CEControllerUp(void)
{
    RF4CEControllerSendTVCommand(RF4CE_CONTROLLER_TV_UP_CMD);
}

/*****************************************************************************
 * extern inline void __attribute__ ((always_inline)) RF4CEControllerDown(void)
 *
 * Sends the down command to the target.
 *****************************************************************************/
extern inline void __attribute__ ((always_inline)) RF4CEControllerDown(void)
{
    RF4CEControllerSendTVCommand(RF4CE_CONTROLLER_TV_DOWN_CMD);
}

/*****************************************************************************
 * extern inline void __attribute__ ((always_inline)) RF4CEControllerLeft(void)
 *
 * Sends the left command to the target.
 *****************************************************************************/
extern inline void __attribute__ ((always_inline)) RF4CEControllerLeft(void)
{
    RF4CEControllerSendTVCommand(RF4CE_CONTROLLER_TV_LEFT_CMD);
}

/*****************************************************************************
 * extern inline void __attribute__ ((always_inline)) RF4CEControllerRight(void)
 *
 * Sends the right command to the target.
 *****************************************************************************/
extern inline void __attribute__ ((always_inline)) RF4CEControllerRight(void)
{
    RF4CEControllerSendTVCommand(RF4CE_CONTROLLER_TV_RIGHT_CMD);
}
/*****************************************************************************
 * extern inline void __attribute__ ((always_inline)) RF4CEControllerChannelChange(int channel)
 *
 * Sends the channel commands to the target.
 ****************************************************************************/
extern inline void __attribute__ ((always_inline)) RF4CEControllerChannelChange(int channel)
{
    int div = 100;

    while(div > 0)
    {
        RF4CE_CONTROLLER_TV_CMD cmd = RF4CE_CONTROLLER_TV_NUMBER_0_CMD + (channel / div);
    
        RF4CEControllerSendTVCommand(cmd);

        channel = channel % div;
        div /= 10;
    }
}
/*****************************************************************************
 * extern inline void __attribute__ ((always_inline)) RF4CEControllerPlay(void)
 *
 * Sends the play command to the target.
 *****************************************************************************/
extern inline void __attribute__ ((always_inline)) RF4CEControllerPlay(void)
{
    RF4CEControllerSendTVCommand(RF4CE_CONTROLLER_TV_PLAY_CMD);
}
/*****************************************************************************
 * extern inline void __attribute__ ((always_inline)) RF4CEControllerStop(void)
 *
 * Sends the stop command to the target.
 *****************************************************************************/
extern inline void __attribute__ ((always_inline)) RF4CEControllerStop(void)
{
    RF4CEControllerSendTVCommand(RF4CE_CONTROLLER_TV_STOP_CMD);
}
/*****************************************************************************
 * extern inline void __attribute__ ((always_inline)) RF4CEControllerPause(void)
 *
 * Sends the pause command to the target.
 *****************************************************************************/
extern inline void __attribute__ ((always_inline)) RF4CEControllerPause(void)
{
    RF4CEControllerSendTVCommand(RF4CE_CONTROLLER_TV_PAUSE_CMD);
}
/*****************************************************************************
 * extern inline void __attribute__ ((always_inline)) RF4CEControllerRecord(void)
 *
 * Sends the record command to the target.
 *****************************************************************************/
extern inline void __attribute__ ((always_inline)) RF4CEControllerRecord(void)
{
    RF4CEControllerSendTVCommand(RF4CE_CONTROLLER_TV_RECORD_CMD);
}
/*****************************************************************************
 * extern inline void __attribute__ ((always_inline)) RF4CEControllerRewind(void)
 *
 * Sends the rewind command to the target.
 *****************************************************************************/
extern inline void __attribute__ ((always_inline)) RF4CEControllerRewind(void)
{
    RF4CEControllerSendTVCommand(RF4CE_CONTROLLER_TV_REWIND_CMD);
}
/*****************************************************************************
 * extern inline void __attribute__ ((always_inline)) RF4CEControllerFastForward(void)
 *
 * Sends the fast forward command to the target.
 *****************************************************************************/
extern inline void __attribute__ ((always_inline)) RF4CEControllerFastForward(void)
{
    RF4CEControllerSendTVCommand(RF4CE_CONTROLLER_TV_FF_CMD);
}
/*****************************************************************************
 * extern inline void __attribute__ ((always_inline)) RF4CEControllerEject(void)
 *
 * Sends the eject command to the target.
 *****************************************************************************/
extern inline void __attribute__ ((always_inline)) RF4CEControllerEject(void)
{
    RF4CEControllerSendTVCommand(RF4CE_CONTROLLER_TV_EJECT_CMD);
}
/*****************************************************************************
 * extern inline void __attribute__ ((always_inline)) RF4CEControllerForward(void)
 *
 * Sends the forward command to the target.
 *****************************************************************************/
extern inline void __attribute__ ((always_inline)) RF4CEControllerForward(void)
{
    RF4CEControllerSendTVCommand(RF4CE_CONTROLLER_TV_FORWARD_CMD);
}
/*****************************************************************************
 * extern inline void __attribute__ ((always_inline)) RF4CEControllerBackward(void)
 *
 * Sends the backward command to the target.
 *****************************************************************************/
extern inline void __attribute__ ((always_inline)) RF4CEControllerBackward(void)
{
    RF4CEControllerSendTVCommand(RF4CE_CONTROLLER_TV_BACKWARD_CMD);
}
/*****************************************************************************
 * extern inline void __attribute__ ((always_inline)) RF4CEControllerSetupMenu(void)
 *
 * Sends the backward command to the target.
 *****************************************************************************/
extern inline void __attribute__ ((always_inline)) RF4CEControllerSetupMenu(void)
{
    RF4CEControllerSendTVCommand(RF4CE_CONTROLLER_TV_SETUP_MENU_CMD);
}
/*****************************************************************************
 * extern inline void __attribute__ ((always_inline)) RF4CEControllerRootMenu(void)
 *
 * Sends the backward command to the target.
 *****************************************************************************/
extern inline void __attribute__ ((always_inline)) RF4CEControllerRootMenu(void)
{
    RF4CEControllerSendTVCommand(RF4CE_CONTROLLER_TV_ROOT_MENU_CMD);
}
#endif
