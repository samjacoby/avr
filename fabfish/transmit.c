#include <avr/io.h>
#include <avr/interrupt.h>

#include "led.h"
#include "transmit.h"

void transmit_setup(void) {
    TCCR0A = (1<<WGM01);
    TCCR0B = 0;
    OCR0A = 63;
    TIMSK0 |= (1<<OCIE0A);
}

void transmit_select(uint8_t chan) {
    transmit_quench();
    DDRD |= ((1<<(5+chan)));
}

void transmit_start(void) {
    led2_on();
    TCNT0 = 0;
    TCCR0B = (1<<CS00);
}

void transmit_quench(void) {
    TCCR0B = 0;
    DDRD &= ~((1<<PD5) | (1<<PD6));
    PORTD &= ~((1<<PD5) | (1<<PD6));
    led2_off();
}

ISR(TIMER0_COMPA_vect) {
    PORTD ^= ((1<<PD6) | (1<<PD5));
}

