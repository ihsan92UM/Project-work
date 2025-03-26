#ifndef STS40_H
#define STS40_H

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/i2c.h>

#define I2C1_NODE DT_NODELABEL(sts40)

/* Structure to hold the STS40 sensor data */
struct sts40_data {
    const struct i2c_dt_spec i2c;
};

/* Function prototypes */
int sts40_init(const struct sts40_data *dev);
int sts40_read_temperature(const struct sts40_data *dev, float *temperature);

#endif /* STS40_H */