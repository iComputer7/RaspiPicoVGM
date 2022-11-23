#include "config.h"
#include "databus.hpp"

class Opl3Chip {
    private:
        DataBus bus;

    public:
        void setAddr(byte addr) {
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

        Opl3Chip() {

        }
};
