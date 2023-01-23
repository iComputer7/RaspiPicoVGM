#include "config.h"
#include "databus.hpp"

class OpmChip {
    private:
        DataBus bus;

    public:
        void setAddr(byte addr) {
            //TODO: consolidate address line stuff and abstract with an address bus class
            gpio_put(PIN_FM_A0, addr & BIT(0));
        }

        void write(byte addr, byte data) {
            setAddr(addr);
            gpio_put(PIN_FM_CS, GPIO_OFF);
            bus.set(data);
            busy_wait_us_32(FM_DATA_VALID_DELAY_US);
            gpio_put(PIN_FM_WR, GPIO_OFF);
            busy_wait_us_32(OPM_WRITE_PULSE_US);

            //all done
            gpio_put(PIN_FM_CS, GPIO_ON);
            gpio_put(PIN_FM_WR, GPIO_ON);
        }

        //Initialize OPM clock provided by the Pico itself
        void clockInit() {
            //TODO: allow more flexibility with clocks so that other RP2040 boards can be accomodated
            //Outputs system clock divided by 36 to GPIO21
            //YM2151 should be 4,000,000 Hz sometimes
            //At 142.8 MHz sys clock it should output 3,966,667 MHz, which is 0.84% lower
            clock_gpio_init(PIN_FM_CLK, CLOCKS_CLK_GPOUT0_CTRL_AUXSRC_VALUE_CLK_SYS, 10);
        }

        //Initialize the OPM chip itself
        void chipInit() {
            gpio_put(PIN_FM_IC, GPIO_OFF);
            busy_wait_ms(10); //TODO: double check datasheet
            gpio_put(PIN_FM_IC, GPIO_ON);
            busy_wait_us_32(OPM_WRITE_PULSE_US);
        }

        OpmChip(DataBus* _datBus) {
            bus = *_datBus;
        }
};