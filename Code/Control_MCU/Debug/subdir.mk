################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Control_MCU.c \
../DCMotor.c \
../EX_EEPROM.c \
../TWI.c \
../Timer.c \
../UART.c 

OBJS += \
./Control_MCU.o \
./DCMotor.o \
./EX_EEPROM.o \
./TWI.o \
./Timer.o \
./UART.o 

C_DEPS += \
./Control_MCU.d \
./DCMotor.d \
./EX_EEPROM.d \
./TWI.d \
./Timer.d \
./UART.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -g2 -gstabs -O0 -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega16 -DF_CPU=8000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


