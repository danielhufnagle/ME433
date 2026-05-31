#include <stdint.h>
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "SPI_RAM.h"

// RAM is 23K256
uint8_t write_status_command = 0b00000001;
uint8_t sequential_mode = 0b01000000;
uint8_t write_command = 0b00000010;
uint8_t read_command = 0b00000011;

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

void SPI_RAM_init(spi_inst_t *spi_port, uint cs) {
    cs_select(cs);
    // write to status register to set to sequential mode
    uint8_t buf[2] = {write_status_command, sequential_mode};
    spi_write_blocking(spi_port, buf, 2);
    cs_deselect(cs);
}

void SPI_RAM_write(spi_inst_t *spi_port, uint cs, uint16_t address, uint16_t value) {
    cs_select(cs);
    uint8_t buf[5] = {write_command, (uint8_t)(address >> 8), (uint8_t)(address & 0xFF), (uint8_t)(value >> 8), (uint8_t)(value & 0xFF)};
    spi_write_blocking(spi_port, buf, 5);
    cs_deselect(cs);
}

uint16_t SPI_RAM_read(spi_inst_t *spi_port, uint cs, uint16_t address) {
    uint8_t write_buf[5] = {read_command, (uint8_t)(address >> 4), (uint8_t)(address & 0xFF), 0, 0}; // last two bytes are garbage
    uint8_t read_buf[5] = {0, 0, 0, 0, 0}; // first three bytes are garbage
    cs_select(cs);
    spi_write_read_blocking(spi_port, write_buf, read_buf, 5);
    cs_deselect(cs);
    return ((uint16_t)(read_buf[3]) << 8) | (uint16_t)(read_buf[4]);
}

void SPI_RAM_read_and_write_DAC(spi_inst_t *RAM_SPI_port, uint RAM_CS, uint16_t address, spi_inst_t *DAC_SPI_port, uint DAC_CS, uint8_t DAC_port) {
    // Read from the RAM
    uint8_t write_buf[5] = {read_command, (uint8_t)(address >> 8), (uint8_t)(address & 0xFF), 0, 0}; // last two bytes are garbage
    uint8_t read_buf[5] = {0, 0, 0, 0, 0}; // first three bytes are garbage
    cs_select(RAM_CS);
    spi_write_read_blocking(RAM_SPI_port, write_buf, read_buf, 5);
    cs_deselect(RAM_CS);
    printf("Read the following from RAM: %x, %x\n", read_buf[3], read_buf[4]);
    
    // preprocess for DAC write (MCP4912)
    uint16_t buf_16 = 0;
    buf_16 |= DAC_port << 15;
    buf_16 |= 0 << 14; // we are unbuffered
    buf_16 |= 1 << 13; // we want gain set to 1x
    buf_16 |= 1 << 12; // we want the DAC to be active and not shut down
    buf_16 |= ((uint16_t)read_buf[3] << 8) | ((uint16_t)read_buf[4]);
    uint8_t buf[2] = {0, 0};
    buf[0] = (buf_16 & 0xFF00) >> 8;
    buf[1] = buf_16 & 0x00FF;
    printf("Spi values sent to DAC: %x, %x\n", buf[0], buf[1]);
    cs_select(DAC_CS);
    spi_write_blocking(DAC_SPI_port, buf, 2);
    cs_deselect(DAC_CS);
}