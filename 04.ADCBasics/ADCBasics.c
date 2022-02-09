
uint16_t result;

void *mainThread(void *arg0)
{

    /* Call driver init functions */
    GPIO_init();
    ADC_init();
    // I2C_init();
    // SPI_init();
    // UART_init();
    // Watchdog_init();

    /* initialize optional ADC parameters */
    ADC_Params params;
    ADC_Params_init(&params);
    params.isProtected=true;

    /* Open ADC channels for usage */
    ADC_Handle adcHandle = ADC_open(1, &params);


    /*loop*/
    while (1) {
        ADC_convert(adcHandle, &result);
    }
}
