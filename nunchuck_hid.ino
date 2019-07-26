/*
* @file nunchuck_hid.ino
*
* Exploratory project to expose data from a pair of Nintendo Wii Nunchucks over the generic USB
*   HID device class as a single gamepad. As such, this data should be visible to any computer
*   supporting the USB HID specification. In theory, this can be used as a subset of a proper game
*   controller.
* M. Erberich
*/

#include <Arduino.h>

#include <UsbGamepad.h>
#include <SoftwareWire.h>
#include "wiichuck.h"

const uint8_t I2C_MASTER0_SCL = 5u;  // Left chuck SCL
const uint8_t I2C_MASTER0_SDA = 6u;  // Left chuck SDA
const uint8_t I2C_MASTER1_SCL = 7u;  // Right chuck SCL
const uint8_t I2C_MASTER1_SDA = 8u;  // Right chuck SDA

const uint32_t SERIAL_BAUD = 250000u;  // Arduino Serial baud rate

SoftwareWire i2c_master0((uint8_t)I2C_MASTER0_SDA, (uint8_t)I2C_MASTER0_SCL);
SoftwareWire i2c_master1((uint8_t)I2C_MASTER1_SDA, (uint8_t)I2C_MASTER1_SCL);

WiiNunchuck left_chuck;   // Connected to software I2C master bus 0
WiiNunchuck right_chuck;  // Connected to software I2C master bus 1

bool has_left_init = false;
bool has_right_init = false;

bool has_left_calib = false;
bool has_right_calib = false;

UsbGamepadReport_t report;  // The buffer in which USB reports will be written
uint8_t idle_rate;

// Function to support enumeration via V-USB
usbMsgLen_t usbFunctionSetup(uint8_t data[8]) {
  usbRequest_t *rq = (usbRequest_t *)((void *)data);

  usbMsgPtr = reinterpret_cast<unsigned char*>(&report);
  if((rq->bmRequestType & USBRQ_TYPE_MASK) == USBRQ_TYPE_CLASS){
    // class request type
    if(rq->bRequest == USBRQ_HID_GET_REPORT){
      // wValue: ReportType (highbyte), ReportID (lowbyte)
      // we only have one report type, so don't look at wValue
      packData(&report);
      return sizeof(report);
    }
    else if (rq->bRequest == USBRQ_HID_GET_IDLE) {
      usbMsgPtr = &idle_rate;
      return 1;
    }
    else if (rq->bRequest == USBRQ_HID_SET_IDLE) {
      idle_rate = rq->wValue.bytes[1];
    }
  }
  else {
    // no vendor specific requests implemented
  }
  return 0;
}

// Pack the USB report structure with latest control data
void packData(UsbGamepadReport_t* report_buf) {
  report_buf->button_mask =
      (left_chuck.buttonC() << 0)
      | (left_chuck.buttonZ() << 1)
      | (right_chuck.buttonC() << 2)
      | (right_chuck.buttonZ() << 3);

  report_buf->c0_joy_x = (int8_t)left_chuck.joyX();
  report_buf->c0_joy_y = -(int8_t)left_chuck.joyY();
  report_buf->c1_joy_x = (int8_t)right_chuck.joyX();
  report_buf->c1_joy_y = -(int8_t)right_chuck.joyY();

  report_buf->c0_acc_x = (int8_t)(left_chuck.accelX() >> 2);
  report_buf->c0_acc_y = (int8_t)(left_chuck.accelY() >> 2);
  report_buf->c0_acc_z = (int8_t)(left_chuck.accelZ() >> 2);
  report_buf->c1_acc_x = (int8_t)(right_chuck.accelX() >> 2);
  report_buf->c1_acc_y = (int8_t)(right_chuck.accelY() >> 2);
  report_buf->c1_acc_z = (int8_t)(right_chuck.accelZ() >> 2);
}

void setup() {
  // Set up Serial for debug printing
  Serial.begin(SERIAL_BAUD);

  // Enable SoftwareWire on both virtual master buses
  i2c_master0.begin();
  i2c_master1.begin();

  // Attempt to set up the nunchucks
  if (left_chuck.begin(&i2c_master0) == WiiNunchuck::ReturnCode::RET_SUCCESS) {
    if (left_chuck.poll() == WiiNunchuck::ReturnCode::RET_SUCCESS) {
      left_chuck.calibrate();
    }
    has_left_init = true;
  }
  if (right_chuck.begin(&i2c_master1) == WiiNunchuck::ReturnCode::RET_SUCCESS) {
    if (right_chuck.poll() == WiiNunchuck::ReturnCode::RET_SUCCESS) {
      right_chuck.calibrate();
    }
    has_right_init = true;
  }

  if (!has_left_init || !has_right_init) {
    Serial.println("Unable to begin. Not all nunchucks indexed.");
    while(1) {}  // Can't exactly report good data without both chucks active
  }
  Serial.println("Setup successful!");

  // Set up V-USB interface
  UsbGamepad.begin();
}

void loop() {
  if (has_left_init) {
    left_chuck.poll();
  }
  if (has_right_init) {
    right_chuck.poll();
  }
  packData(&report);
  UsbGamepad.update(&report);
}
