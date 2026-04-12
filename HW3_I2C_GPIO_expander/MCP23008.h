#ifndef MCP23008_H
#define MCP23008_H

#define MCP23008_ADDR 0x20
#define I2C_PORT i2c0

void MCP23008_init(int sda, int scl);
void pin_input_output(int pin, int direction);
void set_pin(int pin, int level);
int read_pin(int pin);

#endif