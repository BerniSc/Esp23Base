#ifndef GPIO_HPP
#define GPIO_HPP

#include "gpioManager.hpp"

class GPIO {
    private:
        gpio_num_t pinNumber;
        GPIOMode mode;

        bool lastWrite;

        GPIO(gpio_num_t pin, GPIOMode mode); // Private constructor

        friend class GPIOManager;            // GPIOManager to access private constructor -> Factory

    public:
        void setMode(GPIOMode newMode);
        void digitalWrite(bool value);
        bool digitalRead();
        void attachInterrupt(GPIOInterruptType type, std::function<void()> callback);
        void detachInterrupt();
};

#endif // !GPIO_HPP
