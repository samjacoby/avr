#ifndef __interface_h_
#define __interface_h_

#include <stdint.h>

uint8_t interface_read_reg(uint8_t reg);
void interface_write_reg(uint8_t reg, uint8_t val);

#endif // __interface_h_

