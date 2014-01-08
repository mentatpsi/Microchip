When used with Microsoft Windows based operating system, this demo relies on the 
usbccid.sys driver.  The usbccid.sys driver is a standard CCID USB driver created 
by Microsoft.  Although Microsoft provides and distributes this driver, this 
particular driver is not natively included with all OS versions.

Therefore, upon plugging in the USB device for the first time, the Found New Hardware
Wizard may appear, and you may be prompted to install a driver for the device.

If this Wizard appears, make sure to use the "Windows Update" option.  For example,
in Windows XP, the Found New Hardware Wizard should provide the following options:

Can Windows connect to Windows Update to search for software?

(radio button): Yes, this time only
(radio button): Yes, now and every time I connect a device
(radio button): No, not this time

Make sure to select one of the two "Yes" options.  Windows should then use your internet
connection to download and install the latest version of the usbccid driver, for your 
operating system version.



If the Found New Hardware Wizard does not appear, or it did appear but was cancelled before
successfully installing the driver, the driver can still be installed by:

1.  Open the Device Manager
2.  Right click on the USB smart card reader device, and select the "Update Driver..." option.

When prompted by the Hardware Update Wizard, make sure to allow Windows to connect to Windows Update
to search for the driver.  After selecting this option, allow it to search automatically, and it
should find the driver and successfully install.

