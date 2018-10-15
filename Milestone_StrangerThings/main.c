/*Authors: Eric Schroeder and Ian Neilson
Written: October 8, 2018
Last Updated: October 15,2018*/

#define RLED BIT2;
#define GLED BIT3;
#define BLED BIT4;

int byte = 0;
unsigned int total_bytes;

#include <msp430.h>

void initialize_UART(void){
       P4SEL |= BIT4;          // UART TX
       P4SEL |= BIT5;          // UART RX
       P3SEL |= BIT3;          // enable RX on 3.3
       P3SEL |= BIT4;          // enable TX on pin 3.4
       UCA1CTL1 |= UCSWRST;    // Resets state machine
       UCA1CTL1 |= UCSSEL_2;   // SMCLK
       UCA1BR0 = 104;          // 9600 Baud Rate
       UCA1BR1 = 0;            // 9600 Baud Rate
       UCA1MCTL |= UCBRS_1;    // Modulation
       UCA1MCTL |= UCBRF_0;    // Modulation
       UCA1CTL1 &= ~UCSWRST;   // Initializes the state machine
       UCA1IE |= UCRXIE;       // Enables USCI_A0 RX Interrupt
       UCA1IFG &= ~UCRXIFG;    //clears interrupt flags
}
void Set_LED(void){
    P1DIR |= RLED;
    P1SEL |= RLED;
    P1DIR |= GLED;
    P1SEL |= GLED;
    P1DIR |= BLED;
    P1SEL |= BLED;

}
void Set_PWM(void){
    TA0CTL = TASSEL_2 + MC_1;
    TA0CCR0 = 256;
    TA0CCR1 = 0;
    TA0CCR2 = 0;
    TA0CCR3 = 0;
    TA0CCTL1 |= OUTMOD_3;
    TA0CCTL2 |= OUTMOD_3;
    TA0CCTL3 |= OUTMOD_3;
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
#pragma vector = USCI_A1_VECTOR
__interrupt void USCI_A1_ISR(void) {
            switch(byte){
            case 0:
                total_bytes = UCA1RXBUF;
                break;
            case 1:
                TA0CCR1 = (UCA1RXBUF);
                break;
            case 2:
                TA0CCR2 = (UCA1RXBUF);
                break;
            case 3:
                TA0CCR3 = (UCA1RXBUF);
                while(!(UCA1IFG & UCTXIFG));
                UCA1TXBUF = total_bytes - 3;
                break;
            default:
                if(byte >= total_bytes){
                     byte = -1; }
                else{
                while(!(UCA1IFG & UCTXIFG));
                UCA1TXBUF = UCA1RXBUF;
                }
               break;
            }
           byte++;
    }
