#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <dk_buttons_and_leds.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/sensor.h>

#include "remote.h"

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

/*Sensor handler*/

static const struct device *get_bme280_device(void)
{
	const struct device *const dev = DEVICE_DT_GET_ANY(bosch_bme280);

	if (dev == NULL) {
		/* No such node, or the node does not have status "okay". */
		printk("\nError: no device found.\n");
		return NULL;
	}

	if (!device_is_ready(dev)) {
		printk("\nError: Device \"%s\" is not ready; "
		       "check the driver initialization logs for errors.\n",
		       dev->name);
		return NULL;
	}

	printk("Found device \"%s\", getting sensor data\n", dev->name);
	return dev;
}

/*Button handler*/

void button_handler(uint32_t button_state, uint32_t has_changed)
{
	int button_pressed = 0;
	int err;
	if (has_changed & button_state)
	{
		switch (has_changed)
		{
			case DK_BTN1_MSK:
				button_pressed = 1;
				break;
			case DK_BTN2_MSK:
				button_pressed = 2;
				break;
			case DK_BTN3_MSK:
				button_pressed = 3;
				break;
			case DK_BTN4_MSK:
				button_pressed = 4;
				break;
			default:
				break;
		}
		LOG_INF("Button %d pressed.", button_pressed);
		set_button_status(button_pressed);
		err = send_button_notification(current_conn, button_pressed, 1);
		if (err) {
			LOG_ERR("couldn't send notification (err: %d)", err);
		}
	}
}

/* Configurations */

static void configure_dk_buttons_leds(void)
{
	int err;

	err = dk_buttons_init(button_handler);
	if (err)
	{
		LOG_ERR("Cannot init buttons (err: %d)", err);
	}
	err = dk_leds_init();
	if (err)
	{
		LOG_ERR("Cannot init LEDs (err: %d)", err);
	}
}



/* main */

void main(void)
{
	int err;
	int blink_status = 0;
	LOG_INF("Hello World! %s\n", CONFIG_BOARD);

	/*Button setup*/
	configure_dk_buttons_leds();

	/*Sensor setup*/
	const struct device *dev = get_bme280_device();
	if (dev == NULL) {
		return 0;
	}
	/*BLE setup*/
	err = bluetooth_init(&bluetooth_callbacks, &remote_callbacks);
	if (err) {
		LOG_ERR("bt_enable returned %d", err);
	}

	LOG_INF("Running...");
	for (;;) {
		dk_set_led(RUN_STATUS_LED, (blink_status++%2));

		struct sensor_value temp, press, humidity;
		sensor_sample_fetch(dev);
		sensor_channel_get(dev, SENSOR_CHAN_AMBIENT_TEMP, &temp);
		sensor_channel_get(dev, SENSOR_CHAN_PRESS, &press);
		sensor_channel_get(dev, SENSOR_CHAN_HUMIDITY, &humidity);

    // Combine integer and fractional parts
    float temp_value = temp.val1 + (temp.val2 / 1000000.0f);
    float press_value = press.val1 + (press.val2 / 1000000.0f);
    float humidity_value = humidity.val1 + (humidity.val2 / 1000000.0f);

    printk("temp: %.6f; press: %.6f; humidity: %.6f\n", temp_value, press_value, humidity_value);

    set_temperature_status(temp_value);
    send_temperature_notification(current_conn, temp_value);

    set_pressure_status(press_value);
    send_pressure_notification(current_conn, press_value);

    set_humidity_status(humidity_value);
    send_humidity_notification(current_conn, humidity_value);

	k_sleep(K_MSEC(RUN_INTERVAL));
	}
}


