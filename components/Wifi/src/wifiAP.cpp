#include "../include/apStrategy.hpp"

#include <cstring>

#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "esp_system.h"

#define TAG "WifiAccesspoint"

void WifiAccesspoint::configure(void* config) {
    auto* stationConfig = static_cast<AccessPointConfig*>(config);
    assert(stationConfig != nullptr); // Ensure config is correct type

    ssid = stationConfig->ssid;
    password = stationConfig->password;
    channel = stationConfig->channel;
}

void WifiAccesspoint::start() {
    ESP_LOGI(TAG, "Starting Access Point Mode...");

    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    esp_netif_create_default_wifi_ap();

    wifi_init_config_t wifiInitConfig = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&wifiInitConfig));

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_AP));

    wifi_config_t wifiConfig = {};
    strncpy(reinterpret_cast<char*>(wifiConfig.ap.ssid), ssid.c_str(), sizeof(wifiConfig.ap.ssid));
    strncpy(reinterpret_cast<char*>(wifiConfig.ap.password), password.c_str(), sizeof(wifiConfig.ap.password));
    wifiConfig.ap.ssid_len = ssid.length();
    wifiConfig.ap.channel = channel;
    wifiConfig.ap.max_connection = 4; // Maximum number of clients
    wifiConfig.ap.authmode = WIFI_AUTH_WPA2_PSK;

    if (password.empty()) {
        wifiConfig.ap.authmode = WIFI_AUTH_OPEN; // Open network if no password is provided
    }

    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_AP, &wifiConfig));
    ESP_ERROR_CHECK(esp_wifi_start());

    ESP_LOGI(TAG, "Access Point Mode started successfully!");
    ESP_LOGI(TAG, "SSID: %s, Password: %s, Channel: %d", ssid.c_str(), password.c_str(), channel);
}

void WifiAccesspoint::stop() {
    ESP_LOGI(TAG, "Stopping Access Point Mode...");
    ESP_ERROR_CHECK(esp_wifi_stop());
    ESP_ERROR_CHECK(esp_event_loop_delete_default());
    ESP_LOGI(TAG, "Access Point Mode stopped.");
}
