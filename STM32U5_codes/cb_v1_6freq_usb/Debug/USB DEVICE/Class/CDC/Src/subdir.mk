################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../USB\ DEVICE/Class/CDC/Src/usbd_cdc.c \
../USB\ DEVICE/Class/CDC/Src/usbd_cdc_if.c 

OBJS += \
./USB\ DEVICE/Class/CDC/Src/usbd_cdc.o \
./USB\ DEVICE/Class/CDC/Src/usbd_cdc_if.o 

C_DEPS += \
./USB\ DEVICE/Class/CDC/Src/usbd_cdc.d \
./USB\ DEVICE/Class/CDC/Src/usbd_cdc_if.d 


# Each subdirectory must supply rules for building sources it contributes
USB\ DEVICE/Class/CDC/Src/usbd_cdc.o: ../USB\ DEVICE/Class/CDC/Src/usbd_cdc.c USB\ DEVICE/Class/CDC/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m33 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32U545xx -c -I../Core/Inc -I../Drivers/STM32U5xx_HAL_Driver/Inc -I../Drivers/STM32U5xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32U5xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/Ihsan/STM32CubeIDE/workspace_1.17.0/cb_v1_6freq_usb/USB DEVICE/Class/CDC/Inc" -I"C:/Users/Ihsan/STM32CubeIDE/workspace_1.17.0/cb_v1_6freq_usb/USB DEVICE/Core/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"USB DEVICE/Class/CDC/Src/usbd_cdc.d" -MT"$@" --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
USB\ DEVICE/Class/CDC/Src/usbd_cdc_if.o: ../USB\ DEVICE/Class/CDC/Src/usbd_cdc_if.c USB\ DEVICE/Class/CDC/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m33 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32U545xx -c -I../Core/Inc -I../Drivers/STM32U5xx_HAL_Driver/Inc -I../Drivers/STM32U5xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32U5xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/Ihsan/STM32CubeIDE/workspace_1.17.0/cb_v1_6freq_usb/USB DEVICE/Class/CDC/Inc" -I"C:/Users/Ihsan/STM32CubeIDE/workspace_1.17.0/cb_v1_6freq_usb/USB DEVICE/Core/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"USB DEVICE/Class/CDC/Src/usbd_cdc_if.d" -MT"$@" --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-USB-20-DEVICE-2f-Class-2f-CDC-2f-Src

clean-USB-20-DEVICE-2f-Class-2f-CDC-2f-Src:
	-$(RM) ./USB\ DEVICE/Class/CDC/Src/usbd_cdc.cyclo ./USB\ DEVICE/Class/CDC/Src/usbd_cdc.d ./USB\ DEVICE/Class/CDC/Src/usbd_cdc.o ./USB\ DEVICE/Class/CDC/Src/usbd_cdc.su ./USB\ DEVICE/Class/CDC/Src/usbd_cdc_if.cyclo ./USB\ DEVICE/Class/CDC/Src/usbd_cdc_if.d ./USB\ DEVICE/Class/CDC/Src/usbd_cdc_if.o ./USB\ DEVICE/Class/CDC/Src/usbd_cdc_if.su

.PHONY: clean-USB-20-DEVICE-2f-Class-2f-CDC-2f-Src

