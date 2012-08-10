#include "led.h"

void led_setup(void) {
    DDRB |= (1<<PB0) | (1<<PB1);
    DDRD |= (1<<PD7);
}

