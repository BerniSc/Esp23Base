idf_component_register(SRCS "gpio.cpp"
                    INCLUDE_DIRS "include"
                    REQUIRES esp_wifi esp_event)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
