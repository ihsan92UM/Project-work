#include "sts40.h"

static const struct i2c_dt_spec dev_i2c_sts40 = I2C_DT_SPEC_GET(I2C1_NODE_STS40);

void sts40_init(void) {
    if (!device_is_ready(dev_i2c_sts40.bus)) {
        printk("I2C bus %s is not ready!\n", dev_i2c_sts40.bus->name);
    }
}

float sts40_read_temperature(void) {
    uint8_t tx_data[1] = {0xFD};
    int ret = i2c_write_dt(&dev_i2c_sts40, tx_data, sizeof(tx_data));
    if (ret != 0) {
        printk("Failed to write to STS40 sensor\n");
        return -999.0;
    }

    k_msleep(10);
    uint8_t rx_data[3];
    ret = i2c_read_dt(&dev_i2c_sts40, rx_data, sizeof(rx_data));
    if (ret < 0) {
        printk("Failed to read from STS40 sensor\n");
        return -999.0;
    }

    int t_ticks = (rx_data[0] << 8) | rx_data[1];
    return -45.0 + 175.0 * (float)t_ticks / 65535.0;
}
