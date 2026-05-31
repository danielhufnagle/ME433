#include <stdio.h>
#include <math.h>
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "MCP4912.h"
#include "SPI_RAM.h"

#define DAC_SPI spi0
#define DAC_MISO 16
#define DAC_CS 17
#define DAC_SCK 18
#define DAC_MOSI 19
#define RAM_SPI spi1
#define RAM_MISO 12
#define RAM_CS 13
#define RAM_SCK 14
#define RAM_MOSI 15
#define PI 3.14159265358979323846

uint16_t initial_address = 0x0000;

void fill_lut() {
    uint16_t address = 0x0000;
    for (int i = 0; i < 1000; i++) {
        float voltage = 1.65 + 1.65 * sin((float)i / 1000.0 * 2.0 * PI);
        uint16_t lut = (uint16_t)((voltage / 3.3) * 1023.0);
        lut &= 0x3FF;
        lut = lut << 2;
        SPI_RAM_write(RAM_SPI, RAM_CS, address, lut);
        address += 2;
    }
}

int main() {
    stdio_init_all();

    // initialize SPI
    spi_init(DAC_SPI, 1000*1000);
    spi_init(RAM_SPI, 1000*1000);
    gpio_set_function(DAC_MISO, GPIO_FUNC_SPI);
    gpio_set_function(DAC_SCK,  GPIO_FUNC_SPI);
    gpio_set_function(DAC_MOSI, GPIO_FUNC_SPI);
    gpio_set_function(RAM_MISO, GPIO_FUNC_SPI);
    gpio_set_function(RAM_SCK, GPIO_FUNC_SPI);
    gpio_set_function(RAM_MOSI, GPIO_FUNC_SPI);
    gpio_init(DAC_CS);
    gpio_set_dir(DAC_CS, GPIO_OUT);
    gpio_put(DAC_CS, 1);
    gpio_init(RAM_CS);
    gpio_set_dir(RAM_CS, GPIO_OUT);
    gpio_put(RAM_CS, 1);

    SPI_RAM_init(RAM_SPI, RAM_CS);
    fill_lut();

    int sine_index = 0;
    uint16_t address;
    while (true) {
        address = sine_index * 2;
        SPI_RAM_read_and_write_DAC(RAM_SPI, RAM_CS, address, DAC_SPI, DAC_CS, 0);
        sine_index = (sine_index + 1) % 1000;
        sleep_ms(1);
    }
}
