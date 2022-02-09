#include <stdint.h>
#include <stddef.h>

/* Driver Header files */
#include <ti/drivers/GPIO.h>
#include <ti/drivers/UART.h>

/* Example/Board Header files */
#include "Board.h"
#include "string.h"

UART_Handle uart;
UART_Params uartParams;

const char  echoPrompt[] = "Echoing characters:\r\n";
char inchar;

char        input[100];
uint8_t     counter = 0;

void Uart_ReadCallback(UART_Handle handle, void *rxBuf, size_t size)
{

    UART_read(handle, &inchar, 1);

    input[counter++]= inchar;

    if (inchar == '\n'){
        UART_write(handle, input, counter);
        memset(input, 0, sizeof(input));
        counter=0;
    }
}

/*
 *  ======== mainThread ========
 */
void *mainThread(void *arg0)
{

    /* Call driver init functions */
    GPIO_init();
    UART_init();

    /* Configure the LED pin */
    GPIO_setConfig(Board_GPIO_LED0, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);

    /* Turn on user LED */
    GPIO_write(Board_GPIO_LED0, Board_GPIO_LED_ON);

    /* Create a UART with data processing off. */
    UART_Params_init(&uartParams);
    uartParams.readMode = UART_MODE_CALLBACK;
    uartParams.writeDataMode = UART_DATA_BINARY;
    uartParams.readDataMode = UART_DATA_BINARY;
    uartParams.readReturnMode = UART_RETURN_NEWLINE;
    uartParams.readEcho = UART_ECHO_OFF;
    uartParams.baudRate = 115200;
    uartParams.readCallback = Uart_ReadCallback;

    uart = UART_open(Board_UART0, &uartParams);

    UART_write(uart, echoPrompt, sizeof(echoPrompt));


    if (uart == NULL) {
        /* UART_open() failed */
        while (1);
    }

    /* Loop forever echoing */
     while (1){        
     }


}
