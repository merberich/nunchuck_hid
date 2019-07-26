//! WiiNunchuck
//! Poll data from a Wii Nunchuck via software I2C emulation.
//!
//! Michael Erberich

#ifndef WIICHUCK_H_
#define WIICHUCK_H_

#include <inttypes.h>
#include <SoftwareWire.h>

//! This driver supports I/O with a Wii Nunchuck over software-driven I2C on Arduino.
//! See http://wiibrew.org/wiki/Wiimote/Extension_Controllers
//! and http://wiibrew.org/wiki/Wiimote/Extension_Controllers/Nunchuck
//! for implementation details.


//! Packed data structure to pull from raw Wii Nunchuck data register (6 bytes).
//! See cracked Nunchuck documentation for format specifics.
typedef struct {
	uint8_t joy_x;
  uint8_t joy_y;
  uint8_t accel_x;
  uint8_t accel_y;
  uint8_t accel_z;
  uint8_t buttonZ : 1;
  uint8_t buttonC : 1;
  uint8_t lsb_x : 2;
  uint8_t lsb_y : 2;
  uint8_t lsb_z : 2;
} WiiNunchuckData;

class WiiNunchuck {
  public:
    //! Delay between sequential Nunchuck register IO operations, in [us]. Empirically determined.
    static const uint8_t CHUCK_DELAY_US = 200u;

    //! I2C slave address of the Nunchuck.
    static const uint8_t CHUCK_ADDR = 0x52;
    //! Nunchuck read register length, in [bytes].
    static const uint8_t CHUCK_PACKET_SIZE_BYTES = 6u;
    //! Nunchuck Wii Remote extension controller ID.
    const uint8_t CHUCK_ID[CHUCK_PACKET_SIZE_BYTES] = { 0x00, 0x00, 0xA4, 0x20, 0x00, 0x00 };

    //! Initialization command register address.
    static const uint8_t CHUCK_INIT_REG = 0xF0;
    //! Initialization command for readback without encryption.
    static const uint8_t CHUCK_INIT_CMD = 0x55;

    //! Identification command register address.
    static const uint8_t CHUCK_TYPE_WRITE_REG = 0xFB;
    //! Identification command: request next read as ID register (6 bytes).
    static const uint8_t CHUCK_TYPE_WRITE_CMD = 0x00;

    //! Read command regster address.
    static const uint8_t CHUCK_TYPE_READ_REG = 0xFA;
    //! Read command: request that next I2C read will contain data register contents (6 bytes).
    static const uint8_t CHUCK_DATA_READ_CMD = 0x00;

    //! Possible return statuses for driver operations.
    enum ReturnCode : uint8_t {
      RET_SUCCESS = 0x00,      //!< Operation success.
      RET_BAD_ARG = 0x01,      //!< Malformatted or improper parameter argument given.
      RET_FAILED_XFER = 0x02,  //!< Underlying I2C op failed or did not transfer correct size.
      RET_WRONG_ID = 0x03,     //!< Device ID reported was not a Nunchuck.
      RET_NOT_BEGUN = 0x04,    //!< Driver has not been initialized.
      RET_MAX
    };

    //! Initialize this driver, initialize the attached Wii Nunchuck, and verify ID.
    //! Will re-initialize if already initialized, and close if @p i2c is invalid.
    //! @param i2c Pointer to initialized software I2C master driver. Nullptr invalid.
    //! @return Return code indicating operation result.
    ReturnCode begin(SoftwareWire* i2c);

    //! Close out this driver.
    void end();

    //! Configure whether or not debug statements should appear on Serial.
    //! Not recommended if Serial is in critical use elsewhere. Disabled by default.
    //! @param enabled Should debug prints be enabled for this driver.
    void debugPrints(bool enabled);

    //! Request most recent information from the Nunchuck.
    //! @return Return code indicating operation result.
    ReturnCode poll();

    //! Sets current readings for the joystick and accelerometer as the referencezero values.
    //! These offsets will be applied to the raw data in any read operation from this API.
    void calibrate();

    //! Fetch last polled X-axis joystick value.
    //! @return Value in approximately a range of (-100,100) depending on calibration.
    inline int joyX() {
      return (int)data.parsed.joy_x - (int)calib_.joy_x;
    }

    //! Fetch last polled Y-axis joystick value.
    //! @return Value in approximately a range of (-100,100) depending on calibration.
    inline int joyY() {
      return (int)data.parsed.joy_y - (int)calib_.joy_y;
    }

    //! Fetch last polled X-axis accelerometer value.
    inline int accelX() {
      return (int)((data.parsed.accel_x << 2) | data.parsed.lsb_x)
          - (int)((calib_.accel_x << 2) | calib_.lsb_x);
    }

    //! Fetch last polled Y-axis accelerometer value.
    inline int accelY() {
      return (int)((data.parsed.accel_y << 2) | data.parsed.lsb_y)
          - (int)((calib_.accel_y << 2) | calib_.lsb_y);
    }

    //! Fetch last polled Z-axis accelerometer value.
    inline int accelZ() {
      return (int)((data.parsed.accel_z << 2) | data.parsed.lsb_z)
          - (int)((calib_.accel_z << 2) | calib_.lsb_z);
    }

    //! Fetch last polled Z-button (trigger) state.
    inline uint8_t buttonZ() {
      return !data.parsed.buttonZ;
    }

    //! Fetch last polled C-button (bumper) state.
    inline uint8_t buttonC() {
      return !data.parsed.buttonC;
    }

  private:
    //! Raw data from last read, accessible as uint8_t array or struct format.
    union {
      uint8_t buffer[CHUCK_PACKET_SIZE_BYTES];
      WiiNunchuckData parsed;
    } data;

    //! Calibrated reference/zero values for current nunchuck.
    WiiNunchuckData calib_ = {};

    //! Has this driver been initialized?
    bool is_begun_ = false;

    //! Are debug prints enabled?
    bool debug_prints_ = false;

    //! This driver's reference to underlying I2C master driver.
    //! Since each Wii Nunchuck / extension controller has the same I2C address, in order to
    //! read from more than one from a single Arduino without using an additional hardware
    //! multiplexer, it is necessary to separate each instance onto its own I2C bus. Since most
    //! Arduino ICs only have one hardware I2C peripheral, this is done with software emulation.
    SoftwareWire* i2c_ = nullptr;
};

#endif
