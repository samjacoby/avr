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

        case REG_SENSOR_VALUE: return sensor_val;

        default: return 0xFF;
    }
}

void interface_write_reg(uint8_t reg, uint8_t val) {
    switch(reg) {
        case REG_SENSOR_VALUE: controller_set_val(val); break;
    }
}

