#ifndef __adc_h_
#define __adc_h_

extern volatile uint8_t adc_complete; 

void adc_start(void);
void adc_init(void);
void pwm_init(void);
void sensor_read(uint8_t);

#endif // __adc_h_
