/* 
 * File:   PWM_tmr.h
 * Author: Shay
 *
 * Created on December 17, 2013, 7:35 PM
 */

#ifndef PWM_TMR_H
#define	PWM_TMR_H

#ifdef	__cplusplus
extern "C" {
#endif


//#define UP_DIRECTION      TRISAbits.TRISA4
//#define DOWN_DIRECTION    TRISAbits.TRISA7

//#define UP      PORTAbits.RA4
//#define DOWN    PORTAbits.RA7


#define Input   1
#define Output  0

    void InitializeTimer2For_PWM(void);
    void Initialize_PWM(void);
    void SetDutyCycle_PWM(unsigned int DutyCycle);
    void SetPWMPer(unsigned int percent);
    void setRegister();
    int getPWMPeriod(float oscF, int pwmF, int T_prescale);


#ifdef	__cplusplus
}
#endif

#endif	/* PWM_H */




#ifdef	__cplusplus
}
#endif

#endif	/* PWM_TMR_H */

