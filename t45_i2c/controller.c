
#include "controller.h"
#include "feed.h"

int16_t sensor_val

/*
void controller_init(void) {
    controller_status = 0;
    controller_torque = 0;
}
*/
void controller_set_val(uint16_t val) {

    sensor_val = val;
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

