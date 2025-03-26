#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/uuid.h>
#include <zephyr/bluetooth/gatt.h>
#include <zephyr/bluetooth/hci.h>



//SEND TEXT TO nRF5340
/** @brief UUID of the Message Characteristic. **/ /*no need*/
#define BT_UUID_REMOTE_MESSAGE_CHRC_VAL \
	BT_UUID_128_ENCODE(0xe9ea0001, 0xe19b, 0x482d, 0x9293, 0xc7907585fc48)


//STS40
/** @brief UUID of the STS40 temp1 Service. 101 102 103 **/
#define BT_UUID_REMOTE_SERV_VAL \
	BT_UUID_128_ENCODE(0xe9ea0101, 0xe19b, 0x482d, 0x9293, 0xc7907585fc48)

/** @brief UUID of the STS40 temp1 Characteristic. **/
#define BT_UUID_REMOTE_TEMP1_CHRC_VAL \
	BT_UUID_128_ENCODE(0xe9ea0102, 0xe19b, 0x482d, 0x9293, 0xc7907585fc48)


//LIS2DTW12
/** @brief UUID of the LIS2DTW12 Sensor Service. 201 202 203**/
#define BT_UUID_REMOTE_SERV1_VAL \
	BT_UUID_128_ENCODE(0xe9ea0201, 0xe19b, 0x482d, 0x9293, 0xc7907585fc48)

	/** @brief UUID of the Temperature Characteristic. **/
#define BT_UUID_REMOTE_TEMP2_CHRC_VAL \
	BT_UUID_128_ENCODE(0xe9ea0202, 0x44c2, 0x4f25, 0xb96d, 0x305992224e00)  

	/** @brief UUID of the ACC_X Characteristic. **/
#define BT_UUID_REMOTE_ACC_X_CHRC_VAL \
	BT_UUID_128_ENCODE(0xe9ea0203, 0xbb52, 0x4d3d, 0x8d48, 0xb2b646212bfa) 

	/** @brief UUID of the ACC_Y Characteristic. **/
#define BT_UUID_REMOTE_ACC_Y_CHRC_VAL \
	BT_UUID_128_ENCODE(0xe9ea0204, 0x1312, 0x434b, 0xa010, 0x6cc3d0aa4290)
	
	/** @brief UUID of the ACC_Z Characteristic. **/
#define BT_UUID_REMOTE_ACC_Z_CHRC_VAL \
	BT_UUID_128_ENCODE(0xe9ea0205, 0x1312, 0x434b, 0xa010, 0x6cc3d0aa4290)


// Capacitance measurements from STM32U5
/** @brief UUID of the Capacitance Service. 201 202 203**/
#define BT_UUID_REMOTE_SERV2_VAL \
	BT_UUID_128_ENCODE(0xe9ea0301, 0xe19b, 0x482d, 0x9293, 0xc7907585fc48)

/** @brief UUID of the Humidity Characteristic. **/
#define BT_UUID_REMOTE_CH0_CHRC_VAL \
	BT_UUID_128_ENCODE(0xe9ea0302, 0x1312, 0x434b, 0xa010, 0x6cc3d0aa4290)

/** @brief UUID of the Humidity Characteristic. **/
#define BT_UUID_REMOTE_CH1_CHRC_VAL \
	BT_UUID_128_ENCODE(0xe9ea0303, 0x1312, 0x434b, 0xa010, 0x6cc3d0aa4290)

/** @brief UUID of the Humidity Characteristic. **/
#define BT_UUID_REMOTE_CH2_CHRC_VAL \
	BT_UUID_128_ENCODE(0xe9ea0304, 0x1312, 0x434b, 0xa010, 0x6cc3d0aa4290)

/** @brief UUID of the Humidity Characteristic. **/
#define BT_UUID_REMOTE_CH3_CHRC_VAL \
	BT_UUID_128_ENCODE(0xe9ea0305, 0x1312, 0x434b, 0xa010, 0x6cc3d0aa4290)

/** @brief UUID of the Humidity Characteristic. **/
#define BT_UUID_REMOTE_CH4_CHRC_VAL \
	BT_UUID_128_ENCODE(0xe9ea0306, 0x1312, 0x434b, 0xa010, 0x6cc3d0aa4290)

/** @brief UUID of the Humidity Characteristic. **/
#define BT_UUID_REMOTE_CH5_CHRC_VAL \
	BT_UUID_128_ENCODE(0xe9ea0307, 0x1312, 0x434b, 0xa010, 0x6cc3d0aa4290)



//SEND TEXT TO nRF5340
#define BT_UUID_REMOTE_MESSAGE_CHRC       BT_UUID_DECLARE_128(BT_UUID_REMOTE_MESSAGE_CHRC_VAL)

//STS40
#define BT_UUID_REMOTE_SERVICE            BT_UUID_DECLARE_128(BT_UUID_REMOTE_SERV_VAL)

#define BT_UUID_REMOTE_TEMP1_CHRC         BT_UUID_DECLARE_128(BT_UUID_REMOTE_TEMP1_CHRC_VAL)


//LIS2DTW12
#define BT_UUID_REMOTE_SERVICE1            BT_UUID_DECLARE_128(BT_UUID_REMOTE_SERV1_VAL)

#define BT_UUID_REMOTE_TEMP2_CHRC          BT_UUID_DECLARE_128(BT_UUID_REMOTE_TEMP2_CHRC_VAL)
#define BT_UUID_REMOTE_ACC_X_CHRC  		   BT_UUID_DECLARE_128(BT_UUID_REMOTE_ACC_X_CHRC_VAL)
#define BT_UUID_REMOTE_ACC_Y_CHRC          BT_UUID_DECLARE_128(BT_UUID_REMOTE_ACC_Y_CHRC_VAL)
#define BT_UUID_REMOTE_ACC_Z_CHRC          BT_UUID_DECLARE_128(BT_UUID_REMOTE_ACC_Z_CHRC_VAL)

//Capacitance
#define BT_UUID_REMOTE_SERVICE2            BT_UUID_DECLARE_128(BT_UUID_REMOTE_SERV2_VAL)

#define BT_UUID_REMOTE_CH0_CHRC            BT_UUID_DECLARE_128(BT_UUID_REMOTE_CH0_CHRC_VAL)
#define BT_UUID_REMOTE_CH1_CHRC            BT_UUID_DECLARE_128(BT_UUID_REMOTE_CH1_CHRC_VAL)
#define BT_UUID_REMOTE_CH2_CHRC            BT_UUID_DECLARE_128(BT_UUID_REMOTE_CH2_CHRC_VAL)
#define BT_UUID_REMOTE_CH3_CHRC            BT_UUID_DECLARE_128(BT_UUID_REMOTE_CH3_CHRC_VAL)
#define BT_UUID_REMOTE_CH4_CHRC            BT_UUID_DECLARE_128(BT_UUID_REMOTE_CH4_CHRC_VAL)
#define BT_UUID_REMOTE_CH5_CHRC            BT_UUID_DECLARE_128(BT_UUID_REMOTE_CH5_CHRC_VAL)




enum bt_button_notifications_enabled {
	BT_BUTTON_NOTIFICATIONS_ENABLED,
	BT_BUTTON_NOTIFICATIONS_DISABLED,
};

struct bt_remote_service_cb {
	void (*notif_changed)(enum bt_button_notifications_enabled status);
    void (*data_received)(struct bt_conn *conn, const uint8_t *const data, uint16_t len);
};


//edited

int bluetooth_init(struct bt_conn_cb *bt_cb, struct bt_remote_service_cb *remote_cb);

int send_temp1_notification(struct bt_conn *conn, float value);

int send_temp2_notification(struct bt_conn *conn, float value);
int send_acc_x_notification(struct bt_conn *conn, float value);
int send_acc_y_notification(struct bt_conn *conn, float value);
int send_acc_z_notification(struct bt_conn *conn, float value);

void set_temp1_status(float tmp1_value);

void set_temp2_status(float tmp2_value);
void set_acc_x_status(float accx_value);
void set_acc_y_status(float accy_value);
void set_acc_z_status(float accz_value);


int send_ch0_notification(struct bt_conn *conn, float value);
int send_ch1_notification(struct bt_conn *conn, float value);
int send_ch2_notification(struct bt_conn *conn, float value);
int send_ch3_notification(struct bt_conn *conn, float value);
int send_ch4_notification(struct bt_conn *conn, float value);
int send_ch5_notification(struct bt_conn *conn, float value);

void set_ch0_status(float ch0_val);
void set_ch1_status(float ch1_val);
void set_ch2_status(float ch2_val);
void set_ch3_status(float ch3_val);
void set_ch4_status(float ch4_val);
void set_ch5_status(float ch5_val);

//edited