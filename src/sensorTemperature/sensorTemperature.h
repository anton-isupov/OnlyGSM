#ifndef Abstract_Temperature_Sensor_H
#define Abstract_Temperature_Sensor_H

#include "../common/SimpleHardwareUnit.h"
#include "OneWire.h"

class sensorTemperature : public SimpleHardwareUnit {
    public:
        sensorTemperature(uint8_t pin);
        ~sensorTemperature();
        void setup();
        float getTemperature();
    private:
        OneWire *temp;
};

#endif