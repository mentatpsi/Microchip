

#include "TCPIPConfig.h"


#include "TCPIP Stack/TCPIP.h"
#include "MainDemo.h"

#if defined(STACK_USE_AUTOUPDATE_UART) && defined(MRF24WG)

#define MAX_USER_RESPONSE_LEN   (20u)

#define XMODEM_SOH      0x01u
#define XMODEM_EOT      0x04u
#define XMODEM_ACK      0x06u
#define XMODEM_NAK      0x15u
#define XMODEM_CAN      0x18u
#define XMODEM_BLOCK_LEN 128u


extern void dbgPrintf( const char* format, ... ) ;
extern UINT32 ImageUpdate_Addr;
extern UINT32 ImageUpdate_Checksum;
extern UINT32 ImageUpdate_Size;
extern void AutoUpdate_Initialize(void);
extern void AutoUpdate_Completed(void);
extern void AutoUpdate_Restore(void);
void dbgPrintf( const char* format, ... )
{
}
static void XMODEM_SendToModule_subAPI(BYTE *buf)
{
	int i;
	UINT8 buf_module[36];
	buf_module[0]=(ImageUpdate_Addr&0x00FF0000)>>16;
	buf_module[1]=(ImageUpdate_Addr&0x0000FF00)>>8;
	buf_module[2]=(ImageUpdate_Addr&0xFF);
	buf_module[3]=32;
	for(i=0;i<32;i++) buf_module[i+4]=buf[i];
	SendSetParamMsg(PARAM_FLASH_WRITE, buf_module, 36);
	ImageUpdate_Addr += 32;
}


static void XMODEM_SendToModule(BYTE *xmodm_buf)
{
	int i;
	
	//  1. Calculate checksum
	for(i=0;i<128;i++)
	{
		if((ImageUpdate_Size % 4) == 0) ImageUpdate_Checksum += (UINT32)xmodm_buf[i]<<24;
		if((ImageUpdate_Size % 4) == 1) ImageUpdate_Checksum += (UINT32)xmodm_buf[i]<<16;
		if((ImageUpdate_Size % 4) == 2) ImageUpdate_Checksum += (UINT32)xmodm_buf[i]<<8;
		if((ImageUpdate_Size % 4) == 3) ImageUpdate_Checksum += (UINT32)xmodm_buf[i];
		ImageUpdate_Size ++;
	}
	// 2. send 128 bytes				
	XMODEM_SendToModule_subAPI(&xmodm_buf[0]);
	XMODEM_SendToModule_subAPI(&xmodm_buf[32]);
	XMODEM_SendToModule_subAPI(&xmodm_buf[64]);
	XMODEM_SendToModule_subAPI(&xmodm_buf[96]);
}
static BYTE tempData[XMODEM_BLOCK_LEN];

BOOL	AutoUpdate_UartXMODEM_24G(void)
{
    enum SM_FIRMWARE_UPDATE
    {
        SM_FIRMWARE_UPDATE_SOH,
        SM_FIRMWARE_UPDATE_BLOCK,
        SM_FIRMWARE_UPDATE_BLOCK_CMP,
        SM_FIRMWARE_UPDATE_DATA,
        SM_FIRMWARE_UPDATE_CHECKSUM,
        SM_FIRMWARE_UPDATE_FINISH,
    } state;

    BYTE c;
   // MPFS_HANDLE handle;
    BOOL lbDone;
    BYTE blockLen=0;
    BOOL lResult = FALSE;
	BYTE BlockNumber=0, preBlockNum=0;
	BYTE checksum=0;
    
    DWORD lastTick;
    DWORD currentTick;
    state = SM_FIRMWARE_UPDATE_SOH;
    lbDone = FALSE;
	
	if( BUTTON3_IO == 1u) return FALSE;	
	putsUART("\n\rPress S2 (on Explorer16) to start the update.\n\r");
	while(BUTTON2_IO == 1u);
	MACInit();
	DelayMs(100);
    AutoUpdate_Initialize();
	putsUART("I am ready, Please transfer firmware patch by XMODEM.\r\n"); 
	putsUART("If you press S3(On Explorwe16), I will stop update, and restore back to previous firmware.\r\n"); 
    lastTick = TickGet();
    do
    {
        currentTick = TickGet();
        if ( currentTick - lastTick >= (TICK_SECOND*2) )
        {
            lastTick = TickGet();
			while(BusyUART());
            WriteUART(XMODEM_NAK);
        }

    } while(!DataRdyUART());

	
    while(!lbDone)
    {
    	if (BUTTON3_IO == 0u)   // If you want to cancel AutoUpdate, please press S3
    	{
    		putsUART("You press S3 button, revert begin...\r\n");
			AutoUpdate_Restore();
			putsUART("revert done\r\n");
			return FALSE;
    	}
        if(DataRdyUART())
        {
            c = ReadUART();
			lastTick = TickGet();
        }
        else
        {
            // put some timeout to make sure  that we do not wait forever.
             currentTick = TickGet();
	        if ( currentTick - lastTick >= (TICK_SECOND*10) )
	        {
	        	//if time out, copy old patch image from bank2 to bank1
	            putsUART("timeout, revert begin...\r\n");
				AutoUpdate_Restore();
				putsUART("revert done\r\n");
				return FALSE;
	        }
            continue;
        }
		//dbgPrintf("(%02x) ",c); 
        switch(state)
        {
        case SM_FIRMWARE_UPDATE_SOH:
			if(c == XMODEM_SOH)
			{
				state = SM_FIRMWARE_UPDATE_BLOCK;
				dbgPrintf("\r\n! ");
				checksum = c;
				lResult = TRUE;
			}
			else if ( c == XMODEM_EOT )
            {
            	state = SM_FIRMWARE_UPDATE_FINISH;
				
				while(BusyUART());
                WriteUART(XMODEM_ACK);
				lbDone = TRUE;
			}  
			else
			{
				dbgPrintf("\n!error\n");
				while(1);
			}
			break;
		case SM_FIRMWARE_UPDATE_BLOCK:
			BlockNumber = c;
			dbgPrintf("BLK=%d ",BlockNumber);         
			checksum += c;
            state = SM_FIRMWARE_UPDATE_BLOCK_CMP;
            break;

        case SM_FIRMWARE_UPDATE_BLOCK_CMP:
			dbgPrintf("%d ",c);
			dbgPrintf("@:");
			//Judge: Is it correct ?
			if(c != (BlockNumber ^ 0xFF))
			{
				lResult = FALSE;
				dbgPrintf("\nBLOCK_CMP err: %x,%x\n", c, BlockNumber ^ 0xFF );
			}
			else 
			{
				if((BYTE)(preBlockNum+1) != BlockNumber)
				{
					lResult = FALSE;
					dbgPrintf("\nBLOCK  err %x %x\n",preBlockNum+1,BlockNumber);
				}
			}
			checksum += c;
			blockLen = 0;
            state = SM_FIRMWARE_UPDATE_DATA;
            break;
		case SM_FIRMWARE_UPDATE_DATA:
            // Buffer block data until it is over.
            tempData[blockLen++] = c;
            if ( blockLen == XMODEM_BLOCK_LEN )
            {
            	state = SM_FIRMWARE_UPDATE_CHECKSUM;
            }
			checksum += c;
			
            break;
		case SM_FIRMWARE_UPDATE_CHECKSUM:
			dbgPrintf("Checksum=%x=%x ",checksum,c);
			if(checksum != c)
			{
				lResult = FALSE;
				dbgPrintf("\nchecksum  err\n");
			}
			XMODEM_SendToModule(tempData);
			while(BusyUART());
			if(lResult == TRUE)
			{
            	WriteUART(XMODEM_ACK);
				preBlockNum++;
			}
			else
			{
				WriteUART(XMODEM_NAK);
			}
            state = SM_FIRMWARE_UPDATE_SOH;
			break;

		default:
			dbgPrintf("\n!error\n");
			while(1);
			break;
        }

    }
	
	AutoUpdate_Completed();

    return TRUE;
}

#endif
