#ifndef STATION_STRATEGY_HPP
#define STATION_STRATEGY_HPP

#include "abstract/wifiStrategy.hpp"
#include "esp_event.h"

#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"

struct StationConfig {
    std::string ssid;
    std::string password;
    int maxRetry;
};

class WifiStation : public WifiStrategy {
    private:
        std::string ssid;
        std::string password;
        int maxRetry;

        // Static memory for event group
        StaticEventGroup_t staticEventGroup;
        EventGroupHandle_t wifiEventGroup;

        static void _eventHandler(void* arg, esp_event_base_t eventBase, int32_t eventId, void* eventData);

    public:
        void configure(void* config) override;
        void start() override;
        void stop() override;

};

#endif // !STATION_STRATEGY_HPP
