#include <avr/io.h>
#include <avr/interrupt.h>

#include "usi_i2c.h"

static volatile uint8_t i2c_address;
static volatile i2c_state_t i2c_state;
static volatile uint8_t i2c_regaddr;
static volatile uint8_t i2c_mode;

static uint8_t i2c_read_stub(uint8_t reg) { return 0xFF; }
static void i2c_write_stub(uint8_t reg, uint8_t val) { }

static volatile i2c_read_fn_t i2c_read_reg = &i2c_read_stub;
static volatile i2c_write_fn_t i2c_write_reg = &i2c_write_stub;

void i2c_init(uint8_t address) {
    i2c_address = address;

    
    // Sets two-wire mode using SDA and SCL pins. Sets clock
    // to do something interested. External, negative edge.
    USICR = (1<<USICS1) | (1<<USICS0) | (1<<USIWM1) | (1<<USIWM0);
    // Set SCL and SCA high 
    //PORTA1 |= (1<<SCL) | (1<<SDA);
    // Enable output on SCL.
    DDRA |= (1<<SCL);
    // Set the Start Condition Interrupt Enabler (USISIE)
    USICR |= (1<<USISIE);
    // I don't know what this does, but it enables output on PA5.
    //DDRA |= (1<<PA5);
}

void i2c_set_address(uint8_t address) {
    i2c_address = address;
}

void i2c_set_read_fn(i2c_read_fn_t read_fn) {
    i2c_read_reg = read_fn;
}

void i2c_set_write_fn(i2c_write_fn_t write_fn) {
    i2c_write_reg = write_fn;
}

ISR(USI_START_vect) {

    //PORTA1 |= (1<<5);
    USISR = (1<<USISIF) | (1<<USIOIF);
    USICR |= (1<<USIOIE) | (1<<USISIE) | (1<<USIWM0) | (1<<USICS1) | (1<<USICS0);

    // This checks for the start condition: SCL is held high, while SDA is low. 
    // PINB is the input port for these pins -- so this is polling the input
    // pins
    while( (INPORT & (1<<SCL)) && ! (INPORT & (1<<SDA)) );

        USISR = 0;
        i2c_state = USI_STATE_CHIPADDR;
        //PORTA1 &= ~(1<<5);
}

ISR(USI_OVF_vect) {
    uint8_t data;
    data = USIBR;
    switch(i2c_state) {
        case USI_STATE_CHIPADDR:
            // Knocks off rw state.
            if( (data>>1) == i2c_address ) {
                USIDR = 0;
                DDRA |= (1<<SDA);
                // Clear various vonditions
                USISR = (1<<USIOIF) | (1<<USIPF) | (1<<USIDC) | 0x0E;
                // ack chipaddr
                if(data & I2C_READ) {
                    i2c_state = USI_STATE_READDATA;
                } else {
                    i2c_state = USI_STATE_ACK_CHIPADDR;
                }
            } else {
                USICR &= ~((1<<USIOIE) | (1<<USIWM0));
                // Reset all kinds of flags
                USISR = (1<<USISIE) | (1<<USIOIF) | (1<<USIPF) | (1<<USIDC);
            }
            break;
        case USI_STATE_ACK_CHIPADDR:
            DDRA &= ~(1<<SDA);
            i2c_state = USI_STATE_REGADDR;
            USISR = (1<<USIOIF);
            break;
        case USI_STATE_ACK_CHIPADDR_READ:
            DDRA |= (1<<SDA);
            i2c_state = USI_STATE_READDATA;
            USISR = (1<<USIOIF);
            break;
        case USI_STATE_REGADDR:
            i2c_regaddr = data;
            DDRA |= (1<<SDA);
            USIDR = 0;
            i2c_state = USI_STATE_ACK_DATA;
            USISR = (1<<USIOIF) | 0x0E;
            break;
        case USI_STATE_ACK_DATA:
            DDRA &= ~(1<<SDA);
            i2c_state = USI_STATE_DATA;
            USISR = (1<<USIOIF);
            break;
        case USI_STATE_DATA:
            DDRA |= (1<<SDA);
            USIDR = 0;
            i2c_write_reg(i2c_regaddr++, data);
            i2c_state = USI_STATE_ACK_DATA;
            // Setting 0x0E moves the counter to two cycles away from overflow.
            USISR = (1<<USIOIF) | 0x0E;
            break;
        case USI_STATE_READ_READACK:
            //PORTA1 |= (1<<5);
            if(USIDR) {
                // master NACK'd
                DDRA &= ~(1<<SDA); 
                USICR &= ~((1<<USIOIE) | (1<<USIWM0));
                break;
            }
            // master wants more, continue into readdata
        case USI_STATE_READDATA:
            USICR &= ~(1<<USICS0);
            //PORTA1 |= (1<<5);
            DDRA |= (1<<SDA);
            USIDR = i2c_read_reg(i2c_regaddr++);
            USISR = (1<<USIOIF) | (1<<USIPF) | (1<<USIDC);
            i2c_state = USI_STATE_REQ_READACK;
            break;
        case USI_STATE_REQ_READACK:
            USICR |= (1<<USICS0);
            //PORTA1 &= ~(1<<5);
            
            USIDR = 0;
            DDRA &= ~(1<<SDA);
            
            USISR = (1<<USIOIF) | (1<<USIPF) | (1<<USIDC) | (0x0E);
            i2c_state = USI_STATE_READ_READACK;
            break;
    }
}

