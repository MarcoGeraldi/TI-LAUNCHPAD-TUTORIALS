/* For usleep() */
#include <unistd.h>
#include <stdint.h>
#include <stddef.h>
#include <math.h>

/* Driver Header files */
#include <ti/drivers/GPIO.h>
#include <ti/drivers/I2C.h>
// #include <ti/drivers/SPI.h>
// #include <ti/drivers/UART.h>
// #include <ti/drivers/Watchdog.h>
#include <ti/display/Display.h>

/* Board Header file */
#include "Board.h"

#define HDC2010_ADDR      0x40
#define SOFT_RESET_REG    0x0E
#define MEAS_CONFIG_REG   0x0F

static Display_Handle display;

/*
 *  ======== mainThread ========
 */

uint16_t        temperature;
float          temperature_conv;
uint16_t        humidity;
float          humidity_conv;
uint8_t         txBuffer[2];
uint8_t         txBuffer2[1];
uint8_t         rxBuffer[4];
I2C_Handle      i2c;
I2C_Params      i2cParams;
I2C_Transaction i2cTransaction;

void *mainThread(void *arg0)
{
    /* Call driver init functions */
    GPIO_init();
    I2C_init();
    // SPI_init();
    // UART_init();
    // Watchdog_init();

    /* Configure the LED pin */
    GPIO_setConfig(Board_GPIO_LED0, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);

    /* Turn on user LED */
    GPIO_write(Board_GPIO_LED0, Board_GPIO_LED_ON);


    I2C_Params_init(&i2cParams);
    i2cParams.bitRate = I2C_400kHz;
    i2c = I2C_open(Board_I2C_TMP, &i2cParams);

    /*Startup Sequence*/
    i2cTransaction.slaveAddress = HDC2010_ADDR;
    i2cTransaction.writeBuf = txBuffer;
    i2cTransaction.writeCount = 2;
    i2cTransaction.readBuf = NULL;
    i2cTransaction.readCount = 0;
    txBuffer[0] = SOFT_RESET_REG;
    txBuffer[1] = 0x00;
    I2C_transfer(i2c, &i2cTransaction);


    i2cTransaction.slaveAddress = HDC2010_ADDR;
    i2cTransaction.writeBuf = txBuffer;
    i2cTransaction.writeCount = 2;
    i2cTransaction.readBuf = NULL;
    i2cTransaction.readCount = 0;
    txBuffer[0] = MEAS_CONFIG_REG;
    txBuffer[1] = 0x00;
    I2C_transfer(i2c, &i2cTransaction);


    while(1) {

        /*Start Conversion*/
        i2cTransaction.slaveAddress = HDC2010_ADDR;
        i2cTransaction.writeBuf = txBuffer;
        i2cTransaction.writeCount = 2;
        i2cTransaction.readBuf = NULL;
        i2cTransaction.readCount = 0;
        txBuffer[0] = MEAS_CONFIG_REG;
        txBuffer[1] = 0x01;
        I2C_transfer(i2c, &i2cTransaction);

        /*Wait Conversion time 2ms*/
        usleep(2000);

        /*Read Output*/
        i2cTransaction.slaveAddress = HDC2010_ADDR;
        i2cTransaction.writeBuf = txBuffer2;
        i2cTransaction.writeCount = 1;
        i2cTransaction.readBuf = rxBuffer; 
        i2cTransaction.readCount = 4;
        txBuffer2[0]=0x00;
        I2C_transfer(i2c, &i2cTransaction);

        /*Decode Output*/
        temperature = (rxBuffer[1] << 8) | rxBuffer[0];
        temperature_conv = ( temperature / pow(2, 16))*165 - 40;

        humidity = (rxBuffer[3] << 8) | rxBuffer[2];
        humidity_conv = ( humidity / pow(2, 16))*100;
    }
}
