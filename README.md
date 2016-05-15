# miniSpartan6-plus_usb_communication
A project to communicate a FPGA board (miniSpartan6+) and a PC via USB in 12Mbit/s (1.5MByte/s).

# Programs
## ISE
This FPGA program waits signal from serial port. If it received some data, it returns same data.

```sh
# After you create bit file by ISE
cd ./ise/work
sudo xc3sprog -c ftdi mojo_top.bit
```
Original: https://github.com/roman3017/miniSpartan6-plus/tree/master/projects/mojo-uart

## Python
You can communicate with Spartan6 by the following command.

```sh
python ./python/serial_communication.py
```

## CPP
You can send signal to Spartan6 with using libusb. Send only now.

```sh
cd ./cpp
make
sudo serial_communication
```
Original: http://www.dreamincode.net/forums/topic/148707-introduction-to-using-libusb-10/

# License
MIT

# References
https://www.scarabhardware.com/forums/topic/connecting-usb-ft2232-to-an-microblaze-uart/
https://www.scarabhardware.com/forums/topic/how-to-reach-close-to-480mbps-over-usb/
