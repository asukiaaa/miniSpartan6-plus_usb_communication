import serial
import time

# baudrate = 921600
# baudrate = 115200
# baudrate = 1843200
# baudrate = 3686400
# baudrate = 7372800
# baudrate = 14745600
# baudrate = 29491200
# baudrate = 25000000

# baudrate = 1500000  # 1.5Mbit/s
baudrate = 12000000 # 12Mbit/s

# One byte takes 12 bits so 12Mbit/s may almost equal 1.0MByte/s.

spartan_ser = serial.Serial('/dev/ttyUSB1', baudrate)

time.sleep(1)

while True:
  for char_to_send in ['a', 'b', 'c', 'd']:
    spartan_ser.write(char_to_send)
    print "send: " + char_to_send
    value = spartan_ser.read()
    print "received: " + value
    time.sleep(1)
