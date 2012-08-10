#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include <stdio.h>

#include "uart.h"
#include "led.h"
#include "transmit.h"
#include "sense.h"

#define VERSION_MAJOR 3
#define VERSION_MINOR 0

int16_t quad, inphase;

static FILE uart_stream = FDEV_SETUP_STREAM(
    uart_putchar_f,
    uart_getchar_f,
    _FDEV_SETUP_RW
);

void setup(void) {
    stdout = &uart_stream;
    stderr = &uart_stream;
    uart_setup();
    led_setup();
    transmit_setup();

    power_led_on();
}

void loop(void) {
    char c;

    c = uart_getchar(1);
    led1_on();
    switch(c) {
        case 'v':
            printf("FabFish %d.%d\n", VERSION_MAJOR, VERSION_MINOR);
            break;
        case 'T':
            c = uart_getchar(1);
            if(c == '1') {
                transmit_select(0);
            } else if(c == '2') {
                transmit_select(1);
            } else {
                break;
            }
            transmit_start();
            _delay_ms(15000);
            transmit_quench();
            break;
        case 'q':
            transmit_quench();
            break;
        case 'a':
            sense_measure(0, 0, 32);
            sense_get_measurement(&inphase, &quad);
            printf("%d\t%d\n", inphase, quad);
            break;
        case 'x':
            uart_putchar('x');
            uart_putchar('x');
            sense_measure(0, 0, 32);
            sense_get_measurement(&inphase, &quad);
            uart_putchar(inphase & 0xFF);
            uart_putchar(inphase >> 8);
            uart_putchar(quad & 0xFF);
            uart_putchar(quad >> 8);
            sense_measure(0, 1, 32);
            sense_get_measurement(&inphase, &quad);
            uart_putchar(inphase & 0xFF);
            uart_putchar(inphase >> 8);
            uart_putchar(quad & 0xFF);
            uart_putchar(quad >> 8);
            sense_measure(1, 0, 32);
            sense_get_measurement(&inphase, &quad);
            uart_putchar(inphase & 0xFF);
            uart_putchar(inphase >> 8);
            uart_putchar(quad & 0xFF);
            uart_putchar(quad >> 8);
            sense_measure(1, 1, 32);
            sense_get_measurement(&inphase, &quad);
            uart_putchar(inphase & 0xFF);
            uart_putchar(inphase >> 8);
            uart_putchar(quad & 0xFF);
            uart_putchar(quad >> 8);
            break;
        default:
            uart_putchar('?');
    }
    led1_off();
}

int main(void) {
    setup();
    sei();
    for(;;) {
        loop();
    }
    return 0;
}

