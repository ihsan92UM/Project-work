#include "ble.h"

#define LOG_MODULE_NAME remote
LOG_MODULE_REGISTER(LOG_MODULE_NAME);
#define DEVICE_NAME CONFIG_BT_DEVICE_NAME
#define DEVICE_NAME_LEN (sizeof(DEVICE_NAME)-1)

static K_SEM_DEFINE(bt_init_ok, 0, 1);

static float temp1_value = 0;
static float temp2_value = 0;

static float acc_x_value = 0;
static float acc_y_value = 0;
static float acc_z_value = 0;


static float ch0_value = 0;
static float ch1_value = 0;
static float ch2_value = 0;
static float ch3_value = 0;
static float ch4_value = 0;
static float ch5_value = 0;


enum bt_button_notifications_enabled notifications_enabled;
static struct bt_remote_service_cb remote_callbacks;

static const struct bt_data ad[] = {
    BT_DATA_BYTES(BT_DATA_FLAGS, (BT_LE_AD_GENERAL | BT_LE_AD_NO_BREDR)),
    BT_DATA(BT_DATA_NAME_COMPLETE, DEVICE_NAME, DEVICE_NAME_LEN)
};

static const struct bt_data sd[] = {
    BT_DATA_BYTES(BT_DATA_UUID128_ALL, BT_UUID_REMOTE_SERV_VAL),
};


// SEND TEXT to nRF5340
static ssize_t on_write(struct bt_conn *conn, const struct bt_gatt_attr *attr, const void *buf, uint16_t len, uint16_t offset, uint8_t flags);

//STS40
static ssize_t read_temp1_characteristic_cb(struct bt_conn *conn, const struct bt_gatt_attr *attr, void *buf, uint16_t len, uint16_t offset);
void temp1_chrc_ccc_cfg_changed(const struct bt_gatt_attr *attr, uint16_t value);

//LIS2DTW12
static ssize_t read_temp2_characteristic_cb(struct bt_conn *conn, const struct bt_gatt_attr *attr, void *buf, uint16_t len, uint16_t offset);
void temp2_chrc_ccc_cfg_changed(const struct bt_gatt_attr *attr, uint16_t value);

static ssize_t read_acc_x_characteristic_cb(struct bt_conn *conn, const struct bt_gatt_attr *attr, void *buf, uint16_t len, uint16_t offset);
void acc_x_chrc_ccc_cfg_changed(const struct bt_gatt_attr *attr, uint16_t value);

static ssize_t read_acc_y_characteristic_cb(struct bt_conn *conn, const struct bt_gatt_attr *attr, void *buf, uint16_t len, uint16_t offset);
void acc_y_chrc_ccc_cfg_changed(const struct bt_gatt_attr *attr, uint16_t value);

static ssize_t read_acc_z_characteristic_cb(struct bt_conn *conn, const struct bt_gatt_attr *attr, void *buf, uint16_t len, uint16_t offset);
void acc_z_chrc_ccc_cfg_changed(const struct bt_gatt_attr *attr, uint16_t value);


// Capacitance
static ssize_t read_ch0_characteristic_cb(struct bt_conn *conn, const struct bt_gatt_attr *attr, void *buf, uint16_t len, uint16_t offset);
void ch0_chrc_ccc_cfg_changed(const struct bt_gatt_attr *attr, uint16_t value);

static ssize_t read_ch1_characteristic_cb(struct bt_conn *conn, const struct bt_gatt_attr *attr, void *buf, uint16_t len, uint16_t offset);
void ch1_chrc_ccc_cfg_changed(const struct bt_gatt_attr *attr, uint16_t value);

static ssize_t read_ch2_characteristic_cb(struct bt_conn *conn, const struct bt_gatt_attr *attr, void *buf, uint16_t len, uint16_t offset);
void ch2_chrc_ccc_cfg_changed(const struct bt_gatt_attr *attr, uint16_t value);

static ssize_t read_ch3_characteristic_cb(struct bt_conn *conn, const struct bt_gatt_attr *attr, void *buf, uint16_t len, uint16_t offset);
void ch3_chrc_ccc_cfg_changed(const struct bt_gatt_attr *attr, uint16_t value);

static ssize_t read_ch4_characteristic_cb(struct bt_conn *conn, const struct bt_gatt_attr *attr, void *buf, uint16_t len, uint16_t offset);
void ch4_chrc_ccc_cfg_changed(const struct bt_gatt_attr *attr, uint16_t value);

static ssize_t read_ch5_characteristic_cb(struct bt_conn *conn, const struct bt_gatt_attr *attr, void *buf, uint16_t len, uint16_t offset);
void ch5_chrc_ccc_cfg_changed(const struct bt_gatt_attr *attr, uint16_t value);



//STS40
BT_GATT_SERVICE_DEFINE(remote_srv,
BT_GATT_PRIMARY_SERVICE(BT_UUID_REMOTE_SERVICE),
    BT_GATT_CHARACTERISTIC(BT_UUID_REMOTE_TEMP1_CHRC,
                    BT_GATT_CHRC_READ | BT_GATT_CHRC_NOTIFY,
                    BT_GATT_PERM_READ,
                    read_temp1_characteristic_cb, NULL, NULL),
    BT_GATT_CCC(temp1_chrc_ccc_cfg_changed, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),
    BT_GATT_CHARACTERISTIC(BT_UUID_REMOTE_MESSAGE_CHRC,
                    BT_GATT_CHRC_WRITE_WITHOUT_RESP,
                    BT_GATT_PERM_READ | BT_GATT_PERM_WRITE,
                    NULL, on_write, NULL),
);


// LIS2DTW12
BT_GATT_SERVICE_DEFINE(remote_service2,
    BT_GATT_PRIMARY_SERVICE(BT_UUID_REMOTE_SERVICE1),
        BT_GATT_CHARACTERISTIC(BT_UUID_REMOTE_TEMP2_CHRC,
                        BT_GATT_CHRC_READ | BT_GATT_CHRC_NOTIFY,
                        BT_GATT_PERM_READ,
                        read_temp2_characteristic_cb, NULL, NULL),
        BT_GATT_CCC(temp2_chrc_ccc_cfg_changed, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),
    BT_GATT_CHARACTERISTIC(BT_UUID_REMOTE_ACC_X_CHRC,
                        BT_GATT_CHRC_READ | BT_GATT_CHRC_NOTIFY,
                        BT_GATT_PERM_READ,
                        read_acc_x_characteristic_cb, NULL, NULL),
        BT_GATT_CCC(acc_x_chrc_ccc_cfg_changed, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),
    BT_GATT_CHARACTERISTIC(BT_UUID_REMOTE_ACC_Y_CHRC,
                        BT_GATT_CHRC_READ | BT_GATT_CHRC_NOTIFY,
                        BT_GATT_PERM_READ,
                        read_acc_y_characteristic_cb, NULL, NULL),
        BT_GATT_CCC(acc_y_chrc_ccc_cfg_changed, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),
    BT_GATT_CHARACTERISTIC(BT_UUID_REMOTE_ACC_Z_CHRC,
                        BT_GATT_CHRC_READ | BT_GATT_CHRC_NOTIFY,
                        BT_GATT_PERM_READ,
                        read_acc_z_characteristic_cb, NULL, NULL),
    BT_GATT_CCC(acc_z_chrc_ccc_cfg_changed, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),
    );

//Capacitance    
BT_GATT_SERVICE_DEFINE(remote_service3,
    BT_GATT_PRIMARY_SERVICE(BT_UUID_REMOTE_SERVICE1),
        BT_GATT_CHARACTERISTIC(BT_UUID_REMOTE_CH0_CHRC,
                        BT_GATT_CHRC_READ | BT_GATT_CHRC_NOTIFY,
                        BT_GATT_PERM_READ,
                        read_ch0_characteristic_cb, NULL, NULL),
        BT_GATT_CCC(ch0_chrc_ccc_cfg_changed, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),
    BT_GATT_CHARACTERISTIC(BT_UUID_REMOTE_CH1_CHRC,
                        BT_GATT_CHRC_READ | BT_GATT_CHRC_NOTIFY,
                        BT_GATT_PERM_READ,
                        read_ch1_characteristic_cb, NULL, NULL),
        BT_GATT_CCC(ch1_chrc_ccc_cfg_changed, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),
    BT_GATT_CHARACTERISTIC(BT_UUID_REMOTE_CH2_CHRC,
                        BT_GATT_CHRC_READ | BT_GATT_CHRC_NOTIFY,
                        BT_GATT_PERM_READ,
                        read_ch2_characteristic_cb, NULL, NULL),
        BT_GATT_CCC(ch2_chrc_ccc_cfg_changed, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),
    BT_GATT_CHARACTERISTIC(BT_UUID_REMOTE_CH3_CHRC,
                        BT_GATT_CHRC_READ | BT_GATT_CHRC_NOTIFY,
                        BT_GATT_PERM_READ,
                        read_ch3_characteristic_cb, NULL, NULL),
        BT_GATT_CCC(ch3_chrc_ccc_cfg_changed, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),
    BT_GATT_CHARACTERISTIC(BT_UUID_REMOTE_CH4_CHRC,
                        BT_GATT_CHRC_READ | BT_GATT_CHRC_NOTIFY,
                        BT_GATT_PERM_READ,
                        read_ch4_characteristic_cb, NULL, NULL),
        BT_GATT_CCC(ch4_chrc_ccc_cfg_changed, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),
    BT_GATT_CHARACTERISTIC(BT_UUID_REMOTE_CH5_CHRC,
                        BT_GATT_CHRC_READ | BT_GATT_CHRC_NOTIFY,
                        BT_GATT_PERM_READ,
                        read_ch5_characteristic_cb, NULL, NULL),
        BT_GATT_CCC(ch5_chrc_ccc_cfg_changed, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),
    
    );

   
// ends

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


// STS40
static ssize_t read_temp1_characteristic_cb(struct bt_conn *conn, const struct bt_gatt_attr *attr,
			 void *buf, uint16_t len, uint16_t offset)
{
	return bt_gatt_attr_read(conn, attr, buf, len, offset, &temp1_value,
				 sizeof(temp1_value));
}

void temp1_chrc_ccc_cfg_changed(const struct bt_gatt_attr *attr, uint16_t value)
{
    bool notif_enabled = (value == BT_GATT_CCC_NOTIFY);
    LOG_INF("Notifications %s", notif_enabled? "enabled":"disabled");

    notifications_enabled = notif_enabled? BT_BUTTON_NOTIFICATIONS_ENABLED:BT_BUTTON_NOTIFICATIONS_DISABLED;
    if (remote_callbacks.notif_changed) {
        remote_callbacks.notif_changed(notifications_enabled);
    }
}


// LIS2DTW12
static ssize_t read_temp2_characteristic_cb(struct bt_conn *conn, const struct bt_gatt_attr *attr,
			 void *buf, uint16_t len, uint16_t offset)
{
	return bt_gatt_attr_read(conn, attr, buf, len, offset, &temp2_value,
				 sizeof(temp2_value));
}

static ssize_t read_acc_x_characteristic_cb(struct bt_conn *conn, const struct bt_gatt_attr *attr,
			 void *buf, uint16_t len, uint16_t offset)
{
	return bt_gatt_attr_read(conn, attr, buf, len, offset, &acc_x_value,
				 sizeof(acc_x_value));
}

static ssize_t read_acc_y_characteristic_cb(struct bt_conn *conn, const struct bt_gatt_attr *attr,
			 void *buf, uint16_t len, uint16_t offset)
{
	return bt_gatt_attr_read(conn, attr, buf, len, offset, &acc_y_value,
				 sizeof(acc_y_value));
}

static ssize_t read_acc_z_characteristic_cb(struct bt_conn *conn, const struct bt_gatt_attr *attr,
    void *buf, uint16_t len, uint16_t offset)
{
return bt_gatt_attr_read(conn, attr, buf, len, offset, &acc_z_value,
        sizeof(acc_z_value));
}


void temp2_chrc_ccc_cfg_changed(const struct bt_gatt_attr *attr, uint16_t value)
{
    bool notif_enabled = (value == BT_GATT_CCC_NOTIFY);
    LOG_INF("Notifications %s", notif_enabled? "enabled":"disabled");

    notifications_enabled = notif_enabled? BT_BUTTON_NOTIFICATIONS_ENABLED:BT_BUTTON_NOTIFICATIONS_DISABLED;
    if (remote_callbacks.notif_changed) {
        remote_callbacks.notif_changed(notifications_enabled);
    }
}

void acc_x_chrc_ccc_cfg_changed(const struct bt_gatt_attr *attr, uint16_t value)
{
    bool notif_enabled = (value == BT_GATT_CCC_NOTIFY);
    LOG_INF("Notifications %s", notif_enabled? "enabled":"disabled");

    notifications_enabled = notif_enabled? BT_BUTTON_NOTIFICATIONS_ENABLED:BT_BUTTON_NOTIFICATIONS_DISABLED;
    if (remote_callbacks.notif_changed) {
        remote_callbacks.notif_changed(notifications_enabled);
    }
}

void acc_y_chrc_ccc_cfg_changed(const struct bt_gatt_attr *attr, uint16_t value)
{
    bool notif_enabled = (value == BT_GATT_CCC_NOTIFY);
    LOG_INF("Notifications %s", notif_enabled? "enabled":"disabled");

    notifications_enabled = notif_enabled? BT_BUTTON_NOTIFICATIONS_ENABLED:BT_BUTTON_NOTIFICATIONS_DISABLED;
    if (remote_callbacks.notif_changed) {
        remote_callbacks.notif_changed(notifications_enabled);
    }
}

void acc_z_chrc_ccc_cfg_changed(const struct bt_gatt_attr *attr, uint16_t value)
{
    bool notif_enabled = (value == BT_GATT_CCC_NOTIFY);
    LOG_INF("Notifications %s", notif_enabled? "enabled":"disabled");

    notifications_enabled = notif_enabled? BT_BUTTON_NOTIFICATIONS_ENABLED:BT_BUTTON_NOTIFICATIONS_DISABLED;
    if (remote_callbacks.notif_changed) {
        remote_callbacks.notif_changed(notifications_enabled);
    }
}


// Capacitance

static ssize_t read_ch0_characteristic_cb(struct bt_conn *conn, const struct bt_gatt_attr *attr,
    void *buf, uint16_t len, uint16_t offset)
{
return bt_gatt_attr_read(conn, attr, buf, len, offset, &ch0_value,
        sizeof(ch0_value));
}

static ssize_t read_ch1_characteristic_cb(struct bt_conn *conn, const struct bt_gatt_attr *attr,
    void *buf, uint16_t len, uint16_t offset)
{
return bt_gatt_attr_read(conn, attr, buf, len, offset, &ch1_value,
        sizeof(ch1_value));
}

static ssize_t read_ch2_characteristic_cb(struct bt_conn *conn, const struct bt_gatt_attr *attr,
    void *buf, uint16_t len, uint16_t offset)
{
return bt_gatt_attr_read(conn, attr, buf, len, offset, &ch2_value,
        sizeof(ch2_value));
}

static ssize_t read_ch3_characteristic_cb(struct bt_conn *conn, const struct bt_gatt_attr *attr,
    void *buf, uint16_t len, uint16_t offset)
{
return bt_gatt_attr_read(conn, attr, buf, len, offset, &ch3_value,
        sizeof(ch3_value));
}

static ssize_t read_ch4_characteristic_cb(struct bt_conn *conn, const struct bt_gatt_attr *attr,
    void *buf, uint16_t len, uint16_t offset)
{
return bt_gatt_attr_read(conn, attr, buf, len, offset, &ch4_value,
        sizeof(ch4_value));
}

static ssize_t read_ch5_characteristic_cb(struct bt_conn *conn, const struct bt_gatt_attr *attr,
    void *buf, uint16_t len, uint16_t offset)
{
return bt_gatt_attr_read(conn, attr, buf, len, offset, &ch5_value,
        sizeof(ch5_value));
}



void ch0_chrc_ccc_cfg_changed(const struct bt_gatt_attr *attr, uint16_t value)
{
bool notif_enabled = (value == BT_GATT_CCC_NOTIFY);
LOG_INF("Notifications %s", notif_enabled? "enabled":"disabled");

notifications_enabled = notif_enabled? BT_BUTTON_NOTIFICATIONS_ENABLED:BT_BUTTON_NOTIFICATIONS_DISABLED;
if (remote_callbacks.notif_changed) {
remote_callbacks.notif_changed(notifications_enabled);
}
}


void ch1_chrc_ccc_cfg_changed(const struct bt_gatt_attr *attr, uint16_t value)
{
bool notif_enabled = (value == BT_GATT_CCC_NOTIFY);
LOG_INF("Notifications %s", notif_enabled? "enabled":"disabled");

notifications_enabled = notif_enabled? BT_BUTTON_NOTIFICATIONS_ENABLED:BT_BUTTON_NOTIFICATIONS_DISABLED;
if (remote_callbacks.notif_changed) {
remote_callbacks.notif_changed(notifications_enabled);
}
}

void ch2_chrc_ccc_cfg_changed(const struct bt_gatt_attr *attr, uint16_t value)
{
bool notif_enabled = (value == BT_GATT_CCC_NOTIFY);
LOG_INF("Notifications %s", notif_enabled? "enabled":"disabled");

notifications_enabled = notif_enabled? BT_BUTTON_NOTIFICATIONS_ENABLED:BT_BUTTON_NOTIFICATIONS_DISABLED;
if (remote_callbacks.notif_changed) {
remote_callbacks.notif_changed(notifications_enabled);
}
}

void ch3_chrc_ccc_cfg_changed(const struct bt_gatt_attr *attr, uint16_t value)
{
bool notif_enabled = (value == BT_GATT_CCC_NOTIFY);
LOG_INF("Notifications %s", notif_enabled? "enabled":"disabled");

notifications_enabled = notif_enabled? BT_BUTTON_NOTIFICATIONS_ENABLED:BT_BUTTON_NOTIFICATIONS_DISABLED;
if (remote_callbacks.notif_changed) {
remote_callbacks.notif_changed(notifications_enabled);
}
}

void ch4_chrc_ccc_cfg_changed(const struct bt_gatt_attr *attr, uint16_t value)
{
bool notif_enabled = (value == BT_GATT_CCC_NOTIFY);
LOG_INF("Notifications %s", notif_enabled? "enabled":"disabled");

notifications_enabled = notif_enabled? BT_BUTTON_NOTIFICATIONS_ENABLED:BT_BUTTON_NOTIFICATIONS_DISABLED;
if (remote_callbacks.notif_changed) {
remote_callbacks.notif_changed(notifications_enabled);
}
}

void ch5_chrc_ccc_cfg_changed(const struct bt_gatt_attr *attr, uint16_t value)
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

//STS40
int send_temp1_notification(struct bt_conn *conn, float value)
{
    int err = 0;

    struct bt_gatt_notify_params params = {0};
    const struct bt_gatt_attr *attr = &remote_srv.attrs[1];

    params.attr = attr;
    params.data = &value;
    params.len = sizeof(value);
    params.func = on_sent;

    err = bt_gatt_notify_cb(conn, &params);

    return err;
}

/*edit started*/
//LIS2DTW12
int send_temp2_notification(struct bt_conn *conn, float value)
{
    int err = 0;

    struct bt_gatt_notify_params params = {0};
    const struct bt_gatt_attr *attr = &remote_service2.attrs[1];  //3

    params.attr = attr;
    params.data = &value;
    params.len = sizeof(value);
    params.func = on_sent;

    err = bt_gatt_notify_cb(conn, &params);

    return err;
}

int send_acc_x_notification(struct bt_conn *conn, float value)
{
    int err = 0;

    struct bt_gatt_notify_params params = {0};
    const struct bt_gatt_attr *attr = &remote_service2.attrs[3]; //5

    params.attr = attr;
    params.data = &value;
    params.len = sizeof(value);
    params.func = on_sent;

    err = bt_gatt_notify_cb(conn, &params);

    return err;
}

int send_acc_y_notification(struct bt_conn *conn, float value)
{
    int err = 0;

    struct bt_gatt_notify_params params = {0};
    const struct bt_gatt_attr *attr = &remote_service2.attrs[5];  //7

    params.attr = attr;
    params.data = &value;
    params.len = sizeof(value);
    params.func = on_sent;

    err = bt_gatt_notify_cb(conn, &params);

    return err;
}

int send_acc_z_notification(struct bt_conn *conn, float value)
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

// Capacitance

int send_ch0_notification(struct bt_conn *conn, float value)
{
    int err = 0;

    struct bt_gatt_notify_params params = {0};
    const struct bt_gatt_attr *attr = &remote_service3.attrs[1];  //1

    params.attr = attr;
    params.data = &value;
    params.len = sizeof(value);
    params.func = on_sent;

    err = bt_gatt_notify_cb(conn, &params);

    return err;
}

int send_ch1_notification(struct bt_conn *conn, float value)
{
    int err = 0;

    struct bt_gatt_notify_params params = {0};
    const struct bt_gatt_attr *attr = &remote_service3.attrs[3];  //3

    params.attr = attr;
    params.data = &value;
    params.len = sizeof(value);
    params.func = on_sent;

    err = bt_gatt_notify_cb(conn, &params);

    return err;
}


int send_ch2_notification(struct bt_conn *conn, float value)
{
    int err = 0;

    struct bt_gatt_notify_params params = {0};
    const struct bt_gatt_attr *attr = &remote_service3.attrs[5];  //5

    params.attr = attr;
    params.data = &value;
    params.len = sizeof(value);
    params.func = on_sent;

    err = bt_gatt_notify_cb(conn, &params);

    return err;
}


int send_ch3_notification(struct bt_conn *conn, float value)
{
    int err = 0;

    struct bt_gatt_notify_params params = {0};
    const struct bt_gatt_attr *attr = &remote_service3.attrs[7];  //7

    params.attr = attr;
    params.data = &value;
    params.len = sizeof(value);
    params.func = on_sent;

    err = bt_gatt_notify_cb(conn, &params);

    return err;
}


int send_ch4_notification(struct bt_conn *conn, float value)
{
    int err = 0;

    struct bt_gatt_notify_params params = {0};
    const struct bt_gatt_attr *attr = &remote_service3.attrs[9];  //9

    params.attr = attr;
    params.data = &value;
    params.len = sizeof(value);
    params.func = on_sent;

    err = bt_gatt_notify_cb(conn, &params);

    return err;
}


int send_ch5_notification(struct bt_conn *conn, float value)
{
    int err = 0;

    struct bt_gatt_notify_params params = {0};
    const struct bt_gatt_attr *attr = &remote_service3.attrs[11];  //11

    params.attr = attr;
    params.data = &value;
    params.len = sizeof(value);
    params.func = on_sent;

    err = bt_gatt_notify_cb(conn, &params);

    return err;
}





// STS40
void set_temp1_status(float tmp1_value)
{
    temp1_value = tmp1_value;
}

//LIS2DTW12
void set_temp2_status(float tmp2_value)
{
    temp2_value = tmp2_value;
}
void set_acc_x_status(float accx_value)
{
    acc_x_value = accx_value;
}

void set_acc_y_status(float accy_value)
{
    acc_y_value = accy_value;
}

void set_acc_z_status(float accz_value)
{
    acc_z_value = accz_value;
}

//Capacitance
void set_ch0_status(float ch0_val)
{
    ch0_value = ch0_val;
}

void set_ch1_status(float ch1_val)
{
    ch1_value = ch1_val;
}

void set_ch2_status(float ch2_val)
{
    ch2_value = ch2_val;
}

void set_ch3_status(float ch3_val)
{
    ch3_value = ch3_val;
}

void set_ch4_status(float ch4_val)
{
    ch4_value = ch4_val;
}

void set_ch5_status(float ch5_val)
{
    ch5_value = ch5_val;
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