/*****************************************************************************
 * Microchip Graphics Library Demo Application
 * This program shows how to use the Graphics Objects Layer.
 *****************************************************************************
 * FileName:        AN1246Demo.c
 * Processor:       PIC24F, PIC24H, dsPIC, PIC32
 * Compiler:       	MPLAB C30 V3.00, MPLAB C32
 * Company:         Microchip Technology Incorporated
 *
 * Software License Agreement
 *
 * Copyright © 2008 Microchip Technology Inc.  All rights reserved.
 * Microchip licenses to you the right to use, modify, copy and distribute
 * Software only when embedded on a Microchip microcontroller or digital
 * signal controller, which is integrated into your product or third party
 * product (pursuant to the sublicense terms in the accompanying license
 * agreement).  
 *
 * You should refer to the license agreement accompanying this Software
 * for additional information regarding your rights and obligations.
 *
 * SOFTWARE AND DOCUMENTATION ARE PROVIDED “AS IS” WITHOUT WARRANTY OF ANY
 * KIND, EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY WARRANTY
 * OF MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR
 * PURPOSE. IN NO EVENT SHALL MICROCHIP OR ITS LICENSORS BE LIABLE OR
 * OBLIGATED UNDER CONTRACT, NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION,
 * BREACH OF WARRANTY, OR OTHER LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT
 * DAMAGES OR EXPENSES INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL,
 * INDIRECT, PUNITIVE OR CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA,
 * COST OF PROCUREMENT OF SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY
 * CLAIMS BY THIRD PARTIES (INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF),
 * OR OTHER SIMILAR COSTS.
 *
 * Date         Comment
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * 01/23/09	    ...
  * 03/18/11     Ported to common Graphics Application Note Demo
 *****************************************************************************/
#include "Compiler.h"
#include "GenericTypeDefs.h"
#include "HardwareProfile.h"
#include "Graphics/Graphics.h"
#include "AN1246Demo.h"

/////////////////////////////////////////////////////////////////////////////
//                            DEMO STATES
/////////////////////////////////////////////////////////////////////////////
typedef enum
{
    ENTER_SECURE_ST = 0,
    RESET_SECURE_ST,
    CLEAR_SECURE_ST,
    SHOW_SECURE_ST,
} SECURE_STATE;

/////////////////////////////////////////////////////////////////////////////
//                            LOCAL PROTOTYPES
/////////////////////////////////////////////////////////////////////////////
void            CheckCalibration(void);         // check if calibration is needed
void            initKeys(void);                 // initializes the keys
void            SetCommandKeys(TEXTENTRY *pTe); // add commands to keys
void            CreateKeyPad(void);             // creates the KeyEntry object

// XCHAR string compare
int             XcharStrCmp(XCHAR *pCmp1, XCHAR *pCmp2, int len);

// callback utility for the object
WORD            SecurityMsgCallback(WORD objMsg, OBJ_HEADER *pObj, GOL_MSG *pMsg);

SECURE_STATE    ViewState = RESET_SECURE_ST; 

/////////////////////////////////////////////////////////////////////////////
//                                  MAIN
/////////////////////////////////////////////////////////////////////////////
GOL_SCHEME      *AN1246Scheme;                  // alternative style scheme
TEXTENTRY       *pTe;                           // pointer to the text entry object
extern DWORD    tick, prevTick;                 // tick counter,
XCHAR           delKey[] = {'d','e','l',0};
XCHAR           spaceKey[] = {'s','p','c',0};
XCHAR           enterKey[] = {'e','n','t','e','r',0};
XCHAR           exitKey[] = {'M','a','i','n',0};
XCHAR           key1[] = {'1',0};
XCHAR           key2[] = {'2',0};
XCHAR           key3[] = {'3',0};
XCHAR           key4[] = {'4',0};
XCHAR           key5[] = {'5',0};
XCHAR           key6[] = {'6',0};
XCHAR           key7[] = {'7',0};
XCHAR           key8[] = {'8',0};
XCHAR           key9[] = {'9',0};
XCHAR           key0[] = {'0',0};
XCHAR           keystar[] = {'*',0};
XCHAR           keypound[] = {'#',0};

XCHAR           *pKeyNames[] = { key1,    key2, key3,     delKey,
                                 key4,    key5, key6,     spaceKey,
                                 key7,    key8, key9,     enterKey,
                                 keystar, key0, keypound, exitKey
                               };


const XCHAR     TESTEnterCodeText1[] = {'1','2','3',0};
const XCHAR     TESTEnterCodeText2[] = {'1','2','3',0,'1','2','3',0};
XCHAR           EnterCodeText[TEBUFFERSIZE] = {'E','n','t','e','r',' ','I','D',' ','C','o','d','e', 0};
XCHAR           ErrorCodeText[TEBUFFERSIZE] = {'I','n','v','a','l','i','d',' ','c','o','d','e',0};
XCHAR           CorrectCodeText[TEBUFFERSIZE] = {'C','o','d','e',' ','a','c','c','e','p','t','e','d',0};
XCHAR           PassWord[PASSWORD_LEN + 1] = {'3','6','5','4',0};

// this is the user entered keys buffer
XCHAR           CodeEntered[TEBUFFERSIZE] = {0};

/////////////////////////////////////////////////////////////////////////////

/*
    This demo code shows how the TextEntry widget is used. The following 
    definitions of the keys are used:
    del - this is the delete character key
    spc - this key inserts a space character
    enter - this key functions as an enter key to process the entered code
    # - this key clears the text area (or more specifically, it clears the
        buffer that receives the user inputted texts)
    * - this toggles the feature hide echo.
    
    When pressing the "enter" key it compares the current contents of the
    buffer to the predetermined password. See definition of PassWord[] array 
    above. If the contents does not match it outputs the ErrorCodeText[].
    If it matches it outputs the CorrectCodeText[]. After that it reverts to
    EnterCodeText[] and program awaits for user inputs again.
    The key at the lower right corner is a dummy key. It has not function.
*/

/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
// Function: WORD CreateAN1246(void)
// Input: None
// Output: Returns non-zero if the objects are created 
//         returns 0 when one of the objects was not created.
// Overview: Creates the AN1246 Demo screen.
/////////////////////////////////////////////////////////////////////////////
WORD CreateAN1246(void)
{
    // create alternative style scheme
    AN1246Scheme = GOLCreateScheme();

    // set the colors that will be used by the Widget
    AN1246Scheme->Color0 = RGBConvert(0x4C, 0x8E, 0xFF);
    AN1246Scheme->Color1 = RGBConvert(0xFF, 0xBB, 0x4C);
    AN1246Scheme->EmbossDkColor = RGBConvert(0x1E, 0x00, 0xE5);
    AN1246Scheme->EmbossLtColor = RGBConvert(0xA9, 0xDB, 0xEF);
    AN1246Scheme->TextColor1 = BRIGHTBLUE;
    AN1246Scheme->TextColor0 = RGBConvert(0xFF, 0xBB, 0x4C);
    AN1246Scheme->TextColorDisabled = RGBConvert(0xB8, 0xB9, 0xBC);
    AN1246Scheme->ColorDisabled = RGBConvert(0x4C, 0x8E, 0xFF);
    AN1246Scheme->CommonBkColor = RGBConvert(0x4C, 0x8E, 0xFF);

    CreateKeyPad();
    ViewState = ENTER_SECURE_ST;

    return 1;
   
}

/////////////////////////////////////////////////////////////////////////////
// Function: WORD MsgAN1246Callback(WORD objMsg, OBJ_HEADER* pObj, GOL_MSG* pMsg)
// Input: objMsg - translated message for the object,
//        pObj - pointer to the object,
//        pMsg - pointer to the non-translated, raw GOL message
// Output: if the function returns non-zero the message will be processed 
//         by default
// Overview: This is called by GOLMsgCallback() to determine if 
//           a valid message for objects in this demo needs to be processed.
/////////////////////////////////////////////////////////////////////////////
WORD MsgAN1246Callback(WORD objMsg, OBJ_HEADER *pObj, GOL_MSG *pMsg)
{
    WORD    objectID;

    objectID = GetObjID(pObj);

    switch(ViewState)
    {
        case CLEAR_SECURE_ST:
            break;

        case ENTER_SECURE_ST:

            // process key strokes only when in this state
            if(objectID == ID_TE1)
            {
                if(!SecurityMsgCallback(objMsg, pObj, pMsg))
                    return (0);
            }

            break;

        case RESET_SECURE_ST:
            break;

        case SHOW_SECURE_ST:
            break;
    }

    return (1);
}

/////////////////////////////////////////////////////////////////////////////
// Function: WORD GOLDrawCallback()
// Output: if the function returns non-zero the draw control will be passed to GOL
// Overview: it's a user defined function. GOLDraw() function calls it each
//           time when GOL objects drawing is completed. User drawing should be done here.
//           GOL will not change color, line type and clipping region settings while

//           this function returns zero.
/////////////////////////////////////////////////////////////////////////////
WORD DrawAN1246Callback(void)
{
    TEXTENTRY   *pTe = (TEXTENTRY *)GOLFindObject(ID_TE1);

    switch(ViewState)
    {
        case ENTER_SECURE_ST:
            TeSetBuffer(pTe, CodeEntered, TEBUFFERSIZE);
            return (1);

        case RESET_SECURE_ST:

            // remove the hide feature to show messages
            if(GetState(pTe, TE_ECHO_HIDE))
                ClrState(pTe, TE_ECHO_HIDE);

            ViewState = CLEAR_SECURE_ST;
            return (1);

        case CLEAR_SECURE_ST:
            if((tick - prevTick) > CHECKDELAY)
            {

                // in this case, we do not restart but rather clear
                // the disabled state, revert the buffer to the user
                // buffer and redraw the object 	
                // return to user buffer to clear the buffer
                TeSetBuffer(pTe, CodeEntered, TEBUFFERSIZE);
                TeClearBuffer(pTe);

                // set the message to enter the code
                TeSetBuffer(pTe, EnterCodeText, TEBUFFERSIZE);
                ClrState(pTe, TE_DISABLED);
                SetState(pTe, TE_DRAW);

                // set the next state
                ViewState = ENTER_SECURE_ST;
            }
            // re-initialize the keys, this randomizes the key positions
            TeDelKeyMembers(pTe);
            initKeys();
            TeCreateKeyMembers(pTe, pKeyNames);
            SetCommandKeys(pTe);

            return (1);
        default:
            break;
            
    }

    return (1);
}

/////////////////////////////////////////////////////////////////////////////
// Function: WORD SecurityMsgCallback(WORD objMsg, OBJ_HEADER* pObj, GOL_MSG* pMsg)
// Input: objMsg - translated message for the object,
//        pObj - pointer to the object,
//        pMsg - pointer to the non-translated, raw GOL message
// Output: if the function returns non-zero the message will be processed by default

// Overview: Called by GOLMsg() and will function similar to GOLMsg().
/////////////////////////////////////////////////////////////////////////////
WORD SecurityMsgCallback(WORD objMsg, OBJ_HEADER *pObj, GOL_MSG *pMsg)
{
    WORD    objectID;

    objectID = GetObjID(pObj);

    if(objectID == ID_TE1)
    {
        if(objMsg == TE_MSG_ENTER)
        {

            // 	check first if this is for the key with enter label
            if(((TEXTENTRY *)pObj)->pActiveKey->index == ENTERKEYINDEX)
            {
                if(XcharStrCmp(TeGetBuffer((TEXTENTRY *)pObj), (XCHAR *)PassWord, PASSWORD_LEN) == 1)
                {
                    // the code is correct, display the code correct message
                    TeSetBuffer((TEXTENTRY *)pObj, (XCHAR *)CorrectCodeText, TEBUFFERSIZE);
                }
                else
                {
                    // code not correct, display code incorrect message.
                    TeSetBuffer((TEXTENTRY *)pObj, (XCHAR *)ErrorCodeText, TEBUFFERSIZE);
                }
                SetState(pObj, TE_DISABLED | TE_DRAW);
                prevTick = tick;
                ViewState = RESET_SECURE_ST;
                SetState(pObj, TE_UPDATE_TEXT);
                return (1);
            }

            // 	check first if this is for the key with mode label
            if(((TEXTENTRY *)pObj)->pActiveKey->index == MODEKEYINDEX)
            {
                if(GetState(pObj, TE_ECHO_HIDE))
                {
                    ClrState(pObj, TE_ECHO_HIDE);
                }
                else
                {
                    SetState(pObj, TE_ECHO_HIDE);
                }

                SetState(pObj, TE_UPDATE_TEXT);
                return (1);
            }

            if(((TEXTENTRY *)pObj)->pActiveKey->index == CLEARKEYINDEX)
            {
                TeClearBuffer((TEXTENTRY *)pObj);
                SetState(pObj, TE_UPDATE_TEXT);
                return (1);
            }

        }
    }

    return (1);
}

/////////////////////////////////////////////////////////////////////////////
// Function: void  CreateKeyPad(void)
// Input: none
// Output: none

// Overview: Creates the KeyEntry object and initializes the keys used.
/////////////////////////////////////////////////////////////////////////////
void CreateKeyPad(void)
{

    // create the keypad interface
    // initialize the keys
    initKeys();

    // create the object
    pTe = TeCreate
        (
            ID_TE1,                 // object’s ID
            0,
            0,
            GetMaxX(),
            GetMaxY(),              // object’s dimension
            TE_DRAW,                // draw the object after creation
            4,                      // number of horizontal keys
            4,                      // number of vertical keys
            pKeyNames,              // pointer to the array of key names
            (XCHAR *)EnterCodeText, // pointer to the string that will be displayed in the object
            TEBUFFERSIZE,
            NULL,                   // pointer to the font of the diplayed text					
            AN1246Scheme
        );                          // use default style scheme

    // assign command keys
    SetCommandKeys(pTe);
}

/////////////////////////////////////////////////////////////////////////////
// Function: void  XcharStrCmp(XCHAR *pCmp1, XCHAR *pCmp2, int len)
// Input: pCmp1 - pointer to first string to compare
//        pCmp1 - pointer to second string to compare
//        len - how many characters to compare
// Output: 1 if the two strings are the same and 0 if not.

// Overview: Similar to StrCmp().
/////////////////////////////////////////////////////////////////////////////
int XcharStrCmp(XCHAR *pCmp1, XCHAR *pCmp2, int len)
{
    int counter = 0;

    while(counter < len)
    {
        if(*pCmp1++ != *pCmp2++)
            return (0);
        counter++;
    }

    return (1);
}

/////////////////////////////////////////////////////////////////////////////
// Function: void SetCommandKeys(TEXTENTRY *pTe)
// Input: pTe - pointer to the TextEntry object
// Output: none

// Overview: Sets the commands for the selected keys.
/////////////////////////////////////////////////////////////////////////////
void SetCommandKeys(TEXTENTRY *pTe)
{
    TeSetKeyCommand(pTe, DELCHKEYINDEX, TE_DELETE_COM); //assign the 6th key to be a Delete command
    TeSetKeyCommand(pTe, SPACEKEYINDEX, TE_SPACE_COM);  //assign the 5th key to be a Space command
    TeSetKeyCommand(pTe, ENTERKEYINDEX, TE_ENTER_COM);  //assign the Enter command to Enter key
    TeSetKeyCommand(pTe, MODEKEYINDEX, TE_ENTER_COM);   //assign the Enter command to Mode key
    TeSetKeyCommand(pTe, EXITKEYINDEX, TE_ENTER_COM);   //assign the Enter command to Exit key
    TeSetKeyCommand(pTe, CLEARKEYINDEX, TE_ENTER_COM);  //assign the Enter command to Clear text key
}

/////////////////////////////////////////////////////////////////////////////
// Function: void initKeys(void)
// Input: none
// Output: none
// Overview: Randomizes the key positions.
/////////////////////////////////////////////////////////////////////////////
#define MAXITEMS    10
#define MAXENTRY    (MAXITEMS - 1)

/* */
void initKeys(void)
{
    int     randomVal, item, adj;
    XCHAR   *pVarLbl[10] = {(XCHAR*)key1, (XCHAR*)key2, (XCHAR*)key3,
        (XCHAR*)key4, (XCHAR*)key5, (XCHAR*)key6,
        (XCHAR*)key7, (XCHAR*)key8, (XCHAR*)key9,
        (XCHAR*)key0 };

    adj = 0;

    #if 0

    // Use this code to initialize the keys normally
    // randomVal here is not random at all
    for(randomVal = 0; randomVal < MAXITEMS; randomVal++)
    {
        pKeyNames[randomVal + adj] = pVarLbl[randomVal];
        if((randomVal == 5) || (randomVal == 2))
            adj++;
        if(randomVal == 8)
            adj = adj + 2;
    }

    return;
    #else

    // Use this code to randomize the key numbers
    // generate seed for random generation
    srand(tick);

    // this randomizes the positions of the 0-9 keys
    for(item = 0; item < MAXITEMS; item++)
    {

        // random number generator
        randomVal = (rand() % (MAXENTRY - item + 1));

        // assign the key character to a position in the array
        pKeyNames[item + adj] = pVarLbl[randomVal];

        // adjust the list of available pointers
        while(randomVal < (MAXENTRY - item + 1))
        {
            pVarLbl[randomVal] = pVarLbl[randomVal + 1];
            randomVal++;
        }

        // This is to compensate on the position of the keys.
        // The location of the *, #, delete, space and enter characters
        // are fixed. Only the numbers are randomized.
        switch(item)
        {
            case 2:
            case 5:
                adj++;
                break;

            case 8:
                adj = adj + 2;
                break;

            default:
                break;
        }   // end of switch()	
    }       // end of for ()
    #endif
    Nop();
}


