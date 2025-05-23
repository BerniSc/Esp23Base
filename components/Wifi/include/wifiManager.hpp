#ifndef WIFI_MANAGER_HPP
#define WIFI_MANAGER_HPP

#include <memory>
#include <iostream>

#include "abstract/wifiStrategy.hpp"
#include "stationStrategy.hpp"
#include "apStrategy.hpp"

#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"

#include "esp_err.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event.h"


class WifiManager {
    public:
        enum class Mode {
            Station,
            AccessPoint
        };
    private:
        std::unique_ptr<WifiStrategy> mode;
        bool stationAvailable;
        bool apAvailable;

        void setStation();
        void setAP();

    public:
        WifiManager(const Mode& initialMode);

        void start();
        void stop();

        void switchToStation();
        void switchToAP();
};

#endif // !WIFI_MANAGER_HPP
