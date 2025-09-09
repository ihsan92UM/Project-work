# Programming Guide - UART_I2C_nRF5340

## Requirements

#### Hardware
* [nRF5340 DK](https://www.nordicsemi.com/Products/Development-hardware/nRF5340-DK)
* nRF5340 based Custom Board v1 or v2

#### Software
* UART_I2C_nRF5340

> [!IMPORTANT]  
> UART Portion of the code is collected from [Nordic Dev Academy](https://academy.nordicsemi.com/courses/nrf-connect-sdk-fundamentals/lessons/lesson-4-serial-communication-uart/)


# Sensor Connection
> [!NOTE]  
> This setup is only for nRF5340Dk , Custom boards already contains LIS2DTW12 Accelerometer and STS40 temperature sensor according to this setup.


*   Connect `3V3` to `VDD`
*   Connect `GND` to `GND`
*   Connect `SCL` to `P1.03`
*   Connect `SDA` to `P1.02`

> [!NOTE]  
> UART Connection follows the default connection of the board on pin `P0.20 - TX` , `P0.22 - RX`. It can be also viewed in `Device tree visual editor` tab. 

# Programming 
Import the ```sts40+lis2dtw12_v1``` folder into workspace by clicking ```add folder``` from the ```Explorer``` tab.
Once imported the folder, goto ```nRF Connect``` tab and click on ```Add Build Configuration``` under application menu. Once ```build``` is done, ```flash``` the programme and open ```VCOM1``` port and view the result.

# Debugging

Alternatively, Debugging can be done, simply turn on the Debugging optimization flag in the ```build configuration``` inside ```Opitimization level``` option.

# Code Configurations

The folder is structured as , `src` folder contains the `main.c` file and sensor files `lis2dtw12.c`, `uart_rx.c`and `sts40.c` . This modularization helps reducing code congestion in `main.c` file. Include files are in `include` folder. This change also has been added to `CMakeLists.txt` file.

```
lis2dtw12_v1/
├── include/
│   ├── lis2dtw12.h
│   ├── uart_rx.h
│   └── sts40.h
├── src/
│   ├── main.c
│   ├── list2dtw12.c
│   ├── uart_rx.c
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

# Programming Guide - UART_I2C_nRF5340

## Requirements

#### Hardware
* [nRF5340 DK](https://www.nordicsemi.com/Products/Development-hardware/nRF5340-DK)
* nRF5340 based Custom Board v1 or v2

#### Software
* UART_I2C_nRF5340

> [!IMPORTANT]  
> UART Portion of the code is collected from [Nordic Dev Academy](https://academy.nordicsemi.com/courses/nrf-connect-sdk-fundamentals/lessons/lesson-4-serial-communication-uart/)


# Sensor Connection
> [!NOTE]  
> This setup is only for nRF5340Dk , Custom boards already contains LIS2DTW12 Accelerometer and STS40 temperature sensor according to this setup.


*   Connect `3V3` to `VDD`
*   Connect `GND` to `GND`
*   Connect `SCL` to `P1.03`
*   Connect `SDA` to `P1.02`

> [!NOTE]  
> UART Connection follows the default connection of the board on pin `P0.20 - TX` , `P0.22 - RX`. It can be also viewed in `Device tree visual editor` tab. 

# Programming 
Import the ```sts40+lis2dtw12_v1``` folder into workspace by clicking ```add folder``` from the ```Explorer``` tab.
Once imported the folder, goto ```nRF Connect``` tab and click on ```Add Build Configuration``` under application menu. Once ```build``` is done, ```flash``` the programme and open ```VCOM1``` port and view the result.

# Debugging

Alternatively, Debugging can be done, simply turn on the Debugging optimization flag in the ```build configuration``` inside ```Opitimization level``` option.

# Code Configurations

The folder is structured as , `src` folder contains the `main.c` file and sensor files `lis2dtw12.c`, `uart_rx.c`and `sts40.c` . This modularization helps reducing code congestion in `main.c` file. Include files are in `include` folder. This change also has been added to `CMakeLists.txt` file.

```
lis2dtw12_v1/
├── include/
│   ├── lis2dtw12.h
│   ├── uart_rx.h
│   └── sts40.h
├── src/
│   ├── main.c
│   ├── list2dtw12.c
│   ├── uart_rx.c
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

