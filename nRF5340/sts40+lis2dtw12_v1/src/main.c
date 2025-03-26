#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/i2c.h>

#include "sts40.h"
#include "lis2dtw12.h"

int main(void) {
    sts40_init();
    lis2dtw12_init();

    while (1) {
        float temp1 = sts40_read_temperature();
        float temp2 = lis2dtw12_read_temperature();
        float x, y, z;
        lis2dtw12_read_data(&x, &y, &z);

        printk("STS40 Temperature: %.2f°C\n", temp1);
        printk("LIS2DTW12 Temperature: %.2f°C\n", temp2);
        printk("Acceleration X: %.2f mg, Y: %.2f mg, Z: %.2f mg\n", x, y, z);
        k_msleep(1000);
    }
}