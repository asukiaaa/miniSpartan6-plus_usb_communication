//
// A program to read write serial in 12Mbit/s with using libftdi1.
//
// Written by Asuki Kono (https://github.com/asukiaaa)
//
// License: MIT
//
// references:
// http://penguin.tantin.jp/hard/libFTDI.html
// https://www.intra2net.com/en/developer/libftdi/documentation/
// https://github.com/legege/libftdi/blob/master/test/baudrate.cpp
// https://github.com/legege/libftdi/blob/master/src/ftdi.h
//

//
// Issue of this program
// Kerner driver is detouched after running this program.
// simillar issue: http://blog.gmane.org/gmane.comp.hardware.libftdi.general/month=20110601
//
#include <libftdi1/ftdi.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define TARGET_BAUDRATE   12000000 // 12Mbit/s
#define TARGET_VENDOR_ID  1027
#define TARGET_PRODUCT_ID 24592
#define TARGET_INTERFACE  INTERFACE_B

int main(void) {
  struct ftdi_context* ftdic;
  int result;

  // open ftdi a device
  ftdic = ftdi_new();
  ftdi_set_interface(ftdic, TARGET_INTERFACE);
  result = ftdi_usb_open(ftdic, TARGET_VENDOR_ID, TARGET_PRODUCT_ID);

  // result < 0   : opened
  // result == -5 : ftdi_iso is running
  if ( result < 0 && result != -5 ) {
    fprintf(stderr, "unable to open ftdi device: %d (%s)\n", result, ftdi_get_error_string(ftdic));
    exit(-1);
  }

  printf("ftdi open succeeded: %d\n", result);

  //ftdi_set_line_property(ftdic,
  //                       BITS_8,     // bit count
  //                       STOP_BIT_1, // stop bit type
  //                       NONE);      // parity type

  printf("type: %d\n", ftdic->type);
  printf("TYPE_2232H: %d\n", TYPE_2232H);

  // To set boardrate as 12Mbit/s,
  // libftdi version (maybe) 1 or more is needed
  // (This program is tested with using version 1.3)
  result = ftdi_set_baudrate(ftdic, TARGET_BAUDRATE);
  if ( result != 0 ) {
    fprintf(stderr, "failed to set baudrate: %d (%s)\n", result, ftdi_get_error_string(ftdic));
    exit(-1);
  }
  printf("baudrate: %d\n", ftdic->baudrate);

  const char* command_chars = "abcd";
  unsigned char sending_data[1];
  unsigned char received_data[255];
  int received_data_length = 0;

  for ( int i=0; i<4; i++ ) {

    // data send
    sending_data[0] = command_chars[i];
    ftdi_write_data(ftdic, sending_data, 1);
    printf("Send: %c\n", sending_data[0]);

    // data receive
    received_data_length = ftdi_read_data(ftdic, received_data, 254);
    received_data[received_data_length] = 0;
    printf("Received: %s\n", received_data);

    sleep(1);
  }

  ftdi_usb_close(ftdic);
  ftdi_free(ftdic);

  return 0;
}
