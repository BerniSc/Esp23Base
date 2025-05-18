#include "logger.hpp"

#include "freertos/FreeRTOS.h"

extern "C" void app_main(void) {
    Logger lg;
    lg.log("AHSDJK");


    for(int i = 10; i >= 10; i--)
        vTaskDelay(1000 / portTICK_PERIOD_MS);

}
