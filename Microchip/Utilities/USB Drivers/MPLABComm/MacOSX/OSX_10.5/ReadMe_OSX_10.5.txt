ReadMe_OSX_10.5.txt
_____________________________________________________________________________

Dir:              File:                       Permissions:
---------------   --------------------------  -------------
/usr/lib/         libUSBAccessLink.dylib      -rwxr-xr-x

/usr/lib/java/    libUSBAccessLink.dylib      -rwxr-xr-x

/etc/.mplab_ide/  mchpdefport                 -rw-r--r--

-----------------------------------------------------------

If MPLAB X is NOT installed:
Dir:              File:                       Permissions:
---------------   --------------------------  -------------
/etc/.mplab_ide/  mchpmacusbdevice            -rwxr-xr-x

/usr/lib/         libusb-1.0.0.dylib          -rwxr-xr-x
                  libusb-1.0.a                -rwxr-xr-x
                  libusb-1.0.la               -rwxr-xr-x
                  
Create soft link as follows:
$ cd /usr/lib
$ sudo ln -s /usr/lib/libusb-1.0.0.dylib libusb-1.0.dylib                  

-----------------------------------------------------------

If MPLAB X IS installed, libusb files will have been placed
and soft links established appropriately (similar to those
found on Linux systems).

