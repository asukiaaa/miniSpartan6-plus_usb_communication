//
// A program for serial communication via USB
// references:
//   http://www.dreamincode.net/forums/topic/148707-introduction-to-using-libusb-10/
//   http://libusb.sourceforge.net/api-1.0/group__syncio.html
//
#include <iostream>
#include <libusb-1.0/libusb.h>

//
// FTDI lsusb info
// Bus 002 Device 021: ID 0403:6010 Future Technology Devices International, Ltd FT2232C Dual USB-UART/FIFO IC
// vendor_id:  hex 0403 -> decimal 1027
// product_id: hex 6010 -> decimal 24592
//
#define TARGET_VENDOR_ID 1027
#define TARGET_PRODUCT_ID 24592

//
// FTDI device info
//
// Number of possible configurations: 1
// Device Class: 0
// VendorID: 1027
// ProductID: 24592
// Device speed: 3
// Interfaces: 2
// Number of alternate settings: 1 | Interface Number: 0 | Number of endpoints: 2 | Descriptor Type: 5 | EP Address: 129 | Descriptor Type: 5 | EP Address: 2 |
// Number of alternate settings: 1 | Interface Number: 1 | Number of endpoints: 2 | Descriptor Type: 5 | EP Address: 131 | Descriptor Type: 5 | EP Address: 4 |
//
// Spartan6 is connected to port-B, so selected second interface.
//
#define TARGET_INTERFACE_NUM 1
#define TARGET_ENDPOINT_ADDRESS 4

using namespace std;

// a function to print information about a device
void printdev(libusb_device *dev);

int main() {
  // pointer to pointer of device, used to retrieve a list of devices
  libusb_device **devs;
  // a device handle
  libusb_device_handle *dev_handle;
  // a libusb session
  libusb_context *ctx = NULL;
  // for return values
  int r;
  // holding number of devices in list
  ssize_t cnt;

  // initialize the library for the session we just declared
  r = libusb_init(&ctx);
  if(r < 0) {
    // there was an error
    cout<<"Init Error "<<r<<endl;
    return 1;
  }

  // set verbosity level to 3, as suggested in the documentation
  libusb_set_debug(ctx, 3);

  // get the list of devices
  cnt = libusb_get_device_list(ctx, &devs);
  if(cnt < 0) {
    // there was an error
    cout<<"Get Device Error"<<endl;
    return 1;
  }
  cout<<cnt<<" Devices in list."<<endl;

  // print informatino of devices
  for (int i = 0; i < cnt; i++) {
    printdev(devs[i]);
  }

  // these are vendorID and productID I found for my usb device
  dev_handle = libusb_open_device_with_vid_pid(ctx, TARGET_VENDOR_ID, TARGET_PRODUCT_ID);

  if(dev_handle == NULL)
    cout<<"Cannot open device"<<endl;
  else
    cout<<"Device Opened"<<endl;

  // free the list, unref the devices in it
  libusb_free_device_list(devs, 1);

  // find out if kernel driver is attached
  if(libusb_kernel_driver_active(dev_handle, TARGET_INTERFACE_NUM) == 1) {
    cout<<"Kernel Driver Active"<<endl;

    // detach it
    if(libusb_detach_kernel_driver(dev_handle, TARGET_INTERFACE_NUM) == 0)
      cout<<"Kernel Driver Detached!"<<endl;
  }

  // claim interface of device
  r = libusb_claim_interface(dev_handle, TARGET_INTERFACE_NUM);

  if(r < 0) {
    cout<<"Cannot Claim Interface"<<endl;
    return 1;
  }
  cout<<"Claimed Interface"<<endl;

  // used to find out how many bytes were written
  int actual;
  // data to write
  unsigned char *data = new unsigned char[4];
  // some dummy values
  data[0]='a';data[1]='b';data[2]='c';data[3]='a';

  // just to see the data we want to write : abcd
  cout<<"Data->"<<data<<"<-"<<endl;
  cout<<"Writing Data..."<<endl;

  // transfer data for target end point
  r = libusb_bulk_transfer(dev_handle, (TARGET_ENDPOINT_ADDRESS | LIBUSB_ENDPOINT_OUT), data, 4, &actual, 100);

  // we wrote the 4 bytes successfully
  if(r == 0 && actual == 4)
    cout<<"Writing Successful!"<<endl;
  else
    cout<<"Write Error"<<endl;

  // release the claimed interface
  r = libusb_release_interface(dev_handle, TARGET_INTERFACE_NUM);
  if(r!=0) {
    cout<<"Cannot Release Interface"<<endl;
    return 1;
  }
  cout<<"Released Interface"<<endl;

  // close the device we opened
  libusb_close(dev_handle);
  // needs to be called to end the
  libusb_exit(ctx);

  // delete the allocated memory for data
  delete[] data;
  return 0;
}

void printdev(libusb_device *dev) {
  libusb_device_descriptor desc;
  int r = libusb_get_device_descriptor(dev, &desc);
  if (r < 0) {
    cout<<"failed to get device descriptor"<<endl;
    return;
  }
  cout<<"Number of possible configurations: "<<(int)desc.bNumConfigurations<<endl;
  cout<<"Device Class: "<<(int)desc.bDeviceClass<<endl;
  cout<<"VendorID: "<<desc.idVendor<<endl;
  cout<<"ProductID: "<<desc.idProduct<<endl;
  cout<<"Device speed: "<<libusb_get_device_speed(dev)<<endl;
  libusb_config_descriptor *config;
  libusb_get_config_descriptor(dev, 0, &config);
  cout<<"Interfaces: "<<(int)config->bNumInterfaces<<endl;
  const libusb_interface *inter;
  const libusb_interface_descriptor *interdesc;
  const libusb_endpoint_descriptor *epdesc;
  for(int i=0; i<(int)config->bNumInterfaces; i++) {
    inter = &config->interface[i];
    cout<<"Number of alternate settings: "<<inter->num_altsetting<<" | ";
    for(int j=0; j<inter->num_altsetting; j++) {
      interdesc = &inter->altsetting[j];
      cout<<"Interface Number: "<<(int)interdesc->bInterfaceNumber<<" | ";
      cout<<"Number of endpoints: "<<(int)interdesc->bNumEndpoints<<" | ";
      for(int k=0; k<(int)interdesc->bNumEndpoints; k++) {
        epdesc = &interdesc->endpoint[k];
        cout<<"Descriptor Type: "<<(int)epdesc->bDescriptorType<<" | ";
        cout<<"EP Address: "<<(int)epdesc->bEndpointAddress<<" | ";
      }
      cout<<endl;
    }
  }
  cout<<endl<<endl<<endl;
  libusb_free_config_descriptor(config);
}
