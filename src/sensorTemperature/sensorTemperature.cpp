#include "sensorTemperature.h"

sensorTemperature::sensorTemperature(uint8_t pin) {
unitPin = pin;
}

sensorTemperature::~sensorTemperature() {}

void sensorTemperature::setup() {
    this->temp = new OneWire(unitPin);
}

float sensorTemperature::getTemperature() {
 byte data[2];
  this->temp->reset(); // Начинаем взаимодействие со сброса всех предыдущих команд и параметров
  this->temp->write(0xCC); // Даем датчику DS18b20 команду пропустить поиск по адресу. В нашем случае только одно устрйоство 
  this->temp->write(0x44); // Даем датчику DS18b20 команду измерить температуру. Само значение температуры мы еще не получаем - датчик его положит во внутреннюю память
   
  delay(1000); // Микросхема измеряет температуру, а мы ждем.  
   
  this->temp->reset(); // Теперь готовимся получить значение измеренной температуры
  this->temp->write(0xCC); 
  this->temp->write(0xBE); // Просим передать нам значение регистров со значением температуры
 
  // Получаем и считываем ответ
  data[0] = this->temp->read(); // Читаем младший байт значения температуры
  data[1] = this->temp->read(); // А теперь старший
 
  // Формируем итоговое значение: 
  //    - сперва "склеиваем" значение, 
  //    - затем умножаем его на коэффициент, соответсвующий разрешающей способности (для 12 бит по умолчанию - это 0,0625)
  float temperature =  ((data[1] << 8) | data[0]) * 0.0625;
  return temperature;
}