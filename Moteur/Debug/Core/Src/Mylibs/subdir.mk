################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/Mylibs/pwm.c \
../Core/Src/Mylibs/shell.c 

OBJS += \
./Core/Src/Mylibs/pwm.o \
./Core/Src/Mylibs/shell.o 

C_DEPS += \
./Core/Src/Mylibs/pwm.d \
./Core/Src/Mylibs/shell.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/Mylibs/%.o Core/Src/Mylibs/%.su Core/Src/Mylibs/%.cyclo: ../Core/Src/Mylibs/%.c Core/Src/Mylibs/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G431xx -DUSE_FULL_LL_DRIVER -c -I../Core/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32G4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-Mylibs

clean-Core-2f-Src-2f-Mylibs:
	-$(RM) ./Core/Src/Mylibs/pwm.cyclo ./Core/Src/Mylibs/pwm.d ./Core/Src/Mylibs/pwm.o ./Core/Src/Mylibs/pwm.su ./Core/Src/Mylibs/shell.cyclo ./Core/Src/Mylibs/shell.d ./Core/Src/Mylibs/shell.o ./Core/Src/Mylibs/shell.su

.PHONY: clean-Core-2f-Src-2f-Mylibs

