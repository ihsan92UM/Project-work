#include "sts40.h"

/* Initialize the STS40 sensor */
int sts40_init(const struct sts40_data *dev) {
    if (!device_is_ready(dev->i2c.bus)) {
        printk("I2C bus %s is not ready!\n\r", dev->i2c.bus->name);
        return -1;
    }
    printk("I2C bus %s is ready!\n\r", dev->i2c.bus->name);
    return 0;
}

/* Read temperature from the STS40 sensor */
int sts40_read_temperature(const struct sts40_data *dev, float *temperature) {
    uint8_t tx_data[1] = {0xFD}; // Command to trigger temperature measurement
    int ret;

    /* Send the measurement command */
    ret = i2c_write_dt(&dev->i2c, tx_data, sizeof(tx_data));
    if (ret != 0) {
        printk("Failed to write to I2C device address 0x%X, error %d\n", dev->i2c.addr, ret);
        return -1;
    }

    /* Wait for measurement */
    k_msleep(10);

    /* Read the temperature data */
    uint8_t rx_data[3];
    ret = i2c_read_dt(&dev->i2c, rx_data, sizeof(rx_data));
    if (ret < 0) {
        printk("Failed to read from STS40 sensor, error %d\n", ret);
        return -1;
    }

    /* Calculate temperature */
    int t_ticks = (rx_data[0] << 8) | rx_data[1];
    *temperature = -45.0 + 175.0 * (float)t_ticks / 65535.0;

    return 0;
}