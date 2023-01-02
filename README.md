# Pico VGM Player

Hardware VGM player with a Raspberry Pi Pico and a Yamaha YMF262 (OPL3), with some experimental support for dual Philips SAA1099.

**Please be aware that this is very much a proof of concept! It lacks a lot of essential functionality at the moment!
I am incredibly inexperienced with the RP2040 platform as a whole and it certainly shows here.
I think I at least tried to somewhat explain my thought process through comments.
Pull requests and issues are welcomed and encouraged! If you can clean up this mess or have ideas on what I should change, do not hesitate to share them!**

As of right now, it only supports playing uncompressed VGM files that are on the root of the SD card and named `song.vgm`.

**Scroll down to see GPIO mappings!**

Various circuits and code snippets are adapted from:

* (OPL3 subsystem) [Throwback Operator by @AidanHockey5](https://github.com/AidanHockey5/Throwback_Operator)
* (SAA1099 subsystem) [Snark Barker and Sound Blaster 1.0 by @schlae and Creative Labs](https://github.com/schlae/snark-barker)

---

## Todo List (not in any specific order)

* Make a complete schematic
* Get something that resembles a user interface
* Code better error handling that doesn't involve giving up once an error has been encountered
* Change the clock speed of the relevant chips based on what's set in the file header
* Refuse to play files that call for unsupported chips
* Switch to C++ and move stuff into classes to make code more readable and clean
* (Future) VGZ support
* (Future) RetroWave OPL3 support
* (Future) Allow users to pick and choose which chips they have without needing to recompile the firmware
* (Future) YM2612 support
* (Future) Dual SN76489 support
* (Future) OPLL playback by translating OPLL data to OPL3 data (or through a real OPLL if translation is not possible)
* (Future) Pico W support with some way to remotely control playback and manage files on the SD card
* (Future) An actual PCB that integrates the RP2040 directly onto it for skilled solderers who want to save space and have a better voltage regulator

---

## Helpful Resources

* [VGM File Format Specification](https://vgmrips.net/wiki/VGM_Specification)
* [Throwback Operator Source Code](https://github.com/AidanHockey5/Throwback_Operator)
* [Throwback Operator Schematic](https://raw.githubusercontent.com/AidanHockey5/Throwback_Operator/master/Schematic/OPL3_VGM_Player/OPL3_VGM_Player.png)
* [Tube Time - Sound Blaster 1.0 Principles of Operation](http://tubetime.us/index.php/2019/01/19/sound-blaster-1-0-principles-of-operation/)
* [Snark Barker Github](https://github.com/schlae/snark-barker)
* [Snark Barker Schematic](https://raw.githubusercontent.com/schlae/snark-barker/master/SnarkBarker.pdf)

---

## GPIO Pins

*The VGM tick clock needs GPIO0 and GPIO1 connected. This is a temporary hack because I don't know what I'm doing. The RP2040 has bizarre timers.*

Pin | Description
--- | ---
GPIO0 | VGM tick input. Connect to 44.1 KHz clock or else songs won't play!
GPIO1 | 44.1 KHz clock generator output (connected to GPIO0)
GPIO2 | SPI SCK
GPIO3 | SPI MOSI
GPIO4 | SPI MISO
GPIO5 | SD card chip select
GPIO6 | SAA1099 clock
GPIO7 | SAA1099 A0
GPIO8 | SAA1099 WR
GPIO9 | SAA1099 #2 chip select
GPIO10 | SAA1099 #1 chip select
GPIO11 | OPL3 WR
GPIO12 | OPL3 A0
GPIO13 | OPL3 A1
GPIO14 | OPL3 chip select
GPIO15 | OPL3 IC (reset pin)
GPIO16 | Data bus bit 0
GPIO17 | Data bus bit 1
GPIO18 | Data bus bit 2
GPIO19 | Data bus bit 3
GPIO20 | Data bus bit 4
GPIO22 | Data bus bit 5
GPIO26 | Data bus bit 6
GPIO27 | Data bus bit 7
