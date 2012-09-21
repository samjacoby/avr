#include <avr/io.h>
#include <avr/interrupt.h>

// Define relevant registers in the slave device
// #include "register_map.h"
// Version information, for late queries and such.
// #include "version.h"

#define IR_FREG 38000L

void setup(void) {

    // Set chip to run at 16MHz
    CLKPR = (1 << CLKPCE); 
    CLKPR = 0;

    // CONFIGURE TIMER0
    // Configure 38Khz
    // Run the timer w/ a prescale
    TCCR0B = 1 << CS01;

    OCR0A = 50;     // regulates frequency. some number.
    OCR0B = 25;     // constant duty-cucle of 50% 

    TCCR0A |= (1 << WGM01 ) | (1 << COM0B0);

    // Enable interrupts in the right places
    TIMSK0 = 1 << OCIE0B;


    // CONFIGURE TIMER1
    TCCR1A = (1 << WGM12); // CTC on OCR1A
    TCCR1B = (1 << CS12);

    OCR1AH = 0x0e; 
    OCR1AL = 0xa6;

    TIMSK1 = (1 << OCIE1A); // Interrupt enable
    
}

volatile uint16_t ir_count;
ISR(TIM1_COMPA) {
    static uint16_t count;
    count = (count + 1) & 0xffff; // rollover

    if(count < ir_count) { 
        DDRD |= 1 << PD5;
    } else {
        DDRD &= ~(1 << PD5); 
    }
}


void loop() {
   ir_count = 1024; 
}

int main(void) {

    setup();
    sei();

    while(1) {
        loop(); 
    }
    
    return 0;
}

