#include "esp_log.h"
#include "esp_system.h"
#include "logger.hpp"
#include "webServer.hpp"
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

    auto wifiMangager = WifiManager(WifiManager::Mode::AccessPoint);
    wifiMangager.start();
    auto webServer = WebServer(wifiMangager);
    webServer.start();
    webServer.registerEndpoint("/", WebServerStrategy::HttpMethod::GET, [](const std::string& uri) -> std::string {
        return R"HTML(
            <html>
            <head><title>ESP32 WebServer</title>

        <script>
            function toggleGPIO() {
                fetch('/toggle', { method: 'POST' })
                    .then(response => response.text())
                    .then(data => {
                        console.log(data); // Log the server response
                        alert(data); // Show the response in an alert
                    })
                    .catch(error => console.error('Error:', error));
            }
        </script>
            </head>
            <body>
                <button onclick="toggleGPIO()">Toggle GPIO</button>
                <a href="/test">GoTo Test</a>
            </body>
            </html>
        )HTML";
    });
    webServer.registerEndpoint("/test", WebServerStrategy::HttpMethod::GET, [](const std::string& uri) -> std::string {
        ESP_LOGI("MAIN", "Recieved Get for /test");
        return R"(
            <html>
            <head><title>ESP32 WebServer</title></head>
            <body>
                <h1>Welcome to ESP32 WebServer</h1>
                <a href="/">GoBack</a>
            </body>
            </html>
        )";
    });
    webServer.registerEndpoint("/toggle", WebServerStrategy::HttpMethod::POST, [](const std::string& uri) -> std::string {
        ESP_LOGI("MAIN", "RECIEVED POST For T");
        return "TEST";
    });

    for(int i = 1000; i >= 10; i--)
        vTaskDelay(1000 / portTICK_PERIOD_MS);

    lg.log("DOne");
    esp_restart();
}
