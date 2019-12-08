/*
 * uart.c
 *
 *  Created on: Nov 17, 2019
 *      Author: Alexandra Ferguson
 */



#include "msp.h"
#include "uart.h"
#include <String.h>

// RX P3.2 -> TX
// TX P3.3 -> RX

void init_UART(){
    EUSCI_A2->CTLW0 |= EUSCI_A_CTLW0_SWRST;
    EUSCI_A2->CTLW0 |= EUSCI_A_CTLW0_SSEL__SMCLK;
    EUSCI_A2->CTLW0 &= ~(EUSCI_A_CTLW0_PEN | EUSCI_A_CTLW0_MODE0 | EUSCI_A_CTLW0_MODE1 | EUSCI_A_CTLW0_SEVENBIT | EUSCI_A_CTLW0_SPB);

    EUSCI_A2->BRW = 78; // set prescaler
    EUSCI_A2->MCTLW |= 0x20 | EUSCI_A_MCTLW_OS16;

    // Configure port
    P3->SEL0 |= (BIT3 | BIT2);
    P3->SEL1 &= ~(BIT3 | BIT2);

    EUSCI_A2->CTLW0 &= ~(EUSCI_A_CTLW0_SWRST);  // clear UCSWRST

    while(EUSCI_A2->STATW & BIT0);

    //EUSCI_A2->IE |= EUSCI_A_IE_RXIE | EUSCI_A_IE_TXIE;
   // NVIC_EnableIRQ(EUSCIA2_IRQn);
}

void uart_clock_config() {
    CS->KEY = CS_KEY_VAL;
    CS->CTL0 = 0;
    CS->CTL0 |= CS_CTL0_DCOEN | CS_CTL0_DCORSEL_3;
    CS->CTL1 |= CS_CTL1_SELA_2 | CS_CTL1_SELS_3 | CS_CTL1_SELM_3;
    CS->KEY = 0;
}

void send_char(uint8_t data) {
    while(EUSCI_A2->STATW & BIT0);
    EUSCI_A2->TXBUF = data;
    if(sent)
    {
        sent = 0;
        NVIC_EnableIRQ(ADC14_IRQn);
    }
    else
    {
        sent++;
    }
    int i;
    for(i = 0; i < 1000000; i++);
}

char read_data() {
    return EUSCI_A2->RXBUF;
}

void EUSCIA2_IRQHandler() {
    uint32_t interrupt_cause;
    char testdata = 'q';
    interrupt_cause |= EUSCI_A2->IFG;
    if(EUSCI_A_IFG_TXIFG & interrupt_cause)
    {
        EUSCI_A2->IFG &= ~(EUSCI_A_IFG_TXIFG);
       // send_char(testdata);
    }
    if(EUSCI_A_IFG_RXIFG & interrupt_cause)
    {
        EUSCI_A2->IFG &= ~(EUSCI_A_IFG_TXIFG);
       // read_data();

    }
}
