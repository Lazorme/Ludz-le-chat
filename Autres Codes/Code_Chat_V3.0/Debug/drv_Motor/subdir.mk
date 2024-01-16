################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../drv_Motor/capteur.c \
../drv_Motor/control.c \
../drv_Motor/pwm.c \
../drv_Motor/sensor.c 

OBJS += \
./drv_Motor/capteur.o \
./drv_Motor/control.o \
./drv_Motor/pwm.o \
./drv_Motor/sensor.o 

C_DEPS += \
./drv_Motor/capteur.d \
./drv_Motor/control.d \
./drv_Motor/pwm.d \
./drv_Motor/sensor.d 


# Each subdirectory must supply rules for building sources it contributes
drv_Motor/%.o drv_Motor/%.su drv_Motor/%.cyclo: ../drv_Motor/%.c drv_Motor/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0plus -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G070xx -c -I../Core/Inc -I../Drivers/STM32G0xx_HAL_Driver/Inc -I../Drivers/STM32G0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32G0xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM0 -I"C:/Users/tomch/Desktop/gitproj/Ludz-le-chat/Code_Chat_V3.0/drv_LIDAR" -I"C:/Users/tomch/Desktop/gitproj/Ludz-le-chat/Code_Chat_V3.0/drv_Motor" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-drv_Motor

clean-drv_Motor:
	-$(RM) ./drv_Motor/capteur.cyclo ./drv_Motor/capteur.d ./drv_Motor/capteur.o ./drv_Motor/capteur.su ./drv_Motor/control.cyclo ./drv_Motor/control.d ./drv_Motor/control.o ./drv_Motor/control.su ./drv_Motor/pwm.cyclo ./drv_Motor/pwm.d ./drv_Motor/pwm.o ./drv_Motor/pwm.su ./drv_Motor/sensor.cyclo ./drv_Motor/sensor.d ./drv_Motor/sensor.o ./drv_Motor/sensor.su

.PHONY: clean-drv_Motor

