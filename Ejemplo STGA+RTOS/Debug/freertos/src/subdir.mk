################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../freertos/src/FreeRTOSCommonHooks.c \
../freertos/src/basic_io.c \
../freertos/src/consoleprint.c \
../freertos/src/heap_2.c \
../freertos/src/list.c \
../freertos/src/port.c \
../freertos/src/queue.c \
../freertos/src/tasks.c 

OBJS += \
./freertos/src/FreeRTOSCommonHooks.o \
./freertos/src/basic_io.o \
./freertos/src/consoleprint.o \
./freertos/src/heap_2.o \
./freertos/src/list.o \
./freertos/src/port.o \
./freertos/src/queue.o \
./freertos/src/tasks.o 

C_DEPS += \
./freertos/src/FreeRTOSCommonHooks.d \
./freertos/src/basic_io.d \
./freertos/src/consoleprint.d \
./freertos/src/heap_2.d \
./freertos/src/list.d \
./freertos/src/port.d \
./freertos/src/queue.d \
./freertos/src/tasks.d 


# Each subdirectory must supply rules for building sources it contributes
freertos/src/%.o: ../freertos/src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__USE_LPCOPEN -DNO_BOARD_LIB -D__CODE_RED -DCORE_M3 -D__LPC17XX__ -D__REDLIB__ -I"C:\Users\Prototipado\Documents\LPCXpresso_7.6.2_326\workspace\STGA+RTOS\Data\inc" -I"C:\Users\Prototipado\Documents\LPCXpresso_7.6.2_326\workspace\STGA+RTOS\Modules\inc" -I"C:\Users\Prototipado\Documents\LPCXpresso_7.6.2_326\workspace\STGA+RTOS\Drivers\inc" -I"C:\Users\Prototipado\Documents\LPCXpresso_7.6.2_326\workspace\STGA+RTOS\Board\inc" -I"C:\Users\Prototipado\Documents\LPCXpresso_7.6.2_326\workspace\STGA+RTOS\freertos\inc" -I"C:\Users\Prototipado\Documents\LPCXpresso_7.6.2_326\workspace\Lib_FatFs_SD\inc" -I"C:\Users\Prototipado\Documents\LPCXpresso_7.6.2_326\workspace\LIB_GLCDgui\inc" -I"C:\Users\Prototipado\Documents\LPCXpresso_7.6.2_326\workspace\LIB_GLCDPrimitives\fonts" -I"C:\Users\Prototipado\Documents\LPCXpresso_7.6.2_326\workspace\LIB_GLCDPrimitives\inc" -I"C:\Users\Prototipado\Documents\LPCXpresso_7.6.2_326\workspace\Lib_GLCDLowLevel\inc" -I"C:\Users\Prototipado\Documents\LPCXpresso_7.6.2_326\workspace\lpc_chip_175x_6x\inc" -O0 -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -mcpu=cortex-m3 -mthumb -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


