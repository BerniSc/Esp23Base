#include "../privateInclude/httpStrategy.hpp"

#include "esp_log.h"

#define TAG "HttpServer"

HttpServerStrategy::HttpServerStrategy() : serverHandle(nullptr) {}

HttpServerStrategy::~HttpServerStrategy() {
    stop();
}

void HttpServerStrategy::start() {
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    if(httpd_start(&serverHandle, &config) == ESP_OK)
        ESP_LOGI(TAG, "HTTP server started");
    else
        ESP_LOGE(TAG, "Failed to start HTTP server");
}

void HttpServerStrategy::stop() {
    if(serverHandle) {
        httpd_stop(serverHandle);
        serverHandle = nullptr;
        ESP_LOGI(TAG, "HTTP server stopped");
    }
}

void HttpServerStrategy::registerEndpoint(const std::string& uri, HttpMethod method, EndpointCallback callback) {
    if(!serverHandle) {
        ESP_LOGE(TAG, "Server not started. Cannot register endpoint.");
        return;
    }

    endpoints[uri] = { method, callback };

    httpd_uri_t endpointConfig = {};
    endpointConfig.uri = uri.c_str();
    endpointConfig.method = (method == HttpMethod::GET) ? HTTP_GET : HTTP_POST;
    endpointConfig.handler = handleRequest;
    endpointConfig.user_ctx = this;

    if(httpd_register_uri_handler(serverHandle, &endpointConfig) == ESP_OK)
        ESP_LOGI(TAG, "Endpoint registered: %s [%s]", uri.c_str(), (method == HttpMethod::GET) ? "GET" : "POST");
    else
        ESP_LOGE(TAG, "Failed to register endpoint: %s", uri.c_str());
}

esp_err_t HttpServerStrategy::handleRequest(httpd_req_t* req) {
    HttpServerStrategy* self = static_cast<HttpServerStrategy*>(req->user_ctx);
    std::string uri(req->uri);

    auto it = self->endpoints.find(uri);
    if(it != self->endpoints.end() && ((it->second.method == HttpMethod::GET && req->method == HTTP_GET) ||
                                       (it->second.method == HttpMethod::POST && req->method == HTTP_POST))) {
        std::string response = it->second.callback(uri);
        httpd_resp_send(req, response.c_str(), response.size());
        return ESP_OK;
    }

    httpd_resp_send_404(req);
    return ESP_FAIL;
}
