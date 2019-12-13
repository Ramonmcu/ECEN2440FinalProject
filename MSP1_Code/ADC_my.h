/*
 * ADC_my.h
 *
 *  Created on: Nov 16, 2019
 *      Author: rama1
 */

#ifndef ADC_MY_H_
#define ADC_MY_H_

char vert_direction;
char hor_direction;
float read;
int Schedule_event;
int is_empty;

void Configure_ADC(void);
void ADC14_IRQHandler(void);


#endif /* ADC_MY_H_ */
