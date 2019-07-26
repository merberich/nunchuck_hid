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

`@todo`

## License

All project materials are licensed under the GPL v3 unless otherwise noted. See [LICENSE.txt](LICENSE.txt) for details.

## Contributing

At this point in time contributions are closed. Open an issue to request changes or make suggestions (noting active tasks in the backlog below), or fork the repository for your own modifications.

## Backlog

- Provide electrical schematic for example hardware
- Start versioning stuff
- Better break out header for USB information (for interpreting on workstation)
- Modify UsbGamepad (and rename appropriately) to either:
  (a) comply with generic gamepad field requirements in Steam, and not actually provide all of the possible UI elements
  or
  (b) not try to be a gamepad, and rather be a generic (but unique) HID device
- Libusb driver for this device, generic gamepad or not.
  - Create subdirectories for Arduino-specific code, and workstation-specific code.
  - Workstation driver should be as cross-platform as possible.
  - Workstation build system?

## Acknowledgements

Inspired by [Wiichuck](https://github.com/madhephaestus/WiiChuck) library.
Uses [v-usb](https://www.obdev.at/products/vusb/index.html) via [v-usb-for-arduino](https://github.com/gloob/vusb-for-arduino).
Uses [SoftwareWire](https://github.com/Testato/SoftwareWire).
