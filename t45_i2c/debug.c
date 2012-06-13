#include <avr/io.h>

void cycle_pin(int pin) {
    DDRB |= _BV(pin);
    PORTB ^= _BV(pin);

}
