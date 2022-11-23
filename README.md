# Pico OPL3 Player

Hardware VGM player with a Raspberry Pi Pico and a Yamaha YMF262 (OPL3), with some experimental support for dual Philips SAA1099.

**Please be aware that this is very much a proof of concept! It lacks a lot of essential functionality at the moment!
I am incredibly inexperienced with the RP2040 platform as a whole and it certainly shows here.
I think I at least tried to somewhat explain my thought process through comments.
Pull requests and issues are welcomed and encouraged! If you can clean up this mess or have ideas on what I should change, do not hesitate to share them!**

As of right now, it only supports playing uncompressed VGM files that are on the root of the SD card and named `song.vgm`.

***TODO: put a nice table somewhere that has what's hooked up to where and anything else that's needed. For right now you can check config.h to see what each GPIO does.***

Various circuits and code snippets are adapted from Throwback Operator by @AidanHockey5 and Snark Barker/Sound Blaster 1.0 by @schlae and Creative Labs.

---

## Todo List

* Make a complete schematic
* Get something that resembles a user interface
* Code better error handling that doesn't involve giving up once an error has been encountered
* Change the clock speed of the relevant chips based on what's set in the file header
* Refuse to play files that call for unsupported chips
* Switch to C++ and move stuff into classes to make code more readable and clean
* (Future) VGZ support
* (Future) Implement RetroWave OPL3 compatibility support
* (Future) Allow users to pick and choose which chips they have without needing to recompile the firmware
* (Future) YM2612 support
* (Future) Dual SN76489 support
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
