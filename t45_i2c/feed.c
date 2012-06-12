#include "feed.h"

static volatile int16_t sensor_value;

void sensor_set_value(int16_t val) {
   sensor_value = val; 
}
