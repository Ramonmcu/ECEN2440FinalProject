#include "msp.h"
#include "uart.h"
#include "random.h"

/**
 * main.c
 */
void main(void) {
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer
    uart_clock_config();
	init_UART();


	while(1) {

	}

}
