import serial
import sys
import struct
import time

class I2CInterface:

    def __init__(self, port="/dev/ttyUSB0"):
        self.serial = serial.Serial(port, 57600)

    def probe(self, addr):
        self.serial.flushInput()
        self.serial.write("p" + chr(addr))
        resp = self.serial.read(1)
        return resp

    def send_repeat_delay(self, addr, data, rxbytes, delay):
        while(1):
            sensor_val = ord(self.send(addr, chr(data), rxbytes))
            sensor_quad_val = ord(self.send(addr, chr(data + 2), rxbytes))
            str = ''.join(['*' for num in xrange(sensor_val/10)])
            print str + ': %d' % sensor_val
            str2 = ''.join(['#' for num in xrange(sensor_quad_val/10)])
            print str2 + ': %d' % sensor_quad_val

            #    p += '*' 
            #    print p + ":" + sensorval 
            time.sleep(delay)

    def send(self, addr, data, rxbytes):
        self.serial.flushInput()
        self.serial.write("e" + chr(addr) + chr(len(data)) + chr(rxbytes) + data)
        header = self.serial.read(1)
        if header == "!":
            print "i2c start failed"
            return None
        elif header == "~":
            print "repeated start failed"
            return None
        elif header == "*":
            #print "got header"
            nbytes = ord(self.serial.read(1))
            #print "%d bytes to read" % nbytes
            if(nbytes > 0):
                rdata = self.serial.read(nbytes)
                return rdata
            else:
                return ""
        else:
            print "bad response header"
            return None

    def write(self, addr, reg, val):
        self.send(addr, chr(reg) + chr(val), 0)

    def set_torque_mode(self, addr):
        self.send(addr, chr(0x80) + chr(1), 0)

    def set_torque(self, addr, torque):
        data = struct.pack("Bb", 0x81, torque)
        self.send(addr, data, 0)
        
def main(argv):
    print argv
    addr = 0x4A;
    data = 0x80;
    rxbytes = 1;
    i2c = I2CInterface(port=argv[0])
    
    i2c.send_repeat_delay(addr, data, rxbytes, float(argv[1]))


if __name__ == "__main__":
    main(sys.argv[1:])