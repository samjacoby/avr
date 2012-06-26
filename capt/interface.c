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
        case REG_PHASE_SENSOR_VALUE_H: return phase_sensor_val_h;
        case REG_PHASE_SENSOR_VALUE_L: return phase_sensor_val_l; 
        case REG_QUAD_SENSOR_VALUE_H: return quad_sensor_val_h; 
        case REG_QUAD_SENSOR_VALUE_L: return quad_sensor_val_l; 

        default: return 0xFF;
    }
}

/* Don't care about this right now */
void interface_write_reg(uint8_t reg, uint8_t val) {
    switch(reg) {
        case REG_PHASE_SENSOR_VALUE_H: controller_set_val(val, val); break;
    }
}

