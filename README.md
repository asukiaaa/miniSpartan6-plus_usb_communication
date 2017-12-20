# miniSpartan6-plus_usb_communication
A project to communicate a FPGA board (miniSpartan6+) and a PC via USB in 12Mbit/s. One byte takes 12 bits (2 start bits, 8 data bit and 2 stop bits), so It may goes 1.0MByte/s.

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
You can communicate with Spartan6 with using libftdi or ioctl.

### libftdi

libftdi ver1 or more is needed. Ubuntu apg-get supports ver 0.2 so installing latest libftdi from source is needed.

https://www.intra2net.com/en/developer/libftdi/download.php

```sh
cd ./cpp
make
sudo ./bin/serial_with_libftdi
```

### ioctl

```sh
cd ./cpp
make
sudo ./bin/serial_with_ioctl
```

### libusb

I also wrote program with using libusb but it does not run correctry only that.
Libusb can't set baudrate so this program needs to run another program to set baudrate.

```sh
cd ./cpp
make
sudo ./bin/serial_with_ioctl # run to set baudrate
sudo ./bin/serial_with_libusb
```

# License
MIT

# References
- [ubuntuでlibftdiを使って12Mbit/sのシリアル通信をする方法](http://asukiaaa.blogspot.jp/2016/05/ubuntulibftdi12mbits.html)
- [Connecting USB FT2232 to an Microblaze UART](https://www.scarabhardware.com/forums/topic/connecting-usb-ft2232-to-an-microblaze-uart/)
- [How to reach (close to?) 480Mbps over USB?](https://www.scarabhardware.com/forums/topic/how-to-reach-close-to-480mbps-over-usb/)
