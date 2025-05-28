#include "../include/gpio.hpp"

#include "driver/gpio.h"
#include "soc/gpio_num.h"

#include "esp_log.h"

#define TAG "GPIO"

GPIO::GPIO(gpio_num_t pin, GPIOMode mode) : pinNumber(pin), mode(mode), lastWrite(false) {
    setMode(mode);
}

void GPIO::setMode(GPIOMode newMode) {
    mode = newMode;

    // Zero-initialize the gpio_config_t structure
    gpio_config_t io_conf = {};
    io_conf.pin_bit_mask = (1ULL << pinNumber);         // Configure the specific pin

    // Set the mode based on the GPIOMode enum
    switch(newMode) {
        case GPIOMode::INPUT:
            io_conf.mode = GPIO_MODE_INPUT;
            io_conf.pull_up_en = GPIO_PULLUP_DISABLE;
            io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
            break;
        case GPIOMode::OUTPUT:
            io_conf.mode = GPIO_MODE_OUTPUT;
            io_conf.pull_up_en = GPIO_PULLUP_DISABLE;
            io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
            gpio_set_pull_mode(pinNumber, GPIO_FLOATING);       // Explicitly clear remainders of old config, Get optimized away by ESP
            break;
        case GPIOMode::INPUT_PULLUP:
            io_conf.mode = GPIO_MODE_INPUT;
            io_conf.pull_up_en = GPIO_PULLUP_ENABLE;
            io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
            break;
        case GPIOMode::INPUT_PULLDOWN:
            io_conf.mode = GPIO_MODE_INPUT;
            io_conf.pull_up_en = GPIO_PULLUP_DISABLE;
            io_conf.pull_down_en = GPIO_PULLDOWN_ENABLE;
            break;
        case GPIOMode::ANALOG:
            // Analog mode is not directly supported in ESP-IDF GPIO
            return;
    }

    // Disable interrupts for now
    io_conf.intr_type = GPIO_INTR_DISABLE;

    // Apply the configuration
    gpio_config(&io_conf);
}

void GPIO::digitalWrite(bool value) {
    gpio_set_level(pinNumber, value ? 1 : 0);
    lastWrite = value;
    ESP_LOGI(TAG, "Set GPIO %d to %s", pinNumber, value ? "HIGH" : "LOW");
}

bool GPIO::digitalRead() {
    // Read returns 0 if Pin is not configured for either In or InOut
    if(mode == GPIOMode::OUTPUT)
        return lastWrite;
    return gpio_get_level(pinNumber);
}

// Register Func-Callback to Pin Number
void GPIO::attachInterrupt(GPIOInterruptType type, std::function<void()> callback) {
    gpio_int_type_t interruptType;
    switch(type) {
        case GPIOInterruptType::RISING: interruptType = GPIO_INTR_POSEDGE; break;
        case GPIOInterruptType::FALLING: interruptType = GPIO_INTR_NEGEDGE; break;
        case GPIOInterruptType::CHANGE: interruptType = GPIO_INTR_ANYEDGE; break;
        default: interruptType = GPIO_INTR_POSEDGE; break;
    }
    GPIOManager::registerInterrupt(pinNumber, interruptType, callback);
}

void GPIO::detachInterrupt() {
    GPIOManager::deregisterInterrupt(pinNumber);
}
