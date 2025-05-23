idf_component_register(SRCS "wifiManager.cpp" "src/wifiStation.cpp" "src/wifiAP.cpp"
                    INCLUDE_DIRS "include"
                    REQUIRES esp_wifi esp_event)

# set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
