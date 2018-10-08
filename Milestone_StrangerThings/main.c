/*Authors: Eric Schroeder and Ian Neilson
Written: October 8, 2018
Last Updated: October 8,2018*/

#define RLED BIT2;
#define GLED BIT3;
#define BLED BIT4;
#define active_LED BIT7;

#include <msp430.h> 

void initialize_UART(void){
    P4SEL |= BIT4;          //UART TX
    P4SEL |= BIT5;          //UART RX
    UCA1CTL1 |= UCSWRST;    // Resets state machine
    UCA1CTL1 |= UCSSEL_2;   // SMCLK
    //more stuff needed
}

void Set_LED(void){
    P4DIR = active_LED;
    P4OUT &=~ active_LED;
    P1DIR |= RLED;
    P1SEL |= RLED;
    P1DIR |= GLED:
    P1SEL |= GLED;
    P1DIR |= BLED;
    P1SEL |= BLED;

}
void Set_PWM(void){
    TA0 = TASSEL_2 + MC_1 + ID_1 + TACLR;
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

    switch(byte){
    case 0 :
        //Calculate and send length byte
    break;
    case 1 :
        //set red LED PWM value
    break;
    case 2:
        //set green LED PWM value
    break;
    case 3:
        //set blue LED PWM value
    break;
    default:
        //send the rest of the data to the nest node
    break;
    }

    //increment/reset byte counter
}
