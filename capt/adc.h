#ifndef __adc_h_
#define __adc_h_

extern volatile uint8_t adc_complete; 

void adc_init(void);
void adc_start(void);
void adc_stop(void);

void pwm_init(void);
void pwm_start(void);
void pwm_stop(void);

void sensor_read(uint8_t);
void fetch_sensor_read(int16_t *, int16_t *);

#endif // __adc_h_
