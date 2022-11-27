#include "config.h"
#include "databus.hpp"

class Opl3Chip {
    private:
        DataBus bus;

    public:
        void setAddr(byte addr) {
            //TODO: consolidate address line stuff and abstract with an address bus class
            gpio_put(PIN_FM_A0, addr & BIT(0));
            gpio_put(PIN_FM_A1, addr & BIT(1));
        }

        void write(byte addr, byte data) {
            setAddr(addr);
            gpio_put(PIN_FM_CS, GPIO_OFF);
            bus.set(data);
            busy_wait_us_32(FM_DATA_VALID_DELAY_US);
            gpio_put(PIN_FM_WR, GPIO_OFF);
            busy_wait_us_32(FM_WRITE_PULSE_US);

            //all done
            gpio_put(PIN_FM_CS, GPIO_ON);
            gpio_put(PIN_FM_WR, GPIO_ON);
        }

        //Initialize OPL3 clock provided by the Pico itself
        void clockInit() {
            //TODO: allow more flexibility with clocks so that other RP2040 boards can be accomodated
            //Outputs system clock divided by 10 to GPIO21
            //YMF262 should be 14,318,180 Hz (NTSC colorburst freq. x4)
            //At 142.8 MHz sys clock it should output 14.28 MHz, which is 0.2% higher
            clock_gpio_init(PIN_FM_CLK, CLOCKS_CLK_GPOUT0_CTRL_AUXSRC_VALUE_CLK_SYS, 10);
        }

        //Initialize the OPL3 chip itself
        void chipInit() {
            gpio_put(PIN_FM_IC, GPIO_OFF);
            busy_wait_ms(10); //TODO: the datasheet says the reset pulse should be 400 clock cycles
            gpio_put(PIN_FM_IC, GPIO_ON);
            busy_wait_us_32(FM_WRITE_PULSE_US);
        }

        Opl3Chip(DataBus* _datBus) {
            bus = *_datBus;
        }
};
