#ifndef __adc_h_
#define __adc_h_

extern volatile uint8_t adc_complete; 

void timer_start(void);
void timer_stop(void);
void adc_init(void);

#endif // __adc_h_
