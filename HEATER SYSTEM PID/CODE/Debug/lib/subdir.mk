################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../lib/CLCD_I2C.c \
../lib/application.c \
../lib/max6675.c \
../lib/menu.c \
../lib/pid.c 

OBJS += \
./lib/CLCD_I2C.o \
./lib/application.o \
./lib/max6675.o \
./lib/menu.o \
./lib/pid.o 

C_DEPS += \
./lib/CLCD_I2C.d \
./lib/application.d \
./lib/max6675.d \
./lib/menu.d \
./lib/pid.d 


# Each subdirectory must supply rules for building sources it contributes
lib/%.o lib/%.su lib/%.cyclo: ../lib/%.c lib/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I"F:/PID-HEADER/HEATER SYSTEM PID/CODE/lib" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-lib

clean-lib:
	-$(RM) ./lib/CLCD_I2C.cyclo ./lib/CLCD_I2C.d ./lib/CLCD_I2C.o ./lib/CLCD_I2C.su ./lib/application.cyclo ./lib/application.d ./lib/application.o ./lib/application.su ./lib/max6675.cyclo ./lib/max6675.d ./lib/max6675.o ./lib/max6675.su ./lib/menu.cyclo ./lib/menu.d ./lib/menu.o ./lib/menu.su ./lib/pid.cyclo ./lib/pid.d ./lib/pid.o ./lib/pid.su

.PHONY: clean-lib

