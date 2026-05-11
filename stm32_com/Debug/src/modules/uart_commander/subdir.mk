################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (14.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/modules/uart_commander/uart_commander.c 

OBJS += \
./src/modules/uart_commander/uart_commander.o 

C_DEPS += \
./src/modules/uart_commander/uart_commander.d 


# Each subdirectory must supply rules for building sources it contributes
src/modules/uart_commander/%.o src/modules/uart_commander/%.su src/modules/uart_commander/%.cyclo: ../src/modules/uart_commander/%.c src/modules/uart_commander/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I"D:/thuviec/New folder (2)/STM32F1/stm32_com/src/modules/led_blink" -I"D:/thuviec/New folder (2)/STM32F1/stm32_com/src/protocol/device_protocol" -I"D:/thuviec/New folder (2)/STM32F1/stm32_com/src/modules/uart_commander" -I"D:/thuviec/New folder (2)/STM32F1/stm32_com/src/system" -I"D:/thuviec/New folder (2)/STM32F1/stm32_com/platforms/hal" -I"D:/thuviec/New folder (2)/STM32F1/stm32_com/platforms/stm32" -I"D:/thuviec/New folder (2)/STM32F1/stm32_com/src/drivers/led" -I"D:/thuviec/New folder (2)/STM32F1/stm32_com/src/drivers/uart" -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-src-2f-modules-2f-uart_commander

clean-src-2f-modules-2f-uart_commander:
	-$(RM) ./src/modules/uart_commander/uart_commander.cyclo ./src/modules/uart_commander/uart_commander.d ./src/modules/uart_commander/uart_commander.o ./src/modules/uart_commander/uart_commander.su

.PHONY: clean-src-2f-modules-2f-uart_commander

