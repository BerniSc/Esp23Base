#ifndef AP_STRATEGY_HPP
#define AP_STRATEGY_HPP

#include "abstract/wifiStrategy.hpp"
#include "esp_event_base.h"

struct AccessPointConfig {
    std::string ssid;
    std::string password;
    int channel;
    int maxCon;
};

class WifiAccesspoint : public WifiStrategy {
    private:
        std::string ssid;
        std::string password;
        int channel;
        int maxCon;

        static void _eventHandler(void* arg, esp_event_base_t eventBase, int32_t eventId, void* eventData);

    public:
        void configure(void* config) override;
        void start() override;
        void stop() override;
};

#endif // !AP_STRATEGY_HPP
