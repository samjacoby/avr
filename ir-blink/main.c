#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

// Define relevant registers in the slave device
// #include "register_map.h"
// Version information, for late queries and such.
// #include "version.h"

#define IR_FREQ 38000L
#define FLICKER 3750
#define F_CPU 16000000	// 16 MHz oscillator.
#define BaudRate 9600
#define MYUBRR (F_CPU / 16 / BaudRate ) - 1 

volatile uint8_t ir_count;

void setup(void) {

    // Set chip to run at 16MHz
    CLKPR = (1 << CLKPCE); 
    CLKPR = 0;

    // Set-up serial
    DDRD |= 1 << PD1;

    // CONFIGURE TIMER0
    // Configure 38Khz
    // Run the timer w/ a prescale
    TCCR0B = 1 << CS01;

    OCR0A = 25;     // regulates frequency. some number.
    OCR0B = 12;     // doesn't really do anything now... 

    TCCR0A |= (1 << WGM01 ) | (1 << COM0B0);

    // Enable interrupts in the right places
    //TIMSK0 = 1 << OCIE0B;

    // CONFIGURE TIMER1
    TCCR1B |= (1 << WGM12) | (1 << CS11) | (1 << CS10);   // CTC + Clock scaling 
    OCR1A = FLICKER / 64;

    TIMSK1 = (1 << OCIE1A); // Interrupt enable

    DDRB |= 0xff; 
    DDRC |= 0xff; 

}


/* Serial Funcs */
void serialInit() {
 	/*Set baud rate */ 
	UBRR0H = (unsigned char) (MYUBRR>>8); 
	UBRR0L = (unsigned char) MYUBRR; 
	/* Enable receiver and transmitter   */
	UCSR0B = (1<<RXEN0)|(1<<TXEN0); 
	/* Frame format: 8data, No parity, 1stop bit */ 
	UCSR0C = (3<<UCSZ00);  

}

unsigned char serialCheckRxComplete(void)
{
	return( UCSR0A & _BV(RXC0)) ;		// nonzero if serial data is available to read.
}

unsigned char serialCheckTxReady(void)
{
	return( UCSR0A & _BV(UDRE0) ) ;		// nonzero if transmit register is ready to receive new data.
}

unsigned char serialRead(void)
{
	while (serialCheckRxComplete() == 0)		// While data is NOT available to read
	{;;} 
	return UDR0;
}

void serialWrite(unsigned char DataOut)
{
	while (serialCheckTxReady() == 0)		// while NOT ready to transmit 
	{;;} 
	UDR0 = DataOut;
}



ISR(TIMER1_COMPA_vect) {
    static uint8_t count;

    count = (count + 1) & 0b00111111;

    
    if(count < ir_count) { 
        DDRD |= 1 << PD5;
    } else {
        PORTB |= (1 << PB3);
        DDRD &= 0;
    }

}


int inByteInt;
unsigned char inByte;
void loop() {
    sei();

	if (serialCheckRxComplete()) {

		inByte = serialRead();
        ir_count = (int) inByte; 
        //ir_count = 32; 
        /*    
        for(i = 0; i < 64; i++) {
            ir_count = i;
            _delay_ms(15);
        }
        _delay_ms(220);
        for(i = 63; i >= 0; i--) {
            ir_count = i;
            _delay_ms(15);
        }
        _delay_ms(60);
        */
        
    }
    /*
    int readByte;
    unsigned char readBytes[4];


	if (serialCheckRxComplete()) {

        PORTC ^= (1 << PC0);

        //if(i > 3) {
        //    i = 0;
        //}

		inByte = serialRead();
        readByte = map(0, 9, 0, 64);
        ir_count = 32; 

        if(inByte == ' ') {
            PORTB ^= (1 << PB4);
            ir_count = 32;
            //ir_count = atoi(readBytes);
        }

        //readBytes[i] = inByte;

        //i++; 
    }
    PORTB |= (1 << PB4);
    _delay_ms(1000);
    PORTB &= ~(1 << PB4);
    _delay_ms(1000);
    ir_count = 32;
    */
}

int map(int x, int in_min, int in_max, int out_min, int out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

int main(void) {

    setup();
    serialInit();
while(1) {
        loop(); 
    }
    
    return 0;
}



