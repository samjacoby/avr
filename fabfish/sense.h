#ifndef __sense_h_
#define __sense_h_

#include <stdint.h>

void sense_setup(void);

void sense_measure(uint8_t txchan, uint8_t rxchan, uint8_t n);

void sense_get_measurement(int16_t *inphase, int16_t *quad);

#endif // __sense_h_

