Question: What does "Super Device Manager.bat" do?

Answer:
It sets an environment variable, and then launches the standard Windows Device Manager.

Ordinarily, the Windows Device Manager will not show devices unless they are currently
plugged into a machine.  Only devices that are present are visible.  However, it is often
desirable to "see" non-present devices, so that you can manually delete the "Plug and Play Device Node"
(which is a set of registry entries associated with a plug and play device).

It is possible to make non-present devices visible in the device manager, by setting an
environment variable and launching the device manager.

To use the "Super Device Manager.bat" to view non-present device in the device manager, 
do the following:

1.  Log onto the machine as an administrator.

2.  If using Windows XP, just double click on the .bat file.
    If using Windows Vista or Windows 7, right click on the .bat
    file and select the "Run as Administrator" option.  Failure to do this
    will prevent it from working (a "normal" device manager will open instead).

3.  Once the device manager is open, click the "View-->Show Hidden Devices"
    menu option.

4.  Non-present devices should now be visible in the device manager window.
    Non-present devices will appear to be "greyed out".

5.  You may now delete the non-present devices.  This will remove the plug and play
    device node (registry entries) for that device.  This can free up some resources,
    such as the COMx number that may have been allocated to that device if the device 
    was a virtual COM port.
