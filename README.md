# Byggern
Byggern høst 2021 &lt;3 &lt;3  We can do it!

Exercise 1 (Day 1): 
1. Take up the breadboard, insert the voltage regulator LM7805, and connect it to the power
supply and capacitors as described in its datasheet. You might use a 1 µF capacitor on the
input instead of the recommended 0.33 µF. Use a voltage supply of 8 − 12V and verify using
a multimeter that the output voltage is a stable 5V . Turn off the power supply.
2. Insert the AVR ATmega162 MCU and connect its power pins to the output of the regulator.
Remember to use decoupling capacitors as discussed earlier.
3. Connect a reset circuit to the MCU’s reset pin as described in Atmel’s Application Note
AVR042. The diodes can be omitted. Connect the push button between “External Reset”
and ground, and use a 0.1µF capacitor for the RC circuit.
4. Connect the crystal oscillator (4.9152 MHz) close to the XTAL pins of the MCU as described
in the ATmega162 datasheet. Use 22 pF load capacitors between each leg of the crystal and
ground.
5. Connect the JTAG interface for Atmel-ICE to the breadboard. See the Atmel-ICE and
ATmega162 manuals for which pins to connect.
6. Create a simple test program and upload it to the MCU using the Atmel-ICE. For instance,
the test program could make a square wave signal to one of the output pins by toggling a
digital output. This could then be verified by connecting an oscilloscope probe to the relevant
pin.
Now, you have connected the basic components needed to get the MCU up and running. The
following steps will enable the serial communication over RS-232.
8. Insert the MAX233 IC and connect it as described in its datasheet. TXD and RXD pins
on the ATmega162 should be connected to T1IN and R1OUT on the MAX233, respectively.
Correspondingly, T1OUT and R1IN are to be connected to the serial line to the computer by
using a connector and a DB9 M/F cable.
9. Program a driver for ATmega162’s UART interface. This driver should contain functions for
sending and receiving data to and from the serial interface. Think about how to handle the
following situations:
(a) The application tries to send a new character while the UART is busy transmitting the
previous one
(b) The application wants to be notified when a new character is received
10. Create a test program to verify the driver and connections works correctly. The program
should both transmit and receive data over the serial line. Use the terminal program on the
PC when testing.
23
11. To link the printf function to the UART driver you only need to make a function that
transmits one character to the UART, and call the function fdevopen(transmit function,
receive function). For more information about this function, see the section about the
Standard I/O module (stdio.h) in AVR Libc’s user manual at http://www.nongnu.org/
avr-libc/user-manual/group__avr__stdio.html.
12. Verify that you now can send text and values of variables by using the standard printf
function.
13. Set up version control for your code base, see 2.3.3
