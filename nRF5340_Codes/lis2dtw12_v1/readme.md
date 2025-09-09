# Programming Guide - LIS2DTW12_v1

## Requirements

#### Hardware
* [nRF5340 DK](https://www.nordicsemi.com/Products/Development-hardware/nRF5340-DK)
* nRF5340 based Custom Board v1 or v2

#### Software
* lis2dtw12_v1

# Sensor Connection
> [!NOTE]  
> This setup is only for nRF5340Dk , Custom boards already contains LIS2DTW12 Accelerometer sensor.


*   Connect `3V3` to `VDD`
*   Connect `GND` to `GND`
*   Connect `SCL` to `P1.03`
*   Connect `SDA` to `P1.02`


![Accelerometer](/Graphics/IMU_Sensor.png)



# Programming 
Import the ```lis2dtw12_v1``` folder into workspace by clicking ```add folder``` from the ```Explorer``` tab.
Once imported the folder, goto ```nRF Connect``` tab and click on ```Add Build Configuration``` under application menu. Once ```build``` is done, ```flash``` the programme and open ```VCOM1``` port and view the result.

# Debugging

Alternatively, Debugging can be done, simply turn on the Debugging optimization flag in the ```build configuration``` inside ```Opitimization level``` option.

# Code Explanation

The folder is structured as , `src` folder contains the `main.c` file and configuration file `lis2dtw12.c` and `lis2dtw12.h` for sensor configuration. This modularization helps reducing code congestion in `main.c` file.

```
lis2dtw12_v1/
├── src/
│   ├── main.c
│   ├── lis2dtw12.c
│   └── lis2dtw12.h
├── CMakeLists.txt
├── nrf5340dk_nrf5340_cpuapp.overlay
├── prj.conf
└── .gitignore

```

Now moving onto configuration files of the project.

### Overlay file explanation

Overlay file is important when the application required alternative pins or configuration than the default provided by the `nRF Connect SDK` or `Zephyr`. In this case, the pin controls for `I2C1` pheripheral  is redirected to pin `SDA - P1.02` and `SCL - P1.03` where the sensor is connected.

Later, sensor binding is configured, which stores sensors information such as I2C Addresses, and clock frequency of I2C.

```
&pinctrl {
	i2c1_default_alt: i2c1_default_alt {
		group2 {
			psels = <NRF_PSEL(TWIM_SDA, 1, 02)>, <NRF_PSEL(TWIM_SCL, 1, 03)>;
			bias-pull-up;
		};
	};

	i2c1_default_sleep: i2c1_default_sleep {
		group3 {
			psels = <NRF_PSEL(TWIM_SDA, 1, 02)>, <NRF_PSEL(TWIM_SCL, 1, 03)>;
			low-power-enable;
		};
	};
};


&i2c1 {
	status = "okay";
	compatible = "nordic,nrf-twim";
	pinctrl-0 = <&i2c1_default_alt>;
	pinctrl-1 = <&i2c1_default_sleep>;
	pinctrl-names = "default", "sleep";
	clock-frequency = < I2C_BITRATE_STANDARD >;
    lis2dtw12: lis2dtw12@19 {
        compatible = "i2c-device";
        status = "okay"; // https://devzone.nordicsemi.com/f/nordic-q-a/107801/usage-of-interrupts-from-i2c-device
		reg = <0x19>; // https://community.st.com/t5/mems-sensors/dts-configuration-for-lis2dw12/td-p/148824
		label = "LIS2DTW12";
	};
};
```

### prj.conf 

This file enables user to include modules to compile and be used inside the programme. For example, to use `I2C` these two configuration has to be enabled :

```
CONFIG_I2C=y
CONFIG_NRFX_TWIM1=y
```
to print float numbers inside console :
```
CONFIG_STDOUT_CONSOLE=y
CONFIG_CBPRINTF_FP_SUPPORT=y

```
to stop the device to reboot and reboot on fatal errors :
```
CONFIG_REBOOT=n
CONFIG_RESET_ON_FATAL_ERROR=n
```


> [!NOTE]  
> In case of custom board, add these config codes, Custom board uses NORA-B106 nRF5340 Module, which doesn't include a LOW SPEED external oscillator. The custom board also doesn't have a VCOM port so RTT viewer is used. So, simply select RTT viewer instead of VCOM1.

```
# clock config  custom board
CONFIG_CLOCK_CONTROL_NRF_K32SRC_RC=y
CONFIG_CLOCK_CONTROL_NRF_K32SRC_XTAL=n


# custom board logger config
CONFIG_LOG=y
CONFIG_CONSOLE=y
CONFIG_RTT_CONSOLE=y
CONFIG_USE_SEGGER_RTT=y
CONFIG_UART_CONSOLE=n
```


### CMakeLists.txt

This file tells `CMake` which files has to be to added in compilation. In case of any files other than `main.c` , it has to be added by calling `target_sources(app PRIVATE src/main.c src/lis2dtw12.c)` where it includes the path of files to be included.


```
cmake_minimum_required(VERSION 3.20.0)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})

project(lis2dtw12_v1)

# Add application source files
target_sources(app PRIVATE src/main.c src/lis2dtw12.c)
```


### lis2dtw12.c and lis2dtw12.h
Now moving onto the code itself, Inside sts40.h, important imports are added here.

| Import | Purpose |
| :--- | :--- |
| **`kernel.h`** | Core of RTOS, provides threading, scheduling, delays (`k_sleep`, `k_msleep`), and synchronization primitives (semaphores, mutexes). |
| **`device.h`** | Device driver model. Provides the generic `struct device` and the `device_get_binding()` function to get a device handle at runtime. |
| **`devicetree.h`** | Provides macros to access and read nodes (like sensors) and their properties from the Device Tree at compile time (e.g., `DT_NODELABEL`, `DT_PROP`). |
| **`i2c.h`** | I2C protocol API. Provides functions for I2C transactions (`i2c_read`, `i2c_write`, `i2c_write_read`). |



```
#ifndef LIS2DTW12_H
#define LIS2DTW12_H

#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/i2c.h>

#define LIS2DTW12_I2C DT_NODELABEL(lis2dtw12)

int lis2dtw12_init(void);
int lis2dtw12_read_who_am_i(uint8_t *who_am_i);
int lis2dtw12_read_temperature(float *temperature);
int lis2dtw12_read_acceleration(float *x, float *y, float *z);

#endif /* LIS2DTW12_H */
```

This line of code gets the node lable from device tree `#define I2C1_NODE DT_NODELABEL(lis2dtw12)` , this node label is described in `.overlay` file .


### main.c

In the `main.c` file first `lis2dtw12.h` and `kernel.h` is imported
```
#include <zephyr/kernel.h>
#include "lis2dtw12.h"
```

Inside main() , i2c bus is initialized, the initialization is done inside lis2dtw12.c , this also sets the Filter and ODR value for the sensor.
```
if (lis2dtw12_init() != 0) {
	printk("Failed to initialize LIS2DTW12 sensor!\n");
	return -1;
}
```

and finally periodically reading the `Accelerometer` value
```
if (lis2dtw12_read_acceleration(&x, &y, &z) == 0) {
	printk("Acceleration X: %.2f mg, Y: %.2f mg, Z: %.2f mg\n", x, y, z);
} else {
	printk("Failed to read acceleration\n");
}
```

Refer to the `lis2dtw12.c` file for sensor codes and setup.
