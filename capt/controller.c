#include <stdint.h>

#include "controller.h"

int8_t phase_sensor_val_h;
int8_t phase_sensor_val_l;
int8_t quad_sensor_val_h;
int8_t quad_sensor_val_l;

/*
void controller_init(void) {
    controller_status = 0;
    controller_torque = 0;
}
*/
void controller_set_val(int16_t val, int16_t quad_val) {
/*
    // 16 bit values need to be mapped to distinc registers 
    phase_sensor_val_l = val;
    phase_sensor_val_h = val >> 8;
    
    quad_sensor_val_h = quad_val >> 8;
    quad_sensor_val_l = quad_val << 8;
    */
    phase_sensor_val_l = val & 0x00ff;
    phase_sensor_val_h = (val & 0xff00) >> 8;
    
    quad_sensor_val_h = quad_val & 0x00ff; 
    quad_sensor_val_l = (quad_val & 0xff00) >> 8; 
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

