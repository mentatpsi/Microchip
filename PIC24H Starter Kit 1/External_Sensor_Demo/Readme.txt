
		Readme File for Code Example:
		MPLAB Starter Kit for PIC24H MCU
	             External Sensor Demo 
   	           using 12-bit ADC and DMA
             ----------------------------------------

This file contains the following sections:
1. Code Example Description
2. Folder Contents
3. Suggested Development Resources
4. Revision History


1. Code Example Description:
----------------------------
This demo will run on the MPLAB Starter Kit for PIC24H MCU. The demo captures sensor signal
samples coming from an external analog sensor. The sensor signal flows through a differential
amplifier and a low pass anti-aliasing filter. The application uses the 12-bit on-chip ADC 
with the DMA in ping-pong buffering mode. 
Add the code for processing the sensor samples in the ProcessBuffer() function provided in main.c.(optional)
Refer to the MPLAB Starter Kit for PIC24H MCU User Guide for more details on starter kit. 

Follow these steps prior to running the demonstration On the starter kit board:

1. Plug in the external sensor differential outputs at input points TP11 and TP12. 
   (For a description of the range of sensors that are compatible 
   refer to the MPLAB Starter Kit for PIC24H MCU User Guide.)
2. Open the MPLAB workspace External_Sensor_Demo.mcw.
3. Compile the code and program the dsPIC.
4. Put breakpoint inside the ProcessBuffer() function.
5. In MPLAB click on Tools -> Data Monitor and Control Interface to Open DMCI tool. 
   When DMCI window opens up, click on the Dynamic Data View tab. Click on the 
   Load Profile button in the DMCI window. Load the profile from the parent 
   directory: External_Sensor_Demo.dmci
6. Click Run.
7. When the program halts at the breakpoint observe the pin-pong buffers
   (BufferA and BufferB) for the sensor signal samples captured.
   The output buffer can also be observed for the processed signal if 
   the ProcessBuffer() function is populated with the data processing code.


2. Folder Contents:
-------------------
This project folder contains the following sub-folders:

a. h
        This folder contains include files for the code example.
b. src
        This folder contains all the C and Assembler source files (*.c,
        *.s) used in demonstrating the described example. 


3. Required Development Resources:
-----------------------------------
        MPLAB Starter Kit for PIC24H MCU.


4. Revision History :
---------------------
        08/29/2008 - Initial Release of the Code Example
	07/06/2009 - Updated to release with Microchip Graphics Library
