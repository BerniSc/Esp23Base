#ifndef GPIO_MANAGER_HPP
#define GPIO_MANAGER_HPP

#include <map>
#include <functional>
#include <memory>

#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"

#include "driver/gpio.h"
#include "soc/gpio_num.h"

#include "../privateInclude/gpioTypes.hpp"

class GPIO;

class GPIOManager {
    private:
        static std::map<gpio_num_t, std::function<void()>> callbackRegistry;
        static QueueHandle_t interruptQueue;
        static bool isInitialized;

        static void IRAM_ATTR isrHandler(void* arg);
        static void interruptTask(void* arg);

        GPIOManager() = default; // Private constructor for Singleton

    public:
        static void initialize();
        static std::shared_ptr<GPIO> createGPIO(int pin, GPIOMode mode);
        static void registerInterrupt(gpio_num_t pin, gpio_int_type_t type, std::function<void()> callback);
        static void deregisterInterrupt(gpio_num_t pin);

        static void dumpPin(int pin);
        static void dumpPins();
};

#endif // !GPIO_MANAGER_HPP
