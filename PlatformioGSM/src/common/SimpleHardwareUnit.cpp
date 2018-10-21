#include "SimpleHardwareUnit.h"

SimpleHardwareUnit::SimpleHardwareUnit() {};
SimpleHardwareUnit::SimpleHardwareUnit(uint8_t pin, uint8_t mode) {
    setPinMode(pin, mode);
};
SimpleHardwareUnit::~SimpleHardwareUnit() {};

void SimpleHardwareUnit::run() {
    HardwareUnit::run();
};

void SimpleHardwareUnit::setup() {
    HardwareUnit::setup();
    pinMode(unitPin, unitPinMode);
}

void SimpleHardwareUnit::setPinMode(uint8_t pin, uint8_t mode)
{
    unitPinMode = mode;
    unitPin = pin;
};