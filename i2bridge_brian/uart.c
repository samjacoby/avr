#include <avr/io.h>
#include <avr/interrupt.h>
#include "uart.h"

// UART buffers
volatile char rxbuf[64];
volatile uint8_t rdptr = 0;
volatile uint8_t wrptr = 0;

/**
 * Set up the UART
 */
void uart_init() {
	UBRR0H = 0;
	UBRR0L = 42;
	UCSR0A |= (1<<U2X0);
	UCSR0B |= (1<<RXEN0) | (1<<TXEN0) | (1<<RXCIE0);
}

void uart_putchar(uint8_t c) {
	while( ! (UCSR0A & (1<<UDRE0)) );
	UDR0 = c;
}

int uart_putchar_f(char c, FILE *stream) {
    if(c == '\n') uart_putchar_f('\r', stream);
	while( ! (UCSR0A & (1<<UDRE0)) );
	UDR0 = c;
    return 0;
}

void uart_putdata(uint8_t *data, uint8_t count) {
	int i;
	for(i=0; i<count; i++) uart_putchar(data[i]);
}

uint8_t uart_data_available() {
    return !(rdptr == wrptr);
}

int16_t uart_getchar(uint8_t blocking) {
	char c;
    if(blocking) {
        while(rdptr == wrptr) { }
    } else {
        if(rdptr == wrptr) return -1;
    }
    cli();
	c = rxbuf[rdptr];
	rdptr = (rdptr + 1) & 63;
    sei();
	return c;
}

int uart_getchar_f(FILE *stream) {
    char c;
    while(rdptr == wrptr);
    cli();
    c = rxbuf[rdptr];
    rdptr = (rdptr + 1) & 63;
    sei();
    return c;
}

/**
 * UART Receive ISR
 */
ISR(USART_RX_vect) {
	rxbuf[wrptr] = UDR0;
    //uart_putchar(rxbuf[wrptr]);
	wrptr = (wrptr + 1) & 63;
	if(wrptr == rdptr) rdptr = (rdptr + 1) & 63;
}

