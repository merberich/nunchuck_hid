/*
 * Wiichuck library
 * Poll data from a Wii Nunchuck via software I2C
 *
 * Heavily influenced by jnw.walker@gmail.com Wiichuck library.
 * Extended to use SoftwareWire by Testato, provides packet definitions.
 *
 * Michael Erberich
 */

#ifndef WIICHUCK_H_
#define WIICHUCK_H_

#include <inttypes.h>
#include <SoftwareWire.h>

typedef struct {
	uint8_t joyX;
  uint8_t joyY;
  uint8_t accelX;
  uint8_t accelY;
  uint8_t accelZ;
  uint8_t buttonZ : 1;
  uint8_t buttonC : 1;
  uint8_t lsbX : 2;
  uint8_t lsbY : 2;
  uint8_t lsbZ : 2;
} WiichuckData;

class Wiichuck {
  public:
    static const uint8_t CHUCK_DELAY_US = 200u;

    static const uint8_t CHUCK_ADDR = 0x52;  // The seven-bit address (shifted in lib)
    static const uint8_t CHUCK_PACKET_SIZE_BYTES = 6u;
    const uint8_t CHUCK_ID[CHUCK_PACKET_SIZE_BYTES] = { 0x00, 0x00, 0xA4, 0x20, 0x00, 0x00 };

    static const uint8_t DEFAULT_SCL_PIN = 2u;
    static const uint8_t DEFAULT_SDA_PIN = 3u;

    // Using the knockoff initialization commands that do not require decoding
    static const uint8_t CHUCK_INIT_REG = 0xF0;
    static const uint8_t CHUCK_INIT_CMD = 0x55;

    static const uint8_t CHUCK_TYPE_READ_REG = 0xFA;

    static const uint8_t CHUCK_TYPE_WRITE_REG = 0xFB;
    static const uint8_t CHUCK_TYPE_WRITE_CMD = 0x00;

    static const uint8_t CHUCK_DATA_READ_CMD = 0x00;

    uint8_t buffer[CHUCK_PACKET_SIZE_BYTES];

    // Select software I2C pins to drive (pins 2 and 3 by default)
    bool init(uint8_t sda_pin = 3u, uint8_t scl_pin = 2u);

    // Requests data from the nunchuck
    uint8_t poll();

    // Sets the datum values to current readings.
    void calibrate();

    inline int joyX() {
      return (int)data.parsed.joyX - (int)calib.joyX;
    }
    inline int joyY() {
      return (int)data.parsed.joyY - (int)calib.joyY;
    }

    inline int accelX() {
      return (int)((data.parsed.accelX << 2) | data.parsed.lsbX)
          - (int)((calib.accelX << 2) | calib.lsbX);
    }
    inline int accelY() {
      return (int)((data.parsed.accelY << 2) | data.parsed.lsbY)
          - (int)((calib.accelY << 2) | calib.lsbY);
    }
    inline int accelZ() {
      return (int)((data.parsed.accelZ << 2) | data.parsed.lsbZ)
          - (int)((calib.accelZ << 2) | calib.lsbZ);
    }

    inline uint8_t buttonZ() {
      return !data.parsed.buttonZ;
    }
    inline uint8_t buttonC() {
      return !data.parsed.buttonC;
    }

  private:
    // Retrieved data from last read, accessible as uint8_t array or struct format
    union {
      uint8_t buffer[CHUCK_PACKET_SIZE_BYTES];
      WiichuckData parsed;
    } data;

    // Decode nunchuck byte
    //static inline uint8_t decode(uint8_t b) { return (b ^ 0x17) + 0x17; }

    // Calibration data for this wiichuck obj
    WiichuckData calib;

    // Each wiichuck obj should operate its own bus (all chucks have same I2C addresses)
    SoftwareWire i2c;
};

#endif
