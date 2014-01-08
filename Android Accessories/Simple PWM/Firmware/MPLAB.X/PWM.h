/* 
 * File:   PWM.h
 * Author: IBRAHIM NAZIR SANGI
 *
 * Created on July 5, 2013, 10:57 AM
 */

#ifndef PWM_H
#define	PWM_H

#ifdef	__cplusplus
extern "C" {
#endif


#define UP_DIRECTION      TRISAbits.TRISA4
#define DOWN_DIRECTION    TRISAbits.TRISA7

#define UP      PORTAbits.RA4
#define DOWN    PORTAbits.RA7



#define Input   1
#define Output  0

    
    void InitializeTimer2For_PWM(void);
    void Initialize_PWM(void);
    void SetDutyCycle_PWM(unsigned int DutyCycle);


#ifdef	__cplusplus
}
#endif

#endif	/* PWM_H */

