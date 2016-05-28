//
// A program to Communicate FT2232 via 12Mbit/s
//
// Written by Asuki Kono (https://github.com/asukiaaa)
//
// License: MIT
//
// References:
//   http://tldp.org/HOWTO/Serial-Programming-HOWTO/x115.html
//   http://forums.devshed.com/programming-42/ioctl-termios-canonical-read-60821.html
//

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <termios.h>
#include <unistd.h>

#define DEVICE_PATH "/dev/ttyUSB1"
#define TARGET_BAUDRATE 12000000

using namespace std;

int main(void) {
  int fd;
  int serial;
  struct termios oldtio;
  struct termios newtio;

  fd = open(DEVICE_PATH, O_RDWR);
  if (fd <0) {perror("cannot open"); exit(-1); }

  tcgetattr(fd, &oldtio);
  bzero(&newtio, sizeof(&newtio));

  ioctl(fd, TIOCMGET, &serial);

  /*
    BAUDRATE: Set bps rate. You could also use cfsetispeed and cfsetospeed.
    CRTSCTS : output hardware flow control (only used if the cable has
    all necessary lines. See sect. 7 of Serial-HOWTO)
    CS8     : 8n1 (8bit,no parity,1 stopbit)
    CLOCAL  : local connection, no modem contol
    CREAD   : enable receiving characters
  */
  newtio.c_cflag = TARGET_BAUDRATE | CRTSCTS | CS8 | CLOCAL | CREAD;

  /*
    IGNPAR  : ignore bytes with parity errors
    ICRNL   : map CR to NL (otherwise a CR input on the other computer
    will not terminate input)
    otherwise make device raw (no other input processing)
  */
  newtio.c_iflag = IGNPAR | ICRNL;

  newtio.c_oflag = OPOST; // raw output

  newtio.c_lflag = ECHO; // echo off

  tcflush(fd, TCIOFLUSH);
  tcsetattr(fd, TCSANOW, &newtio);

  const char* command_chars = "abcd";

  // data to write
  unsigned char data[1];

  int received_length = 0;
  unsigned char received_data[255];

  for ( int i=0; i<4; i++ ) {

    // data send
    data[0] = command_chars[i];
    write(fd, data, 1);
    printf("Send: %c\n", data[0]);

    usleep(1000);

    // data receive
    received_length = read(fd, received_data, 254);
    received_data[received_length] = 0;
    printf("Received: %s\n", received_data);

    sleep(1);
  }

  int un_read_byte_length;
  ioctl(fd, FIONREAD, &un_read_byte_length);
  printf("un read bytes available: %d\n", un_read_byte_length);

  close(fd);
  ioctl(fd, TIOCSSERIAL, &serial);
}
