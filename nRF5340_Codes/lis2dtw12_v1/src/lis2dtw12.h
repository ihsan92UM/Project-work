#ifndef LIS2DTW12_H
#define LIS2DTW12_H

#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/i2c.h>

#define LIS2DTW12_I2C DT_NODELABEL(lis2dtw12)

int lis2dtw12_init(void);
int lis2dtw12_read_who_am_i(uint8_t *who_am_i);
int lis2dtw12_read_temperature(float *temperature);
int lis2dtw12_read_acceleration(float *x, float *y, float *z);

#endif /* LIS2DTW12_H */
