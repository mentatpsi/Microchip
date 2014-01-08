
		Readme File for Code Example:
		MPLAB Starter Kit for PIC24H MCU     
             ----------------------------------------

This file contains the following sections:
1. Code Example Description
2. Folder Contents
3. Suggested Development Resources
4. Revision History


1. Code Example Description:
----------------------------
The application is described in the steps shown below.
The tri-axial outputs X,Y,Z from the accelerometer are simultaneously sampled by three ADC 
channels AN4, AN5, AN6. The X, Y, Z samples are then processed by the MCU to take approriate 
actions. Visual output is generated on the OLED display. G.711 coded messages stored in the flash 
program memory are decoded and are played out as a PWM wave on the on-board speaker using 
a low cost interface and the Output Compare module.

Follow these steps to run the MPLAB_Starter_Kit_for_PIC24H_Demo demonstration 
On the starter kit board:
1. Power up the starter kit by connecting the board to the USB port of a computer.
You should briefly see a pop-up balloon in the system tray (lower right of desktop)
that states (1) new hardware has been found, (2) drivers are being installed, and
(3) new hardware is ready for use. If you do not see these messages and then
the starter kit does not work, try reconnecting the USB. If this does not work, see
Section 3.8 “Troubleshooting”.
2. When powered up, a welcome message is played out of the speaker while simultaneously 
displaying the start-up screen on the OLED display. After the start-up exercise 
Home screen is displayed on the OLED display.
3. The Home screen has 4 cells: Accelerometer(Acc.) Graph, External(Ext.) Sensor, 
Orientation, Games to choose from as shown below. The starter kit can be maneuvered 
by tilting about X and Y axes. Based on the direction of tilt one of the 4 cells 
is highlighted for selection. Switch S1 or S2 can be pressed to select the highlighted 
cell.                                       
4. Selecting Acc. Graph cell starts the application which captures the tri-axial 
outputs of the accelerometer and displays on the OLED display as a graph.                
Switch S1 or S2 can be pressed anytime to return to the Home screen.
5. Selecting Ext. Sensor cell starts the application which captures the output of the
external sensor which can be plugged-in at points TP12 and TP11. Before displaying 
the external sensor signal an information screen is displayed which prompts user to 
plug-in an external sensor. If the signal displayed on the screen is too fast or too 
slow to visually perceive, switch S1 can be pressed to slow down or speed up the display.
Switch S2 can be pressed anytime to return to the Home screen.
6. Selecting Orientation cell starts the application which indicates the orientation 
of the starter kit. Patterns are displayed on the OLED display and messages are played 
out on the speaker indicating Portrait, Landscape and Plane orientations. The 
acceleration in each of the axes is displayed on the left of the screen as a fraction 
of gravitational acceleration constant on earth, 1g = 9.8 m/s2.              
7. Selecting Games cell displays a new screen with three cells: Home, Bomber Jet, Snake. 
Selecting Home cell will return to the Home screen discussed in point 3.
8. Selecting Bomber Jet cell starts the Bomber Jet game.
The Jet can be maneuvered on the XY plane of the display by tilting the starter kit 
about X and Y axes. Asteroids and alien ships are encountered in the game. The Jet 
should be maneuvered such that it doesn’t collide with the asteroid, alien ship or 
doesn’t get hit by a missile from alien ship. Pressing Switch S1 turns on a protective 
shield momentarily. The shield will destroy any asteroid or alien ship missile in its 
path. Pressing S2 releases missiles form the Bomber Jet. The alien ships and the asteroids
are destroyed when hit by the missiles from Bomber Jet.
Jet missile hitting an alien ship or an asteroid increases the score. Using the protective 
shield decreases the score. The game automatically exits to a Score screen after achieving 
a score of 1000 or when hit by an alien missile, collided with an alien ship or asteroid.
After the Score screen appears Switch S1 or S2 can be pressed any time to return to the 
Games screen.
9. Selecting Snake cell starts the Snake game.
The snake can be maneuvered on the XY plane of the display by tilting the starter kit 
about X and Y axes. Multiple snake food appear which snake has to eat. The snake grows 
in size as and when it eats the food. The snake should be maneuvered to eat the food 
such that it doesn’t collide the 4 walls. The score increases if the snake eats food. 
Using the protective shield decreases the score. The game automatically exits to a 
Score screen after the snake grows tail of length 25 rings or snake hits either of 4 walls. 
After the Score screen appears Switch S1 or S2 can be pressed any time to return to the 
Games screen.


2. Folder Contents:
-------------------
This project folder contains the following sub-folders:

a. h
        This folder contains include files for the code example.
b. src
        This folder contains all the C and Assembler source files (*.c,
        *.s) used in demonstrating the described example. 
c. gld	
	This folder contains the .gld file for PIC24HJ128GP504.
d. obj
	This folder contains all the .o files generated.
e. src\Sounds
	This folder contains all the G.711 compressed speech files
	used in the code example. These were generated using the
	dsPICSpeechRecord utility for G.711 compression available
	on the Microchip website.
f. Pictures
	This folder has all the relavant pictures and icons used in the 
	demonstration. 


3. Required Development Resources:
-----------------------------------
        MPLAB Starter Kit for PIC24H MCU.


4. Revision History :
---------------------
        10/24/2008 - Initial Release of the Code Example
	07/06/2009 - Updated to release with Microchip Graphics Library
	02/12/2010 - Ported to Microchip Graphics Library Version 2.01
