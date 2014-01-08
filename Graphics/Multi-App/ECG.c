/*$6*/



#include "MainDemo.h"

/////////////////////////////////////////////////////////////////////////////
//                            ECG Demo states
/////////////////////////////////////////////////////////////////////////////
typedef enum
{
    ECG_SAMPLE,
    ECG_MEASURE,
    ECG_DISPLAY,
    ECG_ERASE
} ECG_STATES;

/////////////////////////////////////////////////////////////////////////////

//                            MACROS
/////////////////////////////////////////////////////////////////////////////
	#define WAIT_UNTIL_FINISH(x)    	while(!x)
	
    #define GR_CLR_GRID                 LIGHTGRAY
    #define GR_CLR_BACKGROUND           BLACK
    #define GR_CLR_POINTS               BRIGHTGREEN
    #define GR_CLR_ARROWS               BRIGHTRED

    #define ECG_PULSE_WINDOW            50
    #define ECG_PULSE_ANIMATION_DELAY   500
    #define ECG_ARROW_SIZE              5
    #define ECG_DISPLAY_DELAY           1000

    // Dimensions for ECG graph area
	#define ECG_ORIGIN_X    			((GetMaxX()-262+1)/2)
	#define ECG_ORIGIN_Y    			0   

	#define ECG_PANEL_LEFT   			ECG_ORIGIN_X
	#define ECG_PANEL_RIGHT  			ECG_ORIGIN_X+262
	#define ECG_PANEL_TOP    			ECG_ORIGIN_Y
	#define ECG_PANEL_BOTTOM 			ECG_ORIGIN_Y+150
    
    #define ECG_X_TEXT          		ECG_PANEL_LEFT + 40
    #define ECG_X_DIM           		ECG_PANEL_RIGHT - 40
    #define ECG_X_NUM           		ECG_X_DIM - 50
    #define ECG_Y_TEXTLINE1     		ECG_PANEL_BOTTOM
    #define ECG_Y_TEXTLINE2     		ECG_Y_TEXTLINE1 + 25

    // Graph area borders
    #define GR_LEFT     				(ECG_PANEL_LEFT + GOL_EMBOSS_SIZE)
    #define GR_RIGHT    				(ECG_PANEL_RIGHT - GOL_EMBOSS_SIZE)
    #define GR_TOP      				(ECG_PANEL_TOP + GOL_EMBOSS_SIZE)
    #define GR_BOTTOM   				(ECG_PANEL_BOTTOM - GOL_EMBOSS_SIZE)

    // Scanning window size
    #define ECG_WINDOW_SIZE 			8

    // ECG data circular buffer size
    #define ECG_BUFFER_SIZE 			256
    
    #define ECG_DELAY					5

    /////////////////////////////////////////////////////////////////////////////
    //                            STRINGS USED
    /////////////////////////////////////////////////////////////////////////////
    extern XCHAR            			ExitStr[];
	extern XCHAR                		EcgStr[];
	extern XCHAR                		ExtStr[];
	extern XCHAR                		NoiseStr[];
	extern XCHAR                		CleanStr[];
	extern XCHAR                		LabelStr[];
	extern XCHAR                		RateStr[];
	extern XCHAR                		AmplitudeStr[];
	extern XCHAR                		BeatsStr[];
	extern XCHAR                		mVStr[];

/////////////////////////////////////////////////////////////////////////////
//                            GLOBALS
/////////////////////////////////////////////////////////////////////////////
// Buffer for conversion number into string
XCHAR                       buffer[6];

// Bitmap used
extern const IMAGE_FLASH   eheart;
extern const IMAGE_FLASH   eheart2;

// GLOBAL DEFINITIONS AND VARIABLES FOR ECG DEMO
// Array of ECG points to emulate ECG input
const SHORT                 ecgPoints[] ={
    0x0068,0x0068,0x0068,0x0068,0x0068,0x0069,0x0068,0x0068,0x0068,0x0067,0x0067,0x0066,0x0066,0x0068,0x0068,0x0068,0x0067,0x0066,0x0066,0x0066,0x0066,0x0065,0x0065,0x0065,0x0065,0x0065,0x0065,0x0064,0x0063,0x0063,0x0063,0x0064,
    0x0064,0x0065,0x0065,0x0064,0x0064,0x0064,0x0064,0x0065,0x0066,0x0066,0x0062,0x0061,0x0060,0x0059,0x0055,0x004a,0x0049,0x0047,0x0047,0x0048,0x004a,0x004f,0x0053,0x0056,0x005c,0x005d,0x0060,0x0061,0x0062,0x0064,0x0064,0x0064,
    0x0065,0x0066,0x0067,0x0068,0x0068,0x0069,0x0070,0x0070,0x0055,0x0030,0x0004,0x0028,0x005d,0x006c,0x0069,0x0069,0x0069,0x0069,0x0068,0x0067,0x0065,0x0063,0x0061,0x005e,0x005e,0x0062,0x0064,0x0066,0x0067,0x0068,0x0068,0x006a
};

// ECG data circular buffer
SHORT                       ecgBuffer[ECG_BUFFER_SIZE];

// Temporary buffer for graph demo screens
SHORT                       tempBuffer[10];

// Heart animation states
SHORT                       heartStates = 1;

// ECG scan/calculation states
ECG_STATES                  ecgStates = ECG_SAMPLE;

BYTE                        potInput = 0;
BYTE                        noise = 1;
BYTE                        label = 9;

SHORT                       ecgVerY1;
SHORT                       ecgVerY2;
SHORT                       ecgVerX1;
SHORT                       ecgVerX2;

SHORT                       ecgHorY1;
SHORT                       ecgHorY2;
SHORT                       ecgHorX1;
SHORT                       ecgHorX2;

/////////////////////////////////////////////////////////////////////////////
//                            LOCAL PROTOTYPES
/////////////////////////////////////////////////////////////////////////////
void                        ECGDrawArrow(SHORT x1, SHORT y1, SHORT x2, SHORT y2);
void                        ECGCalculation(void);
WORD                        GetPotSamples(WORD number);
void                        ECGShortToString(SHORT value, XCHAR *buffer);
SHORT                       GetNoise(void);

/************************************************************************
 Function: void UpdateECG(WORD tick)
                                                                       
 Overview: Fill the circular buffer with sample data.
  		                                             
 Input: tick - specifies the current tick time. Used to check if the 
 			   pre-defined delay between sample display is reached.
                                                                       
 Output: none
 ************************************************************************/
void UpdateECG(DWORD tick)
{
    static DWORD    prevTick = 0;
    static DWORD    prevHeartTick = 0;
    SHORT           temp;

    switch(ecgStates)
    {
        case ECG_SAMPLE:
            if((tick - prevTick) > ECG_DELAY)
            {
                if(potInput)
                {
                    if(GetPotSamples(ECG_WINDOW_SIZE))
                        GraphECG(); // redraw graph
                }
                else
                {
                    if(GetECGSamples(ECG_WINDOW_SIZE))
                        GraphECG(); // redraw graph
                }

                prevTick = tick;
            }

            break;

        case ECG_MEASURE:
            ECGCalculation();
            SetColor(GR_CLR_BACKGROUND);
            WAIT_UNTIL_FINISH(Bar(ECG_X_NUM, ECG_Y_TEXTLINE1 + 1, ECG_X_DIM, CtrlBtnTop() - 1));
            SetColor(GR_CLR_ARROWS);
            temp = ecgHorX2 - ecgHorX1;
            if(temp)
                temp = 6000 / temp;
            SetFont((void *) &GOLFontDefault);
            ECGShortToString(temp, buffer);
            WAIT_UNTIL_FINISH(OutTextXY(ECG_X_NUM, ECG_Y_TEXTLINE1, buffer));
            ECGShortToString((ecgVerY2 - ecgVerY1), buffer);
            WAIT_UNTIL_FINISH(OutTextXY(ECG_X_NUM, ECG_Y_TEXTLINE2, buffer));

            if(label < 2)
            {
                SetClip(CLIP_ENABLE);
                SetClipRgn(GR_LEFT, GR_TOP, GR_RIGHT, GR_BOTTOM);
                ECGDrawArrow
                (
                    GR_LEFT + ecgHorX1,
                    (ECG_PANEL_TOP + ECG_PANEL_BOTTOM) >> 1,
                    GR_LEFT + ecgHorX2,
                    (ECG_PANEL_TOP + ECG_PANEL_BOTTOM) >> 1
                );
                ECGDrawArrow
                (
                    ECG_ARROW_SIZE + GR_LEFT + ecgVerX1,
                    GR_TOP + ecgVerY1,
                    ECG_ARROW_SIZE + GR_LEFT + ecgVerX1,
                    GR_TOP + ecgVerY2
                );
                SetClip(CLIP_DISABLE);
            }

            ecgStates = ECG_DISPLAY;

        case ECG_DISPLAY:
            if((tick - prevTick) > ECG_DISPLAY_DELAY)
            {
                prevTick = tick;
                ecgStates = ECG_ERASE;
            }

            break;

        case ECG_ERASE:
            if(label < 2)
            {
                SetColor(GR_CLR_BACKGROUND);
                SetClip(CLIP_ENABLE);
                SetClipRgn(GR_LEFT, GR_TOP, GR_RIGHT, GR_BOTTOM);
                ECGDrawArrow
                (
                    GR_LEFT + ecgHorX1,
                    (ECG_PANEL_TOP + ECG_PANEL_BOTTOM) >> 1,
                    GR_LEFT + ecgHorX2,
                    (ECG_PANEL_TOP + ECG_PANEL_BOTTOM) >> 1
                );
                ECGDrawArrow
                (
                    ECG_ARROW_SIZE + GR_LEFT + ecgVerX1,
                    GR_TOP + ecgVerY1,
                    ECG_ARROW_SIZE + GR_LEFT + ecgVerX1,
                    GR_TOP + ecgVerY2
                );
                SetClip(CLIP_DISABLE);
                if(label != 0)
                    label++;
            }

            ecgStates = ECG_SAMPLE;
            break;
    }

    if(heartStates)
        if((tick - prevHeartTick) > ECG_PULSE_ANIMATION_DELAY)
        {
            switch(heartStates)
            {
                case 1:
                    WAIT_UNTIL_FINISH(PutImage(ECG_PANEL_LEFT, ECG_Y_TEXTLINE1 + 10, (void *) &eheart, 1));
                    heartStates++;
                    break;

                default:
                    WAIT_UNTIL_FINISH(PutImage(ECG_PANEL_LEFT, ECG_Y_TEXTLINE1 + 10, (void *) &eheart2, 1));
                    heartStates = 1;
            }

            prevHeartTick = tick;
        }
}

/************************************************************************
 Function: void CreateECG(void)
                                                                       
 Overview: Creates the ECG demo screen.
 
 Input: none
                                                                       
 Output: none
************************************************************************/
void CreateECG(void)
{
    GOLFree();  // free memory for the objects in the previous linked list and start new list
    SetColor(BLACK);
    ClearDevice();

    SetColor(GR_CLR_POINTS);
    SetFont((void *) &GOLFontDefault);
    WAIT_UNTIL_FINISH(OutTextXY(ECG_X_TEXT, ECG_Y_TEXTLINE1, RateStr));
    WAIT_UNTIL_FINISH(OutTextXY(ECG_X_TEXT, ECG_Y_TEXTLINE2, AmplitudeStr));
    WAIT_UNTIL_FINISH(OutTextXY(ECG_X_DIM, ECG_Y_TEXTLINE1, BeatsStr));
    WAIT_UNTIL_FINISH(OutTextXY(ECG_X_DIM, ECG_Y_TEXTLINE2, mVStr));

    if(potInput == 0)
        CreateCtrlButtons(ExitStr, LabelStr, CleanStr, ExtStr);
    else
        CreateCtrlButtons(ExitStr, LabelStr, CleanStr, EcgStr);
}

/************************************************************************
 Function: void GetECGSamples(WORD number)
                                                                       
 Overview: Gets ECG sample data from FLASH (can be any source) into 
 		   temporary buffer. 
 		                                                                          
 Input: number - specifies the sample number to be retrieved.
                                                                       
 Output: Returns zero if samples number in the buffer is less 
 		 than defined by parameter
************************************************************************/
WORD GetECGSamples(WORD number)
{
    static const SHORT  *pointer = ecgPoints;
    static BYTE         counter = 0;

    if(pointer == (ecgPoints + sizeof(ecgPoints) / sizeof(SHORT)))
        pointer = ecgPoints;

    tempBuffer[counter] = *pointer++;

    if(noise)
        tempBuffer[counter] += GetNoise();

    counter++;
    if(counter >= number)
    {
        counter = 0;
        return (1);
    }

    return (0);
}

/************************************************************************
 Function: void PanelECG(void)
                                                                       
 Overview: Draws the box for the ECG data.
 		                                                                          
 Input: none
                                                                       
 Output: none
************************************************************************/
WORD PanelECG(void)
{
    #define ECG_STATE_SET       0
    #define ECG_STATE_DRAW      1
    #define ECG_STATE_VERLINE   2
    #define ECG_STATE_HORLINE   3
    #define ECG_STATE_TEXT      4

    static BYTE     state = ECG_STATE_SET;
    static SHORT    pos;

    switch(state)
    {
        case ECG_STATE_SET:

            // set parameters for panel
            GOLPanelDraw
            (
                ECG_PANEL_LEFT,
                ECG_PANEL_TOP,
                ECG_PANEL_RIGHT,
                ECG_PANEL_BOTTOM,
                0,
                GR_CLR_BACKGROUND,
                altScheme->EmbossDkColor,
                altScheme->EmbossLtColor,
                NULL,
                GOL_EMBOSS_SIZE
            );

            state = ECG_STATE_DRAW;     // change state

        case ECG_STATE_DRAW:
            if(!GOLPanelDrawTsk())      // draw panel for graph
                return (0);             // drawing is not completed
            SetColor(GR_CLR_GRID);
            SetLineType(DOTTED_LINE);
            pos = GR_LEFT + ((GR_RIGHT - GR_LEFT) >> 3);
            state = ECG_STATE_VERLINE;  // change state

        case ECG_STATE_VERLINE:
            while(pos < GR_RIGHT)
            {                           // draw vertical grid lines
                if(IsDeviceBusy())
                    return (0);         // drawing is not completed
                WAIT_UNTIL_FINISH(Line(pos, GR_TOP, pos, GR_BOTTOM));
                pos += (GR_RIGHT - GR_LEFT) >> 3;
            }

            pos = GR_TOP + ((GR_BOTTOM - GR_TOP) >> 3);
            state = ECG_STATE_HORLINE;  // change state

        case ECG_STATE_HORLINE:
            while(pos < GR_BOTTOM)
            {                           // draw vertical grid lines
                if(IsDeviceBusy())
                    return (0);         // drawing is not completed
                WAIT_UNTIL_FINISH(Line(GR_LEFT, pos, GR_RIGHT, pos));
                pos += (GR_BOTTOM - GR_TOP) >> 3;
            }

            SetLineType(SOLID_LINE);

            state = ECG_STATE_SET;      // change to initial state
            return (1);                 // drawing is done
    }

    return (1);
}

/************************************************************************
 Function: void GraphECG(void)
                                                                       
 Overview: Draws the ECG data or graph.
 		     
 Input: none
 
 Output: none
************************************************************************/
void GraphECG(void)
{
    SHORT           x, y;
    static SHORT    sy = 0;
    static SHORT    tsy = 0;
    SHORT           ey;
    static SHORT    *ptr = ecgBuffer;
    static SHORT    pos = 0;
    SHORT           counter;
    SHORT           *pTemp;
    SHORT           temp;

    // remove graph
    SetColor(GR_CLR_BACKGROUND);

    pTemp = ptr;
    temp = pos;

    for(x = 0; x < ECG_WINDOW_SIZE; x++)
    {
        ey = *ptr++;
        pos++;

        if(ey > sy)
        {
            for(y = sy + GR_TOP; y < ey + GR_TOP + 1; y++)
                PutPixel(GR_LEFT + pos, y);
        }
        else
        {
            for(y = ey + GR_TOP; y < sy + GR_TOP + 1; y++)
                PutPixel(GR_LEFT + pos, y);
        }

        if(ptr == (ecgBuffer + ECG_BUFFER_SIZE))
        {
            ptr = ecgBuffer;
            pos = 0;
        }

        sy = ey;
    }

    // copy new data from temporary buffer
    ptr = pTemp;
    pos = temp;

    for(counter = 0; counter < ECG_WINDOW_SIZE; counter++)
    {
        *ptr++ = tempBuffer[counter];
        pos++;
        if(ptr == (ecgBuffer + ECG_BUFFER_SIZE))
        {
            ptr = ecgBuffer;
            pos = 0;
        }
    }

    // draw graph
    SetColor(GR_CLR_POINTS);

    ptr = pTemp;
    pos = temp;

    for(x = 0; x < ECG_WINDOW_SIZE; x++)
    {
        ey = *ptr++;
        pos++;

        if(ey > tsy)
        {
            for(y = tsy + GR_TOP; y < ey + GR_TOP + 1; y++)
                PutPixel(GR_LEFT + pos, y);
        }
        else
        {
            for(y = ey + GR_TOP; y < tsy + GR_TOP + 1; y++)
                PutPixel(GR_LEFT + pos, y);
        }

        if(ptr == (ecgBuffer + ECG_BUFFER_SIZE))
        {
            ptr = ecgBuffer;
            pos = 0;
            ecgStates = ECG_MEASURE;
        }

        tsy = ey;
    }

    // draw grid
    SetColor(LIGHTGRAY);
    SetLineType(DOTTED_LINE);
    for(x = GR_LEFT + ((GR_RIGHT - GR_LEFT) >> 3); x < GR_RIGHT; x += (GR_RIGHT - GR_LEFT) >> 3)
    {
        if((x >= GR_LEFT + temp) && (x <= GR_LEFT + ECG_WINDOW_SIZE + temp))
            WAIT_UNTIL_FINISH(Line(x, GR_TOP, x, GR_BOTTOM));
    }

    for(y = GR_TOP + ((GR_BOTTOM - GR_TOP) >> 3); y < GR_BOTTOM; y += (GR_BOTTOM - GR_TOP) >> 3)
        WAIT_UNTIL_FINISH(Line(GR_LEFT + temp, y, temp + GR_LEFT + ECG_WINDOW_SIZE, y));
    SetLineType(SOLID_LINE);
}

/************************************************************************
 Function: WORD MsgECG(WORD objMsg, OBJ_HEADER* pObj, GOL_MSG* pMsg)
                                                                       
 Overview: This function is called by GOLMsg() function when in this 
 		   particular demo each time the valid message is received for the 
  		   objects in the demo.
 		                                                                          
 Input: objMsg - translated message for the object,
        pObj - pointer to the object,
        pMsg - pointer to the original GOL message.
                                                                       
 Output: If the function returns non-zero the message will 
 		 be processed by the object directly affected by the message.
 		 Default action on the object based on the message will be 
 		 performed.
************************************************************************/
WORD MsgECG(WORD objMsg, OBJ_HEADER *pObj, GOL_MSG *pMsg)
{
    if(objMsg == BTN_MSG_RELEASED)
    {
        switch(GetObjID(pObj))
        {
            case ID_BUTTON_A:

                // do not process if user moved the touch away from the button
                if(pMsg->uiEvent == EVENT_MOVE)
                    break;
                screenState = CREATE_DEMOSELECTION;
                break;

            case ID_BUTTON_B:

                // do not process if user moved the touch away from the button
                if(pMsg->uiEvent == EVENT_MOVE)
                    break;
                if(label)
                    label = 0;
                else
                    label = 1;
                break;

            case ID_BUTTON_C:

                // do not process if user moved the touch away from the button
                if(pMsg->uiEvent == EVENT_MOVE)
                    break;
                if(noise)
                {
                    BtnSetText((BUTTON *)pObj, NoiseStr);
                    noise = 0;
                }
                else
                {
                    BtnSetText((BUTTON *)pObj, CleanStr);
                    noise = 1;
                }

                break;

            case ID_BUTTON_D:

                // do not process if user moved the touch away from the button
                if(pMsg->uiEvent == EVENT_MOVE)
                    break;
                if(potInput)
                {
                    BtnSetText((BUTTON *)pObj, ExtStr);
                    potInput = 0;
                }
                else
                {
                    BtnSetText((BUTTON *)pObj, EcgStr);
                    potInput = 1;
                }

                break;

            default:
                break;
        } // end of switch(GetObjID(pObj))

        return (1); // process by default
    } // end of if(objMsg == BTN_MSG_RELEASED)

    return (1);
}

/* */
void ECGDrawArrow(SHORT x1, SHORT y1, SHORT x2, SHORT y2)
{
    WORD    temp;

    if(y1 == y2)
    {
        if(x1 > x2)
        {
            temp = x1;
            x1 = x2;
            x2 = temp;
        }

        WAIT_UNTIL_FINISH(Line(x1, y1, x1 + ECG_ARROW_SIZE, y1 + ECG_ARROW_SIZE));
        WAIT_UNTIL_FINISH(Line(x1, y1, x1 + ECG_ARROW_SIZE, y1 - ECG_ARROW_SIZE));
        WAIT_UNTIL_FINISH(Line(x2 - ECG_ARROW_SIZE, y2 - ECG_ARROW_SIZE, x2, y2));
        WAIT_UNTIL_FINISH(Line(x2 - ECG_ARROW_SIZE, y2 + ECG_ARROW_SIZE, x2, y2));
        WAIT_UNTIL_FINISH(Line(x1, y1, x2, y2));
        SetFont((void *) &FONTDEFAULT);
        ECGShortToString((x2 - x1) * 10, buffer);
        WAIT_UNTIL_FINISH
        (
            OutTextXY
                (
                    ((x1 + x2 - GetTextWidth(buffer, (void *) &FONTDEFAULT)) >> 1),
                    (y1 - GetTextHeight((void *) &FONTDEFAULT) - ECG_ARROW_SIZE),
                    buffer
                )
        );
    }
    else
    {
        if(y1 > y2)
        {
            temp = y1;
            y1 = y2;
            y2 = temp;
        }

        WAIT_UNTIL_FINISH(Line(x1, y1, x2 + ECG_ARROW_SIZE, y1 + ECG_ARROW_SIZE));
        WAIT_UNTIL_FINISH(Line(x1, y1, x2 - ECG_ARROW_SIZE, y1 + ECG_ARROW_SIZE));
        WAIT_UNTIL_FINISH(Line(x1 + ECG_ARROW_SIZE, y2 - ECG_ARROW_SIZE, x2, y2));
        WAIT_UNTIL_FINISH(Line(x1 - ECG_ARROW_SIZE, y2 - ECG_ARROW_SIZE, x2, y2));
        WAIT_UNTIL_FINISH(Line(x1, y1, x2, y2));
        SetFont((void *) &FONTDEFAULT);
        ECGShortToString((y2 - y1), buffer);
        WAIT_UNTIL_FINISH(OutTextXY((x1 + ECG_ARROW_SIZE), ((y1 + y2 - GetTextHeight((void *) &FONTDEFAULT)) >> 1), buffer));
    }
}

/* */
void ECGCalculation(void)
{
    SHORT   counter;
    DWORD   threshould;
    WORD    temp;

    ecgVerY1 = 0;
    ecgVerY2 = 300;
    ecgVerX1 = 0;
    ecgVerX2 = 0;

    ecgHorY1 = 300;
    ecgHorY2 = 300;
    ecgHorX1 = 0;
    ecgHorX2 = 0;

    threshould = 0;
    for(counter = ECG_BUFFER_SIZE - 1; counter >= 0; counter--)
    {
        threshould += ecgBuffer[counter];
        if(ecgBuffer[counter] >= ecgVerY1)
        {
            ecgVerY1 = ecgBuffer[counter];
            ecgVerX1 = counter;
        }

        if(ecgBuffer[counter] <= ecgVerY2)
        {
            ecgVerY2 = ecgBuffer[counter];
            ecgVerX2 = counter;
        }
    }

    threshould /= ECG_BUFFER_SIZE;
    threshould >>= 1;

    for(counter = 0; counter < ECG_BUFFER_SIZE; counter++)
    {
        if(ecgBuffer[counter] <= threshould)
            if((counter - ecgHorX1) > ECG_PULSE_WINDOW)
            {
                ecgHorY2 = ecgHorY1;
                ecgHorX2 = ecgHorX1;
                ecgHorY1 = ecgBuffer[counter];
                ecgHorX1 = counter;
            }
    }

    if(ecgVerY1 > ecgVerY2)
    {
        temp = ecgVerY1;
        ecgVerY1 = ecgVerY2;
        ecgVerY2 = temp;
        temp = ecgVerX1;
        ecgVerX1 = ecgVerX2;
        ecgVerX2 = temp;
    }

    if(ecgHorX1 > ecgHorX2)
    {
        temp = ecgHorY1;
        ecgHorY1 = ecgHorY2;
        ecgHorY2 = temp;
        temp = ecgHorX1;
        ecgHorX1 = ecgHorX2;
        ecgHorX2 = temp;
    }
}

// Add sample from potentiometer ADC channel into temporary buffer

// Returns zero if samples number in the buffer is less than defined by parameter
WORD GetPotSamples(WORD number)
{
    static BYTE     counter = 0;
    volatile SHORT  temp;

    temp = ADCGetPot() >> 3;
    if(noise)
        temp += GetNoise();

    if((temp + GR_TOP) > GR_BOTTOM)
        temp = GR_BOTTOM - GR_TOP;

    tempBuffer[counter++] = temp;

    if(counter >= number)
    {
        counter = 0;
        return (1);
    }

    return (0);
}

/* */
void ECGShortToString(SHORT value, XCHAR *buffer)
{
    SHORT   result;
    BYTE    pos;

    if(value > 9999)
    {
        buffer[0] = '?';
        buffer[1] = 0;
        return;
    }

    pos = 0;

    result = value / 1000;
    value = value % 1000;
    if(result)
        buffer[pos++] = result + '0';

    result = value / 100;
    value = value % 100;
    if(pos || result)
        buffer[pos++] = result + '0';

    result = value / 10;
    if(pos || result)
        buffer[pos++] = result + '0';

    result = value % 10;
    buffer[pos++] = result + '0';
    buffer[pos++] = 0;
}

/* */
SHORT GetNoise(void)
{

    // this is the noise generator
    return (TMR3 & 0x000f);
}

/************************************************************************
 Function: void InitECGStyleScheme(GOL_SCHEME *pScheme)
                                                                       
 Overview: This function initializes the style scheme used by this
 		   demo.
 		                                                                          
 Input: pScheme - pointer to the style scheme used.
                                                                       
 Output: none
************************************************************************/
void InitECGStyleScheme(GOL_SCHEME *pScheme)
{
    pScheme->Color0 = RGB565CONVERT(0x4C, 0x8E, 0xFF);
    pScheme->Color1 = RGB565CONVERT(0xFF, 0xBB, 0x4C);
    pScheme->EmbossDkColor = RGB565CONVERT(0xD4, 0xED, 0xF7);
    pScheme->EmbossLtColor = RGB565CONVERT(0xD4, 0xED, 0xF7);
    pScheme->ColorDisabled = RGB565CONVERT(0xD4, 0xE1, 0xF7);

    pScheme->TextColor1 = BRIGHTBLUE;
    pScheme->TextColor0 = RGB565CONVERT(0xFF, 0xBB, 0x4C);
    pScheme->TextColorDisabled = RGB565CONVERT(0xB8, 0xB9, 0xBC);
}
