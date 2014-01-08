//
// NOTE: You may delete this file. It is used to generate the mTouch documentation 
//       located in the /Microchip/Help folder of the Microchip Application Libraries.
//



/** @file   mTouch_documentation.h
*   @brief  Internal framework header file used to generate this documentation. Safe to delete.
*/
/**
* @mainpage
*
* @section Intro Introduction
* Thank you for downloading the mTouch Framework v2.3, part of the mTouch Library v1.40.02 package in the MLA.
*
* The mTouch Framework is a software package enabling designers to easily integrate touch technologies
* to their application. It combines high sensitivity with conducted and radiated noise immunity.
*
* Simple configuration file allows for easy setup. Microchip also offers full source code access for more
* advanced touch designers.
*
* <br><center>
* <table border="0">
* <tr valign="top">
* <td align="left">@subpage GettingStarted
*     @li @ref featBasic            "Basic mTouch Sensor Configuration"
*     @li @ref GSGuides             "Quick Start Guides"
*     @li New mTouch Project: @ref GettingStartedNewP8 "MPLAB 8" or @ref GettingStartedNewPX "MPLAB X"
*     @li @ref GettingStartedEval   "mTouch Evaluation Board Projects"
*     @li @ref GettingStartedCustom "Integrating with Custom Hardware"</td>
* <td>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</td>
* <td align="left">@subpage FrameworkFeatures "mTouch Framework Features"
*     @li @ref featBasic            "Keys / Buttons"
*     @li @ref featSliders          "Sliders and Wheels"
*     @li @ref featProximity        "Proximity Sensors"
*     @li @ref featMatrix           "Matrix Keypad Layouts"
*     @li @ref featMostPressed      "Most-Pressed Algorithm"
*     @li @ref featGuard            "Guard Rings / Shields"
*     @li @ref featMutual           "Mutual Capacitance"
*     @li @ref featMode             "Scanning Modes"</td>
* </tr>
* <tr valign="top">
* <td align="left">@subpage optimizing "Optimizing Performance"
*     @li @ref ts-Sensitivity       "Increasing Sensitivity"
*     @li @ref ts-ResponseTime      "Faster Response Times"
*     @li @ref ts-Waveform          "mTouch CVD Waveform Tuning"</td>
* <td>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</td>
* <td align="left">@subpage ResourceRequirements "Resource Requirements"
*     @li @ref PICSupport           "Supported PIC&reg; Microcontrollers"
*     @li @ref rrModules            "Hardware Module Usage"
*     @li @ref rrMemory             "ROM, RAM, and Processing Requirements"
*     @li @subpage RequiredIncludes</td>
* </tr>
* <tr valign="top">
* <td align="left">@subpage troubleshoot "Troubleshooting"
*     @li @ref ts-Comms   "PC Communications"
*     @li @ref BCPGUI     "Backwards-compatible UART for prev. Profilab GUI"</td>
* <td>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</td>
* <td align="left">@subpage SoftwareLicense<br><br>
* @subpage Changelog "Version Change Log"</td>
* </tr>
* </table></center>
*
* <br><br>
*
* @image html filetree.jpg "mTouch Framework File Tree"
*
* <ul>
* <li> <b>/Microchip</b><br>
* Contains the library, read-only source files for the mTouch Framework.
*   <ul>
*   <li> <b>/Help</b><br>
*       Contains documentation for all the Microchip Application Libraries.
*       <ul>
*       <li> <b>/mTouch CVD Help Content</b><br>
*       Contains the documentation for the mTouch Framework.
*       </ul>
*   <li> <b>/mTouchCap</b>
*       <ul>
*       <li> <b>/PIC12F PIC16F Library</b><br>
*       Contains the C source (.c) and C header (.h) files for the mTouch Framework.<br>
*       <b>IMPORTANT</b>: The files in this directory should never be directly editted. Copy to a local application folder.
*           <ul>
*           <li> mTouch Source Files
*               <ul>
*               <li> main.c contains an example application implementation.
*               <li> mTouch.c implements the basic initialization, filtering, and decoding functions for the framework.
*               <li> mTouch_acquisition.c implements a custom mTouch Acquisition method specific to the PIC and configuration.
*               <li> mTouch_slider.c implements the slider and wheel decoding, if enabled.
*               <li> mTouch_proximity.c implements the proximity sensor decoding and state machine, if enabled.
*               <li> mTouch_eeprom.c implements the mTouch EEPROM mapping and read/write functions, if enabled.
*               </ul>
*           <li> mTouch Configuration Files<br>
*               These are the files that should be tweaked for every application.
*               <ul>
*               <li> mTouch_config.h stores the configuration options for the framework.
*               <li> mTouch_config_modes.h stores the configuration options for scanning modes.
*               <li> mTouch_config_slider.h stores the configuration options for sliders and wheels.
*               <li> mTouch_config_cvdAdvanced.h stores the advanced CVD acquisition options.
*               </ul>
*           <li> mTouch Header Files<br>
*               <ul>
*               <li> mTouch.h <b>is the only mTouch file you need to \#include to use the API in your project.</b> 
*                           This is the master header file for the mTouch Framework that defines global variables and 
*                           includes other files based on the current configuration. 
*               <li> mTouch_slider.h is the internal mTouch header file for generating the mTouch Framework's slider and wheel logic
*               <li> mTouch_proximity.h is the internal mTouch header file for generating the mTouch Framework's proximity sensor logic
*               <li> mTouch_eeprom.h is the internal mTouch header file for generating the mTouch Framework's EEPROM logic
*               <li> mTouch_modes.h is the internal mTouch header file for generating the mTouch Framework's scanning mode logic
*               <li> mTouch_optionsForConfig.h contains pre-defined labels for easier user configuration. Do not edit.
*               <li> mTouch_macroLibrary_common.h contains many of the core-non-specific macros for the mTouch Framework.
*               <li> mTouch_processConfiguration.h performs much of the pre-compiling work to confirm a valid configuration 
*                           and then set up the flags and variables to aid the compiler in generating a custom mTouch application.
*               <li> mTouchCVD_macroLibrary_PIC16F.h is one of several PIC-core-specific header files that contains macros 
*                           for generating the acquisition module of the mTouch Framework.
*               <li> mTouchCVD_macroLibrary_PIC16F1.h is one of several PIC-core-specific header files that contains macros 
*                           for generating the acquisition module of the mTouch Framework.
*               <li> mTouchCVD_macroLibrary_PIC18F.h is one of several PIC-core-specific header files that contains macros 
*                           for generating the acquisition module of the mTouch Framework.
*               <li> generic_processorConfigBits.h contains predefined config fuse statements for faster development.
*               </ul><br>
*           <li> mComm Source Files<br>
*               These files implement the mTouch communications module called 'mComm'.
*               <ul>
*               <li> mComm.c contains the basic logic for all supported communication types
*               <li> mComm_opcodes.c implements the read/write actions for each opcode
*               <li> mComm_custom.c implements a custom opcode and should be referenced when creating your own custom behavior.
*               </ul>
*           <li> mComm Header Files<br>
*               <ul>
*               <li> mComm.h is the main header file for the module. Automatically included by mTouch.h.
*               <li> mComm_config.h stores all the configuration options for the mComm module.
*               <li> mComm_optionsForConfig.h contains pre-defined labels for easier user configuration. Do not edit.
*               <li> mComm_processConfiguration.h performs much of the pre-compiling work to confirm a valid configuration and 
*                           then set up the flags and variables to aid the compiler in generating a custom mComm application.
*               </ul>
*           <li> <b>Alternative Configurations/</b>
*               <ul>
*               <li>mTouch_HardwareProfile_12F61x.h is one of many PIC-family-specific header files that contains information 
*                           such available hardware modules, analog channel-to-port mapping, and default register configurations.
*               <li>See <tt>Your MLA Directory/Microchip/mTouchCap/PIC12F PIC16F CVD Library/Alternative Configurations/</tt> 
*                           for all available processor configuration files.
*               </ul>
*           </ul>
*       </ul>
*   </ul>
* <li> <b>/mTouchCapDemos</b>
*   <ul>
*   <li> <b>/PIC16F_CVD_Demos</b><br>
*       Provides some example, pre-configured projects for various PIC microcontrollers and setups.
*       <ul>
*       <li> <b>/02-02091-R2  CSM-CVD Eval Board</b><br>
*           Example project implementing CVD on the mTouch CSM-CVD evaluation board.
*           <ul>
*           <li> <b>/2-Ch Slider</b><br>
*               Implements a 2-channel slider connected to daughter board pin 0 and 1
*           <li> <b>/4-Ch Slider</b><br>
*               Implements a 4-channel slider connected to daughter board pin 0-3
*           <li> <b>/8 Buttons</b><br>
*               Implements an 8-button daughter board connected to pins 0-7
*           <li> <b>/12-Key Matrix</b><br>
*               Implements a 12-key 3x4 matrix daughter board on pins 0-6
*           <li> <b>/Metal-over-Capacitive Daughter Board (AC183026)</b><br>
*               Implements the 4-sensor MoC daught board connected to pins 2-5
*           </ul>
*       <li> <b>/02-02091-R3  CVD Eval Board</b><br>
*           Example project implementing CVD on the mTouch CVD evaluation board.
*           <ul>
*           <li> <b>/2-Ch Slider</b><br>
*               Implements a 2-channel slider connected to daughter board pin 0 and 1
*           <li> <b>/4-Ch Slider</b><br>
*               Implements a 4-channel slider connected to daughter board pin 0-3
*           <li> <b>/8 Buttons</b><br>
*               Implements an 8-button daughter board connected to pins 0-7
*           <li> <b>/12-Key Matrix</b><br>
*               Implements a 12-key 3x4 matrix daughter board on pins 0-6
*           <li> <b>/Metal-over-Capacitive Daughter Board (AC183026)</b><br>
*               Implements the 4-sensor MoC daught board connected to pins 2-5
*           </ul>
*       <li> <b>/02-02091-RA  CSM Eval Board</b><br>
*           Example project implementing CVD on the mTouch CSM evaluation board.
*           <ul>
*           <li> <b>/2-Ch Slider</b><br>
*               Implements a 2-channel slider connected to daughter board pin 0 and 1
*           <li> <b>/4-Ch Slider</b><br>
*               Implements a 4-channel slider connected to daughter board pin 0-3
*           <li> <b>/8 Buttons</b><br>
*               Implements an 8-button daughter board connected to pins 0-7
*           <li> <b>/12-Key Matrix</b><br>
*               Implements a 12-key 3x4 matrix daughter board on pins 0-6
*           <li> <b>/Metal-over-Capacitive Daughter Board (AC183026)</b><br>
*               Implements the 4-sensor MoC daught board connected to pins 2-5
*           </ul>
*       <li> <b>/233-04-1008 Rev F  CSM Eval Board</b><br>
*           Example project implementing CVD on the mTouch CSM evaluation board.
*           <ul>
*           <li> <b>/2-Ch Slider</b><br>
*               Implements a 2-channel slider connected to daughter board pin 0 and 1
*           <li> <b>/4-Ch Slider</b><br>
*               Implements a 4-channel slider connected to daughter board pin 0-3
*           <li> <b>/8 Buttons</b><br>
*               Implements an 8-button daughter board connected to pins 0-7
*           <li> <b>/12-Key Matrix</b><br>
*               Implements a 12-key 3x4 matrix daughter board on pins 0-6
*           <li> <b>/Metal-over-Capacitive Daughter Board (AC183026)</b><br>
*               Implements the 4-sensor MoC daught board connected to pins 2-5
*           </ul>
*       <li> <b>/PIC16F615 - 1 sensor with UART</b><br>
*           Example project implementing oen CVD sensor on PIC12F615 with one-way UART communications out
*       <li> <b>/PIC16F615 - 2 sensors</b><br>
*           Example project implementing two CVD sensors on PIC12F615
*       <li> <b>/PIC16F1527 - 30 sensors</b><br>
*           Example project implementing the maximum number of supported sensors (30) on the PIC16F1527
*       </ul>
*   </ul>
* </ul>
*
*/
/**
* @page Changelog Version Change Log
*
* @section cl020101 Version 2.2
* @li   Bug Fixes:
*   <ul>
*   <li>Sensors' oversampling counter variable now only updates if a scan occurred.
*   <li>Unique oversampling values enabled for only one sensor is now gracefully handled without throwing errors.
*   <li>I2C slave no longer occasionally fails to execute write operations.
*   </ul>
* @li   Added support for hardware CVD handling
* @li   Demo main.c files now have a few additional #if blocks to reduce compile-time errors when minor configuration changes are made.
* @li   Solved a compile-time-error bug with PIC16F72x devices when running at 16MHz
* @li   Added device support:
*   <ul>
*   <li>PIC16(L)F1501/3
*   <li>PIC16(L)F1512/3
*   <li>PIC12(L)F1552
*   </ul>
*
* @section cl0201 Version 2.1
* @li   Enabling a non-TMR0 timer will now also automatically enable the PEIE bit for automatic interrupt use (mTouch.c)
* @li   Default one-way communications now supports the updated Profilab GUI supplied with this framework package (mComm_config.h / mComm.c)
* @li   Configuration values can now be stored in RAM (mTouch_configOptions) instead of EEPROM for use in the two-way communications. See MTOUCH_EEPROM_STORED_AS_RAM in mTouch_config.h
* @li   Individual the oversampling array initialization has been changed to automatically adjust its size based on MTOUCH_NUMBER_SENSORS (mTouch_acquisition.c)
* @li   When mTouch controls the ISR, it will now correctly exit from the function if the mTouch timer was not responsible for the interrupt (mTouch_acquisition.c)
* @li   Fixed an issue with the DAC configuration in the hardware profiles of the 182x and 184x families
* @li   Fixed a bug with the initial EEPROM byte check - previously the EEPROM would revert to factory settings on reset. (mTouch_eeprom.c)
* @li   Fixed the mTouch_ClearToggle(i) API macro: Logical 'not' (<b>!</b>) changed to bit-wise 'not' (<b>~</b>).
*
* @section cl0200 Version 2.0
* @li   Added support for a large number of processors. Refer to this documentations '@ref PICSupport "Resource Requirements"' 
*       page for a full listing.
* @li   The acquisition module has been reworked for added efficiency. 
*   <ul>
*   <li>The assembly-coded jump table has been replaced with a compiler-generated look-up table.
*   <li>The 'scan a' and 'scan b' portions of each sensor's mTouch waveform are dedicated functions that are dynamically 
*       generated at compile-time by the preprocessor macros. This eliminates the previous limitation on the number of sensors 
*       due to the rising length of the single mTouch acquisition function.
*   <li>Individual oversampling values can be enabled from the mTouch_config_cvdAdvanced.h file.
*   </ul>
* @li   Structures have been implemented for a large number of mTouch variables to maximize the efficiency of RAM resources. 
*       Several previously supported processors are now able to implement more sensors due to this change.
* @li   New features:
*   <ul>
*   <li>@ref featMode "Scanning modes" allow different sensor configurations to be scanned based on the application's state.
*   <li>Built-in @ref featProximity "proximity sensor" support with median filter. (mTouch_config.h)
*   <li>@ref featSliders "Sliders and wheels" can be implemented by editing the mTouch_config_slider.h options. Supports up 
*       to 10 sliders with 8 sensors per slider.
*   <li>A @ref featMostPressed "most-pressed algorithm" can be implemented by uncommenting #MTOUCH_MOST_PRESSED_ONLY. 
*       (mTouch_config.h)
*   <li>Toggle mode is now available by uncommenting #MTOUCH_TOGGLE_ENABLED. (mTouch_config.h) It will toggle the output's 
*       value only when it detects a new press - not when it sees a release. Example API available in main.c. 
*   <li>@ref featGuard "Guard rings/shields" can be implemented using any general IO pin or through the DACOUT pin. 
*       (mTouch_config_cvdAdvanced.h)
*   <li>Support for @ref featMatrix "matrix configurations" can be implemented to automatically decode the rows and columns 
*       with debouncing and a dedicated most-pressed algorithm to ensure quality coordinate results. (mTouch_config.h)
*   <li>@ref featMutual "Mutual coupling drives" can be implemented to detect increased coupling between two interconnected 
*       sensors. (mTouch_config_cvdAdvanced.h)
*   <li>mComm communications module added. See mComm_config.h
*       <ul>
*       <li>Implements I2C, SPI, and UART two-way communications to allow changing of configuration values at run-time. 
*       <li>Configuration options and thresholds are stored in EEPROM when this is implemented by defining 
*           #MTOUCH_EEPROM_ENABLED in mTouch_config.h
*       <li>Implements 1-way UART communications in hardware or by using a software-implementation.
*       </ul>
*   </ul>
* @li New configuration options:
*   <ul>
*   <li>#MTOUCH_INTEGRATION_TYPE adjusts the way the framework integrates with the application (mTouch_config.h)
*   <li>#MTOUCH_ISR_TIMER changes which 8-bit timer is used for the mTouch interrupt (mTouch_config.h)
*   <li>#MTOUCH_SCAN_FUNCTIONALITY alternates between scanning one sensor per interrupt or scanning all sensors once per 
*       interrupt (mTouch_config.h)
*   <li>#MTOUCH_ADC_CONTROL determines whether the framework releases the ADC after performing its scan (mTouch_config.h)
*   <li>#APFCON_INITIALIZED can be used to disable "make sure you initialize" warnings on parts with APFCON registers (mTouch_config.h)
*   <li>#MTOUCH_DECIMATION_MAX_STEP adjusts the maximum step size for sensors while going through the decimation filter 
*       (mTouch_config.h)
*   <li>#MTOUCH_JITTER_BITS adjusts the number of bits used to implement the random seed value for jittering (mTouch_config.h)
*   <li>#CVD_FORCE_REF_DAC can be defined to force the CVD acquisition to use the DAC as the reference for the ADC's hold 
*       capacitor (mTouch_config_cvdAdvanced.h)
*   </ul>
* @li Configuration option #CVD_JITTER_ENABLE has been removed. To disable jittering, set the #MTOUCH_JITTER_BITS value to 0.
* @li #MTOUCH_SCALING has been adjusted to allow a larger number of scaling options. (mTouch_config.h)
*
* @section cl0101 Version 1.1
* @li   Non-enhanced Core Device Support
* @li   HI-TECH 9.81 Lite and Standard Compiler Support
* @li   Acquisition variable array #sensor_data is now initialized to 0x47F to reduce the required initialization time
* @li   New configuration option #CVD_SETTLING_DELAY adjusts the amount of time the CVD acquisition will wait for the voltages on 
*       the external sensor and internal ADC hold capacitor to settle. See @ref ts-Waveform "this guide" for more information.
* @li   New configuration option #CVD_CHOLD_CHARGE_DELAY adjusts the amount of time the CVD acquisition will wait for the voltage 
*       on the internal ADC hold capacitor to reach steady state (V<sub>DD</sub> or V<sub>SS</sub>) before connecting the two capacitors and allowing 
*       their voltages to settle. See @ref ts-Waveform "this guide" for more information.
* @li   The scan order of the CVD acquisition has been reversed to ensure maximum sensitivity for larger external sensor 
*       capacitances. This means the waveform will change when viewed on a scope. See @ref ts-Waveform "this guide" for more 
*       information.
* @li   #GENERATE_STORAGE_FUNCTION() and #GENERATE_JUMP_TABLE() macros have been redesigned with a more elegant implementation.
* @li   The mTouch.h file has been split-in-two in order to accomodate the Microchip Application Libraries' directory structure. 
*       A local copy of mTouch.h should be included in your application's includes directory. mTouch_processConfiguration.h, the 
*       other half of the original mTouch.h file, only needs to be available in the library folders and does not require a local 
*       copy.
* @li   Sensor 0's reference is now defined in the process configuration header file to help de-clutter the acquisition.c file.
* @li   Many documentation updates including:
*   <ul>
*   <li> New 'Optimizing Performance' sections designed to make it easier to solve common problems:
*       <ul>
*       <li> @ref ts-Sensitivity    "Increasing Sensitivity"
*       <li> @ref ts-ResponseTime   "Adjusting the Response Time"
*       <li> @ref ts-Waveform       "Tuning the CVD Waveform"
*       <li> @ref ts-Comms          "Establishing PC Communications"
*       </ul>
*   <li> Redesigned main page to help new users find information
*   <li> Guides to creating new projects in @ref GettingStartedNewP8 "MPLAB 8" and @ref GettingStartedNewPX "MPLAB X"
*   </ul>
*
* @section cl0100 Version 1.0 - First Public Release
* @li 64-Pin Device Support
* @li HI-TECH 9.81 PRO Compiler Support
* @li Comm/HardwareProfile update to support devices with >1 UART modules
* @li New MTOUCH_COMM_ASCII_READING and MTOUCH_COMM_ASCII_BASELINE configuration options to quickly adjust comm output
*/
/**
* @page ResourceRequirements Supported PIC&reg; MCUs and Requirements
*
* @section rrCompiler Required Compiler
* @li <b>HI-TECH C Compiler for PIC10/12/16 MCUs v9.81 or later</b><br>
* If a non-PRO license is used to compile the framework, the maximum number of supported sensors will decrease. Noise 
* testing was performed using the PRO license.<br>
* Using a third-party compiler may result in unknown behavior due to the heavy use of compile-time code formatting.<br>
*
* @section rrModules Module Requirements
* @li <b>8-bit Hardware Timer Module for Interrupt</b><br>
* This module is used to schedule regular ADC scans on the sensors. 8-bit timers normally interrupt after 256 counts; however, the mTouch
* framework pre-loads the timer's register with a pseudo-random seed value to slightly jitter the sampling rate. This means that the
* average number of counts before it interrupts is 224.<br>
* You control the timer's prescaler and period registers.<br>
* NOTE: No timer is required if #MTOUCH_INTEGRATION_TYPE is defined as MTOUCH_CALLED_FROM_MAINLOOP.
*
* @li <b>Analog/Digital Converter</b><br>
* The ADC is the core module used by the mTouch framework to acquire a sensor reading. If you wish to perform an ADC reading in 
* between mTouch scans you will need to set the #MTOUCH_ADC_CONTROL configuration option to MTOUCH_RELEASES_ADC_AFTER_SCAN.
* The ADC is initialized for the framework by the mTouch_Init() function.<br>
*
* @section rrMemory Memory/Processing Requirements
* Roughly, the amount of time that will be spent in the mTouch ISR is based on the TMR0 prescaler value.
* 
* <center>
* <table>
* <tr><td><b>TMR0 Prescaler</b></td><td><b>mTouch ISR Processing Duty Cycle</b></td></tr>
* <tr><td align="center">1</td><td align="center"><i>Invalid</i></td></tr>
* <tr><td align="center">2</td><td align="center">40%</td></tr>
* <tr><td align="center">4</td><td align="center">20%</td></tr>
* <tr><td align="center">8</td><td align="center">10%</td></tr>
* </table>
* </center>
*
* The program memory and RAM requirements are determined mainly by the number of sensors.
* <center>
* <table border="0">
* <tr valign="top">
* <td><center><b>Code Size / Program Memory (words)</b></center></td>
* <td><center><b>Data Space / RAM (bytes)</b></center></td>
* </tr>
* <tr valign="top">
* <td>
*     <table>
*         <tr><td>&nbsp;</td><td colspan="2" align="center"><b>Standard Midrange</b></td><td colspan="2" align="center"><b>Enhanced Midrange</b></td><td colspan="2" align="center"><b>Enhanced Midrange<br>w/Hardware CVD</b></td></tr>
*         <tr><td># Sensors</td><td align="center"><b>PRO</b></td><td align="center"><b>LITE</b></td><td align="center"><b>PRO</b></td><td align="center"><b>LITE</b></td><td align="center"><b>PRO</b></td><td align="center"><b>LITE</b></td></tr>
*         <tr><td align="center">1</td><td align="center">653</td><td align="center">885</td><td align="center">648</td><td align="center">884</td><td align="center">587</td><td align="center">813</td></tr>
*         <tr><td align="center">2</td><td align="center">961</td><td align="center">1367</td><td align="center">937</td><td align="center">1387</td><td align="center">814</td><td align="center">1291</td></tr>
*         <tr><td align="center">3</td><td align="center">1025</td><td align="center">1432</td><td align="center">1032</td><td align="center">1483</td><td align="center">819</td><td align="center">1297</td></tr>
*         <tr><td align="center">4</td><td align="center">1089</td><td align="center">1496</td><td align="center">1112</td><td align="center">1563</td><td align="center">824</td><td align="center">1302</td></tr>
*         <tr><td align="center">5</td><td align="center">1153</td><td align="center">1563</td><td align="center">1192</td><td align="center">1643</td><td align="center">829</td><td align="center">1307</td></tr>
*         <tr><td align="center">6</td><td align="center">1221</td><td align="center">1628</td><td align="center">1272</td><td align="center">1731</td><td align="center">842</td><td align="center">1320</td></tr>
*         <tr><td align="center"><b>+1</b></td><td align="center"><b>+68</b></td><td align="center"><b>+65</b></td><td align="center"><b>+80</b></td><td align="center"><b>+88</b></td><td align="center"><b>+5</b></td><td align="center"><b>+5</b></td></tr>
*         <tr><td align="left">Most Pressed</td><td align="center">+250</td><td align="center">+350</td><td align="center">+240</td><td align="center">+340</td><td align="center">+240</td><td align="center">+340</td></tr>
*         <tr><td align="left">Matrix</td><td align="center">+450</td><td align="center">+730</td><td align="center">+430</td><td align="center">+640</td><td align="center">+430</td><td align="center">+640</td></tr>
*         <tr><td align="left">Slider</td><td align="center">+480</td><td align="center">+750</td><td align="center">+460</td><td align="center">+680</td><td align="center">+460</td><td align="center">+680</td></tr>
*         <tr><td align="left">Proximity</td><td align="center">+450</td><td align="center">+1100</td><td align="center">+420</td><td align="center">+740</td><td align="center">+420</td><td align="center">+740</td></tr>
*     </table>
* </td>
* <td>
*     <table>
*         <tr><td>&nbsp;</td><td colspan="2" align="center"><b>Standard Midrange</b></td><td colspan="2" align="center"><b>Enhanced Midrange</b></td></tr>
*         <tr><td># Sensors</td><td align="center"><b>PRO</b></td><td align="center"><b>LITE</b></td><td align="center"><b>PRO</b></td><td align="center"><b>LITE</b></td></tr>
*         <tr><td align="center">1</td><td align="center">44</td><td align="center">49</td><td align="center">36</td><td align="center">43</td></tr>
*         <tr><td align="center">2</td><td align="center">57</td><td align="center">70</td><td align="center">50</td><td align="center">60</td></tr>
*         <tr><td align="center">3</td><td align="center">68</td><td align="center">81</td><td align="center">61</td><td align="center">71</td></tr>
*         <tr><td align="center">4</td><td align="center">79</td><td align="center">92</td><td align="center">72</td><td align="center">82</td></tr>
*         <tr><td align="center">5</td><td align="center">90</td><td align="center">103</td><td align="center">83</td><td align="center">93</td></tr>
*         <tr><td align="center">6</td><td align="center">101</td><td align="center">114</td><td align="center">94</td><td align="center">104</td></tr>
*         <tr><td align="center"><b>+1</b></td><td align="center"><b>+11</b></td><td align="center"><b>+11</b></td><td align="center"><b>+11</b></td><td align="center"><b>+11</b></td></tr>
*         <tr><td align="left">Most Pressed</td><td align="center">+17</td><td align="center">+15</td><td align="center">+10</td><td align="center">+11</td></tr>
*         <tr><td align="left">Matrix</td><td align="center">+31</td><td align="center">+30</td><td align="center">+24</td><td align="center">+25</td></tr>
*         <tr><td align="left">Slider</td><td align="center">+29</td><td align="center">+24</td><td align="center">+24</td><td align="center">+22</td></tr>
*         <tr><td align="left">Proximity</td><td align="center">+28</td><td align="center">+33</td><td align="center">+26</td><td align="center">+27</td></tr>
*     </table>
* </td>
* </tr>
* </table>
* </center>
*
*
* @section PICSupport Supported PIC&reg; Microcontrollers
* @li <b>PIC12/16F61x Family</b>
* <ul>
* <li>PIC12F/HV615      :: 2 sensor limit (Lite: 1 sensor limit)
* <li>PIC12F617         :: All 4 sensors supported
* <li>PIC16F/HV616      :: All 8 sensors supported
* </ul>
* @li <b>PIC12/16F68x Family</b>
* <ul>
* <li>PIC12F683         :: All 4 sensors supported
* <li>PIC16F677         :: 8 sensor limit
* <li>PIC16F684         :: All 8 sensors supported
* <li>PIC16F685         :: All 12 sensors supported
* <li>PIC16F687         :: 8 sensor limit
* <li>PIC16F688         :: All 8 sensors supported
* <li>PIC16F689         :: All 12 sensors supported
* <li>PIC16F690         :: All 12 sensors supported
* </ul>
* @li <b>PIC16F72x Family</b>
* <ul>
* <li>PIC16F/LF722      :: 7 sensor limit
* <li>PIC16F/LF722A     :: 8 sensor limit
* <li>PIC16F/LF723      :: All 11 sensors supported
* <li>PIC16F/LF723A     :: All 11 sensors supported
* <li>PIC16F/LF724      :: All 11 sensors supported
* <li>PIC16F/LF726      :: All 11 sensors supported
* <li>PIC16F/LF727      :: All 11 sensors supported
* </ul>
* @li <b>PIC16F78x Family</b>
* <ul>
* <li>PIC16F/HV785      :: 9 sensor limit
* </ul>
* @li <b>PIC16F150x Family</b>
* <ul>
* <li>PIC12(L)F1501     :: 2 sensor limit (Lite: 1 sensor limit)
* <li>PIC16(L)F1503     :: All 8 sensors supported
* <li>PIC16F/LF1507     :: All 12 sensors supported
* <li>PIC16F/LF1509     :: All 12 sensors supported
* </ul>
* @li <b>PIC16F151x Family</b>
* <ul>
* <li>PIC16F/LF1512 (w/HCVD)    :: 7 sensor limit
* <li>PIC16F/LF1513 (w/HCVD)    :: 15 sensor limit
* <li>PIC16F/LF1516             :: All 17 sensors supported
* <li>PIC16F/LF1518             :: All 17 sensors supported
* <li>PIC16F/LF1517             :: All 28 sensors supported
* <li>PIC16F/LF1519             :: All 28 sensors supported
* </ul>
* @li <b>PIC16F152x Family</b>
* <ul>
* <li>PIC16F/LF1526     :: All 30 sensors supported
* <li>PIC16F/LF1527     :: All 30 sensors supported
* <li><i>This family does not support single-sensor applications.</i>
* </ul>
* @li <b>PIC12LF155x Family</b>
* <ul>
* <li>PIC12LF1552 (w/HCVD)      :: All 4 sensors supported
* </ul>
* @li <b>PIC16F182x Family</b>
* <ul>
* <li>PIC12F/LF1822     :: All 4 sensors supported
* <li>PIC16F/LF1823     :: All 8 sensors supported
* <li>PIC16F/LF1824     :: All 8 sensors supported
* <li>PIC16F/LF1825     :: All 8 sensors supported
* <li>PIC16F/LF1826     :: All 12 sensors supported
* <li>PIC16F/LF1827     :: All 12 sensors supported
* <li>PIC16F/LF1828     :: All 12 sensors supported
* <li>PIC16F/LF1829     :: All 12 sensors supported
* </ul>
* @li <b>PIC16F184x Family</b>
* <ul>
* <li>PIC12F/LF1840     :: All 4 sensors supported
* <li>PIC16F/LF1847     :: All 12 sensors supported
* </ul>
* @li <b>PIC16LF190x Family</b>
* <ul>
* <li>PIC16LF1902       :: 8 sensor limit
* <li>PIC16LF1903       :: All 11 sensors supported
* <li>PIC16LF1906       :: All 11 sensors supported
* <li>PIC16LF1904       :: All 14 sensors supported
* <li>PIC16LF1907       :: All 14 sensors supported
* </ul>
* @li <b>PIC16F193x Family</b>
* <ul>
* <li>PIC16F/LF1933     :: All 11 sensors supported
* <li>PIC16F/LF1936     :: All 11 sensors supported
* <li>PIC16F/LF1938     :: All 11 sensors supported
* <li>PIC16F/LF1934     :: All 14 sensors supported
* <li>PIC16F/LF1937     :: All 14 sensors supported
* <li>PIC16F/LF1939     :: All 14 sensors supported
* </ul>
* @li <b>PIC16F194x Family</b>
* <ul>
* <li>PIC16F/LF1946     :: All 17 sensors supported
* <li>PIC16F/LF1947     :: All 17 sensors supported
* </ul>
* @li <b>PIC18FxxJ50 Family</b>
* <ul>
* <li>PIC18F/LF24J50    :: All 10 sensors supported
* <li>PIC18F/LF25J50    :: All 10 sensors supported
* <li>PIC18F/LF26J50    :: All 10 sensors supported
* <li>PIC18F/LF44J50    :: All 13 sensors supported
* <li>PIC18F/LF45J50    :: All 13 sensors supported
* <li>PIC18F/LF46J50    :: All 13 sensors supported
* <li>PIC18F65J50       :: All 8 sensors supported
* <li>PIC18F66J50       :: All 8 sensors supported
* <li>PIC18F67J50       :: All 8 sensors supported
* <li>PIC18F85J50       :: All 12 sensors supported
* <li>PIC18F86J50       :: All 12 sensors supported
* <li>PIC18F87J50       :: All 12 sensors supported
* </ul>
*
*/
/**
* @page featSliders Sliders and Wheels
*
* The mTouch Framework has a special decoding module for sliders and wheels. It can support up to 10 sliders and 
* up to 8 sensors per slider. The sensors used for the slider/wheel are configured as normal touch sensors. They 
* will be scanned as a normal sensor and then, later, decoded as part of the slider.
*
* @section featSlider-How How It Works
* The slider/wheel decoding module is built on top of the normal decoding. It uses the delta readings of the 
* most-pressed sensor and it's most-pressed neighbor to calculate the touched position. An example is shown below:
* @image html sliderInterpolation.jpg "Slider Position Interpolation"
* The interpolated position is calculated as follows:
* @f[ Interpolated Press Position = (256 \cdot n) \pm \frac{Delta_{n}}{Delta_{n} + Delta_{n \pm 1}} @f]
*
* If #MTOUCH_SLIDER_SCALING_ENABLED is defined, the scaled interpolated position is calculated:
* @f[ Scaled Interpolated Press Position = \frac{InterpolatedPressPosition}{n} @f]
*
* In the equation, n is the most pressed sensor index.
* @image html sliderOutput.jpg "Slider Output Values"
*
* @section featSlider-Config Slider/Wheel Configuration
* <ol>
* <li> Configure the slider/wheel sensors to be scanned as normal sensors in the mTouch_config.h file. Make sure
*      every parameter is set appropriately, so that it can detect a normal touch. See the main configuration
*      guide for more information on this step.
* <li> Open the mTouch_config_slider.h file
*       <ol type="a">
*       <li>Set #MTOUCH_NUMBER_OF_SLIDERS to the total number of sliders/wheels you want to implement.
*       <li>Set #MTOUCH_SLIDER_STEP_SIZE. A small step size will smooth your slider result but will limit
*           its speed and response time. The default value (10) will generally give good results.
*       <li>Enable (recommended) or disable MTOUCH_SLIDER_SCALING_ENABLED based on your needs. If scaling
*           is enabled, the output value will always fall between 0 and 255. If it is disabled, the
*           maximum output value will be dependant on the InterpolatedPressPosition equation shown above.
*       <li>Configure each individual slider/wheel:
*           <ul>
*           <li>For each slider, set MTOUCH_NUMBER_SLIDERx_SENSORS to the number of sensors that will be 
*               used to implement this particularly slider/wheel. (Where 'x' is the slider index.)
*               
*               Example: 
*               @code 
* #define MTOUCH_NUMBER_SLIDER0_SENSORS  4
*               @endcode
*               Slider0 consists of 4 mTouch sensors
*
*           <li>For each sensor of each slider, set MTOUCH_SLIDERx_SENSORy to the MTOUCH_SENSOR index. 
*               (Where 'x' is the slider index and 'y' is the sensor index for that slider.) 
*               
*               Example: 
*               @code 
* #define MTOUCH_SLIDER0_SENSOR1  3
*               @endcode
*               Slider0's second sensor is MTOUCH_SENSOR3
*
*           <li>Define MTOUCH_SLIDERx_WHEEL_ENABLE if the slider is implemented as a wheel on the PCB.
*               This will have it connect the first and last sensors of the slider in the decoding math,
*               as well as implementing a speed and direction indication for use by the application.
*
*           </ul>
*       </ol>
* </ol>
*
* <b>LIMITATION:</b> The framework is not currently able to implement both wheels and sliders in the same
*                    application. The framework is not currently able to support more than one wheel.
*                    So, the framework is able to support up to 10 sliders with 8 sensors per slider 
*                    <b>OR</b> one wheel.
*           
* @section featSlider-Ex Example
* Configure two sliders with scaling enabled. The first slider uses 4 sensors and the second slider uses 6.
*
* @code
* // mTouch_config_slider.h
*
* #define MTOUCH_NUMBER_OF_SLIDERS        2   
* #define MTOUCH_SLIDER_STEP_SIZE         10        
* #define MTOUCH_SLIDER_SCALING_ENABLED       // Slider values will be from 0 to 255
*
* // mTouch Slider0 Configuration 
* #define MTOUCH_NUMBER_SLIDER0_SENSORS   4   // <-- Number of sensors that make up slider 0
* #define MTOUCH_SLIDER0_SENSOR0          3   
* #define MTOUCH_SLIDER0_SENSOR1          4   
* #define MTOUCH_SLIDER0_SENSOR2          5  
* #define MTOUCH_SLIDER0_SENSOR3          6   
* //#define MTOUCH_SLIDER0_WHEEL_ENABLE       // 'Wheel' disabled
*    
* // mTouch Slider1 Configuration 
* #define MTOUCH_NUMBER_SLIDER1_SENSORS   6   // <-- Number of sensors that make up slider 1
* #define MTOUCH_SLIDER1_SENSOR0          7   
* #define MTOUCH_SLIDER1_SENSOR1          8       
* #define MTOUCH_SLIDER1_SENSOR2          9   
* #define MTOUCH_SLIDER1_SENSOR3          10       
* #define MTOUCH_SLIDER1_SENSOR4          11  
* #define MTOUCH_SLIDER1_SENSOR5          12       
* //#define MTOUCH_SLIDER0_WHEEL_ENABLE       // 'Wheel' disabled
* @endcode
*
* @section featSlider-Out Slider/Wheel Output
* The framework provides one output for sliders (position) and three outputs for wheels (position, speed, direction).
* <ul>
* <li>  The slider/wheel position output is stored in the mTouch_slider array.
* <li>  The wheel's extra outputs are stored in a struct called wheelStatus. It contains speed in wheelStatus.speed and direction in 
*       wheelStatus.direction.
*   <ul>
*   <li>The speed range is from 0 to 31, with 0 being no movement and 31 being the fastest.
*   <li>The direction has three states: 0x00 for no-scrolling, 0x01 for scrolling in the positive index direction, 0x02 for scrolling 
*       in the negative index direction.
*   </ul>
* </ul>
*
* @section featSlider-API Slider/Wheel API
* The API for handling the slider and wheel outputs are:
* <ul>
* <li>mTouch_GetSlider(i)   :: returns the value of slider/wheel i
* <li>wheelStatus.direction :: stores the current direction of the wheel
* <li>wheelStatus.speed     :: stores the current speed of the wheel
* </ul>
*
* Example:
* @code
* // Light a line of LEDs based on the slider output value
* // 8 LEDs, 9 total "slider output values" for this application. 0 (all off) to 8 (all on).
* if (mTouch_GetSlider(0) >= 28)  { LED0 = LED_ON; } else { LED0 = LED_OFF; }
* if (mTouch_GetSlider(0) >= 57)  { LED1 = LED_ON; } else { LED1 = LED_OFF; }
* if (mTouch_GetSlider(0) >= 85)  { LED2 = LED_ON; } else { LED2 = LED_OFF; }
* if (mTouch_GetSlider(0) >= 114) { LED3 = LED_ON; } else { LED3 = LED_OFF; }
* if (mTouch_GetSlider(0) >= 142) { LED4 = LED_ON; } else { LED4 = LED_OFF; }
* if (mTouch_GetSlider(0) >= 170) { LED5 = LED_ON; } else { LED5 = LED_OFF; }
* if (mTouch_GetSlider(0) >= 199) { LED6 = LED_ON; } else { LED6 = LED_OFF; }
* if (mTouch_GetSlider(0) >= 227) { LED7 = LED_ON; } else { LED7 = LED_OFF; }
* @endcode
*
* @code
* // Same as above, but with slider scaling disabled.
* // 8 LEDs, 9 total "slider output values" for this application. 0 (all off) to 8 (all on).
* #define MTOUCH_SLIDER0_MAX_VALUE     ((MTOUCH_NUMBER_SLIDER0_SENSORS - 1) * 256)
* if (mTouch_GetSlider(0) >= (uint16_t)( 1/9 * MTOUCH_SLIDER0_MAX_VALUE)) { LED0 = LED_ON; } else { LED0 = LED_OFF; }
* if (mTouch_GetSlider(0) >= (uint16_t)( 2/9 * MTOUCH_SLIDER0_MAX_VALUE)) { LED1 = LED_ON; } else { LED1 = LED_OFF; }
* if (mTouch_GetSlider(0) >= (uint16_t)( 3/9 * MTOUCH_SLIDER0_MAX_VALUE)) { LED2 = LED_ON; } else { LED2 = LED_OFF; }
* if (mTouch_GetSlider(0) >= (uint16_t)( 4/9 * MTOUCH_SLIDER0_MAX_VALUE)) { LED3 = LED_ON; } else { LED3 = LED_OFF; }
* if (mTouch_GetSlider(0) >= (uint16_t)( 5/9 * MTOUCH_SLIDER0_MAX_VALUE)) { LED4 = LED_ON; } else { LED4 = LED_OFF; }
* if (mTouch_GetSlider(0) >= (uint16_t)( 6/9 * MTOUCH_SLIDER0_MAX_VALUE)) { LED5 = LED_ON; } else { LED5 = LED_OFF; }
* if (mTouch_GetSlider(0) >= (uint16_t)( 7/9 * MTOUCH_SLIDER0_MAX_VALUE)) { LED6 = LED_ON; } else { LED6 = LED_OFF; }
* if (mTouch_GetSlider(0) >= (uint16_t)( 8/9 * MTOUCH_SLIDER0_MAX_VALUE)) { LED7 = LED_ON; } else { LED7 = LED_OFF; }
* @endcode
*
* @section featSlider-FAQ Frequently Asked Questions
* <ul>
* <li>  <b>Is it possible to make a slider using only one sensor? It could be a single long triangle that gets
*          thinner/thicker as the finger moves along it.</b><br><br>
*       No. 
*       <ul>
*       <li>With only one sensor, there is no way to distinguish between a finger moving side-to-side on the
*           slider and a finger moving up-and-down over the largest part of the slider. The shift magnitude in
*           both cases will change from 0 to 255.
*       <li>If the slider's current value is 255 (max) and the user presses on position 0 of the slider (the
*           thinnest part) how will the slider know to update its value? The finger's effect on the sensor
*           will be minimal, so the shift will be close to 0. With a 2-channel slider, the shifts on the two
*           sensors will be opposites. So we won't see a shift on the first sensor, but the second one will
*           have a large shift.
*       <li>What if the slider's value is 255 when the user releases in the 'up' direction? How will the 
*           slider know that the user is going up instead of side-to-side? With a 2-channel slider, we compare
*           the relationship between the two sensors to see if their ratio changes. Up-down press directions
*           cause only small changes in the ratio while side-to-side directions cause large changes in the
*           ratio.
*       <li>The only output value of a sensor is its magnitude, but the maximum magnitude will depend on the
*           user of the application. A middle-aged man with large hands will cause a larger maximum shift than 
*           a 50 lb child. So the ratio between two sensors must be used to eliminate this difference.
*       </ul>
*       @note Regular capacitive buttons require only one sensors because we're looking for a step-response in
*           the signal - the transition between pressed and released. We are not attempting to use the magnitude 
*           to further extrapolate information such as a position.
* </ul>
*/

/**
* @page featProximity Proximity Sensors
*
* The mTouch Framework supports proximity detection sensors. Enabling this feature for a sensor will provide a
* higher signal-to-noise ratio by applying a median filter. Future updates may further enhance this capability.
* The API for handling the proximity sensor is exactly the same as with a normal sensor.
*
* @section featProx-How How It Works
* The theory of operation for proximity sensors is the same as touch buttons and keys. However, proximity
* sensors must be able to detect very small changes in capacitance, so a median filter is implemented to
* reduce the noise level. The largest cost of implementing this feature is the RAM requirement since we must 
* store the previous N samples for the median filter. This also means the response time will be longer than
* for a normal sensor while the median filter updates.
*
* The basic idea of a median filter is that it stores a history of the previous scan results in a first-in,
* first-out buffer. After each new scan, the oldest value is replaced with the newest, the array is sorted
* by size, and the middle values are averaged.
*
* @image html medianFilter.jpg "Median Filter Illustration"
*
* @section featProx-Config Proximity Sensor Configuration
* <ol>
* <li> Configure the proximity sensor(s) to be scanned as normal sensors in the mTouch_config.h file. The 
*      differences are that you may need to set a larger oversampling value and a lower threshold value to
*      make the sensor more sensitive. The threshold, unlike for button sensors, should not be set by
*      evaluating the maximum touch shift. Instead, set it by observing the maximum noise level. Using the
*      two-way communications with the mTouch Two-Way GUI is an easy way to adjust these values.<br>
*      You can find the mTouch Two-Way GUI in <tt>Your MLA Directory/mTouchCapDemos/Utilities/PIC12F PIC16F Utilities/mTouch Two-Way GUI</tt>
* <li> At the bottom of mTouch_config.h is the 'Proximity' configuration section.
*   <ol type="a">
*   <li>Set #MTOUCH_NUMBER_PROXIMITY to the total number of proximity sensors in your system.
*   <li>Define one MTOUCH_SENSORx_IS_PROX definition per enabled proximity sensor. The value of the definition
*       must be its 'proximity index' value. The 'Proximity index' is used as the index to proximity variable
*       arrays. In total, the indexes should start at 0 and end with MTOUCH_NUMBER_PROXIMITY - 1. The order is
*       arbitrary.
* @code
*   #define MTOUCH_NUMBER_PROXIMITY     1          
*   #define MTOUCH_SENSOR3_IS_PROX      0   // MTOUCH_SENSOR3 is a proximity sensor with index 0   
* @endcode
* @code
*   #define MTOUCH_NUMBER_PROXIMITY     2          
*   #define MTOUCH_SENSOR4_IS_PROX      0   // MTOUCH_SENSOR4 is a proximity sensor with index 0          
*   #define MTOUCH_SENSOR9_IS_PROX      1   // MTOUCH_SENSOR9 is a proximity sensor with index 1
* @endcode
*   <li> Set the parameters for the median filter:
*       <ul>
*       <li>#MTOUCH_PROX_BUFFER_SIZE decides the size of the buffer for the median filter. Each proximity
*           sensor will have its own median filter array, so be careful when assigning this value as it will
*           have significant effects on the RAM requirements of the proximity implementation. Valid options
*           are 5, 9, and 15. Larger number means more filtering, but higher RAM requirements and slower
*           response times.
*       <li>#MTOUCH_PROX_REMOVE_EXTREME determines how many of the largest and smallest numbers will be
*           removed before averaging the middle values. For example, if we are filtering with a buffer size
*           of 5 and this value is set to 1, we will average the 2nd, 3rd, and 4th values. With a buffer size
*           of 9 and this value set to 2, we will average the 3rd, 4th, 5th, 6th, and 7th values. This value
*           must be less than half the buffer size and greater than 0.
*       <li>Enable (recommended) or disable the #MTOUCH_PROX_USE_32BIT_ACCUM option based on your proximity
*           sensor's output data. Enabling this will prevent math overflow errors when the buffer is being
*           accumulated, but will add to the overall RAM requirement. It's possible you will not need this
*           protection, depending on the configuration, so this has been provided to easily allow toggling
*           between the two. (Example when you don't need the accumulator: buffer size of '5' and remove
*           extreme value of '2'. No accumulation occurs: the middle value is taken as-is.)
*       </ul>
*   </ol>
* </ol>
*           
* @section featProx-Ex Example
* @code
* #define MTOUCH_NUMBER_PROXIMITY       2   // Two proximity sensors
* #define MTOUCH_SENSOR0_IS_PROX        0   // MTOUCH_SENSOR0 is a proximity sensor with index 0
* #define MTOUCH_SENSOR5_IS_PROX        1   // MTOUCH_SENSOR5 is a proximity sensor with index 1
* #define MTOUCH_PROX_BUFFER_SIZE       5   // Median filter stores the last 5 values of each proximity sensor
* #define MTOUCH_PROX_REMOVE_EXTREME    1   // Removes the largest and smallest values before averaging
* #define MTOUCH_PROX_USE_32BIT_ACCUM       // Uses a 32-bit accumulator to prevent math overflow
* @endcode
*
* @section featProx-Out Proximity Sensor Output
* The way to access proximity sensor states is the same as for a normal sensor - using mTouch_GetButtonState(i).
* The index, i, is the normal sensor index - not the proximity index. For example, in the above code we could 
* check the state of the proximity sensors by using mTouch_GetButtonState(0) and mTouch_GetButtonState(5).
*/

/**
* @page featMatrix Matrix Designs
*
* The mTouch Framework supports decoding of matrix'd sensors to simplify the user's design process. This
* method allows for a larger number of possible unique press locations by decoding the state of two
* sensors at once: the row and the column.
*
* @section featMatrix-How How It Works
* The mTouch Framework will scan all sensors as normal. When a new set of readings is available, the framework
* will first perform some logic to determine which row and which column is the most pressed of the matrix. If
* both a row and column are activated, the state machine for the sensors will begin debouncing the result. 
* After the required number of repeated results, the matrix coordinate-output will be updated.
*
* @image html matrix.JPG "Sensors in a Matrix Configuration"
*
* @section featMatrix-Config Proximity Sensor Configuration
* <ol>
* <li> Configure the matrix'd sensors to be scanned as normal sensors in the mTouch_config.h file. We will
*      be defining index ranges for the rows and columns, so when configuring the MTOUCH_SENSORx list, make 
*      sure to place the matrix sensors as a continuous group starting at 0 or place at the very end with no
*      'normal' sensors after it. Within the matrix list, the rows should be grouped together and the columns 
*      should be grouped together.
* @code
* // Example sensor configuration for matrix
* #define MTOUCH_SENSOR0              AN0     // Matrix Row0    
* #define MTOUCH_SENSOR1              AN1     // Matrix Row1
* #define MTOUCH_SENSOR2              AN2     // Matrix Col0
* #define MTOUCH_SENSOR3              AN3     // Matrix Col1
* #define MTOUCH_SENSOR4              AN4     // Matrix Col2
* #define MTOUCH_SENSOR5              AN5     // Normal mTouch Sensor
* #define MTOUCH_SENSOR6              AN6     // Normal mTouch Sensor
* @endcode
* @code
* // Example sensor configuration for matrix
* #define MTOUCH_SENSOR0              AN0     // Normal mTouch Sensor
* #define MTOUCH_SENSOR1              AN1     // Normal mTouch Sensor
* #define MTOUCH_SENSOR2              AN2     // Matrix Col0
* #define MTOUCH_SENSOR3              AN3     // Matrix Col1
* #define MTOUCH_SENSOR4              AN4     // Matrix Col2
* #define MTOUCH_SENSOR5              AN5     // Matrix Row0
* #define MTOUCH_SENSOR6              AN6     // Matrix Row1
* @endcode
* <li> At the bottom of mTouch_config.h is the 'Decoding' section which contains the matrix configuration options.
*   <ol type="a">
*   <li>Uncomment #MTOUCH_MATRIX_ENABLED to enable matrix decoding.
*   <li>Define #MTOUCH_MATRIX_ROW_START and #MTOUCH_MATRIX_ROW_END to the MTOUCH_SENSORx index values for the
*       row sensors. In the first example, above, the start would be '0' and the end would be '1'. In the second
*       example, the start would be '5' and the end would be '6'.
*   <li>Define #MTOUCH_MATRIX_COLUMN_START and #MTOUCH_MATRIX_COLUMN_END to the MTOUCH_SENSORx index values for the
*       column sensors. In the first example, above, the start would be '2' and the end would be '4'. In the second
*       example, the start would be '2' and the end would be '4'.
*   <li>Define one MTOUCH_SENSORx_IS_PROX definition per enabled proximity sensor. The value of the definition
*       must be its 'proximity index' value. The 'Proximity index' is used as the index to proximity variable
*       arrays. In total, the indexes should start at 0 and end with MTOUCH_NUMBER_PROXIMITY - 1. The order is
*       arbitrary.
*   <li>If desired, set the value of #MTOUCH_MOST_PRESSED_THRESHOLD to the number of counts that the most-pressed
*       row or column must be greater than the second-most-pressed row/column to qualify it as the 'most pressed'.
*   </ol>
* </ol>
*           
* @section featMatrix-Ex Example
* @code
* #define MTOUCH_MATRIX_ENABLED               // <-- Matrix decoding is enabled
* #define MTOUCH_MATRIX_ROW_START         0   // <-- The sensor index of the first row
* #define MTOUCH_MATRIX_ROW_END           1   // <-- The sensor index of the last row
* #define MTOUCH_MATRIX_COLUMN_START      2   // <-- The sensor index of the first column
* #define MTOUCH_MATRIX_COLUMN_END        4   // <-- The sensor index of the last column
* #define MTOUCH_MOST_PRESSED_THRESHOLD   10  // <-- The most pressed column/row must be at least 10 counts
*                                             //     greater than the second-most-pressed column/row.
* @endcode
* @code
* #define MTOUCH_MATRIX_ENABLED               // <-- Matrix decoding is enabled
* #define MTOUCH_MATRIX_ROW_START         2   // <-- The sensor index of the first row
* #define MTOUCH_MATRIX_ROW_END           3   // <-- The sensor index of the last row
* #define MTOUCH_MATRIX_COLUMN_START      4   // <-- The sensor index of the first column
* #define MTOUCH_MATRIX_COLUMN_END        6   // <-- The sensor index of the last column
* #define MTOUCH_MOST_PRESSED_THRESHOLD   10  // <-- The most pressed column/row must be at least 10 counts
*                                             //     greater than the second-most-pressed column/row.
* @endcode
*
* @section featMatrix-Out Matrix Output
* The mTouch Framework stores the column and row output coordinates in an 8-bit value to save on RAM requirements.
* To easily decode which row and column is pressed, use the mTouch_Matrix_getRow() and mTouch_Matrix_getColumn() 
* API functions. They will return the row and column indexes, respectively.
*
* Note: The index they will be returning is relative to the matrix rows and matrix columns - not on the MTOUCH_SENSORx
* index value. For example, in the two example MTOUCH_SENSORx configurations above, pressing on Row0's sensor will 
* make mTouch_Matrix_getRow() return '0' - regardless of whether the row is defined as MTOUCH_SENSOR0 or MTOUCH_SENSOR5.
*
* mTouch_Matrix_hasChanged() will return a '0' if there was no change in the matrix output value since the last decode.
* mTouch_Matrix_latchNewValue() will save the current value of the matrix for later comparison.
* mTouch_Matrix_isPressed() will return a '0' if there are no pressed nodes on the matrix.
*
* @section featMatrix-API Example Matrix API
* @code
* if (mTouch_Matrix_hasChanged())         // If the matrix value has changed since last time
* {
*     mTouch_Matrix_latchNewValue();      // Save the new value of the matrix
*     
*     LED1  = LED_OFF;                    // Reset all matrix outputs
*     LED2  = LED_OFF;
*     LED3  = LED_OFF;
*     LED4  = LED_OFF;
*     LED5  = LED_OFF;
*     LED6  = LED_OFF;
*     LED7  = LED_OFF;
*     LED8  = LED_OFF;
*     LED9  = LED_OFF;
*     LED10 = LED_OFF;
*     LED11 = LED_OFF;
*     LED12 = LED_OFF;
* }
* 
* if (mTouch_Matrix_isPressed())          // If the matrix is being pressed
* {
*     switch(mTouch_Matrix_getRow())      // Find the row/column being pressed and output accordingly.
*     {
*         case 0:                         
*             switch(mTouch_Matrix_getColumn())
*             {
*                 case 0: LED1  = LED_ON; break;  // (0, 0)
*                 case 1: LED2  = LED_ON; break;  // (0, 1)
*                 case 2: LED3  = LED_ON; break;  // (0, 2)
*                 case 3: LED4  = LED_ON; break;  // (0, 3)
*             }
*             break;
*         case 1:
*             switch(mTouch_Matrix_getColumn())
*             {
*                 case 0: LED5  = LED_ON; break;  // (1, 0)
*                 case 1: LED6  = LED_ON; break;  // (1, 1)
*                 case 2: LED7  = LED_ON; break;  // (1, 2)
*                 case 3: LED8  = LED_ON; break;  // (1, 3)
*             }
*             break;
*         case 2:
*             switch(mTouch_Matrix_getColumn())
*             {
*                 case 0: LED9  = LED_ON; break;  // (2, 0)
*                 case 1: LED10 = LED_ON; break;  // (2, 1)
*                 case 2: LED11 = LED_ON; break;  // (2, 2)
*                 case 3: LED12 = LED_ON; break;  // (2, 3)
*             }
*             break;
*     }
* }
* @endcode
*/

/**
* @page featMostPressed Most-Pressed Algorithm
*
* The mTouch Framework can be configured to only allow a single mTouch sensor to be activated at a time, if
* desired. If a matrix is being implemented, the matrix sensors will be separated from the non-matrixed sensors
* and only the non-matrixed sensors will be limited to one-pressed-at-a-time.
*
* @section featMostPressed-How How It Works
* The mTouch Framework will scan all sensors as normal. When a new set of readings is available, the framework
* will first perform some logic to determine which row and which column is the most pressed of the matrix. If
* both a row and column are activated, the state machine for the sensors will begin debouncing the result. 
* After the required number of repeated results, the matrix coordinate-output will be updated.
*
* @section featMostPressed-Config Most-Pressed Algorithm Configuration
* <ol>
* <li> Configure the matrix'd sensors to be scanned as normal sensors in the mTouch_config.h file. We will
*      be defining index ranges for the rows and columns, so when configuring the MTOUCH_SENSORx list, make 
*      sure to place the matrix sensors as a continuous group starting at 0 or place at the very end with no
*      'normal' sensors after it. Within the matrix list, the rows should be grouped together and the columns 
*      should be grouped together.
* <li> At the bottom of mTouch_config.h is the 'Decoding' section which contains the matrix configuration options.
*   <ol type="a">
*   <li>Uncomment #MTOUCH_MOST_PRESSED_ONLY to enable the most-pressed algorithm on <b>normal</b> mTouch sensors. 
*       <ul>
*       <li>Matrix sensors are automatically compared against fellow row and column sensors to find the most-pressed.
*           This will not affect them.
*       <li>Proximity sensors use a separate decode state machine and, so, are also excluded from this algorithm.
*           This means you can have a matrix decoding normally, a proximity sensor providing constant data, and several 
*           'normal' mTouch sensors that may only be activated one-at-a-time based on the highest shift value.
*       </ul>
*   <li>Define #MTOUCH_MOST_PRESSED_THRESHOLD with the number of counts that the most pressed sensor must be above 
*       the second-most-pressed sensor to activate the algorithm. If this value is set to 10 and the two most-pressed
*       sensors shifts are only different by 5 counts, no sensors will be activated.
*   </ol>
* </ol>
*           
* @section featMostPressed-Ex Example
* @code
* #define MTOUCH_MOST_PRESSED_ONLY
* #define MTOUCH_MOST_PRESSED_THRESHOLD   10
* @endcode
*
* @section featMostPressed-Out Most-Pressed Output
* There are no special API or output macros for the most-pressed algorithm. Treat them as normal sensors.
*/

/**
* @page featGuard Guard Ring / Shield
* The framework is designed to output a synchronous signal on another pin to help maximize the
* sensitivity of the mTouch sensors. A guard ring is most effective when it encloses a sensor
* and shields it from any nearby communication traces, power planes (including ground), antennas,
* or other noise sources. The theory is that we want to decrease the voltage differential between 
* the capacitive sensor and its environment.
*
* @note One guard ring can be shared among all sensors.
*
* @section featGuard-How How It Works
* Guard rings (or shields) are driven by an I/O or the DACOUT pin to minimize the amount of 
* parasitic capacitance on the sensor. Parasitic capacitance will cause the field lines of the
* sensor to be distorted - pulling them down and decreasing the amount of sensitivity. With
* the guard ring, however, the sensor will be shielded from this effect. 
*
* @image html guardLines.JPG "Sensor Field Lines with and without a Guard Ring"
*
* @section featGuard-Config Guard Ring / Shield Configuration
* There are two possible ways to implement the guard ring.
*
* @image html guardWaveform.JPG "Two Guard Options and Their Behaviors"
*
* To implement the blue line (maximum sensitivity gain), the DACOUT pin must be connected
* to the guard ring on your board. This is the easiest and most fine-tunable of the two options.
* <ol>
* <li>Open the mTouch_config_cvdAdvanced.h file and uncomment #CVD_GUARD_DACOUT_ENABLED
* <li>Define #CVD_GUARD_DACCON0_A with the value of DACCON0 during the settling phase of the first sample.
* <li>Define #CVD_GUARD_DACCON1_A with the value of DACCON1 during the settling phase of the first sample.
* <li>Define #CVD_GUARD_DACCON0_B with the value of DACCON0 during the settling phase of the second sample.
* <li>Define #CVD_GUARD_DACCON1_B with the value of DACCON1 during the settling phase of the second sample.
* @code
* #define CVD_GUARD_DACOUT_ENABLED
* #define CVD_GUARD_DACCON0_A     PIC_DACOUT_DACCON0_HALF_VDD    // Pre-defined value to set '1/2 VDD'
* #define CVD_GUARD_DACCON1_A     PIC_DACOUT_DACCON1_HALF_VDD    // Pre-defined value to set '1/2 VDD'
* #define CVD_GUARD_DACCON0_B     PIC_DACOUT_DACCON0_HALF_VDD    // Pre-defined value to set '1/2 VDD'
* #define CVD_GUARD_DACCON1_B     PIC_DACOUT_DACCON1_HALF_VDD    // Pre-defined value to set '1/2 VDD'
* @endcode
*   'A' refers to the first half of the CVD scan when the external sensor is held low, the internal hold 
*   capacitor is charged high, and then they are connected. 'B' refers to the second half where the reverse 
*   is true. The settling voltage may be different for each of these, so different DACCON values may be 
*   required.
* </ol>
*
* To implement the red line, any I/O pin may be used. A large amount of series resistance should be added
* to the pin to lower the rise/fall time of the pin. The closer the rise/fall time of the I/O is able to
* match the rise/fall of the mTouch sensor, the better the result. Good typical starting value: 100K-200K
* ohms.
* <ol>
* <li>Open the mTouch_config_cvdAdvanced.h file and uncomment #CVD_GUARD_IO_ENABLED
* <li>Define #CVD_GUARD_PORT with the LAT or PORT register for your output pin.
* <li>Define #CVD_GUARD_PIN with the LAT or PORT bit that will access the output pin.
* @code
* #define CVD_GUARD_IO_ENABLED
* #define CVD_GUARD_PORT        LATA        // <-- Guard ring is connected to RA2 / LATA2
* #define CVD_GUARD_PIN         2           
* @endcode
* </ol>
*/

/**
* @page featMutual Mutual Coupling Drives
* In some cases, you may wish to measure the amount of coupling between a driven pin and a capacitive 
* sensor. Detecting ungrounded metal, for instance, is not always easily done with a normal CVD scan.
* The metal will not cause a large enough change in the capacitance of the sensor since it is floating.
* 
* Mutual drives can be used to look for an increase in the coupling between two traces. As that piece
* of floating metal (or a human finger, or...) comes near, the coupling between the two traces will
* increase. (The relative permativity between them is increasing.) 
*
* This allows for two different capacitive features to be measured at once:
* <ul>
* <li> The <b>self capacitance</b> of the sensor, determined by the relationship between the capacitance of
*      the external sensor with the capacitance of the internal ADC hold capacitor.
*  <ul>
*  <li>(Default mTouch Scan)
*  </ul>
* <li> The <b>coupling</b> between the external sensor and the mutual drive.
*  <ul>
*  <li>(Mutual Sensor Drive)
*  </ul>
* </ul>
* @note One mutual drive can be shared among all sensors.
* @image html mutualLines.JPG "Mutual Field Lines with and without Increased Coupling"
* 
*
* @section featMutual-How How It Works
* When the #CVD_MUTUAL_ENABLED option in mTouch_config_cvdAdvanced.h is used, the defined mutual pin is 
* driven so that increased coupling (between the mutual drive and the sensor) will cause a shift in the same 
* direction as adding capacitance to the sensor. Both the CVD scan and the mutual drive will be occuring at 
* the same time. This means you are getting the benefit of both scans in one. As the distance between the 
* object-to-be-detected and the sensor gets smaller, the impact of the mutual scan will increase. As the 
* distance gets larger, the self-capacitance mTouch scan will provide a majority of the sensitivity.
*
* @image html mutualWaveform.JPG "Mutual driving during a normal mTouch scan"
*
* @section featMutual-Config Mutual Sensor Configuration
* <ol>
* <li>Open the mTouch_config_cvdAdvanced.h file and uncomment #CVD_MUTUAL_ENABLED
* <li>Set #CVD_MUTUAL_LAT to the exact LAT or PORT bit for the mutual drive.
* <li>Set #CVD_MUTUAL_TRIS to the exact TRIS bit for the mutual drive.
* </ol>
*           
* @section featMutual-Ex Example
* @code
* #define CVD_MUTUAL_ENABLED                        // <-- Enables a mutual drive during the mTouch scan
* //#define CVD_MUTUAL_EXCLUSIVE_ENABLED            // <-- NOT defined!
* #define CVD_MUTUAL_LAT                    LATA3   // <-- The mutual sensor's LAT pin label
* #define CVD_MUTUAL_TRIS                   TRISA3  // <-- The mutual sensor's TRIS pin label
* @endcode
* @code
* //#define CVD_MUTUAL_ENABLED                      // <-- NOT defined!
* #define CVD_MUTUAL_EXCLUSIVE_ENABLED              // <-- Scanning ONLY with mutual coupling 
*                                                   //      and NO self-capacitance mTouch scan.
* #define CVD_MUTUAL_LAT                    LATC1   // <-- The mutual sensor's LAT pin label
* #define CVD_MUTUAL_TRIS                   TRISC1  // <-- The mutual sensor's TRIS pin label
* @endcode
*
* @section featMutual-Out Mutual Sensor Output
* There is no change to the mTouch API when using a mutual drive sensor.
*/

/**
* @page featMode Scanning Modes
* Scanning modes allow for an easy way to configure different states for the application where
* different sensors can be scanned in each state. For example, you could have a 'sleep' mode
* scanning a single proximity sensor and an 'active' mode that scans all sensors. You could 
* then alternate between the states based on recent user activity to reduce power consumption.
*
* When switching modes, the first scan result is ignored to allow the filters to properly
* reset themselves. The mTouch_state.skippedDecode bit will be set if this occured with
* the most recent decode and should be used to limit when mode changes can occur. An example
* is provided below.
*
* @section featMode-How How It Works
* All mTouch sensors, regardless of which mode they are in, are first defined as a normal
* mTouch sensor in the mTouch_config.h file. The MTOUCH_SENSORx index value for the sensor
* will be used to configure the modes. 
* 
* The MTOUCH_SENSORx index value will be referred to as the "sensor index".
*
* You can define up to 10 different modes. Each mode must have at least one sensor being scanned.
* (If you wish to simply disable scanning, use the mTouch_DisableScanning() macro.) The framework
* will start in Mode0. All decisions on when to change modes and why are left to the application.
* To change a mode, use the mTouch_ChangeMode(i) call, where i is the mode number. 
*
* The framework will only scan and decode the sensors that are enabled for the currently-active
* mode. 
*
* @section featMode-Config Scanning Mode Configuration
* <ol>
* <li>Configure all sensors for the system in mTouch_config.h as normal.
* <li>Open mTouch_config_modes.h and set the value of MTOUCH_NUM_MODES to a value larger than 1.
*   <ul>
*   <li>(MTOUCH_NUM_MODES == 1) or (MTOUCH_NUM_MODES == 0) means 'modes' will not be implemented.
*   </ul>
* @code
* #define MTOUCH_NUM_MODES                    2   // Two modes: Mode0 and Mode1
* @endcode
* <li>For each mode:
*   <ul>
*   <li>Set MTOUCH_MODEx_NUM_SENSORS to the number of sensors that will be scanned in that mode.
* @code
* #define MTOUCH_MODE0_NUM_SENSORS      3   // Three sensors scanned in Mode0.
* @endcode
*   <li>For each sensor in each mode:
*       <ul>
*       <li>Set MTOUCH_MODEx_SENSORx to the sensor index to be scanned.
* @code
* #define MTOUCH_MODE0_SENSOR0    4   // Sensor0 of Mode0 is MTOUCH_SENSOR4
* @endcode
*       </ul>
*   </ul>
* </ol>
*           
* @section featMode-Ex Scanning Mode Configuration Example
* @code
* //
* // mTouch_config.h        ::
* //
* #define MTOUCH_NUMBER_SENSORS         4   // Total of four mTouch sensors
*
* #define MTOUCH_SENSOR0              AN4        
* #define MTOUCH_SENSOR1              AN2            
* #define MTOUCH_SENSOR2              AN7    
* #define MTOUCH_SENSOR3              AN8  
*
* //
* // mTouch_config_modes.h  ::
* //
* #define MTOUCH_NUM_MODES              3   // Three modes: Mode0, Mode1, Mode2
*
* #define MTOUCH_MODE0_NUM_SENSORS      3   // Mode0 has 3 sensors
* #define MTOUCH_MODE0_SENSOR0          0   // Sensor0 of Mode0 is MTOUCH_SENSOR0
* #define MTOUCH_MODE0_SENSOR1          1   // Sensor1 of Mode0 is MTOUCH_SENSOR1
* #define MTOUCH_MODE0_SENSOR2          2   // Sensor2 of Mode0 is MTOUCH_SENSOR2
*
* #define MTOUCH_MODE1_NUM_SENSORS      1   // Mode1 has 1 sensor
* #define MTOUCH_MODE1_SENSOR0          0   // Sensor0 of Mode1 is MTOUCH_SENSOR0
*
* #define MTOUCH_MODE2_NUM_SENSORS      2   // Mode2 has 2 sensors
* #define MTOUCH_MODE2_SENSOR0          2   // Sensor0 of Mode2 is MTOUCH_SENSOR2
* #define MTOUCH_MODE2_SENSOR1          3   // Sensor1 of Mode2 is MTOUCH_SENSOR3
* @endcode
*
* @section featMode-Out Scanning Mode API
* To change the current mode, use the mTouch_ChangeMode(i) macro.
*
* To read the current mode, use mTouch_modeIndex. Do NOT change mTouch_modeIndex directly. Use the mTouch_ChangeMode macro!
*
* When switching modes, the first scan result is ignored to allow the filters to properly
* reset themselves. The mTouch_state.skippedDecode bit will be set if this occured with
* the most recent decode and should be used to limit when mode changes can occur. An example
* is provided below.
*
* An example usage of the API to switch between modes based on the state of different sensors:
* @code
* mTouch_DisableScanning();   // Temporarily disable scanning while this logic completes
* 
* // Are all of the current sensors initialized? If not, stay in the current mode and keep scanning until 
* // they are. mTouch_state.areInitialized is a single-bit reserved for making temporary, local checks 
* // such as this.
* mTouch_state.areInitialized = 1;
* 
* // Assuming four enabled sensors...
* if (mTouch_GetButtonState(0) == MTOUCH_INITIALIZING) {   mTouch_state.areInitialized = 0;    }
* if (mTouch_GetButtonState(1) == MTOUCH_INITIALIZING) {   mTouch_state.areInitialized = 0;    }
* if (mTouch_GetButtonState(2) == MTOUCH_INITIALIZING) {   mTouch_state.areInitialized = 0;    }
* if (mTouch_GetButtonState(3) == MTOUCH_INITIALIZING) {   mTouch_state.areInitialized = 0;    }
* // NOTE: You'll want additional checks here for all sensors being currently scanned.
*
* if (mTouch_state.areInitialized && !mTouch_state.skippedDecode)   // If we're not still initializing,
* {                                                                 // and we didn't skip the decode
*                                                                   // due to just changing modes...
*     if (mTouch_modeIndex == 0)                            // Application Specific: When in Mode0, let's
*     {                                                     //    change to Mode1 based on MTOUCH_SENSOR1.
*         if (mTouch_GetButtonState(1) == MTOUCH_PRESSED)
*         {
*             mTouch_ChangeMode(1); 
*         }
*     }
*     else if (mTouch_modeIndex == 1)                       // Application Specific: When in Mode1, let's
*     {                                                     //    sleep in-between decodes and change to
*                                                           //    Mode0 based on MTOUCH_SENSOR0.
*
*         #if defined(MCOMM_ENABLED) && defined(MCOMM_UART_HARDWARE_USED) 
*                                           // If we are outputting data using a hardware UART...
*             while(MCOMM_UART_TXIF == 0);  // Finish all communications before entering sleep
*         #endif
*         
*         PIC_SWDTEN_ON();                  // If using a software-enabled WDT, enable it now.
*         SLEEP();                          // Sleep, if you want to.
*         NOP();                            // One more instruction is executed before sleeping.
*         
*         if (mTouch_GetButtonState(0) == MTOUCH_PRESSED)   // (Application Specific mode change logic)
*         {
*             mTouch_ChangeMode(0);
*         }
*         
*         PIC_SWDTEN_OFF();                 // Sleep is over --> let's turn off the WDT.
*     }
* }
* mTouch_EnableScanning();    // Re-enable scanning now that mode-changing logic is complete.
* @endcode 
*/


/**
* @page featBasic Basic mTouch Sensor Configuration
* The mTouch Framework scans all sensors in the same basic manner. All sensors (whether normal 'button', part
* of a slider or wheel, a single row in a matrix, or a proximity sensor) start by being scanned using the
* mTouch CVD waveform. Once all enabled sensors have a 'final' reading, the data ready flag is set and the
* decoding routine is called from the main loop.
*
* @section featBasic-Config Basic mTouch Sensor Configuration
* <ol>
* <li>In the mTouch_config.h file, under 'System Setup' start by defining #_XTAL_FREQ to the 
*     frequency of your processor's oscillator in Hz. This does not set the value of your 
*     oscillator - it merely tells the framework what the value is initialized to by your 
*     application.
* <li>Set #MTOUCH_INTEGRATION_TYPE to the behavior you desire for your application.
*   <ul>
*   <li>#MTOUCH_CONTROLS_ISR :: means the framework is the only interrupt in the application
*   <li>#MTOUCH_CALLED_FROM_ISR :: means the application will be calling mTouch_Scan() from its own ISR.
*   <li>#MTOUCH_CALLED_FROM_MAINLOOP :: means the framework will perform scans any time the data-ready flag is checked.
*   </ul>
* <li>Set #MTOUCH_SCAN_FUNCTIONALITY to the behavior you desire for your application.
*   <ul>
*   <li>#MTOUCH_SCANS_ONE_SENSOR :: means the framework only scans a single sensor per mTouch_Scan() call.
*       <ul>
*       <li>This is the preferred value. It provides the best noise immunity due to increased jittering. 
*       </ul>
*   <li>#MTOUCH_SCANS_ALL_SENSORS :: means the framework scans all sensors one time per mTouch_Scan() call.
*       <ul>
*       <li>This can be chosen to increase the sampling rate of the system. Trade-off: slightly decreased noise robustness
*       </ul>
*   </ul>
* <li>Under 'Sensor Setup', set #MTOUCH_NUMBER_SENSORS to the number of sensors to scan, in total.
* @note Each sensor is usually a unique analog channel, but this is not required. To minimize 
*       the possibility of errors in the mTouch waveform, try not to repeat the same analog 
*       channel twice in a row. Also try and avoid having the first and last sensor as the same 
*       analog channel.
* <li>For each sensor:
*   <ol type="a">
*   <li>Set MTOUCH_SENSORx to its corresponding analog channel.
* @code
* #define MTOUCH_SENSOR0                AN0     // Sensor0 is connected to AN4
* @endcode
*   <li>Set THRESHOLD_PRESS_SENSORx to the threshold value for this sensor.
* @code
* #define THRESHOLD_PRESS_SENSOR0       100     // Reading must be 100 counts > the baseline
* @endcode
* @note This value is difficult to determine prior to running the system on the hardware and 
*       examining the level of sensitivity. 100-500 is a good default value but may need to be 
*       drastically adjusted based on the specific hardware design.
*   </ol>
* <li>Under 'Filtering', set #MTOUCH_SAMPLES_PER_SCAN to the amount of oversampling to perform on each sensor.
* <li>Under 'Decoding', set #MTOUCH_BUTTON_TIMEOUT to the maximum number of consecutive 'pressed' state 
*     decisions in a row. After this counter is exceeded, the pressed sensor will be reset to unpressed. 
*     This creates a maximum press time and is an easy way to automatically recover from a stuck sensor.
* <li>Set #MTOUCH_DEBOUNCE_PRESS and #MTOUCH_DEBOUNCE_RELEASE to the amount of debouncing to implement.
* </ol>
*
* @section featBasic-Out Basic mTouch Sensor API
* First, we will need to set up the application to scan mTouch. To do this, we need to create a check in
* the main loop to service the mTouch decode function. If we are using mTouch as one of several ISRs, we
* also need to add the scanning call to our ISR logic.
*
* @code
* void main(void)
* {
*     myApp_Init();                       // Your application's initialization (examples provided in main.c)
*
*     mTouch_Init();                      // mTouch initialization
*
*     #if defined(MCOMM_ENABLED)
*     mComm_Init();                       // mComm initialization
*     #endif
*
*     INTCONbits.GIE = 1;                 // Initialization complete. Begin servicing interrupts.
*
*     while(1)
*     {
*         if (mTouch_isDataReady())       // Is new information ready?
*         {
*             mTouch_Service();           // Decode the newly captured data and transmit new data updates.
*             
*             // Application sensor-state logic checks go here.
*         }
*     }
* }
* @endcode
*
* @code
* void interrupt ISR(void)
* {
*     SAVE_STATE();                       // mTouch Framework-supplied general ISR save state macro. 
*                                         // Not required, but convenient. 
*
*     #if (MTOUCH_INTEGRATION_TYPE == MTOUCH_CALLED_FROM_ISR)
*         if (mTouch_checkInterrupt())    // Checks if the TMRxIE and TMRxIF flags are both equal to 1.
*         {
*             mTouch_Scan();              // Required if running as ISR slave. The mTouch timer interrupt 
*                                         // flag is cleared inside the mTouch_Scan() function.
*         }
*     #elif (MTOUCH_INTEGRATION_TYPE == MTOUCH_CALLED_FROM_MAINLOOP)
*         mTouch_state.isrServiced = 1;   // Alerts the mTouch scanning routine that an interrupt may 
*                                         // have disrupted a scan. This is cleared at the start of a
*                                         // new scan and is checked at the end of the scan.
*                                         // Bad data can affect the readings if this flag is not set.
*     #endif
*     
*     
*     RESTORE_STATE();                    // mTouch Framework-supplied general ISR restore state macro. 
*                                         // Not required, but convienent.
* }
* @endcode
*
* The API for accessing button states is mTouch_GetButtonState(i). The states have been enumerated with
* the labels: #MTOUCH_INITIALIZING, #MTOUCH_RELEASED, and #MTOUCH_PRESSED.
*
* @code
* if (mTouch_GetButtonState(0) < MTOUCH_PRESSED)
* { 
*     LED0 = LED_OFF;                     // Sensor0 not pressed. Turn off LED.
* } else { 
*     LED0 = LED_ON;                      // Sensor0 pressed. Turn on LED.
* }
* @endcode
*
*/

/**
* @page FrameworkFeatures Features
*
* @section Features mTouch Framework Implementation Features
* We focused on creating a system that exhibits high signal sensitivity and noise immunity. To achieve these goals, we 
* implemented the following features:
* @li   @subpage featBasic is fast and simple
* @li   Built-in support for:
*   <ul>
*   <li>    @subpage featProximity provide a higher level of signal-to-noise ratio by implementing an additional median filter
*   <li>    @subpage featSliders decoding based on the relationship between sensor shift percentages
*   <li>    @subpage featMatrix support to help guarantee the reliability of 'node' press decisions
*   <li>    @subpage featMostPressed to limit the number of simultaneous 'pressed' sensors in high-crosstalk systems
*   <li>    @subpage featMode to support applications with multiple states and a desire to scan different sensors in each state
*   <li>    @subpage featGuard traces to <b>actively reduce parasitic capacitance</b> and increase sensitivity
*   <li>    @subpage featMutual to scan for changes in the relative permitivity, or coupling, near the sensor
*   </ul> 
* @li   <b>Detailed warning and error messages</b> guide the user in case of an incorrect configuration value
* @subsection AcquisitionFeatures - Acquisition Module 
* @li   Differential CVD scanning method drastically <b>attenuates low frequency noise</b> (Including 50/60Hz, up to low KHz)
* @li   <b>Sample rate jittering</b> algorithm eliminates issues with resonant noise frequencies
* @li   Generated assembly implementation <b>maximizes noise immunity</b> and <b>minimizes execution time</b>
* @li   Ability to force the scan sequence to use the <b>DAC as the reference voltage</b> to the ADC's hold capacitor.
* @subsection FilteringFeatures - Filtering Module 
* @li   Decimate-filtered result register seeding an accumulator array provides <b>impulse noise rejection</b> and an increase 
*       in signal through <b>oversampling</b>
* @li   Baseline average <b>tracks environmental changes</b> to maximize response time
* @li   Enhanced filtering options allow for <b>customizable filter behavior</b> and update speed based on the 
*       application's requirements
* @li   <b>Median filter</b> implemented for proximity sensors to further increase the SNR
* @subsection DecodingFeatures - Decoding Module 
* @li   <b>Individual thresholds</b> provide the ability to adjust for different sensor sizes and types
* @li   <b>Threshold hysteresis</b> reduces sensor flickering in noisy conditions as well as increasing the reliability of 
*       sensor state transitions
* @li   Configurable <b>press timer</b> to implement a <b>maximum press duration</b> for the application
* @li   <b>Debounce counters</b> provided to further increase reliability, if desired.
* @li   <b>Most Pressed Algorithm</b> is implemented automatically for matrix applications on the rows and columns, separately.
* @li   <b>Toggle mode</b> provided to quickly and efficiently implement a change-on-press-only button output behavior.
* @subsection CommunicationFeatures - Communication 
* @li   Preconfigured <b>software or hardware UART</b> implementation to report ASCII data one-way to a COM port
* @li   <b>UART, I2C, and SPI two-way communication</b> protocol to read/write configuration and RAM values at run-time
* @li   <b>Store configuration values in EEPROM</b> for run-time editing capability
*
*/

/**
* @page GettingStarted Getting Started
*
* @section Expect What to Expect from the mTouch Framework
* When designing with the mTouch Framework, it's important to understand the system's overall behavior. Borrowing
* from the <a href="http://en.wikipedia.org/wiki/Software_framework">Wikipedia article</a> on software frameworks, here
* are the distinguishing features of a framework over a library or normal application:<br>
* @li <b>Inversion of Control</b><br><i>In a framework, unlike in libraries or normal user applications, the overall program's
*                                   flow of control is not dictated by the caller, but by the framework.</i><br>
*                                   The mTouch Framework requires the TMR0 interrupt, ADC, and a significant portion
*                                   of the overall processing time. Developers do, however, have full control over the
*                                   main loop of their application.<br>
*                                   (<b>See also:</b> @ref ResourceRequirements "Resource Requirements")<br><br>
* @li <b>Default Behavior</b><br><i>A framework has default behavior. This default behavior must actually be some useful behavior and not a series of no-ops.</i><br>
*                                   The mTouch Framework's default behavior is to perform a differential CVD scan on the sensors, perform digital
*                                   filtering on the acquired signal, and decode the signal's behavior into a Yes/No press state.<br><br>
* @li <b>Extensibility</b><br><i>A framework can be extended by the user usually by selective overriding or specialized by user code providing specific functionality.</i><br>
*                                   The mTouch Framework comes with a predefined mTouch_Service() function which will perform all of the decoding
*                                   necessary for normal mTouch capacitive buttons. For enhanced capabilities such as sliders, wheels, proximity sensors,
*                                   level sensors, and other capacitive touch uses a custom service function may be required. This simple process is explained
*                                   in detail in the quick start guide.<br><br>
* @li <b>Non-Modifiable Framework Code</b><br><i>The framework code, in general, is not allowed to be modified. Users can extend the framework, but not modify its code.</i><br>
*                                   The mTouch Framework is provided as an open source solution, so it is possible to edit the core acquisition code of the framework. 
*                                   Doing this, however, is <b>highly discouraged</b> and could easily impact the noise immunity of your final solution. For this reason,
*                                   we consider the mTouch ISR and all @c include/ files as essentially non-modifiable.<br>
*
* @section GSGuides Developer Resource Guides
*
* <b>Quick Start Guides</b>
*
* Use these quick start guides to get you up and running with the mTouch Framework.
* @li @subpage GettingStartedEval   "mTouch CVD Evaluation Board"
* @li @subpage GettingStartedCustom "Custom Hardware"
* @li @subpage GettingStartedNewP8  "Creating a New MPLAB 8 Framework Project"
* @li @subpage GettingStartedNewPX  "Creating a New MPLAB X Framework Project"
* @li @subpage PKSARS232            
*/
/**
* @page optimizing Optimizing Performance
* @li @subpage ts-Sensitivity   "Increasing Sensitivity"
* @li @subpage ts-ResponseTime  "Adjusting the Response Time"
* @li @subpage ts-Waveform      "Tuning the CVD Waveform"
* @li @subpage digCom
*/
/**
* @page troubleshoot Troubleshooting
* @li @subpage ts-Comms         "Establishing PC Communications"
* @li @subpage digCom
* @li @subpage BCPGUI           "Backwards-compatible UART for previous Profilab GUI version"
*/
/**
* @page ts-Sensitivity Increasing Sensitivity - Optimizing Performance
*
* @li <b>1. Make sure your application's CVD waveform is correct.</b><br><br>
*        See the @ref ts-Waveform "Waveform Optimization guide" for help with this process. 
* 
* <br>
*
* @li <b>2. Use #MTOUCH_SAMPLES_PER_SCAN to increase the amount of oversampling.</b><br><br>
*        All applications will want to oversample at least 8 times to ensure random noise cannot
*        significantly affect the sensor's reading.<br>
*        <br>
*        Most applications oversample between 30-50 times.<br>
*        <br>
*        The diminishing returns on sensitivity when increasing this value occur at an exponential 
*        rate. This means there is only a small difference at 70 and 80 samples, while there is a 
*        very large difference between 10 and 20 samples. 
* 
* <br>
*
* @li <b>3. Change #MTOUCH_SCALING to 1 to eliminate resolution loss.</b><br><br>
*        When the acquisition module has oversampled #MTOUCH_SAMPLES_PER_SCAN number of times, the final
*        sum must be moved from a 24-bit array to a 16-bit array. The reading value cannot be allowed
*        to exceed the 16-bit limit of 65535. To prevent this from occuring, the framework will use
*        #MTOUCH_SCALING to determine what to divide the accumulation register by. This results in a
*        slight loss in resolution. If this is a concern for your application, setting #MTOUCH_SCALING value to
*        1 will eliminate this step.<br>
*        <br>
*        It is your responsibility to ensure the CVD's reading cannot exceed 65535.
* 
* <br>
*
* @li <b>4. Check that the baseline is not becoming corrupted.</b><br><br>
*        If your baseline is updating too quickly, you may be losing some sensitivity because the baseline
*        is no longer equal to the sensor's unpressed value. To check this, uncomment #MTOUCH_COMM_ASCII_BASELINE
*        in the configuration file and view the baseline in real time with the reading.<br>
*        <br>
*        If the baseline is updating too quickly, you can slow it down by increasing the value of 
*        #MTOUCH_BASELINE_WEIGHT and #MTOUCH_BASELINE_RATE.<br>
*        <br>
*        If the baseline is becoming corrupted due to fast tapping on the sensor, use the 
*        #MTOUCH_NEGATIVE_CAPACITANCE configuration option to adjust how the baseline behaves when the reading
*        has fallen below the baseline. For fast-tapping systems, setting this value to 1 is recommended.
*        Using the '2' value for this option is only recommended for systems that are not going to have
*        any noise spikes.
* 
* <br>
*
* @li <b>5. Decrease the amount of parasitic capacitance affecting the sensor and its trace.</b><br><br>
*        The amount of parasitic capacitance on the sensor will directly impact its maximum level of
*        sensitivity. Eliminate ground planes near your sensor and make sure its trace path is not 
*        running near any high-current or digital lines. This is particularly important for proximity
*        sensors and normal touch sensors with long traces.
*/
/**
* @page ts-ResponseTime Faster Response Time - Optimizing Performance
*
* The response time of your system is going to be determined by the equation:
* @code
* Response Time = mTouch ISR Interrupt Rate * MTOUCH_NUMBER_SENSORS * MTOUCH_SAMPLES_PER_SCAN * MTOUCH_DEBOUNCE_PRESS
* @endcode
*
* <br>
*
* <b>Suggestions for Making the Response Time Faster:</b><br>
*
* <br>
*
* @li <b>1. Decrease #MTOUCH_SAMPLES_PER_SCAN to reduce the amount of oversampling.</b><br><br>
*        This configuration option should never drop below 10 in normal applications. The higher 
*        this value is, the more stable the sensor's readings will be coming out of the acquisition
*        ISR. However - the diminishing returns on this value follow an exponential curve. In other
*        words, the increase in signal fidelity when changing from 10 to 20 samples is about the
*        same as changing from 40 to 80.
*
* <br>
*
* @li <b>2. Reduce the TMR0 Prescaler value.</b><br><br>
*        This will increase the rate at which the timer's counter will increment. Do not set this
*        value to 1:1 or the interrupt will occur too quickly to allow processing in the main loop.
*        This value is determined by the application when it initializes the OPTION register. The
*        framework does not adjust this register.
*
* <br>
*
* @li <b>3. Increase the PIC's oscillator frequency.</b><br><br>
*        This will increase the rate at which the timer's counter will increment, but will also 
*        increase the amount of power needed by the PIC.
*
* <br>
*
* @li <b>4. Decrease #MTOUCH_NUMBER_SENSORS.</b><br><br>
*        The mTouch Framework scans all sensors completely before returning the result to be processed
*        by the main loop application. Reducing the number of sensors being scanned will reduce the
*        number of times the ISR must be called before new data is available.
*
* <br>
*
* @li <b>5. Reduce the amount of debouncing required to change sensor states.</b><br><br>
*        There are two debounce values available:<br>
*        #MTOUCH_DEBOUNCE_PRESS is for the RELEASE-to-PRESS transition.<br>
*        #MTOUCH_DEBOUNCE_RELEASE is for the PRESS-to-RELEASE transition.
*/
/**
* @page ts-Waveform Tuning the CVD Waveform - Optimizing Performance
*
* @section mTouch CVD Waveform Steps
*
* <tt>CVD :: Capacitive Voltage Divider</tt><br>
* <tt>DAC :: Digital-to-Analog Converter</tt><br>
* <tt>ADC :: Analog-to-Digital Converter</tt><br>
* <tt>T<sub>AD</sub> :: Time required for the ADC to complete one bit conversion.</tt><br><br>
*
* <ul>
* <li>  Stage 1 (a) :: Pre-charge
*       <ul>
*       <li>The sensor is set to <b>output low</b>.
*       <li>Either:
*           <ul>
*           <li>The DAC is set to <b>V<sub>DD</sub></b> and the ADC mux is pointed to the DAC.
*           <li>Another analog channel is set as <b>output high</b> and the ADC mux is pointed to that channel.
*           </ul>
*       <li>(Delay, based on #CVD_CHOLD_CHARGE_DELAY)
*       </ul><br><br>
* <li>  Stage 2 (a) :: Acquisition / Settling
*       <ul>
*       <li>The sensor is set to an <b>input</b>.
*       <li>The ADC mux is updated to point to the sensor.
*       <li>(Delay, based on #CVD_SETTLING_DELAY)
*       </ul><br><br>
* <li>  Stage 3 (a) :: Sampling
*       <ul>
*       <li>The ADC's GO/nDONE bit is set.
*       <li>The ADC mux waits 1/2 T<sub>AD</sub> then disconnects from the sensor.
*       <li>(The mTouch framework automatically delays the necessary time for the mux to disconnect.)
*       <li>The sensor is set to an <b>output</b>.
*       </ul>
* </ul>
* The ADC conversion completes while math is being performed on the previous mTouch scan result.
* <ul>
* <li>  Stage 1 (b) :: Pre-charge
*       <ul>
*       <li>The sensor is set to <b>output high</b>.
*       <li>Either:
*           <ul>
*           <li>The DAC (digital-to-analog converter) is set to <b>V<sub>SS</sub></b> and the ADC (analog-to-digital converter) mux is pointed to the DAC.
*           <li>Another analog channel is set as <b>output low</b> and the ADC mux is pointed to that channel.
*           </ul>
*       <li>(Delay, based on #CVD_CHOLD_CHARGE_DELAY)
*       </ul><br><br>
* <li>  Stage 2 (b) :: Acquisition / Settling
*       <ul>
*       <li>The sensor is set to an <b>input</b>.
*       <li>The ADC mux is updated to point to the sensor.
*       <li>(Delay, based on #CVD_SETTLING_DELAY)
*       </ul><br><br>
* <li>  Stage 3 (b) :: Sampling
*       <ul>
*       <li>The ADC's GO/nDONE bit is set.
*       <li>The ADC mux waits 1/2 T<sub>AD</sub> then disconnects from the sensor.
*       <li>(The mTouch framework automatically delays the necessary time for the mux to disconnect.)
*       <li>The sensor is set to an <b>output</b>.
*       </ul>
* </ul>
* The ADC conversion completes. When finished, the first scan is subtracted from the second. The second
* scan is offset by 1024 to make sure this subtract never goes negative.
*
* @section mTouch CVD Waveform Delays
*
* Below is a picture of what the CVD waveform should look like on a scope. Note that sensor lines are
* used both for scanning themselves and the other sensors. You may notice the sensor being pulled high
* and low during other sensor's scans.
*
* @image html cvdWaveform.jpg
*
* The mTouch framework provides the ability to tune the timing of CVD waveform for specific hardware
* designs. The amount of parasitic capacitance, the size of the sensor, the size of the sensor's series
* resistor, and V<sub>DD</sub> all play a role in determining the amount of time required to charge/discharge
* during each step of the scan.
*
* @image html cvdWaveformDelay.jpg "CVD Waveform Delay Configuration Options"
*
* <br>
*
* @li <b>CVD_CHOLD_CHARGE_DELAY</b><br><br>
* This value determines the amount of time provided for charging the internal hold capacitor of the
* ADC. <br><br>
* If this value is <b>too small</b>: There will be noticable crosstalk between closely-indexed sensors
* that is not related to the hardware layout of the application.<br>
* If this value is <b>too large</b>: The mTouch ISR will take an unnecessarily long amount of time to 
* execute.<br><br>
* <b>To correctly set this value:</b>
* <ol>
* <li>Set this value to 0, compile and program.
* <li>Look at the raw values on the mTouch One-Way GUI or through a terminal program.<br>
*     You can find the mTouch One-Way GUI in <tt>Your MLA Directory/mTouchCapDemos/Utilities/PIC12F PIC16F Utilities/mTouch One-Way GUI</tt>
* <li>Produce the largest shift possible on the sensor with the highest amount of capacitance by pressing
* on it. ("Highest Capacitance" usually corresponds to the sensor with the highest unpressed raw value.)
* <ul>
* <li>Do you notice a strange crosstalk behavior on a different sensor? If no, leave this value to 0. If yes...
* </ul>
* <li>Increase the delay until the crosstalk behavior is eliminated. 
* </ol>
* <br>
* Once you've solved the crosstalk for the sensor with the highest amount of capacitance, the others will 
* be fine as well and you have correctly tuned the Chold delay time.
*
* <br>
*
* @li <b>CVD_SETTLING_DELAY</b><br><br>
* This value determines the amount of time provided for the external sensor and internal hold capacitor
* to charge-average their voltages.<br><br>
* If this value is <b>too small</b>: The sensors will not be as sensitive as they could be and your sensor's readings will be V<sub>DD</sub> dependant.<br>
* If this value is <b>too large</b>: The noise immunity of the system will not be as robust as it could be.<br><br>
* <b>To correctly set this value:</b>
* <ol>
* <li>Set this value to 0, compile and program.
* <li>Look at the raw values on the mTouch One-Way GUI or through a terminal program while powering the system at the desired V<sub>DD</sub> level.<br>
*     You can find the mTouch One-Way GUI in <tt>Your MLA Directory/mTouchCapDemos/Utilities/PIC12F PIC16F Utilities/mTouch One-Way GUI</tt>
* <li>Adjust V<sub>DD</sub> by plus and minus 0.5V 
* <ul>
* <li>Do you notice any change in the unpressed value as V<sub>DD</sub> is changing? If no, leave this value alone. If yes...
* </ul>
* <li>Increase the delay until the sensor's reading no longer changes as V<sub>DD</sub> changes.
* </ol>
* <br>
* Once this has occurred, the settling time has been correctly tuned to provide the maximum amount of sensitivity
* while minimizing the framework's susceptibility to noise.
*
*/
/**
* @page ts-Comms PC Communications - Troubleshooting
*
* <center>
* <table border="2">
* <tr valign="top"><td align="left">
*       <center><b>IMPORTANT IMPORTANT IMPORTANT IMPORTANT IMPORTANT IMPORTANT IMPORTANT IMPORTANT</b></center>
*
*       The default PICKit Serial (PKSA) is not loaded with the correct hex file. To use the PKSA to communicate with the PC, one of two
*       hex files must be programmed into the PKSA. A utility program has been provided to help with this step.
*
*       The two hex files you must choose from:
*       @li <b>UART_USB_PICKitSerial_V1.40.hex</b> :: When prompted, choose this hex file if you are implementing <b>one-way communication</b>.
*           This hex file will make your PKSA enumerate on the PC as a COM port. The PKSA will now function as a UART-to-USB converter. You
*           can then view ASCII data in a terminal window or use the mTouch One-Way GUI to graph the data in real-time.
*       @li <b>PKS-0307-WITHBOOT-0103.HEX</b> :: When prompted, choose this hex file if you are implementing <b>two-way communication</b>. This
*           hex file is an updated version of the factory default firmware. Your PKSA will behave exactly as before, but with additional
*           support for RS-232 break characters. You can then use the mTouch Two-Way GUI to change configuration values at run-time and
*           view the effect on the sensors in real-time.
*
*       The PICKit Serial Loader Utility is located in <tt>Your MLA Directory/mTouchCapDemos/Utilities/PIC12F PIC16F Utilities/PICKit Serial Loader</tt>
*
*       The @ref PKSARS232 "PICKit Serial Loader Guide" provides more information.
*
*       <center><b>IMPORTANT IMPORTANT IMPORTANT IMPORTANT IMPORTANT IMPORTANT IMPORTANT IMPORTANT</b></center>
* </td></tr>
* </table>
* </center>
*
* <br><br>
*
* There are two main types of communication built in to the mTouch Framework's mComm module: 
* @li   @subpage ts-Comms-1way "One-way communication" is handled through UART only. It outputs ASCII data to be read from a terminal window
*       or from a GUI that reads COM ports, such as the mTouch One-Way GUI. You can find the mTouch One-Way GUI in 
*       <tt>Your MLA Directory/mTouchCapDemos/Utilities/PIC12F PIC16F Utilities/mTouch One-Way GUI</tt><br><br>
*       If you wish to use the PICKit Serial with this communication type, it must be loaded with the UART-to-USB
*       firmware. See @ref PKSARS232 "this guide" for information on loading this hex file to your PKSA.<br>
*       @note For backward compatibility with previous versions of the Profilab GUI, please follow @ref BCPGUI "this guide".<br><br>
* @li   <b>Two-way communication</b> can be implemented with UART, I2C, and SPI. <br><br>
*       <a href="../mComm Users Guide.pdf"><b>User's Configuration Guide - (PDF)</b></a><br>
*       <a href="../mComm Programmers Guide.pdf"><b>Programmer's Guide - (PDF)</b></a><br>
*       <a href="../mTouch 2-Way GUI User Guide.pdf"><b>mTouch 2-Way GUI User Guide - (PDF)</b></a><br><br>
*       The protocol allows for efficient, direct access to EEPROM and RAM for both reading and writing.
*       The data is binary, so must be interpretted by a master instead of a terminal window. This mode allows changing values 
*       at run-time and execution of user commands. This option is used for communicating to the mTouch Two-Way GUI and for implementing 
*       custom board-level communications.<br><br>
*       If you wish to use the PICKit Serial with the UART two-way communication type, the PKSA firmware must be 
*       updated to the latest PKSA version to support break characters. See @ref PKSARS232 "this guide" for information
*       on updating the PKSA hex file.<br><br>
*/

/**
* @page BCPGUI   PC Communications :: Backward Compatibility :: One-way UART Profilab GUI v1.0
* 
* The new version of the one-way UART GUI uses a different ASCII protocol to support the larger feature 
* set of the mTouch Framework. The old packet structure was:
*   @li Semi-colon delimited
*   @li First byte: Button state mask
*   @li Graph data of all other integers until CR/LF
*   @code 
*   Example: 00001;00100;01000;10000;
*   @endcode
*
* <ol>
* <li>How to adjust mComm_config.h file configuration.
*   @code
*   #define MCOMM_ENABLED                                             // Must be defined
*   #define MCOMM_TYPE                    MCOMM_UART_ONE_WAY          // Must be defined to this value
*   #define MCOMM_UART_BAUDRATE           38400                       // (or) other valid baud rate option
*   @endcode
*   @code
*   #define MCOMM_UART_1WAY_MODULE        MCOMM_UART_HARDWARE_MODULE  // (or) MCOMM_UART_SOFTWARE_IMPLEMENTATION
* 
*     // If MCOMM_UART_SOFTWARE_IMPLEMENTATION is chosen:
*     #define MCOMM_UART_SOFT_TXPORT        PORTA
*     #define MCOMM_UART_SOFT_TXTRIS        TRISA   
*     #define MCOMM_UART_SOFT_TXPIN         5       // <-- The bit of the PORT/TRIS register
*                                                   //     NOT the hardware pin on the device
*   @endcode
*   @code
*   #define MCOMM_UART_1WAY_OUTPUT        MCOMM_UART_1WAY_DECIMAL     // (or) MCOMM_UART_1WAY_HEX        
*   #define MCOMM_UART_1WAY_DELIMITER     ';'                         // <-- IMPORTANT                  
*   @endcode
*
* <li>Modify the bottom half of the mComm.c function: mComm_Service()<br><br>
* <b>Replace this code:</b>
* @code
*         #if defined(MCOMM_ONE_WAY_ENABLED)
*             // OUTPUT LOGIC FOR ONE-WAY COMMUNICATIONS.
* 
*             #if defined(MCOMM_UART_1WAY_OUT_STATE)
*             mComm_UART_Int2ASCII((uint16_t)mTouch_stateMask);
*             #else
*             mComm_UART_Int2ASCII(0);
*             #endif
*             
*             #if defined(MCOMM_UART_1WAY_OUT_TOGGLE) && defined(MTOUCH_TOGGLE_ENABLED)
*             mComm_UART_Int2ASCII((uint16_t)mTouch_toggle);
*             #else
*             mComm_UART_Int2ASCII(0);
*             #endif
*             
*             #if defined(MCOMM_UART_1WAY_OUT_SLIDER) && defined(MTOUCH_NUMBER_OF_SLIDERS) && (MTOUCH_NUMBER_OF_SLIDERS > 0)
*             mComm_UART_Char2ASCII(mTouch_slider[0]);            
*             #else
*             mComm_UART_Char2ASCII(0);
*             #endif
* 
*             #if defined(MCOMM_UART_1WAY_OUT_MATRIX) && defined(MTOUCH_MATRIX_ENABLED)
*             mComm_UART_PutChar('(');
*             if (mTouch_Matrix_isPressed())
*             {
*                 mComm_UART_PutChar((uint8_t)(mTouch_Matrix_getColumn()) + 0x30);
*                 mComm_UART_PutChar(':');
*                 mComm_UART_PutChar((uint8_t)(mTouch_Matrix_getRow())    + 0x30);
*             }
*             else
*             {
*                 mComm_UART_PutChar('x');
*                 mComm_UART_PutChar(':');
*                 mComm_UART_PutChar('x');
*             }
*             mComm_UART_PutChar(')');
*             mComm_UART_PutChar(MCOMM_UART_1WAY_DELIMITER);
*             #endif
*             
*             #if defined(MCOMM_UART_1WAY_OUT_READING) || defined(MCOMM_UART_1WAY_OUT_BASELINE)
*             for (uint8_t i = 0; i < MTOUCH_NUMBER_SENSORS; i++)
*             {
*                 #if defined(MCOMM_UART_1WAY_OUT_READING)
*                 mComm_UART_Int2ASCII(mTouch_GetSensor(i));      
*                 #endif                                          
*                 #if defined(MCOMM_UART_1WAY_OUT_BASELINE)
*                 mComm_UART_Int2ASCII(mTouch_GetAverage(i));     
*                 #endif
*             }
*             #endif
*             
*             mComm_UART_PutChar(0x0D);   // CR                   
*             mComm_UART_PutChar(0x0A);   // LF
*         
*         #endif
* @endcode
* 
* <b>With this code:</b>
* @code
*         #if defined(MCOMM_ONE_WAY_ENABLED)
*             // OUTPUT LOGIC FOR ONE-WAY COMMUNICATIONS.
* 
*             mComm_UART_Int2ASCII((uint16_t)mTouch_stateMask);
*             
*             #if defined(MCOMM_UART_1WAY_OUT_READING) || defined(MCOMM_UART_1WAY_OUT_BASELINE)
*             for (uint8_t i = 0; i < MTOUCH_NUMBER_SENSORS; i++)
*             {
*                 #if defined(MCOMM_UART_1WAY_OUT_READING)
*                 mComm_UART_Int2ASCII(mTouch_GetSensor(i));      
*                 #endif                                          
*                 #if defined(MCOMM_UART_1WAY_OUT_BASELINE)
*                 mComm_UART_Int2ASCII(mTouch_GetAverage(i));     
*                 #endif
*             }
*             #endif
*             
*             mComm_UART_PutChar(0x0D);   // CR                   
*             mComm_UART_PutChar(0x0A);   // LF
*         
*         #endif
* @endcode
* </ol>
*/

/**
* @page ts-Comms-1way   PC Communications :: One-way UART ASCII output
* 
* <ol>
* <li>Make sure the mComm_config.h file is properly configured.
* @code
* #define MCOMM_ENABLED
* #define MCOMM_TYPE                    MCOMM_UART_ONE_WAY
* #define MCOMM_UART_BAUDRATE           38400                       // (or) other valid baud rate option
* @endcode
* @code
* #define MCOMM_UART_1WAY_MODULE        MCOMM_UART_HARDWARE_MODULE  // (or) MCOMM_UART_SOFTWARE_IMPLEMENTATION
* 
*     // If MCOMM_UART_SOFTWARE_IMPLEMENTATION is chosen:
*     #define MCOMM_UART_SOFT_TXPORT        PORTA
*     #define MCOMM_UART_SOFT_TXTRIS        TRISA   
*     #define MCOMM_UART_SOFT_TXPIN         5       // <-- The bit of the PORT/TRIS register
*                                                   //     NOT the hardware pin on the device
* @endcode
* @code
* #define MCOMM_UART_1WAY_OUTPUT        MCOMM_UART_1WAY_DECIMAL     // (or) MCOMM_UART_1WAY_HEX        
* #define MCOMM_UART_1WAY_DELIMITER     ';'                         
* @endcode
* If you wish to use the PIC18F PIC24F One-Way GUI, force the mComm module to follow the GUI's packet structure by defining this value:
* @code
* #define MCOMM_UART_1WAY_OUT_GUIv1_1
* @endcode
* The following options will determine what values are output by the UART. Matrix output is not supported by the GUI but may be seen from a terminal window.
* @code
* #define MCOMM_UART_1WAY_OUT_STATE             // <-- If defined, outputs the state mask
* #define MCOMM_UART_1WAY_OUT_TOGGLE            // <-- If defined, outputs the toggle state mask
* #define MCOMM_UART_1WAY_OUT_SLIDER            // <-- If defined, outputs the slider output value
* #define MCOMM_UART_1WAY_OUT_MATRIX            // <-- If defined, outputs the matrix press coordinate
* #define MCOMM_UART_1WAY_OUT_READING           // <-- If defined, outputs the raw reading values
* #define MCOMM_UART_1WAY_OUT_BASELINE          // <-- If defined, outputs the sensor baseline values
* @endcode
*
* If you are using a processor with an APFCON register that allows for multiple pins to be used as 'TX', your
* application is responsible for initializing APFCON to the correct value for your hardware layout. The mTouch
* framework will warn you about this when compiling. To remove this warning, uncomment the #define at the bottom
* of mTouch_config.h:
* @code
* #define APFCON_INITIALIZED    // For processors with an APFCON register(s), this 
*                               // #define can be uncommented to stop the mTouch 
*                               // Framework from producing a "remember to set 
*                               // APFCON" warning.
*                               //
*                               // RULE OF PROGRAMMING #4: Register bits initialize, 
*                               //      by law, to the value you don't want. Always 
*                               //      explicitly initialize.
* @endcode
*
* <li>Program the board normally (not in debug mode) and connect the PKSA to both the PC and the board.<br>
*
* <li>Make sure both the PIC and the PKSA are being supplied with power. Either:
*       <ul>
*       <li>Supply power to the PIC from an external source that is also supplying power to the PKSA's PWR and GND pins.
*       <li>Supply power to the PIC from the PKSA by pressing and holding the black button for 3 seconds until the green 
*           power LED turns on.
*       <li>Supply power to the PIC from an external source that is NOT connected to the PWR/GND pins of the PKSA and then 
*           supply power to the PKSA by pressing on its black button for 3 seconds until the green power LED turns on.
*       </ul>
*       @note Be careful not to supply power to the PIC from two different sources as you may destroy the board.
*
* <li>Is the PKSA's red LED on?<br>
*       If no -
*       <ul>
*       <li> Use a scope to verify the TX pin of the PKSA is receiving data from the PIC.
*       <li> If it is, the PKSA's firmware has not been updated to perform as a UART-to-USB converter. 
*            See @ref PKSARS232 "this guide" on using the PKSA loader utility to reflash the PICKit 
*            Serial Analyzer's firmware.
*       </ul>
*       If yes - continue to 5.
*
* <li>Determine which COM port has been assigned to the PKSA.<br>
*       In Windows - 
*       <ul>
*       <li> Right click 'My Computer' --> Properties --> Hardware --> Device Manager
*       <li> You should find it listed under "Ports (COM)".
*       </ul>
*       If it's not there, you need to reinstall the Windows driver for the UART-to-USB behavior. The
*       driver can be found in the PKSA Loader Utility's folder. Correct installation is based on the
*       version of Windows you are using, but can be accomplished by right clicking the file and selecting
*       'Install' in most cases.
*
* <li>Open the mTouch One-Way GUI and make sure the communication settings are set to:<br><br>
*       <ul>
*       <li> The baud rate you have chosen for #MTOUCH_UART_BAUDRATE in mComm_config.h.
*       <li> COM Port dependant on current assignment in Device Manager.
*       </ul>
* </ol>
*/

/**
* @page digCom Communication Implementation
*
* <b>This is for one-way communications and only describes the packet structure for the mTouch One-Way GUI</b>.
*
* The communication module of the mTouch Framework has been designed to talk to the included mTouch One-Way
* GUI by default but can easily be adapted to meet your needs. The framework handles all initialization
* requirements once it knows the PIC's oscillator speed and desired baud rate. 
*
* The data packets being sent to the GUI are as shown:
* @code
* 16bitMaskofSensorStates;Sensor0Reading;Sensor1Reading;...;SensorNReading(CR)(LF)
* @endcode
* which looks like this when populated:
* @code
* 00000;02301;02354;02332;02296;02318
* @endcode
* @note A @c 1 in the bit mask means the sensor is pressed. A @c 0 means that it is released.
*
* The data is sent in decimal format with a maximum of 5 digits by default.
*/

/**
* @page PKSARS232 Loading new firmware on the PICKit Serial Analyzer
*
* @section Intro Introduction
*   The PICKit Serial Analyzer (PKSA) is an easy way to connect the UART of your PIC&reg; microcontroller to your PC. 
*   All of the required steps are handled by the PICKit Serial Loader Utility provided with the framework in the
*   'Utilities' folder of the demos.
*
*   <tt>Your MLA Directory/mTouchCapDemos/Utilities/PIC12F PIC16F Utilities/PICKit Serial Loader</tt>
*
*   There are two different ways the PKSA can be used:
*   @li <b>UART-to-USB converter</b> :: UART_USB_PICKitSerial_V1.40.hex :: PKSA behaves as a COM port when connected to the PC.
*   @li <b>Default PKSA Behavior</b> :: PKS-0307-WITHBOOT-0103.HEX :: PKSA behaves as an HID device that is manipulated through a DLL.
*
*   An update to the default PKSA hex file is provided to add support for the RS-232 'break' character. This is required
*   for communicating with the mTouch two-way GUI.
*
* @section Convert Changing the PICKit Serial Analyzer's Hex File
*   <ol>
*   <li>Press the button on the PICKit Serial Analyzer when connecting to PC's USB to enter <tt>Bootloader Mode</tt>.<br>
*     (The 'Target' and 'Busy' LEDs should be flashing quickly.)
*   <li>Open the PICKit Serial Loader Utility provided by the framework in the Utilities folder of the demos.
*   <li>Follow the on-screen instructions to load the new hex file. Hex files and drivers are provided in the same folder 
*       as the loader utility. Make sure to select correct hex file depending on the desired behavior (UART-to-USB COM 
*       port / Updated Default PKSA).
*   </ol>
*
* @section Use Using your PICKit Serial as a UART-to-USB Converter
*   <ol>
*   <li>Unplug and replug the PKSA. Press and hold the black PKSA button to toggle the on-board 5V power supply.
*       <ul>
*       <li>Warning: There is no protection provided on this power supply. Maximum current is 20mA.
*       </ul>
*   <li>LED Status Legend:
*       <ul>
*       <li>Target LED On: Transmission in progress
*       <li>Busy LED On: Reception was successful
*       </ul>
*   </ol>
*
* @section UseNormal Using your PICKit Serial with the default PKSA firmware
*   <ol>
*   <li>Power your mTouch board and connect the PKSA.
*   <li>Open the mTouch Two-Way GUI. The program will begin communicating or will provide error messages with suggestions for resolve any issues.
*   </ol>
*
* @image html pksaExtVdd.JPG "Example schematic connecting the PKSA with an external power supply"
* @image html pksaVdd.JPG "Example schematic of PKSA supplying power to the board"
* @image html pksaExt5Vdd.JPG "Example schematic of connecting PKSA with an external 5V power supply. Notice - no VDD connection required."
*
*   <b>Maximum Speed</b>: 115.2 kbps<br>
*   <b>Voltage Level</b>: Depends on the voltage applied to pin #2. Valid range is from 3-5V.<br>
*   Optional 5V (<20mA) power supply from the PICKit Serial Analyzer.<br>
*
* @section Pinout PICKit Serial Analyzer Pin Configuration
*   <ol>
*   <li>The PC's TX line. The PIC's RX line. (Marked with an arrow)
*   <li>V<sub>DD</sub> : 3-5V
*   <li>V<sub>SS</sub>
*   <li>Aux1 : not used in RS232 configuration
*   <li>Aux2 : not used in RS232 configuration
*   <li>The PC's RX line. The PIC's TX line.
*   </ol>
* @image html pksaPins.JPG
*/
/**
* @page GettingStartedEval mTouch CVD Evaluation Kit Quick Start Guide 
*
* @note This guide assumes you are using the mTouch Evaluation kit as your hardware. For 
* details on where to begin when implementing the mTouch framework on a custom hardware
* design, use @ref GettingStartedCustom "this guide".
*
* Congratulations for the purchase of your new mTouch CVD Evaluation Kit.
*
* Although the kit will run right out of the box, this document will walk you through
* the setup process to quickly configure the kit. Should you need any custom configuration,
* this will get you up and running.
*
* The kit comes pre-programmed and set to operate in the configuration listed below. If
* you need to alter any of the parameters, then a new hex file will need to be generated
* and the main board will need to be reprogrammed with the new firmware. MPLAB IDE will
* then be required as well as a compatible C compiler.
*
* @section GSE_LAYOUTS Hardware Layouts / Configurations:
* Each of the board versions listed below has a matching demo project provided in:
*   <b><tt>Your MLA Directory/mTouchCapDemos/PIC16F_CVD_Demos</tt></b>
*
* @li @subpage  GSE_R3   "02-02091-R3"          :: CVD Eval Board
* @li @ref      GSE_R3   "233-04-2028 Rev A"    :: CVD Eval Board       (Use 02-02091-R3 project)
* @li @subpage  GSE_R2   "02-02091-R2"          :: CSM-CVD Eval Board
* @li @ref      GSE_R2   "02-02091-R1"          :: CSM-CVD Eval Board   (Use 02-02091-R2 project)
* @li @subpage  GSE_RF   "233-04-1008 Rev F"    :: CSM Eval Board
* @li @subpage  GSE_RA   "02-02091-RA"          :: CSM Eval Board
* 
* @image html evalNumber.jpg "Look on the sticker on the back of the evaluation board to match the project's folder name."
*
*
* @section GSE_GUI Communicating with the mTouch GUI:
* The demo hex files and projects  for the evaluation boards are preconfigured to work with 
* the mTouch Two-Way GUI out-of-the box. The default PICKit Serial Analyzer firmware will need 
* to be updated to the latest version. The PICKit Serial Loader utility has been provided to make 
* this easy. You can find it here: <tt>Your MLA Directory/mTouchCapDemos/Utilities/PIC12F PIC16F Utilities/PICKit Serial Loader</tt>
*
* You must load the <b>PKS-0307-WITHBOOT-0103.HEX</b> when requested by the loader utility. See 
* @ref PKSARS232 "this guide".
* 
* Once this is done, power the mTouch evaluation board externally (through any source
* other than the PKSA header), connect the PKSA to the board, and start the mTouch GUI.
*
* @section GSE_CNS Changing the Number of Sensors:
* If you need the stack to manage more or less than the default number, all you need to 
* do is update the constant #MTOUCH_NUMBER_SENSORS to any other value from 1 to 15. This
* constant can be found in the mTouch_config.h file.
*
* @section GSE_ASI Assigning Sensor Indexes to the Correct A/D Channels
* Depending on how the daughter board is connected to the main board, the sensors may
* be connected to different A/D inputs on the PIC microcontroller. In the mTouch
* framework, sensors can be mapped to a specific index in the application. Go to
* mTouch_config.h and look for the section with the definitions shown below, and
* map the sensor indexes on the left side to the correct A/D channels for your setup.
* @code
*   #define MTOUCH_SENSOR0             AN0  
*   #define MTOUCH_SENSOR1             AN1    
*   #define MTOUCH_SENSOR2             AN2     
*   #define MTOUCH_SENSOR3             AN3     
*   #define MTOUCH_SENSOR4             AN4     
*   #define MTOUCH_SENSOR5             AN5     
*   #define MTOUCH_SENSOR6             AN6     
*   #define MTOUCH_SENSOR7             AN7     
*   #define MTOUCH_SENSOR8             AN8     
*   #define MTOUCH_SENSOR9             AN9     
*   #define MTOUCH_SENSOR10            AN10    
*   #define MTOUCH_SENSOR11            AN11    
*   #define MTOUCH_SENSOR12            AN12    
*   #define MTOUCH_SENSOR13            AN13
* @endcode
*
* @section GSE_CAC Changing the Number of Samples per Scan
* The number of samples taken in an application will be based on a trade-off between
* sensitivity and response time. The more samples taken, the better the signal-to-noise
* ratio of the system, but the slower its response to a finger. Typical response times
* are less than 100ms to eliminate delay that is visible to the human eye. If a specific
* response time is required, example equations are provided in the configuration file
* to show how to calculate the rate that will be chosen based on this value.
*
* @section GSE_AST Adjusting the Sensor Threshold Values
* Your mTouch evaluation kit comes already preconfigured with the sensor threshold
* values for the 8-button daughter board. If different sensitivities are needed, go
* to the mTouch_config.h file, locate the section with the definitions shown
* below, and set the new values.
* A general rule of thumb to finding and setting correct values is to determine the
* range of shift for the sensor (min and max shift) and set the threshold to about
* 75% of the range. The following formula can be used: 
* @f[ Threshold_{Press} = min + 0.75 (max - min) @f]
* @code
*   #define THRESHOLD_PRESS_SENSOR0         65         
*   #define THRESHOLD_PRESS_SENSOR1         65
*   #define THRESHOLD_PRESS_SENSOR2         65
*   #define THRESHOLD_PRESS_SENSOR3         65
*   #define THRESHOLD_PRESS_SENSOR4         65
*   #define THRESHOLD_PRESS_SENSOR5         65
*   #define THRESHOLD_PRESS_SENSOR6         65
* @endcode
*
* @section Framework Configuration and Application API Hooks
* See the @ref featBasic "basic sensor configuration" guide and the @ref FrameworkFeatures "feature-specific help" section.
*/

/**
* @page GSE_R3  Quick Start Guide :: CVD Eval Board
*
* @li 02-02091-R3
* @li 233-04-2028 Rev A
*
* @code
* #define MTOUCH_SENSOR0              AN0         
* #define MTOUCH_SENSOR1              AN1         
* #define MTOUCH_SENSOR2              AN2    
* #define MTOUCH_SENSOR3              AN3   
* #define MTOUCH_SENSOR4              AN4
* #define MTOUCH_SENSOR5              AN5
* #define MTOUCH_SENSOR6              AN6
* #define MTOUCH_SENSOR7              AN7
* #define MTOUCH_SENSOR8              AN8 
* #define MTOUCH_SENSOR9              AN9
* #define MTOUCH_SENSOR10             AN10    
* #define MTOUCH_SENSOR11             AN11    
* #define MTOUCH_SENSOR12             AN12    
* #define MTOUCH_SENSOR13             AN13  
* 
* #define LED0  LATC2
* #define LED1  LATC1
* #define LED2  LATC0
* #define LED3  LATA7
* #define LED4  LATA6
* #define LED5  LATA4
* #define LED6  LATD7
* #define LED7  LATD6
* #define LED8  LATD5
* #define LED9  LATD4
* #define LED10 LATD3
* #define LED11 LATD2
* #define LED12 LATD1
* #define LED13 LATD0
* 
* ANSELA  = 0b00000000;
* ANSELB  = 0b00000000;
* ANSELD  = 0b00000000;
* ANSELE  = 0b00000000;
* LATA    = 0b11010000;
* LATB    = 0b00000000;
* LATC    = 0b00000111;
* LATD    = 0b11111111;
* LATE    = 0b00000000;
* TRISA   = 0b00000000;
* TRISB   = 0b00000000;
* TRISC   = 0b00000000;
* TRISD   = 0b00000000;
* TRISE   = 0b00000000;
* @endcode
*/

/**
* @page GSE_R2  Quick Start Guide :: CSM-CVD Eval Board
*
* @li 02-02091-R1
* @li 02-02091-R2
*
* @code
* #define MTOUCH_SENSOR0              AN12    
* #define MTOUCH_SENSOR1              AN10         
* #define MTOUCH_SENSOR2              AN8    
* #define MTOUCH_SENSOR3              AN9  
* #define MTOUCH_SENSOR4              AN11
* #define MTOUCH_SENSOR5              AN13
* #define MTOUCH_SENSOR6              AN4     // Labeled '7' on the board
* 
* #define LED0  LATC0
* #define LED1  LATA6
* #define LED2  LATA7
* #define LED3  LATE2
* #define LED4  LATE1
* #define LED5  LATE0
* #define LED6  LATA3
* #define LED7  LATA2
* #define LED8  LATA1
* #define LED9  LATB7     // ICSPDAT
* #define LED10 LATB6     // ICSPCLK
* #define LED11 LATC7     // RX
* #define LED12 LATC6     // TX
* #define LED13 LATC5
* #define LED14 LATC2
* #define LED15 LATC1
* 
* ANSELA  = 0b00000000;
* ANSELB  = 0b00000000;
* ANSELD  = 0b00000000;
* ANSELE  = 0b00000000;
* LATA    = 0b11001110;
* LATB    = 0b11000000;
* LATC    = 0b11100111;
* LATD    = 0b00000000;
* LATE    = 0b00000111;
* TRISA   = 0b00000000;
* TRISB   = 0b00000000;
* TRISC   = 0b00000000;
* TRISD   = 0b00000000;
* TRISE   = 0b00000000;
* @endcode
*/

/**
* @page GSE_RA  Quick Start Guide :: CSM Eval Board
*
* @li 02-02091-RA
*
* @code
* #define MTOUCH_SENSOR0              AN12         
* #define MTOUCH_SENSOR1              AN10         
* #define MTOUCH_SENSOR2              AN8    
* #define MTOUCH_SENSOR3              AN9  
* #define MTOUCH_SENSOR4              AN11
* #define MTOUCH_SENSOR5              AN13
* #define MTOUCH_SENSOR6              AN4     // Labeled '7' on the board
* 
* #define LED0  LATC0
* #define LED1  LATA6
* #define LED2  LATA7
* #define LED3  LATE2
* #define LED4  LATE1
* #define LED5  LATE0
* #define LED6  LATA3
* #define LED7  LATA2
* #define LED8  LATA1
* #define LED9  LATB7     // ICSPDAT
* #define LED10 LATB6     // ICSPCLK
* #define LED11 LATC7     // RX not connected to PKSA header
* #define LED12 LATC6     // TX not connected to PKSA header
* #define LED13 LATC5
* #define LED14 LATC2
* #define LED15 LATC1
* 
* ANSELA  = 0b00000000;
* ANSELB  = 0b00000000;
* ANSELD  = 0b00000000;
* ANSELE  = 0b00000000;
* LATA    = 0b11001110;
* LATB    = 0b11000000;
* LATC    = 0b11100111;
* LATD    = 0b00000000;
* LATE    = 0b00000111;
* TRISA   = 0b00000000;
* TRISB   = 0b00000000;
* TRISC   = 0b00000000;
* TRISD   = 0b00000000;
* TRISE   = 0b00000000;
* @endcode
*/

/**
* @page GSE_RF  Quick Start Guide :: CSM Eval Board
*
* @li 233-04-1008 Rev F
*
* @code
* #define MTOUCH_SENSOR0              AN12         
* #define MTOUCH_SENSOR1              AN10         
* #define MTOUCH_SENSOR2              AN8    
* #define MTOUCH_SENSOR3              AN9  
* #define MTOUCH_SENSOR4              AN11
* #define MTOUCH_SENSOR5              AN13
* #define MTOUCH_SENSOR6              AN4     // Labeled '7' on the board
* 
* #define LED0  LATC0
* #define LED1  LATA6
* #define LED2  LATA7
* #define LED3  LATE2
* #define LED4  LATE1
* #define LED5  LATE0
* #define LED6  LATA3
* #define LED7  LATA2
* #define LED8  LATA1
* #define LED9  LATB7     // ICSPDAT
* #define LED10 LATB6     // ICSPCLK
* #define LED11 LATC7     // RX
* #define LED12 LATC6     // TX
* #define LED13 LATC5
* #define LED14 LATC2
* #define LED15 LATC1
* 
* ANSELA  = 0b00000000;
* ANSELB  = 0b00000000;
* ANSELD  = 0b00000000;
* ANSELE  = 0b00000000;
* LATA    = 0b11001110;
* LATB    = 0b11000000;
* LATC    = 0b11100111;
* LATD    = 0b00000000;
* LATE    = 0b00000111;
* TRISA   = 0b00000000;
* TRISB   = 0b00000000;
* TRISC   = 0b00000000;
* TRISD   = 0b00000000;
* TRISE   = 0b00000000;
* @endcode
*/


/**
* @page GettingStartedCustom Custom Hardware Quick Start Guide
*
* @note This guide assumes you are using custom hardware. For details on where to begin 
* when implementing the mTouch framework on the mTouch CVD Evaluation Kit, use 
* @ref GettingStartedEval "this guide".
*
* @section GS_C_NewProject  Create a new MPLAB Project
* First, create a new @ref GettingStartedNewP8 "MPLAB 8" or @ref GettingStartedNewPX "MPLAB X" project for your new application.
*
* @section GS_SysConfig System Configuration
* The mTouch_Init() function will automatically configure the ADC, communications module (if enabled),
* and all required interrupts. There are only a few things the user is required to define on power-up:
* <ol>
* <li>Configure the oscillator to provide a fast clock speed.
* @code
* OSCCON = 0b01110000;       // 32 MHz Fosc w/ PLLEN_ON config word on PIC16F1937
* @endcode
* <br>
* <li>  Set the prescaler for the timer you are using for the mTouch scan interrupt. (Weak pull-up 
*       resistors should be disabled.)<br>
*       For more information about the implications of this setting, see @ref rrMemory "Processing Requirements".
* @code
* OPTION_REG = 0b10000000;   // TMR0 Prescaler = 1:2 (~40% processor usage)
* @endcode
* <br>
* <li>Sensor pins should be initialized as digital output low.
* @code
* ANSELA = 0b00000000;       // Digital
* TRISA  = 0b00000000;       // Output
* PORTA  = 0b00000000;       // Low   
* @endcode
* </ol>
* <br>
* @section GS_InitConfig Framework Configuration and Application API Hooks
* See the @ref featBasic "basic sensor configuration" guide and the @ref FrameworkFeatures "feature-specific help" section.
*/
/**
* @page GettingStartedNewP8 Creating a new MPLAB 8 mTouch Framework Project
*
* <b>The easiest way</b> to start a new project is to copy a fresh version of the source directory to your project
* folder. The source directory contains an MPLAB 8 project which can then be used to start your application.
* @li <tt>[Your MLA Directory]/Microchip/mTouchCap/PIC12F PIC16F CVD Library/</tt>
* 
* <b>If you already have an application project</b> and wish to integrate the mTouch Framework with it:
* <ol>
* <li>  Copy the files in <tt>Your MLA Directory/Microchip/mTouchCap/mTouch Framework/</tt> to your local project directory.
*   <ul>
*   <li>Including the <tt>Alternative Configurations/</tt> directory!
*   </ul>
*
* <li>  Add all <tt>.c</tt> files to your MPLAB 8 project.
*
* <li>  Use the following line of code to include the mTouch framework in your project:<br>
* @code
* #include "mTouch.h"
* @endcode
* </ol>
*
* Now check out the main.c file located in the framework's folder or one of the main.c files in
* the mTouchCapDemos folder for suggestions on using the mTouch Framework's API.
*/
/**
* @page GettingStartedNewPX Creating a new MPLAB X mTouch Framework Project
*
* <b>The easiest way to start a new project</b> is to copy a fresh version of the source directory to your project
* folder. The source directory contains an MPLAB X project which can then be used to start your application.
* @li <tt>[Your MLA Directory]/Microchip/mTouchCap/PIC12F PIC16F CVD Library/</tt>
*
* <b>If you already have an application project</b> and wish to integrate the mTouch Framework with it:
* <ol>
* <li>  Copy the files in <tt>Your MLA Directory/Microchip/mTouchCap/mTouch Framework/</tt> to your local project directory.
*   <ul>
*   <li>Including the <tt>Alternative Configurations/</tt> directory!
*   </ul>
*
* <li>  Add all <tt>.c</tt> files to your MPLAB X project.
*
* <li>  Use the following line of code to include the mTouch framework in your project:<br>
* @code
* #include "mTouch.h"
* @endcode
* </ol>
*
* Now check out the main.c file located in the framework's folder or one of the main.c files in
* the mTouchCapDemos folder for suggestions on using the mTouch Framework's API.
*/
/**
* @page RequiredIncludes Required Source Files
*
* The mTouch Framework will automatically choose the correct header files for your chosen device.
*
* These configuration files will have unique settings for each application:
* @li C:/YourApplicationSourceDirectory/mTouch_config.h
* @li C:/YourApplicationSourceDirectory/mTouch_config_slider.h
* @li C:/YourApplicationSourceDirectory/mTouch_config_modes.h
* @li C:/YourApplicationSourceDirectory/mTouch_config_cvdAdvanced.h
*
* These source files are required in the project:
* @li C:/YourApplicationSourceDirectory/mTouch.c
* @li C:/YourApplicationSourceDirectory/mTouch_acquisition.c
* @li C:/YourApplicationSourceDirectory/mTouch_eeprom.c - Only if EEPROM is enabled in mTouch_config.h
* @li C:/YourApplicationSourceDirectory/mTouch_proximity.c - Only if proximity sensors are enabled in mTouch_config.h
* @li C:/YourApplicationSourceDirectory/mTouch_slider.c - Only if sliders are enabled in mTouch_config_slider.h
*
* @section RI_HardwareProfile Hardware Profile
* Each framework-supported PIC microcontroller has an associated HardwareProfile header file
* to define the relationship between the AN channel and its port/pin assignment, how to initialize
* the communications module, and which communications methods are available. This file will
* also perform some error checking on the current configuration to determine if your microcontroller
* will be able to correctly implement the requested framework features.
*
* <i>Only the hardware profile that is associated with your microcontroller's family is required.</i>
*
* Hardware Profile Options:
* @li Alternative Configurations/mTouch_hardwareProfile_12F61x.h  :: PIC12F615, PIC12F617, PIC12HV615
* @li Alternative Configurations/mTouch_hardwareProfile_16F151x.h :: PIC16F1516, PIC16F1517, PIC16F1518, PIC16F1519
* @li Alternative Configurations/mTouch_hardwareProfile_16F152x.h :: PIC16F1526, PIC16F1527
* @li Alternative Configurations/mTouch_hardwareProfile_16F182x.h :: PIC12F1822, PIC16F1823, PIC16F1824, PIC16F1825, PIC16F1826, PIC16F1827, PIC16F1828, PIC16F1829
* @li Alternative Configurations/mTouch_hardwareProfile_16F193x.h :: PIC16F1933, PIC16F1934, PIC16F1936, PIC16F1937, PIC16F1938, PIC16F1939
* @li Alternative Configurations/mTouch_hardwareProfile_16F194x.h :: PIC16F1946, PIC16F1947
* @li and others... see <tt>C:/Your MLA Directory/Microchip/mTouchCap/PIC12F PIC16F CVD Library/Alternative Configurations</tt> for a full listing
* 
*
* @section RI_MacroLibrary Macro Library
* The mTouch acquisition function is implemented using a combination of C and assembly. To support 
* multiple cores with different assembly instruction sets, separate macro definitions are required.
* These macros perform the mTouch scanning routine and initial signal processing on the results. Due
* to noise concerns, we do not recommend editting these macros.
*
* <i>Only the macro library that is associated with your microcontroller's instruction set + mTouch_macroLibrary_common.h are required.</i>
* 
* The correct macro library header file for your processor's core is automatically chosen from the list:
* @li mTouchCVD_macroLibrary_PIC16F.h :: PIC12/16 Non-Enhanced Core, Midrange Devices
* @li mTouchCVD_macroLibrary_PIC16F1.h :: PIC16F1 Enhanced Core, Midrange Devices
* @li mTouchCVD_macroLibrary_PIC18F.h :: PIC18 Devices
*/

/**
* @page SoftwareLicense License Agreement
*
* <b>MICROCHIP IS WILLING TO LICENSE THE ACCOMPANYING SOFTWARE AND DOCUMENTATION TO YOU ONLY ON 
*    THE CONDITION THAT YOU ACCEPT ALL OF THE FOLLOWING TERMS.  TO ACCEPT THE TERMS OF THIS LICENSE, 
*    CLICK "I ACCEPT" AND PROCEED WITH THE DOWNLOAD OR INSTALL.  IF YOU DO NOT ACCEPT THESE LICENSE 
*    TERMS, CLICK "I DO NOT ACCEPT," AND DO NOT DOWNLOAD OR INSTALL THIS SOFTWARE. </b>
* 
* <center><b>NON-EXCLUSIVE SOFTWARE LICENSE AGREEMENT</b></center>
*
* This Nonexclusive Software License Agreement ("Agreement") is a contract between you, your heirs, 
* successors and assigns ("Licensee") and Microchip Technology Incorporated, a Delaware corporation, 
* with a principal place of business at 2355 W. Chandler Blvd., Chandler, AZ 85224-6199, and its 
* subsidiary, Microchip Technology (Barbados) II Incorporated (collectively, "Microchip") for the 
* accompanying Microchip software including, but not limited to, Graphics Library Software, IrDA Stack 
* Software, MCHPFSUSB Stack Software, Memory Disk Drive File System Software, mTouch(TM) Capacitive 
* Library Software, Smart Card Library Software, TCP/IP Stack Software, MiWi(TM) DE Software, Security 
* Package Software, and/or any PC programs and any updates thereto (collectively, the "Software"), 
* and accompanying documentation, including images and any other graphic resources provided by 
* Microchip ("Documentation").
*
* <ol>
* <li><b>Definitions.</b> As used in this Agreement, the following capitalized terms will have 
*     the meanings defined below:
*
*       <ol type="a">
*       <li><b>"Microchip Products"</b> means Microchip microcontrollers and Microchip digital signal controllers.
*       <li><b>"Licensee Products"</b> means Licensee products that use or incorporate Microchip Products.
*       <li><b>"Object Code"</b> means the Software computer programming code that is in binary form (including 
*           related documentation, if any), and error corrections, improvements, modifications, and updates.
*       <li><b>"Source Code"</b> means the Software computer programming code that may be printed out or 
*           displayed in human readable form (including related programmer comments and documentation, if any), 
*           and error corrections, improvements, modifications, and updates.
*       <li><b>"Third Party"</b> means Licensee's agents, representatives, consultants, clients, customers, or 
*           contract manufacturers.
*       <li><b>"Third Party Products"</b> means Third Party products that use or incorporate Microchip Products.
*       </ol>
*
* <li><b>Software License Grant.</b> Microchip grants strictly to Licensee a non-exclusive, 
*     non-transferable, worldwide license to:
*
*       <ol type="a">
*       <li>use the Software in connection with Licensee Products and/or Third Party Products; 
*       <li>if Source Code is provided, modify the Software; provided that Licensee clearly notifies Third 
*           Parties regarding the source of such modifications;
*       <li>distribute the Software to Third Parties for use in Third Party Products, so long as such Third 
*           Party agrees to be bound by this Agreement (in writing or by "click to accept") and this Agreement 
*           accompanies such distribution; 
*       <li>sublicense to a Third Party to use the Software, so long as such Third Party agrees to be bound by 
*           this Agreement (in writing or by "click to accept");
*       <li>with respect to the TCP/IP Stack Software, Licensee may port the ENC28J60.c,  ENC28J60.h, ENCX24J600.c, 
*           and ENCX24J600.h driver source files to a non-Microchip Product used in conjunction with a Microchip 
*           ethernet controller; 
*       <li>with respect to the MiWi (TM) DE Software, Licensee may only exercise its rights when the Software is 
*           embedded on a Microchip Product and used with a Microchip radio frequency transceiver or UBEC UZ2400 
*           radio frequency transceiver which are integrated into Licensee Products or Third Party Products. 
*       </ol>
*
*   For purposes of clarity, Licensee may NOT embed the Software on a non-Microchip Product, except as described in this Section.  
*
* <li><b>Documentation License Grant.</b>  Microchip grants strictly to Licensee a non-exclusive, 
*     non-transferable, worldwide license to use the Documentation in support of Licensee's authorized use of 
*     the Software
*
* <li><b>Third Party Requirements.</b> Licensee acknowledges that it is Licensee's responsibility to comply with 
*     any third party license terms or requirements applicable to the use of such third party software, specifications,
*     systems, or tools.  This includes, by way of example but not as a limitation, any standards setting 
*     organizations requirements and, particularly with respect to the Security Package Software, local encryption 
*     laws and requirements.  Microchip is not responsible and will not be held responsible in any manner for 
*     Licensee's failure to comply with such applicable terms or requirements. 
*
* <li><b>Open Source Components.</b> Notwithstanding the license grant in Section 1 above, Licensee further 
*     acknowledges that certain components of the Software may be covered by so-called "open source" software 
*     licenses ("Open Source Components").  Open Source Components means any software licenses approved as open 
*     source licenses by the Open Source Initiative or any substantially similar licenses, including without 
*     limitation any license that, as a condition of distribution of the software licensed under such license, 
*     requires that the distributor make the software available in source code format.  To the extent required 
*     by the licenses covering Open Source Components, the terms of such license will apply in lieu of the terms 
*     of this Agreement.  To the extent the terms of the licenses applicable to Open Source Components prohibit 
*     any of the restrictions in this Agreement with respect to such Open Source Components, such restrictions 
*     will not apply to such Open Source Component.
*
* <li><b>Licensee Obligations.</b>  Licensee will not: (a) engage in unauthorized use, modification, disclosure 
*     or distribution of Software or Documentation, or its derivatives; (b) use all or any portion of the 
*     Software, Documentation, or its derivatives except in conjunction with Microchip Products, Licensee Products 
*     or Third Party Products; or (c) reverse engineer (by disassembly, decompilation or otherwise) Software or 
*     any portion thereof.  Licensee may not remove or alter any Microchip copyright or other proprietary rights 
*     notice posted in any portion of the Software or Documentation.  Licensee will defend, indemnify and hold 
*     Microchip and its subsidiaries harmless from and against any and all claims, costs, damages, expenses 
*     (including reasonable attorney's fees), liabilities, and losses, including without limitation: (x) any 
*     claims directly or indirectly arising from or related to the use, modification, disclosure or distribution 
*     of the Software, Documentation, or any intellectual property rights related thereto; (y) the use, sale and 
*     distribution of Licensee Products or Third Party Products; and (z) breach of this Agreement.  
*
* <li><b>Confidentiality.</b>  Licensee agrees that the Software (including but not limited to the Source Code, 
*     Object Code and library files) and its derivatives, Documentation and underlying inventions, algorithms, 
*     know-how and ideas relating to the Software and the Documentation are proprietary information belonging to 
*     Microchip and its licensors ("Proprietary Information").  Except as expressly and unambiguously allowed 
*     herein, Licensee will hold in confidence and not use or disclose any Proprietary Information and will 
*     similarly bind its employees and Third Party(ies) in writing.  Proprietary Information will not include 
*     information that: (i) is in or enters the public domain without breach of this Agreement and through no 
*     fault of the receiving party; (ii) the receiving party was legally in possession of prior to receiving it; 
*     (iii) the receiving party can demonstrate was developed by the receiving party independently and without 
*     use of or reference to the disclosing party's Proprietary Information; or (iv) the receiving party receives 
*     from a third party without restriction on disclosure.  If Licensee is required to disclose Proprietary 
*     Information by law, court order, or government agency, License will give Microchip prompt notice of such 
*     requirement in order to allow Microchip to object or limit such disclosure.  Licensee agrees that the 
*     provisions of this Agreement regarding unauthorized use and nondisclosure of the Software, Documentation 
*     and related Proprietary Rights are necessary to protect the legitimate business interests of Microchip and 
*     its licensors and that monetary damage alone cannot adequately compensate Microchip or its licensors if 
*     such provisions are violated.  Licensee, therefore, agrees that if Microchip alleges that Licensee or Third 
*     Party has breached or violated such provision then Microchip will have the right to injunctive relief, 
*     without the requirement for the posting of a bond, in addition to all other remedies at law or in equity.
*
* <li><b>Ownership of Proprietary Rights.</b>  Microchip and its licensors retain all right, title and interest 
*     in and to the Software and Documentation including, but not limited to all patent, copyright, trade secret 
*     and other intellectual property rights in the Software, Documentation, and underlying technology and all 
*     copies and derivative works thereof (by whomever produced).  Licensee and Third Party use of such 
*     modifications and derivatives is limited to the license rights described in this Agreement. 
*
* <li><b>Termination of Agreement.</b> Without prejudice to any other rights, this Agreement terminates 
*     immediately, without notice by Microchip, upon a failure by Licensee or Third Party to comply with any 
*     provision of this Agreement.  Upon termination, Licensee and Third Party will immediately stop using the 
*     Software, Documentation, and derivatives thereof, and immediately destroy all such copies.
*
* <li><b>Warranty Disclaimers.</b> THE SOFTWARE AND DOCUMENTATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY 
*     KIND, EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF MERCHANTABILITY, TITLE, 
*     NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE.  MICROCHIP AND ITS LICENSORS ASSUME NO RESPONSIBILITY 
*     FOR THE ACCURACY, RELIABILITY OR APPLICATION OF THE SOFTWARE OR DOCUMENTATION.  MICROCHIP AND ITS LICENSORS 
*     DO NOT WARRANT THAT THE SOFTWARE WILL MEET REQUIREMENTS OF LICENSEE OR THIRD PARTY, BE UNINTERRUPTED OR 
*     ERROR-FREE.  MICROCHIP AND ITS LICENSORS HAVE NO OBLIGATION TO CORRECT ANY DEFECTS IN THE SOFTWARE.
*
* <li><b>Limited Liability.</b> IN NO EVENT WILL MICROCHIP OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER ANY LEGAL 
*     OR EQUITABLE THEORY FOR ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES INCLUDING BUT NOT LIMITED TO INCIDENTAL, 
*     SPECIAL, INDIRECT, PUNITIVE OR CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT OF 
*     SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES (INCLUDING BUT NOT LIMITED TO ANY 
*     DEFENSE THEREOF), OR OTHER SIMILAR COSTS.  The aggregate and cumulative liability of Microchip and its 
*     licensors for damages hereunder will in no event exceed $1000 or the amount Licensee paid Microchip for the 
*     Software and Documentation, whichever is greater.  Licensee acknowledges that the foregoing limitations are 
*     reasonable and an essential part of this Agreement.
*
* <li><b>General.</b>  THIS AGREEMENT WILL BE GOVERNED BY AND CONSTRUED UNDER THE LAWS OF THE STATE OF ARIZONA 
*     AND THE UNITED STATES WITHOUT REGARD TO CONFLICTS OF LAWS PROVISIONS.  Licensee agrees that any disputes 
*     arising out of or related to this Agreement, Software or Documentation will be brought exclusively in 
*     either the U.S. District Court for the District of Arizona, Phoenix Division, or the Superior Court of 
*     Arizona located in Maricopa County, Arizona.  This Agreement will constitute the entire agreement between 
*     the parties with respect to the subject matter hereof.  It will not be modified except by a written 
*     agreement signed by an authorized representative of Microchip.  If any provision of this Agreement will be 
*     held by a court of competent jurisdiction to be illegal, invalid or unenforceable, that provision will be 
*     limited or eliminated to the minimum extent necessary so that this Agreement will otherwise remain in full 
*     force and effect and enforceable.  No waiver of any breach of any provision of this Agreement will 
*     constitute a waiver of any prior, concurrent or subsequent breach of the same or any other provisions 
*     hereof, and no waiver will be effective unless made in writing and signed by an authorized representative 
*     of the waiving party.  Licensee agrees to comply with all import and export laws and restrictions and 
*     regulations of the Department of Commerce or other United States or foreign agency or authority. The 
*     indemnities, obligations of confidentiality, and limitations on liability described herein, and any right 
*     of action for breach of this Agreement prior to termination, will survive any termination of this Agreement. 
*     Any prohibited assignment will be null and void.  Use, duplication or disclosure by the United States 
*     Government is subject to restrictions set forth in subparagraphs (a) through (d) of the Commercial 
*     Computer-Restricted Rights clause of FAR 52.227-19 when applicable, or in subparagraph (c)(1)(ii) of the 
*     Rights in Technical Data and Computer Software clause at DFARS 252.227-7013, and in similar clauses in the 
*     NASA FAR Supplement.  Contractor/manufacturer is Microchip Technology Inc., 2355 W. Chandler Blvd., 
*     Chandler, AZ 85224-6199.
* </ol>
*
* If Licensee has any questions about this Agreement, please write to Microchip Technology Inc., 2355 W. Chandler 
* Blvd., Chandler, AZ 85224-6199 USA. ATTN: Marketing.
*
* Copyright (c) 2012 Microchip Technology Inc.  All rights reserved.
*
* License Rev. No. 05-012412
*
*/






//=================================================================================
//   ____                           
//  / ___|_ __ ___  _   _ _ __  ___ 
// | |  _| '__/ _ \| | | | '_ \/ __|
// | |_| | | | (_) | |_| | |_) \__ \
//  \____|_|  \___/ \__,_| .__/|___/
//                       |_|   
//=================================================================================

/**
* @defgroup Configuration Configuration Options
*
* The mTouch Configuration Options provide an easy way to manipulate the behavior of the
* framework while staying focused on the end application. Detailed explanations of each
* option are provided below.
*/
/**
* @defgroup Acquisition Acquisition
* @brief Generates and executes a configured version of the CVD acquisition routine
*
* The mTouch Acquisition module generates an assembly implementation of the interrupt
* service routine that is designed to maximize noise immunity and minimize execution
* time. It will automatically add and remove macros as the number of sensors in the
* application changes. By default, it will use the previously scanned sensor as the
* reference for the current scan. If the application only has a single sensor, a special
* acquisition method which does not require a reference sensor is used. This specialized
* method should not be used in systems with more than one sensor due to crosstalk
* concerns.
*/
/**
* @defgroup Decoding Filtering and Decoding
*
* The mTouch Filtering and Decoding module executes the detection algorithms for the
* framework. Using the latched output from the acquisition module, the service function
* is called each time a new value is available for each sensor. It will then loop
* through all of the active sensors and determine if they have changed state.
*
* This is achieved by implementing a baseline average of the sensor's reading. The
* baseline is only updated while the sensor is in the #MTOUCH_RELEASED state. The difference
* between the baseline and the current reading is compared against the #THRESHOLD_PRESS_SENSOR0
* (or equivalent) value to determine if a change in state is necessary.
*
* If the system sees that the reading has dipped below the baseline - the opposite direction
* of a finger press - the baseline will update itself based on the behavior chosen by
* configuration option #MTOUCH_NEGATIVE_CAPACITANCE. 
*/


//=================================================================================
//   ____             __ _          ___        _   _                 
//  / ___|___  _ __  / _(_) __ _   / _ \ _ __ | |_(_) ___  _ __  ___ 
// | |   / _ \| '_ \| |_| |/ _` | | | | | '_ \| __| |/ _ \| '_ \/ __|
// | |__| (_) | | | |  _| | (_| | | |_| | |_) | |_| | (_) | | | \__ \
//  \____\___/|_| |_|_| |_|\__, |  \___/| .__/ \__|_|\___/|_| |_|___/
//                         |___/        |_|    
//=================================================================================
/** @name System Setup (mTouch_config.h)
*
*   These configuration options must be uniquely set for each application. They
*   define the manner in which the framework will be implemented in the application.
*/
//@{
    /**
    * @def      _XTAL_FREQ
    * @ingroup  Configuration
    * @brief    The oscillator frequency of your system in Hz
    * @hideinitializer
    */
    
    /**
    * @def      MTOUCH_INTEGRATION_TYPE
    * @ingroup  Configuration
    * @brief    Defines the manner in which the framework will integrate with the 
    *           application.
    *
    *   Valid options:
    *   @li     #MTOUCH_CONTROLS_ISR :: The framework defines and controls the entire ISR
    *   @li     #MTOUCH_CALLED_FROM_ISR :: Your application's ISR function is responsible 
    *           for calling mTouch_Scan(). Example code is provided in main.c
    *   @li     #MTOUCH_CALLED_FROM_MAINLOOP :: The mainloop mTouch API will automatically 
    *           call the scan function.    
    *
    * @hideinitializer
    */
    
    /**
    * @def      MTOUCH_ISR_TIMER
    * @ingroup  Configuration
    * @brief    If the mTouch scanning routine is called from the ISR, this defines which 
    *           timer's interrupt flag is used.
    *
    *   8-bit Timers Only. Not compatible with TMR1/3/5. 
    *
    *   @note   You are responsible for initializing the timer with any desired pre- or 
    *           post-scaling options including any period registers (PR2/4/6). 
    *
    * @hideinitializer
    */
    
    /**
    * @def      MTOUCH_SCAN_FUNCTIONALITY
    * @ingroup  Configuration
    * @brief    Determines how many sensors are scanned in a single 'scan' function call
    *
    *   Valid options:
    *   @li     #MTOUCH_SCANS_ONE_SENSOR :: One mTouch function call will result in one 
    *           sensor being scanned once.
    *   @li     #MTOUCH_SCANS_ALL_SENSORS :: One mTouch function call will result in all 
    *           sensors being scanned once.
    *
    * @hideinitializer
    */
    
    /**
    * @def      MTOUCH_ADC_CONTROL
    * @ingroup  Configuration
    * @brief    Determines whether or not the framework controls the ADC at all times
    *
    *   Valid options:
    *   @li     #MTOUCH_ALWAYS_CONTROLS_ADC :: ADC is considered under the control of the 
    *           framework at all times. This is the more efficient method of implementation.
    *   @li     #MTOUCH_RELEASES_ADC_AFTER_SCAN :: ADC is released and available for use 
    *           between scans. This is less efficient, but provides greater flexibility.
    *
    * @hideinitializer
    */
//@}



/** @name Sensor Setup (mTouch_config.h)
*
*   These configuration options initialize the sensors in the framework by defining
*   which analog pins are to be scanned and how much sensitivity is expected from
*   them (thresholds).
*/
//@{
    /**
    * @def      MTOUCH_NUMBER_SENSORS
    * @ingroup  Configuration
    * @brief    Number of active-mode mTouch sensor inputs to scan
    * @hideinitializer
    */
    
    /**
    * @def      MTOUCH_SENSOR0
    * @ingroup  Configuration
    * @brief    Analog port assignment for the active-mode sensor with index 0
    * @hideinitializer
    */
    
    /**
    * @def      THRESHOLD_PRESS_SENSOR0
    * @ingroup  Configuration
    * @brief    Defines the press threshold of the active-mode sensor with index 0
    *
    *   This is how far the reading must deviate from the average/baseline before it 
    *   exits the #MTOUCH_RELEASED state and enters the #MTOUCH_PRESSED state. The 
    *   default values are only placeholders and should be edited to fit your design.
    *
    *   The release threshold is automatically calculated off this value and the value
    *   of #MTOUCH_RELEASE_THRESH_FACTOR. This allows the user to focus on where to 
    *   place a single threshold and the other value will follow it automatically.
    *
    * @attention To determine this value, it is highly recommended to look at the 
    *            communications output and see how your sensors are behaving. This is
    *            the only way to safely choose a threshold on any new design.
    * 
    * @hideinitializer
    */
    
    /**
    * @def      MTOUCH_RELEASE_THRESH_FACTOR
    * @ingroup  Configuration
    * @brief    Defines the hysteresis relationship between the press and release 
    *           thresholds
    *
    *   This value will be multiplied by each of the THRESHOLD_PRESS_SENSOR#
    *   values to calculate individual release thresholds for each sensor.
    *
    *   If your system is not releasing quickly enough, make this number slightly 
    *   larger to raise the threshold. If your system is releasing too quickly, make 
    *   this number slightly smaller.
    *
    *   For example:
    *
    *       RELEASE_THRESHOLD_FACTOR is 0.5<br>
    *       THRESHOLD_PRESS_SENSOR0 is 100.<br>
    *       THRESHOLD_PRESS_SENSOR1 is 150.
    *
    *       @code
    *       THRESHOLD_RELEASE_SENSOR0 = THRESHOLD_PRESS_SENSOR0 * RELEASE_THRESHOLD_FACTOR
    *       @endcode
    *
    *   This means the reading on the active-mode sensor with index '0' will need to 
    *   shift up, during a press, more than 100 counts above the baseline in order to 
    *   trigger a press. Once there, the baseline will stop updating and the sensor 
    *   will remain in the pressed state until the reading returns to within 50 counts 
    *   of the baseline <b>or</b> until the press timeout counter has completed. 
    *   Likewise, the active-mode sensor with index '1' will need to shift up by 150 
    *   and then down to below 75 in order to enter and exit the pressed state.
    *
    * @hideinitializer
    */
//@}


/** @name UART-PC Communications Setup (mComm_config.h)
*
*   These configuration options define if and how the mTouch framework is to 
*   communicate with a PC.
*
*   @attention  It is highly recommended that you look at the raw data output 
*               to determine how the system is behaving and where to place the 
*               thresholds.
*/
//@{
    /**
    * @def      MCOMM_ENABLED
    * @ingroup  Configuration
    * @brief    If enabled, communications will be implemented as defined in mComm_config.h
    * @hideinitializer
    */
    /**
    * @def      MCOMM_TYPE
    * @ingroup  Configuration
    * @brief    Defines one of four available communication options: #MCOMM_UART_ONE_WAY, 
    *           #MCOMM_UART_TWO_WAY, #MCOMM_I2C_TWO_WAY, and #MCOMM_SPI_TWO_WAY.
    *
    * @li #MCOMM_UART_ONE_WAY :: Transmit-only <b>ASCII</b> UART communications - Hardware/Software UART
    *   @note   To use the PICKit Serial with this option, the UART-to-USB hex file must be loaded into 
    *           the PKSA. This option should be selected to view data from a terminal window or to 
    *           communicate with any of the Profilab GUIs.
    * @li #MCOMM_UART_TWO_WAY :: Bidirectional <b>Binary</b> UART communications - UART module required.
    *   @note   To use the PICKit Serial with this option, the default PKSA's firmware must be updated to 
    *           support RS-232 break characters. This option should be selected to use the two-way mTouch 
    *           GUI to edit configuration values while plotting the sensors' data.
    * @li #MCOMM_I2C_TWO_WAY :: Bidirectional <b>Binary</b> I2C communications - SSP module required  
    *   @note   None of the provided mTouch GUIs use this method of communication.
    * @li #MCOMM_SPI_TWO_WAY :: Bidirectional <b>Binary</b> SPI communications - SSP module required
    *   @note   None of the provided mTouch GUIs use this method of communication.
    * @hideinitializer
    */
    /**
    * @def      MCOMM_UART_ONE_WAY
    * @brief    Option for #MCOMM_TYPE :: Transmit-only ASCII UART communications - Hardware/Software UART
    * @hideinitializer
    */
    /**
    * @def      MCOMM_UART_TWO_WAY
    * @brief    Option for #MCOMM_TYPE :: Bidirectional Binary UART communications - UART module required.
    * @hideinitializer
    */
    /**
    * @def      MCOMM_I2C_TWO_WAY
    * @brief    Option for #MCOMM_TYPE :: Bidirectional Binary I2C communications - SSP module required   
    * @hideinitializer
    */
    /**
    * @def      MCOMM_SPI_TWO_WAY
    * @brief    Option for #MCOMM_TYPE :: Bidirectional Binary SPI communications - SSP module required
    * @hideinitializer
    */
    /**
    * @def      MCOMM_UART_BAUDRATE
    * @ingroup  Configuration
    * @brief    If UART is being used, this defines the communication baud rate.
    *
    * Valid UART baudrate options depend on whether the UART communications will be implemented 
    * in hardware or software.
    *
    * Choose one of the listed speeds and compile. If the mComm module is unable to support the
    * requested speed given your current Fosc value, error messages will guide you to choosing a
    * better alternative.
    *
    * (bps)
    * @li 1200 
    * @li 2400 
    * @li @b 9600 *
    * @li @b 19200 *
    * @li @b 38400 *
    * @li 57600
    * @li 115200
    *
    * <b>* Bolded options</b> are compatible with the software-implementation of the UART in most
    * Fosc configurations.
    * @hideinitializer
    */
//@}


/** @name Filtering Setup (mTouch_config.h)
*
*   These configuration options define if and how the mTouch framework is to 
*   communicate with a PC.
*/
//@{
    /**
    * @def      MTOUCH_SAMPLES_PER_SCAN
    * @ingroup  Configuration
    * @brief    Determines the degree of oversampling and the sample rate of your system
    *
    *   This is the number of times each sensor will be scanned before asserting the 
    *   mTouch_state.dataReady bit. If the application requires a specific response 
    *   time, there are several variables that  will be important to the calculation 
    *   of this parameter.
    *
    *   @li _XTAL_FREQ is the PICs oscillation frequency and will determine how quickly 
    *       the TMR0 counter will increment.
    *
    *   @li The interrupt timer's pre/postscaler also determines how quickly the counter 
    *       will increment. This value is not set by the framework.
    *
    *   @li MTOUCH_NUMBER_SENSORS defines how many times the ISR will interrupt between 
    *       each decrement of the sample counter. The sample counter is initialized
    *       with MTOUCH_SAMPLES_PER_SCAN as its value.
    *
    * @hideinitializer
    */
    /**
    * @def      MTOUCH_BASELINE_WEIGHT
    * @ingroup  Configuration
    * @brief    Determines the weight given to the raw value when updating the baseline.
    *
    *   When the average updates itself using a new reading, this value determines what 
    *   weight is given to the reading variable in the calculation of the new baseline.
    *
    *   The reading variable will have a weight of 1/MTOUCH_BASELINE_WEIGHT in the baseline 
    *   calculation. 
    *
    *   Example:
    *
    *   If MTOUCH_BASELINE_WEIGHT is 4, the baseline calculation would be:
    *   @code
    *   baseline = (1/4)*reading + (3/4)*baseline
    *   @endcode
    *   <b>Allowed Values :: 1 - 4</b>
    *
    * @hideinitializer
    */
    /**
    * @def      MTOUCH_BASELINE_RATE
    * @ingroup  Configuration
    * @brief    Sets the update rate of the baseline when unpressed.
    *
    *   This value is used to seed a counter variable that will cause the baseline to be 
    *   updated only once every Nth new reading value, where N is MTOUCH_BASELINE_RATE.
    *
    *   Example:
    *
    *   If MTOUCH_BASELINE_RATE is 2, the baseline will use every other new reading to update 
    *   itself.
    *
    *   <b>Allowed Values :: 1 - 65535</b>
    *
    * @hideinitializer
    */
    /**
    * @def      MTOUCH_JITTER_BITS
    * @ingroup  Configuration
    * @brief    Defines the number of bits to use to implement a random delay between
    *           mTouch scans.
    *
    *   A random value is loaded into the interrupt timer to vary the interrupt's rate.
    *   The maximum number of bits of the random value is determined by this option.
    *   It is set to a value of '6' by default. More bits = more randomness.
    *
    *   <b>Allowed Values :: 0 - 8</b>
    *
    * @hideinitializer
    */
    /**
    * @def      MTOUCH_DECIMATION_MAX_STEP
    * @ingroup  Configuration
    * @brief    Maximum allowed change of the framework's latched mTouch result
    *           variable from a single mTouch sample. 
    *
    *   To achieve maximum noise immunity, this value should be set to '1'. If the
    *   reading is having trouble keeping up with the level of sensitivity in your
    *   application, however, this value can be used to increase the maximum speed
    *   of the sensor's reading.
    *
    * @hideinitializer
    */
    /**
    * @def      MTOUCH_SCALING
    * @ingroup  Configuration
    * @brief    mTouch post-scaler of reading value
    *
    *   This option will determine the amount of post-scaling that is implemented on the 
    *   acquisition's accumulator register before being stored in the results register.
    *   The scaling factor should be set so that it is not possible for the result 
    *   register to overflow (Max output value is 65535.)
    *
    *   Set this option to the number of times the framework should right shift the 
    *   accumulator value to achieve the reading. When in doubt, '4' is usually a good 
    *   option.
    *
    * @hideinitializer
    */
//@}



/** @name Decoding Setup (mTouch_config.h)
*
*   These configuration options will adjust the decoding behavior of the mTouch Framework.
*   Editting these values is optional but allows for a more highly customized final
*   result.
*/
//@{
    /**
    * @def      MTOUCH_POWER_UP_SAMPLES
    * @ingroup  Configuration
    * @brief    Number of times each sensor should be scanned and decoded before exitting
    *           the initialization state and entering the released state.
    *
    *   Allowable Range: 1-65535
    *
    * @hideinitializer
    */
    /**
    * @def      MTOUCH_BUTTON_TIMEOUT
    * @ingroup  Configuration
    * @brief    Number of mTouch decode function calls where the sensor is consecutively
    *           pressed before resetting the sensor to its released state
    *
    *   This feature will reset the sensor's state if it remains in the 'pressed' state
    *   for too long. 
    *
    *   For example: 
    *
    *   Let's assume the dataReady flag is set once every 100ms due to the 
    *   MTOUCH_SAMPLES_PER_SCAN option. If we want a timeout of 10sec, the 
    *   MTOUCH_BUTTON_TIMEOUT value should be set to 100. If we want a timeout of 7.4 
    *   seconds, then BUTTON_TIMEOUT should be set to 74.
    *
    *   @code
    *   Timeout Duration = dataReady Toggle Rate * BUTTON_TIMEOUT 
    *   @endcode
    *
    *   Setting to '0' or commenting the #define will disable the timeout feature.
    *
    *   <b>Allowable Range :: [ 0, 2 - 255 ]</b>
    *
    * @hideinitializer
    */
    /**
    * @def      MTOUCH_DEBOUNCE_RELEASE
    * @ingroup  Configuration
    * @brief    Number of consecutive scans a sensor must be decoded as released before 
    *           changing states
    *
    *   Increasing this value will significantly slow response time, so make sure the 
    *   scan rate is fast enough to continue to provide the desired response rate.
    *
    * @hideinitializer
    */
    /**
    * @def      MTOUCH_DEBOUNCE_PRESS
    * @ingroup  Configuration
    * @brief    Number of consecutive scans a sensor must be decoded as pressed before 
    *           changing states
    *
    *   Increasing this value will significantly slow response time, so make sure the 
    *   scan rate is fast enough to continue to provide the desired response rate.
    *
    * @hideinitializer
    */
    /**
    * @def      MTOUCH_MOST_PRESSED_ONLY
    * @ingroup  Configuration
    * @brief    If enabled, only one active-mode sensor will be able to be pressed at
    *           any one time.
    *
    *   The sensor with the largest shift is allowed to remain pressed. All others are
    *   reset to the released state.
    *
    * @hideinitializer
    */
    /**
    * @def      MTOUCH_TOGGLE_ENABLED
    * @ingroup  Configuration
    * @brief    If enabled, a toggle latch bit will be implemented for each active sensor.
    *
    *   The toggle bit is changed on-press only. So when the user presses the first time,
    *   the toggle bit is set high. When the user releases, nothing happens. When the user
    *   presses again, the toggle bit is set low. When the user releases, nothing happens.
    *   Etc.
    *
    * @hideinitializer
    */
    /**
    * @def      MTOUCH_MOST_PRESSED_THRESHOLD
    * @brief    This configuration option determines how much more pressed the most-pressed 
    *           sensor must be above all other sensors in order to allow activation.
    *
    *   If the value is '10', the most pressed sensor's shift value must be at 
    *   least 10 counts higher than the second-most-pressed sensor's shift.
    *
    *   For example, if Sensor0's shift is 400 counts, all other sensor's shifts must be
    *   390 or lower to activate this sensor. 
    *
    * @hideinitializer
    */
    /**
    * @def      MTOUCH_MATRIX_ROW_START
    * @brief    Defines which MTOUCH_SENSORx sensor is the start-index of the 'row' sensors
    * @hideinitializer
    */
    /**
    * @def      MTOUCH_MATRIX_ROW_END
    * @brief    Defines which MTOUCH_SENSORx sensor is the final-index of the 'row' sensors
    * @hideinitializer
    */
    /**
    * @def      MTOUCH_MATRIX_COLUMN_START
    * @brief    Defines which MTOUCH_SENSORx sensor is the start-index of the 'column' sensors
    * @hideinitializer
    */
    /**
    * @def      MTOUCH_MATRIX_COLUMN_END
    * @brief    Defines which MTOUCH_SENSORx sensor is the final-index of the 'column' sensors
    * @hideinitializer
    */
//@}

/** @name Slider and Wheel Setup (mTouch_config_slider.h)
*
*   These options will configure if there are any sliders and wheels in your application
*   as well as how they are decoded. The framework is able to support up to 10 sliders
*   and 8 sensors per slider. (The framework does not support 80 unique sensors - if an
*   application actually used all 10 sliders with 8 sensors per slider, it would need to
*   re-use some active-mode sensors multiple times.)
*
*/
//@{
    /**
    * @def      MTOUCH_NUMBER_OF_SLIDERS
    * @ingroup  Configuration
    * @brief    Defines the number of sliders to be implemented
    *
    *   Any non-zero value will enable this module.
    *
    * @hideinitializer
    */
    /**
    * @def      MTOUCH_NUMBER_SLIDER0_SENSORS
    * @ingroup  Configuration
    * @brief    Required for each slider. Defines the number of sensors that make up slider 0. 
    *
    *   @attention  When more than one slider is enabled, each slider needs its own
    *               version of this configuration option. Ex:
    *               MTOUCH_NUMBER_SLIDER1_SENSORS, MTOUCH_NUMBER_SLIDER2_SENSORS, etc.
    *               
    * @hideinitializer
    */
    /**
    * @def      MTOUCH_SLIDER0_SENSOR0
    * @ingroup  Configuration
    * @brief    Required for each sensor of each slider. Defines the active-mode sensor index of Slider 0's first sensor.
    *
    *   @attention  This is the index value of the active-mode sensor - NOT the analog
    *               channel number. 
    *               
    * @hideinitializer
    */
    /**
    * @def      MTOUCH_SLIDER_STEP_SIZE
    * @ingroup  Configuration
    * @brief    Defines the maximum value that the latched slider output value is allowed
    *           to change based on one new reading. Similar to MTOUCH_DECIMATION_MAX_STEP.
    * 
    * @hideinitializer
    */
    /**
    * @def      MTOUCH_SLIDER0_WHEEL_ENABLE
    * @ingroup  Configuration
    * @brief    If defined, tells the framework to decode the slider as a wheel.
    * 
    * @hideinitializer
    */
    /**
    * @def      MTOUCH_SLIDER_SCALING_ENABLED
    * @ingroup  Configuration
    * @brief    If defined, slider output values will always fall between 0 and 255,
    *           regardless of the number of sensors. If not, the maximum value will be
    *           based on the number of sensors in the slider.
    * 
    * @hideinitializer
    */
//@}

/** @name Proximity Sensor Configuration
*
*   These options define which sensors are proximity sensors and how the framework is
*   to implement the median filter. Read @ref featProximity "this guide" for more 
*   information about proximity sensors and configuration.
*
*/
//@{
    /**
    * @def      MTOUCH_NUMBER_PROXIMITY
    * @ingroup  Configuration
    * @brief    Defines how many proximity sensors are activated.
    *
    *   There must be one #MTOUCH_SENSORx_IS_PROX definition per enabled proximity sensor.
    *   The value of the definition must be it's "proximity-id" value.
    *
    *   "Proximity-ID" is used as an index to the proximity variables. In total, the IDs
    *   should start at 0 and end with #MTOUCH_NUMBER_PROXIMITY-1. 
    *
    *   The order is arbitrary.
    *
    * @code
    * #define MTOUCH_NUMBER_PROXIMITY     2
    *
    * #define MTOUCH_SENSOR5_IS_PROX      0     // MTOUCH_SENSOR5 is a proximity sensor with index 0
    * #define MTOUCH_SENSOR2_IS_PROX      1     // MTOUCH_SENSOR2 is a proximity sensor with index 1
    * @endcode
    * @hideinitializer
    */
    /**
    * @def      MTOUCH_SENSORx_IS_PROX
    * @ingroup  Configuration
    * @brief    Sets MTOUCH_SENSORx as a proximity sensor. Sensor value will be run through a
    *           median filter to increase the signal-to-noise ratio.
    *
    *   The value of the definition should be the proximity index value. Index values should
    *   start at 0 and end with #MTOUCH_NUMBER_PROXIMITY-1.
    *
    * @hideinitializer
    */
    /**
    * @def      MTOUCH_PROX_BUFFER_SIZE
    * @ingroup  Configuration
    * @brief    Determines the size of the buffer used in the proximity median filter
    *
    *   This number determines the history length of the median filter. Each value
    *   is an integer, so '5' requires 10-bytes per proximity sensor.
    *
    *   Options -
    *   @li 5   :: Least filtering, fastest response time
    *   @li 9   ::
    *   @li 15  :: Most filtering, longest response time
    *
    * @hideinitializer
    */
    /**
    * @def      MTOUCH_PROX_REMOVE_EXTREME
    * @ingroup  Configuration
    * @brief    The median filter's N largest and N smallest values are not included
    *           in the filter's output value. Must be less than half the buffer size.
    *               
    * @hideinitializer
    */
    /**
    * @def      MTOUCH_PROX_USE_32BIT_ACCUM
    * @ingroup  Configuration
    * @brief    If defined, the accumulator used during the decoding process of the 
    *           proximity sensor will be implemented as a 32-bit value. 
    *
    * This solves overflow problems - at the cost of additional temporary RAM requirements.
    * @hideinitializer
    */
//@}

/** @name Advanced CVD Scan Controls (mTouch_config_cvdAdvanced.h)
*
*   These controls provide additional, more advanced options for adjusting the behavior
*   of the CVD scan. Incorrect configuration of these options can result in very strange,
*   erratic behavior.
*
*/
//@{
    /**
    * @def      CVD_SETTLING_DELAY
    * @ingroup  Configuration
    * @brief    Determines the amount of time provided for the external sensor and 
    *           internal hold capacitor  to balance their voltages during the CVD 
    *           scanning process.
    *
    *   If this value is <b>too small</b>: The sensors will not be as sensitive as they 
    *   could be and your sensor's readings will be V<sub>DD</sub> dependant.
    *
    *   If this value is <b>too large</b>: The noise immunity of the system will not be 
    *   as robust as it could be.<br>
    *
    *   To correctly set this value:
    *   <ol>
    *    <li>Set this value to 0, compile and program.
    *    <li>Look at the raw values on the mTouch One-Way GUI or through a terminal program while
    *        powering the system at the desired V<sub>DD</sub> level.<br>
    *        You can find the mTouch One-Way GUI in <tt>Your MLA Directory/mTouchCapDemos/Utilities/PIC12F PIC16F Utilities/mTouch One-Way GUI</tt>
    *    <li>Adjust V<sub>DD</sub> by plus and minus 0.5V 
    *       <ul>
    *       <li>Do you notice any change in the unpressed value as V<sub>DD</sub> is changing? If no, 
    *           leave this value alone. If yes...
    *       </ul>
    *    <li>Increase the delay until the sensor's reading no longer changes as V<sub>DD</sub> changes.
    *    </ol>
    *   Once this has occurred, the settling time has been correctly tuned to provide the 
    *   maximum amount of sensitivity while minimizing the framework's susceptibility to 
    *   noise.
    * 
    * @hideinitializer
    */
    /**
    * @def      CVD_CHOLD_CHARGE_DELAY
    * @ingroup  Configuration
    * @brief    Determines the amount of time provided for charging the internal 
    *           hold capacitor of the ADC during the CVD scanning process.
    *
    *   If this value is <b>too small</b>: There will be noticable crosstalk between 
    *   closely-indexed sensors that is not related to the hardware layout of the application.
    *
    *   If this value is <b>too large</b>: The mTouch ISR will take an unnecessarily long 
    *   amount of time to execute.<br>
    *
    *   To correctly set this value:
    *   <ol>
    *    <li>Set this value to 0, compile and program.
    *    <li>Look at the raw values on the mTouch One-Way GUI or through a terminal program.<br>
    *        You can find the mTouch One-Way GUI in <tt>Your MLA Directory/mTouchCapDemos/Utilities/PIC12F PIC16F Utilities/mTouch One-Way GUI</tt>
    *    <li>Press as hard as you can on the sensor with the highest amount of capacitance. 
    *       (This usuallycorresponds to the sensor with the highest unpressed raw value.)
    *       <ul>
    *        <li>Do you notice a strange crosstalk behavior on a different sensor? If no, 
    *            leave this value to 0. If yes...
    *       </ul>
    *    <li>Increase the delay until the crosstalk behavior is eliminated. 
    *   </ol>
    * 
    * @hideinitializer
    */
    /**
    * @def      CVD_SWITCH_DELAY   
    * @ingroup  Configuration
    * @brief    Determines the amount of added NOPs between when the sensor is set to
    *           an input and the ADC mux is pointed to the sensor.
    *
    * This option was created due to a rare behavior that can appear in some devices.
    *
    * The ideal value of this option for noise immunity is 0. This will minimize the 
    * amount of time the sensor is set as an input.
    *    
    * #CVD_SWITCH_DELAY should <b>only</b> be increased if the output driver is not disabling 
    * itself quickly enough, causing the sensor's output driver to affect the internal ADC's
    * hold voltage as soon as the mux is connected. The effect will be noticeable on the second
    * sample of the waveform: the settling point will be slightly unstable and will be visibly 
    * coupling to V<sub>DD</sub>.
    *
    * The behavior is most-noticeable on the second scan due to P-type transistors being less 
    * ideal than N-type transistors. The behavior is most-common when Fosc is a high value 
    * (16MHz or greater).
    *
    * If you are seeing the behavior, the correct value for this option will vary based on 
    * Fosc and the microcontroller in question; but, in general, 1-3 NOPs for 16MHz, 3-5 
    * for 32MHz.
    *
    * @hideinitializer
    */
    /**
    * @def      CVD_GUARD_IO_ENABLED
    * @ingroup  Configuration
    * @brief    If defined, the framework will implement a guard sensor using an IO port.
    *
    * @hideinitializer
    */
    /**
    * @def      CVD_GUARD_PORT
    * @ingroup  Configuration
    * @brief    If CVD_GUARD_IO_ENABLED is defined, this determines which port to use for 
    *           the guard.
    *
    * @hideinitializer
    */
    /**
    * @def      CVD_GUARD_PIN
    * @ingroup  Configuration
    * @brief    If CVD_GUARD_IO_ENABLED is defined, this determines which pin to use for 
    *           the guard.
    *
    * @hideinitializer
    */
    /**
    * @def      CVD_GUARD_DACOUT_ENABLED
    * @ingroup  Configuration
    * @brief    If defined, the framework will implement a guard sensor using the DAC's
    *           DACOUT pin. If available, this is recommended over the IO option as it
    *           provides better results.
    *
    * @hideinitializer
    */
    /**
    * @def      CVD_GUARD_DACCON0_A
    * @ingroup  Configuration
    * @brief    If CVD_GUARD_DACOUT_ENABLED is defined, this determines what value is
    *           loaded into the DACCON0 register prior to the first ADC sample of the
    *           differential CVD scan.
    *
    * @hideinitializer
    */
    /**
    * @def      CVD_GUARD_DACCON1_A
    * @ingroup  Configuration
    * @brief    If CVD_GUARD_DACOUT_ENABLED is defined, this determines what value is
    *           loaded into the DACCON1 register prior to the first ADC sample of the
    *           differential CVD scan.
    *
    * @hideinitializer
    */
    /**
    * @def      CVD_GUARD_DACCON0_B
    * @ingroup  Configuration
    * @brief    If CVD_GUARD_DACOUT_ENABLED is defined, this determines what value is
    *           loaded into the DACCON0 register prior to the second ADC sample of the
    *           differential CVD scan.
    *
    * @hideinitializer
    */
    /**
    * @def      CVD_GUARD_DACCON1_B
    * @ingroup  Configuration
    * @brief    If CVD_GUARD_DACOUT_ENABLED is defined, this determines what value is
    *           loaded into the DACCON1 register prior to the second ADC sample of the
    *           differential CVD scan.
    *
    * @hideinitializer
    */
    /**
    * @def      CVD_MUTUAL_ENABLED
    * @ingroup  Configuration
    * @brief    If defined, the framework will integrate a mutual sensor in to the
    *           differential CVD scan.
    *
    * @hideinitializer
    */
    /**
    * @def      CVD_MUTUAL_EXCLUSIVE_ENABLED
    * @ingroup  Configuration
    * @brief    If defined, the framework will attempt to perform an exclusively-mutual
    *           scan. NOTE: Not yet tested!
    *
    * @hideinitializer
    */
    /**
    * @def      CVD_MUTUAL_LAT
    * @ingroup  Configuration
    * @brief    If CVD_MUTUAL_ENABLED is defined, determines what LAT register is used
    *           for the mutual sensor.
    *
    * @hideinitializer
    */
    /**
    * @def      CVD_MUTUAL_TRIS
    * @ingroup  Configuration
    * @brief    If CVD_MUTUAL_ENABLED is defined, determines which pin is used for the
    *           mutual sensor.
    *
    * @hideinitializer
    */
//@}


/**
* @def      APFCON_INITIALIZED
* @brief    If defined, disables the reminder-warning to initialize the APFCON register.
* @hideinitializer
*/

/** @name   Non-critical Values
*   @brief  These values are used for tracking version numbers when communicating with a
*           master. They have been provided for use by the EEPROM storage function so its
*           easy to implement version tracking in your application. They are not required
*           and do not have an impact on the generation of the firmware.
*/
//@{
/**
* @def      MTOUCH_BOARD_ID
* @brief    No effect on framework behavior.
* @hideinitializer
*/
/**
* @def      MTOUCH_FRAMEWORK_VERSION
* @brief    No effect on framework behavior.
* @hideinitializer
*/
/**
* @def      MTOUCH_HARDWARE_VERSION
* @brief    No effect on framework behavior.
* @hideinitializer
*/
/**
* @def      MTOUCH_SOFTWARE_VERSION
* @brief    No effect on framework behavior.
* @hideinitializer
*/
//@}

