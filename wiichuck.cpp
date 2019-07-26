
#include <Arduino.h>
#include <SoftwareWire.h>
#include "wiichuck.h"

WiiNunchuck::ReturnCode WiiNunchuck::begin(SoftwareWire* i2c) {
  if (i2c == nullptr || is_begun_) {
    end();
    return ReturnCode::RET_BAD_ARG;
  }
  i2c_ = i2c;

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
    if (debug_prints_) {
      Serial.println("Failed to read target type. Re-initialize.");
    }
    return ReturnCode::RET_FAILED_XFER;
  }
  if (memcmp(type_buf, CHUCK_ID, CHUCK_PACKET_SIZE_BYTES)) {
    if (debug_prints_) {
      Serial.println("ID of target not recognized: ");
      for (uint8_t i = 0u; i < CHUCK_PACKET_SIZE_BYTES; i++) {
        Serial.print("0x");
        Serial.print(type_buf[i], HEX);
        Serial.print(",");
      }
      Serial.println();
      Serial.println("Re-initialize.");
    }
    return ReturnCode::RET_WRONG_ID;
  }
  else if (debug_prints_) {
    Serial.print("Nunchuck identified.");
  }

	// Set default calibration
	calib_.joy_x = calib_.joy_y = 128;
	calib_.accel_x = calib_.accel_y = calib_.accel_z = 125; // accel and lsb together == 500.
	calib_.lsb_x = calib_.lsb_y = calib_.lsb_z = 0;

  is_begun_ = true;

  return ReturnCode::RET_SUCCESS;
}

void WiiNunchuck::end() {
  is_begun_ = false;
  i2c_ = nullptr;
}

void WiiNunchuck::debugPrints(bool enable) {
  debug_prints_ = enable;
}

WiiNunchuck::ReturnCode WiiNunchuck::poll() {
  if (!is_begun_) {
    return ReturnCode::RET_NOT_BEGUN;
  }

  // Send conversion command
  i2c_->beginTransmission(CHUCK_ADDR);
  i2c_->write(CHUCK_DATA_READ_CMD);
  uint8_t res = i2c_->endTransmission();

  if (res != SOFTWAREWIRE_NO_ERROR) {
    if (debug_prints_) {
      Serial.print("Read command failure: ");
      Serial.println(res);
    }
    return ReturnCode::RET_FAILED_XFER;
  }

  delayMicroseconds(CHUCK_DELAY_US);

  uint8_t n = i2c_->requestFrom(CHUCK_ADDR, CHUCK_PACKET_SIZE_BYTES);
  uint8_t bytes = 0;
  while(i2c_->available() && bytes < CHUCK_PACKET_SIZE_BYTES) {
    data.buffer[bytes++] = i2c_->read();
  }

  if (n != CHUCK_PACKET_SIZE_BYTES || bytes != CHUCK_PACKET_SIZE_BYTES) {
    if (debug_prints_) {
      Serial.print("Read buffer failure, got: ");
      Serial.print(n);
      Serial.print(", read ");
      Serial.println(bytes);
    }
    return ReturnCode::RET_FAILED_XFER;
  }

  return ReturnCode::RET_SUCCESS;
}

void WiiNunchuck::calibrate() {
	calib_.joy_x = data.parsed.joy_x;
	calib_.joy_y = data.parsed.joy_y;
	calib_.accel_x = data.parsed.accel_x;
	calib_.accel_y = data.parsed.accel_y;
	calib_.accel_z = data.parsed.accel_z;
	calib_.lsb_x = data.parsed.lsb_x;
	calib_.lsb_y = data.parsed.lsb_y;
	calib_.lsb_z = data.parsed.lsb_z;
}
