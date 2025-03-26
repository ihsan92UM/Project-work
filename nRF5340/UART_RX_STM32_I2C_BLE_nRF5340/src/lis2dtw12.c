#include "lis2dtw12.h"

static const struct i2c_dt_spec dev_i2c_lis2dtw12 = I2C_DT_SPEC_GET(I2C1_NODE_LIS2DTW12);

void lis2dtw12_init(void) {
    if (!device_is_ready(dev_i2c_lis2dtw12.bus)) {
        printk("I2C bus %s is not ready!\n", dev_i2c_lis2dtw12.bus->name);
    }
    // Configure sensor
    uint8_t ctrl1[] = {0x20, 0x75};  // Set ODR and mode
    if (i2c_write_dt(&dev_i2c_lis2dtw12, ctrl1, sizeof(ctrl1)) != 0) {
        //printk("Failed to configure CTRL1 register\n");
        return -1;
    }

    uint8_t ctrl6[] = {0x25, 0x04};  // Set filter, FS, and low noise mode
    if (i2c_write_dt(&dev_i2c_lis2dtw12, ctrl6, sizeof(ctrl6)) != 0) {
        //printk("Failed to configure CTRL6 register\n");
        return -1;
    }

    //printk("LIS2DTW12 initialized successfully!\n");
    return 0;
}

float lis2dtw12_read_temperature(void) {
    uint8_t registers[] = {0x0D, 0x0E};
    uint8_t data[2] = {0};
    for (int i = 0; i < 2; i++) {
        int ret = i2c_write_read_dt(&dev_i2c_lis2dtw12, &registers[i], 1, &data[i], 1);
        if (ret != 0) {
            printk("Failed to read temperature register 0x%X\n", registers[i]);
            return -999.0;
        }
    }
    int16_t temp_raw = (data[1] << 8) | data[0];
    return (float)temp_raw / 256.0 + 25.0;
}

void lis2dtw12_read_data(float *x, float *y, float *z) {
    uint8_t registers[] = {0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D};
    uint8_t data[6] = {0};
    for (int i = 0; i < 6; i++) {
        int ret = i2c_write_read_dt(&dev_i2c_lis2dtw12, &registers[i], 1, &data[i], 1);
        if (ret != 0) {
            printk("Failed to read register 0x%X\n", registers[i]);
            return;
        }
    }
    
    int16_t x_raw = (data[1] << 8) | data[0];
    int16_t y_raw = (data[3] << 8) | data[2];
    int16_t z_raw = (data[5] << 8) | data[4];
    
    *x = x_raw * 0.061;
    *y = y_raw * 0.061;
    *z = z_raw * 0.061;
}