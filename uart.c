#include <xc.h>
#include "uart.h"

//initialize uart
void init_uart(unsigned long baud)
{
    TRISC6 = 1;
    TRISC7 = 1;

    SPEN = 1;
    CREN = 1;
    TXEN = 1;

    SPBRG = (FOSC / (16 * baud)) - 1;

    RCIE = 1;
}

//uart getchar function
unsigned char uart_getchar(void)
{
    while (RCIF != 1)
    {
        continue;
    }
    RCIF = 0;
    return RCREG;
}

//uart putchar function
void uart_putchar(unsigned char data)
{
    TXREG = data;
    do
    {
        continue;
    } while (TXIF != 1);
    TXIF = 0;
}

//uart put string function
void uart_puts(const char *s)
{
    while (*s)
    {
        uart_putchar(*s++);
    }
}