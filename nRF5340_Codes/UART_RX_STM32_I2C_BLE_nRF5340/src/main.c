#include <zephyr/kernel.h>

#include <zephyr/sys/printk.h>
#include <zephyr/logging/log.h>
#include <dk_buttons_and_leds.h>  // for built in leds

#include <zephyr/device.h>
#include <zephyr/devicetree.h>

#include <zephyr/drivers/uart.h>
#include <zephyr/drivers/i2c.h>

#include "ble.h"
#include "sts40.h"
#include "lis2dtw12.h"
#include "uart_rx.h"


#define LOG_MODULE_NAME app
LOG_MODULE_REGISTER(LOG_MODULE_NAME);

#define RUN_STATUS_LED DK_LED1
#define CONN_STATUS_LED DK_LED2
#define RUN_INTERVAL 1000

static struct bt_conn *current_conn;

/* Declarations */
void on_connected(struct bt_conn *conn, uint8_t err);
void on_disconnected(struct bt_conn *conn, uint8_t reason);
void on_notif_changed(enum bt_button_notifications_enabled status);
void on_data_received(struct bt_conn *conn, const uint8_t *const data, uint16_t len);

struct bt_conn_cb bluetooth_callbacks = {
	.connected 		= on_connected,
	.disconnected 	= on_disconnected,
};
struct bt_remote_service_cb remote_callbacks = {
	.notif_changed = on_notif_changed,
    .data_received = on_data_received,
};

/* Callbacks */

void on_connected(struct bt_conn *conn, uint8_t err)
{
	if(err) {
		LOG_ERR("connection err: %d", err);
		return;
	}
	LOG_INF("Connected.");
	current_conn = bt_conn_ref(conn);
	dk_set_led_on(CONN_STATUS_LED);
}

void on_disconnected(struct bt_conn *conn, uint8_t reason)
{
	LOG_INF("Disconnected (reason: %d)", reason);
	dk_set_led_off(CONN_STATUS_LED);
	if(current_conn) {
		bt_conn_unref(current_conn);
		current_conn = NULL;
	}
}

void on_notif_changed(enum bt_button_notifications_enabled status)    
{
	if (status == BT_BUTTON_NOTIFICATIONS_ENABLED) {
		LOG_INF("Notifications enabled");
	}
	else {
		LOG_INF("Notificatons disabled");
	}
}

void on_data_received(struct bt_conn *conn, const uint8_t *const data, uint16_t len)
{
	uint8_t temp_str[len+1];
	memcpy(temp_str, data, len);
	temp_str[len] = 0x00;

	LOG_INF("Received data on conn %p. Len: %d", (void *)conn, len);
	LOG_INF("Data: %s", temp_str);
}



/* LED Configurations */

static void configure_dk_leds(void)
{
	int err;
	err = dk_leds_init();
	if (err)
	{
		LOG_ERR("Cannot init LEDs (err: %d)", err);
	}
}


void process_uart_data(void) {

        printk("ch0: %d\n", uart_rx_values[0]);
        set_ch0_status(uart_rx_values[0]);
        send_ch0_notification(current_conn, uart_rx_values[0]);
        
        printk("ch1: %d\n", uart_rx_values[1]);
        set_ch1_status(uart_rx_values[1]);
        send_ch1_notification(current_conn, uart_rx_values[1]);
        
        printk("ch2: %d\n", uart_rx_values[2]);
        set_ch2_status(uart_rx_values[2]);
        send_ch2_notification(current_conn, uart_rx_values[2]);
        
        printk("ch3: %d\n", uart_rx_values[3]);
        set_ch3_status(uart_rx_values[3]);
        send_ch3_notification(current_conn, uart_rx_values[3]);
        
        printk("ch4: %d\n", uart_rx_values[4]);
        set_ch4_status(uart_rx_values[4]);
        send_ch4_notification(current_conn, uart_rx_values[4]);
        
        printk("ch5: %d\n", uart_rx_values[5]);
        set_ch5_status(uart_rx_values[5]);
        send_ch5_notification(current_conn, uart_rx_values[5]);

}

void process_i2c_data(void) {

}


int main(void) {

        float x, y, z;
        int err;
        LOG_INF("Hello World! %s\n", CONFIG_BOARD);


        configure_dk_leds();
        uart_init();
        sts40_init();
        lis2dtw12_init();
        
        /*BLE setup*/
        err = bluetooth_init(&bluetooth_callbacks, &remote_callbacks);
        if (err) {
                LOG_ERR("bt_enable returned %d", err);
        }


        
        for (;;) {
                float temp1 = sts40_read_temperature();
                float temp2 = lis2dtw12_read_temperature();
                lis2dtw12_read_data(&x, &y, &z);
        
                printk("STS40 Temperature: %.2f°C\n", temp1);
                printk("LIS2DTW12 Temperature: %.2f°C\n", temp2);
                printk("Acceleration X: %.2f mg, Y: %.2f mg, Z: %.2f mg\n", x, y, z);

                set_temp1_status(temp1);
                send_temp1_notification(current_conn, temp1);
                
                set_temp2_status(temp2);
                send_temp2_notification(current_conn, temp2);
                
                set_acc_x_status(x);
                send_acc_x_notification(current_conn, x);
                
                set_acc_y_status(y);
                send_acc_y_notification(current_conn, y);
                
                set_acc_z_status(z);
                send_acc_z_notification(current_conn, z);

                process_uart_data();

                k_sleep(K_MSEC(RUN_INTERVAL));
            }
}




