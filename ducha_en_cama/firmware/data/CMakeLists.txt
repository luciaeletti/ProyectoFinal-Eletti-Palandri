idf_build_get_property(target IDF_TARGET)

# Always compiled source files
set(srcs
    "src/conditions.c"
    "src/alarms.c"
    "src/definitions.c"

    )

# Always included headers
set(includes "inc")

idf_component_register(SRCS ${srcs}
                       INCLUDE_DIRS ${includes}
                       REQUIRES drivers
                       REQUIRES modules)