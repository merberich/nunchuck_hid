# NunchuckHid

NunchuckHid: Use two Nintendo WII Nunchucks as a singular USB HID device.

`@todo`

## Getting Started

`@todo`

## Installing

`@todo`
- Testato's Arduino SoftwareWire library v1.51+ must be installed
- Copy directory `UsbGamepad` to Arduino libraries directory.

## Versioning

This project follows the [Semantic Versioning Guidelines](https://semver.org/). Current version of the project is `0.0.1`.

## License

All project materials are licensed under the GPL v3 unless otherwise noted. See [LICENSE.txt](LICENSE.txt) for details.

## Contributing

At this point in time contributions are closed. Open an issue to request changes or make suggestions (noting active tasks in the backlog below), or fork the repository for your own modifications.

## Backlog

- A PCB implementing the schematic
- Remainder of `@todo`'s in this document
- Better break out header for USB information (for interpreting on workstation)
- Modify UsbGamepad (and rename appropriately) to act as a new generic device (not a gamepad).
  - Treat accelerometers as joysticks as well? Usages for generic sensors?
  - Investigate HID composite device report descriptors to separate joysticks
    - Alternatively, separate internal physical collections to maintain single device setup
    - See:
      - [HID spec](https://usb.org.10-1-108-210.causewaynow.com/sites/default/files/documents/hid1_11.pdf)
      - [HID Usage tables](https://usb.org.10-1-108-210.causewaynow.com/sites/default/files/documents/hut1_12v2.pdf)
      - [HID Report Descriptor tutorials](https://eleccelerator.com/tutorial-about-usb-hid-report-descriptors/)
  - Look into adding Feature option for command + control of optional modes
    - Calibration
    - Reporting of device status (errors?)
- [libusb/hidapi](https://github.com/libusb/hidapi) driver for this device
  - Create subdirectories for Arduino-specific code, and workstation-specific code.
  - Workstation driver should be as cross-platform as possible.
  - Workstation build system?
- Apply for a proper PID for this design on [pid.codes](http://pid.codes/)

## Acknowledgements

Inspired by [Wiichuck](https://github.com/madhephaestus/WiiChuck) library.
Uses [v-usb](https://www.obdev.at/products/vusb/index.html) via [v-usb-for-arduino](https://github.com/gloob/vusb-for-arduino).
Uses [SoftwareWire](https://github.com/Testato/SoftwareWire).
