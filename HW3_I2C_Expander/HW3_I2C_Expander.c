#include <stdio.h>
#include <stdint.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "MCP23008.h"

#define I2C_SDA 8
#define I2C_SCL 9
#define HEARTBEAT_PIN 0
#define HEARTBEAT_INTERVAL 500000

int prev_time;
int heartbeat_state = 0;

int main()
{
    stdio_init_all();
    gpio_init(HEARTBEAT_PIN);
    gpio_set_dir(HEARTBEAT_PIN, 1);
    MCP23008_init(I2C_SDA, I2C_SCL);
    pin_io(7, PIN_OUTPUT);
    pin_io(0, PIN_INPUT);
    prev_time = time_us_64();
    while (true) {
        int button = read_pin(0);
        if (button == 0) {
            set_pin(7, 1);
        }
        else {
            set_pin(7, 0);
        }
        if (time_us_64() - prev_time >= HEARTBEAT_INTERVAL) {
            prev_time = time_us_64();
            if (heartbeat_state == 0) {
                heartbeat_state = 1;
            }
            else {
                heartbeat_state = 0;
            }
        }
        gpio_put(HEARTBEAT_PIN, heartbeat_state);
    }
}
