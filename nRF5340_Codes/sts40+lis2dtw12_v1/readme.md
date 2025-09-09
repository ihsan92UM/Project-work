# Programming Guide - sts40+lis2dtw12_v1

## Requirements

#### Hardware
* [nRF5340 DK](https://www.nordicsemi.com/Products/Development-hardware/nRF5340-DK)
* nRF5340 based Custom Board v1 or v2

#### Software
* sts40+lis2dtw12_v1

# Sensor Connection
> [!NOTE]  
> This setup is only for nRF5340Dk , Custom boards already contains LIS2DTW12 Accelerometer and STS40 temperature sensor according to this setup.


*   Connect `3V3` to `VDD`
*   Connect `GND` to `GND`
*   Connect `SCL` to `P1.03`
*   Connect `SDA` to `P1.02`


#### LIS2DTW12 Accelerometer sensor
![Accelerometer](/Graphics/IMU_Sensor.png)

#### STS40 Temperature sensor
![temperature ](/Graphics/Temp_Sensor.png)





# Programming 
Import the ```sts40+lis2dtw12_v1``` folder into workspace by clicking ```add folder``` from the ```Explorer``` tab.
Once imported the folder, goto ```nRF Connect``` tab and click on ```Add Build Configuration``` under application menu. Once ```build``` is done, ```flash``` the programme and open ```VCOM1``` port and view the result.

# Debugging

Alternatively, Debugging can be done, simply turn on the Debugging optimization flag in the ```build configuration``` inside ```Opitimization level``` option.

# Code Configurations

The folder is structured as , `src` folder contains the `main.c` file and sensor files `lis2dtw12.c` and `sts40.c` . This modularization helps reducing code congestion in `main.c` file. Include files are in `include` folder. This change also has been added to `CMakeLists.txt` file.

```
lis2dtw12_v1/
├── include/
│   ├── lis2dtw12.h
│   └── sts40.h
├── src/
│   ├── main.c
│   ├── list2dtw12.c
│   └── sts40.c
├── CMakeLists.txt
├── nrf5340dk_nrf5340_cpuapp.overlay
├── prj.conf
└── .gitignore

```



### prj.conf 

Add these extra codes to make it run on Custom boards.

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

Add this line `include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)` to include the `include` folder to compile.

```
cmake_minimum_required(VERSION 3.20.0)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})

project(sts40+lis2dtw12_v1)

# Include the "include" directory for header files
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# Include source files
target_sources(app PRIVATE src/main.c 
                           src/sts40.c 
                           src/lis2dtw12.c)
```

This code combines two individual sensors code. Refer to the individual sensor codes for detailed explanation.