## NOTE: The PDF named milestone_1_report.pdf will be used in place of this
## ALSO: The PDF is not showing up on the website for some reason but it shows up on GitHub Desktop
# Milestone 1: Stranger Things Light Wall

This project creates an addressable RGB LED which can be connected in series with another board running the same program to create a pattern of colors based on an input of a specified number of bytes, initially sent to the first board in the series. The result is that the LEDs turn on as the color specified by the input.

## Getting Started

These instructions will get you a copy of the project up and running on your local machine for development and testing purposes. See deployment for notes on how to deploy the project on a live system.

## Explination of Code

This program begins by initializing the IO ports, which include the RX input at port 3.3, along with the RGB LED at ports 1.2, 1.3, and 1.4 and the TX output at port 3.4. Once the IO ports are initialized the PWM is initialized in up mode using SMCLK. Next is the initializion the UART signal which includes setting a 9600 Baud Rate, setting modulation, initializing the state machine and enabling the RX interrupt. After this the RGB LEDs are set to off initially, then the interrupts are enabled and the processor enters low power mode. The processor is now waiting for the interrupt. This interrupt happens when the UART signal is recieved at the RX port. When the UART signal is received it sends the first three values to the RGB buffer, which determines the duty cycles of the three LEDs. Next the UART signal is sent to the UART TX Buffer, which removes the first three values from the signal and then sends the signal to the next node in the circuit. Finally the TX Buffer is cleared and the next UART signal is ready to be received.

## Running the tests

Explain how to run the automated tests for this system

This system was tested using the program Realterm. Within Realterm you can send and recieved UART codes, which is what we did. We send a UART code to the RX port for the desired color output and then checked the resulting output from the TX port to ensure that it was the value we expected, which is the same as the RX signal minus the first three values.

## Built With

* TI MSP430F5529 microcontroller
* Code Composer Studio 8.1.0 - MSP430 integrated development environment used
* Standard breadboard with resistors, wires and RGB LED

## Authors

* **Eric Schroeder**
* **Ian Nielsen**

## Acknowledgments

* This project was created for Embedded Systems at Rowan University under the direction of Russell Trafford
