# Programming Guide - I2C Scanner nRF5340DK

## Requirements

#### Hardware
* [nRF5340 DK](https://www.nordicsemi.com/Products/Development-hardware/nRF5340-DK)


#### Software
* i2c_scanner  

> [!WARNING]  
> This code is adapted from [Golioth](https://blog.golioth.io/how-to-use-zephyr-shell-for-interactive-prototyping-with-i2c-sensors/)

# Sensor Connection
> [!NOTE]  
> This setup is only for nRF5340Dk , it is not tested with Custom Boards.


*   Connect `3V3` to `VDD`
*   Connect `GND` to `GND`
*   Connect `SCL` to `P1.03`
*   Connect `SDA` to `P1.02`

# Programming 
Import the ```i2c_scanner``` folder into workspace by clicking ```add folder``` from the ```Explorer``` tab

once imported the folder, goto ```nRF Connect``` tab and click on ```Add Build Configuration``` under application menu.

Once ```build``` is done, ```flash``` the programme and open ```VCOM1``` port.

In the Console it will open a interactive  UART shell where user can input commands as bellow:

type in `i2c -h` to open i2c reated commands
```
uart:~$ i2c -h
```

if typed `device list`, it will open all the available pheripherals
```
uart:~$ device list
```

finally using `i2c scan i2c@9000 ` command at available pheripheral will show all the `sensors address`.
```
uart:~$ i2c scan i2c@9000
```

![results](/Graphics/i2c_scanner.png)


