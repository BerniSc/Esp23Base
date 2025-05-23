idf_component_register(SRCS "main.cpp"
                    INCLUDE_DIRS "."
                    REQUIRES nvs_flash Logger Wifi)

# set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
