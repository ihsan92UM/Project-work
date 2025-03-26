#ifndef STS40_H
#define STS40_H

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/i2c.h>

#define I2C1_NODE_STS40 DT_NODELABEL(sts40)

void sts40_init(void);
float sts40_read_temperature(void);

#endif