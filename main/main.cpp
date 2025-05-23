#include "logger.hpp"
#include "wifiManager.hpp"

#include "freertos/FreeRTOS.h"

#include "esp_err.h"
#include "nvs_flash.h"

extern "C" void app_main(void) {
    //Initialize NVS
    esp_err_t ret = nvs_flash_init();
    if(ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }

    Logger lg;
    lg.log("AHSDJK");

    for(int i = 10; i >= 10; i--)
        vTaskDelay(1000 / portTICK_PERIOD_MS);

    auto wifiMangager = WifiManager(WifiManager::Mode::Station);
    wifiMangager.start();

    for(int i = 100; i >= 10; i--)
        vTaskDelay(1000 / portTICK_PERIOD_MS);

    lg.log("DOne");
    
}
