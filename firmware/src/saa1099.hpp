#include "config.h"
#include "databus.hpp"

class Saa1099Chip {
    private:
        DataBus bus;

    public:
        //chip: false = 1st chip, true = 2nd chip; addr: false = 0, true = 1
        void write(bool chip, byte addr, byte data) {
            if (chip) {
                //true = 2nd chip
                gpio_put(PIN_SAA_CS1, GPIO_ON);
                gpio_put(PIN_SAA_CS2, GPIO_OFF); //these are active low btw
            } else {
                //false = 1st chip
                gpio_put(PIN_SAA_CS2, GPIO_ON);
                gpio_put(PIN_SAA_CS1, GPIO_OFF); 
            }

            gpio_put(PIN_SAA_A0, addr);
            bus.set(data);
            busy_wait_us_32(SAA_DATA_VALID_DELAY_US);
            gpio_put(PIN_SAA_WR, GPIO_OFF);
            busy_wait_us_32(SAA_WRITE_PULSE_US);

            //done
            gpio_put(PIN_SAA_WR, GPIO_ON);
            gpio_put(PIN_SAA_CS1, GPIO_ON);
            gpio_put(PIN_SAA_CS2, GPIO_ON);
        }

        void clockInit() {
            //SAA1099 clock
            //Sound Blaster 1.0 clocks them at 7.159 MHz
            //At 142.8 MHz sys clock this outputs 7.1849 MHz according to my oscilloscope, which is 0.14% higher
            uint slice_num_saa = pwm_gpio_to_slice_num(PIN_SAA_CLK);
            pwm_set_wrap(slice_num_saa, 1); //divide by 2 prescaler, at 142.8 MHz this is 71.4 MHz
            pwm_set_chan_level(slice_num_saa, PWM_CHAN_A, 1);
            pwm_set_clkdiv(slice_num_saa, 9.9734F); //freq = (prescaled_sys_clk / 2) / divider
            pwm_set_enabled(slice_num_saa, true);
        }

        void chipInit() {
            //SAA1099 init - first chip
            write(false, 1, 0x1c); //A0 high selects a register, register 1C is frequency reset + sound enable
            write(false, 0, 0b11); //A0 low writes to the register, this resets the frequency for all channels and enables sound output

            //SAA1099 init - second chip
            write(true, 1, 0x1c);
            write(true, 0, 0b11);
        }

        Saa1099Chip(DataBus* _datBus) {
            bus = *_datBus;
        }
};
