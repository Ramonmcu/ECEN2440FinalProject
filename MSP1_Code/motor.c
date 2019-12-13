/*
 * motor.c
 *
 *  Created on: Dec 8, 2019
 *      Author: rama1
 */

#include "msp.h"
#include "motor.h"
void config_motor(uint8_t duty_cycle)
{
    config_pwm_timer(duty_cycle);

    //config pwm pins
    P6->DIR|= (BIT4|BIT5|BIT6);
    //P6->SEL0|= (BIT4|BIT5);
    //P6->SEL1|= ~(BIT4|BIT5);
    P6->OUT|= ~(BIT4|BIT5);

    //config gpio pins
    //AIN1 -> 4.0
    //AIN2 -> 4.2
    //L
    P4->DIR|= (BIT0|BIT2);
    P4->OUT=0;

    //BIN1 -> 5.0
    //BIN2 -> 5.2
    //R
    P5->DIR|= (BIT0|BIT2);
    P5->OUT=0;


}

void config_pwm_timer(uint8_t duty_cycle)
{
    TIMER_A0->CTL |= BIT9; // set SMCLK Bits 9 and 8 = 10;
    TIMER_A0->CCTL[1] |= BIT7|BIT6|BIT5;  // set the reset/set output modes

    uint8_t DC;
    DC = CALC_PERIOD * duty_cycle/100; // calculate duty cycle out of 30 from passed in value
    TA0CCR0 |= CALC_PERIOD; // set upper limit of the timer to be 30 ticks
    TA0CCR1 |= DC; // set variable limit of timer to be a percentage of 30 ticks to determine duty cycle

    TIMER_A0->CCTL[0] |= BIT7|BIT6|BIT5|BIT4;
    TIMER_A0->CTL &= ~(BIT5|BIT4);

    //Configure Interrupts
    NVIC_SetPriority(TA0_0_IRQn, 0); //sets interrupt priority for CCTL[0]
    //enable interrupts
    NVIC_EnableIRQ(TA0_0_IRQn);
    TIMER_A0->CTL |= BIT4; //set clock
    TIMER_A0->R = 0x00; // clear Timer A0 counter for comparing
}

void toggle()
{
    P6->OUT ^= (BIT4|BIT5);
}

void forward()
{
    //AIN1 -> HIGH
    //BIN1 -> HIGH
    P4->OUT|=(BIT0);
    P5->OUT|=(BIT0);
}

void reverse()
{
    //AIN2 -> HIGH
    //BIN2 -> HIGH
    P4->OUT|=(BIT2);
    P5->OUT|=(BIT2);
}

void left()
{
    P4->OUT|=(BIT2);
    P5->OUT|=(BIT0);
}

void right()
{
    P4->OUT|=(BIT0);
    P5->OUT|=(BIT2);
}

void stop()
{
    P4->OUT=0;
    P5->OUT=0;
}
void TA0_0_IRQHandler(void)
{
  uint16_t interrupt_cause;
  interrupt_cause |= TIMER_A_CTL_IFG;
  if(interrupt_cause & 0x01)
  {
    toggle();
    interrupt_cause &= ~TIMER_A_CTL_IFG;
    TIMER_A0->CCTL[0] &= ~TIMER_A_CTL_IFG; // clear flag
  }
}
