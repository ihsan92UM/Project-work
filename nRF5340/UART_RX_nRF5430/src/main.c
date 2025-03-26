#include <zephyr/kernel.h>
#include <zephyr/drivers/uart.h>
#include <zephyr/sys/printk.h>


#define RX_BUFFER_SIZE 15  // Match the transmitter's buffer size

static uint8_t rxBuffer[RX_BUFFER_SIZE];  // Buffer for receiving UART data
static uint8_t rxIndex = 0;

static void uart_cb(const struct device *dev, struct uart_event *evt, void *user_data)
{
	switch (evt->type) {
	
	case UART_TX_DONE:
		// do something
		break;

	case UART_TX_ABORTED:
		// do something
		break;
		
                case UART_RX_RDY:
                // Copy received data into the buffer
                for (int i = 0; i < evt->data.rx.len; i++) {
                    // Check for buffer overflow
                    if (rxIndex >= RX_BUFFER_SIZE) {
                        rxIndex = 0;  // Reset buffer index to prevent overflow
                    }
            
                    // Store the received byte
                    rxBuffer[rxIndex++] = evt->data.rx.buf[evt->data.rx.offset + i];
            
                    // Check if a complete packet is received
                    if (rxIndex == RX_BUFFER_SIZE) {
                        // Verify the start and end bytes
                        if (rxBuffer[0] == 0xAA && rxBuffer[RX_BUFFER_SIZE - 2] == 0x55) {
                            // Reconstruct uint16_t values
                            uint16_t values[6];
                            for (int j = 0; j < 6; j++) {
                                values[j] = (rxBuffer[1 + 2 * j] << 8) | rxBuffer[2 + 2 * j];
                            }
            
                            // Print the received values
                            printk("Received: %d, %d, %d, %d, %d, %d\n",
                                   values[0], values[1], values[2],
                                   values[3], values[4], values[5]);
                        } else {
                            printk("Invalid packet format!\n");
                        }
            
                        // Reset the buffer index for the next packet
                        rxIndex = 0;
                    }
                }
                break;

	case UART_RX_BUF_REQUEST:
		// do something
		break;

	case UART_RX_BUF_RELEASED:
		// do something
		break;
		
	case UART_RX_DISABLED:
		// do something
                uart_rx_enable(dev, rxBuffer, sizeof(rxBuffer), 100);
		break;

	case UART_RX_STOPPED:
		// do something
		break;
		
	default:
		break;
	}
}


int main(void)
{
        const struct device *uart = DEVICE_DT_GET(DT_NODELABEL(uart0));

        if (!device_is_ready(uart)) {
        printk("UART device %s is not ready\n", uart->name);
        return -ENODEV;
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
		return -ENOSYS;
	}

        err = uart_callback_set(uart, uart_cb, NULL);
        if (err) {
                return err;
        }

         
        uart_rx_enable(uart, rxBuffer, sizeof(rxBuffer), 100);

        printk("Hello World!\n");
        return 0;
}
