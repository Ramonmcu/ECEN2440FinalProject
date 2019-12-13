/*
 * psense.c
 *
 *  Created on: Dec 8, 2019
 *      Author: rama1
 */
#include "msp.h"
#include "psense.h"

void config_psense(void)
{
    ADC14->CTL0|= ADC14_CTL0_SSEL__SMCLK; // route SMCLK
    ADC14->CTL0|= ADC14_CTL0_PDIV__64; //divide clc by 64
    ADC14->CTL0|= ADC14_CTL0_DIV__8; //divide clk by 8
    ADC14->CTL0|= ADC14_CTL0_CONSEQ_2; //repeat-single-channel conversion
    ADC14->CTL0|= ADC14_CTL0_SHT1_2; //sample and hold time for 16 clk cycles
    ADC14->CTL0|= ADC14_CTL0_SHT0_2; //sample and hold time for 16 clk cycles
    ADC14->CTL0|= ADC14_CTL0_SHP;
    ADC14->CTL0|= ADC14_CTL0_MSC;
    ADC14->CTL0|= ADC14_CTL0_ON; // turn on enc

    ADC14->CTL1|= ADC14_CTL1_RES__14BIT; // set encoder to 14 bit res
    ADC14->CTL1|= (0x0)<<ADC14_CTL1_CSTARTADD_OFS; //set write address to MEM0
    ADC14->CTL1|= ADC14_CTL1_REFBURST;

    ADC14->LO0= 0x600;  //check

    ADC14->MCTL[0]|= ADC14_MCTLN_VRSEL_15; //select V(R+) and V(R-) reference voltages
    ADC14->MCTL[0]|= ADC14_MCTLN_INCH_4; //input channel select
    ADC14->MCTL[0]|= ADC14_MCTLN_WINC ; //enable comparator mode

    REF_A->CTL0= (REF_A_CTL0_OUT |REF_A_CTL0_ON); // turn on external Vref

    //Route Pins
    P5->SEL1|= BIT1;
    P2->DIR|= BIT3;


    //Enable interrupts
    ADC14->IER1|= ADC14_IER1_LOIE;
    ADC14->IER0|= ADC14_IER0_IE0;

    //Enable and start Conversions
    ADC14->CTL0|= ADC14_CTL0_ENC;
    ADC14->CTL0|= ADC14_CTL0_SC;

    NVIC_EnableIRQ(ADC14_IRQn);
}


