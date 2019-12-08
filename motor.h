/*
 * motor.h
 *
 *  Created on: Dec 8, 2019
 *      Author: rama1
 */

#ifndef MOTOR_H_
#define MOTOR_H_

#define SYSTEM_CLOCK 12000000
#define PWM_FREQUENCY 100000
#define CALC_PERIOD 120


void config_motor(uint8_t);
void config_pwm_timer(uint8_t);
void toggle();
void forward();
void reverse();
void left();
void right();




#endif /* MOTOR_H_ */
