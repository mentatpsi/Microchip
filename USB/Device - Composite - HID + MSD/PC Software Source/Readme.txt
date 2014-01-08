The "HID PnP Demo Composite HID+MSD Demo only (PID=0x0054).exe" executable program
was generated using the exact same source code as that used to create the
"HID PnP Demo.exe", with one exeception.

The HID PnP Demo.exe source code is found in the following directory (assuming default
installation directory):

C:\Microchip Solutions\USB Device - HID - Custom Demos\Generic HID - PnP Demo - PC Software\


To create the "HID PnP Demo Composite HID+MSD Demo only (PID=0x0054).exe" file, the 
"Generic HID - PnP Demo - PC Software" source code was compiled, with the MY_DEVICE_ID
definition (declared near the top of Form1.h) set to:

#define MY_DEVICE_ID  "Vid_04d8&Pid_0054"

(if using the C# source code, the VID/PID is set in Form1.cs, in the line that reads:
String DeviceIDToFind = "Vid_04d8&Pid_003f";
in the CheckIfPresentAndGetUSBDevicePath() function)


The "GenericHIDSimpleDemo Composite HID+MSD Demo only (PID=0x0054).exe" executable
program was generated  using the exact same source code as that used to create the
"GenericHIDSimpleDemo.exe", with one exeception.

The GenericHIDSimpleDemo.exe source code is found in the following directory (assuming default
installation directory):

C:\Microchip Solutions\USB Device - HID - Custom Demos\Generic HID - Simple Demo - PC Software


To create the "GenericHIDSimpleDemo Composite HID+MSD Demo only (PID=0x0054).exe" file, the 
"Generic HID - Simple Demo - PC Software" source code was compiled, with the MY_DEVICE_ID
definition (declared near the top of Form1.h) set to:

#define MY_DEVICE_ID  "Vid_04d8&Pid_0054"