/*********************************************************************
 *
 *                Thermo Sensor with SPI(TM) Interface
 *
 *********************************************************************
 * FileName:        temperature.h
 * Dependencies:    See INCLUDES section below
 * Processor:       PIC18
 * Compiler:        C18 2.30.01+
 * Company:         Microchip Technology, Inc.
 *
 * Software License Agreement
 *
 * The software supplied herewith by Microchip Technology Incorporated
 * (the “Company”) for its PICmicro® Microcontroller is intended and
 * supplied to you, the Company’s customer, for use solely and
 * exclusively on Microchip PICmicro Microcontroller products. The
 * software is owned by the Company and/or its supplier, and is
 * protected under applicable copyright laws. All rights are reserved.
 * Any use in violation of the foregoing restrictions may subject the
 * user to criminal sanctions under applicable laws, as well as to
 * civil liability for the breach of the terms and conditions of this
 * license.
 *
 * THIS SOFTWARE IS PROVIDED IN AN “AS IS” CONDITION. NO WARRANTIES,
 * WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED
 * TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. THE COMPANY SHALL NOT,
 * IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL OR
 * CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 *
 ********************************************************************/

#ifndef TEMPERATURE_H
#define TEMPERATURE_H

/** E X T E R N S ************************************************************/
extern WORD_VAL temperature;
extern char tempString[10];

/** P U B L I C  P R O T O T Y P E S *****************************************/
void InitTempSensor(void);
BOOL AcquireTemperature(void);
void UpdateCelsiusASCII(void);
#if defined(PIC18F87J50_PIM)
    void PollTempOnHPCExplorer(void);
#else
    #define PollTempOnHPCExplorer()
#endif

#endif //TEMPERATURE_H
