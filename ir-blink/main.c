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

    // Configure 38Khz
    // Run the timer w/ a prescale
    TCCR0B = 1 << CS01;

    OCR0A = 53;         // regulates frequency. some number.
    OCR0B = OCR0A / 2;  // constant duty-cucle of 50% 

    TCCR0A = 1 << COM0B0;

    // Enable interrupts in the right places
    TIMSK0 = 1 << OCIE0A;

    DDRD = 1 << PD6;

}


void loop() {
}

int main(void) {

    setup();
    sei();

    while(1) {
        loop(); 
    }
    
    return 0;
}

