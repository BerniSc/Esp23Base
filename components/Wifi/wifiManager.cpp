#include "include/wifiManager.hpp"
#include "esp_log.h"

#ifdef CONFIG_WIFI_STATION_MODE
#define WIFI_SSID CONFIG_WIFI_SSID
#define WIFI_PASSWORD CONFIG_WIFI_PASSWORD
#define WIFI_MAX_RETRY CONFIG_WIFI_MAX_RETRY
#else 
#define WIFI_SSID ""
#define WIFI_PASSWORD ""
#define WIFI_MAX_RETRY 0
#endif    

#ifdef CONFIG_WIFI_ACCESS_POINT
#define WIFI_AP_SSID CONFIG_WIFI_AP_SSID
#define WIFI_AP_PASSWORD CONFIG_WIFI_AP_PASSWORD
#define WIFI_AP_CHANNEL CONFIG_WIFI_AP_CHANNEL
#define WIFI_APP_MAX_CON CONFIG_WIFI_AP_MAX_CON
#else 
#define WIFI_AP_SSID ""
#define WIFI_AP_PASSWORD ""
#define WIFI_AP_CHANNEL 0
#define WIFI_APP_MAX_CON 0
#endif    

#define TAG "WifiManager"

WifiManager::WifiManager(const Mode& initialMode) : stationAvailable(false), apAvailable(false) {
#ifdef CONFIG_WIFI_STATION_MODE
    stationAvailable = true;
#endif
#ifdef CONFIG_WIFI_ACCESS_POINT
    apAvailable = true;
#endif
    ESP_LOGI(TAG, "Config says we can have: %s %s", (stationAvailable ? "Station there" : ""), (apAvailable ? "Accesspoint there" : ""));

    // Set initial mode based on constructor arg
    switch(initialMode) {
        case Mode::Station:
            if(!stationAvailable)
                throw std::runtime_error("Station mode is not available (disabled in Kconfig).");
            setStation();
            break;
        case Mode::AccessPoint:
            if(!apAvailable)
                throw std::runtime_error("Access Point mode is not available (disabled in Kconfig).");
            setAP();
            break;
        default:
            throw std::invalid_argument("Invalid initial mode.");
    }
}


void WifiManager::setStation() {
    auto stationMode = std::make_unique<WifiStation>();
    StationConfig config = {
        WIFI_SSID,
        WIFI_PASSWORD,
        WIFI_MAX_RETRY
    };
    ESP_LOGD(TAG, "connecting %s with %s", WIFI_SSID, WIFI_PASSWORD);
    stationMode->configure(&config);
    mode = std::move(stationMode);
}

void WifiManager::setAP() {
    auto apMode = std::make_unique<WifiAccesspoint>();
    AccessPointConfig config = {
        WIFI_AP_SSID,
        WIFI_AP_PASSWORD,
        WIFI_AP_CHANNEL,
        WIFI_APP_MAX_CON
    };
    ESP_LOGD(TAG, "setting up %s with %s", WIFI_AP_SSID, WIFI_AP_PASSWORD);
    apMode->configure(&config);
    mode = std::move(apMode);
}

void WifiManager::start() {
    if(mode)
        mode->start();
    else
        ESP_LOGE(TAG, "No WiFi mode is set. Please set a mode before starting.");
}

void WifiManager::stop() {
    if(mode)
        mode->stop();
    else
        ESP_LOGE(TAG, "No WiFi mode is set. Nothing to stop.");
}


void WifiManager::switchToStation() {
    if(!stationAvailable) {
        ESP_LOGE(TAG, "Station mode is not available (disabled in Kconfig).");
        return;
    }

    if(mode) {
        ESP_LOGI(TAG, "Stopping current mode...");
        mode->stop();
    }

    setStation();
    if(mode) {
        ESP_LOGI(TAG, "Starting Station mode...");
        mode->start();
    }
}

void WifiManager::switchToAP() {
    if(!apAvailable) {
        ESP_LOGE(TAG, "Access Point mode is not available (disabled in Kconfig).");
        return;
    }

    if(mode) {
        ESP_LOGI(TAG, "Stopping current mode...");
        mode->stop();
    }

    setAP();
    if(mode) {
        ESP_LOGI(TAG, "Starting Access Point mode...");
        mode->start();
    }
}
