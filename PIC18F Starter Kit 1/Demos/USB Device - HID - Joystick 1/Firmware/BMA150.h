#define BMA150_ADDR15 0x15
#define BMA150_ADDR14 0x14
#define BMA150_ADDR13 0x13
#define BMA150_ADDR12 0x12
#define BMA150_ADDR11 0x11
#define BMA150_ADDR10 0x10
#define BMA150_ADDR0F 0x0F
#define BMA150_ADDR0E 0x0E
#define BMA150_ADDR0D 0x0D
#define BMA150_ADDR0C 0x0C
#define BMA150_ADDR0B 0x0B
#define BMA150_ADDR0A 0x0A
#define BMA150_ADDR09 0x09

#define BMA150_TEMP 0x08

#define BMA150_ACC_Z_MSB 0x07
#define BMA150_ACC_Z_LSB 0x06
#define BMA150_ACC_Y_MSB 0x05
#define BMA150_ACC_Y_LSB 0x04
#define BMA150_ACC_X_MSB 0x03
#define BMA150_ACC_X_LSB 0x02
#define BMA150_VERSION 0x01
#define BMA150_CHIP_ID 0x00

#define BMA150_READ 0x80
#define BMA150_WRITE 0x00

#define BMA150_RANGE_2G 0
#define BMA150_RANGE_4G 1
#define BMA150_RANGE_8G 2

#define BMA150_BW_25 0x00
#define BMA150_BW_50 0x01
#define BMA150_BW_100 0x02
#define BMA150_BW_190 0x03
#define BMA150_BW_375 0x04
#define BMA150_BW_750 0x05
#define BMA150_BW_1500 0x06


typedef struct
{
    WORD x;
    WORD y;
    WORD z;
} BMA150_XYZ, *PBMA150_XYZ;

typedef union
{
    //Address 0x00
    struct
    {
        BYTE chip_id :3;
        BYTE :5;
    } ;

    //Address 0x01
    struct
    {
        BYTE ml_version :4;
        BYTE al_version :4;
    } ;

    //Address 0x02
    struct
    {
        BYTE new_data_x :1;
        BYTE :5;
        BYTE acc_x :2;
    } ;

    //Address 0x03
    struct
    {
        BYTE acc_x :8;
    } ;

    //Address 0x04
    struct
    {
        BYTE new_data_y :1;
        BYTE :5;
        BYTE acc_y :2;
    } ;

    //Address 0x05
    struct
    {
        BYTE acc_y :8;
    } ;

    //Address 0x06
    struct
    {
        BYTE new_data_z :1;
        BYTE :5;
        BYTE acc_z :2;
    } ;

    //Address 0x07
    struct
    {
        BYTE acc_z :8;
    } ;

    //Address 0x14
    struct
    {
        BYTE bandwidth :3;  
        BYTE range :2;
		BYTE :3;
    } ;

    BYTE val;
} BMA150_REG;
void BMA150_GetXYZ(PBMA150_XYZ);
void InitBma150 (void);
void BMA150_WriteByte(BYTE address, BYTE data);
BYTE BMA150_ReadByte(BYTE address);
//S&Dstkiss123
