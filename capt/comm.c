#include <avr/io.h>
#include <avr/interrupt.h>
//#include <avr/delay.h>

#include "usi_i2c_45.h"
#include "interface.h"
#include "controller.h"
#include "adc.h"


// Define relevant registers in the slave device
#include "register_map.h"
// Version information, for late queries and such.
#include "version.h"

void setup(void) {
    // Set the i2c device address;
    uint8_t device_address; 

    // Set chip to run at 8MHz
    CLKPR = (1 << CLKPCE);
    CLKPR = 0;


    // Set the device address. This can be loaded from EEPROM if need be.
    device_address = 0x4A;
    i2c_init(device_address);
    i2c_set_read_fn(&interface_read_reg);
    i2c_set_write_fn(&interface_write_reg);

    // Set up ADC and PWM
    adc_init();
    adc_start();
    pwm_init();

}

void loop() {

}

int main(void) {

    setup();
    sei();

    while(1) {
        loop(); 
    }
    
    return 0;
}
