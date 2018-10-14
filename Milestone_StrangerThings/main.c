/*Authors: Eric Schroeder and Ian Neilson
Written: October 8, 2018
Last Updated: October 14,2018*/

#define RLED BIT2;
#define GLED BIT3;
#define BLED BIT4;
#define active_LED BIT7;

volatile unsigned int byte;
volatile unsigned int total_bytes;

#include <msp430.h> 

void initialize_UART(void){
       P4SEL |= BIT4;          // UART TX
       P4SEL |= BIT5;          // UART RX
       UCA1CTL1 |= UCSWRST;    // Resets state machine
       UCA1CTL1 |= UCSSEL_2;   // SMCLK
       UCA1BR0 = 6;            // 9600 Baud Rate
       UCA1BR1 = 0;            // 9600 Baud Rate
       UCA1MCTL |= UCBRS_0;    // Modulation
       UCA1MCTL |= UCBRF_13;   // Modulation
       UCA1MCTL |= UCOS16;     // Modulation
       UCA1CTL1 &= ~UCSWRST;   // Initializes the state machine
       UCA1IE |= UCRXIE;       // Enables USCI_A0 RX Interrupt
}
void Set_LED(void){
    P4DIR = active_LED;
    P4OUT &=~ active_LED;
    P1DIR |= RLED;
    P1SEL |= RLED;
    P1DIR |= GLED;
    P1SEL |= GLED;
    P1DIR |= BLED;
    P1SEL |= BLED;

}
void Set_PWM(void){
    TA0CTL = TASSEL_2 + MC_1 + ID_1 + TACLR;
    TA0CCR0 = 1000;
    TA0CCR1 = 500;
    TA0CCR2 = 500;
    TA0CCR3 = 500;
    TA0CCTL1 |= OUTMOD_7;
    TA0CCTL2 |= OUTMOD_7;
    TA0CCTL3 |= OUTMOD_7;
}

int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	initialize_UART();
	Set_LED();
	Set_PWM();
	__bis_SR_register(LPM0_bits + GIE);        // Set low power mode
	while(1);
}
#pragma vector = USCI_A0_VECTOR
__interrupt void USCI_A0_ISR(void) {
    P4OUT |= active_LED;
    switch(__even_in_range(UCA1IV,USCI_UCTXIFG)){
    case 0: break;  //if vector is 0 there is no interrupt
    case 2:
            switch(byte){
            case 0: total_bytes = UCA1RXBUF;
                byte++;
                break;
            case 1:
                TA0CCR1 = (UCA1RXBUF);
                byte++;
                break;
            case 2:
                TA0CCR2 = (UCA1RXBUF);
                byte++;
                break;
            case 3:
                TA0CCR3 = (UCA1RXBUF);
                UCA1TXBUF = total_bytes - 3;
                byte++;
                break;
            default:
                UCA1TXBUF = UCA1RXBUF;
                byte++;
                if(byte == total_bytes)
                    byte = 0;
               break;
            }
    }
}
