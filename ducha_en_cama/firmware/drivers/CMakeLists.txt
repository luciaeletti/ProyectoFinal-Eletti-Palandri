idf_build_get_property(target IDF_TARGET)

# Always compiled source files
set(srcs
    "microcontroller/src/gpio_mcu.c"
    "microcontroller/src/gpio_fast_out_mcu.c"
    "microcontroller/src/delay_mcu.c"
    "microcontroller/src/timer_mcu.c"
    "microcontroller/src/i2c_mcu.c"
    "microcontroller/src/general.c"
    "devices/src/KeyPad.c"
    "devices/src/LCD_I2C.c"
    "devices/src/ds18b20.c"
    "devices/src/FDC1004.c"
    )

# Always included headers
set(includes "microcontroller/inc"
             "devices/inc")

idf_component_register(SRCS ${srcs}
                       INCLUDE_DIRS ${includes}
                       REQUIRES driver esp_rom)