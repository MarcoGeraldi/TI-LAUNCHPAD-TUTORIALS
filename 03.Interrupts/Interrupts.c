/* Board Header file */
#include "Board.h"

void gpioButtonFxn0(uint_least8_t index);

/*
 *  ======== mainThread ========
 */
void *mainThread(void *arg0)
{

    /* Call driver init functions */
    GPIO_init();
    // I2C_init();
    // SPI_init();
    // UART_init();
    // Watchdog_init();

    //Config GPIO Pins
    GPIO_setConfig(Board_GPIO_LED0, GPIO_CFG_OUT_STD
                   | GPIO_CFG_OUT_LOW);
    GPIO_setConfig(Board_GPIO_BTN1,
                   GPIO_CFG_IN_PU | GPIO_CFG_IN_INT_FALLING);

    // install Button callback
    GPIO_setCallback(Board_GPIO_BTN1, gpioButtonFxn0);
    // Enable interrupts
    GPIO_enableInt(Board_GPIO_BTN1);

    /*loop*/
    while (1) {

    }
}

//
//  ======== gpioButtonFxn0 ========
//  Callback function for the GPIO interrupt on Board_GPIO_BUTTON0.
//
void gpioButtonFxn0(uint_least8_t index)
{
    // Toggle the LED
    GPIO_toggle(Board_GPIO_LED0);
}