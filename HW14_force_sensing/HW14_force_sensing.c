#include <stdio.h>
#include "pico/stdlib.h"
#include "HX711.h"

#define HX_DATA 2
#define HX_CLK 3

float avg = -3370000.0f; // this is the average value that I'm getting from mine.
int raws[1000];
float filtered[1000];

int main()
{
    stdio_init_all();

    HX711_init(HX_DATA, HX_CLK);

    while (true) {
        int num = 0;
        uint64_t t[1000];
        scanf("%d", &num);
        for (int i = 0; i < num; i++) {
            int val = HX711_read();
            avg = (float)val * 0.01 + avg * 0.99;
            filtered[i] = avg;
            raws[i] = val;
            t[i] = to_ms_since_boot(get_absolute_time());
        }
        for (int i = 0; i < num; i++) {
            printf("%d %llu %d %f\n", i, t[i], raws[i], filtered[i]);
        }
    }
}