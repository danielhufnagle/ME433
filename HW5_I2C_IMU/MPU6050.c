#include "MPU6050.h"
#include "hardware/i2c.h"

MPU_data_t MPU_data;
int MPU_sda;
int MPU_scl;
i2c_inst_t* i2c_inst;

void MPU_init(i2c_inst_t *i2c, uint sda, uint scl) {
    // set up i2c in the library
    i2c_inst = i2c;
    MPU_sda = sda;
    MPU_scl = scl;

    // check who am i
    uint8_t val;
    uint8_t reg = WHO_AM_I;
    i2c_write_blocking(i2c_inst, MPU_ADDR, &reg, 1, true);
    i2c_read_blocking(i2c_inst, MPU_ADDR, &val, 1, false);

    // turn on the IMU
    val = 0x00;
    reg = PWR_MGMT_1;
    uint8_t buf[2] = {reg, val};
    i2c_write_blocking(i2c_inst, MPU_ADDR, buf, 2, false);

    // enable accelerometer
    val = 0b11100000; // have all axes self test and set sensitivity to ±2g
    reg = ACCEL_CONFIG;
    i2c_write_blocking(i2c_inst, MPU_ADDR, buf, 2, false);

    // enable gyroscope
    val = 0b11110000; // have all axes self test and set sensitivity to ±2000º/s
    reg = GYRO_CONFIG;
    i2c_write_blocking(i2c_inst, MPU_ADDR, buf, 2, false);
}

MPU_data_t MPU_read() {
    uint8_t reg = ACCEL_XOUT_H; // sequential read will access all subsequent registers
    uint8_t read_buf[14] = {0};
    i2c_write_blocking(i2c_inst, MPU_ADDR, &reg, 1, true);
    i2c_read_blocking(i2c_inst, MPU_ADDR, read_buf, 14, false);
    

    // piece together the IMU raw data
    int16_t x_accel_int = ((int16_t)read_buf[0] << 8) | read_buf[1];
    int16_t y_accel_int = ((int16_t)read_buf[2] << 8) | read_buf[3];
    int16_t z_accel_int = ((int16_t)read_buf[4] << 8) | read_buf[5];
    int16_t x_gyro_int = ((int16_t)read_buf[6] << 8) | read_buf[7];
    int16_t y_gyro_int = ((int16_t)read_buf[8] << 8) | read_buf[9];
    int16_t z_gyro_int = ((int16_t)read_buf[10] << 8) | read_buf[11];
    int16_t temp_int = ((int16_t)read_buf[12] << 8) | read_buf[13];

    // fill MPU_data struct
    MPU_data.x_accel = (float)x_accel_int * 0.000061;
    MPU_data.y_accel = (float)y_accel_int * 0.000061;
    MPU_data.z_accel = (float)z_accel_int * 0.000061;
    MPU_data.x_gyro = (float)x_gyro_int * 0.007630;
    MPU_data.y_gyro = (float)y_gyro_int * 0.007630;
    MPU_data.z_gyro = (float)z_gyro_int * 0.007630;
    MPU_data.temp = (float)temp_int / 340.0 + 36.53;
    
    return MPU_data;
}