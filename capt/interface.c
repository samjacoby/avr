#include <avr/io.h>
#include "controller.h"

#include "interface.h"
#include "register_map.h"
#include "version.h"

uint8_t interface_read_reg(uint8_t reg) {

    switch(reg) {

        case REG_SIGNATURE: return FW_SIGNATURE;
        case REG_INITIALS1: return INITIALS1;
        case REG_INITIALS2: return INITIALS2;
        case REG_INITIALS3: return INITIALS3;
        case REG_VERSION_L: return VERSION_L;
        case REG_VERSION_H: return VERSION_H;

        case REG_CONTROL: return control_status;
        case REG_DEBUG: return debug;

        case REG_PHASE_SENSOR_VALUE_L: return phase_sensor_val_l; 
        case REG_PHASE_SENSOR_VALUE_H: return phase_sensor_val_h;
        case REG_QUAD_SENSOR_VALUE_L: return quad_sensor_val_l; 
        case REG_QUAD_SENSOR_VALUE_H: return quad_sensor_val_h; 

        default: return 0xFF;
    }
}

void interface_write_reg(uint8_t reg, uint8_t val) {
    switch(reg) {
        case REG_CONTROL: controller_set_status_val(val); break;
        case REG_PHASE_SENSOR_VALUE_H: controller_set_sensor_val(val, val); break;
    }
}

