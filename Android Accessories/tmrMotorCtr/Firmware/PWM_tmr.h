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

    void InitializeTimer1For_PWM(void);
    void Initialize_PWM(void);
    void InitializePWM(void);
    //void SetDutyCycle_PWM(unsigned short DutyCycle);
    /*void SetPWMPer(BYTE percent);*/
    void setRegister();
    int getPWMPeriod(float oscF, int pwmF, int T_prescale);
    void setPWMPeriod(int pwmF);
    void lowerDuty();
    void higherDuty();
    //void setRPM(unsigned int targetRate);
    void SetPWMPer(int percent, int update);
    //void Initialize_PWM2(void);
    //void SetDutyCycle_PWM2(unsigned short DutyCycle);
    //void InitializeTimer2For_PWM(void);
    

#ifdef	__cplusplus
}
#endif

#endif	/* PWM_H */


