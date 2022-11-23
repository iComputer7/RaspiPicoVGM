#pragma once
class DataBus {
    public:
        void set(byte value) {
                #ifdef CONFIG_DATABUS_REVERSED
                #error "databus reversed config is set, not implemented yet"
                #else
                gpio_put(PIN_D0, value & BIT(0));
                gpio_put(PIN_D1, value & BIT(1));
                gpio_put(PIN_D2, value & BIT(2));
                gpio_put(PIN_D3, value & BIT(3));
                gpio_put(PIN_D4, value & BIT(4));
                gpio_put(PIN_D5, value & BIT(5));
                gpio_put(PIN_D6, value & BIT(6));
                gpio_put(PIN_D7, value & BIT(7));
                #endif
        }
};
