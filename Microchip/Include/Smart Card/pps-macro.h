/********************************************************************
 FileName:		pps-macro.h
 Dependencies:	See INCLUDES section
 Processor:		PIC24 Microcontrollers
 Hardware:		This demo is natively intended to be used on Exp 16 board.
 				This demo can be modified for use on other hardware platforms.
 Complier:  	Microchip C30 (for PIC24)
 Company:		Microchip Technology, Inc.

 Software License Agreement:

 The software supplied herewith by Microchip Technology Incorporated
 (the “Company”) for its PIC® Microcontroller is intended and
 supplied to you, the Company’s customer, for use solely and
 exclusively on Microchip PIC Microcontroller products. The
 software is owned by the Company and/or its supplier, and is
 protected under applicable copyright laws. All rights are reserved.
 Any use in violation of the foregoing restrictions may subject the
 user to criminal sanctions under applicable laws, as well as to
 civil liability for the breach of the terms and conditions of this
 license.

 THIS SOFTWARE IS PROVIDED IN AN “AS IS” CONDITION. NO WARRANTIES,
 WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED
 TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. THE COMPANY SHALL NOT,
 IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL OR
 CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.

********************************************************************
 File Description:

 Change History:
  Rev   Description
  ----  -----------------------------------------
  1.01   Initial release
********************************************************************/

#define PPSOut(pin,func)    (_##pin##R = func)
#define PPSIn(pin,func)     (func = pin)

/*
 * PPS Pin codes Definitions
 *
 */

#define RP0                0	//RB0
#define RP1                1	//RB1
#define RP2                2	//RD8
#define RP3                3	//RD10
#define RP4                4	//RD9
#define RP5                5	//RD15
#define RP6                6	//RB6
#define RP7                7	//RB7
#define RP8                8	//RB8
#define RP9                9	//RB9
#define RP10               10	//RF4
#define RP11               11	//RD0
#define RP12               12	//RD11
#define RP13               13	//RB2
#define RP14               14	//RB14
#define RP15               15	//RF8
#define RP16               16	//RF3
#define RP17               17	//RF5
#define RP18               18	//RB5
#define RP19               19	//RG8
#define RP20               20	//RD5
#define RP21               21	//RG6
#define RP22               22	//RD3
#define RP23               23	//RD2
#define RP24               24	//RD1
#define RP25               25	//RD4
#define RP26               26	//RG7
#define RP27               27	//RG9
#define RP28               28	//RB4
#define RP29               29	//RB15
#define RP30               30	//RF2
#define RP31               31	//RF13

//The following pin codes 32..43 are for input functions only

#define RPI32              32	//RF12
#define RPI33              33	//RE8
#define RPI34              34	//RE9
#define RPI35              35	//RA15
#define RPI36              36	//RA14
#define RPI37              37
#define RPI38              38	//RC1
#define RPI39              39	//RC2
#define RPI40              40	//RC3
#define RPI41              41	//RC4
#define RPI42              42	//RD12
#define RPI43              43	//RD14


/*
 * PPS Output Function Definitions
 *
 */

#define C1OUT_FUNC          1
#define C2OUT_FUNC          2
#define U1TX_FUNC           3
#define U1RTS_FUNC          4
#define U2TX_FUNC           5
#define U2RTS_FUNC          6
#define SDO1_FUNC           7
#define SCK1OUT_FUNC        8
#define SS1OUT_FUNC         9
#define SDO2_FUNC           10
#define SCK2OUT_FUNC        11
#define SS2OUT_FUNC         12
#define OC1_FUNC            18
#define OC2_FUNC            19
#define OC3_FUNC            20
#define OC4_FUNC            21
#define OC5_FUNC            22
#define OC6_FUNC            23
#define OC7_FUNC            24
#define OC8_FUNC            25
#define OC9_FUNC            35
#define U3TX_FUNC           28
#define U3RTS_FUNC          29
#define U4TX_FUNC           30
#define U4RTS_FUNC          31
#define SDO3_FUNC           32
#define SCK3OUT_FUNC        33
#define SS3OUT_FUNC         34


/*
 * PPS Input Function Definitions
 *
 */

#define INT1_FUNC           _INT1R
#define INT2_FUNC           _INT2R
#define INT3_FUNC           _INT3R
#define INT4_FUNC           _INT4R

#define IC1_FUNC            _IC1R
#define IC2_FUNC            _IC2R
#define IC3_FUNC            _IC3R
#define IC4_FUNC            _IC4R
#define IC5_FUNC            _IC5R
#define IC6_FUNC            _IC6R
#define IC7_FUNC            _IC7R
#define IC8_FUNC            _IC8R
#define IC9_FUNC            _IC9R

#define OCFA_FUNC           _OCFAR
#define OCFB_FUNC           _OCFBR

#define SCK1IN_FUNC         _SCK1R
#define SDI1_FUNC           _SDI1R
#define SS1IN_FUNC          _SS1R
#define SCK2IN_FUNC         _SCK2R
#define SDI2_FUNC           _SDI2R
#define SS2IN_FUNC          _SS2R
#define SCK3IN_FUNC         _SCK3R
#define SDI3_FUNC           _SDI3R
#define SS3IN_FUNC          _SS3R

#define T1CK_FUNC           _T1CKR
#define T2CK_FUNC           _T2CKR
#define T3CK_FUNC           _T3CKR
#define T4CK_FUNC           _T4CKR
#define T5CK_FUNC           _T5CKR

#define T1GATE_FUNC         _T1CKR
#define T2GATE_FUNC         _T2CKR
#define T3GATE_FUNC         _T3CKR
#define T4GATE_FUNC         _T4CKR
#define T5GATE_FUNC         _T5CKR

#define U1CTS_FUNC          _U1CTSR
#define U1RX_FUNC           _U1RXR
#define U2CTS_FUNC          _U2CTSR
#define U2RX_FUNC           _U2RXR
#define U3CTS_FUNC          _U3CTSR
#define U3RX_FUNC           _U3RXR
#define U4CTS_FUNC          _U4CTSR
#define U4RX_FUNC           _U4RXR



  
