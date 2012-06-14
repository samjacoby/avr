#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/delay.h>

#include "usi_i2c_45.h"
#include "interface.h"
#include "controller.h"

// Define relevant registers in the slave device
#include "register_map.h"
// Version information, for late queries and such.
#include "version.h"

void setup(void) {
    // Set the i2c device address;
    uint8_t device_address;
    CLKPR = (1 << CLKPCE);
    CLKPR = 0;

    device_address = 0x4A;

    // Set the device address. This can be loaded from EEPROM if 
    // need be.
    i2c_init(device_address);
    i2c_set_read_fn(&interface_read_reg);
    i2c_set_write_fn(&interface_write_reg);


}


int main(void) {
    setup();
    sei();

    while(1) {
    
    }
    
    return 0;
}
