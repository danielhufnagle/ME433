#include <stdint.h>
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "MCP4912.h"

// default pico implementations of SPI chip select and deselect
static inline void cs_select(uint cs_pin) {
    asm volatile("nop \n nop \n nop"); // FIXME
    gpio_put(cs_pin, 0);
    asm volatile("nop \n nop \n nop"); // FIXME
}

static inline void cs_deselect(uint cs_pin) {
    asm volatile("nop \n nop \n nop"); // FIXME
    gpio_put(cs_pin, 1);
    asm volatile("nop \n nop \n nop"); // FIXME
}

void write_DAC(float volt, uint8_t port, uint CS, spi_inst_t *SPI_PORT) {
    uint16_t buf_16 = 0;
    buf_16 |= port << 15;
    buf_16 |= 0 << 14; // we are unbuffered
    buf_16 |= 1 << 13; // we want gain set to 1x
    buf_16 |= 1 << 12; // we want the DAC to be active and not shut down
    uint16_t voltage = (uint16_t)((volt / 3.3) * 1023.0);
    voltage &= 0x3FF; // isolate 10 least significant bits
    buf_16 |= voltage << 2;
    uint8_t buf[2] = {0, 0};
    buf[0] = (buf_16 & 0xFF00) >> 8;
    buf[1] = buf_16 & 0x00FF;
    printf("Sending value %d to DAC\n", voltage);
    printf("Spi values sent: %x, %x\n", buf[0], buf[1]);

    cs_select(CS);
    spi_write_blocking(SPI_PORT, buf, 2); // we can use a uint16_t here as buf because
    cs_deselect(CS);
}