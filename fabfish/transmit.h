#ifndef __transmit_h_
#define __transmit_h_

#define TX0 0
#define TX1 1

void transmit_setup(void);

void transmit_select(uint8_t chan);

void transmit_start(void);
void transmit_quench(void);


#endif // __transmit_h_

