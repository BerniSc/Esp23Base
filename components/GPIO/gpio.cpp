#include <functional>
#include <stdexcept>
#include <string>

// Follow this:
// https://github.com/espressif/esp-idf/blob/master/examples/peripherals/gpio/generic_gpio/main/gpio_example_main.c
//
// Would have to be using a GPIO Manager which registers IS like this:     // gpio_install_isr_service(ESP_INTR_FLAG_DEFAULT);
// Also create just one Task as it is essentially a thread and we dont want that many. This one thread polls the Queue
// Create a callback-Registry Map that holds our interrupts and therefore lightningfast executes registered functions
// config can be used to init multiple, therefore maybe try variadic template. Single changes can be done with that: gpio_set_intr_type  
//
// Still love the GPIO approach, but maybe abstract them internally? Oh boi

class GPIO {
    public:
        enum class Mode {
            INPUT,
            OUTPUT,
            INPUT_PULLUP,
            INPUT_PULLDOWN,
            ANALOG
        };

        enum class InterruptType {
            RISING,
            FALLING,
            CHANGE
        };

    private:
        // GPIO Pins -> Maybe Alternative Labels?
        int pinNumber;
        Mode mode;
        // Want callback here in case of Interrupt
        std::function<void()> interruptCallback;

    public:
        GPIO(int pin, Mode mode) : pinNumber(pin), mode(mode) {
            // Maybe no implicit interrupt? Rethink
            enable();
        }

        ~GPIO() {
            disable();
        }

        // Configure as IO?
        void enable() {
        }

        // Disable
        void disable() {
        }

        // Change Mode IO? 
        void setMode(Mode newMode) {
            mode = newMode;
        }

        // Out logic one
        void digitalWrite(bool value) {
            if(mode != Mode::OUTPUT)
                throw std::runtime_error("Pin is not configured as OUTPUT");
        }

        // In logic one. Maybe rethink us forcing to use internall PUPDs? Could add external
        bool digitalRead() {
            if(mode != Mode::INPUT && mode != Mode::INPUT_PULLUP && mode != Mode::INPUT_PULLDOWN)
                throw std::runtime_error("Pin is not configured as INPUT");
            return false; // Placeholder
        }

        // Analog Write
        void analogWrite(int value) {
            if(mode != Mode::ANALOG)
                throw std::runtime_error("Pin is not configured as ANALOG");
        }

        // Analog Read
        int analogRead() {
            if(mode != Mode::ANALOG)
                throw std::runtime_error("Pin is not configured as ANALOG");
            return 0; // Placeholder
        }

        // Attach an Interrupt. Maybe think if we should add other relevant info here as well? Can we make the interrupt get the triggering flank as well as param? Idk
        void attachInterrupt(InterruptType type, std::function<void()> callback) {
            if(!supportsInterrupt())
                throw std::runtime_error("Pin does not support interrupts");
            interruptCallback = callback;
        }

        // Detach IR
        void detachInterrupt() {
            interruptCallback = nullptr;
        }

        // Maybe check against some registry if we support it? Or is there a builtin? This would be fantastic
        bool supportsInterrupt() const {
            return true; // Placeholder
        }

        // AAAAAND The handerl. Possibly have to change this. Either store GPIOs in registry/map or store the callbacks in superclass. Lets see
        void handleInterrupt() {
            if(interruptCallback)
                interruptCallback();
        }
};
