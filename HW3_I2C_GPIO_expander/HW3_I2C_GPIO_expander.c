#include <stdio.h>
#include "pico/stdlib.h"
#include "MCP23008.h"

#define I2C_SDA 8
#define I2C_SCL 9



int main() {
    stdio_init_all();

    MCP23008_init(I2C_SDA, I2C_SCL);
    pin_input_output(0, 1);
    pin_input_output(7, 0);
    
    while (true) {
        int button_pressed = read_pin(0);
        if (button_pressed) {
            set_pin(7, 1);
        }
        else {
            set_pin(7, 0);
        }
    }
}
