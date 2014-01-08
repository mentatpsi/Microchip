#include <stdbool.h>
#include <stdint.h>

typedef struct
{
    bool pressed;
    bool toggled;
    uint32_t debounce;
} BUTTON;

void ButtonDebounce(BUTTON* button, bool pressed);

#define InitAllSwitches()  TRISAbits.TRISA1 = 1; TRISBbits.TRISB11 = 1; TRISFbits.TRISF12 = 1; TRISFbits.TRISF13 = 1; AD1PCFGLbits.PCFG11 = 1;

#define Switch1Pressed()    ((PORTFbits.RF13 == 0) ? TRUE : FALSE)
#define Switch2Pressed()    ((PORTFbits.RF12 == 0) ? TRUE : FALSE)
#define Switch3Pressed()    ((PORTBbits.RB11 == 0) ? TRUE : FALSE)
#define Switch4Pressed()    ((PORTAbits.RA1  == 0) ? TRUE : FALSE)