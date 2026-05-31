#ifndef SPI_RAM_H
#define SPI_RAM_H

#include <stdint.h>
#include "hardware/spi.h"

void SPI_RAM_init(spi_inst_t *spi_port, uint cs);
void SPI_RAM_write(spi_inst_t *spi_port, uint cs, uint16_t address, uint16_t value);
uint16_t SPI_RAM_read(spi_inst_t *spi_port, uint cs, uint16_t address);
// this is for this assignment
void SPI_RAM_read_and_write_DAC(spi_inst_t *RAM_SPI_port, uint RAM_CS, uint16_t address, spi_inst_t *DAC_SPI_port, uint DAC_CS, uint8_t DAC_port);

#endif