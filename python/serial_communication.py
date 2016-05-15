import serial
import time

# serial_speed = 921600
# serial_speed = 115200
# serial_speed = 1843200
# serial_speed = 3686400
# serial_speed = 7372800
# serial_speed = 14745600
# serial_speed = 29491200
# serial_speed = 25000000

# serial_speed = 1500000  # 1.5Mbit/s
serial_speed = 12000000 # 12Mbit/s

# One byte takes 12 bits so 12Mbit/s may almost equal 1.0MByte/s.

spartan_ser = serial.Serial('/dev/ttyUSB1', serial_speed)

time.sleep(1)

while True:
  for char_to_send in ['a', 'b', 'c', 'd']:
    spartan_ser.write(char_to_send)
    print "send: " + char_to_send
    value = spartan_ser.read()
    print "received: " + value
    time.sleep(1)
