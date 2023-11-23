# Remote Controller

### A literal super universal remote controller, you can manipulate TVS, AIR CONDITIONERS, DECODERS and MORE.

The purpose of this project is to have in one room, one infrared remote for everything.

## Contents

  -[How it works?](#how-it-works?)
  -[Development Status](#development-status)
  -[Hardware](#hardware)
  -[Dependencies](#dependencies)

### How it works? (Software)

The structure of the stored Infrared signals is:

Example:

* Profile: LG TV
* Subprofile: Turn on

This kind of profiles are stored on the SD, they are separated in diferent files.

Example: LG_TV.db

This files have the binary of infrared signals with the name of their subprofiles.

### Development Status

Latest Stable Release  [![Release Version](https://img.shields.io/github/release/G4BY0/Control-Remoto.svg?style=plastic)](https://github.com/G4BY0/Control-Remoto/releases/latest/) [![Release Date](https://img.shields.io/github/release-date/G4BY0/Control-Remoto.svg?style=plastic)](https://github.com/G4BY0/Control-Remoto/releases/latest/) [![Downloads](https://img.shields.io/github/downloads/G4BY0/Control-Remoto/latest/total.svg?style=plastic)](https://github.com/G4BY0/Control-Remoto/releases/latest/)

Latest Development Release  [![Release Version](https://img.shields.io/github/release/G4BY0/Control-Remoto/all.svg?style=plastic)](https://github.com/G4BY0/Control-Remoto/releases/) [![Release Date](https://img.shields.io/github/release-date-pre/G4BY0/Control-Remoto.svg?style=plastic)](https://github.com/G4BY0/Control-Remoto/releases/) [![Downloads](https://img.shields.io/github/downloads-pre/G4BY0/Control-Remoto/latest/total.svg?style=plastic)](https://github.com/G4BY0/Control-Remoto/releases/)

### Hardware

* [PCB](https://github.com/G4BY0/Control-Remoto/tree/main/hardware)
* Display Adafruit SH110X
* Six Botons
* [Nodemcu Esp32-S (4MB Flash Memory)](https://docs.ai-thinker.com/_media/esp32/docs/nodemcu-32s_product_specification.pdf)
* Module SD (3.3V, We modded it to work with that voltage)
* BC548 (Battery Medition)
* VS1838 (Infrared Receiver)
* Infrared LED (Infrared Emitter)
* LIPO Battery 3.7-4.2V
* Step Up 1.8v-4.2v to 5V (180mA)
* LIPO Charger USB-C Port
* 2N3904
* Resistors 10K, 10K, 630K (Battery Medition)
* [Six Keycaps](https://github.com/G4BY0/Control-Remoto/tree/main/hardware/STL's)
* [Case](https://github.com/G4BY0/Control-Remoto/tree/main/hardware/STL's)
* Cable USB-C for charging

### Dependencies

* [Adafruit BusIO](https://github.com/adafruit/Adafruit_BusIO)
* [Adafruit GFX Library](https://github.com/adafruit/Adafruit-GFX-Library)
* [Adafruit SH110X](https://github.com/adafruit/Adafruit_SH110x)
* [QRCode](https://www.adafruit.com/products/2471)
* [arduino-esp32](https://www.adafruit.com/products/2471)
