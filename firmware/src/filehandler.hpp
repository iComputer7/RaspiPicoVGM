#include "config.h"

class FileHandler {
    private:
        FRESULT fr;
        FATFS fs;
        FIL vgmFile;
        char* filename;

    public:
        FileHandler(char* _filename) {
            filename = _filename;
        }

        ~FileHandler() {
            //TODO: close file, unmount SD card
        }

        byte readByte() {
            uint bytesRead = 0;
            byte buf = 0;
            FRESULT r = f_read(&vgmFile, &buf, 1, &bytesRead);

            if (r != FR_OK || bytesRead != 1) {
                printf("Failed to read a single byte from the file. Hanging.\n");
                for (;;) {
                    tight_loop_contents();
                }
            }

            return buf;
        }

        uint16_t readU16() {
            uint bytesRead = 0;
            uint16_t buf = 0;
            FRESULT r = f_read(&vgmFile, &buf, 2, &bytesRead);

            if (r != FR_OK || bytesRead != 2) {
                printf("Failed to read two bytes from the file. Hanging.\n");
                for (;;) {
                    tight_loop_contents();
                }
            }

            return buf;
        }

        uint32_t readU32() {
            uint bytesRead = 0;
            uint32_t buf = 0;
            FRESULT r = f_read(&vgmFile, &buf, 4, &bytesRead);

            if (r != FR_OK || bytesRead != 4) {
                printf("Failed to read four bytes from the file. Hanging.\n");
                for (;;) {
                    tight_loop_contents();
                }
            }

            return buf;
        }

        uint32_t readIntoBuffer(void* buf, uint32_t bytesToRead) {
            uint bytesRead = 0;
            FRESULT r = f_read(&vgmFile, buf, bytesToRead, &bytesRead);

            if (r != FR_OK) {
                printf("Failed to read %u bytes from the file. Hanging.\n", bytesToRead);
                for (;;) {
                    tight_loop_contents();
                }
            }

            return bytesRead;
        }

        void skip(int bytesToSkip) {
            FRESULT r = f_lseek(&vgmFile, f_tell(&vgmFile) + bytesToSkip);

            if (r != FR_OK) {
                printf("Failed to skip %u bytes in the file. Hanging.\n", bytesToSkip);
                for (;;) {
                    tight_loop_contents();
                }
            }
        }

        void seek(uint32_t fOffset) {
            FRESULT r = f_lseek(&vgmFile, fOffset);

            if (r != FR_OK) {
                printf("Failed to seek to offset 0x%X (%u) in the file. Hanging.\n", fOffset, fOffset);
                for (;;) {
                    tight_loop_contents();
                }
            }
        }

        uint64_t getPos() {
            return f_tell(&vgmFile);
        }

        //Initialize SD card driver, mount filesystem, and open file for reading
        void init() {
            //Initialize SD card
            if (!sd_init_driver()) {
                printf("ERROR: Could not initialize SD card\n");
                for (;;) {
                    tight_loop_contents();
                }
            } else {
                printf("Success!\n");
            }

            //Mount drive
            fr = f_mount(&fs, "0:", 1);
            if (fr != FR_OK) {
                printf("ERROR: Could not mount filesystem (%d)\n", fr);
                for (;;) {
                    tight_loop_contents();
                }
            }

            //Open file for reading
            fr = f_open(&vgmFile, filename, FA_READ);
            if (fr != FR_OK) {
                printf("ERROR: Could not open file (%d)\n", fr);
                for (;;) {
                    tight_loop_contents();
                }
            }
        }

        //Seek to position 0 in the file
        void rewind() {
            f_rewind(&vgmFile);
        }
};
