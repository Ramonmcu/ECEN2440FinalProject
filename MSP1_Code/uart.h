
/*
 * uart.h
 *
 *  Created on: Nov 18, 2019
 *      Author: Alexandra Ferguson
 */

#ifndef UART_H_
#define UART_H_

int sent;
uint8_t schedule_event;

void init_UART(void);
void uart_clock_config(void);
void enable_UART(void);
void transmit_UART(void);



#endif /* UART_H_ */
