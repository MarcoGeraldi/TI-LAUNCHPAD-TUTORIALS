/*
 *  ======== empty.c ========
 */

/* For usleep() */
#include <unistd.h>
#include <stdint.h>
#include <stddef.h>

/* Driver Header files */
#include <ti/drivers/GPIO.h>
// #include <ti/drivers/I2C.h>
// #include <ti/drivers/SPI.h>
// #include <ti/drivers/UART.h>
// #include <ti/drivers/Watchdog.h>
#include <ti/drivers/timer/GPTimerCC26XX.h>
#include <xdc/runtime/Types.h>
#include <ti/sysbios/BIOS.h>
/* Board Header file */
#include "Board.h"

/*
 *  ======== mainThread ========
 */

uint16_t counter;

 GPTimerCC26XX_Handle hTimer;
 void timerCallback(GPTimerCC26XX_Handle handle, GPTimerCC26XX_IntMask interruptMask) {
        // interrupt callback code goes here. Minimize processing in interrupt.
     if (counter++ == 1000){
         GPIO_toggle(Board_GPIO_LED0);
         counter=0;
     }
 }


void *mainThread(void *arg0)
{
    /* Call driver init functions */
    GPIO_init();
    // I2C_init();
    // SPI_init();
    // UART_init();
    // Watchdog_init();

    /* Configure the LED pin */
    GPIO_setConfig(Board_GPIO_LED0, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);

    /* Turn on user LED */
    GPIO_write(Board_GPIO_LED0, Board_GPIO_LED_ON);


    GPTimerCC26XX_Params params;
    GPTimerCC26XX_Params_init(&params);
    params.width          = GPT_CONFIG_16BIT;
    params.mode           = GPT_MODE_PERIODIC;
    params.direction      = GPTimerCC26XX_DIRECTION_UP;
    params.debugStallMode = GPTimerCC26XX_DEBUG_STALL_OFF;
    hTimer = GPTimerCC26XX_open(Board_GPTIMER0A, &params);
    if(hTimer == NULL) {

      }

    Types_FreqHz  freq;
    BIOS_getCpuFreq(&freq);
    GPTimerCC26XX_Value loadVal = freq.lo / 1000 - 1; //47999
    GPTimerCC26XX_setLoadValue(hTimer, loadVal);
    GPTimerCC26XX_registerInterrupt(hTimer, timerCallback, GPT_INT_TIMEOUT);

    GPTimerCC26XX_start(hTimer);

    while(1) {
      ;
    }
}
