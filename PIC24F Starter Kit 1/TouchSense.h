//******************************************************************************
//******************************************************************************
// Configuration
//******************************************************************************
//******************************************************************************

#if defined USE_GFX_DISPLAY_CONTROLLER_SH1101A
	#if defined TOUCHPAD_DIRECT_TOUCH
		#define TRIP_VALUE                      0x500
		#define TRIP_VALUE_BLASTER				0x1000
	   	#define HYSTERESIS_VALUE                0x65
	#endif

	#if defined TOUCHPAD_USE_OVERLAY
	   	#define TRIP_VALUE                      125 
		#define TRIP_VALUE_BLASTER				125 
	    #define HYSTERESIS_VALUE                2  
	#endif
    #define MAX_ALLOWED_CTMU_VAL            0x3FF //Fullscale
#elif defined USE_GFX_DISPLAY_CONTROLLER_SSD1303
	#if defined TOUCHPAD_DIRECT_TOUCH
	    #define TRIP_VALUE                      0x500
		#define TRIP_VALUE_BLASTER				0x1000
	    #define HYSTERESIS_VALUE                0x65
	#endif

	#if defined TOUCHPAD_USE_OVERLAY
	    #define TRIP_VALUE                      125 
		#define TRIP_VALUE_BLASTER				125 
	    #define HYSTERESIS_VALUE                2  
	#endif
    #define MAX_ALLOWED_CTMU_VAL            0x3FF //Fullscale
#else
    #error Graphics board not defined
#endif


#define ID_TOUCH_PAD                100
#define ID_TOUCH_BUTTON_01          101
#define ID_TOUCH_BUTTON_02          102
#define ID_TOUCH_BUTTON_03          103
#define ID_TOUCH_BUTTON_04          104
#define ID_TOUCH_BUTTON_05          105


int  AdjustCTMUTrimBits( int direction );
void AdjustCurrentSource( void );
void CTMUInit( void );
void ReadCTMU( void );
void TouchSenseButtonsMsg( GOL_MSG* msg );




