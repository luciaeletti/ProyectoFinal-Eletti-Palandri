idf_build_get_property(target IDF_TARGET)

# Always compiled source files
set(srcs
    "src/interface.c"
    "src/acquire.c"
    "src/analyzer.c"
    "src/connection.c"
    )

# Always included headers
set(includes "inc")

idf_component_register(SRCS ${srcs}
                       INCLUDE_DIRS ${includes}
                       REQUIRES drivers esp_wifi esp_event nvs_flash mqtt esp_common esp_system wpa_supplicant
                       REQUIRES data
                        )