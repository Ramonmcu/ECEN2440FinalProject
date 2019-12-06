#include "msp.h"
#include "uart.h"

/**
 * main.c
 */
void main(void)
{
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer
    uart_clock_config();
	init_UART();
	while(1) {
	    send_char('w');
	    int i;
	    for(i = 0; i < 1000000; i++);
	}

}
