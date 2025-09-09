# Programming Guide - STS40_v1

## Requirements

#### Hardware
* [nRF5340 DK](https://www.nordicsemi.com/Products/Development-hardware/nRF5340-DK)
* Custom Board v1 or v2

#### Software
* sts40_v1

# Sensor Connection
> [!NOTE]  
> This setup is only for nRF5340Dk , Custom boards already contains STS40 sensor.


*   Connect `3V3` to `VDD`
*   Connect `GND` to `GND`
*   Connect `SCL` to `P1.03`
*   Connect `SDA` to `P1.02`

![temperature sensor](/Graphics/Temp_Sensor.png)



# Programming 
Import the ```sts40_v1``` folder into workspace by clicking ```add folder``` from the ```Explorer``` tab

![Select folder](/Graphics/select_folder.png)

once imported the folder, goto ```nRF Connect``` tab and click on ```Add Build Configuration``` under application menu.

![Build config](/Graphics/folder_build_config.png)

Once ```build``` is done, ```flash``` the programme and open ```VCOM1``` port and view the result.

![result](/Graphics/folder_results.png)


# Debugging

Alternatively, Debugging can be done, simply turn on the Debugging optimization flag in the ```build configuration``` inside ```Opitimization level``` option.

![Debugging](/Graphics/debug_view.png)


# Code Explanation

The folder is structured as , `src` folder contains the `main.c` file and configuration file `sts40.c` and `.h` for sensor configuration. This modularization helps reducing code congestion in `main.c` file.

```
sts40_v1/
├── src/
│   ├── main.c
│   ├── sts40.c
│   └── sts40.h
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
    sts40: sts40@46 {    /* STS40 I2C address is 0x46 */
        compatible = "i2c-device";
        status = "okay";
        reg = <0x46>;
        label = "STS40";

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

### CMakeLists.txt

This file tells `CMake` which files has to be to added in compilation. In case of any files other than `main.c` , it has to be added by calling `target_sources(app PRIVATE  src/sts40.c)` where it includes the path of files to be included.


```
cmake_minimum_required(VERSION 3.20.0)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})

project(sts40_v1)

target_sources(app PRIVATE src/main.c)
target_sources(app PRIVATE src/main.c src/sts40.c)
```


### sts40.c and sts40.h
Now moving onto the code itself, Inside sts40.h, important imports are added here.

| Import | Purpose |
| :--- | :--- |
| **`kernel.h`** | Core of RTOS, provides threading, scheduling, delays (`k_sleep`, `k_msleep`), and synchronization primitives (semaphores, mutexes). |
| **`device.h`** | Device driver model. Provides the generic `struct device` and the `device_get_binding()` function to get a device handle at runtime. |
| **`devicetree.h`** | Provides macros to access and read nodes (like sensors) and their properties from the Device Tree at compile time (e.g., `DT_NODELABEL`, `DT_PROP`). |
| **`i2c.h`** | I2C protocol API. Provides functions for I2C transactions (`i2c_read`, `i2c_write`, `i2c_write_read`). |



```
#ifndef STS40_H
#define STS40_H

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/i2c.h>

#define I2C1_NODE DT_NODELABEL(sts40)

/* Structure to hold the STS40 sensor data */
struct sts40_data {
    const struct i2c_dt_spec i2c;
};

/* Function prototypes */
int sts40_init(const struct sts40_data *dev);
int sts40_read_temperature(const struct sts40_data *dev, float *temperature);

#endif /* STS40_H */
```

This line of code gets the node lable from device tree `#define I2C1_NODE DT_NODELABEL(sts40)` , this node label is described in `.overlay` file .


### main.c

In the `main.c` file first `sts40.h` and `kernel.h` is imported
```
#include <zephyr/kernel.h>
#include "sts40.h"
```

later a struct to refer to the sensor node 
```
static const struct sts40_data sts40_dev = {
    .i2c = I2C_DT_SPEC_GET(I2C1_NODE),
};
```

Inside main() , i2c bus is initialized, the initialization is done inside sts40.c
```
if (sts40_init(&sts40_dev) != 0) {
	return -1;
}
```

and finally periodically reading the `temperature` value
```
if (sts40_read_temperature(&sts40_dev, &temperature) == 0) {
	printk("Temperature: %.2f °C\n", temperature);
}
```


