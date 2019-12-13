#include "msp.h"
#include "uart.h"
#include "random.h"
#include "motor.h"

/**
 * main.c
 */
void main(void) {
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer
    uart_clock_config();
	init_UART();
	config_motor(50);
	config_psense();



	while(1) {

	    if(schedule_event & 0b01)
	    {
	        schedule_event &= ~0b01;
	        forward();
	    }
	    else if(schedule_event & 0b10)
	    {
	        schedule_event &= ~0b10;
	        right();
	    }
	    else if(schedule_event & 0b100)
	    {
	        schedule_event &= ~0b100;
	        reverse();
	    }
	    else if(schedule_event & 0b1000)
	    {
	        schedule_event &= ~0b1000;
	        left();
	    }
	    else if(schedule_event & 0b10000)
	    {
	        schedule_event &= ~0b10000;
	        stop();
	    }
	}

}
