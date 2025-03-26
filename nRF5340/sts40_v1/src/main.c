#include <zephyr/kernel.h>
#include "sts40.h"

/* Define the STS40 device */
static const struct sts40_data sts40_dev = {
    .i2c = I2C_DT_SPEC_GET(I2C1_NODE),
};

int main(void) {
    float temperature;

    /* Initialize the STS40 sensor */
    if (sts40_init(&sts40_dev) != 0) {
        return -1;
    }

    while (1) {
        /* Read temperature from the STS40 sensor */
        if (sts40_read_temperature(&sts40_dev, &temperature) == 0) {
            printk("Temperature: %.2f °C\n", temperature);
        }

        /* Wait for 1 second before the next read */
        k_msleep(1000);
    }

    return 0;
}