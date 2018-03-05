/*
 * @file UsbGamepad.h
 *
 * Interface for custom HID dual wii nunchuck gamepad. Defines report descriptor and associated
 *   structure for data type. Provides hook definitions necessary to use V-USB.
 * M. Erberich
 */
#ifndef USB_GAMEPAD_H_
#define USB_GAMEPAD_H_

#include <string.h>

#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>  /* for sei() */
#include <util/delay.h>     /* for _delay_ms() */

#include <avr/pgmspace.h>   /* required by usbdrv.h */

#ifdef __cplusplus
extern "C"
{
#endif

#include "usbconfig.h"
#include "utility/usbdrv.h"

#ifdef __cplusplus
}
#endif

/**
* @brief USB HID report descriptor.
*
* Custom interface based on available features of dual Wii nunchuck-based controller. Each
*   controller has:
*   - XYZ accelerometer
*   - C-button
*   - Z-button
*   - XY joystick
* This data is formatted as the following structure
*   uint8 [0: c0butC, 1: c0butZ, 2: c1butC, 3: c1butZ]:
*   uint8 [7-0: c0joyX]
*   uint8 [7-0: c0joyY]
*   uint8 [7-0: c1joyX]
*   uint8 [7-0: c1joyY]
*   int8 [7-0: c0accX]
*   int8 [7-0: c0accY]
*   int8 [7-0: c0accZ]
*   int8 [7-0: c1accX]
*   int8 [7-0: c1accY]
*   int8 [7-0: c1accZ]
*/
PROGMEM const char usbHidReportDescriptor[USB_CFG_HID_REPORT_DESCRIPTOR_LENGTH] = {
  0x05, 0x01,        // USAGE_PAGE (GENERIC DESKTOP)
  0x09, 0x05,        // USAGE (GAME PAD)
  0xA1, 0x01,        // COLLECTION (APPLICATION)
  0xA1, 0x00,        //   COLLECTION (PHYSICAL)
  0x05, 0x09,        //     USAGE_PAGE (BUTTON)
  0x19, 0x01,        //     USAGE_MINIMUM (1)
  0x29, 0x08,        //     USAGE_MAXIMUM (8)
  0x15, 0x00,        //     LOGICAL_MINIMUM (0)
  0x25, 0x01,        //     LOGICAL_MAXIMUM (1)
  0x75, 0x01,        //     REPORT_SIZE (1)
  0x95, 0x08,        //     REPORT_COUNT (8)
  0x81, 0x02,        //     INPUT (DATA, VAR, ABS)  -- Buttons
  0x05, 0x01,        //     USAGE_PAGE (GENERIC DESKTOP)
  0x09, 0x30,        //     USAGE (X)
  0x09, 0x31,        //     USAGE (Y)
  0x09, 0x32,        //     USAGE (Z)
  0x09, 0x35,        //     USAGE (Rz)
  0x15, 0x81,        //     LOGICAL_MINIMUM (0)
  0x25, 0x7F,        //     LOGICAL_MAXIMUM (255)
  0x75, 0x08,        //     REPORT_SIZE (8)
  0x95, 0x04,        //     REPORT_COUNT (4)
  0x81, 0x02,        //     INPUT (DATA, VAR, ABS)  -- Chuck - Joy XY, Chuck 1 joy XY
  0x09, 0x40,        //     USAGE (Vx)
  0x09, 0x41,        //     USAGE (Vy)
  0x09, 0x42,        //     USAGE (Vz)
  0x09, 0x43,        //     USAGE (Vbrx)
  0x09, 0x44,        //     USAGE (Vbry)
  0x09, 0x45,        //     USAGE (Vbrz)
  0x75, 0x08,        //     REPORT_SIZE (8)
  0x95, 0x06,        //     REPORT_COUNT (6)
  0x81, 0x02,        //     INPUT (DATA, VAR, ABS)
  0xC0,              //   END_COLLECTION
  0xC0               // END_COLLECTION
};

struct __attribute__ ((packed)) UsbGamepadReport_t {
  uint8_t button_mask;
  uint8_t c0_joy_x;
  uint8_t c0_joy_y;
  uint8_t c1_joy_x;
  uint8_t c1_joy_y;
  int8_t c0_acc_x;
  int8_t c0_acc_y;
  int8_t c0_acc_z;
  int8_t c1_acc_x;
  int8_t c1_acc_y;
  int8_t c1_acc_z;
};

class UsbGamepadDevice {
 public:
  UsbGamepadDevice() {}

  void begin() {
    // Watchdog
    wdt_enable(WDTO_1S);

    // Set up AVR pins for USB I/O
    PORTD &= ~USBMASK;
    DDRD |= ~USBMASK;

    usbInit();
    cli();
    usbDeviceDisconnect();
    // Fake USB disconnect for 10ms
    uint8_t i = 11u;
    while (--i) {
      wdt_reset();
      delay(1);
    }
    usbDeviceConnect();
    sei();
  }

  void update(UsbGamepadReport_t* report_buf) {
    unsigned char* buf = reinterpret_cast<unsigned char*>(report_buf);
    uint8_t buf_size = sizeof(*report_buf);
    uint8_t len = 0u;
    for (uint8_t i = 0u; i < buf_size; i += 8u) {
      wdt_reset();
      while (!usbInterruptIsReady()) {
        usbPoll();
      }
      // Fill the interrupt buffer after every poll
      len = buf_size - i > 8u ? 8u : buf_size - i;
      usbSetInterrupt(buf + i, len);
    }
  }
};

UsbGamepadDevice UsbGamepad = UsbGamepadDevice();

#endif  // USB_GAMEPAD_H_
