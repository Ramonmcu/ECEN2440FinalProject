# ECEN2440FinalProject
Mariah Carey Robot for ECEN 2440 Final Project - Fall 2019

Two MSP432 development boards were used and the code loaded onto each board is described below.

MSP 1:
*/--------------------------------------------------------------------------------------------------*/
main.c - Initializes files and runs program on an interrupt scheduler

uart.h - Header File for UART Driver
uart.c - Source File for UART Driver that configures UART to operate at 9600 Baud. Configures UART clock, sends and receives a character via Bluetooth App.

ADC_my.h - Header File for ADC Driver
ADC_my.c - Source File for ADC Driver that configures ADC on multiple pins. Configures interrupts for joystick (a future addition to project). 

psense.h - Header file for Pressure Sensor
psense.c - Source File that configures ADC for the pressure sensor.

motor.h - Header File for Motor Driver
motor.c - Configures motor driver and PWM signals. Set motors to move forward, backwards, left, and right


MSP 2:
*/--------------------------------------------------------------------------------------------------*/
