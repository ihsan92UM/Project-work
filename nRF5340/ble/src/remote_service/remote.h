
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/uuid.h>
#include <zephyr/bluetooth/gatt.h>
#include <zephyr/bluetooth/hci.h>

/** @brief UUID of the Button Service. 101 102 103 **/
#define BT_UUID_REMOTE_SERV_VAL \
	BT_UUID_128_ENCODE(0xe9ea0101, 0xe19b, 0x482d, 0x9293, 0xc7907585fc48)

/** @brief UUID of the Button Characteristic. **/
#define BT_UUID_REMOTE_BUTTON_CHRC_VAL \
	BT_UUID_128_ENCODE(0xe9ea0102, 0xe19b, 0x482d, 0x9293, 0xc7907585fc48)

/** @brief UUID of the Message Characteristic. **/ /*no need*/
#define BT_UUID_REMOTE_MESSAGE_CHRC_VAL \
	BT_UUID_128_ENCODE(0xe9ea0103, 0xe19b, 0x482d, 0x9293, 0xc7907585fc48)

//edit starts
/** @brief UUID of the BMP280 Sensor Service. 201 202 203**/
#define BT_UUID_REMOTE_SERV1_VAL \
	BT_UUID_128_ENCODE(0xe9ea0201, 0xe19b, 0x482d, 0x9293, 0xc7907585fc48)

	/** @brief UUID of the Temperature Characteristic. **/
#define BT_UUID_REMOTE_TEMPERATURE_CHRC_VAL \
	BT_UUID_128_ENCODE(0xe9ea0202, 0x44c2, 0x4f25, 0xb96d, 0x305992224e00)  

	/** @brief UUID of the Pressure Characteristic. **/
#define BT_UUID_REMOTE_PRESSURE_CHRC_VAL \
	BT_UUID_128_ENCODE(0xe9ea0203, 0xbb52, 0x4d3d, 0x8d48, 0xb2b646212bfa) 

	/** @brief UUID of the Humidity Characteristic. **/
#define BT_UUID_REMOTE_HUMIDITY_CHRC_VAL \
	BT_UUID_128_ENCODE(0xe9ea0204, 0x1312, 0x434b, 0xa010, 0x6cc3d0aa4290)


// edit ends

#define BT_UUID_REMOTE_SERVICE          BT_UUID_DECLARE_128(BT_UUID_REMOTE_SERV_VAL)
#define BT_UUID_REMOTE_BUTTON_CHRC 	    BT_UUID_DECLARE_128(BT_UUID_REMOTE_BUTTON_CHRC_VAL)
#define BT_UUID_REMOTE_MESSAGE_CHRC     BT_UUID_DECLARE_128(BT_UUID_REMOTE_MESSAGE_CHRC_VAL)

// edit starts
#define BT_UUID_REMOTE_SERVICE1          BT_UUID_DECLARE_128(BT_UUID_REMOTE_SERV1_VAL)
#define BT_UUID_REMOTE_TEMPERATURE_CHRC  BT_UUID_DECLARE_128(BT_UUID_REMOTE_TEMPERATURE_CHRC_VAL)
#define BT_UUID_REMOTE_PRESSURE_CHRC     BT_UUID_DECLARE_128(BT_UUID_REMOTE_PRESSURE_CHRC_VAL)
#define BT_UUID_REMOTE_HUMIDITY_CHRC     BT_UUID_DECLARE_128(BT_UUID_REMOTE_HUMIDITY_CHRC_VAL)
// edit ends



enum bt_button_notifications_enabled {
	BT_BUTTON_NOTIFICATIONS_ENABLED,
	BT_BUTTON_NOTIFICATIONS_DISABLED,
};

struct bt_remote_service_cb {
	void (*notif_changed)(enum bt_button_notifications_enabled status);
    void (*data_received)(struct bt_conn *conn, const uint8_t *const data, uint16_t len);
};



int send_button_notification(struct bt_conn *conn, uint8_t value, uint16_t length);
int send_temperature_notification(struct bt_conn *conn, float value);
int send_pressure_notification(struct bt_conn *conn, float value);
int send_humidity_notification(struct bt_conn *conn, float value);
void set_button_status(uint8_t btn_value);
void set_temperature_status(float tmp_value);
void set_pressure_status(float prs_value);
void set_humidity_status(float humi_value);
int bluetooth_init(struct bt_conn_cb *bt_cb, struct bt_remote_service_cb *remote_cb);
