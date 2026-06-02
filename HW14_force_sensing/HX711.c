#include <stdio.h>
#include "pico/stdlib.h"
#include "hx711.h"

int clk_pin;
int data_pin;

void HX711_init(int data, int clk) {
    gpio_init(data);
    gpio_init(clk);
    data_pin = data;
    clk_pin = clk;
    gpio_set_dir(data, 0);
    // gpio_pull_down(data_pin);
    gpio_set_dir(clk, 1);
}

int HX711_read() {
    // data pin high -> ADC is sampling
    // data pin low -> ADC is ready to read
    while (gpio_get(data_pin) == 1) {
        tight_loop_contents();
    }

    int raw = 0;
    for (int i = 0; i < 24; i++) {
        gpio_put(clk_pin, 1);
        sleep_us(10);
        gpio_put(clk_pin, 0);
        sleep_us(10);
        // read raw data after clock pulse
        // otherwise, I just get constant 1
        raw = (raw << 1) | gpio_get(data_pin);
    }
    // one last pulse to make sure the next read uses a gain of 128
    gpio_put(clk_pin, 1);
    sleep_us(10);
    gpio_put(clk_pin, 0);
    sleep_us(10);

    if (raw & 0x800000) {
        raw |= 0xFF000000;
    }

    // printf("Raw is %d\n", raw);
    return raw;
}