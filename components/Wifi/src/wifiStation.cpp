#include "../privateInclude/stationStrategy.hpp"

#include <cstring>

#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "esp_system.h"
#include "freertos/event_groups.h"

#define TAG "WifiStation"

#define WIFI_CONNECTED_BIT BIT0
#define WIFI_FAIL_BIT BIT1

void WifiStation::configure(void* config) {
    StationConfig* stationConfig = static_cast<StationConfig*>(config);
    assert(stationConfig != nullptr);           // Ensure config is correct type

    ssid = stationConfig->ssid;
    password = stationConfig->password;
    maxRetry = stationConfig->maxRetry;
}

void WifiStation::start() {
    ESP_LOGI(TAG, "Starting Station Mode...");

    wifiEventGroup = xEventGroupCreateStatic(&staticEventGroup);

    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    esp_netif_create_default_wifi_sta();

    wifi_init_config_t wifiInitConfig = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&wifiInitConfig));

    // Prevent hardware bug (3.11 -> ECO and Workarounds for Bugs)
    ESP_ERROR_CHECK(esp_wifi_set_ps(WIFI_PS_NONE));

    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &_eventHandler, this, nullptr));
    ESP_ERROR_CHECK(esp_event_handler_instance_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &_eventHandler, this, nullptr));

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));

    wifi_config_t wifiConfig = {};
    strncpy(reinterpret_cast<char*>(wifiConfig.sta.ssid), ssid.c_str(), sizeof(wifiConfig.sta.ssid));
    strncpy(reinterpret_cast<char*>(wifiConfig.sta.password), password.c_str(), sizeof(wifiConfig.sta.password));
    wifiConfig.sta.threshold.authmode = WIFI_AUTH_WPA2_PSK;
    wifiConfig.sta.pmf_cfg.capable = true;
    wifiConfig.sta.pmf_cfg.required = false;

    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifiConfig));
    ESP_ERROR_CHECK(esp_wifi_start());

    ESP_LOGI(TAG, "Station Mode started successfully!");

    // Wait for connection
    ESP_LOGI(TAG, "Waiting for connection...");
    EventBits_t eventBits = xEventGroupWaitBits(wifiEventGroup, WIFI_CONNECTED_BIT | WIFI_FAIL_BIT, pdFALSE, pdFALSE, portMAX_DELAY);

    if(eventBits & WIFI_CONNECTED_BIT) {
        ESP_LOGI(TAG, "Connected to AP SSID: %s", ssid.c_str());
    } else if(eventBits & WIFI_FAIL_BIT) {
        ESP_LOGE(TAG, "Failed to connect to AP SSID: %s", ssid.c_str());
        ESP_ERROR_CHECK(ESP_FAIL);
    } else {
        ESP_LOGE(TAG, "Unexpected event");
        ESP_ERROR_CHECK(ESP_FAIL);
    }
}

void WifiStation::stop() {
    ESP_LOGI(TAG, "Stopping Station Mode...");
    ESP_ERROR_CHECK(esp_wifi_stop());
    ESP_ERROR_CHECK(esp_event_loop_delete_default());
    if(wifiEventGroup) {
        vEventGroupDelete(wifiEventGroup);
        wifiEventGroup = nullptr;
    }
    ESP_LOGI(TAG, "Station Mode stopped.");
}


void WifiStation::_eventHandler(void* arg, esp_event_base_t eventBase, int32_t eventId, void* eventData) {
    WifiStation* wifiStation = static_cast<WifiStation*>(arg);

    static int connRetryNum = 0;

    if(eventBase == WIFI_EVENT && eventId == WIFI_EVENT_STA_START) {
        esp_wifi_connect();
    } else if(eventBase == WIFI_EVENT && eventId == WIFI_EVENT_STA_DISCONNECTED) {
        if(connRetryNum < wifiStation->maxRetry) {
            esp_wifi_connect();
            connRetryNum++;
            ESP_LOGI(TAG, "Retrying to connect to the AP...");
        } else {
            xEventGroupSetBits(wifiStation->wifiEventGroup, WIFI_FAIL_BIT);
        }
        ESP_LOGI(TAG, "Failed to connect to the AP.");
    } else if(eventBase == IP_EVENT && eventId == IP_EVENT_STA_GOT_IP) {
        auto* event = static_cast<ip_event_got_ip_t*>(eventData);
        ESP_LOGI(TAG, "Got IP: " IPSTR, IP2STR(&event->ip_info.ip));
        connRetryNum = 0;
        xEventGroupSetBits(wifiStation->wifiEventGroup, WIFI_CONNECTED_BIT);
    }
}
