#include <stdio.h>
#include "pico/stdlib.h"

#define UP_BUTTON 0
#define DOWN_BUTTON 1
#define SELECT_BUTTON 2

int main()
{
    stdio_init_all();
    gpio_init(UP_BUTTON);
    gpio_init(DOWN_BUTTON);
    gpio_init(SELECT_BUTTON);
    gpio_set_dir(UP_BUTTON, 0);
    gpio_set_dir(DOWN_BUTTON, 0);
    gpio_set_dir(SELECT_BUTTON, 0);

    while (true) {
        // printf("Hello, world!\n");
        // sleep_ms(1000);
        if (!gpio_get(UP_BUTTON)) {
            printf("u\n");
            sleep_ms(300);
        }
        if (!gpio_get(DOWN_BUTTON)) {
            printf("d\n");
            sleep_ms(300);
        }
        if (!gpio_get(SELECT_BUTTON)) {
            printf("s\n");
            sleep_ms(300);
        }
        sleep_ms(10);
    }
}
