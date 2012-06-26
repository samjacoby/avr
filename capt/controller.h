#ifndef __controller_h_
#define __controller_h_

#include <stdint.h>
/*
#define CONTROLLER_MODE_OFF 0x00
#define CONTROLLER_MODE_TORQUE 0x01
#define CONTROLLER_MODE_POSITION 0x02
#define CONTROLLER_MODE_VELOCITY 0x03

extern uint8_t controller_status;
extern int8_t controller_torque;
*/
extern int8_t phase_sensor_val_h;
extern int8_t phase_sensor_val_l;
extern int8_t quad_sensor_val_h;
extern int8_t quad_sensor_val_l;

//void controller_init(void);
//void controller_task(void);

void controller_set_val(int16_t val, int16_t quad_val);
//void controller_set_torque(int8_t torque);

#endif // __controller_h_

