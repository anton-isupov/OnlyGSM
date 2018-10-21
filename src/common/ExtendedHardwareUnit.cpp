#include "ExtendedHardwareUnit.h"

ExtendedHardwareUnit::ExtendedHardwareUnit(int *pinModesArray) {
    setPinMode(pinModesArray);
};
ExtendedHardwareUnit::~ExtendedHardwareUnit() {};
ExtendedHardwareUnit::ExtendedHardwareUnit() {};
void ExtendedHardwareUnit::run() {
    HardwareUnit::run();
};

void ExtendedHardwareUnit::setup() {
    HardwareUnit::setup();
    // pinMode(pin, pinWorkMode);
}

void ExtendedHardwareUnit::setPinMode(int *pinModesArray)
{
    pinModes = pinModesArray;
};