#include <Arduino.h>
#include "GSM/GSM.h"
#include "sensorTemperature/sensorTemperature.h"

const uint8_t RELE_START_HEAT_PIN = 8;
const uint8_t RELE_STOP_HEAT_PIN = 9;

const String IlyaNumber = "+79503422666";
const String AntonNumber = "+79202979541";

GSM gsm(10,11);
sensorTemperature sensor(6);

void setup() {
    sensor.setup();
    gsm.setup();
    pinMode(8, OUTPUT);

    if (gsm.onReady()) {
    gsm.sendSMS(AntonNumber,"Ready to start"); 
    gsm.sendSMS(IlyaNumber,"Ready to start"); 
    }
}
void loop() {
    if (gsm.onResponse()) {
        String number = gsm.getNumber();
        String result = gsm.getResponse();
        Serial.println(result);
        Serial.println(gsm.getResponse());

        if (result.equals("startHeat") && (number.equals(IlyaNumber) || number.equals(AntonNumber))) {
            digitalWrite(RELE_START_HEAT_PIN,HIGH);
            delay(500);
            digitalWrite(RELE_START_HEAT_PIN, LOW);
            gsm.sendSMS(number,"Pressed startHeat");
        }
        if (result.equals("getTemperature") && (number.equals(IlyaNumber) || number.equals(AntonNumber))) {
            digitalWrite(12, HIGH);
            String temp = (String) sensor.getTemperature();
            gsm.sendSMS(number,"Temperature: " + temp);
        }
        if (result.equals("stopHeat") && (number.equals(IlyaNumber) || number.equals(AntonNumber))) {
            digitalWrite(RELE_STOP_HEAT_PIN,!digitalRead(RELE_STOP_HEAT_PIN));
            gsm.sendSMS(number,"Pressed stopHeat");
        }
    }
}