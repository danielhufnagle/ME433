#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/uart.h"

// UART defines
// By default the stdout UART is `uart0`, so we will use the second one
#define UART_ID uart1
#define BAUD_RATE 115200

// Use pins 4 and 5 for UART1
// Pins can be changed, see the GPIO function select table in the datasheet for information on GPIO assignments
#define UART_TX_PIN 4
#define UART_RX_PIN 5

#define RX_STATE 0
#define TX_STATE 1

int tx_rx_state = RX_STATE;

int main()
{
    stdio_init_all();

    // Set up our UART
    uart_init(UART_ID, BAUD_RATE);
    // Set the TX and RX pins by using the function select on the GPIO
    // Set datasheet for more information on function select
    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);
    
    // Use some the various UART functions to send out data
    // In a default system, printf will also output via the default UART
    
    // Send out a string, with CR/LF conversions
    // uart_puts(UART_ID, " Hello, UART!\n");
    // uart_getc();
    
    // For more examples of UART use see https://github.com/raspberrypi/pico-examples/tree/master/uart

    while (true) {

        // poll serial monitor
        int pc_char = getchar_timeout_us(0); // use this instead of scanf (scanf caused too many headaches with timing issues)
        if (pc_char != PICO_ERROR_TIMEOUT) {
            uint8_t out_c = (uint8_t)pc_char;
            uart_write_blocking(UART_ID, &out_c, 1);
        }

        // poll stm32 uart
        if (uart_is_readable(UART_ID)) {
            uint8_t in_c = uart_getc(UART_ID);
            printf("Pico Received: %c\n", in_c);
        }
    }
}
