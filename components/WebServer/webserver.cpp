#include "webServer.hpp"
#include "esp_log.h"
#include "esp_netif.h"

#include "webServer.hpp"
#include "privateInclude/httpStrategy.hpp"

WebServer::WebServer(WifiManager& manager) : wifiManager(manager), serverStrategy(nullptr) {}

WebServer::~WebServer() {
    stop();
}

void WebServer::start() {
    if(!serverStrategy)
        serverStrategy = std::make_unique<HttpServerStrategy>();
    serverStrategy->start();
}

void WebServer::stop() {
    if(serverStrategy)
        serverStrategy->stop();
}

void WebServer::registerEndpoint(const std::string& uri, WebServerStrategy::HttpMethod method, WebServerStrategy::EndpointCallback callback) {
    if(serverStrategy)
        serverStrategy->registerEndpoint(uri, method, callback);
}
