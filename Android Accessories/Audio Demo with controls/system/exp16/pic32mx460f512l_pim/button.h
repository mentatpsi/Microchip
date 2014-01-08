#include <stdbool.h>
#include <stdint.h>

typedef struct
{
    bool pressed;
    bool toggled;
    uint32_t debounce;
} BUTTON;

void ButtonDebounce(BUTTON* button, bool pressed);

#define InitAllSwitches()  TRISDbits.TRISD6=1; TRISDbits.TRISD7=1; TRISAbits.TRISA7=1;

#define Switch1Pressed()    ((PORTDbits.RD13 == 0) ? TRUE : FALSE)
#define Switch2Pressed()    ((PORTAbits.RA7  == 0) ? TRUE : FALSE)
#define Switch3Pressed()    ((PORTDbits.RD7  == 0) ? TRUE : FALSE)
#define Switch4Pressed()    ((PORTDbits.RD6  == 0) ? TRUE : FALSE)