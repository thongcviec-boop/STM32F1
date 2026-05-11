################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (14.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/protocol/device_protocol/device_protocol.c 

OBJS += \
./src/protocol/device_protocol/device_protocol.o 

C_DEPS += \
./src/protocol/device_protocol/device_protocol.d 


# Each subdirectory must supply rules for building sources it contributes
src/protocol/device_protocol/%.o src/protocol/device_protocol/%.su src/protocol/device_protocol/%.cyclo: ../src/protocol/device_protocol/%.c src/protocol/device_protocol/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I"D:/thuviec/New folder (2)/STM32F1/stm32_com/src/modules/led_blink" -I"D:/thuviec/New folder (2)/STM32F1/stm32_com/src/protocol/device_protocol" -I"D:/thuviec/New folder (2)/STM32F1/stm32_com/src/modules/uart_commander" -I"D:/thuviec/New folder (2)/STM32F1/stm32_com/src/system" -I"D:/thuviec/New folder (2)/STM32F1/stm32_com/platforms/hal" -I"D:/thuviec/New folder (2)/STM32F1/stm32_com/platforms/stm32" -I"D:/thuviec/New folder (2)/STM32F1/stm32_com/src/drivers/led" -I"D:/thuviec/New folder (2)/STM32F1/stm32_com/src/drivers/uart" -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-src-2f-protocol-2f-device_protocol

clean-src-2f-protocol-2f-device_protocol:
	-$(RM) ./src/protocol/device_protocol/device_protocol.cyclo ./src/protocol/device_protocol/device_protocol.d ./src/protocol/device_protocol/device_protocol.o ./src/protocol/device_protocol/device_protocol.su

.PHONY: clean-src-2f-protocol-2f-device_protocol

