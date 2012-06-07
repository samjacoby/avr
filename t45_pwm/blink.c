#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>

/* 
 * A PWM template, that uses timers to 
 * generate PWM on an AtTiny45 
 *
 * Sam Jacoby <sjacoby@media.mit.edu>
 *
 */

/* Interrupt Code
 *
 * This bit of code is executed everytime something magnificent happens
 *
 */

ISR(SIG_OUTPUT_COMPARE0A) {



}




int main(void) {

    /* GTCCR: General Timer/Counter Control Register
     *
     * Turn that sucker on and off
     *
     */

    // Might as well be off while we're setting up, eh?
    GTCCR &= ~_BV(TSM);

    /* TCCR0A: Timer/Counter Control Register A
     *
     * Set this register to control what happens on the output pin, 
     * when the compare match is made with the timer. 
     *
     * We just want to toggle, which is means setting the COM bits to 01,
     * per the datasheet. We don't need to bother clearing the zero, so we
     * just set the COM0A0 bit. 
     *
     * This register also controls the Waveform Generation bits WGM[1:0]. 
     * There is actually another WGM bit that lives on another register, so
     * the datasheet often talks about WGM[2:0], which I found fairly mystifing.
     * Unless we wanted to do Fast PWM mode, we don't need to worry about that
     * last WGM bit, which is on TCCR0B (we will set some bits there, though)
     *
     * We want CTC mode on this guy, though, so we'll WGM01 per the datasheet. 
     *
     */ 

    // Setting toggle mode and Clear Time on Compare Match
    
    TCCR0A |= _BV( COM0A0 ) | _BV ( WGM01 );

    /* TCCR0B: Timer/Counter Control Register B
     *
     * This register controls another range of functions. The key stuff here
     * are the the lower bits, which tell the microcontroller what clock to 
     * listen to, and what speed we want to listen at. We just want to use 
     * the onboard clock -- but 1MHz is pretty quick, so we'll reduce the 
     * speed some. This happens by using a prescaler, which automatically
     * divides the counter speed by certain set values. We'll divide by 64,
     * which gets us a nice 156.25Mhz on a 1MHz clock--a value
     * that Brian Mayton <bmayton@media.mit.edu> told me was a good one.
     *
     */

    // Set the clock source to be the internal one, divided by 64.
    TCCR0B |= _BV( CS01) | _BV( CS00 );

    /* Because we're in CTC mode, we want something in the OCR0A register.
     * This controls what we're going to see on the 0C0A. 
     *
     * Let's start simple with 0xFF.
     */

    OCR0A = 0xFF;

    /* We're going to want to trigger some events synchronously with the timer,
     * so we'll set the TIMSK: Timer/Counter Interrupt Mask here.
     *
     */

    TIMSK |= _BV( OCIE1A )


    // We want the PWM output on PB0 -- so it's got to be set as an output 
    DDRB |= (1 << PB0);

     

    // This enables global interrupts now and forever--until they're turned off 
    sei(); 

    // Turn the timer on, stand back, and do nothing. 
    GTCCR = _BV(TSM);

    while(1) {
    
    }

    return 0;
}

