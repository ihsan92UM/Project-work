#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include "lis2dtw12.h"

int main(void) {
    if (lis2dtw12_init() != 0) {
        printk("Failed to initialize LIS2DTW12 sensor!\n");
        return -1;
    }

    uint8_t who_am_i;
    if (lis2dtw12_read_who_am_i(&who_am_i) == 0) {
        printk("LIS2DTW12 WHO_AM_I: 0x%X\n", who_am_i);
    } else {
        printk("Failed to read WHO_AM_I register\n");
    }

    float temperature;
    if (lis2dtw12_read_temperature(&temperature) == 0) {
        printk("Temperature: %.2f °C\n", temperature);
    } else {
        printk("Failed to read temperature\n");
    }


    float x, y, z;
    while(1){
        
        if (lis2dtw12_read_acceleration(&x, &y, &z) == 0) {
            printk("Acceleration X: %.2f mg, Y: %.2f mg, Z: %.2f mg\n", x, y, z);
        } else {
            printk("Failed to read acceleration\n");
        }

        k_msleep(100);
    }


    return 0;
}
