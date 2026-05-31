#ifndef MCP4912_H
#define MCP4912_H

#include "hardware/spi.h"

void write_DAC(float volt, uint8_t port, uint CS, spi_inst_t *SPI_PORT);

#endif