#include "../inc/BaseConfig.h"

typedef enum {
    P_CHANNEL = 0,
    N_CHANNEL,
    P_TRENCH,
    N_TRENCH
} mosfet_type_t;

class LowPower
{
private:
    uint8_t _pin;
    mosfet_type_t _mosfetType;
    bool _currentState;
public:
    LowPower(uint8_t pin, mosfet_type_t mosfetType = P_CHANNEL) : _pin(pin), _mosfetType(mosfetType) {};

    void init();

    void enable5V();
    void disable5V();
    void toggle5V();
};

void LowPower::init() {
    PRINT_DEBUG_LN("[LOW POWER] Initializing...");
    if (_mosfetType == P_CHANNEL) {
        pinMode(_pin, INPUT_PULLUP);
    }
    else if (_mosfetType == N_CHANNEL)
    {
        #ifdef __AVR__
        pinMode(_pin, OUTPUT);
        digitalWrite(_pin, LOW);
        #else
        pinMode(_pin, INPUT_PULLDOWN);
        #endif
    }
    else {
        PRINT_DEBUG_LN("No, i don't implemented that yet...");
    }
    _currentState = false;
}

void LowPower::enable5V() {
    PRINT_DEBUG_LN("[LOW POWER] 5V on");
    _currentState = true;
    if (_mosfetType == P_CHANNEL) {
        pinMode(_pin, OUTPUT);
        digitalWrite(_pin, LOW);
    }
    else if (_mosfetType == N_CHANNEL)
    {
        pinMode(_pin, OUTPUT);
        digitalWrite(_pin, HIGH);
    }
    else {
        PRINT_DEBUG_LN("No, i don't implemented that yet...");
    }
}

void LowPower::disable5V() {
    _currentState = false;
    if (_mosfetType == P_CHANNEL) {
        pinMode(_pin, INPUT_PULLUP);
        digitalWrite(_pin, HIGH);
    }
    else if (_mosfetType == N_CHANNEL)
    {
        #ifdef __AVR__
        pinMode(_pin, OUTPUT);
        digitalWrite(_pin, LOW);
        #else
        digitalWrite(_pin, LOW);
        pinMode(_pin, INPUT_PULLDOWN);
        #endif
    }
    else {
        PRINT_DEBUG_LN("No, i don't implemented that yet...");
    }
}

void LowPower::toggle5V() {
    if (_currentState) {
        disable5V();
    }
    else {
        enable5V();
    }
}
