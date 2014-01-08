The MSVC++ 2005 source.zip file contains an older version of the HID
Bootloader PC application software.  For new designs, it is recommended
to use the Cross Platform version of the HID bootloader software instead.

The cross platform software is newer, contains some bugfixes for
known issues (with the MSVC++ 2005 source), is written with better programming
practices (less threads, better inline comments, easier to debug), and will
work on Windows/Mac/Linux based platforms.  The cross platform PC software
will also work with the newer bootloader enhacements implemented
starting with the MLA v2013-06 release (ex: flash signing, 
for robust software entry only into the bootloader, and the application 
version reading ability), while the "MSVC++ 2005 source.zip" does not have 
awareness of these newer features.  If you still use the MSVC++ program,
you must disable the "ENABLE_FLASH_SIGNATURE_VERIFICATION" #define option
(if present) in the HID bootloader firmware project, since the MSVC++ bootloader
software doesn't issue the flash signing command at the end of the 
erase/program/verify routine.

Therefore, it is recommended to use the cross platform software
instead, since it supports all bootloader features (such as flash signing)
and is otherwise superior.

Please see the folder:

[MLA install location]\USB\Device - Bootloaders\HID\software_cross_platform