/*****************************************************************************
* | File      	:   DEV_Config.c
* | Author      :   Waveshare team
* | Function    :   Hardware underlying interface
* | Info        :
*----------------
* |	This version:   V2.0
* | Date        :   2020-06-17
* | Info        :   Basic version
*
******************************************************************************/
#include "DEV_Config.h"
#include <unistd.h>

/*****************************************
                GPIO
*****************************************/
void DEV_Digital_Write(UWORD Pin, UBYTE Value)
{
    SYSFS_GPIO_Write(Pin, Value);
}

UBYTE DEV_Digital_Read(UWORD Pin)
{
    return SYSFS_GPIO_Read(Pin);
}

void DEV_GPIO_Mode(UWORD Pin, UWORD Mode)
{
    SYSFS_GPIO_Export(Pin);
    SYSFS_GPIO_Direction(Pin, (0 == Mode || SYSFS_GPIO_IN == Mode) ? SYSFS_GPIO_IN : SYSFS_GPIO_OUT);
}

/**
 * delay x ms
**/
void DEV_Delay_ms(UDOUBLE xms)
{
	usleep((uint32_t)(1000 * xms));
}

static void DEV_GPIO_Init()
{
    DEV_GPIO_Mode(OLED_CS, 1);
    DEV_GPIO_Mode(OLED_RST, 1);
    DEV_GPIO_Mode(OLED_DC, 1);
}

/******************************************************************************
function:	Module Initialize, the library and initialize the pins, SPI protocol
parameter:
Info:
******************************************************************************/
UBYTE DEV_ModuleInit()
{
    DEV_GPIO_Init();
    printf("USE_IIC\r\n");		
    OLED_DC_0;
    OLED_CS_0;
    DEV_HARDWARE_I2C_begin("/dev/i2c-1");
    DEV_HARDWARE_I2C_setSlaveAddress(0x3c);
    return 0;
}

void I2C_Write_Byte(uint8_t value, uint8_t Cmd)
{
    char wbuf[2]={Cmd, value};
    DEV_HARDWARE_I2C_write(wbuf, 2);
}

/******************************************************************************
function:	Module exits, closes SPI and BCM2835 library
parameter:
Info:
******************************************************************************/
void DEV_ModuleExit()
{
    DEV_HARDWARE_I2C_end();
}
