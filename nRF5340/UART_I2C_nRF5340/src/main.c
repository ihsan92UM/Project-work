#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>

#include <zephyr/drivers/uart.h>
#include <zephyr/drivers/i2c.h>


#include "sts40.h"
#include "lis2dtw12.h"
#include "uart_rx.h"

void process_uart_data(void) {

        // Example: Send UART data via Bluetooth (you need to implement this)
        // send_to_bluetooth(rxBuffer, RX_BUFFER_SIZE);
}

void process_i2c_data(void) {
        // Example: Send I2C data via Bluetooth (you need to implement this)
        // send_to_bluetooth(i2cBuffer, I2C_BUFFER_SIZE);
}


int main(void) {

        uart_init();
        sts40_init();
        lis2dtw12_init();

        float x, y, z;

        while (1) {
                float temp1 = sts40_read_temperature();
                float temp2 = lis2dtw12_read_temperature();
                lis2dtw12_read_data(&x, &y, &z);
        
                printk("STS40 Temperature: %.2f°C\n", temp1);
                printk("LIS2DTW12 Temperature: %.2f°C\n", temp2);
                printk("Acceleration X: %.2f mg, Y: %.2f mg, Z: %.2f mg\n", x, y, z);
                k_msleep(1000);
            }
}