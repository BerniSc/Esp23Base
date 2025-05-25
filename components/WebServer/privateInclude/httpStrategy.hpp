#ifndef HTTP_STRATEGY_HPP
#define HTTP_STRATEGY_HPP

#include "esp_http_server.h"

#include "abstract/webserverStrategy.hpp"
#include <map>

class HttpServerStrategy : public WebServerStrategy {
    private:
        httpd_handle_t serverHandle;

        struct Endpoint {
            HttpMethod method;
            EndpointCallback callback;
        };

        std::map<std::string, Endpoint> endpoints;

        static esp_err_t handleRequest(httpd_req_t* req);

    public:
        HttpServerStrategy();
        ~HttpServerStrategy();

        void start() override;
        void stop() override;
        void registerEndpoint(const std::string& uri, HttpMethod method, EndpointCallback callback) override;
};

#endif // !HTTP_STRATEGY_HPP
