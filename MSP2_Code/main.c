#include <msp.h>

#define SPIDIV     0x04                         // 24 MHz/4 gives ~166 ns for each on bit in byte
#define SPILONG    0b11111000                   // ~830 ns (acceptable "on" range 550 to 850 ns)
#define SPISHORT   0b11100000                   // ~398 ns (acceptable "on" range 200 to 500 ns)

const unsigned int PIXELS = 64;                  // Pixels in the strip

void initClock();
void initSPI();
void initWS2812();
void sendByte (unsigned char b);
void sendPixel (unsigned char r, unsigned char g, unsigned char b);
void showColor (unsigned int count, unsigned char r , unsigned char g , unsigned char b);
void scrollColor(unsigned int count, unsigned char r, unsigned char g, unsigned char b);
void show();

int run_matrix(void)
{
  WDTCTL = WDTPW + WDTHOLD;                     // Stop watchdog timer

  initClock();
  initSPI();
  initWS2812();

  while(1){
      showColor(PIXELS, 0x55, 0x00, 0x00);     // red
      __delay_cycles(12000000);
      showColor(PIXELS, 0x00, 0x55, 0x00);     // green
      __delay_cycles(12000000);
}

void initClock(){
    CS->KEY = CS_KEY_VAL;
    CS->CTL0 = 0;
    CS->CTL0 = CS_CTL0_DCORSEL_4; // DCO (24Mhz)
    CS->CTL1 = CS_CTL1_DIVS__1 | CS_CTL1_SELS__DCOCLK | CS_CTL1_SELM__DCOCLK | CS_CTL1_SELA__LFXTCLK;
    CS->KEY = 0;
}

void initSPI(){

    P1->DIR |= BIT6;
    P1->SEL0 |= BIT6;
    //P1->OUT |= BIT6;


    EUSCI_B0->CTLW0 |= EUSCI_B_CTLW0_SWRST;         // Reset EUSCI state

    EUSCI_B0->CTLW0 = EUSCI_B_CTLW0_SWRST |
            EUSCI_B_CTLW0_CKPL |                    // Cap First Edge Change next
            EUSCI_B_CTLW0_MSB |                     // MSB First
            EUSCI_B_CTLW0_MST |                     // Master Mode
            EUSCI_B_CTLW0_SYNC |                    // Synchronous Mode
            EUSCI_B_CTLW0_MODE_0|                   //3-pin SPI (MOSI)
            EUSCI_B_CTLW0_SSEL__SMCLK;                          // SMCLK

    EUSCI_B0->BRW = SPIDIV;                         // clock divider (/3)
    EUSCI_B0->CTLW0 &= ~EUSCI_B_CTLW0_SWRST;        // **Initialize USCI state machine**
    EUSCI_B0->IE |= EUSCI_B_IE_RXIE1;           // Enable USCI0 RX interrupt

    __disable_irq(); //ignore global interrupts
}

// Sends one byte to the LED strip by SPI.
void sendByte (unsigned char b){
    unsigned char bit;
    for (bit = 0; bit < 8; bit++){
      if (b & 0x80) // is MSB set?
          EUSCI_B0->TXBUF = SPILONG;     // long on bit (~830 ns) defined for each clock speed
      else
          EUSCI_B0->TXBUF = SPISHORT;    // short on bit (~398 ns) defined for each clock speed
      b <<= 1;                      // shift next bit into high-order position
    }
}

// Send a single pixel worth of information.  Turn interrupts off while using.
void sendPixel (unsigned char r, unsigned char g, unsigned char b){
    sendByte (g);        // NeoPixel wants colors in GRB order
    sendByte (r);
    sendByte (b);
}

// Display a single color on the whole string.  Turn interrupts off while using.
void showColor (unsigned int count, unsigned char r , unsigned char g , unsigned char b){
    unsigned int pixel;
    for (pixel = 0; pixel < count; pixel++){
        sendPixel (r, g, b);
    }
    show();
}

void scrollColor(unsigned int count, unsigned char r, unsigned char g, unsigned char b) {
    unsigned int pixel;
    for (pixel = 0; pixel < count; pixel++) {
        sendPixel(r,g,b);
        show();
    }
}

// latch the colors
void show(){
    __delay_cycles(432);            // ~18 micro seconds
}

void initWS2812(){
    show ();                        // in case MOSI went high, latch in whatever-we-sent
    showColor(PIXELS, 0x00, 0x00, 0x00);     // now change back to black
    show ();                       // and latch that
}

// SPI interrupt service routine
void EUSCIB0_IRQHandler(void)
{
    if (EUSCI_B0->IFG & EUSCI_B_IFG_TXIFG)
    {
        // Disable tx interrupt
        EUSCI_B0->IE &= ~EUSCI_B__TXIE;

        // Clear the receive interrupt flag
        EUSCI_B0->IFG &= ~EUSCI_B_IFG_RXIFG;
    }
}
