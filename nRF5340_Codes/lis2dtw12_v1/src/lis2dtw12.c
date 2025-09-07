#include "lis2dtw12.h"
#include <zephyr/sys/printk.h>

static const struct i2c_dt_spec lis2dtw12 = I2C_DT_SPEC_GET(LIS2DTW12_I2C);

int lis2dtw12_init(void) {
    if (!device_is_ready(lis2dtw12.bus)) {
        printk("I2C bus %s is not ready!\n", lis2dtw12.bus->name);
        return -1;
    }
    
    // Configure sensor
    uint8_t ctrl1[] = {0x20, 0x75};  // Set ODR and mode
    if (i2c_write_dt(&lis2dtw12, ctrl1, sizeof(ctrl1)) != 0) {
        printk("Failed to configure CTRL1 register\n");
        return -1;
    }

    uint8_t ctrl6[] = {0x25, 0x04};  // Set filter, FS, and low noise mode
    if (i2c_write_dt(&lis2dtw12, ctrl6, sizeof(ctrl6)) != 0) {
        printk("Failed to configure CTRL6 register\n");
        return -1;
    }

    printk("LIS2DTW12 initialized successfully!\n");
    return 0;
}

int lis2dtw12_read_who_am_i(uint8_t *who_am_i) {
    uint8_t reg = 0x0F;
    return i2c_write_read_dt(&lis2dtw12, &reg, sizeof(reg), who_am_i, sizeof(*who_am_i));
}

int lis2dtw12_read_temperature(float *temperature) {
    uint8_t reg = 0x0D;
    uint8_t temp_data[2] = {0};

    if (i2c_write_read_dt(&lis2dtw12, &reg, sizeof(reg), &temp_data[0], sizeof(temp_data[0])) != 0) {
        return -1;
    }

    reg = 0x0E;
    if (i2c_write_read_dt(&lis2dtw12, &reg, sizeof(reg), &temp_data[1], sizeof(temp_data[1])) != 0) {
        return -1;
    }

    int16_t temp_raw = (temp_data[1] << 8) | temp_data[0];
    *temperature = (float)temp_raw / 256.0 + 25.0;

    return 0;
}

int lis2dtw12_read_acceleration(float *x, float *y, float *z) {
    uint8_t reg = 0x28;
    uint8_t acc_data[6] = {0};

    if (i2c_write_read_dt(&lis2dtw12, &reg, sizeof(reg), acc_data, sizeof(acc_data)) != 0) {
        return -1;
    }

    int16_t x_raw = (acc_data[1] << 8) | acc_data[0];
    int16_t y_raw = (acc_data[3] << 8) | acc_data[2];
    int16_t z_raw = (acc_data[5] << 8) | acc_data[4];

    *x = x_raw * 0.061;  // ±2g mode, 0.061 mg/LSB
    *y = y_raw * 0.061;
    *z = z_raw * 0.061;

    return 0;
}
