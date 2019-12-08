/*
 * ADC_my.c
 *
 *  Created on: Nov 16, 2019
 *      Author: rama1
 */


#include "msp.h"
#include "ADC_my.h"

void Configure_ADC()
{
    vert_direction = 'S';
    hor_direction = 'S';
    ADC14->CTL0|= ADC14_CTL0_SSEL__SMCLK; // route SMCLK
    ADC14->CTL0|= ADC14_CTL0_PDIV__64; //divide clc by 64
    ADC14->CTL0|= ADC14_CTL0_DIV__8; //divide clk by 8
    ADC14->CTL0|= ADC14_CTL0_CONSEQ_3; //repeat-single-channel conversion
    ADC14->CTL0|= ADC14_CTL0_SHT1_2; //sample and hold time for 16 clk cycles
    ADC14->CTL0|= ADC14_CTL0_SHT0_2; //sample and hold time for 16 clk cycles
    ADC14->CTL0|= ADC14_CTL0_SHP;
    ADC14->CTL0|= ADC14_CTL0_MSC;
    ADC14->CTL0|= ADC14_CTL0_ON; // turn on enc

    ADC14->CTL1|= ADC14_CTL1_RES__14BIT; // set encoder to 14 bit res
    ADC14->CTL1|= (0x0)<<ADC14_CTL1_CSTARTADD_OFS; //set write address to MEM0
    ADC14->CTL1|= ADC14_CTL1_REFBURST;

    ADC14->LO0= 0xD93;  //check
    ADC14->HI0= 0x3F48; //check
    ADC14->LO1= 0xD93;  //check
    ADC14->HI1= 0x3F48; //check

    ADC14->MCTL[0]|= ADC14_MCTLN_VRSEL_15; //select V(R+) and V(R-) reference voltages
    ADC14->MCTL[0]|= ADC14_MCTLN_INCH_15; //input channel select
    ADC14->MCTL[0]|= ADC14_MCTLN_WINC ; //enable comparator mode

    ADC14->MCTL[1]|= ADC14_MCTLN_WINCTH;
    ADC14->MCTL[1]|= ADC14_MCTLN_VRSEL_15;
    ADC14->MCTL[1]|= ADC14_MCTLN_INCH_9;
    ADC14->MCTL[1]|= ADC14_MCTLN_WINC;
    ADC14->MCTL[1]|= ADC14_MCTLN_EOS;

    REF_A->CTL0= (REF_A_CTL0_OUT |REF_A_CTL0_ON); // turn on external Vref

    //Route Pins
    //P6->SEL0|=BIT0;
    P6->SEL1|= BIT0;
    P4->SEL1|= BIT4;


    //Enable interrupts
    ADC14->IER0|= ADC14_IER0_IE0;
    ADC14->IER0|= ADC14_IER0_IE1;
    ADC14->IER1|= ADC14_IER1_HIIE;
    ADC14->IER1|= ADC14_IER1_LOIE;

    //Enable and start Conversions
    ADC14->CTL0|= ADC14_CTL0_ENC;
    ADC14->CTL0|= ADC14_CTL0_SC;

    NVIC_EnableIRQ(ADC14_IRQn);
}

void ADC14_IRQHandler(void)
{
   uint32_t interrupt_cause=0;
   interrupt_cause |= (ADC14->IFGR0|ADC14->IFGR1);
   if(interrupt_cause & ADC14_IFGR1_HIIFG )
   {
       ADC14->CLRIFGR1 |= ADC14_CLRIFGR1_CLRHIIFG;

       if(ADC14->MEM[0] != 16188)
       {
           hor_direction = 'R';
           vert_direction = 'S';
           Schedule_event |= 0b11;
           NVIC_DisableIRQ(ADC14_IRQn);
       }
       else if(ADC14->MEM[1] != 16188)
       {
           vert_direction = 'F';
           hor_direction = 'S';
           Schedule_event |= 0b11;
           NVIC_DisableIRQ(ADC14_IRQn);
       }
   }
   else if(interrupt_cause & ADC14_IFGR1_LOIFG)
   {
       ADC14->CLRIFGR1 |= ADC14_CLRIFGR1_CLRLOIFG;
       if(ADC14->MEM[0] != 16188)
       {
            hor_direction = 'L';
            vert_direction = 'S';
            Schedule_event |= 0b11;
            NVIC_DisableIRQ(ADC14_IRQn);
       }
       else if(ADC14->MEM[1] != 16188)
       {
           vert_direction = 'B';
           hor_direction = 'S';
           Schedule_event |= 0b11;
           NVIC_DisableIRQ(ADC14_IRQn);
       }
   }
   else if(interrupt_cause & (ADC14_IFGR0_IFG0 |ADC14_IFGR0_IFG1))
   {
       hor_direction = 'S';
       Schedule_event |= 0b01;
       vert_direction = 'S';
       Schedule_event |= 0b10;
       NVIC_DisableIRQ(ADC14_IRQn);
   }
}


