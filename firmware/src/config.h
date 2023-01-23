//#section Config

//#define CONFIG_DATABUS_REVERSED //Set this if the data bus pins are wired backwards

//#sectionend

//Misc Constants
#define GPIO_ON 1
#define GPIO_OFF 0
#define FM_DATA_VALID_DELAY_US 1
#define FM_WRITE_PULSE_US 1
#define SAA_DATA_VALID_DELAY_US 1
#define SAA_WRITE_PULSE_US 1
#define OPM_DATA_VALID_DELAY_US 1 //datasheet says 10 ns
#define OPM_WRITE_PULSE_US 1 //datasheet says 100 ns

//GPIO Pins
#define LED_BUILTIN 25

#define PIN_D0 16
#define PIN_D1 17
#define PIN_D2 18
#define PIN_D3 19
#define PIN_D4 20
#define PIN_D5 22
#define PIN_D6 26
#define PIN_D7 27

#define PIN_FM_CLK 21
#define PIN_FM_A0 12
#define PIN_FM_A1 13
#define PIN_FM_IC 15 //active low. this is just a reset pin.
#define PIN_FM_CS 14 //active low
#define PIN_FM_WR 11 //active low

#define PIN_SAA_CS1 10 //first SAA1099's chip select pin
#define PIN_SAA_CS2 9  //second SAA1099's chip select
#define PIN_SAA_WR 8   //the other control lines are shared
#define PIN_SAA_A0 7
#define PIN_SAA_CLK 6

#define PIN_SD_CS 5 //SD card chip select

//Bit Masks
#define MASK_PIN_D0 (1 << PIN_D0)
#define MASK_PIN_D1 (1 << PIN_D1)
#define MASK_PIN_D2 (1 << PIN_D2)
#define MASK_PIN_D3 (1 << PIN_D3)
#define MASK_PIN_D4 (1 << PIN_D4)
#define MASK_PIN_D5 (1 << PIN_D5)
#define MASK_PIN_D6 (1 << PIN_D6)
#define MASK_PIN_D7 (1 << PIN_D7)
#define MASK_DATABUS (MASK_PIN_D0 | MASK_PIN_D1 | MASK_PIN_D2 | MASK_PIN_D3 | MASK_PIN_D4 | MASK_PIN_D5 | MASK_PIN_D6 | MASK_PIN_D7)

#define MASK_LED_BUILTIN (1 << LED_BUILTIN)
#define MASK_FM_A0 (1 << PIN_FM_A0)
#define MASK_FM_A1 (1 << PIN_FM_A1)
#define MASK_FM_IC (1 << PIN_FM_IC)
#define MASK_FM_CS (1 << PIN_FM_CS)
#define MASK_FM_WR (1 << PIN_FM_WR)
#define MASK_FM_CTRL (MASK_FM_A0 | MASK_FM_A1 | MASK_FM_IC | MASK_FM_CS | MASK_FM_WR)

#define MASK_SAA_CS1 (1 << PIN_SAA_CS1)
#define MASK_SAA_CS2 (1 << PIN_SAA_CS2)
#define MASK_SAA_WR (1 << PIN_SAA_WR)
#define MASK_SAA_A0 (1 << PIN_SAA_A0)
#define MASK_SAA_CTRL (MASK_SAA_A0 | MASK_SAA_CS1 | MASK_SAA_CS2 | MASK_SAA_WR)

//SD card stuff
#include "hw_config.h"
#include "ff.h"
#include "diskio.h"

void spi0_dma_isr();

#pragma once
static spi_t spis[] = {
    {
        .hw_inst = spi0,
        .miso_gpio = 4,
        .mosi_gpio = 3,
        .sck_gpio = 2,
        .baud_rate = 12500 * 1000,
        .dma_isr = spi0_dma_isr
    }};

static sd_card_t sd_cards[] = {
    {
        .pcName = "0:",
        .spi = &spis[0],
        .ss_gpio = PIN_SD_CS,

        .use_card_detect = false,
        .m_Status = STA_NOINIT,
    }
};

void spi0_dma_isr() { spi_irq_handler(&spis[0]); }

size_t sd_get_num() { return count_of(sd_cards); }
sd_card_t *sd_get_by_num(size_t num) {
    if (num <= sd_get_num()) {
        return &sd_cards[num];
    } else {
        return NULL;
    }
}
size_t spi_get_num() { return count_of(spis); }
spi_t *spi_get_by_num(size_t num) {
    if (num <= sd_get_num()) {
        return &spis[num];
    } else {
        return NULL;
    }
}
