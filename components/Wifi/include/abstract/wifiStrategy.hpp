#ifndef WIFI_STRATEGY_HPP
#define WIFI_STRATEGY_HPP

#include <string>

class WifiStrategy {
    public:
        virtual ~WifiStrategy() = default;
        virtual void configure(void* config) = 0;
        virtual void start() = 0;
        virtual void stop() = 0;
};

#endif // !WIFIMODE_STRATEGY_HPP
