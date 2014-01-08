/********************************************************************
 FileName:		MainForm32Bit.cs
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
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.IO;
using System.Runtime.InteropServices;


namespace DriverManagementTool
{
    public partial class MainForm32Bit : Form
    {
        // Define the FLAGS for DIFxAPI function calls (values from difxapi.h)
        internal const UInt32 DRIVER_PACKAGE_FORCE                  = 0x00000004;
        internal const UInt32 DRIVER_PACKAGE_REPAIR                 = 0x00000001;
        internal const UInt32 DRIVER_PACKAGE_SILENT                 = 0x00000002;
        internal const UInt32 DRIVER_PACKAGE_ONLY_IF_DEVICE_PRESENT = 0x00000008;
        internal const UInt32 DRIVER_PACKAGE_LEGACY_MODE            = 0x00000010;
        internal const UInt32 DRIVER_PACKAGE_DELETE_FILES           = 0x00000020;

        //Standard definitions from winnt.h and winerror.h
        internal const UInt32 ERROR_SUCCESS                         = 0x00000000;
        internal const UInt32 ERROR_FILE_NOT_FOUND                  = 0x00000002;
        internal const UInt32 APPLICATION_ERROR_MASK                = 0x20000000;
        internal const UInt32 ERROR_SEVERITY_ERROR                  = 0xC0000000;
        internal const UInt32 ERROR_ACCESS_DENIED                   = 0x00000005;

        //DIFX specific ERROR CODES returned by DIFxAPI calls (values from difxapi.h)
        internal const UInt32 ERROR_DEPENDENT_APPLICATIONS_EXIST    = (APPLICATION_ERROR_MASK|ERROR_SEVERITY_ERROR|0x300);
        internal const UInt32 ERROR_NO_DEVICE_ID                    = (APPLICATION_ERROR_MASK|ERROR_SEVERITY_ERROR|0x301);
        internal const UInt32 ERROR_DRIVER_PACKAGE_NOT_IN_STORE     = (APPLICATION_ERROR_MASK|ERROR_SEVERITY_ERROR|0x302);
        internal const UInt32 ERROR_MISSING_FILE                    = (APPLICATION_ERROR_MASK|ERROR_SEVERITY_ERROR|0x303);
        internal const UInt32 ERROR_INVALID_CATALOG_DATA            = (APPLICATION_ERROR_MASK|ERROR_SEVERITY_ERROR|0x304);
        internal const UInt32 ERROR_NO_SUCH_DEVINST                 = 0xE000020B;
        internal const UInt32 ERROR_SIGNATURE_OSATTRIBUTE_MISMATCH  = 0xE0000244;


        internal const UInt32 ERROR_DIALOG_NOT_ACCEPTED             = 0x800B0110;
        internal const UInt32 ERROR_DIALOG_NOT_ACCEPTED2            = 0xE0000243;

        internal const UInt32 INSTALL_DRIVERS = 0x01;
        internal const UInt32 REMOVE_DRIVERS = 0x02;
        internal const UInt32 AUTO_INSTALL = 0x01;
        internal const UInt32 MANUAL_INSTALL = 0x02;
        
        ////DLL Imports.  Need these to access various C style unmanaged functions contained in their respective DLL files.
        ////--------------------------------------------------------------------------------------------------------------
        [DllImport("difxapi_x86.dll", SetLastError = true, CharSet = CharSet.Unicode)]
        internal static extern UInt32 DriverPackageInstallW(
            [MarshalAs(UnmanagedType.LPWStr)] String DriverPackageInfPath,   //LPCTSTR           Input: Path of the INF file of the driver package to install.
            UInt32 Flags,                           //DWORD             Input: bitwise OR flags for selecting options
            IntPtr pInstallerInfo,                  //PCINSTALLERINFO   Input (optional): Use NULL here when not associating driver with application.
            ref Boolean pNeedReboot);               //BOOL*             Output: boolean notification flag to let caller know if a system reboot is needed

        [DllImport("difxapi_x86.dll", SetLastError = true, CharSet = CharSet.Unicode)]
        internal static extern UInt32 DriverPackagePreinstallW(
            [MarshalAs(UnmanagedType.LPWStr)] String DriverPackageInfPath,      //LPCTSTR           Input: Path of the INF file of the driver package to install.
            UInt32 Flags);                                                      //DWORD             Input: bitwise OR flags for selecting options

        [DllImport("difxapi_x86.dll", SetLastError = true, CharSet = CharSet.Unicode)]
        internal static extern UInt32 DriverPackageGetPathW(
            [MarshalAs(UnmanagedType.LPWStr)] String DriverPackageInfPath,     //LPCTSTR        Input: Path of the INF file of the driver package to install.
            StringBuilder DestInfPath,                                                //LPTSTR         Ouptut: String to populate with the inf file path, for the inf in the driver store.
            ref UInt32 pNumOfChars);                                           //DWORD          Input/Ouput: pointer to a DWORD, the number of chars this function should return in the pDestInfPath.

        [DllImport("difxapi_x86.dll", SetLastError = true, CharSet = CharSet.Unicode)]
        internal static extern UInt32 DriverPackageUninstallW(
            [MarshalAs(UnmanagedType.LPWStr)] String DriverPackageInfPath,  //LPCTSTR           Input: Path of the INF file (in the driver store) of the driver package to remove.
            UInt32 Flags,                                                   //DWORD             Input: bitwise OR flags for selecting options
            IntPtr pInstallerInfo,                                          //PCINSTALLERINFO   Input (optional): Use NULL here when driver not associated with application.
            ref Boolean pNeedReboot);                                       //BOOL*             Output: boolean notification flag to let caller know if a system reboot is needed

        //This class is used to pass program operation info to the background worker process
        class backgroundWorkerArgs
        {
            public UInt32 mode;                 //Either Automatic (with parameters) or Manual
            public UInt32 operation;            //Installing or removing drivers
            public IList<String> resultStrings; //Strings to print to output window for user notification

            public backgroundWorkerArgs()       //default constructor with arbitrary defualt settings
            {
                mode = AUTO_INSTALL;
                operation = INSTALL_DRIVERS;
                resultStrings = new List<String>();
            }
        }
        
        public MainForm32Bit()                  //Constructor called when run without parameters
        {
            InitializeComponent();
        }

        public MainForm32Bit(string[] args)     //Constructor called when run with parameters for auto run
        {
            backgroundWorkerArgs backgroundArgs = new backgroundWorkerArgs();

            InitializeComponent();              //Initialize as normal
            this.Install_btn.Visible = false;   //Hide buttons after initialization - not needed for auto mode
            this.Remove_btn.Visible = false;
            
            if (args.Length > 1)
            {
                //Too many arguments, exit
                MessageBox.Show("Error: Too many arguments, check parameters.", "Error");
            }


            if(args[0] == "-remove")             //parameter "remove" uninstalls drivers in auto mode
            {
                OutputStatus_lstbx.Items.Clear();    //Empy the list box of messages.  Prepare for a new message to be added after the DriverPackageInstall() has finished.
                OutputStatus_lstbx.Items.Add("Starting driver removal...");

                if (backgroundWorker1.IsBusy == true)
                {
                    OutputStatus_lstbx.Items.Add("Error: thread was busy.");
                    MessageBox.Show("Error: Thread was busy.", "Error");
                }
                else
                {
                    //Runs background task in auto mode and uninstalls drivers
                    backgroundArgs.mode = AUTO_INSTALL;
                    backgroundArgs.operation = REMOVE_DRIVERS;
                    backgroundWorker1.RunWorkerAsync((backgroundWorkerArgs)backgroundArgs);

                }
            }
            else if(args[0] == "-auto")          //parameter "auto" installs drivers in auto mode
            {
                OutputStatus_lstbx.Items.Clear();    //Emtpy the list box of messages.  Prepare for a new message to be added after the DriverPackageInstall() has finished.
                //MessageBox.Show("Make sure to accept any driver signature dialogs if they occur.");
                OutputStatus_lstbx.Items.Add("Starting driver installation.  Please wait...");
                    
                if (backgroundWorker1.IsBusy == true)
                {
                    OutputStatus_lstbx.Items.Add("Error: thread was busy.");
                    MessageBox.Show("Error: Thread was busy.", "Error");
                }
                else
                {
                    //Runs background task in auto mode and installs drivers
                    backgroundArgs.mode = AUTO_INSTALL;
                    backgroundArgs.operation = INSTALL_DRIVERS;
                    backgroundWorker1.RunWorkerAsync((backgroundWorkerArgs)backgroundArgs);
                }
            }
            else
            {
                //Any other parameter is invalid, return error
                MessageBox.Show("Error: Invalid Argument, check parameters.", "Error");
            }

        }

        //Install driver button click handling function for running in manual mode
        private void Install_btn_Click(object sender, EventArgs e)
        {
            backgroundWorkerArgs backgroundArgs = new backgroundWorkerArgs();
            
            OutputStatus_lstbx.Items.Clear();    //Emtpy the list box of messages.  Prepare for a new message to be added after the DriverPackageInstall() has finished.
            //MessageBox.Show("Make sure to accept any driver signature dialogs if they occur.");
            OutputStatus_lstbx.Items.Add("Starting driver installation.  Please wait...");
            Install_btn.Enabled = false;
            Remove_btn.Enabled = false;

 
            if (backgroundWorker1.IsBusy == true)
            {
                OutputStatus_lstbx.Items.Add("Error: thread was busy.");
            }
            else
            {
                //Runs background task in manual mode and installs drivers
                backgroundArgs.mode = MANUAL_INSTALL;
                backgroundArgs.operation = INSTALL_DRIVERS; ;
                backgroundWorker1.RunWorkerAsync((backgroundWorkerArgs)backgroundArgs);
            }

        }

        //Remove driver button click handling function for running in manual mode
        private void Remove_btn_Click(object sender, EventArgs e)
        {
            backgroundWorkerArgs backgroundArgs = new backgroundWorkerArgs();
            
            OutputStatus_lstbx.Items.Clear();    //Empy the list box of messages.  Prepare for a new message to be added after the DriverPackageInstall() has finished.
            OutputStatus_lstbx.Items.Add("Starting driver removal...");
            Install_btn.Enabled = false;
            Remove_btn.Enabled = false;
            
            if (backgroundWorker1.IsBusy == true)
            {
                OutputStatus_lstbx.Items.Add("Error: thread was busy.");
            }
            else
            {
                //Runs background task in manual mode and uninstalls drivers
                backgroundArgs.mode = MANUAL_INSTALL;
                backgroundArgs.operation = REMOVE_DRIVERS; ;
                backgroundWorker1.RunWorkerAsync((backgroundWorkerArgs)backgroundArgs);
            }
        }

        private void backgroundWorker1_DoWork(object sender, DoWorkEventArgs e)
        {
            String InfPath = "";
            Boolean NeedsRebootFlag = false;
            UInt32 ReturnErrorCode;
            String ReturnErrorCodeString;
            UInt32 NumberOfCharsToGet = 0;
            backgroundWorkerArgs backgroundArgs = new backgroundWorkerArgs();

            //Arguments must be cast into backgroundWorkerArgs class and
            //copied into a new object in order to be accessed
            backgroundArgs = (backgroundWorkerArgs)e.Argument;  

            if (backgroundArgs.operation == INSTALL_DRIVERS)
            {
                try
                {
                    DirectoryInfo di = new DirectoryInfo(".\\");
                    //Look for all *.inf files in the current directory.
                    FileInfo[] files = di.GetFiles("*.inf", SearchOption.TopDirectoryOnly);
                    //Check if there were no .inf files found.  If so, set error code so status box is updated meaningfully.
                    if (files.Length == 0)
                    {
                        //OutputSatus_lstbx.Items.Add("Driver installation failed.  No .inf file(s) provided.");
                        backgroundArgs.resultStrings.Add("Driver installation failed.  No .inf files provided.");
                        if(backgroundArgs.mode == AUTO_INSTALL)
                        {
                            MessageBox.Show("Error: Driver installation failed, no .inf files provided.", "Error");
                        }
                    }
                    else
                    {
                        //For each .inf file found, install/pre-install the driver.
                        for (uint i = 0; i < files.Length; i++)
                        {
                            InfPath = files[i].FullName;
                            ReturnErrorCode = DriverPackageInstallW(InfPath, DRIVER_PACKAGE_FORCE | DRIVER_PACKAGE_LEGACY_MODE, IntPtr.Zero, ref NeedsRebootFlag);
                            if (ReturnErrorCode == ERROR_SUCCESS)
                            {
                                backgroundArgs.resultStrings.Add("Complete: Driver installation successful.");
                            }
                            else if (ReturnErrorCode == ERROR_ACCESS_DENIED)
                            {
                                MessageBox.Show("The drivers cannot be installed without administrator privileges.", "Info");
                                backgroundArgs.resultStrings.Add("Driver install failed.  Please restart this application with admin privileges.");
                            }
                            else if (ReturnErrorCode == ERROR_NO_SUCH_DEVINST)
                            {
                                backgroundArgs.resultStrings.Add("Complete: Driver was pre-installed to the driver store successfully.");
                                backgroundArgs.resultStrings.Add("Note: If the Found New Hardware Wizard appears upon plugging in");
                                backgroundArgs.resultStrings.Add("the device, allow Windows to search automatically for the driver.");
                            }
                            else if ((ReturnErrorCode == ERROR_DIALOG_NOT_ACCEPTED) || (ReturnErrorCode == ERROR_DIALOG_NOT_ACCEPTED2))
                            {
                                MessageBox.Show("The driver cannot be installed unless you accept security warning or driver signature dialogs.", "Info");
                                backgroundArgs.resultStrings.Add("The driver was not installed.  Please try again.");
                            }
                            else
                            {
                                //Convert the error code to a hexadecimal formatted string, for easier human readability.
                                ReturnErrorCodeString = String.Format("{0:X}", ReturnErrorCode);
                                backgroundArgs.resultStrings.Add("Driver installation failed.  Error code = 0x" + ReturnErrorCodeString + ".");
                                if(backgroundArgs.mode == AUTO_INSTALL)
                                {
                                    MessageBox.Show("Driver installation failed.  Error code = 0x" + ReturnErrorCodeString + ".", "Error");
                                }
                            }
                        }
                    }

                }
                catch (Exception ex)
                {
                    backgroundArgs.resultStrings.Add("Driver installation failed.  Exception: " + ex.ToString() + ".");
                    if(backgroundArgs.mode == AUTO_INSTALL)
                    {
                        MessageBox.Show("Driver installation failed.  Exception: " + ex.ToString() + ".", "Error");
                    }
                }
            }
            else if (backgroundArgs.operation == REMOVE_DRIVERS)
            {
                try
                {
                    DirectoryInfo di = new DirectoryInfo(".\\");
                    //Look for all *.inf files in the current directory, as well as all subdirectories.
                    FileInfo[] files = di.GetFiles("*.inf", SearchOption.TopDirectoryOnly);
                    //Check if there were no .inf files found.  If so, set error code so status box is updated meaningfully.
                    if (files.Length == 0)
                    {
                        backgroundArgs.resultStrings.Add("Driver removal failed.  No .inf file(s) provided.");
                        if(backgroundArgs.mode == AUTO_INSTALL)
                        {
                            MessageBox.Show("Driver removal failed.  No .inf file(s) provided.", "Error");
                        }
                    }
                    else
                    {
                        //For each .inf file found, remove the driver.
                        for (uint i = 0; i < files.Length; i++)
                        {
                            InfPath = files[i].FullName;
                            //Call DriverPackageGetPathW() function first time with num chars = 0, to get the proper/real NumberOfCharsToGet of the entire path.
                            NumberOfCharsToGet = 0;
                            //Allocate a stringbuilder to eventually receive the DriverStoreInfPath return value.  However,
                            //we don't know how large to make it yet, so it is important that the NumberOfCharsToGet is
                            //set = 0 before the first call to DriverPackageGetPathW().
                            StringBuilder DriverStoreInfPath = new StringBuilder();
                            ReturnErrorCode = DriverPackageGetPathW(InfPath, DriverStoreInfPath, ref NumberOfCharsToGet);
                            //Now resize our StringBuilder to be large enough to hold the entire driver store path.
                            DriverStoreInfPath.EnsureCapacity((int)NumberOfCharsToGet);
                            //Call the function a second time to get the actual path.  Path is returned in the DriverStoreInfPath.
                            ReturnErrorCode = DriverPackageGetPathW(InfPath, DriverStoreInfPath, ref NumberOfCharsToGet);

                            if (ReturnErrorCode == ERROR_SUCCESS)
                            {
                                //If we get to here, success, we have the path to the driver .inf file and package in the driver store (typically: C:\Windows\System32\DRVSTORE\[driver name + guid identifier])
                                //Try to remove the driver from the driver store, and from any PnP nodes currently using it.
                                NeedsRebootFlag = false;
                                ReturnErrorCode = DriverPackageUninstallW(DriverStoreInfPath.ToString(), DRIVER_PACKAGE_FORCE, IntPtr.Zero, ref NeedsRebootFlag);

                                if (ReturnErrorCode == ERROR_SUCCESS)
                                {
                                    if (NeedsRebootFlag == true)
                                    {
                                        backgroundArgs.resultStrings.Add("Complete: Driver removal successful.  Please reboot to complete the process.");
                                    }
                                    else
                                    {
                                        backgroundArgs.resultStrings.Add("Complete: Driver removal successful.");
                                    }
                                }
                                else if (ReturnErrorCode == ERROR_ACCESS_DENIED)
                                {
                                    MessageBox.Show("The drivers cannot be removed without administrator privileges.", "Info");
                                    backgroundArgs.resultStrings.Add("Driver removal failed.  Please restart this application with admin privileges.");
                                }
                                else
                                {
                                    //Convert the error code to a hexadecimal formatted string, for easier human readability.
                                    ReturnErrorCodeString = String.Format("{0:X}", ReturnErrorCode);
                                    backgroundArgs.resultStrings.Add("Driver removal failed.  DriverPackageUninstall() error code = 0x" + ReturnErrorCodeString + ".");
                                    if(backgroundArgs.mode == AUTO_INSTALL)
                                    {   
                                        MessageBox.Show("Driver removal failed.  DriverPackageUninstall() error code = 0x" + ReturnErrorCodeString + ".","Error");
                                    }
                                }

                            }
                            else if (ReturnErrorCode == ERROR_ACCESS_DENIED)
                            {
                                MessageBox.Show("The drivers cannot be removed without administrator privileges.", "Info");
                                backgroundArgs.resultStrings.Add("Driver removal failed.  Please restart this application with admin privileges.");
                            }
                            else if (ReturnErrorCode == ERROR_DRIVER_PACKAGE_NOT_IN_STORE)
                            {
                                //In this case, the driver wasn't in the driver store.
                                backgroundArgs.resultStrings.Add("Complete: Driver was not present in the driver store.");
                            }
                            else
                            {
                                //Convert the error code to a hexadecimal formatted string, for easier human readability.
                                ReturnErrorCodeString = String.Format("{0:X}", ReturnErrorCode);
                                backgroundArgs.resultStrings.Add("Driver removal failed.  DriverPackageGetPath() error code = 0x" + ReturnErrorCodeString + ".");
                                if(backgroundArgs.mode == AUTO_INSTALL)
                                {
                                    MessageBox.Show("Driver removal failed.  DriverPackageGetPath() error code = 0x" + ReturnErrorCodeString + ".","Error");
                                }
                            }

                        }
                    }
                }
                catch (Exception ex)
                {
                    backgroundArgs.resultStrings.Add("Driver removal failed.  Exception: " + ex.ToString() + ".");
                    if(backgroundArgs.mode == AUTO_INSTALL)
                    {
                        MessageBox.Show("Driver removal failed.  Exception: " + ex.ToString() + ".", "Error");
                    }
                }
            }

            //Cannot directly update the form from the thread.  Therefore, we pass the result
            //strings to the backgroundWorker1_RunWorkerCompleted() event handler,
            //so that it can then print the strings to the list box on the form.
            e.Result = (backgroundWorkerArgs)backgroundArgs;
        }

        private void backgroundWorker1_RunWorkerCompleted(object sender, RunWorkerCompletedEventArgs e)
        {
            //Upon completion of the thread.  Now update the list box with the new text lines we generated in the thread.
            //The text lines are passed to this function in the RunWorkerCompletedEventArgs e.

            //Arguments must be cast into backgroundWorkerArgs class and
            //copied into a new object in order to be accessed
            backgroundWorkerArgs backgroundArgs = new backgroundWorkerArgs();
            backgroundArgs = (backgroundWorkerArgs)e.Result;
            
            foreach (String i in backgroundArgs.resultStrings)
            {
                OutputStatus_lstbx.Items.Add(i);
            }

            if (backgroundArgs.mode == AUTO_INSTALL)
            {
                this.Close();       //in auto mode, close window on completion
            }
            else
            {
                //Re-enable the buttons now.
                Install_btn.Enabled = true;
                Remove_btn.Enabled = true;
            }
            
        }


    }
}