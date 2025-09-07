#include "remote.h"

#define LOG_MODULE_NAME remote
LOG_MODULE_REGISTER(LOG_MODULE_NAME);
#define DEVICE_NAME CONFIG_BT_DEVICE_NAME
#define DEVICE_NAME_LEN (sizeof(DEVICE_NAME)-1)

static K_SEM_DEFINE(bt_init_ok, 0, 1);

static uint8_t button_value = 0;
// edit started
static uint32_t temperature_value = 0;
static uint32_t pressure_value = 0;
static uint32_t humidity_value = 0;
//

enum bt_button_notifications_enabled notifications_enabled;
static struct bt_remote_service_cb remote_callbacks;

static const struct bt_data ad[] = {
    BT_DATA_BYTES(BT_DATA_FLAGS, (BT_LE_AD_GENERAL | BT_LE_AD_NO_BREDR)),
    BT_DATA(BT_DATA_NAME_COMPLETE, DEVICE_NAME, DEVICE_NAME_LEN)
};

static const struct bt_data sd[] = {
    BT_DATA_BYTES(BT_DATA_UUID128_ALL, BT_UUID_REMOTE_SERV_VAL),
};


/* Button Declarations */

static ssize_t read_button_characteristic_cb(struct bt_conn *conn, const struct bt_gatt_attr *attr, void *buf, uint16_t len, uint16_t offset);
void button_chrc_ccc_cfg_changed(const struct bt_gatt_attr *attr, uint16_t value);
static ssize_t on_write(struct bt_conn *conn, const struct bt_gatt_attr *attr, const void *buf, uint16_t len, uint16_t offset, uint8_t flags);


/*Sensor declarations*/
static ssize_t read_temperature_characteristic_cb(struct bt_conn *conn, const struct bt_gatt_attr *attr, void *buf, uint16_t len, uint16_t offset);
void temperature_chrc_ccc_cfg_changed(const struct bt_gatt_attr *attr, uint16_t value);

static ssize_t read_pressure_characteristic_cb(struct bt_conn *conn, const struct bt_gatt_attr *attr, void *buf, uint16_t len, uint16_t offset);
void pressure_chrc_ccc_cfg_changed(const struct bt_gatt_attr *attr, uint16_t value);

static ssize_t read_humidity_characteristic_cb(struct bt_conn *conn, const struct bt_gatt_attr *attr, void *buf, uint16_t len, uint16_t offset);
void humidity_chrc_ccc_cfg_changed(const struct bt_gatt_attr *attr, uint16_t value);



BT_GATT_SERVICE_DEFINE(remote_srv,
BT_GATT_PRIMARY_SERVICE(BT_UUID_REMOTE_SERVICE),
    BT_GATT_CHARACTERISTIC(BT_UUID_REMOTE_BUTTON_CHRC,
                    BT_GATT_CHRC_READ | BT_GATT_CHRC_NOTIFY,
                    BT_GATT_PERM_READ,
                    read_button_characteristic_cb, NULL, NULL),
    BT_GATT_CCC(button_chrc_ccc_cfg_changed, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),
    BT_GATT_CHARACTERISTIC(BT_UUID_REMOTE_MESSAGE_CHRC,
                    BT_GATT_CHRC_WRITE_WITHOUT_RESP,
                    BT_GATT_PERM_READ | BT_GATT_PERM_WRITE,
                    NULL, on_write, NULL),
);


// edit start
BT_GATT_SERVICE_DEFINE(remote_service2,
BT_GATT_PRIMARY_SERVICE(BT_UUID_REMOTE_SERVICE1),
    BT_GATT_CHARACTERISTIC(BT_UUID_REMOTE_TEMPERATURE_CHRC,
                    BT_GATT_CHRC_READ | BT_GATT_CHRC_NOTIFY,
                    BT_GATT_PERM_READ,
                    read_temperature_characteristic_cb, NULL, NULL),
    BT_GATT_CCC(temperature_chrc_ccc_cfg_changed, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),
BT_GATT_CHARACTERISTIC(BT_UUID_REMOTE_PRESSURE_CHRC,
                    BT_GATT_CHRC_READ | BT_GATT_CHRC_NOTIFY,
                    BT_GATT_PERM_READ,
                    read_pressure_characteristic_cb, NULL, NULL),
    BT_GATT_CCC(pressure_chrc_ccc_cfg_changed, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),
BT_GATT_CHARACTERISTIC(BT_UUID_REMOTE_HUMIDITY_CHRC,
                    BT_GATT_CHRC_READ | BT_GATT_CHRC_NOTIFY,
                    BT_GATT_PERM_READ,
                    read_humidity_characteristic_cb, NULL, NULL),
    BT_GATT_CCC(humidity_chrc_ccc_cfg_changed, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),

);


// edit ends

/* Callbacks */

static ssize_t on_write(struct bt_conn *conn,
			  const struct bt_gatt_attr *attr,
			  const void *buf,
			  uint16_t len,
			  uint16_t offset,
			  uint8_t flags)
{
	LOG_INF("Received data, handle %d, conn %p",
		attr->handle, (void *)conn);

	if (remote_callbacks.data_received) {
		remote_callbacks.data_received(conn, buf, len);
    }
	return len;
}

void on_sent(struct bt_conn *conn, void *user_data)
{
    ARG_UNUSED(user_data);
    LOG_INF("Notification sent on connection %p", (void *)conn);
}

static ssize_t read_button_characteristic_cb(struct bt_conn *conn, const struct bt_gatt_attr *attr,
			 void *buf, uint16_t len, uint16_t offset)
{
	return bt_gatt_attr_read(conn, attr, buf, len, offset, &button_value,
				 sizeof(button_value));
}

void button_chrc_ccc_cfg_changed(const struct bt_gatt_attr *attr, uint16_t value)
{
    bool notif_enabled = (value == BT_GATT_CCC_NOTIFY);
    LOG_INF("Notifications %s", notif_enabled? "enabled":"disabled");

    notifications_enabled = notif_enabled? BT_BUTTON_NOTIFICATIONS_ENABLED:BT_BUTTON_NOTIFICATIONS_DISABLED;
    if (remote_callbacks.notif_changed) {
        remote_callbacks.notif_changed(notifications_enabled);
    }
}

////////////////////////////////
/*edit start*/
static ssize_t read_temperature_characteristic_cb(struct bt_conn *conn, const struct bt_gatt_attr *attr,
			 void *buf, uint16_t len, uint16_t offset)
{
	return bt_gatt_attr_read(conn, attr, buf, len, offset, &temperature_value,
				 sizeof(temperature_value));
}

static ssize_t read_pressure_characteristic_cb(struct bt_conn *conn, const struct bt_gatt_attr *attr,
			 void *buf, uint16_t len, uint16_t offset)
{
	return bt_gatt_attr_read(conn, attr, buf, len, offset, &pressure_value,
				 sizeof(pressure_value));
}

static ssize_t read_humidity_characteristic_cb(struct bt_conn *conn, const struct bt_gatt_attr *attr,
			 void *buf, uint16_t len, uint16_t offset)
{
	return bt_gatt_attr_read(conn, attr, buf, len, offset, &humidity_value,
				 sizeof(humidity_value));
}

void temperature_chrc_ccc_cfg_changed(const struct bt_gatt_attr *attr, uint16_t value)
{
    bool notif_enabled = (value == BT_GATT_CCC_NOTIFY);
    LOG_INF("Notifications %s", notif_enabled? "enabled":"disabled");

    notifications_enabled = notif_enabled? BT_BUTTON_NOTIFICATIONS_ENABLED:BT_BUTTON_NOTIFICATIONS_DISABLED;
    if (remote_callbacks.notif_changed) {
        remote_callbacks.notif_changed(notifications_enabled);
    }

    if (value == BT_GATT_CCC_NOTIFY) {
        printk("Temperature notifications enabled\n");
    } else {
        printk("Temperature notifications disabled\n");
    }
}

void pressure_chrc_ccc_cfg_changed(const struct bt_gatt_attr *attr, uint16_t value)
{
    bool notif_enabled = (value == BT_GATT_CCC_NOTIFY);
    LOG_INF("Notifications %s", notif_enabled? "enabled":"disabled");

    notifications_enabled = notif_enabled? BT_BUTTON_NOTIFICATIONS_ENABLED:BT_BUTTON_NOTIFICATIONS_DISABLED;
    if (remote_callbacks.notif_changed) {
        remote_callbacks.notif_changed(notifications_enabled);
    }
}

void humidity_chrc_ccc_cfg_changed(const struct bt_gatt_attr *attr, uint16_t value)
{
    bool notif_enabled = (value == BT_GATT_CCC_NOTIFY);
    LOG_INF("Notifications %s", notif_enabled? "enabled":"disabled");

    notifications_enabled = notif_enabled? BT_BUTTON_NOTIFICATIONS_ENABLED:BT_BUTTON_NOTIFICATIONS_DISABLED;
    if (remote_callbacks.notif_changed) {
        remote_callbacks.notif_changed(notifications_enabled);
    }
}


/*edit ends*/

void bt_ready(int err)
{
    if (err) {
        LOG_ERR("bt_ready returned %d", err);
    }
    k_sem_give(&bt_init_ok);
}

/* Custom functions */

int send_button_notification(struct bt_conn *conn, uint8_t value, uint16_t length)
{
    int err = 0;

    struct bt_gatt_notify_params params = {0};
    const struct bt_gatt_attr *attr = &remote_srv.attrs[1];

    params.attr = attr;
    params.data = &value;
    params.len = length;
    params.func = on_sent;

    err = bt_gatt_notify_cb(conn, &params);

    return err;
}

/*edit started*/
int send_temperature_notification(struct bt_conn *conn, float value)
{
    int err = 0;

    struct bt_gatt_notify_params params = {0};
    const struct bt_gatt_attr *attr = &remote_service2.attrs[3];  //3

    params.attr = attr;
    params.data = &value;
    params.len = sizeof(value);
    params.func = on_sent;

    err = bt_gatt_notify_cb(conn, &params);

    return err;
}

int send_pressure_notification(struct bt_conn *conn, float value)
{
    int err = 0;

    struct bt_gatt_notify_params params = {0};
    const struct bt_gatt_attr *attr = &remote_service2.attrs[5]; //5

    params.attr = attr;
    params.data = &value;
    params.len = sizeof(value);
    params.func = on_sent;

    err = bt_gatt_notify_cb(conn, &params);

    return err;
}
int send_humidity_notification(struct bt_conn *conn, float value)
{
    int err = 0;

    struct bt_gatt_notify_params params = {0};
    const struct bt_gatt_attr *attr = &remote_service2.attrs[7];  //7

    params.attr = attr;
    params.data = &value;
    params.len = sizeof(value);
    params.func = on_sent;

    err = bt_gatt_notify_cb(conn, &params);

    return err;
}

void set_button_status(uint8_t btn_value)
{
    button_value = btn_value;
}

/*edit started*/
void set_temperature_status(float tmp_value)
{
    temperature_value = tmp_value;
}
void set_pressure_status(float prs_value)
{
    pressure_value = prs_value;
}
void set_humidity_status(float humi_value)
{
    humidity_value = humi_value;
}
/*edit ended*/

int bluetooth_init(struct bt_conn_cb *bt_cb, struct bt_remote_service_cb *remote_cb)
{
    int err;
    LOG_INF("Initializing bluetooth...");

    if (bt_cb == NULL || remote_cb == NULL) {
        return -NRFX_ERROR_NULL;
    }

    bt_conn_cb_register(bt_cb);
    remote_callbacks.notif_changed = remote_cb->notif_changed;

    err = bt_enable(bt_ready);
    if (err) {
        LOG_ERR("bt_enable returned %d", err);
        return err;
    }
    
    k_sem_take(&bt_init_ok, K_FOREVER);

    err = bt_le_adv_start(BT_LE_ADV_CONN, ad, ARRAY_SIZE(ad), sd, ARRAY_SIZE(sd));
    if (err) {
        LOG_ERR("Couldn't start advertising (err = %d)", err);
        return err;
    }



    return err;
}