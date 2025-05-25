idf_component_register(SRCS "webserver.cpp" "src/httpStrategy.cpp"
                    INCLUDE_DIRS "include"
                    REQUIRES Wifi esp_http_server esp_wifi esp_event)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
