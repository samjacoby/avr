#ifndef __feed_h_
#define __feed_h_

#include <avr/io.h>

static volative uint16_t sensor_value;

void sensor_set_value(int16_t val) {
   sensor_value = val; 
}


#endif // __feed_h_
