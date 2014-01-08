'/********************************************************************
' FileName:		Form1.vb
' Dependencies:	When compiled, needs .NET framework 2.0 redistributable 
'               to be installed in order to run.
' Hardware:		Need a free USB port to connect USB peripheral device
'				programmed with appropriate Generic HID firmware.  VID and
'				PID in firmware must match the VID and PID in this
'				program.
' Compiler:  	Microsoft Visual Basic 2008 Express Edition (or higher)
' Company:		Microchip Technology, Inc.

' Software License Agreement:

' The software supplied herewith by Microchip Technology Incorporated
' (the “Company”) for its PIC® Microcontroller is intended and
' supplied to you, the Company’s customer, for use solely and
' exclusively with Microchip PIC Microcontroller products. The
' software is owned by the Company and/or its supplier, and is
' protected under applicable copyright laws. All rights are reserved.
' Any use in violation of the foregoing restrictions may subject the
' user to criminal sanctions under applicable laws, as well as to
' civil liability for the breach of the terms and conditions of this
' license.

' THIS SOFTWARE IS PROVIDED IN AN “AS IS” CONDITION. NO WARRANTIES,
' WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED
' TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
' PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. THE COMPANY SHALL NOT,
' IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL OR
' CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.

'********************************************************************
' File Description:

' Change History:
'  Rev   Date         Description
'  2.8	 06/30/2010	  Initial Release.  Ported project from the existing
'                     HID PnP Demo projects (written in Microsoft Visual 
'                     C++, as well as Microsoft Visual C#) from the
'                     MCHPFSUSB Framework v2.7
'********************************************************************
'NOTE:	All user made code contained in this project is in the Form1.vb file.
'		All other code and files were generated automatically by either the
'		new project wizard, or by the development environment (ex: code is
'		automatically generated if you create a new button on the form, and
'		then double click on it, which creates a click event handler
'		function). 
'********************************************************************/

'IMPORTANT NOTE: In order for this PC application to "find" the USB device,
'this application needs to know the USB Vendor ID (VID) and Product 
'ID (PID) of the USB device we want to communicate with.  This is
'specified in the DeviceIDToFind String shown below.  The VID and PID
'in the DeviceIDToFind string must match the VID/PID from the USB
'device descriptor specified in the microcontroller firmware.
'When changing the VID/PID used by the application, make sure to update
'both the firmware and the DeviceIDToFind string below.

'//NOTE 2: This VB program makes use of several functions in setupapi.dll and
'//other Win32 DLLs.  However, one cannot call the functions directly in a 
'//32-bit DLL if the project is built in "Any CPU" mode, when run on a 64-bit OS.
'//When configured to build an "Any CPU" executable, the executable will "become"
'//a 64-bit executable when run on a 64-bit OS.  On a 32-bit OS, it will run as 
'//a 32-bit executable, and the pointer sizes and other aspects of this 
'//application will be compatible with calling 32-bit DLLs.

'//Therefore, on a 64-bit OS, this application will not work unless it is built in
'//"x86" mode.  When built in this mode, the exectuable always runs in 32-bit mode
'//even on a 64-bit OS.  This allows this application to make 32-bit DLL function 
'//calls, when run on either a 32-bit or 64-bit OS.

'//By default, on a new project, VB normally wants to build in "Any CPU" mode.  
'//To switch to "x86" (32-bit only) mode:
'//1.  Click: Tools --> Options...
'//2.  Under "Project and Solutions", make sure the "Show advanced build configurations" checkbox is checked.
'//3.  Now click: Build --> Configuration Manager...
'//4.  Under "Active Solution Platform", select "x86".  If x86 doesn't exist in the list, select "<New...>"
'//    Then under the "Type or select the new platform" dropdown box, select "x86" and click OK


Imports Microsoft.Win32.SafeHandles
Imports System.Runtime.InteropServices      'Need this for Marshal class and for DLLImport operations
Imports System.Threading

Public Class Form1
    '//----------------------------------------------------------------------------------------------
    '//Use the formatting: "Vid_xxxx&Pid_xxxx" where xxxx is a 16-bit hexadecimal number.
    '//Make sure the value appearing in the parathesis matches the USB device descriptor
    '//of the device that this aplication is intending to find.
    Dim DeviceIDToFind As String = "Vid_04d8&Pid_003f"
    '//----------------------------------------------------------------------------------------------


    '//Constant definitions from setupapi.h
    Const DIGCF_PRESENT As UInteger = &H2
    Const DIGCF_DEVICEINTERFACE As UInteger = &H10
    '//Constants for CreateFile() and other file I/O functions
    Const FILE_ATTRIBUTE_NORMAL As Short = &H80
    Const INVALID_HANDLE_VALUE As Short = -1
    Const GENERIC_READ As UInteger = &H80000000&                    'Hmm... It seems VB doesn't really support unsigned types...
    'Dim GENERIC_READ As UInteger = Convert.ToUInt32(&H80000000)     'Need to use this instead.
    Const GENERIC_WRITE As UInteger = &H40000000
    Const CREATE_NEW As UInteger = &H1
    Const CREATE_ALWAYS As UInteger = &H2
    Const OPEN_EXISTING As UInteger = &H3
    Const FILE_SHARE_READ As UInteger = &H1
    Const FILE_SHARE_WRITE As UInteger = &H2
    '//Constant definitions for certain WM_DEVICECHANGE messages
    Const WM_DEVICECHANGE As UInteger = &H219
    Const DBT_DEVICEARRIVAL As UInteger = &H8000
    Const DBT_DEVICEREMOVEPENDING As UInteger = &H8003
    Const DBT_DEVICEREMOVECOMPLETE As UInteger = &H8004
    Const DBT_CONFIGCHANGED As UInteger = &H18

    '//Other constant definitions
    Const DBT_DEVTYP_DEVICEINTERFACE As UInteger = &H5
    Const DEVICE_NOTIFY_WINDOW_HANDLE As UInteger = &H0
    Const ERROR_SUCCESS As UInteger = &H0
    Const ERROR_NO_MORE_ITEMS As UInteger = &H103
    Const SPDRP_HARDWAREID As UInteger = &H1


    '//Various structure definitions for structures that this code will be using
    Structure SP_DEVICE_INTERFACE_DATA
        Dim cbSize As UInteger
        Dim InterfaceClassGuid As Guid
        Dim Flags As UInteger
        Dim Reserved As IntPtr
    End Structure

    Structure SP_DEVICE_INTERFACE_DETAIL_DATA
        Dim cbSize As UInteger
        Dim DevicePath() As Char
    End Structure

    Structure SP_DEVINFO_DATA
        Dim cbSize As UInteger
        Dim ClassGuid As Guid
        Dim DevInst As UInteger
        Dim Reserved As IntPtr
    End Structure

    Structure DEV_BROADCAST_DEVICEINTERFACE
        Dim dbcc_size As UInteger
        Dim dbcc_devicetype As UInteger
        Dim dbcc_reserved As UInteger
        Dim dbcc_classguid As Guid
        Dim dbcc_name() As Char
    End Structure

    '//DLL Imports.  Need these to access various C style unmanaged functions contained in their respective DLL files.
    '//--------------------------------------------------------------------------------------------------------------

    '//Returns a HDEVINFO type for a device information set.  We will need the 
    '//HDEVINFO as in input parameter for calling many of the other SetupDixxx() functions.
    <DllImport("setupapi.dll", CharSet:=CharSet.Unicode, SetLastError:=True)> _
    Public Shared Function SetupDiGetClassDevs(ByRef ClassGuid As Guid, ByVal Enumerator As IntPtr, ByVal hwndParent As IntPtr, ByVal Flags As UInteger) As IntPtr
    End Function

    '//Gives us "PSP_DEVICE_INTERFACE_DATA" which contains the Interface specific GUID (different
    '//from class GUID).  We need the interface GUID to get the device path.
    <DllImport("setupapi.dll", CharSet:=CharSet.Unicode, SetLastError:=True)> _
    Public Shared Function SetupDiEnumDeviceInterfaces(ByVal DeviceInfoSet As IntPtr, ByVal DeviceInfoData As IntPtr, ByRef InterfaceClassGuid As Guid, ByVal MemberIndex As UInteger, ByRef DeviceInterfaceData As SP_DEVICE_INTERFACE_DATA) As Boolean
    End Function

    ''//SetupDiDestroyDeviceInfoList() frees up memory by destroying a DeviceInfoList
    <DllImport("setupapi.dll", CharSet:=CharSet.Unicode, SetLastError:=True)> _
    Public Shared Function SetupDiDestroyDeviceInfoList(ByVal DeviceInfoSet As IntPtr) As Boolean
    End Function

    ''//SetupDiEnumDeviceInfo() fills in an "SP_DEVINFO_DATA" structure, which we need for SetupDiGetDeviceRegistryProperty()
    <DllImport("setupapi.dll", CharSet:=CharSet.Unicode, SetLastError:=True)> _
    Public Shared Function SetupDiEnumDeviceInfo(ByVal DeviceInfoSet As IntPtr, ByVal MemberIndex As UInteger, ByRef DeviceInterfaceData As SP_DEVINFO_DATA) As Boolean
    End Function

    ''//SetupDiGetDeviceRegistryProperty() gives us the hardware ID, which we use to check to see if it has matching VID/PID
    <DllImport("setupapi.dll", CharSet:=CharSet.Unicode, SetLastError:=True)> _
    Public Shared Function SetupDiGetDeviceRegistryProperty( _
        ByVal DeviceInfoSet As IntPtr, _
        ByRef DeviceInfoData As SP_DEVINFO_DATA, _
        ByVal dwProperty As UInteger, _
        ByRef PropertyRegDataType As UInteger, _
        ByVal PropertyBuffer As IntPtr, _
        ByVal PropertyBufferSize As UInteger, _
        ByRef RequiredSize As UInteger) As Boolean
    End Function

    ''//SetupDiGetDeviceInterfaceDetail() gives us a device path, which is needed before CreateFile() can be used.
    <DllImport("setupapi.dll", CharSet:=CharSet.Unicode, SetLastError:=True)> _
    Public Shared Function SetupDiGetDeviceInterfaceDetail( _
        ByVal DeviceInfoSet As IntPtr, _
        ByRef DeviceInterfaceData As SP_DEVICE_INTERFACE_DATA, _
        ByVal DeviceInterfaceDetailData As IntPtr, _
        ByVal DeviceInterfaceDetailDataSize As UInteger, _
        ByRef RequiredSize As UInteger, _
        ByVal DeviceInfoData As IntPtr) As Boolean
    End Function

    ''//Overload for SetupDiGetDeviceInterfaceDetail().  Need this one since we can't pass NULL pointers directly.
    <DllImport("setupapi.dll", CharSet:=CharSet.Unicode, SetLastError:=True)> _
    Public Shared Function SetupDiGetDeviceInterfaceDetail( _
        ByVal DeviceInfoSet As IntPtr, _
        ByRef DeviceInterfaceData As SP_DEVICE_INTERFACE_DATA, _
        ByVal DeviceInterfaceDetailData As IntPtr, _
        ByVal DeviceInterfaceDetailDataSize As UInteger, _
        ByVal RequiredSize As IntPtr, _
        ByVal DeviceInfoData As IntPtr) As Boolean
    End Function

    '//Need this function for receiving all of the WM_DEVICECHANGE messages.  See MSDN documentation for
    '//description of what this function does/how to use it.
    <DllImport("user32.dll", CharSet:=CharSet.Unicode, SetLastError:=True)> _
    Public Shared Function RegisterDeviceNotification( _
        ByVal hRecipient As IntPtr, _
        ByVal NotificationFilter As IntPtr, _
        ByVal Flags As UInteger) As IntPtr
    End Function

    '//Takes in a device path and opens a handle to the device.  We need to create read and write
    '//handles, before we can read/write to the USB device endpoints
    <DllImport("kernel32.dll", CharSet:=CharSet.Unicode, SetLastError:=True)> _
    Public Shared Function CreateFile( _
        ByVal FileName As String, _
        ByVal dwDesiredAccess As UInteger, _
        ByVal dwShareMode As UInteger, _
        ByVal lpSecurityAttributes As IntPtr, _
        ByVal dwCreationDisposition As UInteger, _
        ByVal dwFlagsAndAttributes As UInteger, _
        ByVal hTemplateFile As IntPtr) As SafeFileHandle
    End Function

    '//Uses a handle (created with CreateFile()), and lets us write USB data to the HID device interrupt OUT endpoint
    <DllImport("kernel32.dll", CharSet:=CharSet.Unicode, SetLastError:=True)> _
    Public Shared Function WriteFile( _
        ByVal hFile As SafeFileHandle, _
        ByVal Buffer As Byte(), _
        ByVal nNumberOfBytesToWrite As UInteger, _
        ByRef lpNumberOfBytesWritten As UInteger, _
        ByVal lpOverlapped As IntPtr) As Boolean
    End Function


    '//Uses a handle (created with CreateFile()), and lets us read USB data from the HID device interrupt IN endpoint.
    <DllImport("kernel32.dll", CharSet:=CharSet.Unicode, SetLastError:=True)> _
    Public Shared Function ReadFile( _
        ByVal hFile As SafeFileHandle, _
        ByVal lpBuffer As IntPtr, _
        ByVal nNumberOfBytesToWrite As UInteger, _
        ByRef lpNumberOfBytesRead As UInteger, _
        ByVal lpOverlapped As IntPtr) As Boolean
    End Function



    '/*** This section is all of the global variables used in this project ***/
    '//--------------------------------------------------------------------------------------------------------

    'Dim DeviceIDToFind As String = "Vid_xxxx&Pid_yyyy" 'NOTE: The global variable DeviceIDToFind is located near the
    'top of this file, to make it easier to find/modify.

    '//Globally Unique Identifier (GUID) for HID class devices.  Windows uses GUIDs to identify things.
    Dim InterfaceClassGuid As New Guid("4D1E55B2-F16F-11CF-88CB-001111000030")

    '//USB related variables that need to have wide scope.
    Dim AttachedState As Boolean = False     '//Need to keep track of the USB device attachment status for proper plug and play operation.
    Dim AttachedButBroken As Boolean = False

    Dim DetailedInterfaceDataStructure As New SP_DEVICE_INTERFACE_DETAIL_DATA
    Dim ReadHandleToUSBDevice As SafeFileHandle
    Dim WriteHandleToUSBDevice As SafeFileHandle
    Dim DevicePath As String    '//Need the find the proper device path before you can open file handles.

    '//Variables used by the application/form updates.
    Dim ADCValue As Integer = 0              '//Updated by ReadWriteThread, read by FormUpdateTimer tick handler (needs to be atomic)
    Dim PushbuttonPressed = False            '//Updated by ReadWriteThread, read by FormUpdateTimer tick handler (needs to be atomic)
    Dim ToggleLEDsPending As Boolean = False '//Updated by ToggleLED(s) button click event handler, used by ReadWriteThread (needs to be atomic)



    '//--------------------------------------------------------------------------------------------------------------------------
    '//FUNCTION:	Form1_Load()
    '//PURPOSE:	    This callback function gets called when the application is initially launched and the form loads.  This is a good
    '//             place to do initialization for the rest of the application.  
    '//INPUT:	    None
    '//OUTPUT:	    May write to the WriteHandleToUSBDevice, ReadHandleToUSBDevice, AttachedState, and AttachedButBroken global variables
    '//--------------------------------------------------------------------------------------------------------------------------
    Private Sub Form1_Load(ByVal sender As Object, ByVal e As System.EventArgs) Handles Me.Load
        '//Initialize tool tips, to provide pop up help when the mouse cursor is moved over objects on the form.
        ANxVoltageToolTip.SetToolTip(ANxVoltage_lbl, "If using a board/PIM without a potentiometer, apply an adjustable voltage to the I/O pin.")
        ANxVoltageToolTip.SetToolTip(progressBar1, "If using a board/PIM without a potentiometer, apply an adjustable voltage to the I/O pin.")
        ToggleLEDToolTip.SetToolTip(ToggleLEDs_btn, "Sends a packet of data to the USB device.")
        PushbuttonStateTooltip.SetToolTip(PushbuttonState_lbl, "Try pressing pushbuttons on the USB demo board/PIM.")

        '//Register for WM_DEVICECHANGE notifications.  This code uses these messages to detect plug and play connection/disconnection events for USB devices
        Dim DeviceBroadcastHeader As New DEV_BROADCAST_DEVICEINTERFACE
        DeviceBroadcastHeader.dbcc_devicetype = DBT_DEVTYP_DEVICEINTERFACE
        DeviceBroadcastHeader.dbcc_size = Marshal.SizeOf(DeviceBroadcastHeader)
        DeviceBroadcastHeader.dbcc_reserved = 0   '//Reserved says not to use...
        DeviceBroadcastHeader.dbcc_classguid = InterfaceClassGuid

        '//Need to get the address of the DeviceBroadcastHeader to call RegisterDeviceNotification(), but
        '//can't use "&DeviceBroadcastHeader".  Instead, using a roundabout means to get the address by 
        '//making a duplicate copy using Marshal.StructureToPtr().
        Dim pDeviceBroadcastHeader As IntPtr = IntPtr.Zero  '//Make a pointer.
        'Note: Below line could throw an exception if the Mashal.AllocHGlobal() fails to allocate the memory, ex: because the system doesn't have enough free RAM available
        pDeviceBroadcastHeader = Marshal.AllocHGlobal(Marshal.SizeOf(DeviceBroadcastHeader)) '//allocate memory for a new DEV_BROADCAST_DEVICEINTERFACE structure, and return the address 
        Marshal.StructureToPtr(DeviceBroadcastHeader, pDeviceBroadcastHeader, False)    '//Copies the DeviceBroadcastHeader structure into the unmanaged memory already allocated at pDeviceBroadcastHeader
        RegisterDeviceNotification(Me.Handle, pDeviceBroadcastHeader, DEVICE_NOTIFY_WINDOW_HANDLE)
        'Now that we have registered to receive Windows Messages like WM_DEVICECHANGE, free the unmanaged RAM we previously allocated
        Marshal.FreeHGlobal(pDeviceBroadcastHeader)

        '//Now make an initial attempt to find the USB device, if it was already connected to the PC and enumerated prior to launching the application.
        '//If it is connected and present, we should open read and write handles to the device so we can communicate with it later.
        '//If it was not connected, we will have to wait until the user plugs the device in, and the WM_DEVICECHANGE callback function can process
        '//the message and again search for the device.
        If CheckIfPresentAndGetUSBDevicePath() Then '//Check and make sure at least one device with matching VID/PID is attached
            Dim ErrorStatusWrite As UInteger
            Dim ErrorStatusRead As UInteger

            '//We now have the proper device path, and we can finally open read and write handles to the device.
            WriteHandleToUSBDevice = CreateFile(DevicePath, GENERIC_WRITE, FILE_SHARE_READ Or FILE_SHARE_WRITE, IntPtr.Zero, OPEN_EXISTING, 0, IntPtr.Zero)
            ErrorStatusWrite = Marshal.GetLastWin32Error()
            ReadHandleToUSBDevice = CreateFile(DevicePath, GENERIC_READ, FILE_SHARE_READ Or FILE_SHARE_WRITE, IntPtr.Zero, OPEN_EXISTING, 0, IntPtr.Zero)
            ErrorStatusRead = Marshal.GetLastWin32Error()


            If (ErrorStatusWrite = ERROR_SUCCESS) And (ErrorStatusRead = ERROR_SUCCESS) Then
                AttachedState = True        '//Let the rest of the PC application know the USB device is connected, and it is safe to read/write to it
                AttachedButBroken = False
                StatusBox_txtbx.Text = "Device Found, AttachedState = TRUE"
            Else '//for some reason the device was physically plugged in, but one or both of the read/write handles didn't open successfully...
                AttachedState = False       '//Let the rest of this application known not to read/write to the device.
                AttachedButBroken = True    '//Flag so that next time a WM_DEVICECHANGE message occurs, can retry to re-open read/write pipes
                If ErrorStatusWrite = ERROR_SUCCESS Then
                    WriteHandleToUSBDevice.Close()
                End If

                If ErrorStatusRead = ERROR_SUCCESS Then
                    ReadHandleToUSBDevice.Close()
                End If
            End If
        Else
            AttachedState = False
            AttachedButBroken = False
        End If


        If AttachedState = True Then
            StatusBox_txtbx.Text = "Device Found, AttachedState = TRUE"
        Else
            StatusBox_txtbx.Text = "Device not found, verify connect/correct firmware"
        End If


        '//Recommend performing USB read/write operations in a separate thread.  Otherwise,
        '//the Read/Write operations are effectively blocking functions and can lock up the
        '//user interface if the I/O operations take a long time to complete.
        ReadWriteThread.RunWorkerAsync()    '//Starts the thread
    End Sub



    '//--------------------------------------------------------------------------------------------------------------------------
    '//FUNCTION:	WndProc()
    '//PURPOSE:	    This callback function gets called when Windows sends windows messages to the form (ex: WM_DEVICECHANGE for instance)
    '//INPUT:	    The windows message.
    '//OUTPUT:	    Updates the AttachedState and AttachedButBroken boolean variables, based on the USB connection status following WM_DEVICECHANGE events.
    '//--------------------------------------------------------------------------------------------------------------------------
    Protected Overrides Sub WndProc(ByRef m As System.Windows.Forms.Message)
        'Check for WM_DEVICECHANGE messages, which Windows will send you when certain events
        '(ex: Plug and Play hardware changes) take place in the system.
        If m.Msg = WM_DEVICECHANGE Then
            If ((m.WParam = DBT_DEVICEARRIVAL) Or (m.WParam = DBT_DEVICEREMOVEPENDING) Or (m.WParam = DBT_DEVICEREMOVECOMPLETE) Or (m.WParam = DBT_CONFIGCHANGED)) Then
                '//WM_DEVICECHANGE messages by themselves are quite generic, and can be caused by a number of different
                '//sources, not just your USB hardware device.  Therefore, must check to find out if any changes relavant
                '//to your device (with known VID/PID) took place before doing any kind of opening or closing of handles/endpoints.
                '//(the message could have been totally unrelated to your application/USB device)

                If CheckIfPresentAndGetUSBDevicePath() = True Then      '//Check and make sure at least one device with matching VID/PID is attached
                    If ((AttachedState = False) Or (AttachedButBroken = True)) Then '//Check the previous attachment state
                        Dim ErrorStatusRead As UInteger
                        Dim ErrorStatusWrite As UInteger

                        '//We obtained the proper device path (from CheckIfPresentAndGetUSBDevicePath() function call), and it
                        '//is now possible to open read and write handles to the device.
                        WriteHandleToUSBDevice = CreateFile(DevicePath, GENERIC_WRITE, FILE_SHARE_READ Or FILE_SHARE_WRITE, IntPtr.Zero, OPEN_EXISTING, 0, IntPtr.Zero)
                        ErrorStatusWrite = Marshal.GetLastWin32Error()
                        ReadHandleToUSBDevice = CreateFile(DevicePath, GENERIC_READ, FILE_SHARE_READ Or FILE_SHARE_WRITE, IntPtr.Zero, OPEN_EXISTING, 0, IntPtr.Zero)
                        ErrorStatusRead = Marshal.GetLastWin32Error()


                        If ((ErrorStatusWrite = ERROR_SUCCESS) And (ErrorStatusRead = ERROR_SUCCESS)) Then
                            AttachedState = True        '//Let the rest of the PC application know the USB device is connected, and it is safe to read/write to it
                            AttachedButBroken = False
                            StatusBox_txtbx.Text = "Device Found, AttachedState = TRUE"
                        Else '//for some reason the device was physically plugged in, but one or both of the read/write handles didn't open successfully...
                            AttachedState = False       '//Let the rest of this application known not to read/write to the device.
                            AttachedButBroken = True    '//Flag so that next time a WM_DEVICECHANGE message occurs, can retry to re-open read/write pipes
                            If ErrorStatusWrite = ERROR_SUCCESS Then
                                WriteHandleToUSBDevice.Close()
                            End If
                            If ErrorStatusRead = ERROR_SUCCESS Then
                                ReadHandleToUSBDevice.Close()
                            End If
                        End If
                    End If
                    '//else we did find the device, but AttachedState was already true.  In this case, don't do anything to the read/write handles,
                    '//since the WM_DEVICECHANGE message presumably wasn't caused by our USB device.  
                Else 'CheckIfPresentAndGetUSBDevicePath() must have returned False

                    If AttachedState = True Then    '//If it is currently set to true, that means the device was just now disconnected
                        AttachedState = False
                        WriteHandleToUSBDevice.Close()
                        ReadHandleToUSBDevice.Close()
                    End If
                    AttachedState = False
                    AttachedButBroken = False
                End If 'If CheckIfPresentAndGetUSBDevicePath() = True
            End If 'If ((m.WParam = DBT_DEVICEARRIVAL) Or (m.WParam = DBT_DEVICEREMOVEPENDING) Or (m.WParam = DBT_DEVICEREMOVECOMPLETE) Or (m.WParam = DBT_CONFIGCHANGED)) Then
        End If 'If m.Msg = WM_DEVICECHANGE Then

        'Call the regular event handler, to it can handle normal windows messages
        MyBase.WndProc(m)
    End Sub




    '//FUNCTION: CheckIfPresentAndGetUSBDevicePath()
    '//PURPOSE:	 Check if a USB device is currently plugged in with a matching VID and PID
    '//INPUT:	 Uses globally declared String DevicePath, globally declared GUID, and the MY_DEVICE_ID constant.
    '//OUTPUT:	 Returns BOOL.  TRUE when device with matching VID/PID found.  FALSE if device with VID/PID could not be found.
    '//			 When returns TRUE, the globally accessable "DetailedInterfaceDataStructure" will contain the device path
    '//			 to the USB device with the matching VID/PID.
    Private Function CheckIfPresentAndGetUSBDevicePath() As Boolean
        'Before we can "connect" our application to our USB embedded device, we must first find the device.
        'A USB bus can have many devices simultaneously connected, so somehow we have to find our device only.
        'This is done with the Vendor ID (VID) and Product ID (PID).  Each USB product line should have
        'a unique combination of VID and PID.  

        'Microsoft has created a number of functions which are useful for finding plug and play devices.  Documentation
        'for each function used can be found in the MSDN library.  We will be using the following functions (unmanaged C functions):

        'SetupDiGetClassDevs()					//provided by setupapi.dll, which comes with Windows
        'SetupDiEnumDeviceInterfaces()			//provided by setupapi.dll, which comes with Windows
        'GetLastError()							//provided by kernel32.dll, which comes with Windows
        'SetupDiDestroyDeviceInfoList()			//provided by setupapi.dll, which comes with Windows
        'SetupDiGetDeviceInterfaceDetail()		//provided by setupapi.dll, which comes with Windows
        'SetupDiGetDeviceRegistryProperty()		//provided by setupapi.dll, which comes with Windows
        'CreateFile()							//provided by kernel32.dll, which comes with Windows

        'In order to call these unmanaged functions, the Marshal class is very useful.

        'We will also be using the following unusual data types and structures.  Documentation can also be found in
        'the MSDN library:
        'PSP_DEVICE_INTERFACE_DATA()
        'PSP_DEVICE_INTERFACE_DETAIL_DATA()
        'SP_DEVINFO_DATA()
        'HDEVINFO()
        'Handle()
        'Guid()

        'The ultimate objective of the following code is to get the device path, which will be used elsewhere for getting
        'read and write handles to the USB device.  Once the read/write handles are opened, only then can this
        'PC application begin reading/writing to the USB device using the WriteFile() and ReadFile() functions.

        'Getting the device path is a multi-step round about process, which requires calling several of the
        'SetupDixxx() functions provided by setupapi.dll.
        Try
            Dim DeviceInfoTable As IntPtr = IntPtr.Zero
            Dim InterfaceDataStructure As New SP_DEVICE_INTERFACE_DATA()
            Dim DetailedInterfaceDataStructure As New SP_DEVICE_INTERFACE_DETAIL_DATA()
            Dim DevInfoData As New SP_DEVINFO_DATA()
            Dim InterfaceIndex As UInteger = 0
            Dim dwRegType As UInteger = 0
            Dim dwRegSize As UInteger = 0
            Dim dwRegSize2 As UInteger = 0
            Dim StructureSize As UInteger = 0
            Dim PropertyValueBuffer As IntPtr = IntPtr.Zero
            Dim MatchFound As Boolean = False
            Dim ErrorStatus As UInteger = 0
            Dim LoopCounter As UInteger = 0

            '//Use the formatting: "Vid_xxxx&Pid_xxxx" where xxxx is a 16-bit hexadecimal number.
            '//Make sure the value appearing in the parathesis matches the USB device descriptor
            '//of the device that this aplication is intending to find.
            Dim DeviceIDToFind As String = "Vid_04d8&Pid_003f"

            '//First populate a list of plugged in devices (by specifying "DIGCF_PRESENT"), which are of the specified class GUID. 
            DeviceInfoTable = SetupDiGetClassDevs(InterfaceClassGuid, IntPtr.Zero, IntPtr.Zero, DIGCF_PRESENT Or DIGCF_DEVICEINTERFACE)
            'Make sure SetupDiGetClassDevs returned a valid/non-NULL pointer
            If DeviceInfoTable <> IntPtr.Zero Then
                While True
                    InterfaceDataStructure.cbSize = Marshal.SizeOf(InterfaceDataStructure)

                    If SetupDiEnumDeviceInterfaces(DeviceInfoTable, IntPtr.Zero, InterfaceClassGuid, InterfaceIndex, InterfaceDataStructure) Then
                        ErrorStatus = Marshal.GetLastWin32Error()

                        If ErrorStatus = ERROR_NO_MORE_ITEMS Then   '//Did we reach the end of the list of matching devices in the DeviceInfoTable?
                            '//Cound not find the device.  Must not have been attached.
                            SetupDiDestroyDeviceInfoList(DeviceInfoTable)   '//Clean up the old structure we no longer need.
                            Return False
                        End If

                        '//Now retrieve the hardware ID from the registry.  The hardware ID contains the VID and PID, which we will then 
                        '//check to see if it is the correct device or not.

                        '//Initialize an appropriate SP_DEVINFO_DATA structure.  We need this structure for SetupDiGetDeviceRegistryProperty().
                        DevInfoData.cbSize = Marshal.SizeOf(DevInfoData)
                        SetupDiEnumDeviceInfo(DeviceInfoTable, InterfaceIndex, DevInfoData)

                        '//First query for the size of the hardware ID, so we can know how big a buffer to allocate for the data.
                        SetupDiGetDeviceRegistryProperty(DeviceInfoTable, DevInfoData, SPDRP_HARDWAREID, dwRegType, IntPtr.Zero, 0, dwRegSize)

                        '//Allocate a buffer for the hardware ID.
                        '//Should normally work, but could throw exception "OutOfMemoryException" if not enough resources available.
                        PropertyValueBuffer = Marshal.AllocHGlobal(CType(dwRegSize, IntPtr))

                        '//Retrieve the hardware IDs for the current device we are looking at.  PropertyValueBuffer gets filled with a 
                        '//REG_MULTI_SZ (array of null terminated strings).  To find a device, we only care about the very first string in the
                        '//buffer, which will be the "device ID".  The device ID is a string which contains the VID and PID, in the example 
                        '//format "Vid_04d8&Pid_003f".
                        SetupDiGetDeviceRegistryProperty(DeviceInfoTable, DevInfoData, SPDRP_HARDWAREID, dwRegType, PropertyValueBuffer, dwRegSize, dwRegSize2)
                        ErrorStatus = Marshal.GetLastWin32Error()

                        '//Now check if the first string in the hardware ID matches the device ID of the USB device we are trying to find.
                        Dim DeviceIDFromRegistry As String
                        DeviceIDFromRegistry = Marshal.PtrToStringUni(PropertyValueBuffer) '//Fill the string with the contents from the PropertyValueBuffer
                        Marshal.FreeHGlobal(PropertyValueBuffer)                            '//No longer need the PropertyValueBuffer, free the memory to prevent potential memory leaks

                        '//Convert both strings to lower case.  This makes the code more robust/portable accross OS Versions
                        DeviceIDFromRegistry = DeviceIDFromRegistry.ToLowerInvariant()
                        DeviceIDToFind = DeviceIDToFind.ToLowerInvariant()
                        '//Now check if the hardware ID we are looking at contains the correct VID/PID
                        MatchFound = DeviceIDFromRegistry.Contains(DeviceIDToFind)

                        If MatchFound Then
                            '//Device must have been found.  In order to open I/O file handle(s), we will need the actual device path first.
                            '//We can get the path by calling SetupDiGetDeviceInterfaceDetail(), however, we have to call this function twice:  The first
                            '//time to get the size of the required structure/buffer to hold the detailed interface data, then a second time to actually 
                            '//get the structure (after we have allocated enough memory for the structure.)
                            DetailedInterfaceDataStructure.cbSize = Marshal.SizeOf(DetailedInterfaceDataStructure)
                            '//First call populates "StructureSize" with the correct value
                            SetupDiGetDeviceInterfaceDetail(DeviceInfoTable, InterfaceDataStructure, IntPtr.Zero, 0, StructureSize, IntPtr.Zero)
                            '//Need to call SetupDiGetDeviceInterfaceDetail() again, this time specifying a pointer to a SP_DEVICE_INTERFACE_DETAIL_DATA buffer with the correct size of RAM allocated.
                            '//First need to allocate the unmanaged buffer and get a pointer to it.
                            Dim pUnmanagedDetailedInterfaceDataStructure As IntPtr = IntPtr.Zero    '//Declare a pointer.
                            pUnmanagedDetailedInterfaceDataStructure = Marshal.AllocHGlobal(CType(StructureSize, IntPtr))    '//Reserve some unmanaged memory for the structure.
                            DetailedInterfaceDataStructure.cbSize = 6   '//Initialize the cbSize parameter (4 bytes for DWORD + 2 bytes for unicode null terminator)
                            Marshal.StructureToPtr(DetailedInterfaceDataStructure, pUnmanagedDetailedInterfaceDataStructure, False) '//Copy managed structure contents into the unmanaged memory buffer.

                            '//Now call SetupDiGetDeviceInterfaceDetail() a second time to receive the device path in the structure at pUnmanagedDetailedInterfaceDataStructure.
                            If SetupDiGetDeviceInterfaceDetail(DeviceInfoTable, InterfaceDataStructure, pUnmanagedDetailedInterfaceDataStructure, StructureSize, IntPtr.Zero, IntPtr.Zero) Then
                                '//Need to extract the path information from the unmanaged "structure".  The path starts at (pUnmanagedDetailedInterfaceDataStructure + sizeof(DWORD)).
                                Dim pToDevicePath As IntPtr
                                pToDevicePath = pUnmanagedDetailedInterfaceDataStructure.ToInt32() + 4  '//Add 4 to the pointer (to get the pointer to point to the path, instead of the DWORD cbSize parameter)
                                DevicePath = Marshal.PtrToStringUni(pToDevicePath)  '//Now copy the path information into the globally defined DevicePath String.

                                '//We now have the proper device path, and we can finally use the path to open I/O handle(s) to the device.
                                SetupDiDestroyDeviceInfoList(DeviceInfoTable)   '//Clean up the old structure we no longer need.
                                Marshal.FreeHGlobal(pUnmanagedDetailedInterfaceDataStructure)   '//No longer need this unmanaged SP_DEVICE_INTERFACE_DETAIL_DATA buffer.  We already extracted the path information.
                                Return True '//Returning the device path in the global DevicePath String
                            Else
                                '//Some unknown failure occurred
                                Dim ErrorCode As UInteger = Marshal.GetLastWin32Error()
                                SetupDiDestroyDeviceInfoList(DeviceInfoTable)       '//Clean up the old structure.
                                Marshal.FreeHGlobal(pUnmanagedDetailedInterfaceDataStructure)   '//No longer need this unmanaged SP_DEVICE_INTERFACE_DETAIL_DATA buffer.  We already extracted the path information.
                                Return False
                            End If
                        End If

                        InterfaceIndex = InterfaceIndex + 1
                        '//Keep looping until we either find a device with matching VID and PID, or until we run out of devices to check.
                        '//However, just in case some unexpected error occurs, keep track of the number of loops executed.
                        '//If the number of loops exceeds a very large number, exit anyway, to prevent inadvertent infinite looping.
                        LoopCounter = LoopCounter + 1
                        If LoopCounter = 10000000 Then  '//Surely there aren't more than 10 million devices attached to any forseeable PC...
                            Return False
                        End If
                    Else
                        '//Else some other kind of unknown error ocurred...
                        ErrorStatus = Marshal.GetLastWin32Error()
                        SetupDiDestroyDeviceInfoList(DeviceInfoTable)   '//Clean up the old structure we no longer need.
                        Return False
                    End If 'If SetupDiEnumDeviceInterfaces(DeviceInfoTable, IntPtr.Zero, InterfaceClassGuid, InterfaceIndex, InterfaceDataStructure) Then
                End While 'While True
            End If  'If DeviceInfoTable <> IntPtr.Zero Then
            Return False
        Catch ex As Exception
            '//Something went wrong if PC gets here.  Maybe a Marshal.AllocHGlobal() failed due to insufficient resources or something.
            Return False
        End Try
    End Function


    '//--------------------------------------------------------------------------------------------------------------------------
    '//FUNCTION:	ToggleLEDs_btn_Click()
    '//PURPOSE:	    Queues up a USB I/O request for the ReadWriteThread_DoWork() thread to handle.
    '//INPUT:	    None.
    '//OUTPUT:	    Uses a global variable as a flag, which the thread watches.
    '//--------------------------------------------------------------------------------------------------------------------------
    Private Sub ToggleLEDs_btn_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles ToggleLEDs_btn.Click
        ToggleLEDsPending = True    '//Will get used asynchronously by the ReadWriteThread
    End Sub


    '//--------------------------------------------------------------------------------------------------------------------------
    '//FUNCTION:	FormUpdateTimer_Tick()
    '//PURPOSE:	    Periodically updates items on the main form, based on connection status changes and I/O results from the ReadWriteThread_DoWork() thread.
    '//INPUT:	    Uses global variables.
    '//OUTPUT:	    Updates the form printed on the screen.
    '//--------------------------------------------------------------------------------------------------------------------------
    Private Sub FormUpdateTimer_Tick(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles FormUpdateTimer.Tick
        '//This timer tick event handler function is used to update the user interface on the form, based on data
        '//obtained asynchronously by the ReadWriteThread and the WM_DEVICECHANGE event handler functions.

        '//Check if user interface on the form should be enabled or not, based on the attachment state of the USB device.
        If AttachedState = True Then
            '//Device is connected and ready to communicate, enable user interface on the form 
            StatusBox_txtbx.Text = "Device Found: AttachedState = TRUE"
            PushbuttonState_lbl.Enabled = True  '//Make the label no longer greyed out
            ANxVoltage_lbl.Enabled = True
            ToggleLEDs_btn.Enabled = True
        End If



        If (AttachedState = False) Or (AttachedButBroken = True) Then
            '//Device not available to communicate. Disable user interface on the form.
            StatusBox_txtbx.Text = "Device Not Detected: Verify Connection/Correct Firmware"
            PushbuttonState_lbl.Enabled = False '//Make the label no longer greyed out
            ANxVoltage_lbl.Enabled = False
            ToggleLEDs_btn.Enabled = False

            PushbuttonState_lbl.Text = "Pushbutton State: Unknown"
            ADCValue = 0
            progressBar1.Value = 0
        End If

        '//Update the various status indicators on the form with the latest info obtained from the ReadWriteThread()

        If AttachedState = True Then
            '//Update the pushbutton state label.
            If PushbuttonPressed = False Then
                PushbuttonState_lbl.Text = "Pushbutton State: Not Pressed"      '//Update the pushbutton state text label on the form, so the user can see the result 
            Else
                PushbuttonState_lbl.Text = "Pushbutton State: Pressed"          '//Update the pushbutton state text label on the form, so the user can see the result 
            End If

            '//Update the ANxx/POT Voltage indicator value (progressbar)
            progressBar1.Value = ADCValue
        End If
    End Sub

    '//--------------------------------------------------------------------------------------------------------------------------
    '//FUNCTION:	ReadWriteThread_DoWork()
    '//PURPOSE:	    This thread executes USB read/write requests and updates globabl variables.
    '//             It is generally preferrable to perform I/O operations in a separate thread from
    '//             the Windows form, so that the I/O operations (which can take a long time to complete/may
    '//             be blocking functions) do not block graphical and other updates to the form (leading
    '//             to the appearance of a "locked up" user interface.
    '//INPUT:	    Requires the global WriteHandleToUSBDevice and ReadHandleToUSBDevice to be initialized
    '//             and valid, in order for this thread to do anything useful.  This thread also takes in other global
    '//             variables like ToggleLEDsPending to determine what to do.
    '//OUTPUT:	    This thread updates global variables like ToggleLEDsPending (after processing the I/O request), and
    '//             ADCValue.
    '//--------------------------------------------------------------------------------------------------------------------------
    Private Sub ReadWriteThread_DoWork(ByVal sender As System.Object, ByVal e As System.ComponentModel.DoWorkEventArgs) Handles ReadWriteThread.DoWork
        '/*This thread does the actual USB read/write operations (but only when AttachedState == true) to the USB device.
        'It is generally preferrable to write applications so that read and write operations are handled in a separate
        'thread from the main form.  This makes it so that the main form can remain responsive, even if the I/O operations
        'take a very long time to complete.

        'Since this is a separate thread, this code below executes independently from the rest of the
        'code in this application.  All this thread does is read and write to the USB device.  It does not update
        'the form directly with the new information it obtains (such as the ANxx/POT Voltage or pushbutton state).
        'The information that this thread obtains is stored in atomic global variables.
        'Form updates are handled by the FormUpdateTimer Tick event handler function.

        'This application sends packets to the endpoint buffer on the USB device by using the "WriteFile()" function.
        'This application receives packets from the endpoint buffer on the USB device by using the "ReadFile()" function.
        'Both of these functions are documented in the MSDN library.  Calling ReadFile() is a not perfectly straight
        'foward in C# environment, since one of the input parameters is a pointer to a buffer that gets filled by ReadFile().
        'The ReadFile() function is therefore called through a wrapper function ReadFileManagedBuffer().

        'All ReadFile() and WriteFile() operations in this example project are synchronous.  They are blocking function
        'calls and only return when they are complete, or if they fail because of some event, such as the user unplugging
        'the device.  It is possible to call these functions with "overlapped" structures, and use them as non-blocking
        'asynchronous I/O function calls.  

        'Note:  This code may perform differently on some machines when the USB device is plugged into the host through a
        'USB 2.0 hub, as opposed to a direct connection to a root port on the PC.  In some cases the data rate may be slower
        'when the device is connected through a USB 2.0 hub.  This performance difference is believed to be caused by
        'the issue described in Microsoft knowledge base article 940021:
        'http://support.microsoft.com/kb/940021/en-us 

        'Higher effective bandwidth (up to the maximum offered by interrupt endpoints), both when connected
        'directly and through a USB 2.0 hub, can generally be achieved by queuing up multiple pending read and/or
        'write requests simultaneously.  This can be done when using	asynchronous I/O operations (calling ReadFile() and
        'WriteFile()	with overlapped structures).  The Microchip	HID USB Bootloader application uses asynchronous I/O
        'for some USB operations and the source code can be used	as an example.*/

        Dim OUTBuffer(65) As Byte       '//Allocate a memory buffer equal to the OUT endpoint size + 1
        Dim INBuffer(65) As Byte        '//Allocate a memory buffer equal to the IN endpoint size + 1
        Dim BytesWritten As UInteger = 0
        Dim BytesRead As UInteger = 0
        Dim ErrorStatus As UInteger


        While True
            Try

                If AttachedState = True Then
                    '//Get ANxx/POT Voltage value from the microcontroller firmware.  Note: some demo boards may not have a pot
                    '//on them.  In this case, the firmware may be configured to read an ANxx I/O pin voltage with the ADC
                    '//instead.  If this is the case, apply a proper voltage to the pin.  See the firmware for exact implementation.
                    OUTBuffer(0) = 0    '//The first byte is the "Report ID" and does not get sent over the USB bus, unless the HID report descriptor uses Report IDs.  Assume the HID report descriptor does not, then always set this byte to 0
                    OUTBuffer(1) = &H37 '//READ_POT command (see the firmware source code), gets 10-bit ADC Value
                    '//Initialize the rest of the 64-byte packet to "0xFF".  Binary '1' bits do not use as much power, and do not cause as much EMI
                    '//when they move across the USB cable.  USB traffic is "NRZI" encoded, where '1' bits do not cause the D+/D- signals to toggle states.
                    '//This initialization is not strictly necessary however.
                    For i As Integer = 2 To 65
                        OUTBuffer(i) = &HFF
                    Next

                    '//To get the ADCValue, first, we send a packet with our "READ_POT" command in it.
                    If WriteFile(WriteHandleToUSBDevice, OUTBuffer, 65, BytesWritten, IntPtr.Zero) Then
                        INBuffer(0) = 0
                        '//Now get the response packet from the firmware.

                        If ReadFileManagedBuffer(ReadHandleToUSBDevice, INBuffer, 65, BytesRead, IntPtr.Zero) Then
                            '//INBuffer[0] is the report ID, which we don't care about.
                            '//INBuffer[1] is an echo back of the command (see microcontroller firmware).
                            '//INBuffer[2] and INBuffer[3] contains the ADC value (see microcontroller firmware).  

                            If INBuffer(1) = &H37 Then
                                ADCValue = (CType(INBuffer(3), UInteger) << 8) + INBuffer(2)   '//Need to reformat the data from two unsigned chars into one unsigned int.
                            End If

                        End If
                    Else
                        ErrorStatus = Marshal.GetLastWin32Error()
                    End If

                    '//Get the pushbutton state from the microcontroller firmware.
                    OUTBuffer(0) = 0            '//The first byte is the "Report ID" and does not get sent over the USB bus.  Always set = 0.
                    OUTBuffer(1) = &H81     '//0x81 is the "Get Pushbutton State" command in the firmware

                    '//This loop is not strictly necessary.  Simply initializes unused bytes to 0xFF.
                    For i As Integer = 2 To 65
                        OUTBuffer(i) = &HFF '//0xFF for lower EMI and power consumption when driving the USB cable.
                    Next

                    If WriteFile(WriteHandleToUSBDevice, OUTBuffer, 65, BytesWritten, IntPtr.Zero) Then '//Blocking function, unless an "overlapped" structure is used	
                        '//Now get the response packet from the firmware.
                        INBuffer(0) = 0

                        If ReadFileManagedBuffer(ReadHandleToUSBDevice, INBuffer, 65, BytesRead, IntPtr.Zero) Then  '//Blocking function, unless an "overlapped" structure is used	
                            '//INBuffer[0] is the report ID, which we don't care about.
                            '//INBuffer[1] is an echo back of the command (see microcontroller firmware).
                            '//INBuffer[2] contains the I/O port pin value for the pushbutton (see microcontroller firmware). 
                            If ((INBuffer(1) = &H81) And (INBuffer(2) = &H1)) Then
                                PushbuttonPressed = False
                            End If
                            If (INBuffer(1) = &H81) And (INBuffer(2) = &H0) Then
                                PushbuttonPressed = True
                            End If
                        End If
                    End If

                    '//Check if this thread should send a Toggle LED(s) command to the firmware.  ToggleLEDsPending will get set
                    '//by the ToggleLEDs_btn click event handler function if the user presses the button on the form.
                    If ToggleLEDsPending = True Then
                        OUTBuffer(0) = 0                '//The first byte is the "Report ID" and does not get sent over the USB bus.  Always set = 0.
                        OUTBuffer(1) = &H80             '//0x80 is the "Toggle LED(s)" command in the firmware

                        '//This loop is not strictly necessary.  Simply initializes unused bytes to 0xFF.
                        For i As Integer = 2 To 65
                            OUTBuffer(i) = &HFF         '//0xFF for lower EMI and power consumption when driving the USB cable.
                        Next

                        '//Now send the packet to the USB firmware on the microcontroller
                        WriteFile(WriteHandleToUSBDevice, OUTBuffer, 65, BytesWritten, IntPtr.Zero) '//Blocking function, unless an "overlapped" structure is used
                        ToggleLEDsPending = False
                    End If

                Else
                    '//Add a small delay.  Otherwise, this while(true) loop can execute very fast and cause 
                    '//high CPU utilization, with no particular benefit to the application.
                    Thread.Sleep(2)
                End If


            Catch ex As Exception
                '//Exceptions can occur during the read or write operations.  For example,
                '//exceptions may occur if for instance the USB device is physically unplugged
                '//from the host while the above read/write functions are executing.

                '//Don't need to do anything special in this case.  The application will automatically
                '//re-establish communications based on the global AttachedState boolean variable used
                '//in conjunction with the WM_DEVICECHANGE messages to dyanmically respond to Plug and Play
                '//USB connection events.
            End Try


        End While


    End Sub


    '//--------------------------------------------------------------------------------------------------------------------------
    '//FUNCTION: ReadFileManagedBuffer()
    '//PURPOSE:	 Wrapper function to call ReadFile()
    '//
    '//INPUT:	 Uses managed versions of the same input parameters as ReadFile() uses.
    '//
    '//OUTPUT:	 Returns boolean indicating if the function call was successful or not.
    '//          Also returns data in the byte[] INBuffer, and the number of bytes read. 
    '//
    '//Notes:    Wrapper function used to call the ReadFile() function.  ReadFile() takes a pointer to an unmanaged buffer and deposits
    '//          the bytes read into the buffer.  However, can't pass a pointer to a managed buffer directly to ReadFile().
    '//          This ReadFileManagedBuffer() is a wrapper function to make it so application code can call ReadFile() easier
    '//          by specifying a managed buffer.
    '//--------------------------------------------------------------------------------------------------------------------------
    Private Function ReadFileManagedBuffer(ByVal hFile As SafeFileHandle, ByRef INBuffer() As Byte, ByVal nNumberOfBytesToRead As UInteger, ByRef lpNumberOfBytesRead As UInteger, ByVal lpOverlapped As IntPtr) As Boolean
        Dim pINBuffer As IntPtr = IntPtr.Zero

        Try
            pINBuffer = Marshal.AllocHGlobal(CType(nNumberOfBytesToRead, Integer))      '//Allocate some unmanged RAM for the receive data buffer.

            If ReadFile(hFile, pINBuffer, nNumberOfBytesToRead, lpNumberOfBytesRead, lpOverlapped) Then
                Marshal.Copy(pINBuffer, INBuffer, 0, lpNumberOfBytesRead)   '//Copy over the data from unmanged memory into the managed byte[] INBuffer
                Marshal.FreeHGlobal(pINBuffer)
                Return True
            Else
                Marshal.FreeHGlobal(pINBuffer)
                Return False
            End If

        Catch ex As Exception
            If pINBuffer <> IntPtr.Zero Then
                Marshal.FreeHGlobal(pINBuffer)
                Return False
            End If
        End Try
    End Function



End Class
