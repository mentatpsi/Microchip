/**********************************************************************
* © 2008 Microchip Technology Inc.
*
* FileName:        		Accelerometer.h
* Dependencies:    		Header (.h) files if applicable, see below
* Processor:       		PIC24HJ128GP504
* Compiler:        		MPLAB® C30 v3.11b or higher
*
* SOFTWARE LICENSE AGREEMENT:
* Microchip Technology Incorporated ("Microchip") retains all ownership and 
* intellectual property rights in the code accompanying this message and in all 
* derivatives hereto.  You may use this code, and any derivatives created by 
* any person or entity by or on your behalf, exclusively with Microchip's
* proprietary products.  Your acceptance and/or use of this code constitutes 
* agreement to the terms and conditions of this notice.
*
* CODE ACCOMPANYING THIS MESSAGE IS SUPPLIED BY MICROCHIP "AS IS".  NO 
* WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED 
* TO, IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY AND FITNESS FOR A 
* PARTICULAR PURPOSE APPLY TO THIS CODE, ITS INTERACTION WITH MICROCHIP'S 
* PRODUCTS, COMBINATION WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION. 
*
* YOU ACKNOWLEDGE AND AGREE THAT, IN NO EVENT, SHALL MICROCHIP BE LIABLE, WHETHER 
* IN CONTRACT, WARRANTY, TORT (INCLUDING NEGLIGENCE OR BREACH OF STATUTORY DUTY), 
* STRICT LIABILITY, INDEMNITY, CONTRIBUTION, OR OTHERWISE, FOR ANY INDIRECT, SPECIAL, 
* PUNITIVE, EXEMPLARY, INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, FOR COST OR EXPENSE OF 
* ANY KIND WHATSOEVER RELATED TO THE CODE, HOWSOEVER CAUSED, EVEN IF MICROCHIP HAS BEEN 
* ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE FULLEST EXTENT 
* ALLOWABLE BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN ANY WAY RELATED TO 
* THIS CODE, SHALL NOT EXCEED THE PRICE YOU PAID DIRECTLY TO MICROCHIP SPECIFICALLY TO 
* HAVE THIS CODE DEVELOPED.
*
* You agree that you are solely responsible for testing the code and 
* determining its suitability.  Microchip has no obligation to modify, test, 
* certify, or support the code.
************************************************************************/

/************************************************************************
 Header Includes													
 ************************************************************************/
#include "Utility.h"

/************************************************************************
 Defines													
 ************************************************************************/
#define ID_ACCELEROMETER_PAD                100
#define ID_ACCELEROMETER_BUTTON_01          101
#define ID_ACCELEROMETER_BUTTON_02          102
#define ID_ACCELEROMETER_BUTTON_03          103
#define ID_ACCELEROMETER_BUTTON_04          104
#define ID_ACCELEROMETER_BUTTON_05          105
#define ID_ACCELEROMETER_BUTTON_06          106

#define  MAX_CHNUM	 						7		// Highest Analog input number in Channel Scan
#define  SAMP_BUFF_SIZE	 					32		// Size of the input buffer per analog input
#define  NUM_CHS2SCAN						7		// Number of channels enabled for channel scan
#define FS_CT   							10000
#define SAMPPRD_CT    						(unsigned int)((GetInstructionClock()/FS_CT)-1)

/************************************************************************
 Function Prototypes													
 ************************************************************************/
void AccelerometerInit(void);
void ReadAccelerometer(void);
void AccelerometerStop(void);
void AccelerometerMsg(GOL_MSG* msg);
void CheckSelectButton(void);
void GraphReadAccelerometer(void);
int VectorAvg(int numElems, int* srcV);
int directionX(int acc);
int directionY(int acc);
int directionZ(int acc);
void initAdc1(void);
void stopAdc1(void);
void initTmr3();
void initDma0(void);
void __attribute__((__interrupt__)) _DMA0Interrupt(void);

extern int direction1X(int acc);
extern int direction1Y(int acc);
extern int direction1Z(int acc);


