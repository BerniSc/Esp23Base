idf_component_register(SRCS "src/gpio.cpp" "src/gpioManager.cpp"
                    INCLUDE_DIRS "include"
                    REQUIRES driver)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
