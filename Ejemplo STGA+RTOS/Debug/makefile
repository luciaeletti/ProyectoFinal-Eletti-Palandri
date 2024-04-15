################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include freertos/src/subdir.mk
-include Modules/src/subdir.mk
-include Drivers/src/subdir.mk
-include Data/src/subdir.mk
-include Board/src/subdir.mk
-include Application/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: STGA+RTOS.axf

# Tool invocations
STGA+RTOS.axf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: MCU Linker'
	arm-none-eabi-gcc -nostdlib -L"C:\Users\Prototipado\Documents\LPCXpresso_7.6.2_326\workspace\Lib_FatFs_SD\Debug" -L"C:\Users\Prototipado\Documents\LPCXpresso_7.6.2_326\workspace\LIB_GLCDgui\Debug" -L"C:\Users\Prototipado\Documents\LPCXpresso_7.6.2_326\workspace\LIB_GLCDPrimitives\Debug" -L"C:\Users\Prototipado\Documents\LPCXpresso_7.6.2_326\workspace\Lib_GLCDLowLevel\Debug" -L"C:\Users\Prototipado\Documents\LPCXpresso_7.6.2_326\workspace\lpc_chip_175x_6x\Debug" -Xlinker -Map="STGA+RTOS.map" -Xlinker --gc-sections -mcpu=cortex-m3 -mthumb -T "STGA+RTOS_Debug.ld" -o "STGA+RTOS.axf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '
	$(MAKE) --no-print-directory post-build

# Other Targets
clean:
	-$(RM) $(OBJS)$(C_DEPS)$(EXECUTABLES) STGA+RTOS.axf
	-@echo ' '

post-build:
	-@echo 'Performing post-build steps'
	-arm-none-eabi-size "STGA+RTOS.axf"; # arm-none-eabi-objcopy -v -O binary "STGA+RTOS.axf" "STGA+RTOS.bin" ; # checksum -p LPC1769 -d "STGA+RTOS.bin";
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY: post-build

-include ../makefile.targets
