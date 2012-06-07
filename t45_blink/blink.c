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

int main(void) {

    // Set the relevant DDRB pins for output
    DDRB |= (1 << PB0) | (1 << PB1) | (1 << PB2);
    while(1) {
        cycle_pin(PB0);
        cycle_pin(PB1);
        cycle_pin(PB2);
    }

    return 0;
}

// Cycle @pin for @delay milliseconds
void cycle_pin(int pin) {

    PORTB |= (1 << pin);
    _delay_ms(DELAY);
    PORTB &= ~(1 << pin);

}
