import sys
import math

# clock prescaler
p_t = [1, 8, 64, 256, 1024]

# adc prescaler
p_adc = [8, 16, 32, 64, 128]

# list of possible cycle gaps 
cycles =  (4,8)

def f_t(p, ocr):
    clk = 8000000
    return float(clk)/(2 * p * (1 + ocr))

r = lambda x, y, n: 1/(((2*float(x))/(13 * y))*(n + 1/4)) - 1

for p in p_t:
    for t in p_adc:
        for i in cycles:

            y = r(p,t,i)

            if(y == int(y) and y > 0 and y < 256):

                # transmission frequency
                f = f_t(p, y)

                # sample frequency
                fs  = 8000000 / (float(t) * 13) 

                if(fs < f):
                    print 'timer: %d, adc: %d, cycles, %d: ocr0a: %f f_t = %f, f_s = %f' % (p,t,i,y, f, fs)




