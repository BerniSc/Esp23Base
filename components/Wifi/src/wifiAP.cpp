#include "../include/apStrategy.hpp"

void WifiAccesspoint::configure(void* config) {
    auto* stationConfig = static_cast<AccessPointConfig*>(config);
    assert(stationConfig != nullptr); // Ensure config is correct type

    ssid = stationConfig->ssid;
    password = stationConfig->password;
    channel = stationConfig->channel;
}

void WifiAccesspoint::start() {

}

void WifiAccesspoint::stop() {

}
