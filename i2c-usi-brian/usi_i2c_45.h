#ifndef __usi_i2c_h_
#define __usi_i2c_h_

#include <stdint.h>

#define SDA PB0
#define SCL PB2

typedef enum {
    USI_STATE_CHIPADDR,
    USI_STATE_ACK_CHIPADDR,
    USI_STATE_ACK_CHIPADDR_READ,
    USI_STATE_REGADDR,
    USI_STATE_ACK_DATA,
    USI_STATE_DATA,
    USI_STATE_READDATA,
    USI_STATE_REQ_READACK,
    USI_STATE_READ_READACK
} i2c_state_t;

#define I2C_READ 1
#define I2C_WRITE 0

/**
 * I2C Write Register function
 * takes address and new value as arguments
 */
typedef void (*i2c_write_fn_t)(uint8_t, uint8_t);

/**
 * I2C Read Register function
 * takes address as argument, returns register value
 */
typedef uint8_t (*i2c_read_fn_t)(uint8_t);

void i2c_init(uint8_t address);
void i2c_set_address(uint8_t address);

void i2c_set_read_fn(i2c_read_fn_t read_fn);
void i2c_set_write_fn(i2c_write_fn_t write_fn);


#endif // __usi_i2c_h_

