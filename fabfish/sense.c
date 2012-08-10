#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "transmit.h"
#include "sense.h"

#define NOP asm volatile("nop\n\t");


volatile uint8_t adc_done = 0;
volatile int16_t measurement[4];

static void sense_start(void);
static void sense_finish(void);

void sense_setup(void) {

}

static void sense_start(void) {
    adc_done = 0;
    ADCSRA = (1<<ADATE) | (1<<ADIE) | (1<<ADPS2) | (1<<ADPS0);
    ADMUX = 0;
}

void sense_measure(uint8_t txchan, uint8_t rxchan, uint8_t n) {
    uint8_t phase = 0;
    uint8_t count = 0;
    uint8_t done = 0;
    int16_t samp;

    measurement[0] = 0;
    measurement[1] = 0;
    measurement[2] = 0;
    measurement[3] = 0;
    
    transmit_select(txchan);
    transmit_start();
    sense_start();
    
    ADMUX |= rxchan;
    _delay_us(290);
    _delay_loop_1(3);
    NOP;

    ADCSRA |= (1<<ADEN);
    ADCSRA |= (1<<ADSC);

    while(!done) {
        while(!adc_done);
        samp = ADC - 512;
        adc_done = 0;
        if(phase & 2) {
            measurement[phase & 1] += samp;
        } else {
            measurement[phase & 1] -= samp;
        }
        if(phase == 3) count++;
        phase = (phase + 1) & 3;
        if(count == n) done = 1;
    }

    sense_finish();
}

static void sense_finish(void) {
    ADCSRA &= 0;
    transmit_quench();
}

void sense_get_measurement(int16_t *inphase, int16_t *quad) {
    *inphase = measurement[0];
    *quad = measurement[1];
}

ISR(ADC_vect) {
    adc_done = 1;
}

