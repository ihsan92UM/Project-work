#ifndef LIS2DTW12_H
#define LIS2DTW12_H

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/i2c.h>

#define I2C1_NODE_LIS2DTW12 DT_NODELABEL(lis2dtw12)

void lis2dtw12_init(void);
float lis2dtw12_read_temperature(void);
void lis2dtw12_read_data(float *x, float *y, float *z);

#endif