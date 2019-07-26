
#include <Arduino.h>
#include <SoftwareWire.h>
#include "wiichuck.h"

bool Wiichuck::begin(SoftwareWire* i2c) {
  i2c_ = i2c;
  if (i2c == nullptr) {
    is_begun_ = false;
    return false;
  }

  // Initialize the attached nunchuck
  i2c_->beginTransmission(CHUCK_ADDR);
  i2c_->write(CHUCK_INIT_REG);
  i2c_->write(CHUCK_INIT_CMD);
  i2c_->endTransmission();

  delayMicroseconds(CHUCK_DELAY_US);

  i2c_->beginTransmission(CHUCK_ADDR);
  i2c_->write(CHUCK_TYPE_WRITE_REG);
  i2c_->write(CHUCK_TYPE_WRITE_CMD);
  i2c_->endTransmission();

  delayMicroseconds(CHUCK_DELAY_US);

  // Confirm the device ID of the nunchuck
  i2c_->beginTransmission(CHUCK_ADDR);
  i2c_->write(CHUCK_TYPE_READ_REG);
  i2c_->endTransmission();

  delayMicroseconds(CHUCK_DELAY_US);

  uint8_t n = i2c_->requestFrom(CHUCK_ADDR, CHUCK_PACKET_SIZE_BYTES);
  uint8_t bytes = 0;
  uint8_t type_buf[CHUCK_PACKET_SIZE_BYTES];
  while(i2c_->available() && bytes < CHUCK_PACKET_SIZE_BYTES) {
    type_buf[bytes++] = i2c_->read();
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

  is_begun_ = true;

  return true;
}

void Wiichuck::end() {
  is_begun_ = false;
  i2c_ = nullptr;
}

uint8_t Wiichuck::poll() {
  if (! is_begun_) {
    return false;
  }

  // Send conversion command
  i2c_->beginTransmission(CHUCK_ADDR);
  i2c_->write(CHUCK_DATA_READ_CMD);
  uint8_t res = i2c_->endTransmission();

  if (res != SOFTWAREWIRE_NO_ERROR) {
    Serial.print("Read command failure: ");
    Serial.println(res);
    return false;
  }

  delayMicroseconds(CHUCK_DELAY_US);

  uint8_t n = i2c_->requestFrom(CHUCK_ADDR, CHUCK_PACKET_SIZE_BYTES);
  uint8_t bytes = 0;
  while(i2c_->available() && bytes < CHUCK_PACKET_SIZE_BYTES) {
    data.buffer[bytes++] = i2c_->read();
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
