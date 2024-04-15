################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/src/Common.c \
../Drivers/src/DSD.c \
../Drivers/src/Display.c \
../Drivers/src/GL865.c \
../Drivers/src/Keypad.c \
../Drivers/src/Modem.c \
../Drivers/src/Sensor.c 

OBJS += \
./Drivers/src/Common.o \
./Drivers/src/DSD.o \
./Drivers/src/Display.o \
./Drivers/src/GL865.o \
./Drivers/src/Keypad.o \
./Drivers/src/Modem.o \
./Drivers/src/Sensor.o 

C_DEPS += \
./Drivers/src/Common.d \
./Drivers/src/DSD.d \
./Drivers/src/Display.d \
./Drivers/src/GL865.d \
./Drivers/src/Keypad.d \
./Drivers/src/Modem.d \
./Drivers/src/Sensor.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/src/%.o: ../Drivers/src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__USE_LPCOPEN -DNO_BOARD_LIB -D__CODE_RED -DCORE_M3 -D__LPC17XX__ -D__REDLIB__ -I"C:\Users\Prototipado\Documents\LPCXpresso_7.6.2_326\workspace\STGA+RTOS\Data\inc" -I"C:\Users\Prototipado\Documents\LPCXpresso_7.6.2_326\workspace\STGA+RTOS\Modules\inc" -I"C:\Users\Prototipado\Documents\LPCXpresso_7.6.2_326\workspace\STGA+RTOS\Drivers\inc" -I"C:\Users\Prototipado\Documents\LPCXpresso_7.6.2_326\workspace\STGA+RTOS\Board\inc" -I"C:\Users\Prototipado\Documents\LPCXpresso_7.6.2_326\workspace\STGA+RTOS\freertos\inc" -I"C:\Users\Prototipado\Documents\LPCXpresso_7.6.2_326\workspace\Lib_FatFs_SD\inc" -I"C:\Users\Prototipado\Documents\LPCXpresso_7.6.2_326\workspace\LIB_GLCDgui\inc" -I"C:\Users\Prototipado\Documents\LPCXpresso_7.6.2_326\workspace\LIB_GLCDPrimitives\fonts" -I"C:\Users\Prototipado\Documents\LPCXpresso_7.6.2_326\workspace\LIB_GLCDPrimitives\inc" -I"C:\Users\Prototipado\Documents\LPCXpresso_7.6.2_326\workspace\Lib_GLCDLowLevel\inc" -I"C:\Users\Prototipado\Documents\LPCXpresso_7.6.2_326\workspace\lpc_chip_175x_6x\inc" -O0 -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -mcpu=cortex-m3 -mthumb -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


