################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (14.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/drivers/led/drv_led.c 

OBJS += \
./src/drivers/led/drv_led.o 

C_DEPS += \
./src/drivers/led/drv_led.d 


# Each subdirectory must supply rules for building sources it contributes
src/drivers/led/%.o src/drivers/led/%.su src/drivers/led/%.cyclo: ../src/drivers/led/%.c src/drivers/led/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I"D:/thuviec/New folder (2)/STM32F1/stm32_com/src/modules/led_blink" -I"D:/thuviec/New folder (2)/STM32F1/stm32_com/src/protocol/device_protocol" -I"D:/thuviec/New folder (2)/STM32F1/stm32_com/src/modules/uart_commander" -I"D:/thuviec/New folder (2)/STM32F1/stm32_com/src/system" -I"D:/thuviec/New folder (2)/STM32F1/stm32_com/platforms/hal" -I"D:/thuviec/New folder (2)/STM32F1/stm32_com/platforms/stm32" -I"D:/thuviec/New folder (2)/STM32F1/stm32_com/src/drivers/led" -I"D:/thuviec/New folder (2)/STM32F1/stm32_com/src/drivers/uart" -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-src-2f-drivers-2f-led

clean-src-2f-drivers-2f-led:
	-$(RM) ./src/drivers/led/drv_led.cyclo ./src/drivers/led/drv_led.d ./src/drivers/led/drv_led.o ./src/drivers/led/drv_led.su

.PHONY: clean-src-2f-drivers-2f-led

