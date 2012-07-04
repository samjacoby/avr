#ifndef __controller_h_
#define __controller_h_

#include <stdint.h>

#define CONTROL_MODE_STOP 0x00
#define CONTROL_MODE_READ 0x01
/*
#define CONTROLLER_MODE_POSITION 0x02
#define CONTROLLER_MODE_VELOCITY 0x03

extern uint8_t controller_status;
extern int8_t controller_torque;
*/
extern uint8_t control_status;
extern uint8_t debug;

// Store the high and low bytes of the ADC vals
extern uint8_t phase_sensor_val_h;
extern uint8_t phase_sensor_val_l;
extern uint8_t quad_sensor_val_h;
extern uint8_t quad_sensor_val_l;

//void controller_init(void);
//void controller_task(void);

void controller_set_status_val(uint8_t);
void controller_set_sensor_val(int16_t, int16_t);
void controller_task(void);
//void controller_set_torque(int8_t torque);

#endif // __controller_h_

