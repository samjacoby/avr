#ifndef __uart_h_
#define __uart_h_

#include <stdio.h>

#define RXD_PIN 0
#define TXD_PIN 1

void uart_init();
int16_t uart_getchar(uint8_t blocking);
void uart_putchar(uint8_t c);
void uart_putdata(uint8_t *data, uint8_t count);
uint8_t uart_data_available();

int uart_getchar_f(FILE *stream);
int uart_putchar_f(char c, FILE *stream);

#endif // __uart_h_

