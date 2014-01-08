/*
 * File:   PWM.h
 * Author: IBRAHIM NAZIR SANGI
 *
 * Created on July 5, 2013, 10:57 AM
 */

#ifndef PWM_TMR_H
#define	PWM_TMR_H

#ifdef	__cplusplus
extern "C" {
#endif


#define UP_DIRECTION      TRISAbits.TRISA4
#define DOWN_DIRECTION    TRISAbits.TRISA7

#define UP      PORTAbits.RA4
#define DOWN    PORTAbits.RA7


#define Input   1
#define Output  0


    void InitializePWM_T2(void);

    void InitializeTimer2For_PWM(void);
    void Initialize_PWM(void);
    void SetDutyCycle_PWM(unsigned int DutyCycle);
    void SetDutyCycle_PWM1(unsigned int DutyCycle);
    //void SetPWMPer1(short percent);
    void Initialize(void);
    void SetPWMPer(int percent, int update);
    
    void setRegister();
    int getPWMPeriod(int pwmF, int oscF, short prescaleT);
    int toSetting(int percentage);
    void setRPM(unsigned int targetRate);
    void fourWave(void);


#ifdef	__cplusplus
}
#endif

#endif	/* PWM_TMR_H */



