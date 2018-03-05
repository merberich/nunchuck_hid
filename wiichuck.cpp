
#include <Arduino.h>
#include <SoftwareWire.h>
#include "wiichuck.h"

bool Wiichuck::init(uint8_t sda_pin, uint8_t scl_pin) {
  // Initialize the software I2C object
  i2c.init(sda_pin, scl_pin, true, true);
  i2c.begin();

  // Initialize the attached nunchuck
  i2c.beginTransmission(CHUCK_ADDR);
  i2c.write(CHUCK_INIT_REG);
  i2c.write(CHUCK_INIT_CMD);
  i2c.endTransmission();

  delayMicroseconds(CHUCK_DELAY_US);

  i2c.beginTransmission(CHUCK_ADDR);
  i2c.write(CHUCK_TYPE_WRITE_REG);
  i2c.write(CHUCK_TYPE_WRITE_CMD);
  i2c.endTransmission();

  delayMicroseconds(CHUCK_DELAY_US);

  // Confirm the device ID of the nunchuck
  i2c.beginTransmission(CHUCK_ADDR);
  i2c.write(CHUCK_TYPE_READ_REG);
  i2c.endTransmission();

  delayMicroseconds(CHUCK_DELAY_US);

  uint8_t n = i2c.requestFrom(CHUCK_ADDR, CHUCK_PACKET_SIZE_BYTES);
  uint8_t bytes = 0;
  uint8_t type_buf[CHUCK_PACKET_SIZE_BYTES];
  while(i2c.available() && bytes < CHUCK_PACKET_SIZE_BYTES) {
    type_buf[bytes++] = i2c.read();
  }
  if (n != CHUCK_PACKET_SIZE_BYTES || bytes != CHUCK_PACKET_SIZE_BYTES) {
    Serial.println("Failed to read target type. Re-initialize.");
    return false;
  }
  if (memcmp(type_buf, CHUCK_ID, CHUCK_PACKET_SIZE_BYTES)) {
    Serial.println("ID of target not recognized: ");
    for (uint8_t i = 0u; i < CHUCK_PACKET_SIZE_BYTES; i++) {
      Serial.print("0x");
      Serial.print(type_buf[i], HEX);
      Serial.print(",");
    }
    Serial.println();
    Serial.println("Re-initialize.");
    return false;
  }
  else {
    Serial.print("Nunchuck identified.");
  }

	// Set default calibration
	calib.joyX = calib.joyY = 128;
	calib.accelX = calib.accelY = calib.accelZ = 125; // accel and lsb together == 500.
	calib.lsbX = calib.lsbY = calib.lsbZ = 0;

  return true;
}

uint8_t Wiichuck::poll() {
  // Send conversion command
  i2c.beginTransmission(CHUCK_ADDR);
  i2c.write(CHUCK_DATA_READ_CMD);
  uint8_t res = i2c.endTransmission();

  if (res != SOFTWAREWIRE_NO_ERROR) {
    Serial.print("Read command failure: ");
    Serial.println(res);
    return false;
  }

  delayMicroseconds(CHUCK_DELAY_US);

  uint8_t n = i2c.requestFrom(CHUCK_ADDR, CHUCK_PACKET_SIZE_BYTES);
  uint8_t bytes = 0;
  while(i2c.available() && bytes < CHUCK_PACKET_SIZE_BYTES) {
    data.buffer[bytes++] = i2c.read();
  }

  if (n != CHUCK_PACKET_SIZE_BYTES || bytes != CHUCK_PACKET_SIZE_BYTES) {
    Serial.print("Read buffer failure, got: ");
    Serial.print(n);
    Serial.print(", read ");
    Serial.println(bytes);
    return false;
  }

  return true;
}

void Wiichuck::calibrate() {
	calib.joyX = data.parsed.joyX;
	calib.joyY = data.parsed.joyY;
	calib.accelX = data.parsed.accelX;
	calib.accelY = data.parsed.accelY;
	calib.accelZ = data.parsed.accelZ;
	calib.lsbX = data.parsed.lsbX;
	calib.lsbY = data.parsed.lsbY;
	calib.lsbZ = data.parsed.lsbZ;
}
