#include <stdio.h>
#include <stdint.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "ssd1306.h"
#include "MPU6050.h"

#define HEARTBEAT_INTERVAL 1000000
#define HEARTBEAT_PIN 0

uint64_t prev_heartbeat;
int heartbeat_state = 0;
// struct repeating_timer timer;
volatile MPU_data_t IMU;
// bool read_IMU(__unused struct repeating_timer *t);

int main()
{
    stdio_init_all();

    // set up heartbeat
    gpio_init(HEARTBEAT_PIN);
    gpio_set_dir(HEARTBEAT_PIN, 1);

    // set up i2c instance
    i2c_init(i2c_default, 400000);
    gpio_set_function(PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C);
    gpio_set_function(PICO_DEFAULT_I2C_SDA_PIN, GPIO_FUNC_I2C);

    // set up display
    ssd1306_setup();
    ssd1306_clear();

    // set up IMU
    MPU_init(i2c_default, PICO_DEFAULT_I2C_SDA_PIN, PICO_DEFAULT_I2C_SCL_PIN);

    // set up repeating timer to read from IMU at 100 Hz
    // add_repeating_timer_ms(-10, read_IMU, NULL, &timer);

    while (true) {
        IMU = MPU_read();

        // printf("X acceleration is %f\n", IMU.x_accel);
        // printf("Y acceleration is %f\n", IMU.y_accel);
        // printf("Z acceleration is %f\n", IMU.z_accel);
        // printf("X gyro is %f\n", IMU.x_gyro);
        // printf("Y gyro is %f\n", IMU.y_gyro);
        // printf("Z gyro is %f\n", IMU.z_gyro);

        ssd1306_clear();
        // display is 128 x 32
        // center of display is (64, 16)
        // scale from acceleration to pixels
        int pixels_x = (int)(IMU.x_accel * 200.0);
        int pixels_y = (int)(IMU.y_accel * 200.0);
        // draw horizontal line for x
        if (pixels_x < 0) {
            for (int i = 64; i > 64 + pixels_x; i--) {
                ssd1306_drawPixel(i, 16, 1);
            }
        }
        else {
            for (int i = 64; i < 64 + pixels_x; i++) {
                ssd1306_drawPixel(i, 16, 1);
            }
        }
        // draw vertical line for y
        if (pixels_y < 0) {
            for (int i = 16; i > 16 + pixels_y; i--) {
                ssd1306_drawPixel(64, i, 1);
            }
        }
        else {
            for (int i = 16; i < 16 + pixels_y; i++) {
                ssd1306_drawPixel(64, i, 1);
            }
        }
        ssd1306_update();
        // heartbeat
        if (time_us_64() - prev_heartbeat >= HEARTBEAT_INTERVAL) {
            prev_heartbeat = time_us_64();
            if (heartbeat_state == 0) {
                heartbeat_state = 1;
            }
            else {
                heartbeat_state = 0;
            }
            gpio_put(HEARTBEAT_PIN, heartbeat_state);
        }
    }
}

// bool read_IMU(__unused struct repeating_timer *t) {
    // IMU = MPU_read();
    // printf("X acceleration is %f\n", IMU.x_accel);
    // printf("Y acceleration is %f\n", IMU.y_accel);
    // printf("Z acceleration is %f\n", IMU.z_accel);
    // printf("X gyro is %f\n", IMU.x_gyro);
    // printf("Y gyro is %f\n", IMU.y_gyro);
    // printf("Z gyro is %f\n", IMU.z_gyro);
    // printf("Temperature is %f\n", IMU.temp);
// }