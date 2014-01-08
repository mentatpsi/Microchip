#define THIS_IS_STACK_APPLICATION


#include "HardwareProfile_update.h"
#include "SPI_MemAccess.h"

#if !defined( MRF24WG) && defined( __PIC32MX__ )

void InitializeBoard(void);

static BOOL	PatchImage_UartXMODEM(void);
static BOOL PatchImage_Verify(void);
#define PATCH_SIGNATURE              (UINT32)(~(0x00043fbc))
static UINT8 SignatureData[12];
DWORD TickGet(void);
int AutoUpdate_UartXMODEM_Roadrunner(void)
{
    UINT32 memindex;
	//If you want to AutoUpdate, please Hold button 4 to boot 
	if( BUTTON3_IO == 1u) return 0;  
	// Initialize application specific hardware
	InitializeBoard();
    // initialize the SPI interface
    WF_SpiInit_Update();
    // Reset the MRF24WB0M (using SPI bus to write/read MRF24WB0M registers
    WFChipReset();

    // Enable access to the MRF24WB0M flash memory
    WFEnableMemAccess();


    putsUART("\n\rPress S2 (on Explorer16) to start the update.\n\r");
    while(BUTTON2_IO == 1u);
    putsUART("\rS2 pressed! Firmware update in progress...\n\r");
    putsUART("\rPlease do NOT turn off the power!\n\r");

    // Erase lower 124Kb of memory
    putsUART("Erasing Low Memory\n\r");
    for(memindex=0;memindex<(FLASH_SIZE_IN_BYTES-0x1000);memindex+=0x1000)
        SectorErase(memindex);

	putsUART("Please use XMODEM to transfer PatchImage by Serial Port\n\r");
	PatchImage_UartXMODEM();
	putsUART("Verify..\r\n");
	PatchImage_Verify();
	while(1); //Pnly for debug
   
   
    return 0; //should never get here.
}




#define MAX_USER_RESPONSE_LEN   (20u)

#define XMODEM_SOH      0x01u
#define XMODEM_EOT      0x04u
#define XMODEM_ACK      0x06u
#define XMODEM_NAK      0x15u
#define XMODEM_CAN      0x18u
#define XMODEM_BLOCK_LEN 128u


UINT32 ImageUpdate_Addr;
UINT32 ImageUpdate_Checksum;
UINT32 ImageUpdate_Size;


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
	WriteSerialFlashArray(xmodm_buf, ImageUpdate_Addr, 128);
	ImageUpdate_Addr += 128;
}

static BOOL PatchImage_Verify(void)
{
	int i,memindex;
	UINT8 CheckBuffer[64];
	UINT32 Checksum_verify=0;
	WFEnableMemAccess();
	ReadSerialFlashArray(CheckBuffer,0,12);
		
	for(i=0;i<12;i++)
	{
		if(CheckBuffer[i] != SignatureData[i])
		{
			putsUART("\n\rVerification Failed at address ");
			sprintf((char *)CheckBuffer, "0x%02X\n\r",i);
			putsUART((char*)CheckBuffer);
			WFDisableMemAccess();
			while(1);
		}
	}
	Checksum_verify = 0;
	for(memindex = 0;memindex < ImageUpdate_Size; memindex+=64)
	{
		ReadSerialFlashArray(CheckBuffer,memindex+12,64);
		for(i=0;i<64;i++)
		{
			if((i % 4) == 0) Checksum_verify += (UINT32)CheckBuffer[i]<<24;
			if((i % 4) == 1) Checksum_verify += (UINT32)CheckBuffer[i]<<16;
			if((i % 4) == 2) Checksum_verify += (UINT32)CheckBuffer[i]<<8;
			if((i % 4) == 3) Checksum_verify += (UINT32)CheckBuffer[i];
		}
		putsUART(".");
	}
	WFDisableMemAccess();
	if(Checksum_verify == ImageUpdate_Checksum)
	{
		putsUART("CheckSum correct!\r\n");
		return TRUE;
	}
	else
	{
		putsUART("CheckSum Error!!!!\r\n");
		return FALSE;
	}
		

}
static BOOL	PatchImage_UartXMODEM(void)
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
    BOOL lbDone;
    BYTE blockLen=0;
    BOOL lResult=FALSE;
	BYTE BlockNumber=0, preBlockNum=0;
	BYTE checksum=0;
    static BYTE tempData[XMODEM_BLOCK_LEN];
    DWORD lastTick;
    DWORD currentTick;
    state = SM_FIRMWARE_UPDATE_SOH;
    lbDone = FALSE;

	ImageUpdate_Addr=12;
	ImageUpdate_Checksum = 0;
	ImageUpdate_Size = 0;
	putsUART("I am ready, Please transfer firmware patch by XMODEM\r\n"); 
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
        if(DataRdyUART())
        {
            c = ReadUART();
			lastTick = TickGet();
        }
        else
        {
            // put some timeout to make sure  that we do not wait forever.
             currentTick = TickGet();
	        if ( currentTick - lastTick >= (TICK_SECOND*600) )
	        {
	        	//if time out, copy old patch image from bank2 to bank1
	            putsUART("timeout ...\r\n");
				while(1); 
				return FALSE;
	        }
            continue;
        }
        switch(state)
        {
        case SM_FIRMWARE_UPDATE_SOH:
			if(c == XMODEM_SOH)
			{
				state = SM_FIRMWARE_UPDATE_BLOCK;
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
				while(1);
			}
			break;
		case SM_FIRMWARE_UPDATE_BLOCK:
			BlockNumber = c;
			checksum += c;
            state = SM_FIRMWARE_UPDATE_BLOCK_CMP;
            break;

        case SM_FIRMWARE_UPDATE_BLOCK_CMP:
			//Judge: Is it correct ?
			if(c != (BlockNumber ^ 0xFF))
			{
				lResult = FALSE;
			}
			else 
			{
				if((BYTE)(preBlockNum+1) != BlockNumber)
				{
					lResult = FALSE;
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
			if(checksum != c)
			{
				lResult = FALSE;
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
			while(1);
			break;
        }

    }
	
	{
	
	
	ImageUpdate_Addr = 0;

	
	SignatureData[0]=(PATCH_SIGNATURE>>24)&0xFF;
	SignatureData[1]=(PATCH_SIGNATURE>>16)&0xFF;
	SignatureData[2]=(PATCH_SIGNATURE>>8 )&0xFF;
	SignatureData[3]=(PATCH_SIGNATURE>>0)&0xFF;
	
	SignatureData[4]=(ImageUpdate_Size>>24)&0xFF;
	SignatureData[5]=(ImageUpdate_Size>>16)&0xFF;
	SignatureData[6]=(ImageUpdate_Size>>8 )&0xFF;
	SignatureData[7]=(ImageUpdate_Size>>0)&0xFF;

	SignatureData[8] =(ImageUpdate_Checksum>>24)&0xFF;
	SignatureData[9] =(ImageUpdate_Checksum>>16)&0xFF;
	SignatureData[10]=(ImageUpdate_Checksum>>8 )&0xFF;
	SignatureData[11]=(ImageUpdate_Checksum>>0)&0xFF;
	WriteSerialFlashArray(SignatureData, ImageUpdate_Addr, 12);
	
	UINT8 buf_im[32]={0};
	sprintf((char *)buf_im,"Size=0x%x, sum=%x\r\n", (unsigned int)ImageUpdate_Size, (unsigned int)ImageUpdate_Checksum);
	putsUART((char *)buf_im);

	putsUART("Update Image Done!\r\n");
	}

    return TRUE;
}

#endif

