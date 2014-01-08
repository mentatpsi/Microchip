/*******************************************************************************
  Accelerometer Library Interface Definition

  Summary:
    This file contains the interface definition for the Accelerometer library.
    
  Description:
    This library provides a low-level abstraction of the Accelerometer device.  It 
    can be used to simplify low-level access to the device without the necessity of 
    interacting directly with the communication module's registers, thus hiding differences from 
    one serial device variant to another.
*******************************************************************************/
//DOM-IGNORE-BEGIN
/*******************************************************************************
FileName:       Accelerometer.h
Dependencies:   See includes
Processor:      PIC32MX

Complier:       Microchip MPLAB C32 v1.06 or higher
Company:        Microchip Technology Inc.

Copyright © 2008-2009 released Microchip Technology Inc.  All rights
reserved.

Microchip licenses to you the right to use, modify, copy and distribute
Software only when embedded on a Microchip microcontroller or digital signal
controller that is integrated into your product or third party product
(pursuant to the sublicense terms in the accompanying license agreement).

You should refer to the license agreement accompanying this Software for
additional information regarding your rights and obligations.

SOFTWARE AND DOCUMENTATION ARE PROVIDED “AS IS” WITHOUT WARRANTY OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF
MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE.
IN NO EVENT SHALL MICROCHIP OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER
CONTRACT, NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR
OTHER LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES
INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE OR
CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT OF
SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES
(INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.
*******************************************************************************/
//DOM-IGNORE-END

#ifndef _ACCELEROMETER_H_FILE
#define _ACCELEROMETER_H_FILE

/*******************************************************************************************************************************************
  Summary:
    Acceleration device return result.

  Description:
    Acceleration device return result
  *******************************************************************************************************************************************/
typedef enum
{
    ACCEL_INVALID,      // Acceleration routine result invalid.
    ACCEL_VALID         // Acceleration routine result valid.
}ACCEL_RESULT;

/*******************************************************************************************************************************************
  Summary:
    Accleration initialization device structure.

  Description:
    Accleration initialization device structure.
  *******************************************************************************************************************************************/
#ifndef ACCEL_INIT
typedef union
{
    struct
    {
        unsigned long sourceClock;      // System clock rate in Hertz
        unsigned long dataRate;         // data clock rate in Hertz
    };
}ACCEL_INIT;
#endif

/*******************************************************************************************************************************************
  Summary:
    Accleration data type.

  Description:
    Accleration data type.
  *******************************************************************************************************************************************/
typedef signed char ACCEL_DATA;  // Acceleration data type.

/*******************************************************************************
  Function:
    ACCEL_RESULT ACCELInitialize(ACCEL_INIT *initialization)

  Summary:
    Initializes the Accelerometer's I/O and communication interface.

  Description:
    This routine configures the I/O (i.e., chip select) and communication interface to the
    external Accelerometer.

  Precondition:
    None.

  Parameters:
    initialization    - pointer to the device specific initialization structure.
    
  Returns:
    * ACCEL_INVALID - Initialization was not complete properly
    * ACCEL_VALID - Initalization was completed

  Example:
    <code>
    ACCEL_INIT init;

    init.sourceClock = SOUCE_CLK_80MHz;
    init.dataRate    = ACCEL_DATARATE_25MHz;

    // Initialize the Accelerometer using 80MHz source clock
    if(ACCELEROMETERInit(&init) == ACCEL_INVALID)
    {
        // handle initialization error
    }
    </code>

  Remarks:
    None
  *****************************************************************************/
ACCEL_RESULT ACCELInitialize(ACCEL_INIT *initialization);

/*******************************************************************************
  Function:
    ACCEL_RESULT ACCELGetXYZAxis(ACCEL_DATA *acc_x, ACCEL_DATA *acc_y, ACCEL_DATA *acc_z)

  Summary:
    Get the 3-axis (X-Y-Z) acceleration.

  Description:
    This routine gets the 3-axis accerelation.

  Precondition:
    The accelerometer needs to be properly initialized by calling ACCELInitialize.

  Parameters:
    acc_x           - the X-axis acceration 
    acc_y           - the Y-axis acceration
    acc_z           - the Z-axis acceration
    
  Returns:
    * ACCEL_VALID - if the acceration data is valid
    * ACCEL_INVALID - if the acceration data is invalid

  Example:
    <code>
    ACCEL_DATA  acc_x, acc_y, acc_z;

    // Get the 3-axis acceration
    if(ACCELGetXYZAxis(&acc_x, &acc_y, &acc_z) == ACCEL_VALID)
    {
        // valid acceration data
    }
    </code>

  Remarks:
    None
  *****************************************************************************/
ACCEL_RESULT ACCELGetXYZAxis(ACCEL_DATA *acc_x, ACCEL_DATA *acc_y, ACCEL_DATA *acc_z);

/*******************************************************************************
  Function:
    ACCEL_RESULT ACCELGetXYAxis(ACCEL_DATA *acc_x, ACCEL_DATA *acc_y)

  Summary:
    Get the 2-axis (X-Y) acceleration.

  Description:
    This routine gets the 2-axis accerelation.

  Precondition:
    The accelerometer needs to be properly initialized by calling ACCELInitialize.

  Parameters:
    acc_x           - the X-axis acceration 
    acc_y           - the Y-axis acceration
    
  Returns:
    * ACCEL_VALID - if the acceration data is valid
    * ACCEL_INVALID - if the acceration data is invalid

  Example:
    <code>
    ACCEL_DATA  acc_x, acc_y;

    // Get the 3-axis acceration
    if(ACCELGetXYAxis(&acc_x, &acc_y) == ACCEL_VALID)
    {
        // valid acceration data
    }
    </code>

  Remarks:
    None
  *****************************************************************************/
ACCEL_RESULT ACCELGetXYAxis(ACCEL_DATA *acc_x, ACCEL_DATA *acc_y);

/*******************************************************************************
  Function:
    ACCEL_RESULT ACCELGetXAxis(ACCEL_DATA *acc_x)

  Summary:
    Get the X-axis acceleration.

  Description:
    This routine gets the X-axis accerelation.

  Precondition:
    The accelerometer needs to be properly initialized by calling ACCELInitialize.

  Parameters:
    acc_x           - the X-axis acceration 
    
  Returns:
    * ACCEL_VALID - if the acceration data is valid
    * ACCEL_INVALID - if the acceration data is invalid

  Example:
    <code>
    ACCEL_DATA  acc_x;

    // Get the X-axis acceration
    if(ACCELGetXAxis(&acc_x) == ACCEL_VALID)
    {
        // valid acceration data
    }
    </code>

  Remarks:
    None
  *****************************************************************************/
ACCEL_RESULT ACCELGetXAxis(ACCEL_DATA *acc_x);

/*******************************************************************************
  Function:
    ACCEL_RESULT ACCELGetYAxis(ACCEL_DATA *acc_y)

  Summary:
    Get the Y-axis acceleration.

  Description:
    This routine gets the Y-axis accerelation.

  Precondition:
    The accelerometer needs to be properly initialized by calling ACCELInitialize.

  Parameters:
    acc_y           - the Y-axis acceration 
    
  Returns:
    * ACCEL_VALID - if the acceration data is valid
    * ACCEL_INVALID - if the acceration data is invalid

  Example:
    <code>
    ACCEL_DATA  acc_y;

    // Get the Y-axis acceration
    if(ACCELGetYAxis(&acc_y) == ACCEL_VALID)
    {
        // valid acceration data
    }
    </code>

  Remarks:
    None
  *****************************************************************************/
ACCEL_RESULT ACCELGetYAxis(ACCEL_DATA *acc_y);


/*******************************************************************************
  Function:
    ACCEL_RESULT ACCELGetZAxis(ACCEL_DATA *acc_z)

  Summary:
    Get the Z-axis acceleration.

  Description:
    This routine gets the Z-axis accerelation.

  Precondition:
    The accelerometer needs to be properly initialized by calling ACCELInitialize.

  Parameters:
    acc_z           - the Z-axis acceration 
    
  Returns:
    * ACCEL_VALID - if the acceration data is valid
    * ACCEL_INVALID - if the acceration data is invalid

  Example:
    <code>
    ACCEL_DATA  acc_z;

    // Get the Z-axis acceration
    if(ACCELGetZAxis(&acc_z) == ACCEL_VALID)
    {
        // valid acceration data
    }
    </code>

  Remarks:
    None
  *****************************************************************************/
ACCEL_RESULT ACCELGetZAxis(ACCEL_DATA *acc_z);

/*******************************************************************************
  Function:
    ACCEL_RESULT ACCELTask(void)

  Summary:
    Handle the Accelerometer tasks.

  Description:
    This routine handles the Accelerometer tasks.

  Precondition:
    The accelerometer needs to be properly initialized by calling ACCELInitialize.

  Parameters:
    None. 
    
  Returns:
    * ACCEL_VALID - the accelerometer is being accessed
    * ACCEL_INVALID - the accelerometer is not being accessed.

  Example:
    <code>
    if(ACCELGetZAxis(&acc_z) == ACCEL_INVALID)
    {
        // do something on a shared bus
    }
    </code>

  Remarks:
    This routine can be used to determine to the communication bus is free.  It 
    can also be used for polling.
  *****************************************************************************/
ACCEL_RESULT ACCELTask(void);
#endif
