#include "../inc/BaseConfig.h"

#if defined(__AVR__)
#include <avr/sleep.h>
#include <avr/wdt.h>
#include <avr/power.h>
#include <avr/interrupt.h>
#elif defined(__arm__)

#else
#error "Processor architecture is not supported."
#endif

typedef enum
{
    P_CHANNEL = 0,
    N_CHANNEL,
    P_TRENCH,
    N_TRENCH,
    NO_MOSFET
} mosfet_type_t;

class LowPower
{
private:
    uint8_t _pin;
    mosfet_type_t _mosfetType;
    bool _currentState;
    unsigned char clockSource = 0;

public:
    LowPower(uint8_t pin, mosfet_type_t mosfetType = P_CHANNEL) : _pin(pin), _mosfetType(mosfetType){};

    void init();

    void shutdownFuntions();
    void powerupFunctions();

    void enable5V();
    void disable5V();
    void toggle5V();
};

void LowPower::init()
{
    PRINT_DEBUG_LN("[LOW POWER] Initializing...");
    if (_mosfetType == P_CHANNEL)
    {
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
    else if (_mosfetType == NO_MOSFET) {
        PRINT_DEBUG_LN("[LOW POWER] Initialized in NO_MOSFET Mode");
    }
    else
    {
        PRINT_DEBUG_LN("No, i don't implemented that yet...");
    }
    _currentState = false;
    clockSource = TCCR2B;
}

void LowPower::enable5V()
{
    PRINT_DEBUG_LN("[LOW POWER] 5V on");
    _currentState = true;
    if (_mosfetType == P_CHANNEL)
    {
        pinMode(_pin, OUTPUT);
        digitalWrite(_pin, LOW);
    }
    else if (_mosfetType == N_CHANNEL)
    {
        pinMode(_pin, OUTPUT);
        digitalWrite(_pin, HIGH);
    }
    else if (_mosfetType == NO_MOSFET) {
        PRINT_DEBUG_LN("[LOW POWER] Initialized in NO_MOSFET Mode");
    }
    else
    {
        PRINT_DEBUG_LN("No, i don't implemented that yet...");
    }
}

void LowPower::disable5V()
{
    _currentState = false;
    if (_mosfetType == P_CHANNEL)
    {
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
    else if (_mosfetType == NO_MOSFET) {
        PRINT_DEBUG_LN("[LOW POWER] Initialized in NO_MOSFET Mode");
    }
    else
    {
        PRINT_DEBUG_LN("No, i don't implemented that yet...");
    }
}

void LowPower::toggle5V()
{
    if (_mosfetType == NO_MOSFET) {
        PRINT_DEBUG_LN("[LOW POWER] Initialized in NO_MOSFET Mode");
        return;
    }
    if (_currentState)
    {
        disable5V();
    }
    else
    {
        enable5V();
    }
}

void LowPower::shutdownFuntions()
{
    // Store current setting
    clockSource = TCCR2B;

    // Remove the clock source to shutdown Timer2
    // TCCR2B &= ~(1 << CS22);
    // TCCR2B &= ~(1 << CS21);
    // TCCR2B &= ~(1 << CS20);

    // power_timer2_disable();

    // ADCSRA &= ~(1 << ADEN);
    // power_adc_disable();

    //power_timer1_disable();
    //power_spi_disable();
    //power_usart0_disable();
    // power_twi_disable();
    if (_mosfetType != NO_MOSFET) {
        disable5V();
    }
}

void LowPower::powerupFunctions()
{
    // power_adc_enable();
    // ADCSRA |= (1 << ADEN);

    // Restore previous setting
    //TCCR2B = clockSource;
    //power_timer2_enable();

    //power_timer1_enable();
    //power_spi_enable();
    //power_usart0_enable();
    // power_twi_enable();
    if (_mosfetType != NO_MOSFET) {
        enable5V();
    }
}
