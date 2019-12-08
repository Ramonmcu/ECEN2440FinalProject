#include "msp.h"
#include "ADC_my.h"
#include "uart.h"


/**
 * main.c
 */

void main(void) {
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // stop watchdog timer
    Schedule_event =0;
    uart_clock_config();
    init_UART();
    Configure_ADC();

    while(1) {

       if(Schedule_event & 0b01)
        {
            Schedule_event &= ~0x01;
            send_char(hor_direction);
        }
        else if(Schedule_event & 0b10)
        {
            Schedule_event &= ~0x10;
            send_char(vert_direction);
        }
    }

}
