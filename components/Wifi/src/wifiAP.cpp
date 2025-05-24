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
    maxCon = stationConfig->maxCon;
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
    wifiConfig.ap.max_connection = maxCon; 
    // Open network if no password is provided
    wifiConfig.ap.authmode = password.empty() ? WIFI_AUTH_OPEN : WIFI_AUTH_WPA2_PSK;

    // Register event handlers
    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT, ESP_EVENT_ANY_ID, _eventHandler, this, nullptr));

    ESP_LOGI(TAG, "Access Point Mode started successfully!");
    ESP_LOGI(TAG, "SSID: %s, Password: %s, Channel: %d", ssid.c_str(), password.c_str(), channel);

    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_AP, &wifiConfig));
    ESP_ERROR_CHECK(esp_wifi_start());

}

void WifiAccesspoint::stop() {
    ESP_LOGI(TAG, "Stopping Access Point Mode...");
    ESP_ERROR_CHECK(esp_wifi_stop());
    ESP_ERROR_CHECK(esp_event_loop_delete_default());
    ESP_LOGI(TAG, "Access Point Mode stopped.");
}

void WifiAccesspoint::_eventHandler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data) {
    ESP_LOGD(TAG, "WifiAP Event was detected...");
    if(event_base == WIFI_EVENT) {
        switch(event_id) {
            case WIFI_EVENT_AP_START:
                ESP_LOGI(TAG, "Access Point started.");
                break;
            case WIFI_EVENT_AP_STOP:
                ESP_LOGI(TAG, "Access Point stopped.");
                break;
            case WIFI_EVENT_AP_STACONNECTED:
                {
                    auto* event = static_cast<wifi_event_ap_staconnected_t*>(event_data);
                    ESP_LOGI(TAG, "Client connected: MAC=%02x:%02x:%02x:%02x:%02x:%02x, AID=%d",
                             event->mac[0], event->mac[1], event->mac[2],
                             event->mac[3], event->mac[4], event->mac[5],
                             event->aid);
                    break;
                }
            case WIFI_EVENT_AP_STADISCONNECTED:
                {
                    auto* event = static_cast<wifi_event_ap_stadisconnected_t*>(event_data);
                    ESP_LOGI(TAG, "Client disconnected: MAC=%02x:%02x:%02x:%02x:%02x:%02x, AID=%d",
                             event->mac[0], event->mac[1], event->mac[2],
                             event->mac[3], event->mac[4], event->mac[5],
                             event->aid);
                    break;
                }
            default:
                break;
        }
    }
}
