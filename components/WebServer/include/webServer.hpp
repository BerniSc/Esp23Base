#ifndef WEB_SERVER_HPP
#define WEB_SERVER_HPP

#include "wifiManager.hpp"
#include "../privateInclude/abstract/webserverStrategy.hpp"
#include "esp_http_server.h"

class WebServer {
    private:
        WifiManager& wifiManager;
        std::unique_ptr<WebServerStrategy> serverStrategy;

    public:
        WebServer(WifiManager& manager);
        ~WebServer();

        void start();
        void stop();

        void registerEndpoint(const std::string& uri, WebServerStrategy::HttpMethod method, WebServerStrategy::EndpointCallback callback);
};

#endif // !WEB_SERVER_HPP
