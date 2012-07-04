#include <stdint.h>
#include <avr/io.h>


#include "adc.h"
#include "controller.h"

uint8_t control_status; 
uint8_t debug;

int16_t inphase, quad;

uint8_t phase_sensor_val_h;
uint8_t phase_sensor_val_l;
uint8_t quad_sensor_val_h;
uint8_t quad_sensor_val_l;

// Initialize status
void controller_init(void) {
    control_status = CONTROL_MODE_STOP;
}


void controller_set_status_val(uint8_t val) {
    control_status = val;
}

/* Read status register, and do the right thing, depending on 
 * various magic values.
 */

void controller_task(void) {

    switch(control_status) {
        case CONTROL_MODE_READ:
            adc_start();
            pwm_start();
            sensor_read(32);
            fetch_sensor_read(&inphase, &quad);
            controller_set_sensor_val(inphase, quad);
            control_status = CONTROL_MODE_STOP;
            break;
        case CONTROL_MODE_STOP:
            adc_stop();
            pwm_stop();
            break;
        default:
            control_status = CONTROL_MODE_STOP;
    }
}


void controller_set_sensor_val(int16_t val, int16_t quad_val) {
    // 16 bit values need to be mapped to distinct "registers" 
    // for relaying over the i2c. 
    /    phase_sensor_val_l = (uint16_t) val & 0x00ff;
    phase_sensor_val_h = (uint16_t) val >> 8;
    quad_sensor_val_l = (uint16_t) quad_val & 0x00ff;  
    quad_sensor_val_h = (uint16_t) quad_val >> 8; 
}
/*
void controller_set_torque(int8_t torque) {
    uint8_t controller_mode = controller_status & 7;

    // only set the torque if we're in external torque control mode
    if(controller_mode == CONTROLLER_MODE_TORQUE) {
        controller_torque = torque;
    }
}

void controller_task(void) {
    uint8_t controller_mode;

    controller_mode = controller_status & 0x7;
    switch(controller_mode) {
        case CONTROLLER_MODE_OFF:
            hbridge_coast();
            break;
        case CONTROLLER_MODE_TORQUE:
            hbridge_set_torque(controller_torque);
            break;
        default:
            hbridge_coast();
            break;
    }
}
*/

