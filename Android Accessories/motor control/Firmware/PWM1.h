#ifndef PWM1
#define	PWM1

#ifdef	__cplusplus
extern "C" {
#endif

 #define  PWM_PERIOD 62500

//#define UP_DIRECTION      TRISAbits.TRISA4
//#define DOWN_DIRECTION    TRISAbits.TRISA7

//#define UP      PORTAbits.RA4
//#define DOWN    PORTAbits.RA7


#define Input   1
#define Output  0

    
    void InitializeTimer2For_PWM(void);
    void Initialize_PWM(void);
    void SetDutyCycle_PWM(unsigned int DutyCycle);
    void PwmInit(void);
    void SetPWM(BYTE potPercentage);
    
    void UpdatePWM(BYTE potPercentage);

    //BYTE pot2LEDs(BYTE potPercentage);




#ifdef	__cplusplus
}
#endif

#endif	/* PWM_H */