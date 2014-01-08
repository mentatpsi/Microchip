void SYS_Init(void);

#define BUTTON_DEBOUNCE_COUNT 0x0FFul

//#define RUN_AT_48MHZ
//#define RUN_AT_24MHZ
#define RUN_AT_60MHZ

#define DEMO_BOARD_NAME_STRING  "PIC32MX460F512L PIM"

// Various clock values
#if defined(RUN_AT_48MHZ)
    #define GetSystemClock()            48000000UL
    #define GetPeripheralClock()        48000000UL
    #define GetInstructionClock()       (GetSystemClock() / 2)
#elif defined(RUN_AT_24MHZ)
    #define GetSystemClock()            24000000UL
    #define GetPeripheralClock()        24000000UL
    #define GetInstructionClock()       (GetSystemClock() / 2)
#elif defined(RUN_AT_60MHZ)
    #define GetSystemClock()            60000000UL
    #define GetPeripheralClock()        60000000UL  // Will be divided down
    #define GetInstructionClock()       (GetSystemClock() / 2) 
#else
    #error Choose a speed
#endif

#define DEBUG_PrintString(a)