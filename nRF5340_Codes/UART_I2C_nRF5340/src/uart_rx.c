// uart_rx.c
#include "uart_rx.h"
#include <zephyr/sys/printk.h>

static uint8_t rxBuffer[RX_BUFFER_SIZE];
static uint8_t rxIndex = 0;

static void uart_cb(const struct device *dev, struct uart_event *evt, void *user_data)
{
	switch (evt->type) {
	case UART_TX_DONE:
		break;
	case UART_TX_ABORTED:
		break;
	case UART_RX_RDY:
		for (int i = 0; i < evt->data.rx.len; i++) {
			if (rxIndex >= RX_BUFFER_SIZE) {
				rxIndex = 0;
			}
			rxBuffer[rxIndex++] = evt->data.rx.buf[evt->data.rx.offset + i];
			if (rxIndex == RX_BUFFER_SIZE) {
				if (rxBuffer[0] == 0xAA && rxBuffer[RX_BUFFER_SIZE - 2] == 0x55) {
					uint16_t values[6];
					for (int j = 0; j < 6; j++) {
						values[j] = (rxBuffer[1 + 2 * j] << 8) | rxBuffer[2 + 2 * j];
					}
					printk("Received: %d, %d, %d, %d, %d, %d\n",
					       values[0], values[1], values[2], values[3], values[4], values[5]);
				} else {
					printk("Invalid packet format!\n");
				}
				rxIndex = 0;
			}
		}
		break;
	case UART_RX_BUF_REQUEST:
		break;
	case UART_RX_BUF_RELEASED:
		break;
	case UART_RX_DISABLED:
		uart_rx_enable(dev, rxBuffer, sizeof(rxBuffer), 100);
		break;
	case UART_RX_STOPPED:
		break;
	default:
		break;
	}
}

void uart_init(void)
{
	const struct device *uart = DEVICE_DT_GET(DT_NODELABEL(uart0));
	if (!device_is_ready(uart)) {
		printk("UART device %s is not ready\n", uart->name);
		return;
	}

	const struct uart_config uart_cfg = {
		.baudrate = 115200,
		.parity = UART_CFG_PARITY_NONE,
		.stop_bits = UART_CFG_STOP_BITS_1,
		.data_bits = UART_CFG_DATA_BITS_8,
		.flow_ctrl = UART_CFG_FLOW_CTRL_NONE
	};

	int err = uart_configure(uart, &uart_cfg);
	if (err == -ENOSYS) {
		return;
	}

	err = uart_callback_set(uart, uart_cb, NULL);
	if (err) {
		return;
	}

	uart_rx_enable(uart, rxBuffer, sizeof(rxBuffer), 100);
}
