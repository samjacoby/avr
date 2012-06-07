#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <avr/wdt.h>

/*
 * Simple pin controlled interrupt and blinker.
 * TODO: button debouncing
 *
 * Sam Jacoby <sjacoby@media.mit.edu>
 *
 */


#define D 100

// This could change when no is looking, so make it volatile
volatile int state;

int main (void)
{
    /* set PORTD for output*/
    DDRC |= (1 << PC0);

    /* PORTC is the onboard LED */
    int i;

    // keep track of a few different blink states 
    state = 1;

    for(i=0;i++;i<3) {
        PORTC |= (1 << PC0);
        _delay_ms(D);
        PORTC &= (1 >> PC0);
    }

    // Set interrupt pin as an input (this is not necessary for interrupts)
    PORTB &= ~(1 << PB0);

    // Initialize interrupt registers
    PCICR |= (1 << PCIE0);
    PCMSK0 |= (1 << PCINT0);

    // Set sleep mode
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);

    // Globally enable interrupts
    sei(); //Enable interrupts 

    // Program Loop
    while (1) {

        if(state == 0) {
            PORTD &=0x00;
            PORTC &= ~(1 << PC0);
            sleep_mode();
        }

        if(state == 1) {
            PORTD ^= (1 << PD3);
        }

        if(state == 2) {
            PORTD ^= (1 << PD5);
        }

        if(state == 3) {
            PORTD ^= (1 << PD6);
        }

        PORTC ^= (1 << PC0);

        _delay_ms(D);
        sei();

    }
    return 0;
}

ISR(PCINT0_vect) {
    state++;
    if(state == 4) {
        state = 0;
    }
    cli();
}
