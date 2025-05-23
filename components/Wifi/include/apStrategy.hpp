#ifndef AP_STRATEGY_HPP
#define AP_STRATEGY_HPP

#include "abstract/wifiStrategy.hpp"

struct AccessPointConfig {
    std::string ssid;
    std::string password;
    int channel;
};

class WifiAccesspoint : public WifiStrategy {
    private:
        std::string ssid;
        std::string password;
        int channel;

    public:
        void configure(void* config) override;
        void start() override;
        void stop() override;
};

#endif // !AP_STRATEGY_HPP
