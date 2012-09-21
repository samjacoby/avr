#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

// Define relevant registers in the slave device
// #include "register_map.h"
// Version information, for late queries and such.
// #include "version.h"

#define IR_FREQ 38000L
#define FLICKER 3750

volatile uint8_t ir_count;

void setup(void) {

    // Set chip to run at 16MHz
    CLKPR = (1 << CLKPCE); 
    CLKPR = 0;

    // CONFIGURE TIMER0
    // Configure 38Khz
    // Run the timer w/ a prescale
    TCCR0B = 1 << CS01;

    OCR0A = 25;     // regulates frequency. some number.
    OCR0B = 12;     // doesn't really do anything now... 

    TCCR0A |= (1 << WGM01 ) | (1 << COM0B0);

    // Enable interrupts in the right places
    //TIMSK0 = 1 << OCIE0B;

    // CONFIGURE TIMER1
    TCCR1B |= (1 << WGM12) | (1 << CS11) | (1 << CS10);   // CTC + Clock scaling 

    OCR1A = FLICKER / 64;

    TIMSK1 = (1 << OCIE1A); // Interrupt enable

    DDRB |= 0xff; 

}



ISR(TIMER1_COMPA_vect) {
    static uint8_t count;

    count = (count + 1) & 0b00111111;

    PORTB ^= (1 << PB4);
    
    if(count < 64) { 
        DDRD |= 1 << PD5;
    } else {
        PORTB |= (1 << PB3);
        DDRD &= 0;
    }

}


void loop() {
    ir_count = 255;
}

int main(void) {

    setup();
    sei();

    while(1) {
        loop(); 
    }
    
    return 0;
}

