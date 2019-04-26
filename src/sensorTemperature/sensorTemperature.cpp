#include "sensorTemperature.h"

sensorTemperature::sensorTemperature(uint8_t pin) {
    unitPin = pin;
}

void sensorTemperature::setup() {
    this->temp = new OneWire(unitPin);
}

float sensorTemperature::getTemperature() {
 byte data[2];
  this->temp->reset();
  this->temp->write(0xCC);
  this->temp->write(0x44);
   
  delay(1000);
   
  this->temp->reset();
  this->temp->write(0xCC); 
  this->temp->write(0xBE);
 
  data[0] = this->temp->read();
  data[1] = this->temp->read();
 
  float temperature =  ((data[1] << 8) | data[0]) * 0.0625;
  return temperature;
}