
/********************************************************************
 FileName:      ChineseFontsfontref.h
 Dependencies:  See INCLUDES section
 Processor:     PIC24F, PIC32MX or dsPIC Microcontrollers
 Hardware:      The code is natively intended to be used on the 
                  following hardware platforms: 
                    Explorer 16 (DM240001)
                    PIC24FJ256DA210 Development Boards (DM240312)
                    MultiMedia Expansion Board (DM320005)
                  The firmware may be modified for use on other hardware 
                    platforms by editing this file (HardwareProfile.h)
 Compiler:  	Microchip C30 (for PIC24F and dsPIC), C32 (for PIC32MX)
 Company:       Microchip Technology, Inc.

 Software License Agreement:

 The software supplied herewith by Microchip Technology Incorporated
 (the �Company�) for its PIC� Microcontroller is intended and
 supplied to you, the Company�s customer, for use solely and
 exclusively on Microchip PIC Microcontroller products. The
 software is owned by the Company and/or its supplier, and is
 protected under applicable copyright laws. All rights are reserved.
 Any use in violation of the foregoing restrictions may subject the
 user to criminal sanctions under applicable laws, as well as to
 civil liability for the breach of the terms and conditions of this
 license.

 THIS SOFTWARE IS PROVIDED IN AN �AS IS� CONDITION. NO WARRANTIES,
 WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED
 TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. THE COMPANY SHALL NOT,
 IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL OR
 CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.

********************************************************************
 File Description:

 Change History:
  Rev   Date         Description
  1.0   01/10/2012   ...
********************************************************************/

BYTE _language = 0;

#include "Graphics/Graphics.h"
#if defined(__C30__) || defined (__XC16__)
    #include "InternalResourceXC16.h"
#else    
    #include "InternalResourceXC32.h"
#endif

enum
{
    _TouchScreenStr = 0,
    _ButtonStr,
    _CheckBoxStr,
    _RadioButtonStr,
    _GroupBoxStr,
    _StaticTextStr,
    _SliderStr,
    _ProgressBarStr,
    _ListBoxStr,
    _EditBoxStr,
    _MeterStr,
    _DialStr,
    _PictureStr,
    _CustomStr,
    _SignatureStr,
    _PlottingStr,
    _EcgStr,
    _HomeStr,
    _HomeLinesStr,
    _LowStr,
    _HighStr,
    _OnStr,
    _OffStr, 
    _EnableStr,
    _DisabledStr,
    _TextLeftStr,
    _TextBottomStr,
    _Rb1Str,
    _Rb2Str,
    _Rb3Str,
    _Rb4Str,
    _Rb5Str,
    _Rb6Str,
    _StaticTextLstStr,
    _LeftStr,
    _CenterStr,
    _RightStr,
    _PIC24SpeedStr,
    _QuestionStr,
    _SingleStr,
    _AlignCenterStr,
    _ListboxLstStr,
    _CallingStr,
    _HoldingStr,
    _HoldStr,
    _AccelStr,
    _DecelStr,
    _ScaleStr,
    _AnimateStr,
    _LeftArrowStr,
    _RightArrowStr,
    _UpArrowStr,
    _DownArrowStr,
    _ExitStr,
    _SetDateTimeStr,
    
    _OnBulbStr,
    _OffBulbStr,
    _SelectionStr,
    _AlignmentStr,
    _ReturnStr,
    _Meter1Str,
    _Meter2Str,
    _ValueLabelStr,
    _MoreStr,
    _BackStr,
    _Group1Str,
    _Group2Str,
    _SetTimeDateStr,
    _NormalLoadStr,
    _LightLoadStr,
    _HeavyLoadStr,
    _UseR6PotStr,
    _ScanModeStr,
    _ScrSelList1,
    _ScrSelList2,
    _ScrSelListLang,
    _Language
};

const XCHAR* cStrings[] = 
{
    cTouchScreenStr,
    cButtonStr,
    cCheckBoxStr,
    cRadioButtonStr,
    cGroupBoxStr,
    cStaticTextStr,
    cSliderStr,
    cProgressBarStr,
    cListBoxStr,
    cEditBoxStr,
    cMeterStr,
    cDialStr,
    cPictureStr,
    cCustomStr,
    cSignatureStr,
    cPlottingStr,
    cEcgStr,
    cHomeStr,
    cHomeLinesStr,
    cLowStr,
    cHighStr,
    cOnStr,
    cOffStr, 
    cEnableStr,
    cDisabledStr,
    cTextLeftStr,
    cTextBottomStr,
    cRb1Str,
    cRb2Str,
    cRb3Str,
    cRb4Str,
    cRb5Str,
    cRb6Str,
    cStaticTextLstStr,
    cLeftStr,
    cCenterStr,
    cRightStr,
    cPIC24SpeedStr,
    cQuestionStr,
    cSingleStr,
    cAlignCenterStr,
    cListboxLstStr,
    cCallingStr,
    cHoldingStr,
    cHoldStr,
    cAccelStr,
    cDecelStr,
    cScaleStr,
    cAnimateStr,
    cLeftArrowStr,
    cRightArrowStr,
    cUpArrowStr,
    cDownArrowStr,
    cExitStr,
    cSetDateTimeStr,
    
    cOnBulbStr,
    cOffBulbStr,
    cSelectionStr,
    cAlignmentStr,
    cReturnStr,
    cMeter1Str,
    cMeter2Str,
    cValueLabelStr,
    cMoreStr,
    cBackStr,
    cGroup1Str,
    cGroup2Str,
    cSetTimeDateStr,
    cNormalLoadStr,
    cLightLoadStr,
    cHeavyLoadStr,
    cUseR6PotStr,
    cScanModeStr,
    cScrSelList1,
    cScrSelList2,
    cScrSelListLang,
    cLanguage
};

const XCHAR* eStrings[] = 
{
    eTouchScreenStr,
    eButtonStr,
    eCheckBoxStr,
    eRadioButtonStr,
    eGroupBoxStr,
    eStaticTextStr,
    eSliderStr,
    eProgressBarStr,
    eListBoxStr,
    eEditBoxStr,
    eMeterStr,
    eDialStr,
    ePictureStr,
    eCustomStr,
    eSignatureStr,
    ePlottingStr,
    eEcgStr,
    eHomeStr,
    eHomeLinesStr,
    eLowStr,
    eHighStr,
    eOnStr,
    eOffStr, 
    eEnableStr,
    eDisabledStr,
    eTextLeftStr,
    eTextBottomStr,
    eRb1Str,
    eRb2Str,
    eRb3Str,
    eRb4Str,
    eRb5Str,
    eRb6Str,
    eStaticTextLstStr,
    eLeftStr,
    eCenterStr,
    eRightStr,
    ePIC24SpeedStr,
    eQuestionStr,
    eSingleStr,
    eAlignCenterStr,
    eListboxLstStr,
    eCallingStr,
    eHoldingStr,
    eHoldStr,
    eAccelStr,
    eDecelStr,
    eScaleStr,
    eAnimateStr,
    eLeftArrowStr,
    eRightArrowStr,
    eUpArrowStr,
    eDownArrowStr,
    eExitStr,
    eSetDateTimeStr,
    
    eOnBulbStr,
    eOffBulbStr,
    eSelectionStr,
    eAlignmentStr,
    eReturnStr,
    eMeter1Str,
    eMeter2Str,
    eValueLabelStr,
    eMoreStr,
    eBackStr,
    eGroup1Str,
    eGroup2Str,
    eSetTimeDateStr,
    eNormalLoadStr,
    eLightLoadStr,
    eHeavyLoadStr,
    eUseR6PotStr,
    eScanModeStr,
    eScrSelList1,
    eScrSelList2,
    eScrSelListLang,
    eLanguage
};

const XCHAR** Strings[] = {eStrings, cStrings};

#define TouchScreenStr      Strings[_language][_TouchScreenStr      ]
#define ButtonStr           Strings[_language][_ButtonStr           ]
#define CheckBoxStr         Strings[_language][_CheckBoxStr         ]
#define RadioButtonStr      Strings[_language][_RadioButtonStr      ]
#define GroupBoxStr         Strings[_language][_GroupBoxStr         ]
#define StaticTextStr       Strings[_language][_StaticTextStr       ]
#define SliderStr           Strings[_language][_SliderStr           ]
#define ProgressBarStr      Strings[_language][_ProgressBarStr      ]
#define ListBoxStr          Strings[_language][_ListBoxStr          ]
#define EditBoxStr          Strings[_language][_EditBoxStr          ]
#define MeterStr            Strings[_language][_MeterStr            ]
#define DialStr             Strings[_language][_DialStr             ]
#define PictureStr          Strings[_language][_PictureStr          ]
#define CustomStr           Strings[_language][_CustomStr           ]
#define SignatureStr        Strings[_language][_SignatureStr        ]
#define PlottingStr         Strings[_language][_PlottingStr         ]
#define EcgStr              Strings[_language][_EcgStr              ]
#define HomeStr             Strings[_language][_HomeStr             ]
#define HomeLinesStr        Strings[_language][_HomeLinesStr        ]
#define LowStr              Strings[_language][_LowStr              ]
#define HighStr             Strings[_language][_HighStr             ]
#define OnStr               Strings[_language][_OnStr               ]
#define OffStr              Strings[_language][_OffStr              ] 
#define EnableStr           Strings[_language][_EnableStr           ]
#define DisabledStr         Strings[_language][_DisabledStr         ]
#define TextLeftStr         Strings[_language][_TextLeftStr         ]
#define TextBottomStr       Strings[_language][_TextBottomStr       ]
#define Rb1Str              Strings[_language][_Rb1Str              ]
#define Rb2Str              Strings[_language][_Rb2Str              ]
#define Rb3Str              Strings[_language][_Rb3Str              ]
#define Rb4Str              Strings[_language][_Rb4Str              ]
#define Rb5Str              Strings[_language][_Rb5Str              ]
#define Rb6Str              Strings[_language][_Rb6Str              ]
#define StaticTextLstStr    Strings[_language][_StaticTextLstStr    ]
#define LeftStr             Strings[_language][_LeftStr             ]
#define CenterStr           Strings[_language][_CenterStr           ]
#define RightStr            Strings[_language][_RightStr            ]
#define PIC24SpeedStr       Strings[_language][_PIC24SpeedStr       ]
#define QuestionStr         Strings[_language][_QuestionStr         ]
#define SingleStr           Strings[_language][_SingleStr           ]
#define AlignCenterStr      Strings[_language][_AlignCenterStr      ]
#define ListboxLstStr       Strings[_language][_ListboxLstStr       ]
#define CallingStr          Strings[_language][_CallingStr          ]
#define HoldingStr          Strings[_language][_HoldingStr          ]
#define HoldStr             Strings[_language][_HoldStr             ]
#define AccelStr            Strings[_language][_AccelStr            ]
#define DecelStr            Strings[_language][_DecelStr            ]
#define ScaleStr            Strings[_language][_ScaleStr            ]
#define AnimateStr          Strings[_language][_AnimateStr          ]
#define LeftArrowStr        Strings[_language][_LeftArrowStr        ]
#define RightArrowStr       Strings[_language][_RightArrowStr       ]
#define UpArrowStr          Strings[_language][_UpArrowStr          ]
#define DownArrowStr        Strings[_language][_DownArrowStr        ]
#define ExitStr             Strings[_language][_ExitStr             ]
#define SetDateTimeStr      Strings[_language][_SetDateTimeStr      ]

#define OnBulbStr           Strings[_language][_OnBulbStr           ]
#define OffBulbStr          Strings[_language][_OffBulbStr          ]
#define SelectionStr        Strings[_language][_SelectionStr        ]
#define AlignmentStr        Strings[_language][_AlignmentStr        ]
#define ReturnStr           Strings[_language][_ReturnStr           ]
#define Meter1Str           Strings[_language][_Meter1Str           ]
#define Meter2Str           Strings[_language][_Meter2Str           ]
#define ValueLabelStr       Strings[_language][_ValueLabelStr       ]
#define MoreStr             Strings[_language][_MoreStr             ]
#define BackStr             Strings[_language][_BackStr             ]
#define Group1Str           Strings[_language][_Group1Str           ]
#define Group2Str           Strings[_language][_Group2Str           ]
#define SetTimeDateStr      Strings[_language][_SetTimeDateStr      ]
#define NormalLoadStr       Strings[_language][_NormalLoadStr       ]
#define LightLoadStr        Strings[_language][_LightLoadStr        ]
#define HeavyLoadStr        Strings[_language][_HeavyLoadStr        ]
#define UseR6PotStr         Strings[_language][_UseR6PotStr         ]
#define ScanModeStr         Strings[_language][_ScanModeStr         ]
#define ScrSelList1         Strings[_language][_ScrSelList1         ]
#define ScrSelList2         Strings[_language][_ScrSelList2         ]
#define ScrSelListLang      Strings[_language][_ScrSelListLang      ]
#define Language            Strings[_language][_Language            ]
