#include <avr/io.h>
#include <avr/delay.h>

/* 
 * A simple blink template for an AtTiny45 
 * Blinks pins PB0, PB1, and PB2 every second.
 *
 * Sam Jacoby <sjacoby@media.mit.edu>
 *
 */


// millisecond delay
#define DELAY 1000

void cycle_pin(int);

#define PIN PB4

int main(void) {

    // Set the relevant DDRB pins for output
    DDRB |= (1 << PIN);

    while(1) {
        PORTB |= (1 << PIN);
        _delay_ms(DELAY);
        PORTB &= ~(1 << PIN);
        _delay_ms(DELAY);
    }

    return 0;
}
