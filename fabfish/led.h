#ifndef __led_h_
#define __led_h_

#include <avr/io.h>

#define led2_on() PORTB |= (1<<PB1)
#define led2_off() PORTB &= ~(1<<PB1)

#define led1_on() PORTB |= (1<<PB0)
#define led1_off() PORTB &= ~(1<<PB0)

#define power_led_on() PORTD |= (1<<PD7)
#define power_led_off() PORD &= ~(1<<PD7)

void led_setup(void);

#endif // __led_h_

