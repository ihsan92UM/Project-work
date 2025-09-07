################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../USB\ DEVICE/Core/Src/usbd_conf.c \
../USB\ DEVICE/Core/Src/usbd_core.c \
../USB\ DEVICE/Core/Src/usbd_ctlreq.c \
../USB\ DEVICE/Core/Src/usbd_desc.c \
../USB\ DEVICE/Core/Src/usbd_ioreq.c 

OBJS += \
./USB\ DEVICE/Core/Src/usbd_conf.o \
./USB\ DEVICE/Core/Src/usbd_core.o \
./USB\ DEVICE/Core/Src/usbd_ctlreq.o \
./USB\ DEVICE/Core/Src/usbd_desc.o \
./USB\ DEVICE/Core/Src/usbd_ioreq.o 

C_DEPS += \
./USB\ DEVICE/Core/Src/usbd_conf.d \
./USB\ DEVICE/Core/Src/usbd_core.d \
./USB\ DEVICE/Core/Src/usbd_ctlreq.d \
./USB\ DEVICE/Core/Src/usbd_desc.d \
./USB\ DEVICE/Core/Src/usbd_ioreq.d 


# Each subdirectory must supply rules for building sources it contributes
USB\ DEVICE/Core/Src/usbd_conf.o: ../USB\ DEVICE/Core/Src/usbd_conf.c USB\ DEVICE/Core/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m33 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32U545xx -c -I../Core/Inc -I../Drivers/STM32U5xx_HAL_Driver/Inc -I../Drivers/STM32U5xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32U5xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/Ihsan/STM32CubeIDE/workspace_1.17.0/cb_v1_6freq_usb/USB DEVICE/Class/CDC/Inc" -I"C:/Users/Ihsan/STM32CubeIDE/workspace_1.17.0/cb_v1_6freq_usb/USB DEVICE/Core/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"USB DEVICE/Core/Src/usbd_conf.d" -MT"$@" --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
USB\ DEVICE/Core/Src/usbd_core.o: ../USB\ DEVICE/Core/Src/usbd_core.c USB\ DEVICE/Core/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m33 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32U545xx -c -I../Core/Inc -I../Drivers/STM32U5xx_HAL_Driver/Inc -I../Drivers/STM32U5xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32U5xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/Ihsan/STM32CubeIDE/workspace_1.17.0/cb_v1_6freq_usb/USB DEVICE/Class/CDC/Inc" -I"C:/Users/Ihsan/STM32CubeIDE/workspace_1.17.0/cb_v1_6freq_usb/USB DEVICE/Core/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"USB DEVICE/Core/Src/usbd_core.d" -MT"$@" --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
USB\ DEVICE/Core/Src/usbd_ctlreq.o: ../USB\ DEVICE/Core/Src/usbd_ctlreq.c USB\ DEVICE/Core/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m33 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32U545xx -c -I../Core/Inc -I../Drivers/STM32U5xx_HAL_Driver/Inc -I../Drivers/STM32U5xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32U5xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/Ihsan/STM32CubeIDE/workspace_1.17.0/cb_v1_6freq_usb/USB DEVICE/Class/CDC/Inc" -I"C:/Users/Ihsan/STM32CubeIDE/workspace_1.17.0/cb_v1_6freq_usb/USB DEVICE/Core/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"USB DEVICE/Core/Src/usbd_ctlreq.d" -MT"$@" --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
USB\ DEVICE/Core/Src/usbd_desc.o: ../USB\ DEVICE/Core/Src/usbd_desc.c USB\ DEVICE/Core/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m33 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32U545xx -c -I../Core/Inc -I../Drivers/STM32U5xx_HAL_Driver/Inc -I../Drivers/STM32U5xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32U5xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/Ihsan/STM32CubeIDE/workspace_1.17.0/cb_v1_6freq_usb/USB DEVICE/Class/CDC/Inc" -I"C:/Users/Ihsan/STM32CubeIDE/workspace_1.17.0/cb_v1_6freq_usb/USB DEVICE/Core/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"USB DEVICE/Core/Src/usbd_desc.d" -MT"$@" --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
USB\ DEVICE/Core/Src/usbd_ioreq.o: ../USB\ DEVICE/Core/Src/usbd_ioreq.c USB\ DEVICE/Core/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m33 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32U545xx -c -I../Core/Inc -I../Drivers/STM32U5xx_HAL_Driver/Inc -I../Drivers/STM32U5xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32U5xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/Ihsan/STM32CubeIDE/workspace_1.17.0/cb_v1_6freq_usb/USB DEVICE/Class/CDC/Inc" -I"C:/Users/Ihsan/STM32CubeIDE/workspace_1.17.0/cb_v1_6freq_usb/USB DEVICE/Core/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"USB DEVICE/Core/Src/usbd_ioreq.d" -MT"$@" --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-USB-20-DEVICE-2f-Core-2f-Src

clean-USB-20-DEVICE-2f-Core-2f-Src:
	-$(RM) ./USB\ DEVICE/Core/Src/usbd_conf.cyclo ./USB\ DEVICE/Core/Src/usbd_conf.d ./USB\ DEVICE/Core/Src/usbd_conf.o ./USB\ DEVICE/Core/Src/usbd_conf.su ./USB\ DEVICE/Core/Src/usbd_core.cyclo ./USB\ DEVICE/Core/Src/usbd_core.d ./USB\ DEVICE/Core/Src/usbd_core.o ./USB\ DEVICE/Core/Src/usbd_core.su ./USB\ DEVICE/Core/Src/usbd_ctlreq.cyclo ./USB\ DEVICE/Core/Src/usbd_ctlreq.d ./USB\ DEVICE/Core/Src/usbd_ctlreq.o ./USB\ DEVICE/Core/Src/usbd_ctlreq.su ./USB\ DEVICE/Core/Src/usbd_desc.cyclo ./USB\ DEVICE/Core/Src/usbd_desc.d ./USB\ DEVICE/Core/Src/usbd_desc.o ./USB\ DEVICE/Core/Src/usbd_desc.su ./USB\ DEVICE/Core/Src/usbd_ioreq.cyclo ./USB\ DEVICE/Core/Src/usbd_ioreq.d ./USB\ DEVICE/Core/Src/usbd_ioreq.o ./USB\ DEVICE/Core/Src/usbd_ioreq.su

.PHONY: clean-USB-20-DEVICE-2f-Core-2f-Src

