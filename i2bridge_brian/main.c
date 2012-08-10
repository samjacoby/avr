#include <avr/io.h>
#include <avr/interrupt.h>

#include "i2cmaster.h"
#include "uart.h"

#define CMD_ACK '*'
#define CMD_NACK '!'

uint8_t node_probe(uint8_t addr) {
    uint8_t status;
    
    status = i2c_start((addr<<1) + I2C_WRITE);
    i2c_stop();

    return status;
}

void init(void) {
    // Turn off CLKDIV8
    CLKPR = (1<<CLKPCE);
    CLKPR = 0;

    // turn on the external pullups
    DDRC |= (1<<PC2);
    PORTC |= (1<<PC2);

    uart_init();
    i2c_init();
}

void loop(void) {
    int c, i;
    uint8_t addr, nw, nr;
    uint8_t buf[128];
    static uint8_t cmd = 0;

    c = uart_getchar(1); 
    switch(c) {
        case '?':
            uart_putdata((uint8_t*)"I2CBr1.0", 8);
            break;

        case 'p':
            // PROBE
            c = uart_getchar(1);
            addr = c & 0xFF;
            if(node_probe(addr)) {
                // device not responding
                uart_putchar('!');
            } else {
                // device responding
                uart_putchar('*');
            }
            break;

        case 'e':
            // EXCHANGE
            addr = uart_getchar(1); // slave address
            nw = uart_getchar(1); // n bytes to write
            nr = uart_getchar(1); // n bytes to read

            // this discards any overflow
            if(nw > 128) nw = 128;
            if(nr > 128) nr = 128;
            
            for(i=0; i<nw; i++) {
                buf[i] = uart_getchar(1);
            }

            addr = addr<<1;

            if(i2c_start(addr+I2C_WRITE)) {
                // start failed
                uart_putchar('!');
                break;
            }

            for(i=0; i<nw; i++) {
                i2c_write(buf[i]);
            }


            if(nr > 0) {
                if(i2c_rep_start(addr+I2C_READ)) {
                    uart_putchar('~');
                    break;
                }

                for(i=0; i<nr-1; i++) {
                    buf[i] = i2c_readAck();
                }
                buf[nr-1] = i2c_readNak();
            }

            i2c_stop();

            uart_putchar('*');
            uart_putchar(nr);
            for(i=0; i<nr; i++) {
                uart_putchar(buf[i]);
            }
            break;

        default:
            uart_putchar('?');
    }
}

int main(void) {
    init();
    sei();

    for(;;) {
        loop();
    }

    return 0;
}
    
