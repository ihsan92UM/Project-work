#ifndef UART_RX_H
#define UART_RX_H

#include <zephyr/kernel.h>
#include <zephyr/drivers/uart.h>

#define RX_BUFFER_SIZE 15

void uart_init(void);

#endif 