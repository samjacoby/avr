import serial
import struct

class I2CInterface:

    def __init__(self, port="/dev/cu.usbserial-FTF0GEYG"):
        self.serial = serial.Serial(port, 57600)

    def probe(self, addr):
        self.serial.flushInput()
        self.serial.write("p" + chr(addr))
        resp = self.serial.read(1)
        return resp

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
            print "got header"
            nbytes = ord(self.serial.read(1))
            print "%d bytes to read" % nbytes
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
        

