#ifndef WEB_SERVER_STRATEGY_HPP
#define WEB_SERVER_STRATEGY_HPP

#include <functional>
#include <string>

class WebServerStrategy {
    public:
        enum class HttpMethod {
            GET,
            POST
        };

        using EndpointCallback = std::function<std::string(const std::string&)>;

        virtual ~WebServerStrategy() = default;

        virtual void start() = 0;
        virtual void stop() = 0;
        virtual void registerEndpoint(const std::string& uri, HttpMethod method, EndpointCallback callback) = 0;
};

#endif // !WEB_SERVER_STRATEGY_HPP
