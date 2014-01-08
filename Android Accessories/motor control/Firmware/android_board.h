#ifndef android_board_H
#define	android_board_H

#ifdef	__cplusplus
extern "C" {
#endif


#define UP_DIRECTION      TRISAbits.TRISA4
#define DOWN_DIRECTION    TRISAbits.TRISA7

//#define UP      PORTAbits.RA4
//#define DOWN    PORTAbits.RA7


#define Input   1
#define Output  0

#define USB_ApplicationDataEventHandler( BYTE address, USB_EVENT event, void *data, DWORD size );
#define USB_ApplicationEventHandler( BYTE address, USB_EVENT event, void *data, DWORD size );

//SetLEDs(BYTE setting);
//ReadPOT(void);
//setPWM(BYTE potPercentage);
//pot2LEDs(BYTE potPercentage);
	

#ifdef	__cplusplus
}
#endif

#endif	/* PWM_H */