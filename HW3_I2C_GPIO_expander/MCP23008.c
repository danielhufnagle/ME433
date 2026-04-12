#include <stdint.h>
#include "MCP23008.h"
#include "pico/stdlib.h"
#include "hardware/i2c.h"

void MCP23008_init(int sda, int scl) {
    // initializes the chip I2C connection

    // pull ups are external
    i2c_init(I2C_PORT, 400000);
    gpio_set_function(sda, GPIO_FUNC_I2C);
    gpio_set_function(scl, GPIO_FUNC_I2C);
}

void pin_input_output(int pin, int direction) {
    // sets direction of pin to input or output
    // 1 is input, 0 is output
    
    // read io polarity register (0x00)
    uint8_t reg = 0x00;
    uint8_t val;
    i2c_write_blocking(i2c_default, MCP23008_ADDR, &reg, 1, true);
    i2c_read_blocking(i2c_default, MCP23008_ADDR, &val, 1, false);
    
    // modify value according to pin and write back
    if (direction) {
        val |= (1 << pin);
    }
    else {
        val &= ~(1 << pin);
    }

    uint8_t buf[2] = {reg, val};
    i2c_write_blocking(i2c_default, MCP23008_ADDR, buf, 2, false);
}

void set_pin(int pin, int level) {
    // sets output pin high

    // read gpio register (0x09)
    uint8_t reg = 0x09;
    uint8_t val;
    i2c_write_blocking(i2c_default, MCP23008_ADDR, &reg, 1, true);
    i2c_read_blocking(i2c_default, MCP23008_ADDR, &val, 1, false);

    // modify value according to pin and write back
    if (level) {
        val |= (1 << pin);
    }
    else {
        val &= ~(1 << pin);
    }

    uint8_t buf[2] = {reg, val};
    i2c_write_blocking(i2c_default, MCP23008_ADDR, buf, 2, false);
}

int read_pin(int pin) {
    // reads input pin value

    // read gpio register (0x09)
    uint8_t reg = 0x09;
    uint8_t val;
    i2c_write_blocking(i2c_default, MCP23008_ADDR, &reg, 1, true);
    i2c_read_blocking(i2c_default, MCP23008_ADDR, &val, 1, false);
    
    return (val & (1 << pin) != 0);
}