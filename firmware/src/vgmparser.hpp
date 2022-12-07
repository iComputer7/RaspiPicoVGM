#include "filehandler.hpp"

#define HEADER_U32(x) *(uint32_t*)(&header_data[x])

//struct to unpack dual chip flag and clock from header
typedef struct {
    bool t6w28 : 1;      //bit 31
    bool dual : 1;       //bit 30
    uint32_t clock : 29; //bits 0-29
} DualChipClk;

//Enum with all the chips we care about and their offsets in the header
//only defining chips that are used or might be used
enum class VgmHeaderChip : byte {
    SN76489 = 0xc,
    YM2612 = 0x2c, //OPN2
    YM3812 = 0x50, //OPL2
    YM3526 = 0x54, //OPL1
    YMF262 = 0x5c, //OPL3
    SAA1099 = 0xc8
};

class VgmParser {
    private:
        FileHandler* file;
        Opl3Chip* opl3;
        Saa1099Chip* saa1099;

        uint16_t delayCycles = 0;
        uint32_t startOffset = 0;
        uint32_t music_length = 0;
        uint32_t loopOffset = 0;
        byte header_data[0xff];

        DualChipClk* getHeaderClock(byte offset) {
            return (DualChipClk*)(&header_data[offset]);
        }

    public:
        VgmParser(FileHandler* _fileHandler, Opl3Chip* _opl3, Saa1099Chip* _saa1099) {
            file = _fileHandler;
            opl3 = _opl3;
            saa1099 = _saa1099;
        }

        void tick();

        void readHeader() {
            printf("Reading 0xFF bytes from file and parsing header.\n");
            file->rewind();
            uint bytesRead = file->readIntoBuffer(header_data, 0xff);
            printf("Read %X (%u) bytes.\n", bytesRead, bytesRead);

            music_length = HEADER_U32(0x4) + 4; //this is a relative offset instead of an absolute one because fuck you
            loopOffset = HEADER_U32(0x1c) + 0x1c; //ditto
            
            //determining file start
            if (getVersion() < 0x0150) {
                startOffset = 0x40;
            } else {
                startOffset = HEADER_U32(0x34) + 0x34;
            }
        }

        //VGM file's version, as a BCD coded number
        uint32_t getVersion() {
            return HEADER_U32(0x08);
        }

        //Chip's expected clock speed from file header, in Hz
        uint32_t getChipClock(VgmHeaderChip chip) {
            DualChipClk* headerClk = getHeaderClock((byte)chip);
            return headerClk->clock;
        }

        //Dual chip bit from file header
        bool isDual(VgmHeaderChip chip) {
            DualChipClk* headerClk = getHeaderClock((byte)chip);
            
            if (chip == VgmHeaderChip::SAA1099) {
                //SAA1099 has some special conditions
                if (!(getVersion() >= 0x0171)) { //if file is older than 1.71
                    return false;
                }
            }
            
            return headerClk->dual;
        }

        //Song's length, in samples
        uint32_t getLengthSamples() {
            return HEADER_U32(0x18);
        }

        //Song's length, in seconds
        float getLengthSeconds() {
            return (1.0F / 44100.0F) * getLengthSamples();
        }

        //File offset where song actually starts
        uint32_t getStartOffset() {
            return startOffset;
        }

        //File offset where song loops back to
        uint32_t getLoopOffset() {
            return loopOffset;
        }

        //Check if chip is present
        bool isPresent(VgmHeaderChip chip) {
            if (chip == VgmHeaderChip::SAA1099) {
                //SAA1099 has some special conditions
                if (!(getVersion() >= 0x0171)) { //if file is older than 1.71
                    return false;
                }
            }
            
            return getChipClock(chip) > 0;
        }
};

void VgmParser::tick() {
    gpio_xor_mask(MASK_LED_BUILTIN);

    if (delayCycles > 0) {
        delayCycles--;
        return;
    }

    //Parse VGM commands
    //Full command listing and VGM file spec: https://vgmrips.net/wiki/VGM_Specification
    byte curByte = file->readByte();
    switch (curByte) {
        case 0x4F: { //Game Gear PSG stereo register write
            file->skip(1);
            break;
        }

        //TODO: SN76489 stuff because of the tandy 3 voice sound
        case 0x50: { //SN76489/SN76496 write
            file->skip(1);
            break;
        }

        case 0x61: { //Wait x cycles
            delayCycles = file->readU16();
            break;
        }

        case 0x62: { //Wait 735 samples
            delayCycles = 735;
            break;
        }

        case 0x63: { //Wait 882 samples
            delayCycles = 882;
            break;
        }

        //Short Delays
        case 0x70:
        case 0x71:
        case 0x72:
        case 0x73:
        case 0x74:
        case 0x75:
        case 0x76:
        case 0x77:
        case 0x78:
        case 0x79:
        case 0x7A:
        case 0x7B:
        case 0x7C:
        case 0x7D:
        case 0x7E:
        case 0x7F: {
            delayCycles = curByte & 0xf;
            break;
        }

        case 0x66: { //Loop
            printf("%X: Got command 0x66. Looping back to offset 0x%X.\n", file->getPos(), loopOffset);
            file->seek(loopOffset);
            break;
        }

        //YMF262 stuff
        case 0x5A: //YM3812 (OPL2)
        case 0x5B: //YM3526 (OPL1)
        case 0x5E: { //YMF262 port 0
            byte regi = file->readByte();
            byte data = file->readByte();

            //send register
            opl3->write(0, regi);
            busy_wait_us_32(FM_WRITE_PULSE_US);
            
            //send data
            opl3->write(1, data);
            busy_wait_us_32(FM_WRITE_PULSE_US);

            break;
        }

        case 0x5F: { //YMF262 port 1
            byte regi = file->readByte();
            byte data = file->readByte();

            //send register
            opl3->write(0b10, regi);
            busy_wait_us_32(FM_WRITE_PULSE_US);
            
            //send data
            opl3->write(0b11, data);
            busy_wait_us_32(FM_WRITE_PULSE_US);

            break;
        }

        //SAA1099 stuff
        case 0xBD: { //SAA1099 write
            byte regi = file->readByte();
            byte data = file->readByte();

            //figuring out which chip to write to
            //bit 7: low = chip 1, high = chip 2
            bool chip = (regi & BIT(7)) != 0;

            //send register
            saa1099->write(chip, true, regi);
            busy_wait_us_32(SAA_WRITE_PULSE_US);
            
            //send data
            saa1099->write(chip, false, data);
            busy_wait_us_32(SAA_WRITE_PULSE_US);

            break;
        }

        //Chips that won't be handled
        //Commands with 2 byte long parameters
        case 0x51: //YM2413
        case 0x54: //YM2151
        case 0x55: //YM2203
        case 0x56: //YM2608 port 0
        case 0x57: //YM2608 port 1
        case 0x58: //YM2610 port 0
        case 0x59: //YM2610 port 1
        case 0x5C: //Y8950
        case 0x5D: //YMZ280B
        case 0x52: //YM2612 port 0
        case 0x53: { //YM2612 port 1
            file->skip(1);
            break;
        }

        //PCM stuff
        case 0x67: { //data block
            //TODO: skip these
            break;
        }
        case 0xE0: { //seek to offset in PCM data bank
            file->skip(3);
            break;
        }

        //YM2612 port 0 address 2A write from data bank, then wait n samples
        case 0x80:
        case 0x81:
        case 0x82:
        case 0x83:
        case 0x84:
        case 0x85:
        case 0x86:
        case 0x87:
        case 0x88:
        case 0x89:
        case 0x8A:
        case 0x8B:
        case 0x8C:
        case 0x8D:
        case 0x8E:
        case 0x8F: {
            break;
        }

        default: {
            printf("%X: Encountered unknown command %X. Ignoring.\n", file->getPos(), curByte);
            break;
        }
    }

    if (file->getPos() == music_length) {
        file->seek(startOffset);
        printf("Reached end of song. Looping.\n");
    }

}