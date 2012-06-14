#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/delay.h>

/* 
 * An ADC template. This code sets the ADC to
 * synchronously sample a receive pin, in
 * quadrature with the transmit from the PWM
 *
 * Sam Jacoby <sjacoby@media.mit.edu>
 *
 */



int main(void) {
    
    // We want the PWM output on PB0 -- so it's got to be set as an output 
    DDRB |= (1 << PB4) | (1 << PB0);

    // This enables global interrupts now and forever--until they're turned off 
    sei(); 

    while(1) {
    
    }

    return 0;
}
