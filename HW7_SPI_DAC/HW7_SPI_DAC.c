#include <stdio.h>
#include <math.h>
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "MCP4912.h"

#define SAMPLE_PERIOD_US 5000 // set to 5000 for proper values
#define TRIANGLE_SAMPLES 200
#define SINE_SAMPLES 100
#define PI 3.14159265358979323846

float get_triangle_voltage(int sample) {
    if (sample < TRIANGLE_SAMPLES / 2) {
        return (float)sample / (TRIANGLE_SAMPLES / 2) * 3.3;
    } else {
        return 3.3 - (float)(sample - TRIANGLE_SAMPLES / 2) / (TRIANGLE_SAMPLES / 2) * 3.3;
    }
}

int main() {
    stdio_init_all();

    spi_init(spi_default, 1000 * 1000); // the baud, or bits per second
    gpio_set_function(PICO_DEFAULT_SPI_RX_PIN, GPIO_FUNC_SPI);
    gpio_set_function(PICO_DEFAULT_SPI_SCK_PIN, GPIO_FUNC_SPI);
    gpio_set_function(PICO_DEFAULT_SPI_TX_PIN, GPIO_FUNC_SPI);
    gpio_init(PICO_DEFAULT_SPI_CSN_PIN);
    gpio_set_dir(PICO_DEFAULT_SPI_CSN_PIN, GPIO_OUT);
    gpio_put(PICO_DEFAULT_SPI_CSN_PIN, 1);

    int sine_index = 0;
    int triangle_index = 0;

    while (true) {
        // write_DAC(2.0, 0, PICO_DEFAULT_SPI_CSN_PIN, spi_default);
        // printf("Setting port 0 to 2 volts\n");
        // write_DAC(2.0, 1, PICO_DEFAULT_SPI_CSN_PIN, spi_default);
        // printf("Setting port 1 to 2 volts\n");
        // sleep_ms(2000);
        // write_DAC(0.0, 0, PICO_DEFAULT_SPI_CSN_PIN, spi_default);
        // printf("Setting port 0 to 0 volts\n");
        // write_DAC(0.0, 1, PICO_DEFAULT_SPI_CSN_PIN, spi_default);
        // printf("Setting port 1 to 0 volts\n");
        // sleep_ms(2000);
        float sine_voltage =  1.65 + 1.65 * sin((((float)sine_index / (float)SINE_SAMPLES) * 2 * PI));
        float triangle_voltage = get_triangle_voltage(triangle_index);

        write_DAC(sine_voltage, 0, PICO_DEFAULT_SPI_CSN_PIN, spi_default);
        write_DAC(triangle_voltage, 1, PICO_DEFAULT_SPI_CSN_PIN, spi_default);

        sine_index = (sine_index + 1) % SINE_SAMPLES;
        triangle_index = (triangle_index + 1) % TRIANGLE_SAMPLES;
        sleep_us(SAMPLE_PERIOD_US);
    }
    return 0;
}