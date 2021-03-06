#include <avr/io.h>
#include <avr/interrupt.h>

//#include "interface.h"
//#include "register_map.h"

volatile uint8_t adc_complete = 0;
volatile int16_t measurement[2];

void pwm_start(void) {
    // Set the clock source to be the internal one. CS00 is no prescaling. 
    TCCR0B = _BV( CS00 );
}

void pwm_stop(void) {
    // Clear clock bits to disable PWM 
    TCCR0B &= 0xf8;
}

// Let er' roll. 
void adc_start(void) {
    // Start a conversion
    ADCSRA |= _BV( ADEN );
    ADCSRA |= _BV( ADSC );

}

void adc_stop(void) {
    // Disable conversion
    ADCSRA &= ~(_BV( ADEN ) | _BV( ADSC ));
}

// Initialize the ADC.
// We're going to be PWMing on PB1 and listening on PB3.
// How's that sound?
void adc_init(void) {

    /* ADMUX: ADC Multiplexer Selection Register 
     *
     * REFS[2:0]
     * Set voltage reference bits. We'll just use Vcc.
     *
     * NOTE: not used because, well, we don't need to set anything here.
     */

    // This sets pin PB3 as the analog input read
    ADMUX = 0x03;

    /* ADCSRA: Analog-Digital Converter Status Register A
     *
     * This controls just about everything we're interested in.
     *
     * ADEN
     * Enables the ADC.
     *
     * ADSC
     * Might as well start sampling. It'll be synchronous with the clock, so 
     * it should be fine. There is a 25 cycle start-up anyhow.
     *
     * ADATE
     * Enable auto-triggering, which is necessary for free-running mode. We
     * choose the trigger source in ADCRSB, but it's all zeros for FRM so 
     * we won't.
     *
     * ADIE
     * Allow ADC interrupts to report home 
     *
     * ADPS[2:0]
     * These three bits control the prescaling from the system clock.
     * There is less resolution in the prescaling control for the ADC, 
     * so we want to choose a value that make sense.
     *
     */

    // This is ADC prescaling /16.
    ADCSRA = _BV( ADATE ) | _BV( ADEN ) | _BV( ADIE ) | _BV( ADPS2 );

    // Enable output on this pin, for debugging on a scope.
    DDRB |= _BV(PB4);

}

/* Handle the initialization of the PWM 
 *
 */
void pwm_init() {

    // Initiliazing the PWM settings shouldn't start it off rolling.
    pwm_stop();

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
    // We're interested in setting PB1, here, which is OC0B, enabled by COM0B0
    
    TCCR0A = _BV( COM0B0 ) | _BV ( WGM01 );

    // Set the corresponding PWM pin to output
    DDRB |= _BV( PB1 );

    /* TCCR0B: Timer/Counter Control Register B
     *
     * This register controls another range of functions. The key stuff here
     * are the the lower bits, which tell the microcontroller what clock to 
     * listen to, and what speed we want to listen at. We just want to use 
     * the onboard clock -- but 8MHz (or 1MHz) is pretty quick, so we'll reduce the 
     * speed some. This happens by using a prescaler, which automatically
     * divides the counter speed by certain set values.      
     * 
     */



    // The output frequency is CLK / 2 * prescale * (1 + OCROA)
    OCR0A = 31;

}

void sensor_read(uint8_t sample_number) {

    uint8_t read_complete = 0;
    uint8_t counter = 0;
    uint8_t phase = 0;

    int16_t sample;

    measurement[0] = 0;
    measurement[1] = 0;

    int16_t samples[4];
    samples[0] = 1;
    samples[1] = 2;
    samples[2] = 2;
    samples[3] = 1;
    // Read a number of samples
    while(!read_complete) {
        /*
        sample = ADC; 
        sample = 0x3ff;
        measurement[0] = sample;
        measurement[1] = sample;
        read_complete = 1;
*/
        // This blocks, waiting for the interrupt to hit.
        // Once the interrupt is hit, we drop out of the while, and do
        // everything that follows. 
        //while(!adc_complete);
        // Read the sample, subtracting out half...
        //sample = ADC;
        sample = ADC - 512;
        //sample = ADC - 512;

        //sample = samples[phase];

        adc_complete = 0;

        // Depending on which reading this is, stash it in the right place
        if(phase & 2) { 
            measurement[phase & 1] -= sample;
//            measurement[phase & 1] = -128;
        } else {
            measurement[phase & 1] += sample;
 //           measurement[phase & 1] = 128;
        }
        // If we're at the last read, add this to the tally.
        
        if(phase == 3) counter++;

        // Incremement the phase between 0 and 3
        phase = (phase + 1) & 3;

        // If we've done all the reading we want to do, go home.
        if(counter == sample_number)  {
            read_complete = 1;
        }
    }
}

/* Deliver the goods back home 
 */
void fetch_sensor_read(int16_t *inphase, int16_t *quad) {

    *inphase = measurement[0];
    *quad = measurement[1];

}

/* Interrupt Code
 * This bit of code is executed everytime something magnificent happens
 */

// ADC Complete interrupt
// This needs some kinds of counter control

ISR(SIG_ADC) {
    // Pulse for debug
    PORTB ^= _BV ( PB4 );
    PORTB ^= _BV ( PB4 );
    adc_complete = 1;
}
