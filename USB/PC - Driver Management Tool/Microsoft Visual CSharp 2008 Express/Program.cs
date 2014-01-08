/********************************************************************
 FileName:		Program.cs
 Dependencies:	When compiled, needs .NET framework 2.0 redistributable to run.
 Hardware:		Personal Computer running Windows 2000 or later.  32-bit and 64-bit platforms supported.
 Compiler:  	Microsoft Visual C# 2008 Express Edition (or better)
 Company:		Microchip Technology, Inc.

 Software License Agreement:

 The software supplied herewith by Microchip Technology Incorporated
 (the “Company”) for its PIC® Microcontroller is intended and
 supplied to you, the Company’s customer, for use solely and
 exclusively with Microchip PIC Microcontroller products. The
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
  Rev   Date         Description
  2.7	03/10/2010	 Initial Release.  
  2.7   03/19/2010   Added support for parameters to allow silent 
                     operation when called from installer script.
********************************************************************/


using System;
using System.Collections.Generic;
using System.Windows.Forms;
using System.Diagnostics;
using System.Runtime.InteropServices;

namespace DriverManagementTool
{
    static class Program
    {
        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        [STAThread]
        static void Main(string[] args)
        {
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);


            //Check parameters before launching forms
            //Closing application in main form constructor causes exceptions
            if (args.Length == 1)
            {
                if (args[0] != "-remove" &&
                    args[0] != "-auto")
                {
                    MessageBox.Show("Error: Invalid Argument, check parameters.", "Error");
                    return;
                }
            }
            else if(args.Length > 1)
            {
                MessageBox.Show("Error: Too many arguments, check parameters.", "Error");
                return;
            }


            //Check if we are running on a 32-bit system, or a 64-bit system.  If 32-bit, IntPtr.Size should be 4 bytes.
            if (IntPtr.Size == 4)
            {
                //Must be running on a 32-bit operating system.  Launch the MainForm32Bit form.
                if (args.Length == 0)
                {
                    //Launch default form with add/remove buttons
                    Application.Run(new MainForm32Bit());
                }
                else
                {
                    //Launch with parameters to allow automatic add/remove
                    Application.Run(new MainForm32Bit(args));
                }

            }
            else
            {
                //Must be running on a 64-bit OS.  In this case, we want to launch the MainForm64Bit form instead.  The contents and
                //purpose of this form are the same as the MainForm32Bit, except for the DLL imports.  In order to install drivers
                //on a 64-bit operating system, it is required that the application that is trying to perform the installation also
                //be a 64-bit executable.  When this project is built with the "Any CPU" option, the "Just In Time" compiler will build
                //this program as a 64-bit executable, when the user tries to launch the program.
                //However, a 64-bit executable cannot directly use a 32-bit DLL.  A 64-bit executable program must use a 64-bit DLL.  
                //Therefore, the DLL Imports need to be different between the 64-bit vs. 32-bit forms, as the DIFxAPI functions that
                //get called need to come from the proper DLL file.
                if (args.Length == 0)
                {
                    //Launch default form with add/remove buttons
                    Application.Run(new MainForm64Bit());
                }
                else
                {
                    //Launch with parameters to allow automatic add/remove
                    Application.Run(new MainForm64Bit(args));
                }
            }
        }
    }
}
