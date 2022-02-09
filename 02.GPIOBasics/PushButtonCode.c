void *mainThread(void *arg0)
{

    /* Call driver init functions */
    GPIO_init();
    // I2C_init();
    // SPI_init();
    // UART_init();
    // Watchdog_init();

    /* Configure the LED and BTN pins */
    GPIO_setConfig(Board_GPIO_LED0, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);

    GPIO_setConfig(Board_GPIO_BTN1, GPIO_CFG_IN_PU);

    /*loop*/
    while (1) {
        /* Read Board BTN-1 State*/
        if ( GPIO_read(Board_GPIO_BTN1)){
            /*Button is released -> LED0 OFF*/
            GPIO_write(Board_GPIO_LED0, 0);
        } else{
            /*Button is pressed -> LED0 ON*/
            GPIO_write(Board_GPIO_LED0, 1);
        }
    }
}