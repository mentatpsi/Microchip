/******************************************************************************

FileName:        boot_regions_pic24.c
Dependencies:    None
Processor:       PIC24
Compiler:        C30
Company:         Microchip Technology, Inc.

Software License Agreement

The software supplied herewith by Microchip Technology Incorporated
(the "Company") for its PICmicro(R) Microcontroller is intended and
supplied to you, the Company’s customer, for use solely and
exclusively on Microchip PICmicro Microcontroller products. The
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

Change History
  Rev      Description
  -----    ----------------------------------
*******************************************************************************/

// Include files
#include "custom_bin_boot.h"


// The my_boot_regions array holds the ranges that will be bootloaded.
//  The boot regions need to be page aligned.
//  If changing the boot regions, make sure to change the linker file as well.
//  Make sure the number of boot regions in my_boot_regions equals the NUM_BOOT_REGIONS constant in boot_config.h
//  The syntax for adding more regions is the following:
//      my_boot_regions[] = {{startAddress1, endAddress1}, {startAddress2, endAddress2}, ... {startAddressN, endAddressN}}
ROM BOOT_REGION my_boot_regions[] = {{0x00003000, 0x00006000}};
