################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (14.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/system/app_main.c 

OBJS += \
./src/system/app_main.o 

C_DEPS += \
./src/system/app_main.d 


# Each subdirectory must supply rules for building sources it contributes
src/system/%.o src/system/%.su src/system/%.cyclo: ../src/system/%.c src/system/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I"D:/thuviec/New folder (2)/STM32F1/stm32_com/src/modules/led_blink" -I"D:/thuviec/New folder (2)/STM32F1/stm32_com/src/protocol/device_protocol" -I"D:/thuviec/New folder (2)/STM32F1/stm32_com/src/modules/uart_commander" -I"D:/thuviec/New folder (2)/STM32F1/stm32_com/src/system" -I"D:/thuviec/New folder (2)/STM32F1/stm32_com/platforms/hal" -I"D:/thuviec/New folder (2)/STM32F1/stm32_com/platforms/stm32" -I"D:/thuviec/New folder (2)/STM32F1/stm32_com/src/drivers/led" -I"D:/thuviec/New folder (2)/STM32F1/stm32_com/src/drivers/uart" -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-src-2f-system

clean-src-2f-system:
	-$(RM) ./src/system/app_main.cyclo ./src/system/app_main.d ./src/system/app_main.o ./src/system/app_main.su

.PHONY: clean-src-2f-system

