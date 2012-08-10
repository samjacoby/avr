#include "controller.h"
#include "encoder.h"

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

        case REG_CONTROLLER_STATUS: return controller_status;
        case REG_CONTROLLER_TORQUE: return controller_torque;

        case REG_ENCODER_STATUS: return encoder_status;

        default: return 0xFF;
    }
}

void interface_write_reg(uint8_t reg, uint8_t val) {
    switch(reg) {
        case REG_CONTROLLER_STATUS: controller_set_status(val); break;
        case REG_CONTROLLER_TORQUE: controller_set_torque(val); break;
    }
}

